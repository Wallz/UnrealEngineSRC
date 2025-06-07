// ====================================================================
//  Class:  xInterface.GUIButton
//
//	GUIGFXButton - The basic button class.  It can be used for icon buttons
//  or Checkboxes
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIGFXButton extends GUIButton 
	Native;

#exec OBJ LOAD FILE=InterfaceContent.utx		

cpptext
{
		void Draw(UCanvas* Canvas);
}
		
var(Menu)	Material 		Graphic;		// The graphic to display
var(Menu)	eIconPosition	Position;		// How do we draw the Icon
var(Menu)	bool			bCheckBox;		// Is this a check box button (ie: supports 2 states)
var(Menu)	bool			bClientBound;	// Graphic is drawn using clientbounds if true

var		bool			bChecked;	

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);

	if (bCheckBox)
		OnCLick = InternalOnClick;
}

function SetChecked(bool bNewChecked)
{
	if (bCheckBox)
	{
		bChecked = bNewChecked;
		OnChange(Self);	
	}
}

function bool InternalOnClick(GUIComponent Sender)
{
	if (bCheckBox)
		bChecked = !bChecked;
		
	OnChange(Self);		
	return true;
} 

defaultproperties
{
	Position=ICP_Normal
	bCheckBox=false
	bRepeatClick=true
	bChecked=false

}
