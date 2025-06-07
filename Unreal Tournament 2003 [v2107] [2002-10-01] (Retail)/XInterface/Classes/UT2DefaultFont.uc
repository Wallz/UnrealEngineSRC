// ====================================================================
//  Class:  A font..
//
//	One of the system fonts used by the menus 
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class UT2DefaultFont extends GUIFont;

#exec OBJ LOAD File=UT2003Fonts.utx

defaultproperties
{
	KeyName="UT2DefaultFont"
	FontArray=(Font'FontNeuzeit9')
	bFixedSize=true
}
