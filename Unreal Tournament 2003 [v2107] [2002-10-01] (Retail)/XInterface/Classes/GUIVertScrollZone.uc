// ====================================================================
//  Class:  XInterface.GUIVertScrollZone
//
//  The VertScrollZone is the background zone for a vertical scrollbar.
//  When the user clicks on the zone, it caculates it's percentage.
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIVertScrollZone extends GUIComponent
	Native;

cpptext
{
		void Draw(UCanvas* Canvas);
}

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	
	OnClick = InternalOnClick; 
}

function bool InternalOnClick(GUIComponent Sender)
{
	local float perc;
	
	if (!IsInBounds())
		return false;
	
	perc = ( Controller.MouseY - ActualTop() ) / ActualHeight();
	OnScrollZoneClick(perc);

	return true;
		
}
	

delegate OnScrollZoneClick(float Delta)		// Should be overridden
{
}

defaultproperties
{
	StyleName="ScrollZone"
	bNeverFocus=true
	bAcceptsInput=true
	bCaptureMouse=true
	bRepeatClick=true	
}
