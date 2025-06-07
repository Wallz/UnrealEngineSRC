class KeyInventory extends Inventory;

var KeyPickup MyPickup;

function UnLock(LockedObjective O)
{
	O.DisableObjective(Pawn(Owner));
}

function Destroyed()
{
	MyPickup.GotoState('Pickup');
	Super.Destroyed();
}