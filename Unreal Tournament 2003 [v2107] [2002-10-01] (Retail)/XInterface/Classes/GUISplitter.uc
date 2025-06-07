// ====================================================================
//  Class:  xInterface.GUISplitter
//
//	GUISplitters allow the user to size two other controls (usually Panels)
//
//  Written by Jack Porter
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================
class GUISplitter extends GUIPanel
	native;

cpptext
{
		void PreDraw(UCanvas* Canvas);
		void Draw(UCanvas* Canvas);
		UBOOL MouseMove(INT XDelta, INT YDelta);
		UBOOL MousePressed(UBOOL IsRepeat);
		void SplitterUpdatePositions();
}	
	
enum EGUISplitterType
{
	SPLIT_Vertical,
	SPLIT_Horizontal,
};

var(Menu)			EGUISplitterType	SplitOrientation;
var(Menu)			float				SplitPosition;			// 0.0 - 1.0
var					float				SplitAreaSize;			// size of splitter thing

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);

	if(SplitOrientation == SPLIT_Vertical)
		MouseCursorIndex = 3;
	else
		MouseCursorIndex = 5;

	SplitterUpdatePositions();
}

native function SplitterUpdatePositions();

defaultproperties
{
	StyleName="SquareButton"
	SplitOrientation=SPLIT_Vertical
	bCaptureTabs=False
	bNeverFocus=False
	bTabStop=False
	bAcceptsInput=True
	SplitPosition=0.5
	SplitAreaSize=8
}