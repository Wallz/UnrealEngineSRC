//=============================================================================
// xWeaponBase
//=============================================================================
class xWeaponBase extends xPickUpBase
    placeable;

var() class<Weapon> WeaponType;

function PrebeginPlay()
{
	Super.PreBeginPlay();
	if ( Level.IsDemoBuild() && WeaponType.Default.bNotInDemo )
		WeaponType = WeaponType.Default.DemoReplacement;
}

simulated function PostBeginPlay()
{
	if (WeaponType != None)
		PowerUp = WeaponType.default.PickupClass;
    Super.PostBeginPlay();
}

defaultproperties
{
    SpiralEmitter=class'XEffects.Spiral'
    
    DrawScale=0.75
    DrawType=DT_StaticMesh
    StaticMesh=XGame_rc.WildcardChargerMesh
    Skins(0)=Texture'XGameTextures.WildcardChargerTex'
    Skins(1)=Texture'XGameTextures.WildcardChargerTex'
    Texture=None
    
    CollisionRadius=60.000000
    CollisionHeight=6.000000
}
