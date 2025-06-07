class LockedObjective extends GameObjective;

var() name KeyTag;	// tag of key which unlocks this objective
var KeyPickup MyKey;

function FindKey()
{
	if ( (MyKey != None) && !MyKey.bDeleteMe )
		return;

	MyKey = None;

	ForEach AllActors(class'KeyPickup',MyKey,KeyTag)
		break;
}

/* Reset() 
reset actor to initial state - used when restarting level without reloading.
*/
function Reset()
{
	Super.Reset();
	SetCollision(true, false, false);
}

/* TellBotHowToDisable()
tell bot what to do to disable me.
return true if valid/useable instructions were given
*/
function bool TellBotHowToDisable(Bot B)
{
	local KeyInventory K;
	local Controller C;

	K = KeyInventory(B.Pawn.FindInventoryType(class'KeyInventory'));

	// if bot has key, tell bot to come find me
	if ( (K != None) && (K.Tag == KeyTag) )
		return Super.TellBotHowToDisable(B);

	// does other player on bot's team have key - if so follow him
	for ( C=Level.ControllerList; C!=None; C=C.NextController )
		if ( (C.PlayerReplicationInfo != None) && (C.Pawn != None)
			&& (C.PlayerReplicationInfo.Team == B.PlayerReplicationInfo.Team) )
		{
			K = KeyInventory(C.Pawn.FindInventoryType(class'KeyInventory'));
			if ( (K != None) && (K.Tag == KeyTag) )
			{
				B.Squad.TellBotToFollow(B,C);
				return true;
			}
		}

	// if not find key
	FindKey();
	if ( MyKey == None )
		return false;

	if ( B.ActorReachable(MyKey) )
	{
		B.GoalString = "almost at "$MyKey;
		B.MoveTarget = MyKey;
		B.SetAttractionState();
		return true;
	}

	B.GoalString = "No path to key "$MyKey;
	if ( !B.FindBestPathToward(MyKey,false,true) )
		return false;
	B.GoalString = "Follow path to "$MyKey;
	B.SetAttractionState();
	return true;
}

function DisableObjective(Pawn Instigator)
{
	Super.DisableObjective(Instigator);
	SetCollision(false, false, false);
}

function Touch(Actor Other)
{
	local KeyInventory K;
	if ( Pawn(Other) != None )
	{
		K = KeyInventory(Pawn(Other).FindInventoryType(class'KeyInventory'));
		if ( (K != None) && (K.Tag == KeyTag) )
			K.UnLock(self);
	}
}

defaultproperties
{
	KeyTag=KeyPickup
	bCollideActors=true
}