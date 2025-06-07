class RedeemerWarhead extends Pawn;

var float Damage, DamageRadius, MomentumTransfer;
var class<DamageType> MyDamageType;
var Pawn OldPawn;
var	RedeemerTrail SmokeTrail;
var float YawAccel, PitchAccel;

// camera shakes //
var() vector ShakeRotMag;           // how far to rot view
var() vector ShakeRotRate;          // how fast to rot view
var() float  ShakeRotTime;          // how much time to rot the instigator's view
var() vector ShakeOffsetMag;        // max view offset vertically
var() vector ShakeOffsetRate;       // how fast to offset view vertically
var() float  ShakeOffsetTime;       // how much time to offset view

var bool	bStaticScreen;
var bool	bFireForce;

var TeamInfo MyTeam;

replication
{
    reliable if (Role == ROLE_Authority && bNetOwner)
        bStaticScreen;
        
    reliable if ( Role < ROLE_Authority )
		ServerBlowUp;
}


function TeamInfo GetTeam()
{
	if ( PlayerReplicationInfo != None )
		return PlayerReplicationInfo.Team;
	return MyTeam;
}

simulated function Destroyed() 
{
	RelinquishController();
	if ( SmokeTrail != None )
		SmokeTrail.Destroy();
	Super.Destroyed();
}

simulated function bool IsPlayerPawn()
{
	return false;
}

event bool EncroachingOn( actor Other )
{
	if ( Other.bWorldGeometry )
		return true;
		
	return false;
}

function RelinquishController()
{
	if ( Controller == None )
		return;
	Controller.Pawn = None;
	if ( !Controller.IsInState('GameEnded') )
	{
		if ( (OldPawn != None) && (OldPawn.Health > 0) )
			Controller.Possess(OldPawn);
		else
		{
			if ( OldPawn != None )
				Controller.Pawn = OldPawn;
			else
				Controller.Pawn = self;
			Controller.PawnDied(Controller.Pawn);
		}
	}
	Instigator = OldPawn;
	Controller = None;
}

simulated function PostBeginPlay()
{
	local vector Dir;
	
	Dir = Vector(Rotation);
    Velocity = AirSpeed * Dir;
    Acceleration = Velocity;

	if ( Level.NetMode != NM_DedicatedServer)
	{
		SmokeTrail = Spawn(class'RedeemerTrail',self,,Location - 40 * Dir);
		SmokeTrail.SetBase(self);
	}
}

simulated function PostNetBeginPlay()
{
	Super.PostNetBeginPlay();
	
	if ( PlayerController(Controller) != None )
	{
		Controller.SetRotation(Rotation);
		PlayerController(Controller).SetViewTarget(self);
		Controller.GotoState(LandMovementState);
	}
}

simulated function FaceRotation( rotator NewRotation, float DeltaTime )
{
}
	
function UpdateRocketAcceleration(float DeltaTime, float YawChange, float PitchChange)
{
    local vector X,Y,Z;
	local float PitchThreshold;
	local int Pitch;
	
	YawAccel = (1-2*DeltaTime)*YawAccel + DeltaTime*YawChange;
	PitchAccel = (1-2*DeltaTime)*PitchAccel + DeltaTime*PitchChange;
	SetRotation(rotator(Velocity));
	GetAxes(Rotation,X,Y,Z);
	PitchThreshold = 3000;
	Pitch = Rotation.Pitch & 65535;
	if ( (Pitch > 16384 - PitchThreshold) && (Pitch < 49152 + PitchThreshold) )
	{
		if ( Pitch > 49152 - PitchThreshold )
			PitchAccel = Max(PitchAccel,0);
		else if ( Pitch < 16384 + PitchThreshold )
			PitchAccel = Min(PitchAccel,0);
	}
	Acceleration = Velocity + 5*(YawAccel*Y + PitchAccel*Z);
	if ( Acceleration == vect(0,0,0) )
		Acceleration = Velocity;
		
	Acceleration = Normal(Acceleration) * AccelRate;
}

