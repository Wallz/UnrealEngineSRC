// ====================================================================
//  Class:  XInterface.GUIVertScrollBar

// ====================================================================

class GUIVertScrollBar extends GUIScrollBarBase
		Native;
		
cpptext
{
		void PreDraw(UCanvas* Canvas);
}		
		
var		float			GripTop;		// Where in the ScrollZone is the grip	- Set Natively
var		float			GripHeight;		// How big is the grip - Set Natively

var		float			GrabOffset; // distance from top of button that the user started their drag. Set natively.	


function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	
	GUIVertScrollZone(Controls[0]).OnScrollZoneClick = ZoneClick;
	Controls[1].OnClick = UpTickClick;
	Controls[2].OnClick = DownTickClick;
	Controls[3].OnCapturedMouseMove = GripMouseMove;
	Controls[3].OnClick = GripClick;
}

function UpdateGripPosition(float NewPos)
{
	MyList.MakeVisible(NewPos);
	GripTop = NewPos;
}

// Record location you grabbed the grip
function bool GripClick(GUIComponent Sender)
{
	GrabOffset = Controller.MouseY - Controls[3].ActualTop();

	return true;
}

function bool GripMouseMove(float deltaX, float deltaY)
{
	local float NewPerc,NewTop;
	
	// Calculate the new Grip Top using the mouse cursor location.	
	NewPerc = (  float(Controller.MouseY) - (GrabOffset + Controls[0].ActualTop()) )  /(Controls[0].ActualHeight()-GripHeight);
	NewTop = FClamp(NewPerc,0.0,1.0);

	UpdateGripPosition(Newtop);
	
	return true;	
}

function ZoneClick(float Delta)
{
	if ( Controller.MouseY < Controls[3].Bounds[1] )
		MoveGripBy(-MyList.ItemsPerPage);
	else if ( Controller.MouseY > Controls[3].Bounds[3] )
		MoveGripBy(MyList.ItemsPerPage);
		
	return;
}

function MoveGripBy(int items)
{
	local int TopItem;

	TopItem = MyList.Top + items;
	if (MyList.ItemCount > 0)
	{
		MyList.SetTopItem(TopItem);
		AlignThumb();
	}
}

function bool UpTickClick(GUIComponent Sender)
{
	WheelUp();
	return true;
}

function bool DownTickClick(GUIComponent Sender)
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
	local float NewTop;
	
	if (MyList.ItemCount==0)
		NewTop = 0;
	else
	{
		NewTop = Float(MyList.Top) / Float(MyList.ItemCount-MyList.ItemsPerPage);
		NewTop = FClamp(NewTop,0.0,1.0);
	}
		
	GripTop = NewTop;
}
	

// NOTE:  Add graphics for no-man's land about and below the scrollzone, and the Scroll nub.		

defaultproperties
{
	Begin Object Class=GUIVertScrollZone Name=ScrollZone
		Name="ScrollZone"
	End Object
	Controls(0)=GUIVertScrollZone'ScrollZone'
	Begin Object Class=GUIVertScrollButton Name=UpBut
		UpButton=true
		Name="UpBut"
	End Object
	Controls(1)=GUIVertScrollButton'UpBut'
	Begin Object Class=GUIVertScrollButton Name=DownBut
		UpButton=false
		Name="DownBut"
	End Object
	Controls(2)=GUIVertScrollButton'DownBut'
	Begin Object Class=GUIVertGripButton Name=Grip
		Name="Grip"
	End Object
	Controls(3)=GUIVertGripButton'Grip'
	bAcceptsInput=true;
	WinWidth=0.0375
}
