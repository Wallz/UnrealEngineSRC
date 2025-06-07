class BioChargedFire extends ProjectileFire;

var() float GoopUpRate;
var() int MaxGoopLoad;
var() int GoopLoad;
var() Sound HoldSound;

function InitEffects()
{
    Super.InitEffects();
    if( FlashEmitter == None )
        FlashEmitter = Weapon.FireMode[0].FlashEmitter;
}

function DrawMuzzleFlash(Canvas Canvas)
{
	if ( FlashEmitter != None )
		FlashEmitter.SetRotation(Weapon.Rotation);
    Super.DrawMuzzleFlash(Canvas);
}

function ModeHoldFire()
{
    if (Weapon.Ammo[ThisModeNum].AmmoAmount > 0)
    {
        Super.ModeHoldFire();
        GotoState('Hold');
    }
}

function float MaxRange()
{
	return 1500;
}

simulated function bool AllowFire()
{
    return (Weapon.Ammo[ThisModeNum].AmmoAmount > 0 || GoopLoad > 0);
}

simulated function PlayStartHold()
{
    Weapon.PlayAnim('AltFire', 1.0 / (GoopUpRate*MaxGoopLoad), 0.1);
}

simulated function PlayFiring()
{
	Super.PlayFiring();
	Weapon.OutOfAmmo();
}

state Hold
{
    simulated function BeginState()
    {
        GoopLoad = 0;
        SetTimer(GoopUpRate, true);
        Weapon.PlaySound(sound'WeaponSounds.Biorifle_charge',SLOT_Interact,TransientSoundVolume);
        ClientPlayForceFeedback( "BioRiflePowerUp" );  // jdf
        Timer();
    }

    simulated function Timer()
    {
		if ( Weapon.Ammo[ThisModeNum].AmmoAmount > 0 )
			GoopLoad++;
        Weapon.ConsumeAmmo(ThisModeNum, 1);
        if (GoopLoad == MaxGoopLoad || Weapon.Ammo[ThisModeNum].AmmoAmount == 0)
        {
            FreezeAnimAt(float(GoopLoad)/float(MaxGoopLoad), 0);
            SetTimer(0.0, false);
            GotoState('');
        }
    }

    simulated function EndState()
    {
        StopForceFeedback( "BioRiflePowerUp" );  // jdf
    }
}

function projectile SpawnProjectile(Vector Start, Rotator Dir)
{
    local BioGlob Glob;    

    GotoState('');

    if (GoopLoad == 0) return None;

    Glob = Spawn(class'BioGlob',,, Start, Dir);
    Glob.Damage *= DamageAtten;
    Glob.SetGoopLevel(GoopLoad);

    GoopLoad = 0;
    if ( Weapon.Ammo[ThisModeNum].AmmoAmount <= 0 )
        Weapon.OutOfAmmo();
    return Glob;
}

function StartBerserk()
{
    GoopUpRate = default.GoopUpRate*0.75;
}

function StopBerserk()
{
    GoopUpRate = default.GoopUpRate;
}

defaultproperties
{
    AmmoClass=class'BioAmmo'
    AmmoPerFire=0

    FireAnim=Fire
    FireEndAnim=None

    ProjectileClass=class'XWeapons.BioGlob'
    FireRate=0.33
    bFireOnRelease=true
    GoopUpRate=0.25
    MaxGoopLoad=10

    ProjSpawnOffset=(X=20,Y=9,Z=-6)
    FlashEmitterClass=None
    FireSound=Sound'WeaponSounds.BioRifle.BioRifleFire'
    HoldSound=Sound'WeaponSounds.BioRifle.BioRiflePowerUp'
    
    FireForce="BioRifleFire";  // jdf
    
    bSplashDamage=true
    bRecommendSplashDamage=true
    BotRefireRate=0.5
    bTossed=true
    WarnTargetPct=+0.8

    ShakeOffsetMag=(X=-4.0,Y=0.0,Z=-4.0)
    ShakeOffsetRate=(X=1000.0,Y=0.0,Z=1000.0)
    ShakeOffsetTime=2
    ShakeRotMag=(X=100.0,Y=0.0,Z=0.0)
    ShakeRotRate=(X=1000.0,Y=0.0,Z=0.0)
    ShakeRotTime=2
}
