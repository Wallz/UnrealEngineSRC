//=============================================================================
// DMMutator.
//=============================================================================

class DMMutator extends Mutator;

var() globalconfig bool bMegaSpeed;
var() globalconfig float AirControl;

// mc - localized PlayInfo descriptions & extra info
var private localized string DMMutPropsDisplayText[2];

function bool MutatorIsAllowed()
{
	return true;
}

function bool AlwaysKeep(Actor Other)
{
	if ( NextMutator != None )
		return ( NextMutator.AlwaysKeep(Other) );
	return false;
}

function ModifyPlayer(Pawn Other)
{
	Other.AirControl = AirControl;

	if ( NextMutator != None )
		NextMutator.ModifyPlayer(Other);
}

function bool CheckReplacement(Actor Other, out byte bSuperRelevant)
{
	// set bSuperRelevant to false if want the gameinfo's super.IsRelevant() function called
	// to check on relevancy of this actor.

	bSuperRelevant = 1;
	if ( Pawn(Other) != None )
	{
		Pawn(Other).AirControl = AirControl;
		Pawn(Other).bAutoActivate = true;
		if ( bMegaSpeed )
		{
			Pawn(Other).GroundSpeed *= 1.4;
			Pawn(Other).WaterSpeed *= 1.4;
			Pawn(Other).AirSpeed *= 1.4;
			Pawn(Other).AccelRate *= 1.4;
		}
	}
	bSuperRelevant = 0;
	return true;
}

static function FillPlayInfo(PlayInfo PlayInfo)
{
local int i;

	Super.FillPlayInfo(PlayInfo);  // Always begin with calling parent

	PlayInfo.AddSetting("Game",  "bMegaSpeed", default.DMMutPropsDisplayText[i++], 1, 70, "Check");
	PlayInfo.AddSetting("Game",  "AirControl", default.DMMutPropsDisplayText[i++], 1, 90, "Text", "8;0.1:10.0");
}

defaultproperties
{
	AirControl=+0.35
	DefaultWeaponName=""
	DMMutPropsDisplayText(0)="Mega Speed"
	DMMutPropsDisplayText(1)="Air Control"
}