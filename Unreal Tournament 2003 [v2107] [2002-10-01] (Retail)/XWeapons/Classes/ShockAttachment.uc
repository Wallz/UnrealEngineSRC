class ShockAttachment extends xWeaponAttachment;

var class<xEmitter>     MuzFlashClass;
var xEmitter            MuzFlash;

simulated function Destroyed()
{
    if (MuzFlash != None)
        MuzFlash.Destroy();

    Super.Destroyed();
}

simulated event ThirdPersonEffects()
{
    local rotator r;

    if ( Level.NetMode != NM_DedicatedServer && FlashCount > 0 )
	{
		if ( FiringMode == 0 )
			WeaponLight();
        else
        {
            if (MuzFlash == None)
            {
                MuzFlash = Spawn(MuzFlashClass);
                AttachToBone(MuzFlash, 'tip');
            }
            if (MuzFlash != None)
            {
                MuzFlash.mStartParticles++;
                r.Roll = Rand(65536);
                SetBoneRotation('Bone_Flash', r, 0, 1.f);
            }
        }
    }

    Super.ThirdPersonEffects();
}

defaultproperties
{
    MuzFlashClass=class'XEffects.ShockProjMuzFlash3rd'
    bHeavy=false
    bRapidFire=false
    bAltRapidFire=false
    Mesh=mesh'Weapons.ShockRifle_3rd'
    
    bDynamicLight=false
    LightType=LT_Steady
    LightEffect=LE_NonIncidence
    LightPeriod=3
    LightBrightness=150
    LightHue=200
    LightSaturation=70
    LightRadius=4.0
}
