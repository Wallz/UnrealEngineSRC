//=============================================================================
// Shock Rifle
//=============================================================================
class ShockRifle extends Weapon
    config(user);

#EXEC OBJ LOAD FILE=InterfaceContent.utx

#exec OBJ LOAD FILE=..\Sounds\WeaponSounds.uax
#exec OBJ LOAD FILE=XEffectMat.utx

var ShockProjectile ComboTarget;	// used by AI
var bool			bRegisterTarget;
var	bool			bWaitForCombo;
var vector			ComboStart;

// AI Interface
function float GetAIRating()
{
	local Bot B;

	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return AIRating;

	if ( bWaitForCombo )
		return 1.5;
	if ( !B.ProficientWithWeapon() )
		return AIRating;
	if ( B.Stopped() )
		return (AIRating + 0.3);
	else if ( VSize(B.Enemy.Location - Instigator.Location) > 1600 )
		return (AIRating + 0.1); 
	else if ( B.Enemy.Location.Z > B.Location.Z + 200 )
		return (AIRating + 0.15);

	return AIRating;
}

function SetComboTarget(ShockProjectile S)
{
	if ( !bRegisterTarget || (bot(Instigator.Controller) == None) || (Instigator.Controller.Enemy == None) )
		return;

	bRegisterTarget = false;
	ComboStart = Instigator.Location;
	ComboTarget = S;
	ComboTarget.Monitor(Bot(Instigator.Controller).Enemy);
}

function float RangedAttackTime()
{
	local bot B;

	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return 0;

	if ( B.CanComboMoving() )
		return 0;

	return FMin(2,0.3 + VSize(B.Enemy.Location - Instigator.Location)/class'ShockProjectile'.default.Speed);
}

simulated function bool StartFire(int mode)
{
	if ( bWaitForCombo && (Bot(Instigator.Controller) != None) )
	{
		if ( (ComboTarget == None) || ComboTarget.bDeleteMe )
			bWaitForCombo = false;
		else 
			return false;
	}
	return Super.StartFire(mode);
}

function DoCombo()
{
	if ( bWaitForCombo )
	{
		bWaitForCombo = false;
		if ( (Instigator != None) && (Instigator.Weapon == self) )
			StartFire(0);
	}
}

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	local float EnemyDist, MaxDist;
	local bot B;

	bWaitForCombo = false;
	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return 0;

	EnemyDist = VSize(B.Enemy.Location - Instigator.Location);
	if ( B.Skill > 5 )
		MaxDist = 3 * class'ShockProjectile'.default.Speed;
	else
		MaxDist = 2 * class'ShockProjectile'.default.Speed;
	if ( (EnemyDist > MaxDist) || (EnemyDist < 150) )
	{
		ComboTarget = None;
		return 0;
	}

	if ( (ComboTarget != None) && !ComboTarget.bDeleteMe && B.CanCombo() )
	{
		bWaitForCombo = true;
		return 0;
	}
	
	ComboTarget = None;

	if ( (EnemyDist > 2000) && (FRand() < 0.5) )
		return 0; 
	
	if ( B.CanCombo() && B.ProficientWithWeapon() )
	{
		bRegisterTarget = true;
		return 1;
	}
	if ( FRand() < 0.7 )
		return 0;
	return 1;
}
// end AI Interface

defaultproperties
{
    ItemName="Shock Rifle"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=322,Y1=190,X2=444,Y2=280)

    FireModeClass(0)=ShockBeamFire
    FireModeClass(1)=ShockProjFire
    InventoryGroup=4
    Mesh=mesh'Weapons.ShockRifle_1st'
    BobDamping=1.8
    PickupClass=class'ShockRiflePickup'
    EffectOffset=(X=65,Y=14,Z=-10)
    IdleAnim=Idle
    SelectAnim=Pickup
    PutDownAnim=PutDown

    UV2Texture=Material'XGameShaders.WeaponEnvShader'

    DrawScale=1.0
    PlayerViewOffset=(X=-10,Y=3,Z=-2)
    PlayerViewPivot=(Pitch=1000,Roll=-500,Yaw=-800)
    DisplayFOV=60
    SelectSound=Sound'WeaponSounds.ShockRifle.SwitchToShockRifle'
    AttachmentClass=class'ShockAttachment'
	SelectForce="SwitchToShockRifle"

    Skins(0)=Material'WeaponSkins.ShockTex0' // these are set in weapons.ukx to shaders but...
    Skins(1)=Material'WeaponSkins.ShockTex0'
    Skins(2)=Shader'WeaponSkins.ShockLaser.LaserShader'

	AIRating=+0.63
	CurrentRating=+0.63
	
    bDynamicLight=false
    LightType=LT_Steady
    LightEffect=LE_NonIncidence
    LightPeriod=3
    LightBrightness=150
    LightHue=200
    LightSaturation=70
    LightRadius=4.0

	DefaultPriority=4
}
