class MinigunAttachment extends xWeaponAttachment;

var class<MinigunMuzFlash3rd>     mMuzFlashClass;
var class<MinigunMuzAltFlash3rd>  mMuzAltFlashClass;
var xEmitter            mMuzFlash3rd;

var class<xEmitter>     mTracerClass;
var xEmitter            mTracer;
var float               mTracerFreq;
var float               mTracerLen;
var float               mLastTracerTime;
var bool                mbGotHit;
var byte OldSpawnHitCount;
var vector              mTracerLoc;
var rotator             mHitRot;
var float               mTracerUpdateTime;

var float               mCurrentRoll;
var float               mRollInc;
var float               mRollUpdateTime;

var class<xEmitter>     mShellCaseEmitterClass;
var xEmitter            mShellCaseEmitter;
var() vector            mShellEmitterOffset;


function Destroyed()
{
    if (mTracer != None)
        mTracer.Destroy();

    if (mMuzFlash3rd != None)
        mMuzFlash3rd.Destroy();

    if (mShellCaseEmitter != None)
        mShellCaseEmitter.Destroy();

	Super.Destroyed();
}

simulated function UpdateRoll(float dt)
{
    local rotator r;

    UpdateRollTime(false);

    if (mRollInc <= 0.f)
        return;

    mCurrentRoll += dt*mRollInc;
    mCurrentRoll = mCurrentRoll % 65536.f;
    r.Roll = int(mCurrentRoll);

    SetBoneRotation('Bone Barrels', r, 0, 1.f);
}

simulated function UpdateRollTime(bool bUpdate)
{
    local float diff;

    diff = Level.TimeSeconds - mRollUpdateTime;

    if (bUpdate)
        mRollUpdateTime = Level.TimeSeconds;

    // TODO: clean up!
    if (diff > 0.2)
    {
        mbGotHit = false;
        mRollInc = 0.f;
    }
}

simulated function vector GetTracerStart()
{
    local Pawn p;

    p = Pawn(Owner);

    if ( (p != None) && p.IsFirstPerson() && p.Weapon != None )
    {
        // 1st person
        return p.Weapon.GetEffectStart();
    }

    // 3rd person
	if ( mMuzFlash3rd != None )
		return mMuzFlash3rd.Location;
	else
		return Location;
}

simulated function UpdateTracer()
{
    local float len;
    local float invSpeed, hitDist;

    if (Level.NetMode == NM_DedicatedServer)
        return;

    if (mTracer == None)
    {
        mTracer = Spawn(mTracerClass);
        AttachToBone(mTracer, 'tip');
    }
    if ( Level.bDropDetail || Level.DetailMode == DM_Low )
		mTracerFreq = 2 * Default.mTracerFreq;
	else
		mTracerFreq = Default.mTracerFreq;

    if (mTracer != None && mbGotHit && 
        Level.TimeSeconds > mLastTracerTime + mTracerFreq)
    {
        mTracer.SetLocation(mTracerLoc);
        mTracer.SetRotation(mHitRot);

		hitDist = VSize(mHitLocation - mTracerLoc);

        len = mTracerLen * hitDist;
        invSpeed = 1.f / mTracer.mSpeedRange[1];

        mTracer.mLifeRange[0] = len * invSpeed;
        mTracer.mLifeRange[1] = mTracer.mLifeRange[0];
        mTracer.mSpawnVecB.Z = -1.f * (1.0-mTracerLen) * hitDist * invSpeed;
        mTracer.mStartParticles = 1;

        mLastTracerTime = Level.TimeSeconds;

        GotoState('TickTracer');
    }
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
    local class<MinigunMuzFlash3rd> muzClass;

    if ( (Level.NetMode == NM_DedicatedServer) || (Instigator == None) 
		|| ((Level.TimeSeconds - LastRenderTime > 0.2) && (PlayerController(Instigator.Controller) == None)) )
        return;
	WeaponLight();

    if ( FlashCount > 0 )
	{
        if (FiringMode == 0)
        {
            muzClass = mMuzFlashClass;
            mTracerFreq = 0.11f;
            mRollInc = 65536.f*3.f;
        }
        else
        {
            muzClass = mMuzAltFlashClass;
            mTracerFreq = 0.33f;
            mRollInc = 65536.f;
        } 

        UpdateRollTime(true);
		mTracerLoc = GetTracerStart();
		
		mHitRot = rotator(mHitLocation - Instigator.Location);
		mTracerUpdateTime = Level.TimeSeconds;
		mbGotHit = true;
        UpdateTracer();
		if ( OldSpawnHitCount != SpawnHitCount )
		{
			OldSpawnHitCount = SpawnHitCount;
			GetHitInfo();
			if ( FiringMode == 0 )
				Spawn(class'HitEffect'.static.GetHitEffect(mHitActor, mHitLocation, mHitNormal),,, mHitLocation, Rotator(mHitNormal));
			else
				Spawn(class'XEffects.ExploWallHit',,, mHitLocation, Rotator(mHitNormal));
		}
        if (mMuzFlash3rd == None)
        {
            mMuzFlash3rd = Spawn(mMuzFlashClass);
            AttachToBone(mMuzFlash3rd, 'tip');
        }
        if (mMuzFlash3rd != None)
        {
            mMuzFlash3rd.Trigger(self, None);
            r.Roll = Rand(65536);
            SetBoneRotation('Bone Flash', r, 0, 1.f);
        }

        if ( (mShellCaseEmitter == None) && (Level.DetailMode != DM_Low) )
        {
            mShellCaseEmitter = Spawn(mShellCaseEmitterClass);
            if ( mShellCaseEmitter != None )
				AttachToBone(mShellCaseEmitter, 'shell');
        }
        if (mShellCaseEmitter != None)
            mShellCaseEmitter.mStartParticles++;
    }
    else
    {
        GotoState('');
    }

    Super.ThirdPersonEffects();
}

state TickTracer
{
    simulated function Tick(float deltaTime)
    {
        UpdateRoll(deltaTime);
    }
}

defaultproperties
{
    mTracerLen=0.8
    mTracerFreq=0.15
    mTracerClass=class'XEffects.Tracer'
    mMuzFlashClass=class'XEffects.MinigunMuzFlash3rd'
    mMuzAltFlashClass=class'XEffects.MinigunMuzAltFlash3rd'
    mShellCaseEmitterClass=class'XEffects.ShellSpewer'
    
    bHeavy=true
    bRapidFire=true
    bAltRapidFire=true
    Mesh=mesh'Weapons.Minigun_3rd'

    bDynamicLight=false
    LightType=LT_Pulse
    LightEffect=LE_NonIncidence
    LightPeriod=3
    LightBrightness=150
    LightHue=30
    LightSaturation=150
    LightRadius=4.0
}
