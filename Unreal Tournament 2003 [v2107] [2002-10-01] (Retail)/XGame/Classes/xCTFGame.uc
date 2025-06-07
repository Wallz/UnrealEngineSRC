//=============================================================================
// xCTFGame.
//=============================================================================
class xCTFGame extends CTFGame
    config;

#exec OBJ LOAD FILE=GameSounds.uax	
#exec OBJ LOAD File=XGameShadersB.utx

static function PrecacheGameTextures(LevelInfo myLevel)
{
	class'xTeamGame'.static.PrecacheGameTextures(myLevel);
}

static function PrecacheGameStaticMeshes(LevelInfo myLevel)
{
	class'xDeathMatch'.static.PrecacheGameStaticMeshes(myLevel);
}

defaultproperties
{
    MapListType="XInterface.MapListCaptureTheFlag"
    DeathMessageClass=class'XGame.xDeathMessage'
    HUDType="XInterface.HudBCaptureTheFlag"
	DefaultEnemyRosterClass="xGame.xTeamRoster"

    ScreenShotName="MapThumbnails.ShotCTFGame"
    DecoTextName="XGame.CTFGame"

    Acronym="CTF"
	OtherMesgGroup="CTFGame"
}
