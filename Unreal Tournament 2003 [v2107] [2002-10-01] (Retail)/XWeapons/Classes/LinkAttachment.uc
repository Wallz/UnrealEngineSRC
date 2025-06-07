class LinkAttachment extends xWeaponAttachment;

var LinkMuzFlashProj3rd MuzFlash;
var int Links;

replication
{
    reliable if (Role == ROLE_Authority)
        Links;
}

simulated function Destroyed()
{
    if (MuzFlash != None)
        MuzFlash.Destroy();

    Super.Destroyed();
}

simulated event ThirdPersonEffects()
{
    local Rotator R;

    if ( Level.NetMode != NM_DedicatedServer && FlashCount > 0 )
	{
        if (FiringMode == 0)
        {
            if (MuzFlash == None)
            {
                MuzFlash = Spawn(class'LinkMuzFlashProj3rd');
                AttachToBone(MuzFlash, 'tip');
            }
            if (MuzFlash != None)
            {
                MuzFlash.mSizeRange[0] = MuzFlash.default.mSizeRange[0] * (class'LinkFire'.default.LinkScale[Min(Links,5)]+1); // (1.0 + 0.3*float(Links));
                MuzFlash.mSizeRange[1] = MuzFlash.mSizeRange[0];
                if (Links > 0)
                    MuzFlash.Skins[0] = FinalBlend'XEffectMat.LinkMuzProjYellowFB';
                else
                    MuzFlash.Skins[0] = FinalBlend'XEffectMat.LinkMuzProjGreenFB';
                MuzFlash.Trigger(self, None);
                R.Roll = Rand(65536);
                SetBoneRotation('bone flashA', R, 0, 1.0);
            }
        }
    }

    Super.ThirdPersonEffects();
}

defaultproperties
{
    bHeavy=false
    bRapidFire=true
    bAltRapidFire=true
    Mesh=mesh'Weapons.LinkGun_3rd'
}