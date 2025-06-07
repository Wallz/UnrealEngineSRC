class LinkFire extends WeaponFire;

var LinkBeamEffect Beam;
var Sound MakeLinkSound;
var float UpTime;
var Pawn LockedPawn;
var float LinkBreakTime;
var() float LinkBreakDelay;
var float LinkScale[6];

var String MakeLinkForce;

var() class<DamageType> DamageType;
var() int Damage;
var() float MomentumTransfer;

var() float TraceRange;
var() float LinkFlexibility;

var		bool bDoHit;
var()	bool bFeedbackDeath;
var		bool bInitAimError;
var		bool bLinkFeedbackPlaying;
var		bool bStartFire;

var rotator DesiredAimError, CurrentAimError;

simulated function DestroyEffects()
{
	Super.DestroyEffects();
    if ( Level.NetMode != NM_Client )
    {
        if (Beam != None)
            Beam.Destroy();
    }
}

simulated function ModeTick(float dt)
{
    local Vector StartTrace, EndTrace, X,Y,Z, V;
    local Vector HitLocation, HitNormal, EndEffect;
    local Actor Other;
    local Rotator Aim;
    local LinkGun LinkGun;
    local float MT, Step, ls;
	local bot B;
	local bool bShouldStop;
    local int AdjustedDamage;
	local LinkBeamEffect LB;
	
    if (!bIsFiring)
    {
		bInitAimError = true;
        return;
    }
    LinkGun = LinkGun(Weapon);

    if (LinkGun.Links < 0)
    {
        log("warning:"@Instigator@"linkgun had"@LinkGun.Links@"links");
        LinkGun.Links = 0;
    }

    ls = LinkScale[Min(LinkGun.Links,5)];

    if (FlashEmitter != None)
    {
        // set fuzzle flash color
        if (LinkGun.Linking)
        {
            if (Instigator.PlayerReplicationInfo.Team == None || Instigator.PlayerReplicationInfo.Team.TeamIndex == 0)
                FlashEmitter.Skins[0] = Texture'XEffectMat.link_muz_red';
            else
                FlashEmitter.Skins[0] = Texture'XEffectMat.link_muz_blue';
        }
        else
        {
            if (LinkGun.Links > 0)
                FlashEmitter.Skins[0] = Texture'XEffectMat.link_muz_yellow';
            else
                FlashEmitter.Skins[0] = Texture'XEffectMat.link_muz_green';
        }

        // adjust muzzle flash size to link size
	    FlashEmitter.mSizeRange[0] = FlashEmitter.default.mSizeRange[0] * (ls*0.5 + 1);
	    FlashEmitter.mSizeRange[1] = FlashEmitter.mSizeRange[0];
    }

    if ( LinkGun.Ammo[ThisModeNum].AmmoAmount >= AmmoPerFire && ((UpTime > 0.0) || (Instigator.Role < ROLE_Authority)) )
    {
        UpTime -= dt;
        LinkGun.GetViewAxes(X,Y,Z);

        // the to-hit trace always starts right in front of the eye
        StartTrace = Instigator.Location + Instigator.EyePosition() + X*Instigator.CollisionRadius;

        TraceRange = default.TraceRange + LinkGun.Links*250;
        
        if ( Instigator.Role < ROLE_Authority )
        {
			if ( Beam == None )
				ForEach DynamicActors(class'LinkBeamEffect', LB )
					if ( !LB.bDeleteMe && (LB.Instigator != None) && (LB.Instigator == Instigator) )
					{
						Beam = LB;
						break;
					}
			if ( Beam != None )
				LockedPawn = Beam.LinkedPawn;
		}	
        if ( LockedPawn != None ) 
			TraceRange *= 1.5;

        if (Instigator.Role == ROLE_Authority) 
		{
			B = Bot(Instigator.Controller);
			if ( (B != None) && (PlayerController(B.Squad.SquadLeader) != None) && (B.Squad.SquadLeader.Pawn != None) )
			{
				if ( IsLinkable(B.Squad.SquadLeader.Pawn) 
					&& B.Squad.SquadLeader.Pawn.Weapon.FireMode[1].bIsFiring
					&& (VSize(B.Squad.SquadLeader.Pawn.Location - StartTrace) < TraceRange) )
				{
					Other = Trace(HitLocation, HitNormal, B.Squad.SquadLeader.Pawn.Location, StartTrace, true); 
					if ( Other == B.Squad.SquadLeader.Pawn )
					{
						B.Focus = B.Squad.SquadLeader.Pawn;
						if ( B.Focus != LockedPawn )
							SetLinkTo(B.Squad.SquadLeader.Pawn); 
						B.SetRotation(Rotator(B.Focus.Location - StartTrace));
 						X = Normal(B.Focus.Location - StartTrace);
 					}
 					else if ( B.Focus == B.Squad.SquadLeader.Pawn )
						bShouldStop = true;
				}
 				else if ( B.Focus == B.Squad.SquadLeader.Pawn )
					bShouldStop = true;
			}
		}	
		if ( LockedPawn != None )
		{
			EndTrace = LockedPawn.Location + LockedPawn.EyeHeight*Vect(0,0,0.5); // beam ends at approx gun height
			if ( Instigator.Role == ROLE_Authority )
			{
				V = Normal(EndTrace - StartTrace);
				if ( (V dot X < LinkFlexibility) || LockedPawn.Health <= 0 || LockedPawn.bDeleteMe || (VSize(EndTrace - StartTrace) > 1.5 * TraceRange) )
				{
					SetLinkTo(None);
				}
			}
		}

        if ( LockedPawn == None )
        {
            if ( Bot(Instigator.Controller) != None )
            {
				if ( bInitAimError )
				{
					CurrentAimError = AdjustAim(StartTrace, AimError);
					bInitAimError = false;
				}
				else
				{
					BoundError();
					CurrentAimError.Yaw = CurrentAimError.Yaw + Instigator.Rotation.Yaw;
				}
				// smooth aim error changes
				Step = 7500.0 * dt;
				if ( DesiredAimError.Yaw ClockWiseFrom CurrentAimError.Yaw )
				{
					CurrentAimError.Yaw += Step;
					if ( !(DesiredAimError.Yaw ClockWiseFrom CurrentAimError.Yaw) )
					{
						CurrentAimError.Yaw = DesiredAimError.Yaw;
						DesiredAimError = AdjustAim(StartTrace, AimError);
					}
				}
				else
				{
					CurrentAimError.Yaw -= Step;
					if ( DesiredAimError.Yaw ClockWiseFrom CurrentAimError.Yaw )
					{
						CurrentAimError.Yaw = DesiredAimError.Yaw;
						DesiredAimError = AdjustAim(StartTrace, AimError);
					}
				}
				CurrentAimError.Yaw = CurrentAimError.Yaw - Instigator.Rotation.Yaw;
				if ( BoundError() )
					DesiredAimError = AdjustAim(StartTrace, AimError);
				CurrentAimError.Yaw = CurrentAimError.Yaw + Instigator.Rotation.Yaw;
				
				Aim = Rotator(Instigator.Controller.Target.Location - Instigator.Location);
				
				Aim.Yaw = CurrentAimError.Yaw;
				
				// save difference
				CurrentAimError.Yaw = CurrentAimError.Yaw - Instigator.Rotation.Yaw;
			}
			else
	            Aim = AdjustAim(StartTrace, AimError);

            X = Vector(Aim);
            EndTrace = StartTrace + TraceRange * X;
        }
        
        Other = Trace(HitLocation, HitNormal, EndTrace, StartTrace, true);
        if (Other != None && Other != Instigator)
			EndEffect = HitLocation;
		else
			EndEffect = EndTrace;
		if (Beam != None)
			Beam.EndEffect = EndEffect;

		if ( Instigator.Role < ROLE_Authority )
			return;
			
        if (Other != None && Other != Instigator)
        {
            // target can be linked to
            if (IsLinkable(Other))
            {
                if (Other != lockedpawn)
                    SetLinkTo(Pawn(Other));

                if (lockedpawn != None)
                    LinkBreakTime = LinkBreakDelay;
            }
            else
            {
                // stop linking
                if (lockedpawn != None)
                {
                    if (LinkBreakTime <= 0.0)
                        SetLinkTo(None);
                    else
                        LinkBreakTime -= dt;
                }

                // beam is updated every frame, but damage is only done based on the firing rate
                if (bDoHit)
                {
                    if (Beam != None) 
						Beam.bLockedOn = false;

                    Instigator.MakeNoise(1.0);

                    AdjustedDamage = Damage*(1.5*Linkgun.Links+1);

                    if ( !Other.bWorldGeometry )
                    {
                        if (Level.Game.bTeamGame && Pawn(Other) != None && (Pawn(Other).PlayerReplicationInfo != None)
							&& Pawn(Other).PlayerReplicationInfo.Team == Instigator.PlayerReplicationInfo.Team) // so even if friendly fire is on you can't hurt teammates
                            AdjustedDamage = 0;

                        if (Other.Physics == PHYS_Falling || Other.Physics == PHYS_Flying || Other.Physics == PHYS_Swimming)
                            MT = MomentumTransfer;
                        else
                            MT = 0.0;
                        Other.TakeDamage(AdjustedDamage, Instigator, HitLocation, MT*X, DamageType);

                        if (Beam != None)
                            Beam.bLockedOn = true;
                    }
                }
			} 
        }
			
		if ( bShouldStop )
			B.StopFiring();
		else
		{
			// beam effect is created and destroyed when firing starts and stops 
	        
			if ( (Beam == None) && bIsFiring )
				Beam = Spawn(class'LinkBeamEffect',Instigator);
	 
			if (Beam != None)
			{
				if (LinkGun.Linking)
					Beam.LinkColor = Instigator.PlayerReplicationInfo.Team.TeamIndex + 1;
				else
					Beam.LinkColor = 0;
				Beam.Links = LinkGun.Links;
				Beam.LinkedPawn = LockedPawn;
				Beam.bHitSomething = (Other != None);
				Beam.EndEffect = EndEffect;
			}
		}      
    }
    else
    {
        StopFiring();
    }

    bStartFire = false;
    bDoHit = false;
}