simulated function PhysicsVolumeChange( PhysicsVolume Volume )
{
}

simulated function Landed( vector HitNormal )
{
	BlowUp(Location);
}

simulated function HitWall(vector HitNormal, actor Wall)
{
	BlowUp(Location);
}

function UnPossessed()
{
	BlowUp(Location);
}

simulated singular function Touch(Actor Other)
{
	if ( Other.bBlockActors )
		BlowUp(Location);
}

function TakeDamage( int Damage, Pawn instigatedBy, Vector hitlocation, 
							Vector momentum, class<DamageType> damageType) 
{
	if ( Damage > 0 )
	{
		if ( InstigatedBy == None )
			BlowUp(Location);
		else
		{
    		Spawn(class'SmallRedeemerExplosion');	
		    RelinquishController();
		    SetCollision(false,false,false);
		    HurtRadius(Damage, DamageRadius*0.125, MyDamageType, MomentumTransfer, Location);
		    Destroy();
		}
	}
}

function Fire( optional float F )
{
	ServerBlowUp();
	if ( F == 1 )
	{
		OldPawn.Health = -1;
		OldPawn.KilledBy(OldPawn);
	}
}

function ServerBlowUp()
{
	BlowUp(Location);
}

function BlowUp(vector HitLocation)
{
	if ( Role == ROLE_Authority )
	{
		bHidden = true;
        Spawn(class'RedeemerExplosion',,, HitLocation - 100 * Normal(Velocity), Rot(0,16384,0));
		GotoState('Dying'); 
	}
}

function bool DoJump( bool bUpdating )
{
	return false;
}

singular event BaseChange()
{
}

simulated function DrawHUD(Canvas Canvas)
{
    Canvas.Style = 255;
	Canvas.SetPos(0,0);
	Canvas.DrawColor = class'Canvas'.static.MakeColor(255,255,255);
	if ( bStaticScreen )
		Canvas.DrawTile( Material'ScreenNoiseFB', Canvas.SizeX, Canvas.SizeY, 0.0, 0.0, 512, 512 );
	else
	{	
	Canvas.DrawTile( Material'XGameShaders.RedeemerReticle', 0.5 * Canvas.SizeX, 0.5 * Canvas.SizeY, 0, 0, 512, 512 ); 
	Canvas.SetPos(0.5*Canvas.SizeX,0);
	Canvas.DrawTile( Material'XGameShaders.RedeemerReticle', 0.5 * Canvas.SizeX, 0.5 * Canvas.SizeY, 512, 0, -512, 512 ); 
	Canvas.SetPos(0,0.5*Canvas.SizeY);
	Canvas.DrawTile( Material'XGameShaders.RedeemerReticle', 0.5 * Canvas.SizeX, 0.5 * Canvas.SizeY, 0, 512, 512, -512 ); 
	Canvas.SetPos(0.5*Canvas.SizeX,0.5*Canvas.SizeY);
	Canvas.DrawTile( Material'XGameShaders.RedeemerReticle', 0.5 * Canvas.SizeX, 0.5 * Canvas.SizeY, 512, 512, -512, -512 ); 
	}	
}

simulated event PlayDying(class<DamageType> DamageType, vector HitLoc);

function Died(Controller Killer, class<DamageType> damageType, vector HitLocation)
{
	BlowUp(Location);
}

auto state Flying
{
	function Tick(float DeltaTime)
	{
		if ( !bFireForce && (PlayerController(Controller) != None) )
		{
			bFireForce = true;
			PlayerController(Controller).ClientPlayForceFeedback("FlakCannonAltFire");  // jdf
		}			
		if ( (OldPawn == None) || (OldPawn.Health <= 0) )
			BlowUp(Location);
		else if ( Controller == None )
		{
			if ( OldPawn.Controller == None )
				OldPawn.KilledBy(OldPawn);
			BlowUp(Location);
		}			
	}
}

