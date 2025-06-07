//=============================================================================
// LinkGunPickup.
//=============================================================================
class LinkGunPickup extends UTWeaponPickup;

static function StaticPrecache(LevelInfo L)
{
    L.AddPrecacheMaterial(Texture'XEffectMat.link_muz_green');
    L.AddPrecacheMaterial(Texture'XEffectMat.link_muzmesh_green');
    L.AddPrecacheMaterial(Texture'XEffectMat.link_ring_green');
    L.AddPrecacheMaterial(Texture'XEffectMat.link_beam_green');
	L.AddPrecacheMaterial(Texture'XEffectMat.link_spark_green');
	L.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.linkprojectile');
	L.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.LinkGunPickup');
}

simulated function UpdatePrecacheMaterials()
{
    Level.AddPrecacheMaterial(Texture'XEffectMat.link_muz_green');
    Level.AddPrecacheMaterial(Texture'XEffectMat.link_muzmesh_green');
    Level.AddPrecacheMaterial(Texture'XEffectMat.link_ring_green');
    Level.AddPrecacheMaterial(Texture'XEffectMat.link_beam_green');
	Level.AddPrecacheMaterial(Texture'XEffectMat.link_spark_green');
}

simulated function UpdatePrecacheStaticMeshes()
{
	Level.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.linkprojectile');
	Super.UpdatePrecacheStaticMeshes();
}

defaultproperties
{
    InventoryType=class'LinkGun'

    PickupMessage="You got the Link Gun."
    PickupSound=Sound'PickupSounds.LinkGunPickup'
    PickupForce="LinkGunPickup"  // jdf

	MaxDesireability=+0.7

    StaticMesh=StaticMesh'WeaponStaticMesh.LinkGunPickup'
    DrawType=DT_StaticMesh
    DrawScale=0.75
}