function bool BoundError()
{
	CurrentAimError.Yaw = CurrentAimError.Yaw & 65535;
	if ( CurrentAimError.Yaw > 2048 )
	{
		if ( CurrentAimError.Yaw < 32768 )
		{
			CurrentAimError.Yaw = 2048;
			return true;
		}
		else if ( CurrentAimError.Yaw < 63487 )
		{
			CurrentAimError.Yaw = 63487;
			return true;
		}
	}
	return false;
}					

function DoFireEffect()
{
    bDoHit = true;
    UpTime = FireRate+0.1;
}

function PlayFiring()
{
	if (LinkGun(Weapon).Links <= 0 && Weapon.Ammo[ThisModeNum].AmmoAmount >= AmmoPerFire)
		ClientPlayForceFeedback("BLinkGunBeam1");
    Super.PlayFiring();
}

function StopFiring()
{
    if (Beam != None)
    {
        Beam.Destroy();
        Beam = None;
    }
    SetLinkTo(None);
    bStartFire = true;
    bFeedbackDeath = false;
    if (LinkGun(Weapon).Links <= 0)
		StopForceFeedback("BLinkGunBeam1");
}

function SetLinkTo(Pawn Other)
{
    if (LockedPawn != None && Weapon != None)
    {
        RemoveLink(1 + LinkGun(Weapon).Links, Instigator);
        LinkGun(Weapon).Linking = false;
    }

    LockedPawn = Other;

    if (LockedPawn != None)
    {
        if (!AddLink(1 + LinkGun(Weapon).Links, Instigator))
        {
            bFeedbackDeath = true;
        }
        LinkGun(Weapon).Linking = true;
        
        LockedPawn.PlaySound(MakeLinkSound, SLOT_None);
    }
}

