class TriggeredObjective extends GameObjective;

var Actor MyTrigger;

function FindTrigger()
{
	local Actor A;

	if ( (MyTrigger != None) && !MyTrigger.bDeleteMe )
		return;

	MyTrigger = None;
	ForEach AllActors(class'Actor',A)
		if ( A.Event == Tag )
		{
			MyTrigger = A;
			return;
		}
}

function Trigger(Actor Other, Pawn Instigator)
{
	DisableObjective(Instigator);
}

/* TellBotHowToDisable()
tell bot what to do to disable me.
return true if valid/useable instructions were given
*/
function bool TellBotHowToDisable(Bot B)
{
	// FIXME - fix trigger special handling
	FindTrigger();

	if ( MyTrigger == None )
		return false;

	if ( B.ActorReachable(MyTrigger) )
	{
		B.GoalString = "Failed to activate trigger "$MyTrigger;
		B.MoveTarget = MyTrigger.SpecialHandling(B.Pawn);
		if ( B.MoveTarget == None )
			return false;
		B.GoalString = "Go to activate trigger "$MyTrigger;
		B.SetAttractionState();
		return true;
	}

	B.FindBestPathToward(MyTrigger, true,true);
	if ( B.MoveTarget == None )
		return false;
	B.GoalString = "Follow path to "$MyTrigger;
	B.SetAttractionState();
	return true;
}


function bool BotNearObjective(Bot B)
{
	if ( (MyBaseVolume != None)
		&& B.Pawn.IsInVolume(MyBaseVolume) )
		return true;

	if ( MyTrigger == None )
		return false;
	
	return ( (VSize(MyTrigger.Location - B.Pawn.Location) < 2000)	&& B.LineOfSightTo(MyTrigger) );
}

defaultproperties
{
}