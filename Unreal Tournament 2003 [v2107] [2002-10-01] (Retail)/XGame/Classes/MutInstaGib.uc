//=============================================================================
// InstaGib - all hits are instant kill! old sk00l
//=============================================================================
class MutInstaGib extends Mutator;

var name WeaponName, AmmoName;
var string WeaponString, AmmoString;
var bool bAllowTranslocator;


function string RecommendCombo(string ComboName)
{
	if ( (ComboName != "xGame.ComboSpeed") && (ComboName != "xGame.ComboInvis") )
	{
		if ( FRand() < 0.65 )
			ComboName = "xGame.ComboInvis";
		else
			ComboName = "xGame.ComboSpeed";
	}
	if ( NextMutator != None )
		return NextMutator.RecommendCombo(ComboName);
	return ComboName;
}

function bool AlwaysKeep(Actor Other)
{
	if ( Other.IsA(WeaponName) || Other.IsA(AmmoName) )
		return true;

	if ( NextMutator != None )
		return ( NextMutator.AlwaysKeep(Other) );
	return false;
}

function bool CheckReplacement(Actor Other, out byte bSuperRelevant)
{
	if ( Other.IsA('Weapon') )
	{
        if ( Other.IsA('BallLauncher') )
        {
            bSuperRelevant = 0;
            return true;
        }

		if ( Other.IsA('TransLauncher') && bAllowTranslocator )
		{
            bSuperRelevant = 0;
            return true;
        }

		if ( !Other.IsA(WeaponName) )
		{
			Level.Game.bWeaponStay = false;
			ReplaceWith(Other, WeaponString);
			return false;
		}
	}

	if ( Other.IsA('Pickup') )
		return false;

	if ( Other.IsA('xPickupBase') )
		Other.bHidden = true;
		
	bSuperRelevant = 0;
	return true;
}

defaultproperties
{
	AmmoName=ShockAmmo
	AmmoString="xWeapons.ShockAmmo"
	WeaponName=SuperShockRifle
	WeaponString="xWeapons.SuperShockRifle"
	DefaultWeaponName="xWeapons.SuperShockRifle"

    IconMaterialName="MutatorArt.nosym"
    ConfigMenuClassName=""
    GroupName="Arena"
    FriendlyName="InstaGib"
    Description="Instant-kill combat with modified Shock Rifles."
}
