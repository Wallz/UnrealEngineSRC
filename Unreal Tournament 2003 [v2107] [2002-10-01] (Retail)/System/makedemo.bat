@if NOT A%1==A cd %1

@IF EXIST ..\CutdownMaps rd ..\CutdownMaps /s /q
@IF EXIST ..\CutdownTextures rd ..\CutdownTextures /s /q
@IF EXIST ..\CutdownStaticMeshes rd ..\CutdownStaticMeshes /s /q
@IF EXIST ..\CutdownSounds rd ..\CutdownSounds /s /q

@mkdir ..\CutdownMaps
@mkdir ..\CutdownTextures
@mkdir ..\CutdownStaticMeshes
@mkdir ..\CutdownSounds

copy ..\Textures\MapThumbnails.utx ..\CutdownTextures
copy ..\Textures\DemoPlayerSkins.utx ..\CutdownTextures\PlayerSkins.utx
copy ..\Textures\InterfaceContent.utx ..\CutdownTextures
copy ..\Textures\new_TeamSymbols.utx ..\CutdownTextures
copy ..\Textures\DemoPlayerPictures.utx ..\CutdownTextures\PlayerPictures.utx 
copy ..\Textures\TeamSymbols_UT2003.utx ..\CutdownTextures

ucc Editor.CutdownContent Entry.ut2 NVidiaLogo.ut2 DM-Antalus.ut2 CTF-Citadel.ut2 DM-Asbestos.ut2 BR-Anubis.ut2
ucc Editor.TextureStrip ..\CutdownTextures\*.utx

ucc Editor.StripSource Core.u
ucc Editor.StripSource Editor.u
ucc Editor.StripSource Engine.u
ucc Editor.StripSource Fire.u
ucc Editor.StripSource GamePlay.u
ucc Editor.StripSource IpDrv.u
ucc Editor.StripSource UnrealEd.u
ucc Editor.StripSource UnrealGame.u
ucc Editor.StripSource UWeb.u
ucc Editor.StripSource Vehicles.u
ucc Editor.StripSource XAdmin.u
ucc Editor.StripSource XEffects.u
ucc Editor.StripSource XGame.u
ucc Editor.StripSource XGame_rc.u
ucc Editor.StripSource XInterface.u
ucc Editor.StripSource XPickups.u
ucc Editor.StripSource XPickups_rc.u
ucc Editor.StripSource XWeapons.u
ucc Editor.StripSource XWeapons_rc.u
ucc Editor.StripSource XWebAdmin.u

copy ..\Textures\UT2003Fonts.utx ..\CutdownTextures\UT2003Fonts.utx /y

ucc master SetupUT2003_Demo.ini

@rd ..\CutdownMaps /s /q
@rd ..\CutdownTextures /s /q
@rd ..\CutdownStaticMeshes /s /q
@rd ..\CutdownSounds /s /q
