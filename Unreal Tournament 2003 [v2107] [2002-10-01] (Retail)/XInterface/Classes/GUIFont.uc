// ====================================================================
//  Class:  xInterface.GUIFont
// 
//  GUIFont is used to give a single pipeline for handling fonts at
//	multiple resolutions while at the same time supporting resolution
//	independant fonts (for browsers, etc). 
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIFont extends GUI
	Native;

var(Menu) string		KeyName;
var(Menu) bool			bFixedSize;		// If true, only FontArray[0] is used
var(Menu) array<Font>	FontArray;		// Holds all of the fonts 		

native event Font GetFont(int XRes);			// Returns the font for the current resolution

defaultproperties
{
}
