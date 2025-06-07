#exec OBJ LOAD FILE=EpicParticles.utx

class DeResPart extends Emitter;

defaultproperties
{
    Begin Object Class=SpriteEmitter Name=SpriteEmitter0
        Acceleration=(Z=-40.000000)
        UseColorScale=True
        ColorScale(0)=(Color=(B=19,G=255,R=6))
        ColorScale(1)=(RelativeTime=0.700000,Color=(G=190,R=190))
        ColorScale(2)=(RelativeTime=1.000000,Color=(G=190,R=190))
        ColorMultiplierRange=(Z=(Min=0.500000,Max=0.500000))
        FadeOutStartTime=0.700000
        FadeOut=True
		CoordinateSystem=PTCS_Independent
        MaxParticles=200
        StartLocationRange=(X=(Min=-8.000000,Max=8.000000),Y=(Min=-8.000000,Max=8.000000),Z=(Min=-8.000000,Max=8.000000))
        UseRotationFrom=PTRS_Actor
        UseSizeScale=True
        UseRegularSizeScale=False
        SizeScale(0)=(RelativeSize=1.700000)
        SizeScale(1)=(RelativeTime=1.000000)
        StartSizeRange=(X=(Min=0.750000,Max=1.250000),Y=(Min=0.750000,Max=1.250000))
        UniformSize=True
        UseSkeletalLocationAs=PTSU_SpawnOffset
        SkeletalScale=(X=0.380000,Y=0.380000,Z=0.380000)
        Texture=Texture'EpicParticles.Spots.DeRez01aw'
        LifetimeRange=(Min=0.900000,Max=0.900000)
		SecondsBeforeInactive=0
        Name="SpriteEmitter0"
    End Object
    Emitters(0)=SpriteEmitter'SpriteEmitter0'

	bHighDetail=true
	bHardAttach=true
    bNoDelete=false
    RemoteRole=ROLE_None
    bNetTemporary=true
	bHidden=false
	AutoDestroy=true
}