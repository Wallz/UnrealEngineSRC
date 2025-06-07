//=============================================================================
// rocket.
//=============================================================================
class RedeemerProjectile extends Projectile;

var	RedeemerTrail SmokeTrail;

// camera shakes //
var() vector ShakeRotMag;           // how far to rot view
var() vector ShakeRotRate;          // how fast to rot view
var() float  ShakeRotTime;          // how much time to rot the instigator's view
var() vector ShakeOffsetMag;        // max view offset vertically
var() vector ShakeOffsetRate;       // how fast to offset view vertically
var() float  ShakeOffsetTime;       // how much time to offset view

simulated function Destroyed() 
{
	if ( SmokeTrail != None )
		SmokeTrail.Destroy();
	Super.Destroyed();
}

simulated function PostBeginPlay()
{
	local vector Dir;
	
	Dir = vector(Rotation);
	Velocity = speed * Dir;
	
	if ( Level.NetMode != NM_DedicatedServer)
	{
		SmokeTrail = Spawn(class'RedeemerTrail',self,,Location - 40 * Dir);
		SmokeTrail.SetBase(self);
	}
}

event bool EncroachingOn( actor Other )
{
	if ( Other.bWorldGeometry )
		return true;
		
	return false;
}

simulated function ProcessTouch (Actor Other, Vector HitLocation)
{
	if ( Other != instigator ) 
		Explode(HitLocation,Vect(0,0,1));
}

simulated function Explode(vector HitLocation, vector HitNormal) 
{
	BlowUp(HitLocation);
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
		    SetCollision(false,false,false);
		    HurtRadius(Damage, DamageRadius*0.125, MyDamageType, MomentumTransfer, Location);
		    Destroy();
		}
	}
}

simulated event FellOutOfWorld(eKillZType KillType)
{
	BlowUp(Location);
}	

function BlowUp(vector HitLocation)
{
    Spawn(class'RedeemerExplosion',,, HitLocation - 100 * Normal(Velocity), Rot(0,16384,0));
	MakeNoise(1.0);
	SetPhysics(PHYS_None);
	bHidden = true;
    GotoState('Dying'); 
}

state Dying
{
	function TakeDamage( int Damage, Pawn instigatedBy, Vector hitlocation, 
							Vector momentum, class<DamageType> damageType) {}
							
    function BeginState()
    {
		bHidden = true;
		SetPhysics(PHYS_None);
		SetCollision(false,false,false);
		Spawn(class'IonCore',,, Location, Rotation);
		ShakeView();
		if ( SmokeTrail != None )
			SmokeTrail.Destroy();
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
    PlaySound(sound'WeaponSounds.redeemer_explosionsound');
    HurtRadius(Damage, DamageRadius*0.125, MyDamageType, MomentumTransfer, Location);
    Sleep(0.5);
    HurtRadius(Damage, DamageRadius*0.300, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.475, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.650, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.825, MyDamageType, MomentumTransfer, Location);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*1.000, MyDamageType, MomentumTransfer, Location);
    Destroy();
}
defaultproperties
{
    bNetTemporary=false

    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'WeaponStaticMesh.RedeemerMissile'
    DrawScale=0.5
    AmbientGlow=96
    bUnlit=false

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

    bCollideActors=true
    bCollideWorld=true
    CollisionHeight=12.0
    CollisionRadius=24.0

    ShakeRotMag=(Z=250)
    ShakeRotRate=(Z=2500)
    ShakeRotTime=6
    ShakeOffsetMag=(Z=10)
    ShakeOffsetRate=(Z=200)
    ShakeOffsetTime=10
    
	bProjTarget=true
    speed=1000.0
    MaxSpeed=1000.0

    bBounce=false
    bFixedRotationDir=True
    RotationRate=(Roll=50000)
    DesiredRotation=(Roll=30000)
    
    TransientSoundVolume=1.0
    TransientSoundRadius=5000.0
}
