//=============================================================================
// Copyright 2001 Digital Extremes - All Rights Reserved.
// Confidential.
//=============================================================================
class xTeamGame extends TeamGame;

#exec OBJ LOAD FILE=TeamSymbols.utx				// needed right now for Link symbols, etc.

static function PrecacheGameTextures(LevelInfo myLevel)
{
	class'xDeathMatch'.static.PrecacheGameTextures(myLevel);

	myLevel.AddPrecacheMaterial(Material'TeamSymbols.TeamBeaconT');
	myLevel.AddPrecacheMaterial(Material'TeamSymbols.LinkBeaconT');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.RedShell');
	myLevel.AddPrecacheMaterial(Material'XEffectMat.BlueShell');

}

static function PrecacheGameStaticMeshes(LevelInfo myLevel)
{
	class'xDeathMatch'.static.PrecacheGameStaticMeshes(myLevel);
}

defaultproperties
{
    MapListType="XInterface.MapListTeamDeathMatch"
    HUDType="XInterface.HudBTeamDeathMatch"
    DeathMessageClass=class'XGame.xDeathMessage'
	DefaultEnemyRosterClass="xGame.xTeamRoster"

    ScreenShotName="MapThumbnails.ShotTeamGame"
    DecoTextName="XGame.TeamGame"

    Acronym="TDM"
}