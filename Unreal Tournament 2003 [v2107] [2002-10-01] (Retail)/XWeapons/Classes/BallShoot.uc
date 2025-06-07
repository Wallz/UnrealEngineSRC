class BallShoot extends ProjectileFire;

simulated function bool AllowFire()
{
    return true;
}

function ModeDoFire()
{
    Super.ModeDoFire();
    
    // make sure to switch to a new weapon
    if (PlayerController(Instigator.Controller) != None && Instigator.IsLocallyControlled()
        && (Instigator.PendingWeapon == Weapon || Instigator.PendingWeapon == None))
    {
        PlayerController(Instigator.Controller).ClientSwitchToBestWeapon();
    }
}

function DoFireEffect()
{
    local xBombFlag Bomb;
    
    if (Weapon.Role != ROLE_Authority)
        return;

    if( Instigator.PlayerReplicationInfo.HasFlag == None )
        return;
    
    Bomb = xBombFlag( Instigator.PlayerReplicationInfo.HasFlag );

    if ( Bomb == None )
        return;

    BallLauncher(Weapon).launchedBall = true;

    Super.DoFireEffect();
}

function projectile SpawnProjectile(Vector Start, Rotator Dir)
{
    local xBombFlag bomb;
    local vector ThrowDir;

    Bomb = xBombFlag ( Weapon.Instigator.Controller.PlayerReplicationInfo.HasFlag );
    Bomb.PassTarget = BallLauncher(Weapon).PassTarget;
    if ( Bomb.PassTarget != None )
		ThrowDir = Instigator.Controller.AdjustToss(Bomb.ThrowSpeed,Instigator.Location,Bomb.PassTarget.Location,true);
	else
		ThrowDir = Bomb.ThrowSpeed * vector(Dir);
    bomb.Throw(Start, ThrowDir);
	BallLauncher(Weapon).PassTarget = None;
    return None;
}

defaultproperties
{
    AmmoClass=class'BallAmmo'
    AmmoPerFire=0

    FireRate=0.25
    bWaitForRelease=true
    bModeExclusive=false

    FireSound=Sound'WeaponSounds.ballgun_launch'
    FireForce="ballgun_launch"  // jdf
}