function bool AddLink(int Size, Pawn Starter)
{
    local Inventory Inv;
    if (LockedPawn != None && !bFeedbackDeath)
    {
        if (LockedPawn == Starter)
        {
            return false;
        }
        else
        {
            Inv = LockedPawn.FindInventoryType(class'LinkGun');
            if (Inv != None)
            {
                if (LinkFire(LinkGun(Inv).FireMode[1]).AddLink(Size, Starter))
                    LinkGun(Inv).Links += Size;
                else
                    return false;
            }
        }
    }
    return true;
}

function RemoveLink(int Size, Pawn Starter)
{
    local Inventory Inv;
    if (LockedPawn != None && !bFeedbackDeath)
    {
        if (LockedPawn != Starter)
        {
            Inv = LockedPawn.FindInventoryType(class'LinkGun');
            if (Inv != None)
            {
                LinkFire(LinkGun(Inv).FireMode[1]).RemoveLink(Size, Starter);
                LinkGun(Inv).Links -= Size;
            }
        }
    }
}

function bool IsLinkable(Actor Other)
{
    local Pawn P;
    local LinkGun LG;
    local LinkFire LF;
    local int sanity;

    if (Other.IsA('Pawn') && Other.bProjTarget)
    {
        P = Pawn(Other);

        if ( (P.Weapon == None || !P.Weapon.IsA('LinkGun')) )
            return false;

        // pro-actively prevent link cycles from happening
        LG = LinkGun(P.Weapon);
        LF = LinkFire(LG.FireMode[1]);
        while (LF != None && LF.LockedPawn != None && LF.LockedPawn != P && sanity < 20)
        {
            if (LF.LockedPawn == Instigator)
                return false;
            LG = LinkGun(LF.LockedPawn.Weapon);
            if (LG == None)
                break;
            LF = LinkFire(LG.FireMode[1]);
            sanity++;
        }

        return (Level.Game.bTeamGame && P.PlayerReplicationInfo != None
            && P.PlayerReplicationInfo.Team == Instigator.PlayerReplicationInfo.Team);
    }
    return false;
}

