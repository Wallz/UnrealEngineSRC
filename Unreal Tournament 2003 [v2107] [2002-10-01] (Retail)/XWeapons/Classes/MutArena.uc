class MutArena extends Mutator
    config(user);

var() config string ArenaWeaponClassName;
var bool bInitialized;
var class<Weapon> ArenaWeaponClass;
var string ArenaWeaponPickupClassName;
var string ArenaAmmoPickupClassName;

function Initialize()
{
    local int FireMode;

	bInitialized = true;
	DefaultWeaponName = ArenaWeaponClassName;
	ArenaWeaponClass = class<Weapon>(DynamicLoadObject(ArenaWeaponClassName,class'Class'));
	DefaultWeapon = ArenaWeaponClass;
	ArenaWeaponPickupClassName = string(ArenaWeaponClass.default.PickupClass);
    for( FireMode = 0; FireMode<2; FireMode++ )
    {
        if( (ArenaWeaponClass.default.FireModeClass[FireMode] != None)
        && (ArenaWeaponClass.default.FireModeClass[FireMode].default.AmmoClass != None)
        && (ArenaWeaponClass.default.FireModeClass[FireMode].default.AmmoClass.default.PickupClass != None) )
        {
			ArenaAmmoPickupClassName = string(ArenaWeaponClass.default.FireModeClass[FireMode].default.AmmoClass.default.PickupClass);
			break;
		}
	}
}

function bool CheckReplacement( Actor Other, out byte bSuperRelevant )
{
    if ( !bInitialized )
		Initialize();
		
	bSuperRelevant = 0;
    if ( xWeaponBase(Other) != None )
		xWeaponBase(Other).WeaponType = ArenaWeaponClass;
    else if ( (Weapon(Other) != None) && (Other.Class != ArenaWeaponClass) )
    {
		if ( Other.IsA('BallLauncher') )
            return true;
        ReplaceWith(Other,ArenaWeaponClassName);
 		return false;
 	}
	else if ( (WeaponPickup(Other) != None) && (string(Other.Class) != ArenaWeaponPickupClassName) )
		ReplaceWith( Other, ArenaWeaponPickupClassName);
	else if ( (Ammo(Other) != None) && (string(Other.Class) != ArenaAmmoPickupClassName) )
		ReplaceWith( Other, ArenaAmmoPickupClassName);
	else
		return true;
	return false;
}    

defaultproperties
{
    IconMaterialName="MutatorArt.nosym"
    ConfigMenuClassName="XInterface.UT2ArenaConfig"
    GroupName="Arena"
    FriendlyName="Arena"
    Description="Replace weapons and ammo in map."
    ArenaWeaponClassName="XWeapons.RocketLauncher"
}
