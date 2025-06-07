
class WeaponFire extends Actor
    native;

var() Weapon Weapon;
var() int ThisModeNum;

// animation //
var() Name PreFireAnim;
var() Name FireAnim;
var() Name FireLoopAnim;
var() Name FireEndAnim;
var() Name ReloadAnim;

var() float PreFireAnimRate;
var() float FireAnimRate;
var() float FireLoopAnimRate;
var() float FireEndAnimRate;
var() float ReloadAnimRate;
var() float TweenTime;

// sound //
var() Sound FireSound;
var() Sound ReloadSound;
var() Sound NoAmmoSound;

// jdf ---
// Force Feedback //
var() String FireForce;
var() String PreFireForce;
// --- jdf

// timing //
var() float PreFireTime;       // seconds before first shot
var() float FireRate;          // seconds betewwn shots
var() float MaxHoldTime;
var() float HoldTime;
var   float ServerStartFireTime;
var   float NextFireTime;
var() bool  bFireOnRelease;    // if true, shot will be fired when button is released, HoldTime will be the time the button was held for
var() bool  bWaitForRelease;   // if true, fire button must be released between each shot
var() bool  bModeExclusive;    // if true, no other fire modes can be active at the same time as this one

var   bool  bIsFiring;
var   bool  bNowWaiting;
var   bool  bServerDelayStopFire;
var   bool  bServerDelayStartFire;

// ammo //
var() class<Ammunition> AmmoClass;
var() int AmmoPerFire;
var() int AmmoClipSize;
var() float Load;

// camera shakes //
var() vector ShakeRotMag;           // how far to rot view
var() vector ShakeRotRate;          // how fast to rot view
var() float  ShakeRotTime;          // how much time to rot the instigator's view
var() vector ShakeOffsetMag;        // max view offset vertically
var() vector ShakeOffsetRate;       // how fast to offset view vertically
var() float  ShakeOffsetTime;       // how much time to offset view

// AI //
var() class<Projectile> ProjectileClass;
var() float BotRefireRate;
var() float WarnTargetPct;
var() bool bSplashDamage;
var() bool bSplashJump;
var() bool bRecommendSplashDamage;
var() bool bTossed;
var() bool bLeadTarget;
var() bool bInstantHit;

// other useful stuff //
var() bool  bPawnRapidFireAnim; // for determining what anim the firer should play
var() bool  bReflective;

// muzzle flash & smoke //
var() bool bAttachSmokeEmitter;
var() bool bAttachFlashEmitter;
var() class<xEmitter> FlashEmitterClass;
var() xEmitter FlashEmitter;
var() class<xEmitter> SmokeEmitterClass;
var() xEmitter SmokeEmitter;


var() float AimError; // 0=none 1000=quite a bit
var() float Spread; // rotator units. no relation to AimError
var() enum ESpreadStyle
{
    SS_None,
    SS_Random, // spread is max random angle deviation
    SS_Line,   // spread is angle between each projectile
    SS_Ring
} SpreadStyle;

var int FireCount;
var() float DamageAtten; // attenuate instant-hit/projectile damage by this multiplier

simulated function PostBeginPlay()
{
    Load = AmmoPerFire;

    if (bFireOnRelease)
        bWaitForRelease = true;

    if (bWaitForRelease)
        bNowWaiting = true;
}

simulated function Destroyed()
{
    DestroyEffects();
    Super.Destroyed();
}

simulated function DestroyEffects()
{
    if (FlashEmitter != None)
    {
        //log("Destroyed "$FlashEmitter);
        FlashEmitter.Destroy();
    }
    if (SmokeEmitter != None)
    {
        SmokeEmitter.Destroy();
    }
}

simulated function InitEffects()
{
    // don't even spawn on server
    if ( (Level.NetMode == NM_DedicatedServer) || (AIController(Instigator.Controller) != None) )
		return;
    if ( (FlashEmitterClass != None) && ((FlashEmitter == None) || FlashEmitter.bDeleteMe) )
    {
        FlashEmitter = Spawn(FlashEmitterClass);
        //log("Spawned "$FlashEmitter);
    }
    if ( (SmokeEmitterClass != None) && ((SmokeEmitter == None) || SmokeEmitter.bDeleteMe) )
    {
        SmokeEmitter = Spawn(SmokeEmitterClass);
    }
}

