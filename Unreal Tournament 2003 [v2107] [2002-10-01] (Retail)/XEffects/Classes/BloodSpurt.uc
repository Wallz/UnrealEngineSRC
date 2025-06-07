//=============================================================================
// BloodSpurt.
//=============================================================================
class BloodSpurt extends xEmitter;

#exec TEXTURE IMPORT NAME=pcl_Blooda FILE=TEXTURES\Blooda.tga GROUP=Skins Alpha=1  DXT=5
var Class<Actor>    BloodDecalClass;
var texture Splats[3];

simulated function PostNetBeginPlay()
{
	if ( Level.NetMode != NM_DedicatedServer )
		WallSplat();
}

simulated function WallSplat()
{
	local vector WallHit, WallNormal;
	local Actor WallActor;

	if ( (FRand() > 0.8) || (BloodDecalClass == None) )
		return;
	WallActor = Trace(WallHit, WallNormal, Location + 200 * vector(Rotation), Location, false);
	if ( WallActor != None )	
		spawn(BloodDecalClass,,,WallHit + 20 * (WallNormal + VRand()), rotator(-WallNormal));
}

static function PrecacheContent(LevelInfo Level)
{
	local int i;
	
	Super.PrecacheContent(Level);
	if ( Default.BloodDecalClass != None )
	{
		for ( i=0; i<3; i++ )
			Level.AddPrecacheMaterial(Default.splats[i]);
	}	
}

defaultproperties  
{
	splats(0)=Material'BloodFX.BloodSplat1' 
	splats(1)=Material'BloodFX.BloodSplat2'
	splats(2)=Material'BloodFX.BloodSplat3'

    BloodDecalClass=class'BloodSplatter'
    Style=STY_Alpha
    mParticleType=PT_Sprite
    mDirDev=(X=0.1000000,Y=0.100000,Z=0.100000) 
    mPosDev=(X=0.00000,Y=0.00000,Z=0.00000)
    mDelayRange(0)=0.000000 
    mDelayRange(1)=0.00000
    mLifeRange(0)=1.00000 
    mLifeRange(1)=2.000000
    mSpeedRange(0)=0.000000 
    mSpeedRange(1)=85.000000 
    mSizeRange(0)=5.500000 
    mSizeRange(1)=9.500000
    mMassRange(0)=0.02000
    mMassRange(1)=0.040000
    mGrowthRate=3.0
    mRegenRange(0)=0.000000
    mRegenRange(1)=0.000000
    mRegenDist=0.000000
    mMaxParticles=13
    mStartParticles=13
    DrawScale=1.000000
    ScaleGlow=1.000000
    mAirResistance=0.6
    mAttenuate=True
    mRegen=False
    Skins(0)=Texture'pcl_Blooda'
    CollisionRadius=0.000000
    CollisionHeight=0.000000
    mColorRange(0)=(R=255,G=255,B=255,A=255)
    mColorRange(1)=(R=255,G=255,B=255,A=255)
    mCollision=False
    mRandOrient=True
    bForceAffected=False
    mRandTextures=True
    mNumTileColumns=4
    mNumTileRows=4
    bUnlit=True
    LifeSpan=3.5
    bNetTemporary=True
}