class UnrealPawn extends Pawn
	abstract
	config(User);

var	() bool		bNoDefaultInventory;	// don't spawn default inventory for this guy
var bool		bAcceptAllInventory;	// can pick up anything
var(AI) bool	bIsSquadLeader;			// only used as startup property
var bool		bSoakDebug;				// use less verbose version of debug display
var bool		bKeepTaunting;
var byte		LoadOut;		

var config byte SelectedEquipment[16];	// what player has selected (replicate using function)
var()	string	RequiredEquipment[16];	// allow L.D. to modify for single player
var		string	OptionalEquipment[16];	// player can optionally incorporate into loadout

var		float	AttackSuitability;		// range 0 to 1, 0 = pure defender, 1 = pure attacker

var eDoubleClickDir CurrentDir;
var vector			GameObjOffset;
var rotator			GameObjRot;
var(AI) name		SquadName;			// only used as startup property

// allowed voices
var string VoiceType;

var globalconfig bool bPlayerShadows;
var globalconfig bool bBlobShadow;

var int spree;

simulated function bool CheckTauntValid( name Sequence ) 
{
	local int i;

	for(i=0; i<TauntAnims.Length; i++)
	{
		if(Sequence == TauntAnims[i])
			return true;
	}
	return false;
}

/* DisplayDebug()
list important actor variable on canvas.  Also show the pawn's controller and weapon info
*/
simulated function DisplayDebug(Canvas Canvas, out float YL, out float YPos)
{
	local string T;
	local float XL;

	if ( !bSoakDebug )
	{
		Super.DisplayDebug(Canvas, YL, YPos);
		return;
	}

	Canvas.Style = ERenderStyle.STY_Normal;
	Canvas.StrLen("TEST", XL, YL);
	YPos = YPos + 8*YL;
	Canvas.SetPos(4,YPos);
	Canvas.SetDrawColor(255,255,0);
	T = GetDebugName();
	if ( bDeleteMe )
		T = T$" DELETED (bDeleteMe == true)";
	Canvas.DrawText(T, false);
	YPos += 3 * YL;
	Canvas.SetPos(4,YPos);

	if ( Controller == None )
	{
		Canvas.SetDrawColor(255,0,0);
		Canvas.DrawText("NO CONTROLLER");
		YPos += YL;
		Canvas.SetPos(4,YPos);
	}
	else
		Controller.DisplayDebug(Canvas,YL,YPos);

	YPos += 2*YL;
	Canvas.SetPos(4,YPos);
	Canvas.SetDrawColor(0,255,255);
	Canvas.DrawText("Anchor "$Anchor$" Serpentine Dist "$SerpentineDist$" Time "$SerpentineTime);
	YPos += YL;
	Canvas.SetPos(4,YPos);

	T = "Floor "$Floor$" DesiredSpeed "$DesiredSpeed$" Crouched "$bIsCrouched$" Try to uncrouch "$UncrouchTime;
	if ( (OnLadder != None) || (Physics == PHYS_Ladder) )
		T=T$" on ladder "$OnLadder;
	Canvas.DrawText(T);
	YPos += YL;
	Canvas.SetPos(4,YPos);
}

/* BotDodge()
returns appropriate vector for dodge in direction Dir (which should be normalized)
*/
function vector BotDodge(Vector Dir)
{
	local vector Vel;
	
	Vel = GroundSpeed*Dir;
	Vel.Z = JumpZ;
	return Vel;
}

function HoldGameObject(GameObject gameObj, name GameObjBone)
{
	if ( GameObjBone == 'None' )
	{
		GameObj.SetPhysics(PHYS_Rotating);
		GameObj.SetLocation(Location);
		GameObj.SetBase(self);
		GameObj.SetRelativeLocation(vect(0,0,0));
	}	
	else
	{
		AttachToBone(gameObj,GameObjBone);
		gameObj.SetRelativeRotation(GameObjRot);
		gameObj.SetRelativeLocation(GameObjOffset);
	}
}

function EndJump();	// Called when stop jumping

simulated function ShouldUnCrouch();	

simulated event SetAnimAction(name NewAction)
{
	AnimAction = NewAction;
	PlayAnim(AnimAction);
}

function String GetDebugName()
{
	if ( (Bot(Controller) != None) && Bot(Controller).bSoaking && (Level.Pauser != None) )
		return GetHumanReadableName()@Bot(Controller).SoakString;
	if ( PlayerReplicationInfo != None )
		return PlayerReplicationInfo.PlayerName;
	return GetItemName(string(self));
}

function FootStepping(int side);

function name GetWeaponBoneFor(Inventory I)
{
	return 'weapon_bone';
}

