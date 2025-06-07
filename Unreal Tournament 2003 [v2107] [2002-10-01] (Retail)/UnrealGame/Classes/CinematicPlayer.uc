// ====================================================================
//  Class:  UnrealGame.CinematicPlayer
//  Parent: UnrealGame.UnrealPlayer
//
//  <Enter a description here>
// ====================================================================

class CinematicPlayer extends UnrealPlayer;

function string FindMenu()
{
	local int i;
	local ScriptedSequence SS;
	
	if ( Level.Game.CurrentGameProfile != none )
	{
		return "XInterface.UT2SinglePlayerMain";
	}

	foreach AllActors(class'ScriptedSequence',SS)
	{
		for (i=0;i<SS.Actions.Length;i++)
		{
			if (ACTION_GotoMenu(SS.Actions[i])!=None)
				return ACTION_GotoMenu(SS.Actions[i]).MenuName;
		}
	}
	
	return "XInterface.UT2MainMenu";		// Default back to the main menu
}

exec function Fire( optional float F )
{
	GotoMenu(FindMenu());
}

exec function AltFire( optional float F )
{
	GotoMenu(FindMenu());
}

exec function ShowMenu()
{
	GotoMenu(FindMenu());
}

exec function GotoMenu(string MenuName)
{
	Player.GUIController.OpenMenu(MenuName);
	ConsoleCommand( "DISCONNECT" );
}

defaultproperties
{

}
