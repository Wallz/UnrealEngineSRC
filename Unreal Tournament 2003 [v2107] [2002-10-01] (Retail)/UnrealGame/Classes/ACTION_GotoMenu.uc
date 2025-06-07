// ====================================================================
//  Class:  UnrealGame.ACTION_GotoMenu
//  Parent: GamePlay.ScriptedAction
//
//  When called, it will transfer the sequecne to a menu
// ====================================================================

class ACTION_GotoMenu extends ScriptedAction;

var(Action) string		MenuName;
var(Action) bool		bDisconnect;

function bool InitActionFor(ScriptedController C)
{
	local PlayerController CP;
	
	ForEach C.AllActors(class'PlayerController',CP)
	{
		CP.ClientOpenMenu(MenuName,bDisconnect);
		return false;
	}
		
	return false;
}

defaultproperties
{
	MenuName="Xinterface.UT2MainMenu"
	bDisconnect=true
}


defaultproperties
{

}
