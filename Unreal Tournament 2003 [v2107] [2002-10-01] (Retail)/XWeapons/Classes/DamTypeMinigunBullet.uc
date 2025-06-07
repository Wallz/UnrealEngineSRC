class DamTypeMinigunBullet extends WeaponDamageType
	abstract;

defaultproperties
{
    DeathString="%o was mowed down by %k's minigun."
	MaleSuicide="%o turned the minigun on himself."
	FemaleSuicide="%o turned the minigun on herself."

    WeaponClass=class'Minigun'
    KDamageImpulse=2000
}

