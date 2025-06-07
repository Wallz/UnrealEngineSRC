class TestVignette extends Vignette;

#exec OBJ LOAD FILE=InterfaceContent.utx

var() String Backgrounds[3];
var() transient Texture Background;

var() Texture Logo;
var() float LogoPosX, LogoPosY;
var() float LogoScaleX, LogoScaleY;

var() Font LoadingFont;
var() localized String LoadingString;
var() float LoadingPosX, LoadingPosY;
var() Color LoadingColor;

simulated event Init()
{
    local int i;

    Super.PreBeginPlay();

    i = Rand( ArrayCount( Backgrounds ) );

    Background = Texture( DynamicLoadObject( Backgrounds[i], class'Texture') );

    if( Background == None )
        log( Backgrounds[i] $" not found for Vignette", 'Error' );
}
simulated function ScreenText(Canvas C, String Text, float posX, float posY, float ScaleX,float ScaleY)
{
    C.Style = ERenderStyle.STY_Alpha;
    C.Font = LoadingFont;
    C.DrawColor = LoadingColor;
	C.FontScaleX = ScaleX;
	C.FontScaleY = ScaleY;
    C.DrawScreenText( Text, posX, posY, DP_MiddleMiddle );
}
simulated event DrawVignette( Canvas C, float Progress )
{
    local float ResScaleX, ResScaleY;
    local float PosX, PosY, DX, DY;

    C.Reset();

    ResScaleX = C.SizeX / 640.0;
    ResScaleY = C.SizeY / 480.0;

	C.Style = ERenderStyle.STY_Alpha;
    C.DrawColor = C.MakeColor( 255, 255, 255 );

    C.SetPos( 0, 0 );
	C.DrawTile( Background, C.SizeX, C.SizeY, 0, 0, Background.USize, Background.VSize );

    DX = Logo.USize * ResScaleX * LogoScaleX;
    DY = Logo.VSize * ResScaleY * LogoScaleY;
    PosX = (LogoPosX * C.SizeX) - (DX * 0.5);
    PosY = (LogoPosY * C.SizeY) - (DY * 0.5);

    C.SetPos( PosX, PosY );
	C.Style = ERenderStyle.STY_Alpha;
    C.DrawTile( Logo, DX, DY, 0, 0, Logo.USize, Logo.VSize );
/*
	C.Style = ERenderStyle.STY_Alpha;
    C.Font = LoadingFont;
    C.DrawColor = LoadingColor;
	C.FontScaleX = ResScaleX;
	C.FontScaleY = ResScaleY;
    C.DrawScreenText( LoadingString, LoadingPosX, LoadingPosY, DP_MiddleMiddle );
*/
    ScreenText( C, LoadingString, LoadingPosX, LoadingPosY, ResScaleX, ResScaleY );

}

defaultproperties
{
    Backgrounds[0]="InterfaceContent.Backgrounds.bg09"
    Backgrounds[1]="InterfaceContent.Backgrounds.bg10"
    Backgrounds[2]="InterfaceContent.Backgrounds.bg11"
 
    Logo=Texture'InterfaceContent.Logos.Logo'
    LogoScaleX=0.5
    LogoScaleY=0.5
    LogoPosX=0.49
    LogoPosY=0.25

    LoadingFont=Font'FontLarge'
    LoadingString=". . . L O A D I N G . . ."
    LoadingPosX=0.5
    LoadingPosY=0.9
    LoadingColor=(R=255,G=255,B=255,A=255)
}
