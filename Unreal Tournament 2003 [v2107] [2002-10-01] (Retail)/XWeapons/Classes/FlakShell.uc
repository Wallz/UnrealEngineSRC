//=============================================================================
// flakshell
//=============================================================================
class flakshell extends Projectile;

var	xemitter trail;
var vector initialDir;

simulated function PostBeginPlay()
{
	local Rotator R;
	
	if ( !PhysicsVolume.bWaterVolume && (Level.NetMode != NM_DedicatedServer) )
		Trail = Spawn(class'FlakShellTrail',self);

	Super.PostBeginPlay();
	Velocity = Vector(Rotation) * Speed;  
	R = Rotation;
	R.Roll = 32768;
	SetRotation(R);
	Velocity.z += TossZ; 
	initialDir = Velocity;
}

simulated function destroyed()
{
	if (Trail!=None) 
		Trail.mRegen=False;
	Super.Destroyed();
}


simulated function ProcessTouch(Actor Other, Vector HitLocation)
{
	if ( Other != Instigator )
	{
		SpawnEffects(HitLocation, -1 * Normal(Velocity) );
		Explode(HitLocation,Normal(HitLocation-Other.Location));
	}
}

simulated function SpawnEffects( vector HitLocation, vector HitNormal )
{
	PlaySound (Sound'WeaponSounds.BExplosion1',,3*TransientSoundVolume);
	if ( EffectIsRelevant(Location,false) )
	{
		spawn(class'FlakExplosion',,,HitLocation + HitNormal*16 );
		spawn(class'FlashExplosion',,,HitLocation + HitNormal*16 );
		spawn(class'RocketSmokeRing',,,HitLocation + HitNormal*16, rotator(HitNormal) );
		if ( (ExplosionDecal != None) && (Level.NetMode != NM_DedicatedServer) )
			Spawn(ExplosionDecal,self,,HitLocation, rotator(-HitNormal));
	}
}

simulated function Landed( vector HitNormal )
{
	SpawnEffects( Location, HitNormal );
	Explode(Location,HitNormal);
}

simulated function HitWall (vector HitNormal, actor Wall)
{
	Landed(HitNormal);
}

event TakeDamage( int Damage, Pawn EventInstigator, vector HitLocation, vector Momentum, class<DamageType> DamageType)
{
	Landed( Normal(EventInstigator.Location-Location) );
}

simulated function Explode(vector HitLocation, vector HitNormal)
{
	local vector start;
    local rotator rot;
    local int i;
    local FlakChunk NewChunk;

	start = Location + 10 * HitNormal;
	if ( Role == ROLE_Authority )
	{
		HurtRadius(damage, 220, MyDamageType, MomentumTransfer, HitLocation);	
		for (i=0; i<6; i++)
		{
			rot = Rotation;
			rot.yaw += FRand()*32000-16000;
			rot.pitch += FRand()*32000-16000;
			rot.roll += FRand()*32000-16000;
			NewChunk = Spawn( class 'FlakChunk',, '', Start, rot);
		}
	}
    Destroy();
}

defaultproperties
{
    ExplosionDecal=class'RocketMark'
	TossZ=+200.0
	bProjTarget=True
    speed=1100.000000
    Damage=90.000000
    MomentumTransfer=75000
    bNetTemporary=True
    Physics=PHYS_Falling
    MyDamageType=class'DamTypeFlakShell'
    LifeSpan=6.000000
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'WeaponStaticMesh.FlakShell'
    DrawScale=6.0
    AmbientGlow=100
    AmbientSound=Sound'WeaponSounds.BaseProjectileSounds.BFlakCannonProjectile'
    SoundRadius=100
    SoundVolume=255
    ForceType=FT_Constant
    ForceScale=5.0
    ForceRadius=60.0
    bSwitchToZeroCollision=true
}
