//=============================================================================
// Link Gun
//=============================================================================
class LinkGun extends Weapon
    config(user);

#EXEC OBJ LOAD FILE=InterfaceContent.utx

var() int Links;
var() bool Linking;
var bool bBotLinkUp;

replication
{
    unreliable if (Role == ROLE_Authority)
        Linking, Links;
}

simulated function Destroyed()
{
    if (Role == ROLE_Authority)
    {
        LinkFire(FireMode[1]).SetLinkTo(None);
    }
    Super.Destroyed();
}

simulated function bool StartFire(int Mode)
{
	local SquadAI S;
	local Bot B;
	
	if ( (Role == ROLE_Authority) && (PlayerController(Instigator.Controller) != None) && (UnrealTeamInfo(Instigator.PlayerReplicationInfo.Team) != None))
	{
		// tell followers to focus on me
		S = UnrealTeamInfo(Instigator.PlayerReplicationInfo.Team).AI.GetSquadLedBy(Instigator.Controller);
		if ( S != None )
		{
			for ( B=S.SquadMembers; B!=None; B=B.NextSquadMember )
				if ( (HoldSpot(B.GoalScript) == None)
					&& (B.Pawn != None)
					&& (LinkGun(B.Pawn.Weapon) != None)
					&& B.Pawn.Weapon.FocusOnLeader(true) )
				{
					B.Focus = Instigator;
					B.FireWeaponAt(Instigator);
				}
		}
	}
	return Super.StartFire(Mode);
}

// AI Interface
function bool FocusOnLeader(bool bLeaderFiring)
{
	local Bot B;
	local Pawn LeaderPawn;
	local Actor Other;
	local vector HitLocation, HitNormal, StartTrace;
		
	if ( bBotLinkUp )
		return true;
		
	B = Bot(Instigator.Controller);
	if ( B == None )
		return false;
	if ( PlayerController(B.Squad.SquadLeader) != None )
		LeaderPawn = B.Squad.SquadLeader.Pawn;
	if ( LeaderPawn == None )
		return false;
	if ( !bLeaderFiring && !LeaderPawn.Weapon.IsFiring() )
		return false;
	if ( LinkGun(LeaderPawn.Weapon) != None ) 
	{
		StartTrace = Instigator.Location + Instigator.EyePosition();
		if ( VSize(LeaderPawn.Location - StartTrace) < LinkFire(FireMode[1]).TraceRange ) 
		{
			Other = Trace(HitLocation, HitNormal, LeaderPawn.Location, StartTrace, true);
			if ( Other == LeaderPawn )
				return true;
		}
	}
	return false;
}	 

function float GetAIRating()
{
	local Bot B;
	
	B = Bot(Instigator.Controller);
	if ( B == None )
		return AIRating;
	
	if ( (PlayerController(B.Squad.SquadLeader) != None)
		&& (B.Squad.SquadLeader.Pawn != None)
		&& (LinkGun(B.Squad.SquadLeader.Pawn.Weapon) != None) )
		return 1.2;	
	return AIRating * FMin(Pawn(Owner).DamageScaling, 1.5);
}

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	local float EnemyDist;
	local bot B;

	B = Bot(Instigator.Controller);
	if ( B == None )
		return 0;
	if ( FocusOnLeader(B.Focus == B.Squad.SquadLeader.Pawn) )
	{
		B.Focus = B.Squad.SquadLeader.Pawn; 
		return 1;
	}

	if ( B.Enemy == None )
		return 0;
	EnemyDist = VSize(B.Enemy.Location - Instigator.Location);
	if ( EnemyDist > LinkFire(FireMode[1]).TraceRange )
		return 0;
	return 1;
}

function float SuggestAttackStyle()
{
	return 0.8;
}

function float SuggestDefenseStyle()
{
    return -0.4;
}
// End AI Interface

function ConsumeAmmo(int mode, float load)
{
    local Pawn Other;
    local LinkGun OtherWeapon, Head;
    local int sanity;

    if (Linking)
    {
        Head = self;
        while (Head != None && Head.Linking && sanity < 20)
        {
            Other = LinkFire(Head.FireMode[1]).LockedPawn;
            if (Other == None)
                break;
            else
            {
                OtherWeapon = LinkGun(Other.Weapon);
                if (OtherWeapon == None)
                    break;
                else
                    Head = OtherWeapon;
            }
            sanity++;
        }
                   
        if (Head.FireMode[0].bIsFiring || Head.FireMode[1].bIsFiring)
        {
            if (Ammo[Mode] != None)
                Ammo[Mode].UseAmmo(1);
        }
    }
    else if (Links == 0)
    {
        if (Ammo[mode] != None)
            Ammo[mode].UseAmmo(1);
    }
}

simulated function IncrementFlashCount(int mode)
{
    Super.IncrementFlashCount(mode);
	if ( WeaponAttachment(ThirdPersonActor) != None )
        LinkAttachment(ThirdPersonActor).Links = Links;
}

simulated function bool PutDown()
{
    LinkFire(FireMode[1]).SetLinkTo(None);
    Links = 0;
    return Super.PutDown();
}

simulated function BringUp(optional Weapon PrevWeapon)
{
    Links = 0;
    Super.BringUp();
}

// jdf ---
simulated event WeaponTick(float dt)
{
	local PlayerController PC;
	local LinkFire LF;
	
	PC = PlayerController(Instigator.Controller);
	LF = LinkFire(FireMode[1]);
	
	if (PC != None && LF != None)
	{
		if (Links > 0 && !LF.bLinkFeedbackPlaying)
		{
			LF.bLinkFeedbackPlaying = true;
			PC.ClientPlayForceFeedback(LF.MakeLinkForce);
			if (!LF.bIsFiring)
				PC.ClientPlayForceFeedback("BLinkGunBeam1");
		}
		else if (Links <= 0 && LF.bLinkFeedbackPlaying)
		{
			LF.bLinkFeedbackPlaying = false;
			PC.StopForceFeedback("BLinkGunBeam1");	
		}
	}		
}
// --- jdf

defaultproperties
{
	bMatchWeapons=true
    ItemName="Link Gun"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=200,Y1=190,X2=321,Y2=280)

    FireModeClass(0)=LinkAltFire
    FireModeClass(1)=LinkFire
    InventoryGroup=5
    DrawScale=1.0
    Mesh=mesh'Weapons.LinkGun_1st'
    BobDamping=1.575000
    PickupClass=class'LinkGunPickup'
    EffectOffset=(X=100,Y=25,Z=-3)
    IdleAnimRate=0.03
    PutDownAnim=PutDown
    DisplayFOV=60

    PlayerViewOffset=(X=-2,Y=-2,Z=-3)
    PlayerViewPivot=(Pitch=0,Roll=0,Yaw=500)
    UV2Texture=Material'XGameShaders.WeaponEnvShader'

    AttachmentClass=class'LinkAttachment'
    SelectSound=Sound'WeaponSounds.LinkGun.SwitchToLinkGun'
	SelectForce="SwitchToLinkGun"
	
	AIRating=+0.68
	CurrentRating=+0.68

    Links=0

	DefaultPriority=5
}
