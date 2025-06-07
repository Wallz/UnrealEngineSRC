//=============================================================================
// Minigun.
//=============================================================================
class MinigunPickup extends UTWeaponPickup;

static function StaticPrecache(LevelInfo L)
{
    L.AddPrecacheMaterial(Texture'XEffects.TracerT');
    L.AddPrecacheMaterial(Texture'XEffects.ShellCasingTex');
    L.AddPrecacheMaterial(Texture'XGameShaders.MinigunFlash');
	L.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.MinigunPickup');
}

simulated function UpdatePrecacheMaterials()
{
    Level.AddPrecacheMaterial(Texture'XEffects.TracerT');
    Level.AddPrecacheMaterial(Texture'XEffects.ShellCasingTex');
    Level.AddPrecacheMaterial(Texture'XGameShaders.MinigunFlash');
}

defaultproperties
{
    InventoryType=class'Minigun'

    PickupMessage="You got the Minigun."
    PickupSound=Sound'PickupSounds.MinigunPickup'
    PickupForce="MinigunPickup"  // jdf

	MaxDesireability=+0.73

    StaticMesh=StaticMesh'WeaponStaticMesh.MinigunPickup'
    DrawType=DT_StaticMesh
    DrawScale=0.75
}
