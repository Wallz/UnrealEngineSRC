//=============================================================================
// QuadJump - Allows you to quad-jump instead of the normal double jump!
//=============================================================================
class MutQuadJump extends Mutator;

function ModifyPlayer(Pawn Other)
{
	local xPawn x;
	x = xPawn(Other);
	
	if(x != None)
	{
		// Increase the number of times a player can jump in mid air
		x.MaxMultiJump = 3;
		
		// Also increase a bit the amount they jump each time
		x.MultiJumpBoost = 50;
	}
}

defaultproperties
{
    IconMaterialName="MutatorArt.nosym"
    ConfigMenuClassName=""
    GroupName="Jumping"
    FriendlyName="QuadJump"
    Description="When double jump just isn't enough."
}