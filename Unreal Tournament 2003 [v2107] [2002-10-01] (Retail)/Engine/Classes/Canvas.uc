//=============================================================================
// Canvas: A drawing canvas.
// This is a built-in Unreal class and it shouldn't be modified.
//
// Notes.
//   To determine size of a drawable object, set Style to STY_None,
//   remember CurX, draw the thing, then inspect CurX and CurYL.
//=============================================================================
class Canvas extends Object
	native
	noexport;

// added for drawing solid primatives.
#exec TEXTURE IMPORT NAME=WhiteTexture FILE=Textures\White.tga MIPS=0
#exec TEXTURE IMPORT NAME=BlackTexture FILE=Textures\Black.tga MIPS=0
#exec TEXTURE IMPORT NAME=GrayTexture  FILE=Textures\Gray.tga MIPS=0

// simple default font, so various stuff doesn't crash
#exec Font Import File=Textures\SmallFont.bmp Name="DefaultFont"

// Modifiable properties.
var font    Font;            // Font for DrawText.
var float   FontScaleX, FontScaleY; // Scale for DrawText & DrawTextClipped. // gam
var float   SpaceX, SpaceY;  // Spacing for after Draw*.
var float   OrgX, OrgY;      // Origin for drawing.
var float   ClipX, ClipY;    // Bottom right clipping region.
var float   CurX, CurY;      // Current position for drawing.
var float   Z;               // Z location. 1=no screenflash, 2=yes screenflash.
var byte    Style;           // Drawing style STY_None means don't draw.
var float   CurYL;           // Largest Y size since DrawText.
var color   DrawColor;       // Color for drawing.
var bool    bCenter;         // Whether to center the text.
var bool    bNoSmooth;       // Don't bilinear filter.
var const int SizeX, SizeY;  // Zero-based actual dimensions.
var Plane   ColorModulate;   // sjs - Modulate all colors by this before rendering

var bool    bRenderLevel;    // gam - Will render the level if enabled.

// Stock fonts.
var font TinyFont, SmallFont, MedFont;
var string TinyFontName, SmallFontName, MedFontName;

// Internal.
var const viewport Viewport; // Viewport that owns the canvas.
var const int      pCanvasUtil; // sjs

// native functions.
native(464) final function StrLen( coerce string String, out float XL, out float YL ); // Wrapped!
native(465) final function DrawText( coerce string Text, optional bool CR );
native(466) final function DrawTile( material Mat, float XL, float YL, float U, float V, float UL, float VL );
native(467) final function DrawActor( Actor A, bool WireFrame, optional bool ClearZ, optional float DisplayFOV );
native(468) final function DrawTileClipped( Material Mat, float XL, float YL, float U, float V, float UL, float VL );
native(469) final function DrawTextClipped( coerce string Text, optional bool bCheckHotKey );
native(470) final function TextSize( coerce string String, out float XL, out float YL ); // Clipped!
native(480) final function DrawPortal( int X, int Y, int Width, int Height, actor CamActor, vector CamLocation, rotator CamRotation, optional int FOV, optional bool ClearZ );
native final function vector WorldToScreen( vector WorldLoc );
native final function GetCameraLocation( out vector CameraLocation, out rotator CameraRotation );

native final function SetScreenLight( int index, vector Position, color lightcolor, float radius );
native final function SetScreenProjector( int index, vector Position, color color, float radius, texture tex );
native final function DrawScreenActor( Actor A, optional float FOV, optional bool WireFrame, optional bool ClearZ );
native final function Clear(optional bool ClearRGB, optional bool ClearZ);
native final function WrapStringToArray(string Text, out array<string> OutArray, float dx, string EOL);
static native final function WrapText( out String Text, out String Line, float dx, Font F, float FontScaleX );


// jmw - These are two helper functions.  The use the whole texture only.  If you need better support, use DrawTile

native final function DrawTileStretched(material Mat, float XL, float YL);
native final function DrawTileJustified(material Mat, byte Justification, float XL, float YL);
native final function DrawTileScaled(material Mat, float XScale, float YScale);
native final function DrawTextJustified(coerce string String, byte Justification, float x1, float y1, float x2, float y2);

// UnrealScript functions.
event Reset()
{
	Font        = Default.Font;
	FontScaleX  = Default.FontScaleX; // gam
	FontScaleY  = Default.FontScaleY; // gam
	SpaceX      = Default.SpaceX;
	SpaceY      = Default.SpaceY;
	OrgX        = Default.OrgX;
	OrgY        = Default.OrgY;
	CurX        = Default.CurX;
	CurY        = Default.CurY;
	Style       = Default.Style;
	DrawColor   = Default.DrawColor;
	CurYL       = Default.CurYL;
	bCenter     = false;
	bNoSmooth   = false;
	Z           = 1.0;
    ColorModulate = Default.ColorModulate; // sjs
}
final function SetPos( float X, float Y )
{
	CurX = X;
	CurY = Y;
}
final function SetOrigin( float X, float Y )
{
	OrgX = X;
	OrgY = Y;
}
final function SetClip( float X, float Y )
{
	ClipX = X;
	ClipY = Y;
}
final function DrawPattern( material Tex, float XL, float YL, float Scale )
{
	DrawTile( Tex, XL, YL, (CurX-OrgX)*Scale, (CurY-OrgY)*Scale, XL*Scale, YL*Scale );
}
final function DrawIcon( texture Tex, float Scale )
{
	if ( Tex != None )
		DrawTile( Tex, Tex.USize*Scale, Tex.VSize*Scale, 0, 0, Tex.USize, Tex.VSize );
}
final function DrawRect( texture Tex, float RectX, float RectY )
{
	DrawTile( Tex, RectX, RectY, 0, 0, Tex.USize, Tex.VSize );
}

