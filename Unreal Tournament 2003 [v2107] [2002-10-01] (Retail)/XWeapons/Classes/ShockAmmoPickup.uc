class ShockAmmoPickup extends UTAmmoPickup;

defaultproperties
{
    InventoryType=class'ShockAmmo'

    PickupMessage="You picked up a Shock Core."
    PickupSound=Sound'PickupSounds.ShockAmmoPickup'
    PickupForce="ShockAmmoPickup"  // jdf

    CollisionHeight=32.000000
    AmmoAmount=10

    StaticMesh=StaticMesh'WeaponStaticMesh.ShockAmmoPickup'
    DrawType=DT_StaticMesh
}
