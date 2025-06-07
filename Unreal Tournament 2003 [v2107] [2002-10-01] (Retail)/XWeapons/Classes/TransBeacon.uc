
//=============================================================================
// Translocator Beacon
//=============================================================================
class TransBeacon extends TranslocatorBeacon;

var bool bCanHitOwner, bHitWater;
var xEmitter Trail;
var xEmitter Flare;
var Actor TranslocationTarget;	// for AI
var bool bDamaged;
var bool bNoAI;
var int Disruption;
var int DisruptionThreshold;
var Pawn Disruptor;
var TransBeaconSparks Sparks;
var class<TransTrail> TransTrailClass;
var class<TransFlareBlue> TransFlareClass;

replication
{
    reliable if ( Role == ROLE_Authority )
        Disruption;
}

simulated function Destroyed()
{
    if ( Trail != None )
        Trail.mRegen = false;
    if ( Flare != None )
    {
		Flare.mRegen = false;
        Flare.Destroy();
    }
    if ( Sparks != None )
        Sparks.Destroy();
	Super.Destroyed();
}

event EncroachedBy( actor Other )
{
	if ( Mover(Other) != None )
		Destroy();
}

simulated function bool Disrupted()
{
	return ( Disruption > DisruptionThreshold );
}

simulated function PostBeginPlay()
{
    local Rotator r;

    Super.PostBeginPlay();

    if ( Role == ROLE_Authority )
    {
        Velocity = Speed * Vector(Rotation);
        r.Yaw = Rotation.Yaw;
        r.Pitch = 0;
        r.Roll = 0;
        SetRotation(r);
        bCanHitOwner = false;
    }
    Trail = Spawn(TransTrailClass, self,, Location, Rotation);
    SetTimer(0.3,false);
}

simulated function PhysicsVolumeChange( PhysicsVolume Volume )
{
}

simulated function Landed( vector HitNormal )
{
    HitWall( HitNormal, None );
}

simulated function ProcessTouch( actor Other, vector HitLocation )
{
    if ( Other == Instigator && Physics == PHYS_None )
        Destroy();
    else if ( (Other != Instigator) || bCanHitOwner )
        HitWall( Normal(HitLocation-Other.Location), Other );
}

// poll for disruption
simulated function Timer()
{
    if ( Level.NetMode == NM_DedicatedServer )
        return;

    if ( !Disrupted() )
    {
        SetTimer(0.3, false);
        return;
    }

    // create the disrupted effect
    if (Sparks == None)
    {
        Sparks = Spawn(class'TransBeaconSparks',,,Location+vect(0,0,5),Rotator(vect(0,0,1)));
        Sparks.SetBase(self);
    }

    if (Flare != None)
        Flare.Destroy();
}

function TakeDamage(int Damage, Pawn EventInstigator, vector HitLocation, vector Momentum, class<DamageType> DamageType)
{
    if ( Level.Game.bTeamGame && (EventInstigator != None)
		&& (EventInstigator.PlayerReplicationInfo != None)
		&& ((Instigator == None) || (EventInstigator.PlayerReplicationInfo.Team == Instigator.PlayerReplicationInfo.Team)) )
    {
		return;
    }
    else
    {
        Disruption += Damage;
		Disruptor = EventInstigator;
    }
}

simulated function HitWall( vector HitNormal, actor Wall )
{
    bCanHitOwner = true;
	Velocity = 0.3*(( Velocity dot HitNormal ) * HitNormal * (-2.0) + Velocity);   // Reflect off Wall w/damping
	Speed = VSize(Velocity);

	if ( Speed < 20 && Wall.bWorldGeometry )
	{
		if ( Level.NetMode != NM_DedicatedServer )
			PlaySound(ImpactSound, SLOT_Misc );
		bBounce = false;
		SetPhysics(PHYS_None);

		if (Trail != None)
			Trail.mRegen = false;

		if (Level.NetMode != NM_DedicatedServer)
		{
			Flare = Spawn(TransFlareClass, self,, Location, rot(16384,0,0));
			Flare.SetBase(self);
		}
	}
	
    if( Wall.IsA('Pawn') && (HitNormal dot vect(0,0,1)) > 0.9 ) // try to prevent trans disc sitting on top of a player
    {
        Velocity += Normal((vect(0,0,5) + VRand())) * Speed;
    }
}

// AI Interface
function SetTranslocationTarget(actor T)
{
	TranslocationTarget = T;
	GotoState('MonitoringThrow');
}

function bool IsMonitoring(actor A)
{
	return false;
}

