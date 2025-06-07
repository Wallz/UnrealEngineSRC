//=============================================================================
// ZoomSuperShockRifle
//=============================================================================
class ZoomSuperShockRifle extends SuperShockRifle;

defaultproperties
{
    FireModeClass(1)=SniperZoom
}


var(Gfx) float testX;
var(Gfx) float testY;

var(Gfx) float borderX;
var(Gfx) float borderY;

var(Gfx) float focusX;
var(Gfx) float focusY;
var(Gfx) float innerArrowsX;
var(Gfx) float innerArrowsY;
var(Gfx) Color ArrowColor;
var(Gfx) Color TargetColor;
var(Gfx) Color NoTargetColor;
var(Gfx) Color FocusColor;
var(Gfx) Color ChargeColor;

var(Gfx) vector RechargeOrigin;
var(Gfx) vector RechargeSize;

var transient float LastFOV;
var() bool zoomed;
var() xEmitter  chargeEmitter;

simulated function PostBeginPlay()
{
    Super.PostBeginPlay();
}

simulated function Destroyed()
{
    if (chargeEmitter != None)
        chargeEmitter.Destroy();

    Super.Destroyed();
}

simulated function ClientWeaponThrown()
{
    if( Instigator != None && Instigator.Controller.IsA( 'PlayerController' ) )
        PlayerController(Instigator.Controller).EndZoom();
    Super.ClientWeaponThrown();
}

// compensate for bright fog
simulated function SetZoomBlendColor(Canvas c)
{
    local Byte    val;
    local Color   clr;
    local Color   fog;

    clr.R = 255;
    clr.G = 255;
    clr.B = 255;
    clr.A = 255;

    if( Instigator.Region.Zone.bDistanceFog )
    {
        fog = Instigator.Region.Zone.DistanceFogColor;
        val = 0;
        val = Max( val, fog.R);
        val = Max( val, fog.G);
        val = Max( val, fog.B);

        if( val > 128 )
        {
            val -= 128;
            clr.R -= val;
            clr.G -= val;
            clr.B -= val;
        }
    }
    c.DrawColor = clr;
}

