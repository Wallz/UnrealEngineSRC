class TransFire extends ProjectileFire;

var() Sound TransFireSound;
var() Sound RecallFireSound;
var TransDummyBeacon DummyBeacon;
var TransRecallEffect RecallEffect;
var bool bBeaconDeployed; // meaningful for client
var() String TransFireForce;
var() String RecallFireForce;

simulated function DestroyEffects()
{
	Super.DestroyEffects();
	
    if (DummyBeacon != None)
        DummyBeacon.Destroy();

    if (RecallEffect != None)
        RecallEffect.Destroy();
}

function InitEffects()
{
    // don't even spawn on server
    if ( (Level.NetMode == NM_DedicatedServer) || (AIController(Instigator.Controller) != None) )
		return;
    Super.InitEffects();
    
    if( DummyBeacon == None )
        DummyBeacon = Spawn(class'TransDummyBeacon');
    Weapon.AttachToBone(DummyBeacon, 'beaconbone');
    if ( RecallEffect == None )
        RecallEffect = Spawn(class'TransRecallEffect');
    Weapon.AttachToBone(RecallEffect, 'beaconbone');

    SetTimer(0.1, true);
}

simulated function PlayFiring()
{
    if (!bBeaconDeployed)
    {
        Weapon.PlayAnim(FireAnim, FireAnimRate, TweenTime);
        ClientPlayForceFeedback( TransFireForce );  // jdf
    }
}

function Rotator AdjustAim(Vector Start, float InAimError)
{
    return Instigator.Controller.Rotation;
}

simulated function bool AllowFire()
{
    return ( TransLauncher(Weapon).AmmoCharge >= 1.0 );
}

simulated function DoRecallEffect()
{
    RecallEffect.mStartParticles = RecallEffect.ClampToMaxParticles(100);
    ClientPlayForceFeedback( RecallFireForce );  // jdf
}

simulated function Timer()
{
    if (DummyBeacon != None)
    {
        if (TransLauncher(Weapon).TransBeacon != None && !bBeaconDeployed)
        {
            bBeaconDeployed = true;
            DummyBeacon.bHidden = true;
        }
        else if (TransLauncher(Weapon).TransBeacon == None && bBeaconDeployed)
        {
            bBeaconDeployed = false;
            DummyBeacon.bHidden = false;
            DoRecallEffect();
        }
    }
}

function projectile SpawnProjectile(Vector Start, Rotator Dir)
{
    local TransBeacon TransBeacon;

    if (TransLauncher(Weapon).TransBeacon == None)
    {
		if ( (Instigator == None) || (Instigator.PlayerReplicationInfo == None) || (Instigator.PlayerReplicationInfo.Team == None) )
			TransBeacon = Spawn(class'XWeapons.TransBeacon',,, Start, Dir);
		else if ( Instigator.PlayerReplicationInfo.Team.TeamIndex == 0 )
			TransBeacon = Spawn(class'XWeapons.RedBeacon',,, Start, Dir);
		else
			TransBeacon = Spawn(class'XWeapons.BlueBeacon',,, Start, Dir);
        TransLauncher(Weapon).TransBeacon = TransBeacon;
        Weapon.PlaySound(TransFireSound,SLOT_Interact,,,,,false);
    }
    else
    {
        TransLauncher(Weapon).ViewPlayer();
        TransLauncher(Weapon).TransBeacon.Destroy();
        TransLauncher(Weapon).TransBeacon = None;
        Weapon.PlaySound(RecallFireSound,SLOT_Interact,,,,,false);
    }
    return TransBeacon;
}

defaultproperties
{
    AmmoClass=None
    AmmoPerFire=1
    FireAnimRate=1.5

    ProjectileClass=class'XWeapons.TransBeacon'

    ProjSpawnOffset=(X=25,Y=8,Z=-10)
    FireRate=0.25
    bWaitForRelease=true
    bModeExclusive=false
    FireSound=None
    TransFireSound=Sound'WeaponSounds.Translocator.TranslocatorFire'
    RecallFireSound=Sound'WeaponSounds.Translocator.TranslocatorModuleRegeneration'
    TransFireForce="TranslocatorFire"  // jdf
    RecallFireForce="TranslocatorModuleRegeneration"  // jdf

    BotRefireRate=0.3
    bLeadTarget=false
}
