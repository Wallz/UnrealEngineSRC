// ====================================================================
//  Class:  XInterface.GUIHorzScrollButton
//  Parent: XInterface.GUIGFXButton
//
//  <Enter a description here>
// ====================================================================

class GUIHorzScrollButton extends GUIGFXButton
		Native;

#exec OBJ LOAD FILE=InterfaceContent.utx

var(Menu)	bool	LeftButton;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);
	
	if (LeftButton)
		Graphic = Material'InterfaceContent.Menu.ArrowBlueLeft';
	else
		Graphic = Material'InterfaceContent.Menu.ArrowBlueRight';
}


defaultproperties
{
	StyleName="RoundScaledButton"
	//StyleName="NoBackground"
	LeftButton=false
	Position=ICP_Scaled
	bNeverFocus=true
	bCaptureMouse=true	
}