final function SetDrawColor(byte R, byte G, byte B, optional byte A)
{
	local Color C;
	
	C.R = R;
	C.G = G;
	C.B = B;
	if ( A == 0 )
		A = 255;
	C.A = A;
	DrawColor = C;
}

static final function Color MakeColor(byte R, byte G, byte B, optional byte A)
{
	local Color C;
	
	C.R = R;
	C.G = G;
	C.B = B;
	if ( A == 0 )
		A = 255;
	C.A = A;
	return C;
}

// Draw a vertical line
final function DrawVertical(float X, float height)
{
    SetPos( X, CurY);
    DrawRect(Texture'engine.WhiteSquareTexture', 2, height);
}

// Draw a horizontal line
final function DrawHorizontal(float Y, float width)
{
    SetPos(CurX, Y);
    DrawRect(Texture'engine.WhiteSquareTexture', width, 2);
}

// Draw Line is special as it saves it's original position

final function DrawLine(int direction, float size)
{
    local float X, Y;

    // Save current position
    X = CurX;
    Y = CurY;

    switch (direction) 
    {
      case 0:
		  SetPos(X, Y - size);
		  DrawRect(Texture'engine.WhiteSquareTexture', 2, size);
		  break;
    
      case 1:
		  DrawRect(Texture'engine.WhiteSquareTexture', 2, size);
		  break;

      case 2:
		  SetPos(X - size, Y);
		  DrawRect(Texture'engine.WhiteSquareTexture', size, 2);
		  break;
		  
	  case 3:
		  DrawRect(Texture'engine.WhiteSquareTexture', size, 2);
		  break;
    }
    // Restore position
    SetPos(X, Y);
}

final simulated function DrawBracket(float width, float height, float bracket_size)
{
    local float X, Y;
    X = CurX;
    Y = CurY;

	Width  = max(width,5);
	Height = max(height,5);
	
    DrawLine(3, bracket_size);
    DrawLine(1, bracket_size);
    SetPos(X + width, Y);
    DrawLine(2, bracket_size);
    DrawLine(1, bracket_size);
    SetPos(X + width, Y + height);
    DrawLine(0, bracket_size);
    DrawLine(2, bracket_size);
    SetPos(X, Y + height);
    DrawLine(3, bracket_size);
    DrawLine( 0, bracket_size);

    SetPos(X, Y);
}

final simulated function DrawBox(canvas canvas, float width, float height)
{
	local float X, Y;
	X = canvas.CurX;
	Y = canvas.CurY;
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', 2, height);
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', width, 2);
	canvas.SetPos(X + width, Y);
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', 2, height);
	canvas.SetPos(X, Y + height);
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', width+1, 2);
	canvas.SetPos(X, Y);
}

simulated function DrawScreenText (String Text, float X, float Y, EDrawPivot Pivot)
{
    local int TextScreenWidth, TextScreenHeight;
    local float UL, VL;

    X *= SizeX;
    Y *= SizeY;

	TextSize (Text, UL, VL);

    TextScreenWidth = UL;
    TextScreenHeight = VL;

    switch (Pivot)
    {
        case DP_UpperLeft:
            break;

        case DP_UpperMiddle:
            X -= TextScreenWidth / 2;
            break;

        case DP_UpperRight:
            X -= TextScreenWidth;
            break;

        case DP_MiddleRight:
            X -= TextScreenWidth;
            Y -= TextScreenHeight / 2;
            break;

        case DP_LowerRight:
            X -= TextScreenWidth;
            Y -= TextScreenHeight;
            break;

        case DP_LowerMiddle:
            X -= TextScreenWidth / 2;
            Y -= TextScreenHeight;
            break;

        case DP_LowerLeft:
            Y -= TextScreenHeight;
            break;

        case DP_MiddleLeft:
            Y -= TextScreenHeight / 2;
            break;

        case DP_MiddleMiddle:
            X -= TextScreenWidth / 2;
            Y -= TextScreenHeight / 2;
            break;
    }
    
	SetPos (X, Y);
    DrawTextClipped (Text);
}

defaultproperties
{
     Style=1
     FontScaleX=1.0
     FontScaleY=1.0
	 Z=1
     DrawColor=(R=127,G=127,B=127,A=255)
	 TinyFontName="UT2003Fonts.FontMono"
     SmallFontName="UT2003Fonts.FontMono"
     MedFontName="UT2003Fonts.FontMono800x600"
     pCanvasUtil=0
     ColorModulate=(X=1.0,Y=1.0,Z=1.0,W=1.0)
     bRenderLevel=true
	 Font=DefaultFont
}
