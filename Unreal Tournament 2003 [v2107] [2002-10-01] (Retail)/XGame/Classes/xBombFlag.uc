//=============================================================================
// xBomb
// This is the bomb.  Someone set us up the bomb.
//=============================================================================
class xBombFlag extends GameObject;

var() String        BombLauncherClassName;
var() class<Weapon> PrevWeaponClass;
var() Pawn          PassTarget;
var() float         ThrowSpeed;
var() float         Elasticity;
var() sound         ImpactSound;
var() float         ThrowerTouchDelay;
var() float         ThrowerTime;
var() vector        InitialDir;
var() float         SeekInterval;
var() transient float         SeekAccum;
var material TeamShader[2];
var byte TeamHue[2];

#exec OBJ LOAD FILE=XGameShaders.utx

function ClearHolder()
{
    if (Holder == None)       
        return;

    Holder.PlayerReplicationInfo.HasFlag = None;
    if ( (Holder.PendingWeapon != None) && Holder.PendingWeapon.IsA('BallLauncher') )
		Holder.PendingWeapon = None;
    if ( (Holder.Weapon != None) && (Holder.Controller != None) && Holder.Weapon.IsA('BallLauncher') )
		Holder.Controller.ClientSwitchToBestWeapon();
    Holder = None;
    HolderPRI = None;
}

// State transitions
function SetHolder(Controller C)
{
    local Class<Weapon> BombLauncherClass;
	local Weapon W;
	local BombingRunSquadAI S;
	local Inventory Inv;
	local Pawn P;

	// update AI before changing states
    if ( Bot(C) != None )
		S = BombingRunSquadAI(Bot(C).Squad);
	else if ( PlayerController(C) != None )
	{
        S = BombingRunSquadAI(UnrealTeamInfo(C.PlayerReplicationInfo.Team).AI.FindHumanSquad());
        if ( S != None && S.SquadLeader != C )
			S = None;		
    }
    if ( S != None )
		S.BombTakenBy(C);

    Super.SetHolder(C);
	Instigator = Holder;

    BombLauncherClass = Class<Weapon>( DynamicLoadObject( BombLauncherClassName , class'Class' ) );
    assert( BombLauncherClass != None );
    PrevWeaponClass = C.Pawn.Weapon.Class;

    if( ClassIsChildOf( PrevWeaponClass, BombLauncherClass ) )
        PrevWeaponClass = None;

	// Make sure the new holds has the BombLauncher
	
    Inv = None;
	P = C.Pawn;
	if (P==None)
		return;

	for( Inv=P.Inventory; Inv!=None; Inv=Inv.Inventory )
	{
		if ( ClassIsChildOf(Inv.Class, BombLauncherClass) )
			break;
	}

	if (Inv==None)
	{
		//	Give the Ball Launcher to the player
		
        W = Spawn(BombLauncherClass,,,P.Location);
        if( W == None )
        {
			log(self@"could not spawn a launcher for player"@p);
			return;
        }
        w.GiveTo(P);
	}
	
    C.ClientSetWeapon( BombLauncherClass );
}

function SetThrow(vector start)
{
	Instigator = Holder;
	ThrowerTime = Level.TimeSeconds;
    SetLocation(start);
}	

function Throw(vector start, vector dir)
{
    SetThrow(start);
    Drop(Dir);
    if( PassTarget != None )
        Enable('Tick');
}


function bool ValidHolder(Actor Other)
{
    local Pawn P;
    local Vector RefNormal;

    if ( (Other == Instigator) && (Level.TimeSeconds - ThrowerTime < ThrowerTouchDelay) )
        return false;

    P = Pawn(Other);
    if( P != None && P.Weapon != None )
    {
        // bounce the ball off the shield
        if( P.Weapon.CheckReflect( Location, RefNormal, 0 ) )
        {
            P.Weapon.DoReflectEffect(10);
            Velocity = (Elasticity * 2.5) * (( Velocity dot RefNormal ) * RefNormal * (-2.0) + Velocity);
	        RandSpin(30000);
            return false;
        }
    }
    return Super.ValidHolder(Other);
}

