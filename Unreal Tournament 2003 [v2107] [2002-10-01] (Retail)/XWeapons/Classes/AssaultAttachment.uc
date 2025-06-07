class AssaultAttachment extends xWeaponAttachment;

var byte OldSpawnHitCount;
var class<xEmitter>     mMuzFlashClass;
var xEmitter            mMuzFlash3rd;
var xEmitter            mMuzFlash3rdAlt;

simulated function Destroyed()
{
    if (mMuzFlash3rd != None)
        mMuzFlash3rd.Destroy();
    if (mMuzFlash3rdAlt != None)
        mMuzFlash3rdAlt.Destroy();
    Super.Destroyed();
}

/* UpdateHit
- used to update properties so hit effect can be spawn client side
*/
function UpdateHit(Actor HitActor, vector HitLocation, vector HitNormal)
{
	SpawnHitCount++;
	mHitLocation = HitLocation;
	mHitActor = HitActor;
	mHitNormal = HitNormal;
}

simulated event ThirdPersonEffects()
{
    local rotator r;

    if ( Level.NetMode != NM_DedicatedServer )
	{
        if (FiringMode == 0)
        {
			WeaponLight();
			if ( OldSpawnHitCount != SpawnHitCount )
			{
				OldSpawnHitCount = SpawnHitCount;
				GetHitInfo();
				Spawn(class'HitEffect'.static.GetHitEffect(mHitActor, mHitLocation, mHitNormal),,, mHitLocation, Rotator(mHitNormal));
			}
			if (mMuzFlash3rd == None)
            {
                mMuzFlash3rd = Spawn(mMuzFlashClass);
                AttachToBone(mMuzFlash3rd, 'tip');
            }
            mMuzFlash3rd.mStartParticles++;
            r.Roll = Rand(65536);
            SetBoneRotation('Bone_Flash', r, 0, 1.f);
        }
        else if (FiringMode == 1 && FlashCount > 0)
        {
			WeaponLight();
            if (mMuzFlash3rdAlt == None)
            {
                mMuzFlash3rdAlt = Spawn(mMuzFlashClass);
                AttachToBone(mMuzFlash3rdAlt, 'tip2');
            }
            mMuzFlash3rdAlt.mStartParticles++;
            r.Roll = Rand(65536);
            SetBoneRotation('Bone_Flash02', r, 0, 1.f);
        }
    }

    Super.ThirdPersonEffects();
}

defaultproperties
{
    Mesh=mesh'Weapons.AssaultRifle_3rd'
    mMuzFlashClass=class'XEffects.AssaultMuzFlash3rd'
    bHeavy=false
    bRapidFire=true
    bAltRapidFire=false

    bDynamicLight=false
    LightType=LT_Pulse
    LightEffect=LE_NonIncidence
    LightPeriod=3
    LightBrightness=150
    LightHue=30
    LightSaturation=150
    LightRadius=4.0
}
