#exec OBJ LOAD FILE=EpicParticles.utx

class RedeemerPickup extends UTWeaponPickup;

function PrebeginPlay()
{
	Super.PreBeginPlay();
	if ( Level.IsDemoBuild() )
		Destroy();
}

function SetWeaponStay()
{
	bWeaponStay = false;
}

function float GetRespawnTime()
{
	return ReSpawnTime;
}

static function StaticPrecache(LevelInfo L)
{
	L.AddPrecacheMaterial(Material'EpicParticles.Smokepuff2');
	L.AddPrecacheMaterial(Material'EpicParticles.IonBurn');
	L.AddPrecacheMaterial(Material'EpicParticles.IonWave');
	L.AddPrecacheMaterial(Material'EpicParticles.BurnFlare1');
	L.AddPrecacheMaterial(Material'EpicParticles.WhiteStreak01aw');
	L.AddPrecacheMaterial(Material'EpicParticles.Smokepuff');
	L.AddPrecacheMaterial(Material'EpicParticles.SoftFlare');
	L.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.RedeemerPickup');
}

simulated function UpdatePrecacheMaterials()
{
	Level.AddPrecacheMaterial(Material'EpicParticles.Smokepuff2');
	Level.AddPrecacheMaterial(Material'EpicParticles.IonBurn');
	Level.AddPrecacheMaterial(Material'EpicParticles.IonWave');
	Level.AddPrecacheMaterial(Material'EpicParticles.BurnFlare1');
	Level.AddPrecacheMaterial(Material'EpicParticles.WhiteStreak01aw');
	Level.AddPrecacheMaterial(Material'EpicParticles.Smokepuff');
	Level.AddPrecacheMaterial(Material'EpicParticles.SoftFlare');
}

defaultproperties
{
    InventoryType=class'Redeemer'

    PickupMessage="You got the Redeemer."
    PickupSound=Sound'PickupSounds.FlakCannonPickup'
    PickupForce="FlakCannonPickup"

	MaxDesireability=+1.0

    StaticMesh=StaticMesh'WeaponStaticMesh.RedeemerPickup'
    DrawType=DT_StaticMesh
    DrawScale=1.0
    
    RespawnTime=120.0
    bWeaponStay=false
}
