// ====================================================================
//  Class:  XInterface.GUIHorzGripButton
//  Parent: XInterface.GUIGFXButton
//
//  <Enter a description here>
// ====================================================================

class GUIHorzGripButton extends GUIGFXButton
		Native;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);
	Graphic = Material'InterfaceContent.Menu.ButGrip';
}


defaultproperties
{
	StyleName="RoundButton"
	Position=ICP_Bound
	bNeverFocus=true
	bCaptureMouse=true	
}

defaultproperties
{

}