State MonitoringThrow
{
	function bool IsMonitoring(actor A)
	{
		return ( A == TranslocationTarget );
	}

	function Destroyed()
	{
		local Bot B;

		B = Bot(Instigator.Controller);
		if ( B != None )
		{
			B.TranslocationTarget = None;
			B.RealTranslocationTarget = None;
			B.bPreparingMove = false;
			B.SwitchToBestWeapon();
		}
		Global.Destroyed();
	}

	function EndMonitoring()
	{
		GotoState('');
	}

	function EndState()
	{
		local Bot B;

		B = Bot(Instigator.Controller);
		if ( (B != None) && !bNoAI )
		{
			B.TranslocationTarget = None;
			B.RealTranslocationTarget = None;
			B.bPreparingMove = false;
			B.SwitchToBestWeapon();
		}
	}

	simulated function HitWall( vector HitNormal, actor Wall )
	{
		Global.HitWall(HitNormal,Wall);
		if ( Physics == PHYS_None )
		{
			if ( (Bot(Instigator.Controller) != None) && Bot(Instigator.Controller).bPreparingMove )
			{
				Bot(Instigator.Controller).MoveTimer = -1;
				if ( (JumpSpot(TranslocationTarget) != None) && (Instigator.Controller.MoveTarget == TranslocationTarget) )
					JumpSpot(TranslocationTarget).FearCost += 900;
			}
			EndMonitoring();
		}
	}

	function BotTranslocate()
	{
		if ( TransLauncher(Instigator.Weapon) != None )
			Instigator.Weapon.FireMode[1].DoFireEffect();
		EndMonitoring();
	}

	function Touch(Actor Other)
	{
		local Pawn P;
		
		P = Pawn(Other);
		if ( (P == None) || (P == Instigator) )
			return;
		ProcessTouch( Other, Location );
		if ( (Bot(P.Controller) == None) || Level.Game.IsOnTeam(P.Controller,Instigator.PlayerReplicationInfo.Team.TeamIndex) )
		{
			EndMonitoring();
			return;
		}
		if ( Bot(P.Controller).ProficientWithWeapon() && (2 + FRand() * 8 < Bot(P.Controller).Skill) )
			BotTranslocate();
	}

	// FIXME - consider making this a timer or projectile latent function instead of using tick?
	function Tick(float DeltaTime)
	{
		local vector Dist, Dir;
		local float ZDiff;

		if ( (TranslocationTarget == None) || (Instigator.Controller == None) 
			|| ((GameObject(Instigator.Controller.MoveTarget) != None) && (Instigator.Controller.MoveTarget != TranslocationTarget))
			|| ((TranslocationTarget != Instigator.Controller.MoveTarget)
				&& (TranslocationTarget != Instigator.Controller.RouteGoal)
				&& (TranslocationTarget != Instigator.Controller.RouteCache[0])
				&& (TranslocationTarget != Instigator.Controller.RouteCache[1])
				&& (TranslocationTarget != Instigator.Controller.RouteCache[2])) )
		{
			EndMonitoring();
			return;
		}

		Dist = Location - TranslocationTarget.Location;
		ZDiff = Dist.Z;
		Dist.Z = 0;
		Dir = TranslocationTarget.Location - Instigator.Location;
		Dir.Z = 0;
		if ( VSize(Dist) < TranslocationTarget.CollisionRadius )
		{
			if ( ZDiff > -0.9 * TranslocationTarget.CollisionHeight )
			{
				Instigator.Controller.MoveTarget = TranslocationTarget;
				BotTranslocate();
			}
			return;
		}
		Dir = TranslocationTarget.Location - Instigator.Location;
		Dir.Z = 0;
		if ( (Dist Dot Dir) > 0 )
		{
			if ( (Bot(Instigator.Controller) != None) && Bot(Instigator.Controller).bPreparingMove )
			{
				Bot(Instigator.Controller).MoveTimer = -1;
				if ( (JumpSpot(TranslocationTarget) != None) && (Instigator.Controller.MoveTarget == TranslocationTarget) )
					JumpSpot(TranslocationTarget).FearCost += 400;
			}
			EndMonitoring();
			return;
		}
	}
}
// END AI interface

defaultproperties
{
    ExplosionDecal=class'RocketMark'
    MyDamageType=class'DamTypeTeleFrag'
    Speed=1200
    MaxSpeed=2000
    Damage=0
    DamageRadius=100
    MomentumTransfer=50000
    ImpactSound=Sound'WeaponSounds.P1GrenFloor1'
    Physics=PHYS_Falling
    Mesh=Mesh'Weapons.TransBeacon'
    DrawScale=1.5
    AmbientGlow=64
    bUnlit=false
    bBounce=true
    bNetTemporary=false
    bUpdateSimulatedPosition=true
	NetUpdateFrequency=8
    AmbientSound=Sound'WeaponSounds.Redeemer_Flight'
	
    CollisionRadius=10.000000
	CollisionHeight=10.00000
	PrePivot=(X=0.0,Y=0.0,Z=-7.0)
    SoundRadius=7
    SoundVolume=250
    SoundPitch=128
    bProjTarget=true
    Disruption=0
    DisruptionThreshold=65
    bNetNotify=true

	bOwnerNoSee=true
    
    TransTrailClass=class'TransTrail'
    TransFlareClass=class'TransFlareRed'
}
