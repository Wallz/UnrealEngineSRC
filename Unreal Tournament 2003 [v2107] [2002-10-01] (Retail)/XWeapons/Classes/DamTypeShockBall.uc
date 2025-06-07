class DamTypeShockBall extends WeaponDamageType
	abstract;

static function GetHitEffects(out class<xEmitter> HitEffects[4], int VictemHealth )
{
    HitEffects[0] = class'HitSmoke';
}

defaultproperties
{
    DeathString="%o was wasted by %k's shock core."
	MaleSuicide="%o snuffed himself with the shock core."
	FemaleSuicide="%o snuffed herself with the shock core."

    WeaponClass=class'ShockRifle'
    bDetonatesGoop=true
}