function RandSpin(float spinRate)
{
	DesiredRotation = RotRand();
	RotationRate.Yaw = spinRate * 2 *FRand() - spinRate;
	RotationRate.Pitch = spinRate * 2 *FRand() - spinRate;
	RotationRate.Roll = spinRate * 2 *FRand() - spinRate;
    bFixedRotationDir = true;
}

// Events
function HitWall( vector HitNormal, actor Wall )
{
	Velocity = Elasticity*(( Velocity dot HitNormal ) * HitNormal * (-2.0) + Velocity);
	RandSpin(30000);

    PlaySound(ImpactSound, SLOT_Misc );

	if (VSize(Velocity) < 20) 
        Landed(HitNormal);
}

function Landed(vector hitNormal)
{
    Velocity = vect(0,0,0);
    if ( Holder == None )
		SetPhysics(PHYS_Rotating);
}

function TakeDamage( int Damage, Pawn EventInstigator, vector HitLocation, vector Momentum, class<DamageType> DamageType)
{
    if (Momentum != Vect(0,0,0) && !bHome && (Holder == None) )
    {
        SetPhysics(PHYS_Falling);
        Velocity += Momentum/Mass;
    }
}

// Logging
function LogDropped()
{
	if ( Holder == None )
		return;
	if ( Holder.Health <= 0 )
		BroadcastLocalizedMessage( MessageClass, 2, Holder.PlayerReplicationInfo, None, Holder.PlayerReplicationInfo.Team );
    if ( Level.Game.GameStats!=None )
        Level.Game.GameStats.GameEvent("bomb_dropped","255",Holder.PlayerReplicationInfo);
}

function LogReturned()
{
	if ( xBombingRun(Level.Game).ResetCountDown == 0 )
		BroadcastLocalizedMessage( MessageClass, 3, None, None, None );
}

function SendHomeDisabled(float TimeOut)
{
    CalcSetHome();
    GotoState('HomeDisabled');
}

// States
auto state Home
{
    ignores SendHome, Score, Drop;

    function LogTaken(Controller c)
    {
		OldTeam = C.PlayerReplicationInfo.Team;
        BroadcastLocalizedMessage( MessageClass, 6, C.PlayerReplicationInfo, None, C.PlayerReplicationInfo.Team );
        if (Level.Game.GameStats!=None)
            Level.Game.GameStats.GameEvent("bomb_taken","255",C.PlayerReplicationInfo);
    }
    
    function BeginState()
    {
		Super.BeginState();
		SetPhysics(PHYS_Rotating);
        Level.Game.GameReplicationInfo.FlagState[0] = EFlagState.FLAG_Home;
        Level.Game.GameReplicationInfo.FlagState[1] = EFlagState.FLAG_Home;
	}
}


state HomeDisabled
{
    ignores Score, Drop;

    function bool IsHome()
    {
        return true;
    }

    function BeginState()
    {
        SetDisable(true);
        Level.Game.GameReplicationInfo.FlagState[0] = EFlagState.FLAG_Home;
        Level.Game.GameReplicationInfo.FlagState[1] = EFlagState.FLAG_Home;
        bHome = true;
        SetLocation(HomeBase.Location);
        SetRotation(HomeBase.Rotation);
        SetCollision(false, false, false);
        bHidden = true;
    }

    function EndState()
    {
        SetDisable(false);
        bHome = false;
        SetCollision(true, false, false);
        bHidden = false;
    }
}

state Held
{
    ignores SetHolder, SendHome;
	
	function BeginState()
    {
        Level.Game.GameReplicationInfo.FlagState[Holder.PlayerReplicationInfo.Team.TeamIndex] = EFlagState.FLAG_HeldFriendly;
        if ( Holder.PlayerReplicationInfo.Team.TeamIndex == 0 )
	        Level.Game.GameReplicationInfo.FlagState[1] = EFlagState.FLAG_HeldEnemy;
        else
	        Level.Game.GameReplicationInfo.FlagState[0] = EFlagState.FLAG_HeldEnemy;
        Super.BeginState();
		bDynamicLight = false;
		LightType = LT_None;
        Skins[0] = TeamShader[Holder.PlayerReplicationInfo.Team.TeamIndex];
        RepSkin = Skins[0];
        SetStaticMesh(StaticMesh'XGame_RC.BombEffectMesh');
        SetDrawScale(1.0);
    }

    function EndState()
    {
        Super.EndState();
		bDynamicLight = true;
		LightType = LT_Steady;
        Skins[0] = Default.Skins[0];
        RepSkin = Skins[0];
        SetStaticMesh(Default.StaticMesh);
        SetDrawScale(Default.DrawScale);
    }
}

