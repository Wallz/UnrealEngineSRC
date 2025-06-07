// ====================================================================
//  Class:  XInterface.GUIVertGripButton
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIVertGripButton extends GUIGFXButton
		Native;


function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);
//	Graphic = Material'InterfaceContent.Menu.ButGrip';
}


defaultproperties
{
	StyleName="RoundButton"
	Position=ICP_Bound
	bNeverFocus=true
	bCaptureMouse=true	
	OnClickSound=CS_None
}
