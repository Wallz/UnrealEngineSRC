//=============================================================================
// AssaultRiflePickup.
//=============================================================================
class AssaultRiflePickup extends UTWeaponPickup;

defaultproperties
{
    InventoryType=class'AssaultRifle'

    PickupMessage="You got the Assault Rifle."
    PickupSound=Sound'PickupSounds.AssaultRiflePickup'
    PickupForce="AssaultRiflePickup"  // jdf

	MaxDesireability=+0.4

    StaticMesh=StaticMesh'WeaponStaticMesh.AssaultRiflePickup'
    DrawType=DT_StaticMesh
    DrawScale=0.75
}
