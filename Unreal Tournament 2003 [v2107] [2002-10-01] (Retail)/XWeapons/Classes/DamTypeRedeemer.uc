class DamTypeRedeemer extends WeaponDamageType
	abstract;

defaultproperties
{
    DeathString="%o was PULVERIZED!"
	MaleSuicide="%o was PULVERIZED!"
	FemaleSuicide="%o was PULVERIZED!"

    WeaponClass=class'Redeemer'
    bSuperWeapon=true
    bArmorStops=false

	bKUseOwnDeathVel=true
	KDeathVel=600
	KDeathUpKick=600
}
