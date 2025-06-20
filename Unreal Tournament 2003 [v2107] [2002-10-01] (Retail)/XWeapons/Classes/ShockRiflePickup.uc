//=============================================================================
// ShockRiflePickup
//=============================================================================
class ShockRiflePickup extends UTWeaponPickup;


static function StaticPrecache(LevelInfo L)
{
    L.AddPrecacheMaterial(Material'WeaponSkins.lasermist');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_mark_heat');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_flash');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_flare_a');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_core');
    L.AddPrecacheMaterial(Material'XEffectMat.purple_line');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_sparkle');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_core_low');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_Energy_green_faded');
    L.AddPrecacheMaterial(Material'XEffectMat.Shock_Elec_a');
    L.AddPrecacheMaterial(Material'XEffectMat.shock_gradient_b');
    L.AddPrecacheMaterial(Material'XEffectMat.Shock_ring_a');
    L.AddPrecacheMaterial(Material'XEffectMat.ShockComboFlash');
    L.AddPrecacheMaterial(Material'XGameShaders.shock_muzflash_1st');
    L.AddPrecacheMaterial(Material'XWeapons_rc.ShockBeamTex');
    L.AddPrecacheMaterial(Material'XEffects.SaDScorcht');
    L.AddPrecacheMaterial(Material'WeaponSkins.ShockTex0');
	L.AddPrecacheStaticMesh(StaticMesh'Editor.TexPropSphere');
	L.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.ShockRiflePickup');
}

simulated function UpdatePrecacheMaterials()
{
    Level.AddPrecacheMaterial(Material'WeaponSkins.lasermist');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_mark_heat');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_flash');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_flare_a');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_core');
    Level.AddPrecacheMaterial(Material'XEffectMat.purple_line');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_sparkle');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_core_low');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_Energy_green_faded');
    Level.AddPrecacheMaterial(Material'XEffectMat.Shock_Elec_a');
    Level.AddPrecacheMaterial(Material'XEffectMat.shock_gradient_b');
    Level.AddPrecacheMaterial(Material'XEffectMat.Shock_ring_a');
    Level.AddPrecacheMaterial(Material'XEffectMat.ShockComboFlash');
    Level.AddPrecacheMaterial(Material'XGameShaders.shock_muzflash_1st');
    Level.AddPrecacheMaterial(Material'XWeapons_rc.ShockBeamTex');
    Level.AddPrecacheMaterial(Material'XEffects.SaDScorcht');
    Level.AddPrecacheMaterial(Material'WeaponSkins.ShockTex0');
}

simulated function UpdatePrecacheStaticMeshes()
{
	Level.AddPrecacheStaticMesh(StaticMesh'Editor.TexPropSphere');
	Super.UpdatePrecacheStaticMeshes();
}

defaultproperties
{ 
    InventoryType=class'ShockRifle'

    PickupMessage="You got the Shock Rifle."
    PickupSound=Sound'PickupSounds.ShockRiflePickup'
    PickupForce="ShockRiflePickup"  // jdf

	MaxDesireability=+0.63

    StaticMesh=StaticMesh'WeaponStaticMesh.ShockRiflePickup'
    DrawType=DT_StaticMesh
    DrawScale=0.75
}
