class TransRecall extends WeaponFire;

var() Sound TransFailedSound;
var bool bGibMe;
var Material TransMaterials[2];

simulated function PlayFiring()
{
    if (TransFire(Weapon.FireMode[0]).bBeaconDeployed)
    {
        Weapon.PlayAnim(FireAnim, FireAnimRate, TweenTime);
    }
}

simulated function bool AllowFire()
{
    local bool success;
    
    success = ( TransLauncher(Weapon).AmmoCharge >= 1.0 );

    if (!success && Role == ROLE_Authority && TransLauncher(Weapon).TransBeacon != None)
    {
        if( Instigator.Controller != None && Instigator.Controller.IsA('PlayerController'))
            PlayerController(Instigator.Controller).ClientPlaySound(TransFailedSound);
    }

    return success;
}

function Translocate()
{
    local TransBeacon TransBeacon;
	local Actor HitActor;
	local Vector HitNormal,HitLocation,dest,Vel2D;
    local Vector PrevLocation,Diff;
    local xPawn XP;
    local controller C;
	local bool bFailedTransloc;
	local int EffectNum;
	
    TransBeacon = TransLauncher(Weapon).TransBeacon;

    // gib if the translocator is disrupted
    if ( TransBeacon.Disrupted() )
	{
		if (Level.Game.GameStats!=None)
			Level.Game.GameStats.SpecialEvent(Instigator.PlayerReplicationInfo,"translocate_gib");
        bGibMe = true; // delay gib to avoid destroying player and weapons right away in the middle of all this
		return;
	}

    dest = TransBeacon.Location;
    if ( TransBeacon.Physics == PHYS_None )
        dest += vect(0,0,1) * Instigator.CollisionHeight;
    else
        dest += vect(0,0,0.5) * Instigator.CollisionHeight;
	HitActor = Trace(HitLocation,HitNormal,dest,Location,true);
	if ( HitActor != None )
		dest = TransBeacon.Location;    
   
    TransBeacon.SetCollision(false, false, false);    
    
    if (Instigator.PlayerReplicationInfo.HasFlag != None)
        Instigator.PlayerReplicationInfo.HasFlag.Drop(0.5 * Instigator.Velocity);
    
    PrevLocation = Instigator.Location;
    
	// verify won't telefrag teammate or recently spawned player
	for ( C=Level.ControllerList; C!=None; C=C.NextController )
		if ( (C.Pawn != None) && (C.Pawn != Instigator) 
			&& (C.SameTeamAs(Instigator.Controller) || (Level.TimeSeconds - C.Pawn.SpawnTime < DeathMatch(Level.Game).SpawnProtectionTime)) )
		{ 
			Diff = Dest - C.Pawn.Location;
			if ( Abs(Diff.Z) < C.Pawn.CollisionHeight + 2 * Instigator.CollisionHeight )
			{
				Diff.Z = 0;
				if ( VSize(Diff) < C.Pawn.CollisionRadius + Instigator.CollisionRadius + 8 )
				{
					bFailedTransloc = true;
					break;
				}
			}
		}

    if ( !bFailedTransloc && (Instigator.SetLocation(dest) || BotTranslocation()) )
    {
		TransLauncher(Weapon).ReduceAmmo();

        // spawn out
        XP = xPawn(Instigator);
        if( XP != None )
            XP.DoTranslocateOut(PrevLocation);
 
		// bound XY velocity to prevent cheats
		Vel2D = Instigator.Velocity;
		Vel2D.Z = 0;
		Vel2D = Normal(Vel2D) * FMin(Instigator.GroundSpeed,VSize(Vel2D));
		Vel2D.Z = Instigator.Velocity.Z;
		Instigator.Velocity = Vel2D;
		
        if ( Instigator.PlayerReplicationInfo.Team != None )
			EffectNum = Instigator.PlayerReplicationInfo.Team.TeamIndex;
			
        Instigator.SetOverlayMaterial( TransMaterials[EffectNum], 1.0, false );
        Instigator.PlayTeleportEffect( false, false);

		if ( !Instigator.PhysicsVolume.bWaterVolume )
		{
			if ( Bot(Instigator.Controller) != None )
			{
				Instigator.Velocity.X = 0;
				Instigator.Velocity.Y = 0;
				Instigator.Velocity.Z = -150;
				Instigator.Acceleration = vect(0,0,0);
			}
			Instigator.SetPhysics(PHYS_Falling);
		}
    }
    else if( PlayerController(Instigator.Controller) != None )
            PlayerController(Instigator.Controller).ClientPlaySound(TransFailedSound);

    TransBeacon.Destroy();
    TransLauncher(Weapon).TransBeacon = None;
    TransLauncher(Weapon).ViewPlayer();
}

function ModeDoFire()
{
    local TransBeacon TransBeacon;

    Super.ModeDoFire();

    if (Role == ROLE_Authority && bGibMe)
    {
        TransBeacon = TransLauncher(Weapon).TransBeacon;
        TransLauncher(Weapon).TransBeacon = None;
        TransLauncher(Weapon).ViewPlayer();
        Instigator.GibbedBy(TransBeacon.Disruptor);
        TransBeacon.Destroy();
        bGibMe = false;
    }
}

function DoFireEffect()
{
    if (TransLauncher(Weapon).TransBeacon != None)
    {
        Translocate();
    }
}

// AI Interface
function bool BotTranslocation()
{
	local Bot B;

	B = Bot(Instigator.Controller);
	if ( (B == None) || !B.bPreparingMove || (B.RealTranslocationTarget == None) )
		return false;

	// if bot failed to translocate, event though beacon was in target cylinder, 
	// try at center of cylinder
	return ( Instigator.SetLocation(B.RealTranslocationTarget.Location) );
}
// END AI interface

defaultproperties
{
	TransMaterials(0)=Material'PlayerTransRed'
	TransMaterials(1)=Material'PlayerTrans'
    AmmoClass=None
    AmmoPerFire=0
    FireAnim=Recall
    FireRate=0.25
    bWaitForRelease=false
    bModeExclusive=false
    TransFailedSound=Sound'WeaponSounds.BSeekLost1'

    BotRefireRate=0.3
}
