// ====================================================================
//  Class:  xInterface.GUITitleBar
//
//  Used for the Top and Bottom titles on a page
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUITitleBar extends GUIComponent
	Native;

cpptext
{
		void PreDraw(UCanvas* Canvas);
		void Draw(UCanvas* Canvas);	
}		
	
var localized	string 			Caption;		// The caption that get's displayed in here
var				eTextAlign		Justification;	// How to draw		
var				GUITabControl	DockedTabs;		// Set this to a Tab control and that control will be centered undeneath
var				bool			bDockTop;		// If True, dock the control ON TOP of this one
var				bool			bUseTextHeight;	// Should this control scale to the text height
var				material		Effect;			// Allows you to overlay a cool shader effect.					

defaultproperties
{
	bDockTop=false
	bUseTextHeight=true
	bTabStop=false
	bNeverFocus=true
	Justification=TXTA_Left;
}
