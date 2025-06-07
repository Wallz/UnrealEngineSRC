class DamTypeAssaultBullet extends WeaponDamageType
	abstract;

defaultproperties
{
    DeathString="%o was ventilated by %k's assault rifle."
	MaleSuicide="%o assaulted himself."
	FemaleSuicide="%o assaulted herself."

    WeaponClass=class'AssaultRifle'
	KDamageImpulse=2000
}

