//=============================================================================
// SuperShockRifle
//=============================================================================
class SuperShockRifle extends ShockRifle;

#exec OBJ LOAD FILE=..\Sounds\WeaponSounds.uax
#exec OBJ LOAD FILE=XEffectMat.utx

function float GetAIRating()
{
	return AIRating;
}

simulated function bool StartFire(int mode)
{
	bWaitForCombo = false;
	return Super.StartFire(mode);
}

function float RangedAttackTime()
{
	return 0;
}

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	return 0;
}

defaultproperties
{
    bCanThrow=false
	AIRating=+1.0
	
    FireModeClass(0)=SuperShockBeamFire
    FireModeClass(1)=SuperShockBeamFire
    InventoryGroup=4
    ItemName="Super Shock Rifle"
    PickupClass=class'SuperShockRiflePickup'
}
