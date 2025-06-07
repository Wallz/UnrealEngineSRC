// ====================================================================
//	Class: XInterface. UT2NumericEdit 
//
//  A Combination of an EditBox and 2 spinners
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUINumericEdit extends GUIMultiComponent
	Native;

cpptext
{
		void Draw(UCanvas* Canvas);
}	
	
var(Menu)	string				Value;
var(Menu)	bool				bLeftJustified;
var(Menu)	int					MinValue;
var(Menu)	int					MaxValue;
var(Menu)	int					Step;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	
	Super.Initcomponent(MyController, MyOwner);

	GUIEditBox(Controls[0]).OnChange = EditOnChange;
	GUIEditBox(Controls[0]).SetText(Value);
	GUIEditBox(Controls[0]).OnKeyEvent = EditKeyEvent;

	Controls[0].INIOption  = INIOption;
	Controls[0].INIDefault = INIDefault;
	Controls[0].Hint	   = Hint;

	CalcMaxLen();
			
	Controls[1].OnClick = SpinnerPlusClick;
	Controls[1].FocusInstead = Controls[0];
	Controls[2].OnClick = SpinnerMinusClick;
	Controls[2].FocusInstead = Controls[0];

}

function CalcMaxLen()
{
	local int digitcount,x;

	digitcount=1;
	x=10;
	while (x<MaxValue)
	{
		digitcount++;
		x*=10;
	}
	
	GUIEditBox(Controls[0]).MaxWidth = DigitCount;
}
function SetValue(int V)
{
	if (v<MinValue)
		v=MinValue;
		
	if (v>MaxValue)
		v=MaxValue;
		
	GUIEditBox(Controls[0]).SetText(""$v);
}

function bool SpinnerPlusClick(GUIComponent Sender)
{
	local int v;
	
	v = int(Value) + Step;
	if (v>MaxValue)
	  v = MaxValue;
	  
	GUIEditBox(Controls[0]).SetText(""$v);
	return true;
}

function bool SpinnerMinusClick(GUIComponent Sender)
{
	local int v;

	v = int(Value) - Step;
	if (v<MinValue)
		v=MinValue;
		
	GUIEditBox(Controls[0]).SetText(""$v);
	return true;
}

function bool EditKeyEvent(out byte Key, out byte State, float delta)
{
	if ( (key==0xEC) && (State==3) )
	{
		SpinnerPlusClick(none);
		return true;
	}
	
	if ( (key==0xED) && (State==3) )
	{
		SpinnerMinusClick(none);
		return true;
	}
	
	return GUIEditBox(Controls[0]).InternalOnKeyEvent(Key,State,Delta);
		

}

function EditOnChange(GUIComponent Sender)
{
	Value = GUIEditBox(Controls[0]).TextStr;
}

defaultproperties
{
	Begin Object Class=GUIEditBox Name=EditBox1
		TextStr=""
		bIntOnly=true
		Name="EditBox1"
	End Object
	Controls(0)=GUIEditBox'EditBox1'
	Begin Object Class=GUISpinnerButton Name=Plus
		PlusButton=true
		Name="Plus"
	End Object
	Controls(1)=GUISpinnerButton'Plus'
	Begin Object Class=GUISpinnerButton Name=Minus
		PlusButton=false
		Name="Minus"
	End Object
	Controls(2)=GUISpinnerButton'Minus'
	Value="0"
	Step=1
	bAcceptsInput=true;
	bLeftJustified=false;
	WinHeight=0.06
}
