#exec OBJ LOAD FILE=..\Sounds\MenuSounds.uax

class ONSGrenadeProjectile extends Projectile;

var bool bCanHitOwner;
var xEmitter Trail;
var() float DampenFactor, DampenFactorParallel;
var class<xEmitter> HitEffectClass;
var float LastSparkTime;
var Actor IgnoreActor; //don't stick to this actor
var byte Team;
var Emitter Beacon;

replication
{
	reliable if (bNetDirty && Role == ROLE_Authority)
		IgnoreActor, Team;
}

simulated function Destroyed()
{
    if ( Trail != None )
        Trail.mRegen = false; // stop the emitter from regenerating
    //explosion
    if ( !bNoFX )
    {
		if ( EffectIsRelevant(Location,false) )
		{
			Spawn(class'ONSGrenadeExplosionEffect',,, Location, rotator(vect(0,0,1)));
			Spawn(ExplosionDecal,self,, Location, rotator(vect(0,0,-1)));
		}
		PlaySound(sound'WeaponSounds.BExplosion3',,2.5*TransientSoundVolume);
	}
    if ( ONSGrenadeLauncher(Owner) != None)
    	ONSGrenadeLauncher(Owner).CurrentGrenades--;

	if ( Beacon != None )
		Beacon.Destroy();

    Super.Destroyed();
}

simulated function PostBeginPlay()
{
	local PlayerController PC;

    Super.PostBeginPlay();

    if ( Level.NetMode != NM_DedicatedServer)
    {
		PC = Level.GetLocalPlayerController();
		if ( (PC.ViewTarget != None) && VSize(PC.ViewTarget.Location - Location) < 5500 )
	        Trail = Spawn(class'GrenadeSmokeTrail', self,, Location, Rotation);
    }

    Velocity = Speed * Vector(Rotation);
    RandSpin(25000);
    if (PhysicsVolume.bWaterVolume)
        Velocity = 0.6*Velocity;

    if (Role == ROLE_Authority && Instigator != None)
    	Team = Instigator.GetTeamNum();
}

simulated function PostNetBeginPlay()
{
	if ( Level.NetMode != NM_DedicatedServer )
	{
		if (Team == 1)
			Beacon = spawn(class'ONSGrenadeBeaconBlue', self);
		else
			Beacon = spawn(class'ONSGrenadeBeaconRed', self);

		if (Beacon != None)
			Beacon.SetBase(self);
	}
	Super.PostNetBeginPlay();
}

simulated function Landed( vector HitNormal )
{
    HitWall( HitNormal, None );
}

simulated function ProcessTouch( actor Other, vector HitLocation )
{
    if (!bPendingDelete && Base == None && Other != IgnoreActor && (!Other.bWorldGeometry && Other.Class != Class && (Other != Instigator || bCanHitOwner)))
	Stick(Other, HitLocation);
}

simulated function HitWall( vector HitNormal, actor Wall )
{
    local Vector VNorm;
	local PlayerController PC;

    if (Vehicle(Wall) != None)
    {
        Touch(Wall);
        return;
    }

    // Reflect off Wall w/damping
    VNorm = (Velocity dot HitNormal) * HitNormal;
    Velocity = -VNorm * DampenFactor + (Velocity - VNorm) * DampenFactorParallel;

    RandSpin(100000);
    Speed = VSize(Velocity);

    if ( Speed < 40 )
    {
        bBounce = False;
        SetPhysics(PHYS_None);
        if ( Trail != None )
            Trail.mRegen = false; // stop the emitter from regenerating
    }
    else
    {
		if ( (Level.NetMode != NM_DedicatedServer) && (Speed > 250) )
			PlaySound(ImpactSound, SLOT_Misc );
        if ( !Level.bDropDetail && (Level.DetailMode != DM_Low) && (Level.TimeSeconds - LastSparkTime > 0.5) && EffectIsRelevant(Location,false) )
        {
			PC = Level.GetLocalPlayerController();
			if ( (PC.ViewTarget != None) && VSize(PC.ViewTarget.Location - Location) < 2000 )
				Spawn(HitEffectClass,,, Location, Rotator(HitNormal));
            LastSparkTime = Level.TimeSeconds;
        }
    }
}

simulated function Explode(vector HitLocation, vector HitNormal)
{
    LastTouched = Base;
    BlowUp(HitLocation);
    Destroy();
}

simulated function TakeDamage(int Damage, Pawn EventInstigator, vector HitLocation, vector Momentum, class<DamageType> DamageType)
{
	local Actor A;

	if (Damage > 0)
	{
		if (Base != None && DamageType != MyDamageType)
		{
			if (EventInstigator == None || EventInstigator != Instigator)
			{
				UnStick();
				return;
			}
		}
		else if (IgnoreActor != None) //just got knocked off something I was stuck to, so don't explode
			foreach VisibleCollidingActors(IgnoreActor.Class, A, DamageRadius)
				return;

		Explode(Location, vect(0,0,1));
	}
}

simulated function Stick(actor HitActor, vector HitLocation)
{
    if ( Trail != None )
        Trail.mRegen = false; // stop the emitter from regenerating

    bBounce = False;
    LastTouched = HitActor;
    SetPhysics(PHYS_None);
    SetBase(HitActor);
    if (Base == None)
    {
    	UnStick();
    	return;
    }
    bCollideWorld = False;
    bProjTarget = true;

	PlaySound(Sound'MenuSounds.Select3',,2.5*TransientSoundVolume);
}

simulated function UnStick()
{
	Velocity = vect(0,0,0);
	IgnoreActor = Base;
	SetBase(None);
	SetPhysics(PHYS_Falling);
	bCollideWorld = true;
	bProjTarget = false;
	LastTouched = None;
}

simulated function BaseChange()
{
	if (!bPendingDelete && Physics == PHYS_None && Base == None)
		UnStick();
}

simulated function PawnBaseDied()
{
	Explode(Location, vect(0,0,1));
}

defaultproperties
{
	TossZ=+0.0
    HitEffectClass=class'XEffects.WallSparks'
    DampenFactor=0.5
    DampenFactorParallel=0.8
    ExplosionDecal=class'ONSRocketScorch'
    MyDamageType=class'DamTypeONSGrenade'
    LifeSpan=0.0
    Speed=1200
    MaxSpeed=1200
    Damage=100
    DamageRadius=175.0
    MomentumTransfer=50000
    CollisionRadius=1
    CollisionHeight=1
    ImpactSound=Sound'WeaponSounds.P1GrenFloor1'
    Physics=PHYS_Falling
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'VMWeaponsSM.PlayerWeaponsGroup.VMGrenade'
    DrawScale=0.075
    AmbientGlow=100
    bBounce=True
    bFixedRotationDir=True
    DesiredRotation=(Pitch=12000,Yaw=5666,Roll=2334)
    bHardAttach=True
    bNetTemporary=false
    bOnlyDirtyReplication=true
    RemoteRole=ROLE_SimulatedProxy
    bProjTarget=true
    bSwitchToZeroCollision=true
    CullDistance=+5000.0
    bUseCollisionStaticMesh=true
}
