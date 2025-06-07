//=============================================================================
// Assault Rifle
//=============================================================================
class AssaultRifle extends Weapon
    config(user);

#EXEC OBJ LOAD FILE=InterfaceContent.utx

simulated function PostNetBeginPlay()
{
	Super.PostNetBeginPlay();
	
	if ( (Role < ROLE_Authority) && (Instigator != None) && (Instigator.Controller != None) && (Instigator.Weapon != self) && (Instigator.PendingWeapon != self) )
		Instigator.Controller.ClientSwitchToBestWeapon();
}

simulated function DrawWeaponInfo(Canvas Canvas)
{
	local int i,Count;
	local float ScaleFactor;
	
	Count = Min(8,Ammo[1].AmmoAmount);
	ScaleFactor = 99 * Canvas.ClipX/1600;
	Canvas.Style = ERenderStyle.STY_Alpha;
	Canvas.DrawColor = class'HUD'.Default.WhiteColor;
	
    for( i=0; i<Count; i++ )
    {
		Canvas.SetPos(Canvas.ClipX - (0.5*i+1) * ScaleFactor, 0.75 * Canvas.ClipY);
		Canvas.DrawTile( Material'InterfaceContent.Hud.SkinA', ScaleFactor, ScaleFactor, 0, 520, 99, 99);
	}
}

// Allow fire modes to return to idle on weapon switch (server)
simulated function DetachFromPawn(Pawn P)
{
    AssaultGrenade(FireMode[1]).ReturnToIdle();
    Super.DetachFromPawn(P);
}

function byte BestMode()
{
	if ( (FRand() < 0.1) && (Ammo[1].AmmoAmount >= FireMode[1].AmmoPerFire) )
		return 1;
    if ( Ammo[0].AmmoAmount >= FireMode[0].AmmoPerFire )
		return 0;
	return 1;
}

simulated function float ChargeBar()
{
	return FMin(1,FireMode[1].HoldTime/AssaultGrenade(FireMode[1]).mHoldClampMax);
} 

defaultproperties
{
    ItemName="Assault Rifle"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=200,Y1=100,X2=321,Y2=189)

	bShowChargingBar=true
    bCanThrow=false
    FireModeClass(0)=AssaultFire
    FireModeClass(1)=AssaultGrenade
    InventoryGroup=2
    Mesh=mesh'Weapons.AssaultRifle_1st'
    BobDamping=1.7
    PickupClass=class'AssaultRiflePickup'
    EffectOffset=(X=100.0,Y=25.0,Z=-10.0)
    PutDownAnim=PutDown
    DisplayFOV=70
    AttachmentClass=class'AssaultAttachment'
    
    DrawScale=1.0
    PlayerViewOffset=(X=-8,Y=5,Z=-6)
    PlayerViewPivot=(Pitch=400,Roll=0,Yaw=0)
    UV2Texture=Material'XGameShaders.WeaponEnvShader'
    SelectSound=Sound'WeaponSounds.AssaultRifle.SwitchToAssaultRifle'
	SelectForce="SwitchToAssaultRifle"

	AIRating=+0.4
    CurrentRating=0.4

    bDynamicLight=false
    LightType=LT_Pulse
    LightEffect=LE_NonIncidence
    LightPeriod=3
    LightBrightness=150
    LightHue=30
    LightSaturation=150
    LightRadius=4.0

	DefaultPriority=3
}