function CheckBob(float DeltaTime, vector Y)
{
	local float OldBobTime;
	local int m,n;

	OldBobTime = BobTime;
	Super.CheckBob(DeltaTime,Y);
	
	if ( (Physics != PHYS_Walking) || (VSize(Velocity) < 10)
		|| ((PlayerController(Controller) != None) && PlayerController(Controller).bBehindView) )
		return;
	
	m = int(0.5 * Pi + 9.0 * OldBobTime/Pi);
	n = int(0.5 * Pi + 9.0 * BobTime/Pi);

	if ( (m != n) && !bIsWalking && !bIsCrouched )
		FootStepping(0);
}

/* IsInLoadout()
return true if InventoryClass is part of required or optional equipment
*/
function bool IsInLoadout(class<Inventory> InventoryClass)
{
	local int i;
	local string invstring;

	if ( bAcceptAllInventory )
		return true;
		
	invstring = string(InventoryClass);

	for ( i=0; i<16; i++ )
	{
		if ( RequiredEquipment[i] ~= invstring )
			return true;
		else if ( RequiredEquipment[i] == "" )
			break;
	}

	for ( i=0; i<16; i++ )
	{
		if ( OptionalEquipment[i] ~= invstring )
			return true;
		else if ( OptionalEquipment[i] == "" )
			break;
	}
	return false;
}

function AddDefaultInventory()
{
	local int i;

	if ( IsLocallyControlled() )
	{
		for ( i=0; i<16; i++ )
			if ( RequiredEquipment[i] != "" )
				CreateInventory(RequiredEquipment[i]);

		for ( i=0; i<16; i++ )
			if ( (SelectedEquipment[i] == 1) && (OptionalEquipment[i] != "") )
				CreateInventory(OptionalEquipment[i]);

	    Level.Game.AddGameSpecificInventory(self);
	}
	else
	{
	    Level.Game.AddGameSpecificInventory(self);

		for ( i=15; i>=0; i-- )
			if ( (SelectedEquipment[i] == 1) && (OptionalEquipment[i] != "") )
				CreateInventory(OptionalEquipment[i]);

		for ( i=15; i>=0; i-- )
			if ( RequiredEquipment[i] != "" )
				CreateInventory(RequiredEquipment[i]);
	}

	// HACK FIXME
	if ( inventory != None )
		inventory.OwnerEvent('LoadOut');

	Controller.ClientSwitchToBestWeapon();
}

function CreateInventory(string InventoryClassName)
{
	local Inventory Inv;
	local class<Inventory> InventoryClass;

	InventoryClass = Level.Game.BaseMutator.GetInventoryClass(InventoryClassName);
	if( (InventoryClass!=None) && (FindInventoryType(InventoryClass)==None) )
	{
		Inv = Spawn(InventoryClass);
		if( Inv != None )
		{
			Inv.GiveTo(self);
			Inv.PickupFunction(self);
		}
	}
}

function bool Dodge(eDoubleClickDir DoubleClickMove)
{
	local vector X,Y,Z;

	if ( bIsCrouched || bWantsToCrouch || (Physics != PHYS_Walking) )
		return false;

    GetAxes(Rotation,X,Y,Z);
	if (DoubleClickMove == DCLICK_Forward)
		Velocity = 1.5*GroundSpeed*X + (Velocity Dot Y)*Y;
	else if (DoubleClickMove == DCLICK_Back)
		Velocity = -1.5*GroundSpeed*X + (Velocity Dot Y)*Y; 
	else if (DoubleClickMove == DCLICK_Left)
		Velocity = 1.5*GroundSpeed*Y + (Velocity Dot X)*X; 
	else if (DoubleClickMove == DCLICK_Right)
		Velocity = -1.5*GroundSpeed*Y + (Velocity Dot X)*X; 

	Velocity.Z = 210;
	CurrentDir = DoubleClickMove;
	SetPhysics(PHYS_Falling);
	return true;
}

simulated function PostBeginPlay()
{
	Super.PostBeginPlay();

	if ( Level.bStartup && !bNoDefaultInventory )
		AddDefaultInventory();
}

function PostNetBeginPlay()
{
	local SquadAI S;
	local RosterEntry R;
	
	if ( Level.bStartup )
	{
		if ( UnrealMPGameInfo(Level.Game) == None )
		{
			if ( Bot(Controller) != None )
			{
				ForEach DynamicActors(class'SquadAI',S,SquadName)
					break;
				if ( S == None )
					S = spawn(class'SquadAI');
				S.Tag = SquadName;
				if ( bIsSquadLeader || (S.SquadLeader == None) )
					S.SetLeader(Controller);
				S.AddBot(Bot(Controller));
			}
		}
		else
		{
			R = GetPlacedRoster();
			UnrealMPGameInfo(Level.Game).InitPlacedBot(Controller,R);
		}
	}
}

function RosterEntry GetPlacedRoster()
{
	return None;
}

function SetMovementPhysics()
{
	if (Physics == PHYS_Falling)
		return;
	if ( PhysicsVolume.bWaterVolume )
		SetPhysics(PHYS_Swimming);
	else
		SetPhysics(PHYS_Walking); 
}

