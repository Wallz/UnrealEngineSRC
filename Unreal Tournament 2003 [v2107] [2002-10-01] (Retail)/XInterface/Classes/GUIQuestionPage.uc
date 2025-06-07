class GUIQuestionPage extends GUIPage;

var GUILabel lMessage;
var Material MessageIcon;	// Like Warning/Question/Exclamation
var localized string ButtonNames[8]; // Buttons Names: Ok, Cancel, Retry, Continue, Yes, No, Abort, Ignore.  Clamped [0,7].
var array<GUIButton> Buttons;
var GUIButton DefaultButton, CancelButton;

delegate OnButtonClick(byte bButton);

function InitComponent(GUIController pMyController, GUIComponent MyOwner)
{
	OnPreDraw=InternalOnPreDraw;
	Super.Initcomponent(pMyController, MyOwner);
	lMessage=GUILabel(Controls[1]);
	ParentPage.InactiveFadeColor=class'Canvas'.static.MakeColor(128,128,128,255);
}

function bool InternalOnPreDraw(Canvas C)
{
local float XL, YL;
local int i;
local array<string> MsgArray;

	// Organize the layout for the Label and the Buttons Top
	if (lMessage.TextFont != "")
		C.Font = Controller.GetMenuFont(lMessage.TextFont).GetFont(C.SizeX);

	C.TextSize("W", XL, YL);
	C.WrapStringToArray(lMessage.Caption, MsgArray, lMessage.ActualWidth(), "|");

	YL *= MsgArray.Length;

	if (lMessage.Style != None)
		YL += lMessage.Style.BorderOffsets[1] + lMessage.Style.BorderOffsets[3];

	// transform YL to a %
	lMessage.WinHeight =  (YL +1 )/ C.SizeY;
	WinHeight = (YL + Buttons[0].ActualHeight() + 60) / C.SizeY ;
	WinTop = (C.SizeY - ActualHeight())/2;
	lMessage.WinTop = WinTop + 20;
	for (i = 0; i<Buttons.Length; i++)
	{
		Buttons[i].WinTop = WinTop + 40 + YL;
	}
	OnPreDraw=None;
	return true;
}

function SetupQuestion(string Question, byte bButtons, byte ActiveButton)
{
	lMessage.Caption = Question;

	// Create Buttons Based on Buttons parameter
	if ((bButtons & QBTN_Abort) != 0)
		AddButton(6);
	if ((bButtons & QBTN_Retry) != 0)
		AddButton(2);
	if ((bButtons & QBTN_Cancel) != 0)
	{
		AddButton(1);
		CancelButton = Buttons[Buttons.Length-1];
	}
	if ((bButtons & QBTN_Continue) != 0)
		AddButton(3);
	if ((bButtons & QBTN_Ignore) != 0)
		AddButton(7);
	if ((bButtons & QBTN_Ok) != 0)
		AddButton(0);
	if ((bButtons & QBTN_Yes) != 0)
		AddButton(4);
	if ((bButtons & QBTN_No) != 0)
		AddButton(5);
	LayoutButtons(ActiveButton);
}

function AddButton(int idesc)
{
local GUIButton btn;

	btn = new class'GUIButton';
	btn.InitComponent(Controller, MenuOwner);
	Controls[Controls.Length] = btn;
	Buttons[Buttons.Length] = btn;
	btn.Caption = ButtonNames[Clamp(idesc,0,7)];
	btn.OnClick = ButtonClick;
	btn.Tag = 1 << idesc;
//	btn.bVisible = false;
}

function LayoutButtons(byte ActiveButton)
{
local int i;
local float left, colw, btnw;

	// Simply center the button(s)
	colw = 1/(Buttons.Length + 1);
	btnw = colw * 0.66;
	left = colw - btnw/2;

	for (i = 0; i<Buttons.Length; i++)
	{
		Buttons[i].WinLeft = left;
		Buttons[i].WinWidth = btnw;
		Buttons[i].WinHeight = 0.042773;
		Buttons[i].WinTop = 0.6;
		left += colw;
	}
}

function bool ButtonClick(GUIComponent Sender)
{
local int T;

	T = GUIButton(Sender).Tag;

	ParentPage.InactiveFadeColor=ParentPage.Default.InactiveFadeColor;
	Controller.CloseMenu(true);
	OnButtonClick(T);
	return true;
}

function string Replace(string Src, string Tag, string Value)
{
local string retval;
local int p, tsz;

	Tag="%"$Tag$"%";
	tsz = Len(Tag);
	p = InStr(Src, Tag);
	while (p != -1)
	{
		retval = retval$Left(Src, p)$Value;
		Src=Mid(Src, p+tsz);
		p = InStr(Src, Tag);
	}
	return retval$Src;
}

defaultproperties
{
	Begin Object Class=GUIImage Name=imgBack
		Image=Material'InterfaceContent.Menu.SquareBoxA'
		ImageStyle=ISTY_Stretched
		WinTop=0.0
		WinLeft0.0
		WinHeight=1.0
		WinWidth=1.0
		bScaleToParent=true
		bBoundToParent=true
	End Object

	Begin Object Class=GUILabel Name=lblQuestion
		WinTop=0.2
		WinLeft=0.1
		WinHeight=0.4
		WinWidth=0.8
		bMultiLine=true
//		bVisible=false
	End Object
	Controls(0)=imgBack
	Controls(1)=lblQuestion

	WinTop=0.25;
	WinHeight=0.5;
	BackgroundColor=(R=64,G=64,B=64,A=255)
	BackgroundRStyle=MSTY_Alpha

	ButtonNames(0)="Ok"
	ButtonNames(1)="Cancel"
	ButtonNames(2)="Retry"
	ButtonNames(3)="Continue"
	ButtonNames(4)="Yes"
	ButtonNames(5)="No"
	ButtonNames(6)="Abort"
	ButtonNames(7)="Ignore"
	// if add more buttonnames, make sure to expand size of array declaration
}