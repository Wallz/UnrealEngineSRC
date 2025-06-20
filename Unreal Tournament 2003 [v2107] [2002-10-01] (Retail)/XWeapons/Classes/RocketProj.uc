//=============================================================================
// rocket.
//=============================================================================
class RocketProj extends Projectile;

var bool bRing,bHitWater,bWaterStart;
var int NumExtraRockets;
var	xEmitter SmokeTrail;
var Effects Corona;
var byte FlockIndex;
var RocketProj Flock[2];

var() float	FlockRadius;
var() float	FlockStiffness;
var() float FlockMaxForce;
var() float	FlockCurlForce;
var bool bCurl;
var vector Dir;

replication
{
    reliable if ( bNetInitial && (Role == ROLE_Authority) )
        FlockIndex, bCurl;
}

simulated function Destroyed() 
{
	if ( SmokeTrail != None )
		SmokeTrail.mRegen = False;
	if ( Corona != None )
		Corona.Destroy();
	Super.Destroyed();
}

simulated function PostBeginPlay()
{
	if ( Level.NetMode != NM_DedicatedServer)
	{
		SmokeTrail = Spawn(class'RocketTrailSmoke',self);
		Corona = Spawn(class'RocketCorona',self); 
	}

	Dir = vector(Rotation);
	Velocity = speed * Dir;
	if (PhysicsVolume.bWaterVolume)
	{
		bHitWater = True;
		Velocity=0.6*Velocity;
	}
    if ( Level.bDropDetail )
	{
		bDynamicLight = false;
		LightType = LT_None;
	}
}

simulated function PostNetBeginPlay()
{
	local RocketProj R;
	local int i;
	
	Super.PostNetBeginPlay();

	if ( FlockIndex != 0 )
	{
	    SetTimer(0.1, true);
	    
	    // look for other rockets
	    if ( Flock[1] == None )
	    {
			ForEach DynamicActors(class'RocketProj',R)
				if ( R.FlockIndex == FlockIndex )
				{
					Flock[i] = R;
					if ( R.Flock[0] == None )
						R.Flock[0] = self;
					else if ( R.Flock[0] != self )
						R.Flock[1] = self;
					i++;
					if ( i == 2 )
						break;
				}
		}
	}
}

simulated function Landed( vector HitNormal )
{
	Explode(Location,HitNormal);
}

simulated function ProcessTouch (Actor Other, Vector HitLocation)
{
	if ( (Other != instigator) && (!Other.IsA('Projectile') || Other.bProjTarget) ) 
		Explode(HitLocation,Vect(0,0,1));
}

function BlowUp(vector HitLocation)
{
	HurtRadius(Damage, DamageRadius, MyDamageType, MomentumTransfer, HitLocation );
	MakeNoise(1.0);
}

simulated function Explode(vector HitLocation, vector HitNormal) 
{
	PlaySound(sound'WeaponSounds.BExplosion3',,2.5*TransientSoundVolume);
    if ( EffectIsRelevant(Location,false) )
    {
    	Spawn(class'RocketExplosion',,,HitLocation + HitNormal*16,rotator(HitNormal));	
        Spawn(class'ExplosionCrap',,, HitLocation, rotator(HitNormal));
		if ( (ExplosionDecal != None) && (Level.NetMode != NM_DedicatedServer) )
			Spawn(ExplosionDecal,self,,Location, rotator(-HitNormal));
    }
	
	BlowUp(HitLocation);
	Destroy(); 
}

simulated function Timer()
{
    local vector ForceDir, CurlDir;
    local float ForceMag;
    local int i;
		
	Velocity =  Default.Speed * Normal(Dir * 0.5 * Default.Speed + Velocity);  
	
	// Work out force between flock to add madness
	for(i=0; i<2; i++)
	{			
		if(Flock[i] == None)
			continue;
	
		// Attract if distance between rockets is over 2*FlockRadius, repulse if below.
		ForceDir = Flock[i].Location - Location;
		ForceMag = FlockStiffness * ( (2 * FlockRadius) - VSize(ForceDir) );
		Acceleration = Normal(ForceDir) * Min(ForceMag, FlockMaxForce);
		
		// Vector 'curl'
		CurlDir = Flock[i].Velocity Cross ForceDir;
		if ( bCurl == Flock[i].bCurl )
			Acceleration += Normal(CurlDir) * FlockCurlForce;
		else
			Acceleration -= Normal(CurlDir) * FlockCurlForce;
	}
}

defaultproperties
{
    speed=1350.0
    MaxSpeed=1350.0
    Damage=90.0
    DamageRadius=220.0  
    MomentumTransfer=50000
    MyDamageType=class'DamTypeRocket'
    ExplosionDecal=class'RocketMark'
    RemoteRole=ROLE_SimulatedProxy
    LifeSpan=6.0
    AmbientSound=Sound'WeaponSounds.RocketLauncher.RocketLauncherProjectile'
    SoundVolume=255
    SoundRadius=100
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'WeaponStaticMesh.RocketProj'
    DrawScale=1.0
    AmbientGlow=96
    bUnlit=True
    LightType=LT_Steady
    LightEffect=LE_QuadraticNonIncidence
    LightBrightness=255
    LightHue=28
    LightRadius=5
    bDynamicLight=true
    bBounce=false
    bFixedRotationDir=True
    RotationRate=(Roll=50000)
    DesiredRotation=(Roll=30000)
    ForceType=FT_Constant
    ForceScale=5.0
    ForceRadius=100.0
    bCollideWorld=true

    FlockRadius=12
    FlockStiffness=-40
    FlockMaxForce=600
    FlockCurlForce=450  
}