simulated function vector GetFireStart(vector X, vector Y, vector Z)
{
    return Instigator.Location + Instigator.EyePosition() + X*Instigator.CollisionRadius;
}

function StartBerserk()
{
    Damage = default.Damage * 1.33;
    Damage = default.Damage * 1.33;
}

function StopBerserk()
{
    Damage = default.Damage;
    Damage = default.Damage;
}

defaultproperties
{
    NoAmmoSound=Sound'WeaponSounds.P1Reload5'

    AmmoClass=class'LinkAmmo'
    AmmoPerFire=1
    DamageType=class'DamTypeLinkShaft'
    Damage=15
    MomentumTransfer=20000.0
    bPawnRapidFireAnim=true

    FireAnim=AltFire
    FireEndAnim=None

    MakeLinkSound=Sound'WeaponSounds.LinkGun.LinkActivated'
    MakeLinkForce="LinkActivated"

    FlashEmitterClass=class'xEffects.LinkMuzFlashBeam1st'

    TraceRange=1100      // range of everything
    LinkFlexibility=0.64 // determines how easy it is to maintain a link.
                         // 1=must aim directly at linkee, 0=linkee can be 90 degrees to either side of you
    
    LinkBreakDelay=0.5   // link will stay established for this long extra when blocked (so you don't have to worry about every last tree getting in the way)

    FireRate=0.2

    BotRefireRate=0.99
	WarnTargetPct=+0.2

    ShakeOffsetMag=(X=0.0,Y=1.0,Z=1.0)
    ShakeOffsetRate=(X=1000.0,Y=1000.0,Z=1000.0)
    ShakeOffsetTime=3
    ShakeRotMag=(X=0.0,Y=0.0,Z=60.0)
    ShakeRotRate=(X=0.0,Y=0.0,Z=4000.0)
    ShakeRotTime=6
    
    bInitAimError=true

    LinkScale(0)=0.0
    LinkScale(1)=0.5
    LinkScale(2)=0.9
    LinkScale(3)=1.2
    LinkScale(4)=1.4
    LinkScale(5)=1.5
}
