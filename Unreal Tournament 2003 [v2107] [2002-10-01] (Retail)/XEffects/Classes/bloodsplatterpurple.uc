class BloodSplatterPurple extends xScorch;

var texture Splats[3];

simulated function PostBeginPlay()
{
    ProjTexture = splats[Rand(3)];
    Super.PostBeginPlay();
}

defaultproperties
{
	LifeSpan=5
	splats(0)=Material'BloodFX.BloodSplat1P' 
	splats(1)=Material'BloodFX.BloodSplat2P'
	splats(2)=Material'BloodFX.BloodSplat3P'
	ProjTexture=Material'BloodFX.BloodSplat1P'
	RemoteRole=ROLE_None
    FOV=6
    bClipStaticMesh=True
}