simulated event RenderOverlays( Canvas Canvas )
{
	local float tileScaleX;
	local float tileScaleY;
	local float bX;
	local float bY;
	local float fX;
	local float fY;
	local float charge;

	local float tX;
	local float tY;

	local float barOrgX;
	local float barOrgY;
	local float barSizeX;
	local float barSizeY;

    if ( LastFOV > PlayerController(Instigator.Controller).DesiredFOV )
    {
        PlaySound(Sound'WeaponSounds.LightningGun.LightningZoomIn', SLOT_Misc,,,,,false);
    }
    else if ( LastFOV < PlayerController(Instigator.Controller).DesiredFOV )
    {
        PlaySound(Sound'WeaponSounds.LightningGun.LightningZoomOut', SLOT_Misc,,,,,false);
    }
    LastFOV = PlayerController(Instigator.Controller).DesiredFOV;

    if (PlayerController(Instigator.Controller).DesiredFOV == PlayerController(Instigator.Controller).DefaultFOV)
	{
        Super.RenderOverlays(Canvas);
		zoomed=false;
	}
	else
    {
		
		if ( FireMode[0].NextFireTime <= Level.TimeSeconds )
		{
			charge = 1.0;
		}
		else
		{
			charge = 1.0 - ((FireMode[0].NextFireTime-Level.TimeSeconds) / FireMode[0].FireRate);
		}

		tileScaleX = Canvas.SizeX / 640.0f;
		tileScaleY = Canvas.SizeY / 480.0f;

		bX = borderX * tileScaleX;
		bY = borderY * tileScaleY;
		fX = focusX * tileScaleX;
		fY = focusY * tileScaleX;
		
		tX = testX * tileScaleX;
		tY = testY * tileScaleX;

		barOrgX = RechargeOrigin.X * tileScaleX;
		barOrgY = RechargeOrigin.Y * tileScaleY;

		barSizeX = RechargeSize.X * tileScaleX;
		barSizeY = RechargeSize.Y * tileScaleY;

        SetZoomBlendColor(Canvas);

        Canvas.Style = 255;
		Canvas.SetPos(0,0);
        Canvas.DrawTile( Material'ZoomFB', Canvas.SizeX, Canvas.SizeY, 0.0, 0.0, 512, 512 ); // !! hardcoded size
        
		// draw border corners
        Canvas.Style = ERenderStyle.STY_Alpha;
		Canvas.SetPos(0,0);
		Canvas.DrawTile( Texture'SniperBorder', bX, bY, 0.0, 0.0, Texture'SniperBorder'.USize, Texture'SniperBorder'.VSize );

		Canvas.SetPos(Canvas.SizeX-bX,0);
		Canvas.DrawTile( Texture'SniperBorder', bX, bY, 0.0, 0.0, -Texture'SniperBorder'.USize, Texture'SniperBorder'.VSize );

		Canvas.SetPos(Canvas.SizeX-bX,Canvas.SizeY-bY);
		Canvas.DrawTile( Texture'SniperBorder', bX, bY, 0.0, 0.0, -Texture'SniperBorder'.USize, -Texture'SniperBorder'.VSize );

		Canvas.SetPos(0,Canvas.SizeY-bY);
		Canvas.DrawTile( Texture'SniperBorder', bX, bY, 0.0, 0.0, Texture'SniperBorder'.USize, -Texture'SniperBorder'.VSize );


		Canvas.DrawColor = FocusColor;
        Canvas.DrawColor.A = 255; // 255 was the original -asp. WTF??!?!?!
		Canvas.Style = ERenderStyle.STY_Alpha;

		Canvas.SetPos((Canvas.SizeX*0.5)-fX,(Canvas.SizeY*0.5)-fY);
		Canvas.DrawTile( Texture'SniperFocus', fX*2.0, fY*2.0, 0.0, 0.0, Texture'SniperFocus'.USize, Texture'SniperFocus'.VSize );
	
        fX = innerArrowsX * tileScaleX;
		fY = innerArrowsY * tileScaleY;
        
        Canvas.DrawColor = ArrowColor;
        Canvas.SetPos((Canvas.SizeX*0.5)-fX,(Canvas.SizeY*0.5)-fY);
		Canvas.DrawTile( Texture'SniperArrows', fX*2.0, fY*2.0, 0.0, 0.0, Texture'SniperArrows'.USize, Texture'SniperArrows'.VSize );

		
		// Draw the Charging meter  -AsP 
		Canvas.DrawColor = ChargeColor;
        Canvas.DrawColor.A = 255; 

		if(charge <1)
		    Canvas.DrawColor.R = 255*charge;
		else
        {
            Canvas.DrawColor.R = 0;
		    Canvas.DrawColor.B = 0;
        }
        
		if(charge == 1)
		    Canvas.DrawColor.G = 255;
		else
		    Canvas.DrawColor.G = 0;
		
		Canvas.Style = ERenderStyle.STY_Alpha;
		Canvas.SetPos( barOrgX, barOrgY );
		Canvas.DrawTile(Texture'Engine.WhiteTexture',barSizeX,barSizeY*charge, 0.0, 0.0,Texture'Engine.WhiteTexture'.USize,Texture'Engine.WhiteTexture'.VSize*charge);
		zoomed = true;
	}
}

simulated function ClientStartFire(int mode)
{
    if (mode == 1)
    {
        FireMode[mode].bIsFiring = true;
        if( Instigator.Controller.IsA( 'PlayerController' ) )
            PlayerController(Instigator.Controller).ToggleZoom();
    }
    else
    {
        Super.ClientStartFire(mode);
    }
}

simulated function ClientStopFire(int mode)
{
    if (mode == 1)
    {
        FireMode[mode].bIsFiring = false;
        if( Instigator.Controller.IsA( 'PlayerController' ) )
            PlayerController(Instigator.Controller).StopZoom();
    }
    else
    {
        Super.ClientStopFire(mode);
    }
}

simulated function BringUp(optional Weapon PrevWeapon)
{
    if ( PlayerController(Instigator.Controller) != None )
        LastFOV = PlayerController(Instigator.Controller).DesiredFOV;
    Super.BringUp();
}

simulated function bool PutDown()
{
    if( Instigator.Controller.IsA( 'PlayerController' ) )
        PlayerController(Instigator.Controller).EndZoom();
    if ( Super.PutDown() )
    {
		GotoState('');
		return true;
	}
	return false;
}

defaultproperties
{
	borderX=60.0
	borderY=60.0

	focusX=135
	focusY=105

	testX=100
	testY=100

    innerArrowsX=42.0
    innerArrowsY=42.0

	ChargeColor=(R=255,G=255,B=255,A=255)
    FocusColor=(R=71,G=90,B=126,A=215)
    NoTargetColor=(R=200,G=200,B=200,A=255)
    TargetColor=(R=255,G=255,B=255,A=255)
    ArrowColor=(R=255,G=0,B=0,A=255)

    UV2Texture=Material'XGameShaders.WeaponEnvShader'

    RechargeOrigin=(X=600,Y=330,Z=0)
	RechargeSize=(X=10,Y=-180,Z=0)
	zoomed=false

    bSniping=true
}