function TakeDrowningDamage()	
{	
	TakeDamage(5, None, Location + CollisionHeight * vect(0,0,0.5)+ 0.7 * CollisionRadius * vector(Controller.Rotation), vect(0,0,0), class'Drowned'); 
}

simulated function PlayFootStep(int Side)
{
	if ( (Role==ROLE_SimulatedProxy) || (PlayerController(Controller) == None) || PlayerController(Controller).bBehindView )
	{
		FootStepping(Side);
		return;
	}
}

//-----------------------------------------------------------------------------

/* 
Pawn was killed - detach any controller, and die
*/
simulated function ChunkUp( Rotator HitRotation, float ChunkPerterbation ) // gam
{
	if ( (Level.NetMode != NM_Client) && (Controller != None) )
	{
		if ( Controller.bIsPlayer )
			Controller.PawnDied(self);
		else
			Controller.Destroy();
	}

	bTearOff = true;
	HitDamageType = class'Gibbed'; // make sure clients gib also
	if ( (Level.NetMode == NM_DedicatedServer) || (Level.NetMode == NM_ListenServer) )
		GotoState('TimingOut');
	if ( Level.NetMode == NM_DedicatedServer ) 
		return;
	if ( class'GameInfo'.static.UseLowGore() )
	{
		Destroy();
		return;
	}
	SpawnGibs(HitRotation,ChunkPerterbation);

	if ( Level.NetMode != NM_ListenServer )
		Destroy();
}

// spawn gibs (local, not replicated)
simulated function SpawnGibs(Rotator HitRotation, float ChunkPerterbation);

/* TimingOut - where gibbed pawns go to die (delay so they can get replicated)
*/
State TimingOut
{
ignores BaseChange, Landed, AnimEnd, Trigger, Bump, HitWall, HeadVolumeChange, PhysicsVolumeChange, Falling, BreathTimer;

	function TakeDamage( int Damage, Pawn instigatedBy, Vector hitlocation, 
							Vector momentum, class<DamageType> damageType)
	{
	}

	function BeginState()
	{
		SetPhysics(PHYS_None);
		SetCollision(false,false,false);
		LifeSpan = 1.0;
		if ( Controller != None )
		{
			if ( Controller.bIsPlayer )
				Controller.PawnDied(self);
			else
				Controller.Destroy();
		}
	}
}

State Dying
{
ignores AnimEnd, Trigger, Bump, HitWall, HeadVolumeChange, PhysicsVolumeChange, Falling, BreathTimer;

	function Landed(vector HitNormal)
	{
		LandBob = FMin(50, 0.055 * Velocity.Z); 
		if ( Level.NetMode == NM_DedicatedServer )
			return;
		if ( Shadow != None )
			Shadow.Destroy();
	}

	singular function BaseChange()
	{
		Super.BaseChange();
		// fixme - wake up karma
	}

	function BeginState()
	{
		if ( (LastStartSpot != None) && (Level.TimeSeconds - LastStartTime < 7) )
			LastStartSpot.LastSpawnCampTime = Level.TimeSeconds;
		SetCollision(true,false,false);
        if ( bTearOff && (Level.NetMode == NM_DedicatedServer) )
			LifeSpan = 1.0;
		else
			SetTimer(2.0, false);
        SetPhysics(PHYS_Falling);
		bInvulnerableBody = true;
		if ( Controller != None )
		{
			if ( Controller.bIsPlayer )
				Controller.PawnDied(self);
			else
				Controller.Destroy();
		}
	}
}

defaultproperties
{
	 SquadName=Squad
	 MeleeRange=+20.0
     ControllerClass=class'UnrealGame.Bot'
	 bMuffledHearing=true

     Buoyancy=+00099.000000
     UnderWaterTime=+00020.000000
     BaseEyeHeight=+00060.000000
     EyeHeight=+00060.000000
     CollisionRadius=+00034.000000
     CollisionHeight=+00078.000000
	 CrouchHeight=+39.0
	 CrouchRadius=+34.0
     GroundSpeed=+00600.000000
     AirSpeed=+00600.000000
     WaterSpeed=+00300.000000
     AccelRate=+02048.000000
     JumpZ=+00420.000000
     bCanStrafe=True
	 bCanSwim=true
     DrawType=DT_Mesh
     LightBrightness=70
     LightHue=40
     LightSaturation=128
     LightRadius=6
     RotationRate=(Pitch=0,Yaw=20000,Roll=2048)
	 AmbientGlow=17
	 AirControl=+0.35
	 bStasis=false
	 bCanCrouch=true
	 bCanClimbLadders=True
	 bCanPickupInventory=True
	 WalkingPct=+0.3
	ForceType=FT_DragAlong
	ForceRadius=100
	ForceScale=2.5
	SightRadius=+12000.0
	LoadOut=255
	AttackSuitability=+0.5
	VoiceType="UnrealGame.TempVoice"
	bAcceptAllInventory=true
}
