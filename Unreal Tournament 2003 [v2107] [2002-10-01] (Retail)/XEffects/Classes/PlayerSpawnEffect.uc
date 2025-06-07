class PlayerSpawnEffect extends Emitter;

/* misnamed, as this is actually the pickup respawn effect
*/
simulated function PostBeginPlay()
{
	Super.PostBeginPlay();
	PlaySound(sound'WeaponSounds.item_respawn');
}

defaultproperties
{
    Begin Object Class=SpriteEmitter Name=SpriteEmitter1
        UseColorScale=True
        ColorScale(0)=(Color=(B=255,G=255,R=255))
        ColorScale(1)=(RelativeTime=0.200000,Color=(B=0,G=170,R=255))
        ColorScale(2)=(RelativeTime=1.000000,Color=(B=0,G=120,R=255))
        FadeOutStartTime=0.800000
        FadeOut=True
        MaxParticles=50
        RespawnDeadParticles=False
        StartLocationShape=PTLS_Sphere
        SphereRadiusRange=(Min=16.000000,Max=16.000000)
        MeshSpawningStaticMesh=StaticMesh'ParticleMeshes.Simple.ParticleSphere3'
        MeshSpawning=PTMS_Random
        MeshScaleRange=(X=(Min=0.750000,Max=0.750000),Y=(Min=0.750000,Max=0.750000),Z=(Min=0.750000,Max=0.750000))
        UniformMeshScale=False
        RevolutionsPerSecondRange=(Z=(Min=0.200000,Max=0.200000))
        UseSizeScale=True
        UseRegularSizeScale=False
        SizeScale(0)=(RelativeSize=1.000000)
        SizeScale(1)=(RelativeTime=1.000000,RelativeSize=2.000000)
        StartSizeRange=(X=(Min=10.000000,Max=10.000000),Y=(Min=10.000000,Max=10.000000),Z=(Min=10.000000,Max=10.000000))
        UniformSize=True
        InitialParticlesPerSecond=300.000000
        AutomaticInitialSpawning=False
        Texture=Texture'EpicParticles.Flares.Sharpstreaks2'
        LifetimeRange=(Min=1.000000,Max=1.000000)
        StartVelocityRadialRange=(Min=100.000000,Max=100.000000)
        VelocityLossRange=(X=(Min=0.100000,Max=0.100000),Y=(Min=0.100000,Max=0.100000),Z=(Min=0.600000,Max=0.600000))
        GetVelocityDirectionFrom=PTVD_AddRadial
        SecondsBeforeInactive=0
        Name="SpriteEmitter1"
    End Object
    Emitters(0)=SpriteEmitter'SpriteEmitter1'
    bNoDelete=false
    AutoDestroy=true
    RemoteRole=ROLE_DumbProxy
    bNetTemporary=true
}