class Redeemer extends Weapon
    config(user);

function PrebeginPlay()
{
	Super.PreBeginPlay();
	if ( Level.IsDemoBuild() )
		Destroy();
}

simulated event ClientStopFire(int Mode)
{
    if (Role < ROLE_Authority)
    {
        StopFire(Mode);
    }
    if ( Mode == 0 )
		ServerStopFire(Mode);    
}

simulated event WeaponTick(float dt)
{
	if ( (Instigator.Controller == None) || HasAmmo() )
		return;
	Instigator.Controller.SwitchToBestWeapon();
}


// AI Interface
function float SuggestAttackStyle()
{
    return -1.0;
}

function float SuggestDefenseStyle()
{
    return -1.0;
}

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	return 0;
}

function float GetAIRating()
{
	local Bot B;

	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return 0.4;

	if ( VSize(B.Enemy.Location - Instigator.Location) < 2400 )
		return 0.4;
	
	return AIRating;
}

defaultproperties
{
    FireModeClass(0)=RedeemerFire
    FireModeClass(1)=RedeemerGuidedFire
    InventoryGroup=0
    GroupOffset=1
    Mesh=mesh'Weapons.Redeemer_1st'
    ItemName="Redeemer"
    BobDamping=1.4
    PickupClass=class'RedeemerPickup'
    EffectOffset=(X=0.0,Y=0.0,Z=-0.0)
    AttachmentClass=class'RedeemerAttachment'
    PutDownAnim=PutDown

    DisplayFOV=60
    DrawScale=1.2
    PlayerViewOffset=(X=14,Y=0,Z=-28)
    PlayerViewPivot=(Pitch=1000,Roll=0,Yaw=-400)
    UV2Texture=Material'XGameShaders.WeaponEnvShader'
    SelectSound=Sound'WeaponSounds.Redeemer_change'
	IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=322,Y1=7,X2=444,Y2=98)
	SelectForce="SwitchToFlakCannon"

	AIRating=1.5
	CurrentRating=1.5
	SelectAnim=Pickup
	
	bNotInDemo=true
	DemoReplacement=class'xWeapons.RocketLauncher'

	DefaultPriority=12
}