function DoFireEffect()
{
}

function DrawMuzzleFlash(Canvas Canvas)
{
    // Draw smoke first
    if (SmokeEmitter != None && SmokeEmitter.Base != Weapon)
    {
        SmokeEmitter.SetLocation( Weapon.GetEffectStart() );
        Canvas.DrawActor( SmokeEmitter, false, false, Weapon.DisplayFOV );
    }

    if (FlashEmitter != None && FlashEmitter.Base != Weapon)
    {
        FlashEmitter.SetLocation( Weapon.GetEffectStart() );
        Canvas.DrawActor( FlashEmitter, false, false, Weapon.DisplayFOV ); 
    }
}

function FlashMuzzleFlash()
{
    if (FlashEmitter != None)
        FlashEmitter.Trigger(Weapon, Instigator);
}

function StartMuzzleSmoke()
{
    if ( !Level.bDropDetail && (SmokeEmitter != None) )
        SmokeEmitter.Trigger(Weapon, Instigator);
}

function ShakeView()
{
    local PlayerController P;

    P = PlayerController(Instigator.Controller);
    if (P != None)
    {
        P.ShakeView(ShakeRotMag, ShakeRotRate, ShakeRotTime, 
                    ShakeOffsetMag, ShakeOffsetRate, ShakeOffsetTime);        
    }
}

// jdf ---
function ClientPlayForceFeedback( String EffectName )
{
    local PlayerController PC;

    PC = PlayerController(Instigator.Controller);
    if (PC != None && PC.bEnableWeaponForceFeedback )
    {
        PC.ClientPlayForceFeedback(EffectName);
    }
}

function StopForceFeedback( String EffectName )
{
    local PlayerController PC;

    PC = PlayerController(Instigator.Controller);
    if (PC != None && PC.bEnableWeaponForceFeedback )
		PC.StopForceFeedback(EffectName);
}
// --- jdf

function Update(float dt)
{
}

function StartFiring()
{
}

function StopFiring()
{
}

function StartBerserk()
{
    FireRate = default.FireRate * 0.75;
    FireAnimRate = default.FireAnimRate * 0.75;
}

function StopBerserk()
{
    FireRate = default.FireRate;
    FireAnimRate = default.FireAnimRate;
}

function bool IsFiring()
{
	return bIsFiring;
}

event ModeTick(float dt);

event ModeDoFire()
{
    if (!AllowFire())
        return;

    if (MaxHoldTime > 0.0)
        HoldTime = FMin(HoldTime, MaxHoldTime);

    // server
    if (Weapon.Role == ROLE_Authority)
    {
        Weapon.ConsumeAmmo(ThisModeNum, Load);
        DoFireEffect();
        if ( (Instigator == None) || (Instigator.Controller == None) )
			return;
        if ( AIController(Instigator.Controller) != None )
            AIController(Instigator.Controller).WeaponFireAgain(BotRefireRate, true);
        Instigator.SpawnTime = -100000;
    }

    // client
    if (Instigator.IsLocallyControlled())
    {
        ShakeView();
        PlayFiring();
        FlashMuzzleFlash();
        StartMuzzleSmoke();
    }
    else // server
    {
        ServerPlayFiring();
    }

    Weapon.IncrementFlashCount(ThisModeNum);

    // set the next firing time. must be careful here so client and server do not get out of sync
    if (bFireOnRelease)
    {
        if (bIsFiring)
            NextFireTime += MaxHoldTime + FireRate;
        else
            NextFireTime = Level.TimeSeconds + FireRate;
    }
    else
    {
        NextFireTime += FireRate;
        NextFireTime = FMax(NextFireTime, Level.TimeSeconds);
    }

    Load = AmmoPerFire;
    HoldTime = 0;

    if (Instigator.PendingWeapon != Weapon && Instigator.PendingWeapon != None)
    {
        bIsFiring = false;
        Weapon.PutDown();
    }
}

event ModeHoldFire()
{
    if (Instigator.IsLocallyControlled())
        PlayStartHold();
}


