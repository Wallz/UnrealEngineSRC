//=============================================================================
// xDeathMatch.
//=============================================================================
class xDeathMatch extends DeathMatch;

#exec OBJ LOAD FILE=WeaponSkins.utx
#exec OBJ LOAD FILE=XEffectMat.utx
#exec OBJ LOAD FILE=WeaponStaticMesh.usx

static function PrecacheGameTextures(LevelInfo myLevel)
{
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.AssaultTex0');
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.GrenadeEndTex');
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.GrenadeTex');
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.ShieldTex0');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.TransRingEnergy');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.Minigun_burst');
	myLevel.AddPrecacheMaterial(Material'XEffects.pcl_Spark');
	myLevel.AddPrecacheMaterial(Material'XEffects.EmitSmoke_t');
	myLevel.AddPrecacheMaterial(Material'XEffects.SmokeTex');
	myLevel.AddPrecacheMaterial(Material'XEffects.rocketblastmark');
	myLevel.AddPrecacheMaterial(Material'XEffects.Rexpt');
	myLevel.AddPrecacheMaterial(Material'XEffects.SmokeAlphab_t');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.shock_ring_b');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.shield_elec_green_a');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.SlimeSkin');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.goop_green_a');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.edgegrad');
	myLevel.AddPrecacheMaterial(Material'XGameShadersB.TransRingEnergyRed');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.PlayerShield');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.Shield3rdFB');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.ShieldRip3rdFB');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.LinkGunShell');
	myLevel.AddPrecacheMaterial(Material'Engine.BlobTexture');
	myLevel.AddPrecacheMaterial(Material'EpicParticles.Sharpstreaks2');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.LEnergy');
	
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.TransLauncherTex0');
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.BioRifleGlass0');
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.TransBeaconTex0');
	myLevel.AddPrecacheMaterial(Material'XEffects.TransTrailT');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.TransPlayerCell');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.TransPlayerCellRed');
	myLevel.AddPrecacheMaterial(Material'XEffects.GreySpark');
}

static function PrecacheGameStaticMeshes(LevelInfo myLevel)
{
	myLevel.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.shield');
	myLevel.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.grenademesh');
	myLevel.AddPrecacheStaticMesh(StaticMesh'WeaponStaticMesh.AssaultRiflePickup');
}
	
defaultproperties
{
    MapListType="XInterface.MapListDeathMatch"
    HUDType="XInterface.HudBDeathMatch"
	DeathMessageClass=class'XGame.xDeathMessage'

    ScreenShotName="MapThumbnails.ShotDeathmatch"
    DecoTextName="XGame.Deathmatch"

    Acronym="DM"
    DefaultEnemyRosterClass="XGame.xDMRoster"
}
