// ====================================================================
//  Class:  XInterface.UT2SpinnerButton
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUISpinnerButton extends GUIGFXButton
	Native;

var(Menu)	bool	PlusButton;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);
	
	if (PlusButton)
		Graphic = Material'InterfaceContent.Menu.NumButPlus';
	else
		Graphic = Material'InterfaceContent.Menu.NumButMinus';
}


defaultproperties
{
	StyleName="RoundScaledButton"
	PlusButton=false
	Position=ICP_Scaled
	bNeverFocus=true
	bRepeatClick=true
	bCaptureMouse=true
}
