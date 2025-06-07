class ShieldAltFire extends WeaponFire;

var ShieldEffect ShieldEffect;
var() float AmmoRegenTime;
var() float ChargeupTime;
var	  float RampTime;
var Sound ChargingSound;                // charging sound

simulated function Destroyed()
{
    if (Role == ROLE_Authority)
    {
        if ( ShieldEffect != None )
            ShieldEffect.Destroy();
    }
    Super.Destroyed();
}

function DoFireEffect()
{
    local ShieldAttachment Attachment;

    Attachment = ShieldAttachment(Weapon.ThirdPersonActor);
    Instigator.AmbientSound = ChargingSound;
    
    if( Attachment != None && Attachment.ShieldEffect3rd != None )
        Attachment.ShieldEffect3rd.bHidden = false;

    SetTimer(AmmoRegenTime, true);
}

function PlayFiring()
{
    ClientPlayForceFeedback("ShieldNoise");  // jdf
    SetTimer(AmmoRegenTime, true);
    Weapon.LoopAnim('Shield');
}

function StopFiring()
{
    local ShieldAttachment Attachment;

    Attachment = ShieldAttachment(Weapon.ThirdPersonActor);
	Instigator.AmbientSound = None;
    
    if( Attachment != None && Attachment.ShieldEffect3rd != None )
    {
        Attachment.ShieldEffect3rd.bHidden = true;
        StopForceFeedback( "ShieldNoise" );  // jdf
    }

    SetTimer(AmmoRegenTime, true);
}

function TakeHit(int Drain)
{
    if (ShieldEffect != None)
    {
        ShieldEffect.Flash(Drain);
    }

    SetBrightness(true);
}

function StartBerserk()
{
}

function StopBerserk()
{
}

function SetBrightness(bool bHit)
{
    local ShieldAttachment Attachment;
 	local float Brightness;

	Brightness = Weapon.Ammo[1].AmmoAmount;
	if ( RampTime < ChargeUpTime )
		Brightness *= RampTime/ChargeUpTime; 
    if (ShieldEffect != None)
        ShieldEffect.SetBrightness(Brightness);

    Attachment = ShieldAttachment(Weapon.ThirdPersonActor);
    if( Attachment != None )
        Attachment.SetBrightness(Brightness, bHit);
}

function DrawMuzzleFlash(Canvas Canvas)
{
    Super.DrawMuzzleFlash(Canvas);

    if (ShieldEffect == None)
    {
        ShieldEffect = Spawn(class'ShieldEffect', self);
    }

    if ( bIsFiring && Weapon.Ammo[1].AmmoAmount > 0 )
    {
        ShieldEffect.SetLocation( Weapon.GetEffectStart() );
        ShieldEffect.SetRotation( Instigator.GetViewRotation() );
        Canvas.DrawActor( ShieldEffect, false, false, Weapon.DisplayFOV );
    }
}

function Timer()
{
   if( Weapon.Ammo[1] == None )
        return;

    if (!bIsFiring)
    {
		RampTime = 0;
        if (Weapon.Ammo[1].AmmoAmount < Weapon.Ammo[1].MaxAmmo)
            Weapon.Ammo[1].AmmoAmount += 1;
        else
            SetTimer(0, false);
    }
    else
    {
        if (!Weapon.Ammo[1].UseAmmo(1))
        {
            if (Weapon.ClientState == WS_ReadyToFire)
                Weapon.PlayIdle();
            StopFiring();
        }
        else
			RampTime += AmmoRegenTime;
    }
	
	SetBrightness(false);
}

defaultproperties
{
    AmmoClass=class'ShieldAmmo'
    AmmoPerFire=0
    AmmoRegenTime=0.2
    ChargeUpTime=3.0

    FireAnim=None
    FireAnimRate=1.0
    FireEndAnim=Idle
    FireLoopAnim=None       
    
    ChargingSound=Sound'WeaponSounds.BShield1'
    FireSound=Sound'WeaponSounds.Translocator.TranslocatorModuleRegeneration'
    FireForce="TranslocatorModuleRegeneration"  // jdf
    bPawnRapidFireAnim=true
    FireRate=1.0
    bModeExclusive=true
    bWaitForRelease=true
    MaxHoldTime=0.0

    BotRefireRate=1.0
}
