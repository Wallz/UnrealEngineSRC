// ====================================================================
//  Class:  XInterface.GUIListBoxBase
//
//  The GUIListBoxBase is a wrapper for a GUIList and it's ScrollBar   
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIListBoxBase extends GUIMultiComponent
		Native;

cpptext
{
	void PreDraw(UCanvas* Canvas);		
	void Draw(UCanvas* Canvas);										// Handle drawing of the component natively
}		
		
var		GUIVertScrollBar	ScrollBar;
var		bool				bVisibleWhenEmpty;						// List box is visible when empty.

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	local int i;
	local GUIListBase LocalList;

	LocalList = GUIListBase(Controls[0]);
	ScrollBar = GUIVertScrollBar(Controls[1]);	
	
	LocalList.StyleName = StyleName;	
	LocalList.bVisibleWhenEmpty = bVisibleWhenEmpty;
	
	Super.Initcomponent(MyController, MyOwner);

	
	LocalList.MyScrollBar = ScrollBar;
	ScrollBar.MyList = LocalList;	
	
	ScrollBar.FocusInstead = LocalList;
	
	for (i=0;i<ScrollBar.Controls.Length;i++)
		ScrollBar.Controls[i].FocusInstead = LocalList;
}

defaultproperties
{
	// NOTE: Controls(0) is a GUIListBase - defined in a subclass
	Begin Object Class=GUIVertScrollBar Name=TheScrollbar
		Name="TheScrollbar"
		bVisible=false
	End Object
	Controls(1)=GUIVertScrollBar'TheScrollbar'
	bAcceptsInput=true;
	StyleName="ListBox"
	bVisibleWhenEmpty=False
}
