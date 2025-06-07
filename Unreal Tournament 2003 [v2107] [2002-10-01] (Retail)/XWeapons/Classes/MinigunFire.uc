class MinigunFire extends InstantFire;

// For controlling the roll of the barrel
var() float MaxRollSpeed;
var() float RollSpeed;
var() float BarrelRotationsPerSec;
var() int   RoundsPerRotation;
var() float FireTime;
var() Sound WindingSound;
var() Sound FiringSound;
var MiniGun Gun;
var() float WindUpTime;

var() String FiringForce;
var() String WindingForce;

function StartBerserk()
{
    DamageMin = default.DamageMin * 1.33;
    DamageMax = default.DamageMax * 1.33;
}

function StopBerserk()
{
    DamageMin = default.DamageMin;
    DamageMax = default.DamageMax;
}

function PostBeginPlay()
{
    Super.PostBeginPlay();
    FireRate = 1.f / (RoundsPerRotation * BarrelRotationsPerSec);
    MaxRollSpeed = 65536.f*BarrelRotationsPerSec;
    Gun = Minigun(Owner);
}

function FlashMuzzleFlash()
{
    local rotator r;
    r.Roll = Rand(65536);
    Weapon.SetBoneRotation('Bone_Flash', r, 0, 1.f);
    Super.FlashMuzzleFlash();
}

function InitEffects()
{
    Super.InitEffects();
    if ( FlashEmitter != None )
		Weapon.AttachToBone(FlashEmitter, 'flash');
}

function PlayAmbientSound(Sound aSound)
{
    if ( (Minigun(Weapon) == None) || (Instigator == None) || (aSound == None && ThisModeNum != Gun.CurrentMode) )
        return;

    Instigator.AmbientSound = aSound;
    Gun.CurrentMode = ThisModeNum;
}

function StopRolling()
{
    if (Gun == None || ThisModeNum != Gun.CurrentMode)
        return;

    RollSpeed = 0.f;
    Gun.RollSpeed = 0.f;
}

function PlayPreFire() {}
function PlayStartHold() {}
function PlayFiring() {}
function PlayFireEnd() {}
function StartFiring();
function StopFiring();
function bool IsIdle()
{
	return false;
}

auto state Idle
{
	function bool IsIdle()
	{
		return true;
	}

    function BeginState()
    {
        PlayAmbientSound(None);
        StopRolling();
    }

    function EndState()
    {
        PlayAmbientSound(WindingSound);
    }

    function StartFiring()
    {
        RollSpeed = 0;
		FireTime = (RollSpeed/MaxRollSpeed) * WindUpTime;
        GotoState('WindUp');
    }
}

state WindUp
{
    function BeginState()
    {
        ClientPlayForceFeedback(WindingForce);  // jdf
    }

    function EndState()
    {
        if (ThisModeNum == 0)
        {
            if (!Weapon.FireMode[1].bIsFiring)
                StopForceFeedback(WindingForce);
        }
        else
        {
            if (!Weapon.FireMode[0].bIsFiring)
                StopForceFeedback(WindingForce);
        }        
    }

    function ModeTick(float dt)
    {
        FireTime += dt;
        RollSpeed = (FireTime/WindUpTime) * MaxRollSpeed;

        if ( !bIsFiring )
        {
			GotoState('WindDown');
			return;
		}
        
        if (RollSpeed >= MaxRollSpeed)
        {
            RollSpeed = MaxRollSpeed;
            FireTime = WindUpTime;
            Gun.UpdateRoll(dt, RollSpeed, ThisModeNum);
			GotoState('FireLoop');
            return;
        }

        Gun.UpdateRoll(dt, RollSpeed, ThisModeNum);
    }

    function StopFiring()
    {
        GotoState('WindDown');
    }
}

state FireLoop
{
    function BeginState()
    {
        NextFireTime = Level.TimeSeconds - 0.1; //fire now!
        PlayAmbientSound(FiringSound);
        ClientPlayForceFeedback(FiringForce);  // jdf
        Gun.LoopAnim(FireLoopAnim, FireLoopAnimRate, TweenTime);
        Gun.SpawnShells(RoundsPerRotation*BarrelRotationsPerSec);
    }

    function StopFiring()
    {
        GotoState('WindDown');
    }

    function EndState()
    {
        PlayAmbientSound(WindingSound);
        StopForceFeedback(FiringForce);  // jdf
        Gun.LoopAnim(Gun.IdleAnim, Gun.IdleAnimRate, TweenTime);
        Gun.SpawnShells(0.f);
     }

    function ModeTick(float dt)
    {
        Super.ModeTick(dt);
        Gun.UpdateRoll(dt, RollSpeed, ThisModeNum);
        if ( !bIsFiring )
        {
			GotoState('WindDown');
			return;
		}
    }
}

state WindDown
{
    function BeginState()
    {
        ClientPlayForceFeedback(WindingForce);  // jdf
    }

    function EndState()
    {
        if (ThisModeNum == 0)
        {
            if ( (Weapon == None) || !Weapon.FireMode[1].bIsFiring )
                StopForceFeedback(WindingForce);
        }
        else
        {
            if ( (Weapon == None) || !Weapon.FireMode[0].bIsFiring )
                StopForceFeedback(WindingForce);
        }        
    }

    function ModeTick(float dt)
    {
        FireTime -= dt;
        RollSpeed = (FireTime/WindUpTime) * MaxRollSpeed;
         
        if (RollSpeed <= 0.f)
        {
            RollSpeed = 0.f;
            FireTime = 0.f;
            Gun.UpdateRoll(dt, RollSpeed, ThisModeNum);
            GotoState('Idle');
            return;
        }

        Gun.UpdateRoll(dt, RollSpeed, ThisModeNum);
    }

    function StartFiring()
    {
        GotoState('WindUp');
    }
}

defaultproperties
{
	Momentum=+2000.0
    RoundsPerRotation=5
    BarrelRotationsPerSec=3.0

    AmmoClass=class'MinigunAmmo'
    AmmoPerFire=1
    DamageType=class'DamTypeMinigunBullet'
    DamageMin=6
    DamageMax=6
    bPawnRapidFireAnim=true
    SpreadStyle=SS_Random
    Spread=0.08

    FireLoopAnimRate=9.f
    WindUpTime=0.5f
    PreFireTime=0.5f
    TweenTime=0.1f

    FlashEmitterClass=class'XEffects.MinigunMuzFlash1st'
    SmokeEmitterClass=class'xEffects.MinigunMuzzleSmoke'

    FiringSound=Sound'WeaponSounds.MiniGun.MiniFireb'
    WindingSound=Sound'WeaponSounds.MiniGun.MiniEmpty'
    FiringForce="minifireb"  // jdf
    WindingForce="miniempty"  // jdf

    BotRefireRate=0.99
    AimError=900

    ShakeOffsetMag=(X=1.0,Y=1.0,Z=1.0)
    ShakeOffsetRate=(X=1000.0,Y=1000.0,Z=1000.0)
    ShakeOffsetTime=2
    ShakeRotMag=(X=50.0,Y=50.0,Z=50.0)
    ShakeRotRate=(X=10000.0,Y=10000.0,Z=10000.0)
    ShakeRotTime=2
}
 