class GUIScrollTextBox extends GUIListBoxBase
	native;

var GUIScrollText MyScrollText;

var bool			bRepeat;		// Should the sequence be repeated ?
var float			InitialDelay;	// Initial delay after new content was set
var float			CharDelay;		// This is the delay between each char
var float			EOLDelay;		// This is the delay to use when reaching end of line
var float			RepeatDelay;	// This is used after all the text has been displayed and bRepeat is true
var	eTextAlign		TextAlign;			// How is text Aligned in the control

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	MyScrollText = GUIScrollText(Controls[0]);
	MyScrollText.InitialDelay = InitialDelay;
	MyScrollText.CharDelay = CharDelay;
	MyScrollText.EOLDelay = EOLDelay;
	MyScrollText.RepeatDelay = RepeatDelay;
	MyScrollText.TextAlign = TextAlign;
	MyScrollText.bRepeat = bRepeat;
	MyScrollText.OnADjustTop  = InternalOnAdjustTop;
	Super.Initcomponent(MyController, MyOwner);
}

function SetContent(string NewContent, optional string sep)
{
	MyScrollText.SetContent(NewContent, sep);
}

function Restart()
{
	MyScrollText.Restart();
}

function Stop()
{
	MyScrollText.Stop();
}

function InternalOnAdjustTop(GUIComponent Sender)
{
	MyScrollText.EndScrolling();
	
}

defaultproperties
{
	Begin Object Class=GUIScrollText Name=TheText
	End Object

	Controls(0)=GUIScrollText'TheText'

	TextAlign=TXTA_Left
	InitialDelay=0.0
	CharDelay=0.25
	EOLDelay=0.75
	RepeatDelay=3.0
}
