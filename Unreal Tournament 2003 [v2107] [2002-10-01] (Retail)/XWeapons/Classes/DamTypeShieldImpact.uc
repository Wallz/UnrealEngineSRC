class DamTypeShieldImpact extends WeaponDamageType
	abstract;

defaultproperties
{
    DeathString="%o was pulverized by %k's shield cannon."
	MaleSuicide="%o threw his weight around once too often."
	FemaleSuicide="%o threw her weight around once too often."

    WeaponClass=class'ShieldGun'
    bDetonatesGoop=true

	bKUseOwnDeathVel=true
	KDeathVel=450
	KDeathUpKick=300
}