state Dying
{
ignores Trigger, Bump, HitWall, HeadVolumeChange, PhysicsVolumeChange, Falling, BreathTimer;

	function Fire( optional float F ) {}
	function BlowUp(vector HitLocation) {}
	function ServerBlowUp() {}
	function Timer() {}
	function TakeDamage( int Damage, Pawn instigatedBy, Vector hitlocation, 
							Vector momentum, class<DamageType> damageType) {}
							
    function BeginState()
    {
		bHidden = true;
		bStaticScreen = true;
		SetPhysics(PHYS_None);
		SetCollision(false,false,false);
		Spawn(class'IonCore',,, Location, Rotation);
		if ( SmokeTrail != None )
			SmokeTrail.Destroy();
		ShakeView();
    }

    function ShakeView()
    {
        local Controller C;
        local PlayerController PC;
        local float Dist, Scale;

        for ( C=Level.ControllerList; C!=None; C=C.NextController )
        {
            PC = PlayerController(C);
            if ( PC != None && PC.ViewTarget != None )
            {
                Dist = VSize(Location - PC.ViewTarget.Location);
                if ( Dist < DamageRadius * 2.0)
                {
                    if (Dist < DamageRadius)
                        Scale = 1.0;
                    else
                        Scale = (DamageRadius*2.0 - Dist) / (DamageRadius);
                    C.ShakeView(ShakeRotMag*Scale, ShakeRotRate, ShakeRotTime, ShakeOffsetMag*Scale, ShakeOffsetRate, ShakeOffsetTime);
                }
            }
        }
    }

Begin:
	Instigator = self;
    PlaySound(sound'WeaponSounds.redeemer_explosionsound');
    HurtRadius(Damage, DamageRadius*0.125, MyDamageType, MomentumTransfer, Location);
    Sleep(0.5);
    HurtRadius(Damage, DamageRadius*0.300, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.475, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    RelinquishController();
    HurtRadius(Damage, DamageRadius*0.650, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.825, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*1.000, MyDamageType, MomentumTransfer, Location);
    Destroy();
}

defaultproperties
{
	LandMovementState=PlayerRocketing
    RemoteRole=ROLE_SimulatedProxy
    NetPriority=3
    bNetTemporary=false
    bGameRelevant=true
    bReplicateInstigator=true
	bUpdateSimulatedPosition=true
	bSimulateGravity=false
    Physics=PHYS_Flying
    AirSpeed=1000.0
    AccelRate=1200.0

    bStasis=false
    bCanCrouch=false
    bCanClimbLadders=false
    bCanPickupInventory=false
	bNoTeamBeacon=true
	
    BaseEyeHeight=0.0
    EyeHeight=0.0

    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'WeaponStaticMesh.RedeemerMissile'
    DrawScale=0.5
    AmbientGlow=96
    bUnlit=false

	bNetInitialRotation=true
	bSpecialHUD=true
	bHideRegularHUD=true
    bDynamicLight=true
    LightType=LT_Steady
    LightEffect=LE_QuadraticNonIncidence
    LightBrightness=255
    LightHue=28
    LightRadius=6

    ForceType=FT_DragAlong
    ForceScale=5.0
    ForceRadius=100.0

    Damage=250.0
    DamageRadius=2000.0
    MomentumTransfer=200000
    MyDamageType=class'DamTypeRedeemer'

    AmbientSound=Sound'WeaponSounds.redeemer_flight'
    SoundRadius=100
    SoundVolume=255

	bCanTeleport=false
	bDirectHitWall=true
    bCollideActors=true
    bCollideWorld=true
    bBlockActors=false
    bBlockPlayers=false
    CollisionHeight=12.0
    CollisionRadius=24.0

    ShakeRotMag=(Z=250)
    ShakeRotRate=(Z=2500)
    ShakeRotTime=6
    ShakeOffsetMag=(Z=10)
    ShakeOffsetRate=(Z=200)
    ShakeOffsetTime=10
    
    TransientSoundVolume=1.0
    TransientSoundRadius=5000.0
}
