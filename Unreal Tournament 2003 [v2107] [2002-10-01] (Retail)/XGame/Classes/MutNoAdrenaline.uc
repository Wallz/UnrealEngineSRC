//=============================================================================
// NoAdrenaline - disables all the super cool combo moves
//=============================================================================
class MutNoAdrenaline extends Mutator;

function bool IsRelevant(Actor Other, out byte bSuperRelevant)
{
	if ( Other.IsA('AdrenalinePickup') )
	{
    	bSuperRelevant = 0;
		return false;
    }
    if ( Controller(Other) != None )
		Controller(Other).bAdrenalineEnabled = false;
	return true;
}

defaultproperties
{
    IconMaterialName="MutatorArt.nosym"
    ConfigMenuClassName=""
    GroupName="Adrenaline"
    FriendlyName="No Adrenaline"
    Description="Adrenaline pickups are removed from the map."
}