simulated function bool AllowFire()
{
    return ( Weapon.Ammo[ThisModeNum] != None && Weapon.Ammo[ThisModeNum].AmmoAmount >= AmmoPerFire);
}

//// server propigation of firing ////
function ServerPlayFiring()
{
    Weapon.PlayOwnedSound(FireSound,SLOT_Interact,TransientSoundVolume,,,,false);
}


//// client animation ////

function PlayPreFire()
{
    if (Weapon.HasAnim(PreFireAnim))
    {
        Weapon.PlayAnim(PreFireAnim, PreFireAnimRate, TweenTime);
        ClientPlayForceFeedback(PreFireForce);  // jdf
    }
}

function PlayStartHold()
{
}

function PlayFiring()
{
    if (FireCount > 0)
    {
        if (Weapon.HasAnim(FireLoopAnim))
        {
            Weapon.PlayAnim(FireLoopAnim, FireLoopAnimRate, 0.0);
        }
        else
        {
            Weapon.PlayAnim(FireAnim, FireAnimRate, TweenTime);
        }
    }
    else
    {
        Weapon.PlayAnim(FireAnim, FireAnimRate, TweenTime);
    }
    
    Weapon.PlayOwnedSound(FireSound,SLOT_Interact,TransientSoundVolume,,,,false);
        
    ClientPlayForceFeedback(FireForce);  // jdf
    
    FireCount++;
}

function PlayFireEnd()
{
    if (Weapon.HasAnim(FireEndAnim))
    {
        Weapon.PlayAnim(FireEndAnim, FireEndAnimRate, TweenTime);
    }
}

function Rotator AdjustAim(Vector Start, float InAimError)
{
    local Ammunition Ammo;

    // stuff Ammo with AI info
    Ammo = Weapon.Ammo[ThisModeNum];
    if (Ammo == None)
    {
        Log("warning:"@Weapon@self@"needs an ammo class for nefarious AI purposes");
        return Instigator.Rotation;
    }
    else
    {
        Ammo.bTossed = bTossed;
        Ammo.bTrySplash = bRecommendSplashDamage;
        Ammo.bLeadTarget = bLeadTarget;
        Ammo.bInstantHit = bInstantHit;
        Ammo.ProjectileClass = ProjectileClass;
		Ammo.WarnTargetPct = WarnTargetPct;
        Ammo.MaxRange = MaxRange(); //amb: for autoaim
        return Instigator.AdjustAim(Ammo, Start, InAimError);
    }
}

simulated function vector GetFireStart(vector X, vector Y, vector Z)
{
    return Instigator.Location + Instigator.EyePosition();
}

simulated function DisplayDebug(Canvas Canvas, out float YL, out float YPos)
{
    Canvas.SetDrawColor(0,255,0);
    Canvas.DrawText("  FIREMODE "$GetItemName(string(self))$" IsFiring "$bIsFiring);
    YPos += YL;
    Canvas.SetPos(4,YPos);
/*
    Canvas.DrawText("  FireOnRelease "$bFireOnRelease$" HoldTime "$HoldTime$" MaxHoldTime "$MaxHoldTime);
    YPos += YL;
    Canvas.SetPos(4,YPos);

    Canvas.DrawText("  NextFireTime "$NextFireTime$" NowWaiting "$bNowWaiting);
    YPos += YL;
    Canvas.SetPos(4,YPos);
*/
}

function float MaxRange()
{
	return 5000;
}

defaultproperties
{
	TransientSoundVolume=+0.5
    bHidden=true
    RemoteRole=ROLE_None

    PreFireAnim=PreFire
    FireAnim=Fire
    FireLoopAnim=FireLoop
    FireEndAnim=FireEnd
    ReloadAnim=Reload
    
    PreFireAnimRate=1.0
    FireAnimRate=1.0
    FireLoopAnimRate=1.0
    FireEndAnimRate=1.0
    ReloadAnimRate=1.0
    TweenTime=0.1
    
    PreFireTime=0.0
    FireRate=0.5

    bModeExclusive=true

    // AI //
    bSplashDamage=false
    bRecommendSplashDamage=false
    BotRefireRate=0.95
    bTossed=false
    bLeadTarget=false
    bInstantHit=true
    ProjectileClass=None
	AimError=600

    DamageAtten=1.0
}
