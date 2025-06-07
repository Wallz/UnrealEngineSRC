@if NOT A%1==A cd %1

@IF EXIST ..\CutdownMaps rd ..\CutdownMaps /s /q
@IF EXIST ..\CutdownTextures rd ..\CutdownTextures /s /q
@IF EXIST ..\CutdownStaticMeshes rd ..\CutdownStaticMeshes /s /q
@IF EXIST ..\CutdownSounds rd ..\CutdownSounds /s /q

@mkdir ..\CutdownMaps
@mkdir ..\CutdownTextures
@mkdir ..\CutdownStaticMeshes
@mkdir ..\CutdownSounds

copy ..\sounds\AnnouncerFemale2k4.uax ..\CutdownSounds
copy ..\sounds\AnnouncerMale2k4.uax ..\CutdownSounds
copy ..\sounds\AnnouncerAssault.uax ..\CutdownSounds
copy ..\sounds\tauntpack.uax ..\CutdownSounds

copy ..\Textures\UT2004Thumbnails.utx ..\CutdownTextures
copy ..\Textures\DemoPlayerSkins.utx ..\CutdownTextures
copy ..\Textures\InterfaceContent.utx ..\CutdownTextures
copy ..\Textures\HudContent.utx ..\CutdownTextures
copy ..\Textures\TeamSymbols_UT2003.utx ..\CutdownTextures
copy ..\Textures\InstaGibEffects.utx ..\CutdownTextures
copy ..\Textures\LastManStanding.utx ..\CutdownTextures
copy ..\Textures\ServerIcons.utx ..\CutdownTextures
copy ..\Textures\2k4Menus.utx ..\CutdownTextures
copy ..\Textures\XGameShaders.utx ..\CutdownTextures
copy ..\Textures\XGameShaders2004.utx ..\CutdownTextures
copy ..\Textures\ONSInterface-TX.utx ..\CutdownTextures

ucc Editor.CutdownContent Entry.ut2 NVidiaLogo.ut2 NoIntro.ut2 ONS-Torlan.ut2 ONS-Primeval.ut2 DM-Rankin.ut2 DM-1on1-Idoma.ut2 DM-Deck17.ut2 AS-Convoy.ut2 CTF-FaceClassic.ut2 CTF-BridgeOfFate.UT2 BR-Colossus.ut2 CTF-TwinTombs.ut2 
ucc Editor.TextureStrip ..\CutdownTextures\*.utx

copy ..\Textures\UT2003Fonts.utx ..\CutdownTextures\UT2003Fonts.utx /y
copy ..\Textures\2K4Fonts.utx ..\CutdownTextures\2K4Fonts.utx /y

ucc master SetupUT2004Demo.ini

rem @rd ..\CutdownMaps /s /q
rem @rd ..\CutdownTextures /s /q
rem @rd ..\CutdownStaticMeshes /s /q
rem @rd ..\CutdownSounds /s /q
