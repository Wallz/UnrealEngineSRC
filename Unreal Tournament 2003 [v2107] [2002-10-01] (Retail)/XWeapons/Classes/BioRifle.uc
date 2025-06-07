class BioRifle extends Weapon
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

	// if retreating, favor this weapon
	EnemyDir = B.Enemy.Location - Instigator.Location;
	EnemyDist = VSize(EnemyDir);
	if ( EnemyDist > 1500 )
		return 0.1;
	if ( B.IsRetreating() )
		return (AIRating + 0.4);	
	if ( -1 * EnemyDir.Z > EnemyDist )
		return AIRating + 0.1;
	if ( (B.Enemy.Weapon != None) && B.Enemy.Weapon.bMeleeWeapon )
		return (AIRating + 0.3);
	if ( EnemyDist > 1000 )
		return 0.35;
	return AIRating;
}

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	if ( FRand() < 0.8 )
		return 0;
	return 1;
}

function float SuggestAttackStyle()
{
	local Bot B;
	local float EnemyDist;
	
	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return 0.4;
	
	EnemyDist = VSize(B.Enemy.Location - Instigator.Location);
	if ( EnemyDist > 1500 )
		return 1.0;
	if ( EnemyDist > 1000 )
		return 0.4;
	return -0.4;
}

function float SuggestDefenseStyle()
{
	local Bot B;

	B = Bot(Instigator.Controller);
	if ( (B == None) || (B.Enemy == None) )
		return 0;

	if ( VSize(B.Enemy.Location - Instigator.Location) < 1600 )
		return -0.6;
	return 0;
}

// End AI Interface

simulated function AnimEnd(int Channel)
{
    local name anim;
    local float frame, rate;
    GetAnimParams(0, anim, frame, rate);

    if (anim == 'AltFire')
        LoopAnim('Hold', 1.0, 0.1);
    else
        Super.AnimEnd(Channel);
}

simulated function bool HasAmmo()
{
    return ( (Ammo[0].AmmoAmount >= 1) || FireMode[1].bIsFiring );
}

defaultproperties
{
    ItemName="Bio-Rifle"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=322,Y1=281,X2=444,Y2=371)

    FireModeClass(0)=BioFire
    FireModeClass(1)=BioChargedFire
    InventoryGroup=3

    Mesh=mesh'Weapons.BioRifle_1st'
    BobDamping=2.2
    PickupClass=class'BioRiflePickup'
    EffectOffset=(X=100.0,Y=32.0,Z=-20.0)
    AttachmentClass=class'BioAttachment'
    PutDownAnim=PutDown

    DisplayFOV=60
    DrawScale=1.0
    PlayerViewOffset=(X=7,Y=3,Z=0)
    PlayerViewPivot=(Pitch=0,Roll=0,Yaw=0)
    UV2Texture=Material'XGameShaders.WeaponEnvShader'
    SelectSound=Sound'WeaponSounds.FlakCannon.SwitchToFlakCannon'
	SelectForce="SwitchToFlakCannon"

	AIRating=+0.55
	CurrentRating=+0.55

	DefaultPriority=6
}
