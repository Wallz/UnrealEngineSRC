class DamTypeLinkPlasma extends WeaponDamageType
	abstract;

static function GetHitEffects(out class<xEmitter> HitEffects[4], int VictemHealth )
{
    HitEffects[0] = class'HitSmoke';
}

defaultproperties
{
    DeathString="%o was served an extra helping of %k's plasma."
	MaleSuicide="%o fried himself with his own plasma blast."
	FemaleSuicide="%o fried herself with her own plasma blast."

    WeaponClass=class'LinkGun'

    bDetonatesGoop=true
}

