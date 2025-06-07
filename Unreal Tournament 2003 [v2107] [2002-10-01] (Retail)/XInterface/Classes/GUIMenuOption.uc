// ====================================================================
//  Class:  xInterface.GUIMultiComponent
//
//	MenuOptions combine a label and any other component in to 1 single
//  control.  The Label is left justified, the control is right.
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIMenuOption extends GUIMultiComponent
		Native;

cpptext
{
		void PreDraw(UCanvas* Canvas);
}		
		
var(Menu)	string			ComponentClassName;		// Name of the component to spawn
var(Menu)	localized string	Caption;				// Caption for the label
var(Menu)	string			LabelFont;				// Name of the Font for the label
var(Menu)	Color			LabelColor;				// Color for the label	
var(Menu)	bool			bHeightFromComponent;	// Get the Height of this component from the Component
var(Menu)	float			CaptionWidth;			// How big should the Caption be
var(Menu)	float			ComponentWidth;			// How big should the Component be (-1 = 1-CaptionWidth)
var(Menu)	bool			bFlipped;				// Draw the Component to the left of the caption
var(Menu)	eTextAlign		LabelJustification;		// How do we justify the label
var(Menu)	eTextAlign		ComponentJustification;	// How do we justify the label
var(Menu)	bool			bSquare;				// Use the Height for the Width
var(Menu)	bool			bVerticalLayout;		// Layout controls vertically
		
var			GUILabel		MyLabel;				// Holds the label
var			GUIComponent	MyComponent;			// Holds the component


function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	local class<GUIComponent> CClass;
	
	Super.Initcomponent(MyController, MyOwner);
	
	// Create the two components

	MyLabel = new class'GUILabel';
	if (MyLabel==None)
	{
		log("Failed to create "@self@" due to problems creating GUILabel");
		return;
	}
	
	if (bFlipped)
	{
		if (LabelJustification==TXTA_Left)
			LabelJustification=TXTA_Right;
			
		else if (LabelJustification==TXTA_Right)
			LabelJustification=TXTA_Left;
			
		if (ComponentJustification==TXTA_Left)
			ComponentJustification=TXTA_Right;
			
		else if (ComponentJustification==TXTA_Right)
   			ComponentJustification=TXTA_Left;
	}		
	
	
	Controls[0]=MyLabel;
	
	MyLabel.Caption  	= Caption;
	MyLabel.TextFont 	= LabelFont;
	MyLabel.TextColor	= LabelColor;
	MyLabel.TextAlign   = LabelJustification;
	MyLabel.StyleName 	= "TextLabel";	 
	MyLabel.InitComponent(MyController,Self);

	CClass = class<GUIComponent>(DynamicLoadObject(ComponentClassName,class'class'));
	if (CClass != None)
	{
		if (MyComponent == None || !MyComponent.IsA(CClass.Name))
		{
			// Attempt to spawn the component
			MyComponent = new CClass;
		
			// Check for errors
			if (MyComponent == None)
			{
				log("Could not create requested menu component"@ComponentClassName);
				return;
			}
		}
	}
	else
	{
		log("Could not DLO menu component"@ComponentClassName);
		return;
	}
	
	Controls[1] = MyComponent;
	
	MyComponent.Hint	   = Hint;
	
	MyComponent.InitComponent(MyController,Self);
	
	if (bHeightFromComponent && !bVerticalLayout)
		WinHeight = MyComponent.WinHeight;
		
	MyComponent.OnChange = InternalOnChange;
	
	MyComponent.FriendlyLabel = MyLabel;
			
}

function InternalOnChange(GUIComponent Sender)
{
	OnChange(self);
}

defaultproperties
{
	bHeightFromComponent=true
	CaptionWidth=0.5
	ComponentWidth=-1
	WinWidth=0.500000
	WinHeight=0.060000
	WinLeft=0.496094
	WinTop=0.347656
	LabelFont="UT2MenuFont"
	LabelColor=(R=255,G=255,B=255,A=255)	
	bFlipped=false
	LabelJustification=TXTA_Left
	ComponentJustification=TXTA_Right	
	bSquare=false
}
