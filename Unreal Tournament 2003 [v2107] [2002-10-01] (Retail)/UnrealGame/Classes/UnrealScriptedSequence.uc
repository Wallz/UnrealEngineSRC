class UnrealScriptedSequence extends ScriptedSequence;

var UnrealScriptedSequence EnemyAcquisitionScript;
var Controller CurrentUser;
var UnrealScriptedSequence NextScript;	// list of scripts with same tag
var bool bFirstScript;				// first script in list of scripts
var() bool bSniping;				// bots should snipe when using this script as a defense point
var() bool bDontChangeScripts;		// bot should go back to this script, not look for other compatible scripts
var bool  bFreelance;					// true if not claimed by any game objective
var() bool bRoamingScript;				// if true, roam after reaching
var() byte priority;				// used when several scripts available (e.g. defense scripts for an objective)
var() name EnemyAcquisitionScriptTag;	// script to go to after leaving this script for an acquisition
var() float EnemyAcquisitionScriptProbability;	// likelihood that bot will use acquisitionscript
var() name SnipingVolumeTag;		// area defined by volume in which to look for (distant) sniping targets
var() class<Weapon> WeaponPreference;	// bots using this defense point will preferentially use this weapon

var float NumChecked;
var bool bAvoid;

/* Reset() 
reset actor to initial state - used when restarting level without reloading.
*/
function Reset()
{
	FreeScript();
}

function FreeScript()
{
	CurrentUser = None;
}

function BeginPlay()
{
	local UnrealScriptedSequence S;
	local SnipingVolume V;

	Super.BeginPlay();

	if ( EnemyAcquisitionScriptTag != '' )
	{
		ForEach AllActors(class'UnrealScriptedSequence',EnemyAcquisitionScript,EnemyAcquisitionScriptTag)
			break;
	}

	if ( bFirstScript )
	{
		// first one initialized - create script list
		ForEach AllActors(class'UnrealScriptedSequence',S,Tag)
			if ( S != self )
			{
				NextScript = S;
				NextScript.bFirstScript = false;	
				break;
			}
	}
	
	if ( SnipingVolumeTag != 'None' )
		ForEach AllActors(class'SnipingVolume',V,SnipingVolumeTag)
			V.AddDefensePoint(self);
}

function bool HigherPriorityThan(UnrealScriptedSequence S, Bot B)
{
	NumChecked = 1;
	if ( bAvoid )
	{
		bAvoid = false;
		return false;
	}
	if ( (CurrentUser != None) && !CurrentUser.bDeleteMe 
		&& CurrentUser.SameTeamAs(B) )
		return false;
	if ( (S == None) || (S.Priority < Priority) )
		return true;
	if ( S.Priority > Priority )
		return false;
	if ( (B.FavoriteWeapon != None) && (B.FavoriteWeapon == WeaponPreference) )
		return true;
	S.NumChecked += 1;
	return ( FRand() < 1/S.NumChecked );
}

defaultproperties
{
	bFirstScript=true
	bFreelance=true
	EnemyAcquisitionScriptProbability=+1.0
    Begin Object Class=Action_MOVETOPOINT Name=DefensePointDefaultAction1
    End Object
    Begin Object Class=Action_WAITFORTIMER Name=DefensePointDefaultAction2
		PauseTime=3.0
    End Object
    Actions(0)=ScriptedAction'DefensePointDefaultAction1'
    Actions(1)=ScriptedAction'DefensePointDefaultAction2'
	ScriptControllerClass=class'UnrealGame.Bot'
}
