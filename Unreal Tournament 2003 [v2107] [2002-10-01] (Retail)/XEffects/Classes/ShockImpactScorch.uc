class ShockImpactScorch extends xScorch;

#exec OBJ LOAD FILE=XEffectMat.utx

defaultproperties
{
    FrameBufferBlendingOp=PB_Add
	DrawScale=+0.5
	ProjTexture=Texture'XEffectMat.Shock.shock_mark_heat'
    LifeSpan=2.2
}
