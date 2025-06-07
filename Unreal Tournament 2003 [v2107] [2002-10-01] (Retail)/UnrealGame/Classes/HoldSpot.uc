class HoldSpot extends UnrealScriptedSequence
	notplaceable;

function FreeScript()
{
	Destroy();
}

defaultproperties
{
	bCollideWhenPlacing=false
	bStatic=false
	bNoDelete=false
}