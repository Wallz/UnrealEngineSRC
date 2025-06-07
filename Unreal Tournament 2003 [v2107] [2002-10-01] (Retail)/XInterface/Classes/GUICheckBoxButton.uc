// ====================================================================
//  Class:  XInterface.GUIGFXButton
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUICheckBoxButton extends GUIGFXButton
	Native;

	
function string LoadINI()
{
	local string s;
	
	s = Super.LoadINI();

	if (S=="")
		return s;
		
	bChecked = bool(s);	
	return s;
}

function SaveINI(string Value)
{
	Super.SaveINI(""$bChecked);
}
	
	
defaultproperties
{
	bCheckBox=true
	Position=ICP_Scaled
	bRepeatClick=false
	Graphic=Material'InterfaceContent.menus.CheckBoxMark'

}
