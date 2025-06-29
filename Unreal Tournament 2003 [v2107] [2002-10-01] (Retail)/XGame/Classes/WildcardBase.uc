//=============================================================================
// WildcardBase
//=============================================================================
class WildcardBase extends xPickUpBase
    placeable;

// todo: add ability for LD to select which ones can and can't be spawned...
// todo: we need a mesh specifically for this...

var() class<TournamentPickup> PickupClasses[8];
var() bool bSequential;
var int NumClasses;
var int CurrentClass;
 
simulated function PostBeginPlay()
{
	if ( Role == ROLE_Authority )
	{
		NumClasses = 0;
		while (NumClasses < ArrayCount(PickupClasses) && PickupClasses[NumClasses] != None)
			NumClasses++;

		if (bSequential)
			CurrentClass = 0;
		else
			CurrentClass = Rand(NumClasses);

		PowerUp = PickupClasses[CurrentClass];
	}
	Super.PostBeginPlay();
}

function TurnOn()
{
	if (bSequential)
		CurrentClass = (CurrentClass+1)%NumClasses;
	else
		CurrentClass = Rand(NumClasses);

	PowerUp = PickupClasses[CurrentClass];

	if( myPickup != None )
		myPickup = myPickup.Transmogrify(PowerUp);
}

defaultproperties
{
	bSequential=false
    SpiralEmitter=class'XEffects.Spiral'
    
    DrawScale=1.0
    DrawType=DT_StaticMesh
    StaticMesh=XGame_rc.AmmoChargerMesh
    Texture=None
    
    PickupClasses(0)=class'XPickups.HealthPack'
    PickupClasses(1)=class'XPickups.SuperShieldPack'
    PickupClasses(2)=class'XPickups.SuperHealthPack'
    PickupClasses(3)=class'XPickups.UDamagePack'

    CollisionRadius=60.000000
    CollisionHeight=6.000000
}
