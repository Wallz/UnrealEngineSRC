class Painter extends Weapon
    config(user);

#exec OBJ LOAD FILE=XGameShaders.utx

var(Gfx) float borderX;
var(Gfx) float borderY;

var(Gfx) float focusX;
var(Gfx) float focusY;
var(Gfx) float innerArrowsX;
var(Gfx) float innerArrowsY;
var(Gfx) Color ArrowColor;
var(Gfx) Color FocusColor;
var(Gfx) Color ChargeColor;

var(Gfx) vector RechargeOrigin;
var(Gfx) vector RechargeSize;

var transient float LastFOV;
var bool zoomed;

var Vector EndEffect;
var vector MarkLocation;
var IonCannon FirstCannon;

function PrebeginPlay()
{
	Super.PreBeginPlay();
	if ( Level.IsDemoBuild() )
		Destroy();
}

simulated function PostBeginPlay()
{
    Super.PostBeginPlay();
    
    if ( Role == ROLE_Authority )
    {
		foreach DynamicActors(class'IonCannon', FirstCannon)	
			if ( FirstCannon.bFirstCannon )
				break;
	}
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

	local float barOrgX;
	local float barOrgY;
	local float barSizeX;
	local float barSizeY;
    
    local PainterFire PainterFire;

    //    FireSound=Sound'WeaponSounds.LightningGun.LightningScope'
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
		PainterFire = PainterFire(FireMode[0]);
        if (PainterFire.bIsFiring && PainterFire.bValidMark && Level.TimeSeconds - PainterFire.MarkTime > 0.4)
        {
            charge = FMin(1.0, ((Level.TimeSeconds - PainterFire.MarkTime) / PainterFire.PaintDuration));
            if (charge >= 1.0) charge = 0.0;
        }
        else
            charge = 0.0;
       
		tileScaleX = Canvas.SizeX / 640.0f;
		tileScaleY = Canvas.SizeY / 480.0f;

		bX = borderX * tileScaleX;
		bY = borderY * tileScaleY;
		fX = focusX * tileScaleX;
		fY = focusY * tileScaleX;
		
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
    if( Instigator.Controller.IsA( 'PlayerController' ) )
        LastFOV = PlayerController(Instigator.Controller).DesiredFOV;
    Super.BringUp();
}

simulated function bool PutDown()
{
    if( Instigator.Controller.IsA( 'PlayerController' ) )
        PlayerController(Instigator.Controller).EndZoom();
    return Super.PutDown();
}


simulated function bool HasAmmo()
{
    return (FireMode[0] != None && Ammo[0] != None && Ammo[0].AmmoAmount >= 1);
}

function IonCannon CheckMark(vector MarkLocation, bool bFire)
{
	local IonCannon I;
	
	For ( I=FirstCannon; I!=None; I=I.nextCannon )
		if ( I.CheckMark(Instigator,MarkLocation,bFire) )
			return I;
			
	return None;
}

// AI Interface
function float GetAIRating()
{
	local Bot B;
	
	B = Bot(Instigator.Controller);
	if ( B == None )
		return AIRating;
	if ( (B.Enemy == None) || (Instigator.Location.Z < B.Enemy.Location.Z) || !B.EnemyVisible() )
		return 0;
	MarkLocation = B.Enemy.Location - B.Enemy.CollisionHeight * vect(0,0,1);
	if ( CheckMark(MarkLocation, false) != None )
		return 2.0;
	if ( TerrainInfo(B.Enemy.Base) == None )
		return 0;
	return 0.1; 		
}

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	return 0;
}

function float SuggestAttackStyle()
{
    return 0;
}

function float SuggestDefenseStyle()
{
    return 2;
}

function float RangedAttackTime()
{
	return PainterFire(FireMode[0]).PaintDuration + 0.2;
}

function bool RecommendRangedAttack()
{ 
	return true;
}

// End AI Interface


defaultproperties
{
    FireModeClass(0)=PainterFire
    FireModeClass(1)=PainterZoom
    InventoryGroup=0
    DrawScale=1.0
    Mesh=mesh'Weapons.Painter_1st'
    ItemName="Ion Painter"
    BobDamping=1.575000
    PickupClass=class'PainterPickup'
    EffectOffset=(X=100,Y=25,Z=-3)
    PutDownAnim=PutDown
    DisplayFOV=60
    PlayerViewOffset=(X=25,Y=2,Z=-1)
    PlayerViewPivot=(Pitch=0,Roll=0,Yaw=0)
    UV2Texture=Material'XGameShaders.WeaponEnvShader'
    AttachmentClass=class'PainterAttachment'
    SelectSound=Sound'WeaponSounds.LinkGun.SwitchToLinkGun'
	IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=322,Y1=372,X2=444,Y2=462)
	SelectForce="SwitchToLinkGun"
	
    AIRating=1.0
    CurrentRating=1.0

    borderX=60.0
    borderY=60.0
    focusX=135
    focusY=105
    innerArrowsX=42.0
    innerArrowsY=42.0
    ChargeColor=(R=255,G=255,B=255,A=255)
    FocusColor=(R=71,G=90,B=126,A=215)
    ArrowColor=(R=200,G=200,B=200,A=255)
    RechargeOrigin=(X=600,Y=330,Z=0)
	RechargeSize=(X=10,Y=-180,Z=0)
	
	bNotInDemo=true
	DemoReplacement=class'xWeapons.SniperRifle'

	DefaultPriority=11
}
