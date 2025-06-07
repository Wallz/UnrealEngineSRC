class DamTypeShockBeam extends WeaponDamageType
	abstract;

static function GetHitEffects(out class<xEmitter> HitEffects[4], int VictemHealth )
{
    HitEffects[0] = class'HitSmoke';
}

defaultproperties
{
    DeathString="%o was fatally enlightened by %k's shock beam."
	MaleSuicide="%o somehow managed to shoot himself with the shock rifle."
	FemaleSuicide="%o somehow managed to shoot herself with the shock rifle."

    DamageOverlayMaterial=Material'XGameShaders.PlayerShaders.LightningHit'
    DamageOverlayTime=1.0

    WeaponClass=class'ShockRifle'

    GibPerterbation=0.75
    bDetonatesGoop=true
}

