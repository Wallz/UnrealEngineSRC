// ====================================================================
//  Class:  XInterface.GUIHorzScrollBar
//  Parent: XInterface.GUIMultiComponent
//
//  <Enter a description here>
// ====================================================================

class GUIHorzScrollBar extends GUIScrollBarBase
		Native;

cpptext
{
		void PreDraw(UCanvas* Canvas);
}		
		
var		float			GripLeft;		// Where in the ScrollZone is the grip	- Set Natively
var		float			GripWidth;		// How big is the grip - Set Natively

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	
	GUIVertScrollZone(Controls[0]).OnScrollZoneClick = ZoneClick;
	Controls[1].OnClick = LeftTickClick;
	Controls[2].OnClick = RightTickClick;
	Controls[3].OnCapturedMouseMove = GripMouseMove;
	
}

function UpdateGripPosition(float NewPos)
{
	MyList.MakeVisible(NewPos);
	GripLeft = NewPos;
}

function bool GripMouseMove(float deltaX, float deltaY)
{
	local float NewPerc,NewLeft;

	if (deltaX==0)	// Don't care about horz movement
		return true;


	deltaX*=-1;
	
	// Calculate the new Grip Left using the mouse cursor location.
	
	NewPerc = abs(deltaX) / (Controls[0].ActualWidth()-GripWidth);

	if (deltaX<0)
		NewPerc*=-1;

	NewLeft = FClamp(GripLeft+NewPerc,0.0,1.0);

	UpdateGripPosition(NewLeft);
	
	return true;	
}

function ZoneClick(float Delta)
{
	if ( Controller.MouseX < Controls[3].Bounds[0] )
		MoveGripBy(-MyList.ItemsPerPage);
	else if ( Controller.MouseX > Controls[3].Bounds[2] )
		MoveGripBy(MyList.ItemsPerPage);
		
	return;
}

function MoveGripBy(int items)
{
	local int LeftItem;

	LeftItem = MyList.Top + items;
	if (MyList.ItemCount > 0)
	{
		MyList.SetTopItem(LeftItem);
		AlignThumb();
	}
}

function bool LeftTickClick(GUIComponent Sender)
{
	WheelUp();
	return true;
}

function bool RightTickClick(GUIComponent Sender)
{
	WheelDown();
	return true;
}

function WheelUp()
{
	if (!Controller.CtrlPressed)
		MoveGripBy(-1);
	else
		MoveGripBy(-MyList.ItemsPerPage);
}

function WheelDown()
{
	if (!Controller.CtrlPressed)
		MoveGripBy(1);
	else
		MoveGripBy(MyList.ItemsPerPage);
}

function AlignThumb()
{
	local float NewLeft;
	
	if (MyList.ItemCount==0)
		NewLeft = 0;
	else
	{
		NewLeft = Float(MyList.Top) / Float(MyList.ItemCount-MyList.ItemsPerPage );
		NewLeft = FClamp(NewLeft,0.0,1.0);
	}
		
	GripLeft = NewLeft;
}
	

// NOTE:  Add graphics for no-man's land about and below the scrollzone, and the Scroll nub.		

defaultproperties
{
	Begin Object Class=GUIHorzScrollZone Name=HScrollZone
	End Object
	Controls(0)=GUIHorzScrollZone'HScrollZone'
	Begin Object Class=GUIHorzScrollButton Name=HLeftBut
		LeftButton=true
	End Object
	Controls(1)=GUIHorzScrollButton'HLeftBut'
	Begin Object Class=GUIHorzScrollButton Name=HRightBut
		LeftButton=false
	End Object
	Controls(2)=GUIHorzScrollButton'HRightBut'
	Begin Object Class=GUIHorzGripButton Name=HGrip
	End Object
	Controls(3)=GUIHorzGripButton'HGrip'
	bAcceptsInput=true;
	WinWidth=0.0375
}
