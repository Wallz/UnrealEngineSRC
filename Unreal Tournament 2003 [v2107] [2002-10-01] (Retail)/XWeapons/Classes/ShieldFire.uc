class ShieldFire extends WeaponFire;

var class<DamageType> DamageType;       // weapon fire damage type (no projectile, so we put this here)
var float ShieldRange;                  // from pawn centre
var float MinHoldTime;                  // held for this time or less will do minimum damage/force. held for MaxHoldTime will do max
var float MinForce, MaxForce;           // force to other players
var float MinDamage, MaxDamage;         // damage to other players
var float SelfForceScale;               // %force to self (when shielding a wall)
var float SelfDamageScale;              // %damage to self (when shielding a wall)
var Sound ChargingSound;                // charging sound
var xEmitter ChargingEmitter;           // emitter class while charging
var float AutoFireTestFreq;
var float FullyChargedTime;				// held for this long will do max damage
var bool bAutoRelease;

// jdf ---
var String ChargingForce;
var bool bStartedChargingForce;
// --- jdf

simulated function DestroyEffects()
{
    if (ChargingEmitter != None)
    {
        ChargingEmitter.Destroy();
    }
	Super.DestroyEffects();
}

simulated function InitEffects()
{
	if ( Level.NetMode != NM_DedicatedServer )
	{
		ChargingEmitter = Spawn(class'XEffects.ShieldCharge');
		ChargingEmitter.mRegenPause = true;
	}
    bStartedChargingForce = false;  // jdf
    Super.InitEffects();
}

function DrawMuzzleFlash(Canvas Canvas)
{
    if (ChargingEmitter != None && HoldTime > 0.0 && !bNowWaiting)
    {
        ChargingEmitter.SetLocation( Weapon.GetEffectStart() );
        Canvas.DrawActor( ChargingEmitter, false, false, Weapon.DisplayFOV );
    }    

    if (FlashEmitter != None)
    {
        FlashEmitter.SetLocation( Weapon.GetEffectStart() );
        FlashEmitter.SetRotation(Weapon.Rotation);
        Canvas.DrawActor( FlashEmitter, false, false, Weapon.DisplayFOV ); 
    }

    if ( (Instigator.AmbientSound == ChargingSound)
		&& ((HoldTime <= 0.0) || bNowWaiting) )
        Instigator.AmbientSound = None;    

}

function Rotator AdjustAim(Vector Start, float InAimError)
{
	local rotator Aim, EnemyAim;
	
	if ( AIController(Instigator.Controller) != None )
	{
		Aim = Instigator.Rotation;
		if ( Instigator.Controller.Enemy != None )
		{
			EnemyAim = rotator(Instigator.Controller.Enemy.Location - Start);
			Aim.Pitch = EnemyAim.Pitch;
		}
		return Aim;
	}	
	else  
		return super.AdjustAim(Start,InAimError);
}

function DoFireEffect()
{
	local Vector HitLocation, HitNormal, StartTrace, EndTrace, X,Y,Z;
    local Rotator Aim;
	local Actor Other;
    local float Scale, Damage, Force;

	Instigator.MakeNoise(1.0);
    Weapon.GetViewAxes(X,Y,Z);
	bAutoRelease = false;
	StartTrace = Instigator.Location;
    
    Aim = AdjustAim(StartTrace, AimError);

	EndTrace = StartTrace + ShieldRange * Vector(Aim); 

	Other = Trace(HitLocation, HitNormal, EndTrace, StartTrace, true);
    
    Scale = (FClamp(HoldTime, MinHoldTime, FullyChargedTime) - MinHoldTime) / (FullyChargedTime - MinHoldTime); // result 0 to 1
    Damage = MinDamage + Scale * (MaxDamage - MinDamage);
    Force = MinForce + Scale * (MaxForce - MinForce);

    Instigator.AmbientSound = None;
    
    if (ChargingEmitter != None)
        ChargingEmitter.mRegenPause = true;

    if ( Other != None && Other != Instigator )
    {
	    if ( Pawn(Other) != None )
            Other.TakeDamage(Damage, Instigator, HitLocation, Force*(X+vect(0,0,0.5)), DamageType);
	    else
	    {
            if (xPawn(Instigator).bBerserk) 
				Force *= 2.0;	                
            Instigator.TakeDamage(SelfDamageScale*Damage, Instigator, HitLocation, -SelfForceScale*Force*X, DamageType);
        }
	}

    SetTimer(0, false);
}

function ModeHoldFire()
{
    SetTimer(AutoFireTestFreq, true);
}

