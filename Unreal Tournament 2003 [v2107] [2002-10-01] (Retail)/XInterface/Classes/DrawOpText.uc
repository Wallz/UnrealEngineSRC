class DrawOpText extends DrawOpBase;

var string			Text;
var string			FontName;			// Font to use
var int				MaxLines;			// In case a multiline is too tall.
var byte			Justification;		// How to justify multiline text
var byte			WrapMode;			// Mode: 0 = Normal, 1=Clipped, 2=Justified, 3=Multiline

function Draw(Canvas Canvas)
{
local Font Fnt;

	Super.Draw(Canvas);
	if (FontName != "")
	{
		Fnt = GetFont(FontName, Canvas.SizeX);
		if (Fnt != None)
			Canvas.Font = Fnt;
	}

	switch(WrapMode)
	{
	case 3:	// Multiline
		DrawMultiLine(Canvas);
		break;
	case 2: // Justified
		Canvas.DrawTextJustified(Text, Justification, Lft * Canvas.SizeX, Top * Canvas.SizeY, (Lft + Width) * Canvas.SizeX, (Top + Height) * Canvas.SizeY);
		break;
	case 1: // Clipped
		Canvas.ClipX = (Top+Width) * Canvas.SizeX;
		Canvas.DrawTextClipped(Text);
		Canvas.ClipX = Canvas.SizeX;
		break;
	default:
		Canvas.DrawText(Text);
	}
}

function DrawMultiLine(Canvas Canvas)
{
local string TextCopy, Line;
local int ltp;
local float XL, YL, xTop, xRight;

	ltp = MaxLines;
	xTop = Top * Canvas.SizeY;
	xRight = (Lft + Width) * Canvas.SizeY;
	TextCopy = Text;
	Canvas.WrapText(TextCopy, Line, Width * Canvas.SizeX, Canvas.Font, 1.0);
	while (ltp > 0)
	{
		Canvas.TextSize(Line, XL, YL);
		if (Len(Line) > 0)
			Canvas.DrawTextJustified(Line, Justification, Lft * Canvas.SizeX, xTop, xRight, xTop + YL);

		xTop += YL;
		Canvas.WrapText(TextCopy, Line, Width * Canvas.SizeX, Canvas.Font, 1.0);
		if (TextCopy == "" && Line == "")
			break;
	}
}

defaultproperties
{
	MaxLines=99
	FontName="XInterface.UT2DefaultFont"
}