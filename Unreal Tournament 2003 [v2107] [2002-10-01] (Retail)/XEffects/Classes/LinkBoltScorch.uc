class LinkBoltScorch extends xScorch;

#exec TEXTURE IMPORT NAME=LBScorcht FILE=TEXTURES\DECALS\LinkBoltDecal.tga LODSET=2 Alpha=1 UCLAMPMODE=CLAMP VCLAMPMODE=CLAMP

defaultproperties
{
	DrawScale=+0.5
	ProjTexture=Material'rocketblastmark'
    LifeSpan=2
}
