class LinkAmmoPickup extends UTAmmoPickup;

defaultproperties
{
    InventoryType=class'LinkAmmo'

    PickupMessage="You picked up link charges."
    PickupSound=Sound'PickupSounds.LinkAmmoPickup'
    PickupForce="LinkAmmoPickup"  // jdf

    AmmoAmount=30

    CollisionHeight=10.500000
    MaxDesireability=0.240000

    StaticMesh=StaticMesh'WeaponStaticMesh.LinkAmmoPickup'
    DrawType=DT_StaticMesh
}
