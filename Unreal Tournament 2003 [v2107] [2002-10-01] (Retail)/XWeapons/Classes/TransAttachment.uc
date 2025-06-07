class TransAttachment extends xWeaponAttachment;

function InitFor(Inventory I)
{
    Super.InitFor(I);
}

simulated event ThirdPersonEffects()
{
    Super.ThirdPersonEffects();
}

defaultproperties
{
    bHeavy=true
    bRapidFire=false
    bAltRapidFire=false
    Mesh=mesh'Weapons.TransLauncher_3rd'
}
 