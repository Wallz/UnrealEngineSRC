//=============================================================================
// Grenade.
//=============================================================================
class Grenade extends Projectile;

var float ExplodeTimer;
var bool bCanHitOwner, bHitWater;
var xEmitter Trail;
var() float DampenFactor, DampenFactorParallel;
var class<xEmitter> HitEffectClass;
var float LastSparkTime;
var bool bTimerSet;

replication
{
    reliable if (Role==ROLE_Authority)
        ExplodeTimer;
}

simulated function Destroyed()
{
    if ( Trail != None )
        Trail.mRegen = false; // stop the emitter from regenerating
	Super.Destroyed();
}

simulated function PostBeginPlay()
{
    Super.PostBeginPlay();

    if ( Level.NetMode != NM_DedicatedServer) 
    {
        Trail = Spawn(class'GrenadeSmokeTrail', self,, Location, Rotation);
    }

    if ( Role == ROLE_Authority )
    {
        Velocity = Speed * Vector(Rotation);
        RandSpin(25000);    
        bCanHitOwner = false;
        if (Instigator.HeadVolume.bWaterVolume)
        {
            bHitWater = true;
            Velocity = 0.6*Velocity;            
        }
    }   
}

simulated function PostNetBeginPlay()
{
	if ( Physics == PHYS_None )
    {
        SetTimer(ExplodeTimer, false); 
        bTimerSet = true;
    }
}

simulated function Timer()
{
    Explode(Location, vect(0,0,1));
}

simulated function Landed( vector HitNormal )
{
    HitWall( HitNormal, None );
}

simulated function ProcessTouch( actor Other, vector HitLocation )
{
    if ( Pawn(Other) != None && (Other != Instigator || bCanHitOwner) )
    {
		Explode(HitLocation, Normal(HitLocation-Other.Location));
    }
}
  
simulated function HitWall( vector HitNormal, actor Wall )
{
    local Vector VNorm;

    if (!bTimerSet)
    {
        SetTimer(ExplodeTimer, false); 
        bTimerSet = true;
    }

    // Reflect off Wall w/damping
    VNorm = (Velocity dot HitNormal) * HitNormal;
    Velocity = -VNorm * DampenFactor + (Velocity - VNorm) * DampenFactorParallel;

    RandSpin(100000);
    Speed = VSize(Velocity);

    if ( Speed < 20 ) 
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
			Spawn(HitEffectClass,,, Location, Rotator(HitNormal));
            LastSparkTime = Level.TimeSeconds;
        }
    }
}

simulated function Explode(vector HitLocation, vector HitNormal)
{
    BlowUp(HitLocation);
	PlaySound(sound'WeaponSounds.BExplosion3',,2.5*TransientSoundVolume);
    if ( EffectIsRelevant(Location,false) )
    {
        Spawn(class'RocketExplosion',,, HitLocation, rotator(vect(0,0,1)));
		Spawn(ExplosionDecal,self,,HitLocation, rotator(-HitNormal));
    }
    Destroy();
}

defaultproperties
{
	TossZ=+0.0
    HitEffectClass=class'XEffects.WallSparks'
    DampenFactor=0.5
    DampenFactorParallel=0.8
    ExplosionDecal=class'RocketMark'
    MyDamageType=class'DamTypeAssaultGrenade'
    Speed=1500
    MaxSpeed=2000
    Damage=70
    DamageRadius=240.0
    MomentumTransfer=75000
    ExplodeTimer=2.0
    ImpactSound=Sound'WeaponSounds.P1GrenFloor1'
    Physics=PHYS_Falling
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'WeaponStaticMesh.GrenadeMesh'
    DrawScale=3.0
    AmbientGlow=100
    bBounce=True
    bFixedRotationDir=True
    DesiredRotation=(Pitch=12000,Yaw=5666,Roll=2334)
}
