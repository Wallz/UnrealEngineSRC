// ====================================================================
//  Class:  xInterface.GUIImage
//
//	GUIImage - A graphic image used by the menu system.  It encapsulates
//	Material.
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIImage extends GUIComponent
	Native;

cpptext
{
		void Draw(UCanvas* Canvas);
}	
	
var(Menu) Material 			Image;				// The Material to Render
var(Menu) color				ImageColor;			// What color should we set
var(Menu) eImgStyle			ImageStyle;			// How should the image be displayed
var(Menu) EMenuRenderStyle	ImageRenderStyle;	// How should we display this image
var(Menu) eImgAlign			ImageAlign;			// If ISTY_Justified, how should image be aligned
var(Menu) int				X1,Y1,X2,Y2;		// If set, it will pull a subimage from inside the image

defaultproperties
{
	ImageColor=(R=255,G=255,B=255,A=255)
	ImageStyle=ISTY_Normal
	ImageRenderStyle=MSTY_Alpha	
	ImageAlign=IMGA_TopLeft
	X1=-1
	X2=-1
	Y1=-1
	Y2=-1
}
