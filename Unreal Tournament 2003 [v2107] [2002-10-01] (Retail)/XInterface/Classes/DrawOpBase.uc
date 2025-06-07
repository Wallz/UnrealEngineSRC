class DrawOpBase extends Object;

var float			Top, Lft, Height, Width;	// Location/size is always range 0.0 to 1.0
var byte 			RenderStyle;				// Render Style to use for this particular Image
var Color			DrawColor;					// Color to set Canvas to.

function SetPos(float T, float L)
{
	Top = T;
	Lft = L;
}

function SetSize(float H, float W)
{
	Height = H;
	Width = W;
}

function Draw(Canvas Canvas)
{
	Canvas.SetPos(Lft * Canvas.SizeX, Top * Canvas.SizeY);
	Canvas.Style = RenderStyle;
	Canvas.DrawColor = DrawColor;
}

simulated function Font GetFont(string FontClassName, float ResX)
{
local class<GUIFont>	FntCls;
local GUIFont Fnt;

	FntCls = class<GUIFont>(DynamicLoadObject(FontClassName, class'Class'));
	if (FntCls != None)
		Fnt = new(None) FntCls;

	if (Fnt == None)
	{
		Log("FONT NOT FOUND");
		return None;
	}

	return Fnt.GetFont(ResX);
}

defaultproperties
{
	DrawColor=(R=255,G=255,B=255,A=255)
	RenderStyle=1
}