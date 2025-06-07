// ====================================================================
//  Class:  xInterface.GUIImage
//
//	GUILabel - A text label that get's displayed.  By default, it
//  uses the default font, however you can override it if you wish.
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUILabel extends GUIComponent
	Native;

cpptext
{
		void Draw(UCanvas* Canvas);
}	
	
var(Menu)	localized 	string				Caption;			// The text to display
var(Menu)				eTextAlign			TextAlign;			// How is the text aligned in it's bounding box
var(Menu)				color				TextColor;			// The Color to display this in.
var(Menu)				color				FocusedTextColor;	// The Color to display this in.
var(Menu)				EMenuRenderStyle	TextStyle;			// What canvas style to use
var(Menu)				string 				TextFont;			// The Font to display it in
var(Menu)				bool				bTransparent;		// Draw a Background for this label
var(Menu)				bool				bMultiLine;			// Will cut content to display on multiple lines when too long
var(Menu)				color				BackColor;			// Background color for this label

defaultproperties
{
	TextAlign=TXTA_Left
	TextColor=(R=255,G=255,B=255,A=255)
	FocusedTextColor=(R=255,G=255,B=0,A=255)
	TextStyle=MSTY_Normal
	TextFont="UT2MenuFont"
	bTransparent=true
	BackColor=(R=0,G=0,B=0,A=255)
}
