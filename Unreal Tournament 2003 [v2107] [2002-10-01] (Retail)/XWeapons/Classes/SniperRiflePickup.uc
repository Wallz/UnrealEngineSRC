//=============================================================================
// SniperRiflePickup.
//=============================================================================
class SniperRiflePickup extends UTWeaponPickup;

#exec OBJ LOAD FILE=WeaponSkins.utx

static function StaticPrecache(LevelInfo L)
{
    L.AddPrecacheMaterial(Material'XGameShaders.WeaponEnvShader');
    L.AddPrecacheMaterial(Texture'SniperBorder');
    L.AddPrecacheMaterial(Texture'SniperFocus');
	L.AddPrecacheMaterial(Texture'SniperArrows');	
	L.AddPrecacheMaterial(Texture'Engine.WhiteTexture');
    L.AddPrecacheMaterial(Texture'WeaponSkins.SniperScreen1');
    L.AddPrecacheMaterial(Texture'WeaponSkins.SniperScreen1Pan');
    L.AddPrecacheMaterial(Texture'WeaponSkins.SniperScreen2');
    L.AddPrecacheMaterial(Texture'XEffects.LightningChargeT');
    L.AddPrecacheMaterial(Texture'XEffects.pcl_BlueSpark');
    L.AddPrecacheMaterial(Texture'XEffects.LightningBoltT');
    L.AddPrecacheMaterial(Material'XGameShaders.fulloverlay');
    L.AddPrecacheMaterial(Material'XGameShaders.scanline');
	L.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.SniperRiflePickup');
}

simulated function UpdatePrecacheMaterials()
{
    Level.AddPrecacheMaterial(Material'XGameShaders.WeaponEnvShader');
    Level.AddPrecacheMaterial(Texture'SniperBorder');
    Level.AddPrecacheMaterial(Texture'SniperFocus');
	Level.AddPrecacheMaterial(Texture'SniperArrows');	
	Level.AddPrecacheMaterial(Texture'Engine.WhiteTexture');
    Level.AddPrecacheMaterial(Texture'WeaponSkins.SniperScreen1');
    Level.AddPrecacheMaterial(Texture'WeaponSkins.SniperScreen1Pan');
    Level.AddPrecacheMaterial(Texture'WeaponSkins.SniperScreen2');
    Level.AddPrecacheMaterial(Texture'XEffects.LightningChargeT');
    Level.AddPrecacheMaterial(Texture'XEffects.pcl_BlueSpark');
    Level.AddPrecacheMaterial(Texture'XEffects.LightningBoltT');
    Level.AddPrecacheMaterial(Material'XGameShaders.fulloverlay');
    Level.AddPrecacheMaterial(Material'XGameShaders.scanline');
}

defaultproperties
{
    InventoryType=class'SniperRifle'

    PickupMessage="You got the Lightning Gun."
    PickupSound=Sound'PickupSounds.SniperRiflePickup'
    PickupForce="SniperRiflePickup"  // jdf

	MaxDesireability=+0.65

    StaticMesh=StaticMesh'WeaponStaticMesh.SniperRiflePickup'
    DrawType=DT_StaticMesh
    DrawScale=0.75
}
