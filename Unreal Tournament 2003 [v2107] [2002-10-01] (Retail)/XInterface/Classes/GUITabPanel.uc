// ====================================================================
//	GUITabButton - A Tab Button has an associated Tab Control, and
//  TabPanel. 
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUITabPanel extends GUIPanel
		Native Abstract;

cpptext
{
		void PreDraw(UCanvas* Canvas);
}
		
		
var(Menu)	bool			bFillHeight;	// If true, the panel will set it's height = Top - ClipY	
var			GUITabButton	MyButton;
		
function InitPanel();	// Should be Subclassed

function ShowPanel(bool bShow)	// Show Panel should be subclassed if any special processing is needed
{
	bVisible = bShow;
}		

function bool CanShowPanel()	// Subclass this function to change selection behavior of the tab
{
	return true;
}

function Refresh();	// Refresh this panel
		
defaultproperties
{
}
