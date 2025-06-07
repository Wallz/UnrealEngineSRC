class ShockComboSphereDark extends MeshEffect;

#exec OBJ LOAD FILE=XEffectMat.utx

event PostBeginPlay()
{
    Super.PostBeginPlay();
    //PlayAnim('Explode', 1, 0);
    SetRotation(RotRand());
}

defaultproperties
{
    //DrawType=DT_Mesh
    //Mesh=VertMesh'ShockSphereMesh'
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'Editor.TexPropSphere'
    Skins(0)=FinalBlend'XEffectMat.ShockDarkFB'
    bUnlit=true
    //bAlwaysFaceCamera=true
    //DrawScale3D=(X=5.5,Y=5.5,Z=5.5)

    LifeTime=1.0
    FadeInterp=(InTime=0.15,InStyle=IS_Linear,OutTime=0.75,OutStyle=IS_Linear)
    ScaleInterp=(Start=0.1,Mid=0.8,End=0.0,InTime=0.3,InStyle=IS_InvExp,OutTime=0.3,OutStyle=IS_InvExp)
}
