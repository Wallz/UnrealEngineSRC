// ====================================================================
//  Class:  XInterface.GUIVertScrollButton
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIVertScrollButton extends GUIGFXButton
		Native;

#exec OBJ LOAD FILE=InterfaceContent.utx

var(Menu)	bool	UpButton;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);
	
	if (UpButton)
		Graphic = Material'InterfaceContent.Menu.ArrowBlueUp';
	else
		Graphic = Material'InterfaceContent.Menu.ArrowBlueDown';
}


defaultproperties
{
	StyleName="RoundScaledButton"
	UpButton=false
	Position=ICP_Scaled
	bNeverFocus=true
	bCaptureMouse=true	
}
