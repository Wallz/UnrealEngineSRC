//=============================================================================
// The IonCannon
//=============================================================================
class IonCannon extends Actor
    placeable;

var() Sound FireSound;
var() float MinRange, MaxRange;
var() float MaxAngle;
var() Vector MarkLocation;
var() class<IonEffect> IonEffectClass;
var() int Damage; // per wave
var() float MomentumTransfer; // per wave
var() float DamageRadius; // of final wave
var() class<DamageType> DamageType;
var Vector BeamDirection;
var Vector DamageLocation;
var AvoidMarker Fear;

// camera shakes //
var() vector ShakeRotMag;           // how far to rot view
var() vector ShakeRotRate;          // how fast to rot view
var() float  ShakeRotTime;          // how much time to rot the instigator's view
var() vector ShakeOffsetMag;        // max view offset vertically
var() vector ShakeOffsetRate;       // how fast to offset view vertically
var() float  ShakeOffsetTime;       // how much time to offset view

var IonCannon nextCannon;
var bool bFirstCannon;

function PostBeginPlay()
{
	local IonCannon C;
	
    Super.PostBeginPlay();
	ForEach DynamicActors(Class'IonCannon',C)
	{
		if ( C.bFirstCannon )
			break;
	}
	
	if ( (C != None) && C.bFirstCannon )
	{
		nextCannon = C;
		C.bFirstCannon = false;
	}
	bFirstCannon = true;  
}

function bool CheckMark(Pawn Aimer, Vector TestMark, bool bFire)
{
    return false;
}

auto state Ready
{
    function bool CheckMark(Pawn Aimer, Vector TestMark, bool bFire)
    {
        local Actor Other;
        local Vector HitLocation, HitNormal, V;
        local float Dist;

        if (IsFiring())
            return false;

        Dist = VSize(Location - TestMark);
        if (Dist < MinRange || Dist > MaxRange)
            return false;

        V = Normal(TestMark - Location);
        if (V dot Vect(0,0,-1) < Cos(MaxAngle))
            return false;

        Other = Trace(HitLocation, HitNormal, TestMark+V*100.0, Location, false);

        if (Other == None || VSize(HitLocation - TestMark) > 25.0)
            return false;

        if (HitNormal.Z < 0.7)
            return false;

        DesiredRotation = Rotator(MarkLocation - Location);

        if (bFire)
        {
            Instigator = Aimer;
            MarkLocation = TestMark;
            GotoState('FireSequence');
        }

        return true;
    }
}

function RemoveFear()
{
	if ( Fear != None )
		Fear.Destroy();
}

state FireSequence
{
	function RemoveFear();
	
    function BeginState()
    {
        BeamDirection = Normal(MarkLocation - Location);
        DesiredRotation = Rotator(BeamDirection);
        DamageLocation = MarkLocation - BeamDirection * 200.0;
    }

    function SpawnEffect()
    {
        local IonEffect IonBeamEffect;

        IonBeamEffect = Spawn(IonEffectClass,,, Location, Rotation);
        if (IonBeamEffect != None)
        {
            IonBeamEffect.AimAt(MarkLocation, Vect(0,0,1));
        }
    }
    
    function ShakeView()
    {
        local Controller C;
        local PlayerController PC;
        local float Dist, Scale;

        for ( C=Level.ControllerList; C!=None; C=C.NextController )
        {
            PC = PlayerController(C);
            if ( PC != None && PC.ViewTarget != None && PC.ViewTarget.Base != None )
            {
                Dist = VSize(DamageLocation - PC.ViewTarget.Location);
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

    function EndState()
    {
		if ( Fear != None )
			Fear.Destroy();
	}

Begin:
    if ( Fear == None )
		Fear = Spawn(class'AvoidMarker',,,MarkLocation);
    Fear.SetCollisionSize(DamageRadius,200); 
    Fear.StartleBots();
    Sleep(0.5);
    SpawnEffect();
    PlaySound(FireSound);
    Sleep(0.5);

    ShakeView();
    HurtRadius(Damage, DamageRadius*0.125, DamageType, MomentumTransfer, DamageLocation);
    Sleep(0.5);
	PlaySound(sound'WeaponSounds.redeemer_explosionsound');
    HurtRadius(Damage, DamageRadius*0.300, DamageType, MomentumTransfer, DamageLocation);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.475, DamageType, MomentumTransfer, DamageLocation);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.650, DamageType, MomentumTransfer, DamageLocation);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*0.825, DamageType, MomentumTransfer, DamageLocation);
    Sleep(0.2);
    HurtRadius(Damage, DamageRadius*1.000, DamageType, MomentumTransfer, DamageLocation);
    GotoState('Ready');
}

function bool IsFiring()
{
    return IsInState('FireSequence');
}

defaultproperties
{
    Physics=PHYS_Rotating
    DrawType=DT_Mesh
    Mesh=Mesh'Weapons.IonCannon'
    bRotateToDesired=true
    RotationRate=(Pitch=15000,Yaw=15000,Roll=15000)
    bCollideActors=false
    bCollideWorld=false
    CollisionRadius=150.0
    CollisionHeight=100.0
    RemoteRole=ROLE_SimulatedProxy

    FireSound=Sound'WeaponSounds.TAGRifle.IonCannonBlast'
    MinRange=1000.0
    MaxRange=10000.0
    MaxAngle=45.0
    IonEffectClass=class'xEffects.IonEffect'
    Damage=150.0
    DamageRadius=2000.0
    MomentumTransfer=150000.0
    DamageType=class'DamTypeIonBlast'
    bStatic=false
    bStasis=false
    TransientSoundVolume=1.0
    TransientSoundRadius=2000.0

    ShakeRotMag=(Z=250)
    ShakeRotRate=(Z=2500)
    ShakeRotTime=6
    ShakeOffsetMag=(Z=10)
    ShakeOffsetRate=(Z=200)
    ShakeOffsetTime=10
}
