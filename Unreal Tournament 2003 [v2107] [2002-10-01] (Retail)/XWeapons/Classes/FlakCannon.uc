//=============================================================================
// Flak Cannon
//=============================================================================
class FlakCannon extends Weapon
    config(user);

#EXEC OBJ LOAD FILE=InterfaceContent.utx

// AI Interface
function float GetAIRating()
{
	local Bot B;
	local float EnemyDist;
	local vector EnemyDir;

	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return AIRating;

	EnemyDir = B.Enemy.Location - Instigator.Location;
	EnemyDist = VSize(EnemyDir);
	if ( EnemyDist > 750 )
	{
		if ( EnemyDist > 2000 )
		{
			if ( EnemyDist > 3500 )
				return 0.2;
			return (AIRating - 0.3);
		}			
		if ( EnemyDir.Z < -0.5 * EnemyDist )
			return (AIRating - 0.3);
	}
	else if ( (B.Enemy.Weapon != None) && B.Enemy.Weapon.bMeleeWeapon )
		return (AIRating + 0.35);
	else if ( EnemyDist < 400 )
		return (AIRating + 0.2);
	return FMax(AIRating + 0.2 - (EnemyDist - 400) * 0.0008, 0.2);
}

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	local vector EnemyDir;
	local float EnemyDist;
	local bot B;

	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return 0;

	EnemyDir = B.Enemy.Location - Instigator.Location;
	EnemyDist = VSize(EnemyDir);
	if ( EnemyDist > 750 )
	{
		if ( EnemyDir.Z < -0.5 * EnemyDist )
			return 1;
		return 0;
	}
	else if ( (B.Enemy.Weapon != None) && B.Enemy.Weapon.bMeleeWeapon )
		return 0;
	else if ( (EnemyDist < 400) || (EnemyDir.Z > 30) )
		return 0;
	else if ( FRand() < 0.65 )
		return 1;
	return 0;
}

function float SuggestAttackStyle()
{
	if ( (AIController(Instigator.Controller) != None)
		&& (AIController(Instigator.Controller).Skill < 3) )
		return 0.4;
    return 0.8;
}

function float SuggestDefenseStyle()
{
    return -0.4;
}
// End AI Interface

defaultproperties
{
    ItemName="Flak Cannon"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=322,Y1=100,X2=444,Y2=200)

    FireModeClass(0)=FlakFire
    FireModeClass(1)=FlakAltFire
    InventoryGroup=7
    Mesh=mesh'Weapons.Flak_1st'
    BobDamping=1.4
    PickupClass=class'FlakCannonPickup'
    EffectOffset=(X=200.0,Y=32.0,Z=-25.0)
    AttachmentClass=class'FlakAttachment'
    PutDownAnim=PutDown

    DisplayFOV=60
    DrawScale=1.0
    PlayerViewOffset=(X=-7,Y=8,Z=0)
    PlayerViewPivot=(Pitch=0,Roll=200,Yaw=16884)
    UV2Texture=Material'XGameShaders.WeaponEnvShader'
    SelectSound=Sound'WeaponSounds.FlakCannon.SwitchToFlakCannon'
	SelectAnim=Pickup
	SelectForce="SwitchToFlakCannon"
	
	AIRating=+0.75
	CurrentRating=+0.75
	
    bDynamicLight=false
    LightType=LT_Steady
    LightEffect=LE_NonIncidence
    LightBrightness=150
    LightHue=30
    LightSaturation=150
    LightRadius=4.0

	DefaultPriority=8
}
