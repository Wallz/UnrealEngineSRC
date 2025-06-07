class BulldogTrigger extends ScriptedSequence
	notplaceable;

var float BTReTriggerDelay;
var	float TriggerTime;
var bool  bCarFlipTrigger;

function Touch( Actor Other )
{
	local xPawn User;

	if( Other.Instigator != None )
	{
		User = xPawn(Other);

		if(User == None)
			return;

		if ( BTReTriggerDelay > 0 )
		{
			if ( Level.TimeSeconds - TriggerTime < BTReTriggerDelay )
				return;
			TriggerTime = Level.TimeSeconds;
		}

		// Send a string message to the toucher.
		if(!bCarFlipTrigger)
			User.ReceiveLocalizedMessage(class'Vehicles.BulldogMessage', 0);
		else
			User.ReceiveLocalizedMessage(class'Vehicles.BulldogMessage', 3);
	}
}

function UsedBy( Pawn user )
{
	if(bCarFlipTrigger)
	{
		Bulldog(Owner).StartFlip(User);
	}
	else
	{
		// Enter vehicle code
		Bulldog(Owner).TryToDrive(User);
	}
}

defaultproperties
{
	bHardAttach=True
    bHidden=True
	bCollideActors=false
    bStatic=false
    CollisionRadius=+0080.000000
	CollisionHeight=+0400.000000
	bCollideWhenPlacing=False
	BTReTriggerDelay=0.1
	bOnlyAffectPawns=True
    RemoteRole=ROLE_None
}