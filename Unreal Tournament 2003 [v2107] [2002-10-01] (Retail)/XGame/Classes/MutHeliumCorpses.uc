//=============================================================================
// HeliumCorpses - reduce gravity for ragdolls, so they float around.
//=============================================================================
class MutHeliumCorpses extends Mutator;

function PostBeginPlay()
{
	Level.KarmaGravScale = 0.1;
}

function ModifyPlayer(Pawn Other)
{
	local xPawn x;
	x = xPawn(Other);
	
	if(x != None)
	{
		// Reduce the amount of upwards kick ragdolls get when they die
		x.RagDeathUpKick = 20;
	}
}

defaultproperties
{
    IconMaterialName="MutatorArt.nosym"
    ConfigMenuClassName=""
    GroupName="CorpseGravity"
    FriendlyName="Floaty Cadavers"
    Description="Your kills weigh lightly."
}