state Dropped
{
    ignores Drop;

    function LogTaken(Controller c)
    {
		if ( C.PlayerReplicationInfo.Team != OldTeam )
			BroadcastLocalizedMessage( MessageClass, 4, C.PlayerReplicationInfo, None, C.PlayerReplicationInfo.Team );
		OldTeam = C.PlayerReplicationInfo.Team;
        if (Level.Game.GameStats!=None)
            Level.Game.GameStats.GameEvent("bomb_pickup","255",C.PlayerReplicationInfo);
    }
	
	function Timer()
	{
	    if (Level.Game.GameStats!=None)
	        Level.Game.GameStats.GameEvent("bomb_returned_timeout","255",None);
			
		Super.Timer();
	}
	
	function BeginState()
	{
		Super.BeginState();
        Level.Game.GameReplicationInfo.FlagState[0] = EFlagState.FLAG_Down;
        Level.Game.GameReplicationInfo.FlagState[1] = EFlagState.FLAG_Down;
    }
}

// pass seeking
function Tick(float delta)
{
	local vector PassTargetDir;
	local float MagnitudeVel;

    SeekAccum += delta;

    if( SeekAccum >= SeekInterval )
    {
        SeekAccum = 0.0;
	    if ( InitialDir == vect(0,0,0) )
		    InitialDir = Normal(Velocity);
    		 
	    if ( (PassTarget != None) && (PassTarget != Instigator) ) 
	    {
		    PassTargetDir = Normal((PassTarget.Location + (vect(0,0,1)*PassTarget.CollisionHeight*0.5)) - Location);
		    if ( (PassTargetDir Dot InitialDir) > 0 )
		    {
			    MagnitudeVel = VSize(Velocity);
			    PassTargetDir = Normal(PassTargetDir * 0.5 * MagnitudeVel + Velocity);
			    Velocity =  MagnitudeVel * PassTargetDir;	
			    Acceleration = 5 * PassTargetDir;	
			    SetRotation(rotator(Velocity));
		    }
		    if ( Level.TimeSeconds - ThrowerTime > 4 )
				Disable('Tick');
	    }
        else
        {
            Disable('Tick');
        }
    }
}

defaultproperties
{
    ThrowerTouchDelay=1.f
    ImpactSound=Sound'WeaponSounds.ball_bounce_v3a'
    Elasticity=0.4
    RemoteRole=ROLE_DumbProxy
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'E_Pickups.BombBall.FullBomb'
    DrawScale=1.500000
    BombLauncherClassName="XWeapons.BallLauncher"
    NetUpdateFrequency=100
    MessageClass=class'XGame.xBombMessage'
    bProjTarget=true
    bHidden=false
    bStasis=false
    bHome=True
    bStatic=False
    Style=STY_Masked

    bFixedRotationDir=True
    RotationRate=(Yaw=30000)
    DesiredRotation=(Yaw=30000)
    Physics=PHYS_Rotating

    bDynamicLight=true
    LightHue=40
    LightBrightness=200
    bUnlit=true
    LightType=LT_Steady
    LightEffect=LE_QuadraticNonIncidence
    LightRadius=6

    TeamHue(0)=0
    TeamHue(1)=170
	TeamShader(0)=Shader'XGameShaders.BRShaders.BombIconRS'
	TeamShader(1)=Shader'XGameShaders.BRShaders.BombIconBS'
	Skins(0)=None
	Skins(1)=Shader'XGameShaders.BRShaders.BombIconYS'
	Skins(2)=Shader'XGameShaders.BRShaders.BombIconYS'

    CollisionRadius=24.0
	CollisionHeight=20.0
    bCollideActors=True
    bCollideWorld=True
    bUseCylinderCollision=true
    Mass=100.0
    Buoyancy=20.000000
    PrePivot=(X=2,Y=0,Z=0.5)
    NetPriority=+00003.000000
    SoundRadius=250
    ThrowSpeed=1300
    bBounce=true
    SeekInterval=0.05
    SeekAccum=0.0
	GameObjBone=spine
}
