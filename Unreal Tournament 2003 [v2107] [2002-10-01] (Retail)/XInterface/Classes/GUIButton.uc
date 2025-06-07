// ====================================================================
//  Class:  xInterface.GUIButton
//
//	GUIButton - The basic button class
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIButton extends GUIComponent
		Native;

cpptext
{
		void Draw(UCanvas* Canvas);
}		
		
var		localized	string			Caption;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	OnKeyEvent=InternalOnKeyEvent;
}

function bool InternalOnKeyEvent(out byte Key, out byte State, float delta)
{
	if (key==0x0D && State==3)	// ENTER Pressed
	{
		OnClick(self);
		return true;
	}
	
	if (key==0x026 && State==1)
	{
		PrevControl(none);
		return true;
	}
			
	if (key==0x028 && State==1)
	{
		NextControl(none);
		return true;
	}
	
	return false;
}



event ButtonPressed();		// Called when the button is pressed;
event ButtonReleased();		// Called when the button is released;

defaultproperties
{
	StyleName="RoundButton"	
	bAcceptsInput=true
	bCaptureMouse=True
	bNeverFocus=false;
	bTabStop=true
	WinHeight=0.04
	bMouseOverSound=true
	OnClickSound=CS_Click
}