function bool IsFiring()
{
	return ( bIsFiring || bAutoRelease );
}

function Timer()
{
    local Actor Other;
    local Vector HitLocation, HitNormal, StartTrace, EndTrace;
    local Rotator Aim;
    local float Regen;
    local float ChargeScale;

    if (HoldTime > 0.0 && !bNowWaiting)
    {        
	    StartTrace = Instigator.Location;  
		Aim = AdjustAim(StartTrace, AimError);
	    EndTrace = StartTrace + ShieldRange * Vector(Aim); 

        Other = Trace(HitLocation, HitNormal, EndTrace, StartTrace, true);
        if ( Other != None && Other != Instigator && Other.IsA('Pawn'))
        {
			bAutoRelease = true;
            bIsFiring = false;
            Instigator.AmbientSound = None;
            if (ChargingEmitter != None)
                ChargingEmitter.mRegenPause = true;
        }
        else
        {                
            Instigator.AmbientSound = ChargingSound;
            ChargeScale = FMin(HoldTime, FullyChargedTime);
            if (ChargingEmitter != None)
            {
                ChargingEmitter.mRegenPause = false;            
                Regen = (HoldTime * HoldTime) + ChargeScale * 10 + 20;
                ChargingEmitter.mRegenRange[0] = Regen;
                ChargingEmitter.mRegenRange[1] = Regen;
                ChargingEmitter.mSpeedRange[0] = ChargeScale * -15.0;
                ChargingEmitter.mSpeedRange[1] = ChargeScale * -15.0;            
                Regen = FMax((ChargeScale / 30.0),0.20);
                ChargingEmitter.mLifeRange[0] = Regen;
                ChargingEmitter.mLifeRange[1] = Regen;
            }
            
            if (!bStartedChargingForce)
            {
                bStartedChargingForce = true;
                ClientPlayForceFeedback( ChargingForce );
            }
        }
    }
    else
    {
		if ( Instigator.AmbientSound == ChargingSound )
			Instigator.AmbientSound = None;
        SetTimer(0, false);
    }
}

simulated function vector GetFireStart(vector X, vector Y, vector Z)
{
    return Instigator.Location;
}

function StartBerserk()
{
    MaxHoldTime = default.MaxHoldTime * 0.75;
    FullyChargedTime = default.FullyChargedTime * 0.75;
}

function StopBerserk()
{
    MaxHoldTime = default.MaxHoldTime;
    FullyChargedTime = default.FullyChargedTime;
}

function PlayPreFire()
{
    Weapon.PlayAnim('Charge', 1.0/FullyChargedTime, 0.1);
}

// jdf ---
function PlayFiring()
{
    bStartedChargingForce = false;
    StopForceFeedback(ChargingForce);
    Super.PlayFiring();
}
// --- jdf

defaultproperties
{
    AmmoClass=class'ShieldAmmo'
    AmmoPerFire=0

    FireAnim=Fire
    FireAnimRate=1.0
    FireEndAnim=None
    PreFireAnim=Charge
    FireLoopAnim=None       
    TweenTime=0.0
    
    DamageType=class'DamTypeShieldImpact'
    ShieldRange=120.0
    MinForce=65000.0
    MaxForce=100000.0
    MinDamage=40.0    
    MaxDamage=150.0
    SelfForceScale=1.0    
    SelfDamageScale=0.5

    FireSound=Sound'WeaponSounds.P1ShieldGunFire'
    ChargingSound=Sound'WeaponSounds.shieldgun_charge'
    // jdf ---
    FireForce="ShieldGunFire"
    ChargingForce="ImpactHammerLoop"
    bStartedChargingForce=false;
    // --- jdf
    FireRate=0.6
    bModeExclusive=true
    FlashEmitterClass=class'xEffects.ForceRingA'
    bFireOnRelease=true
    MaxHoldTime=0.0
    FullyChargedTime=4.0
    MinHoldtime=0.25
    AutoFireTestFreq=0.15
    TransientSoundVolume=+1.0

    BotRefireRate=1.0
	WarnTargetPct=+0.1

    ShakeOffsetMag=(X=-20.0,Y=0.00,Z=0.00)
    ShakeOffsetRate=(X=-1000.0,Y=0.0,Z=0.0)
    ShakeOffsetTime=2
    ShakeRotMag=(X=0.0,Y=0.0,Z=0.0)
    ShakeRotRate=(X=0.0,Y=0.0,Z=0.0)
    ShakeRotTime=2
}
