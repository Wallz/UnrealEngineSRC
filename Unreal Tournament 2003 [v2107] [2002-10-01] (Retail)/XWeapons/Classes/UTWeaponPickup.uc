class UTWeaponPickup extends WeaponPickup;

function RespawnEffect()
{
	spawn(class'PlayerSpawnEffect');
}

defaultproperties
{
    MessageClass=class'PickupMessagePlus'
}