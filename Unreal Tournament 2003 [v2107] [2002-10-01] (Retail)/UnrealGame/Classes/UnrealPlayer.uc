//=============================================================================
// UnrealPlayer.
//=============================================================================
class UnrealPlayer extends PlayerController
	config(User);

var bool		bRising;
var bool		bLatecomer;		// entered multiplayer game after game started
var bool		bDisplayLoser;
var bool		bDisplayWinner;
var class<WillowWhisp> PathWhisps[2];
var int LastTaunt;
var float LastWhispTime;

var() int MultiKillLevel;
var() float LastKillTime;

var float LastTauntAnimTime;


replication
{
	// Things the server should send to the client.
	unreliable if( Role==ROLE_Authority )
		ClientPlayTakeHit,ClientDelayedAnnouncement;
	reliable if ( Role == ROLE_Authority )
		PlayWinMessage, PlayStartupMessage; 

	reliable if ( Role < ROLE_Authority )
		ServerTaunt, ServerChangeLoadout, ServerSpectate, ServerShowPathToBase;
}

function AwardAdrenaline(float amount)
{
	if ( bAdrenalineEnabled )
	{
		if ( (Adrenaline < AdrenalineMax) && (Adrenaline+amount >= AdrenalineMax) && ((Pawn == None) || !Pawn.InCurrentCombo()) )
			ClientDelayedAnnouncement(sound'AnnouncerMain.Adrenalin',15);
		Super.AwardAdrenaline(Amount);
	}
}

function ClientDelayedAnnouncement(sound AnnouncementSound, byte Delay)
{
	local AnnounceAdrenaline A;
	
	A = spawn(class'AnnounceAdrenaline', self);
	A.AnnounceSound = AnnouncementSound;
	A.settimer(0.1*delay,false);
}

function LogMultiKills(float Reward, bool bEnemyKill)
{
	if ( bEnemyKill && (Level.TimeSeconds - LastKillTime < 4) )
	{
		AwardAdrenaline( Reward );
		MultiKillLevel++;
		if (Level.Game.GameStats!=None) 
			Level.Game.GameStats.SpecialEvent(PlayerReplicationInfo,"multikill_"$MultiKillLevel);
	}
	else
		MultiKillLevel=0;
		
	if ( bEnemyKill )
		LastKillTime = Level.TimeSeconds;
}	

exec function ShowAI()
{
	myHUD.ShowDebug();
	if ( UnrealPawn(ViewTarget) != None )
		UnrealPawn(ViewTarget).bSoakDebug = myHUD.bShowDebugInfo;
}

function Possess(Pawn aPawn)
{
	if ( UnrealPawn(aPawn) != None )
	{
		if ( UnrealPawn(aPawn).Default.VoiceType != "" )
			VoiceType = UnrealPawn(aPawn).Default.VoiceType;
		if ( VoiceType != "" )
        {
			PlayerReplicationInfo.VoiceType = class<VoicePack>(DynamicLoadObject(VoiceType,class'Class'));
        }
	}
	Super.Possess(aPawn);
}

function bool AutoTaunt()
{
	return bAutoTaunt; 
}

function bool DontReuseTaunt(int T)
{
	if ( T == LastTaunt )
		return true;

	if( T == Level.LastTaunt[0] || T == Level.LastTaunt[1] )
		return true;

	LastTaunt = T;

	Level.LastTaunt[1] = Level.LastTaunt[0];
	Level.LastTaunt[0] = T;

	return false;
}

exec function SoakBots()
{
	local Bot B;

	log("Start Soaking");
	UnrealMPGameInfo(Level.Game).bSoaking = true;
	ForEach DynamicActors(class'Bot',B)
		B.bSoaking = true;
}

function SoakPause(Pawn P)
{
	log("Soak pause by "$P);
	SetViewTarget(P);
	SetPause(true);
	bBehindView = true;
	myHud.bShowDebugInfo = true;
	if ( UnrealPawn(P) != None )
		UnrealPawn(P).bSoakDebug = true;
}

exec function BasePath(byte num)
{
	if (PlayerReplicationInfo.Team == None )
		return;
	ServerShowPathToBase(num);
}

function ServerShowPathToBase(int TeamNum)
{
	local GameObjective G,Best;

	if ( (Level.NetMode != NM_Standalone) && (Level.TimeSeconds - LastWhispTime < 0.5) )
		return;
	LastWhispTime = Level.TimeSeconds;
	
	if ( (Pawn == None) || (TeamGame(Level.Game) == None) || !TeamGame(Level.Game).CanShowPathTo(self,TeamNum) )
		return;

	for ( G=TeamGame(Level.Game).Teams[0].AI.Objectives; G!=None; G=G.NextObjective )
		if ( G.BetterObjectiveThan(Best,TeamNum,PlayerReplicationInfo.Team.TeamIndex) )
			Best = G;
	if ( (Best != None) && (FindPathToward(Best,false) != None) )
		spawn(PathWhisps[TeamNum],self,,Pawn.Location);	
}

function byte GetMessageIndex(name PhraseName)
{
	if ( PlayerReplicationInfo.VoiceType == None )
		return 0;
	return PlayerReplicationInfo.Voicetype.Static.GetMessageIndex(PhraseName);
}

exec function RandomTaunt()
{
	local int tauntNum;

	if(Pawn == None)
		return;

	// First 4 taunts are 'order' anims. Don't pick them.
	tauntNum = Rand(Pawn.TauntAnims.Length - 4);
	Taunt(Pawn.TauntAnims[4 + tauntNum]);
}

exec function Taunt( name Sequence )
{
	if ( (UnrealPawn(Pawn) != None) && (Pawn.Health > 0) && (Level.TimeSeconds - LastTauntAnimTime > 0.3) && UnrealPawn(Pawn).CheckTauntValid(Sequence) )
	{
		ServerTaunt(Sequence);
        LastTauntAnimTime = Level.TimeSeconds;
	}
}

function ServerTaunt(name AnimName )
{
	Pawn.SetAnimAction(AnimName);
}

function PlayStartupMessage(byte StartupStage)
{
	ReceiveLocalizedMessage( class'StartupMessage', StartupStage, PlayerReplicationInfo );
}

exec function CycleLoadout()
{
	if ( UnrealTeamInfo(PlayerReplicationInfo.Team) != None )
		ServerChangeLoadout(string(UnrealTeamInfo(PlayerReplicationInfo.Team).NextLoadOut(PawnClass)));
}

exec function ChangeLoadout(string LoadoutName)
{
	ServerChangeLoadout(LoadoutName);
}

function ServerChangeLoadout(string LoadoutName)
{
	UnrealMPGameInfo(Level.Game).ChangeLoadout(self, LoadoutName);
}

function NotifyTakeHit(pawn InstigatedBy, vector HitLocation, int Damage, class<DamageType> damageType, vector Momentum)
{
	local int iDam;

	Super.NotifyTakeHit(InstigatedBy,HitLocation,Damage,DamageType,Momentum);

	if ( (Pawn != None) && (Pawn.ShieldStrength > 0) )
		ClientFlash(0.5,vect(700,700,0));
	else if ( Damage > 1 )
		ClientFlash(DamageType.Default.FlashScale,DamageType.Default.FlashFog);

	DamageShake(Damage); 
	iDam = Clamp(Damage,0,250);
	if ( (Level.NetMode == NM_DedicatedServer) || (Level.NetMode == NM_ListenServer) )
		ClientPlayTakeHit(hitLocation - Pawn.Location, iDam, damageType); 
}

function ClientPlayTakeHit(vector HitLoc, byte Damage, class<DamageType> damageType)
{
	HitLoc += Pawn.Location;
    if( bEnableDamageForceFeedback )        // jdf
        ClientPlayForceFeedback("Damage");  // jdf
	Pawn.PlayTakeHit(HitLoc, Damage, damageType);
}
	
function PlayWinMessage(bool bWinner)
{
	if ( bWinner )
		bDisplayWinner = true;
	else
		bDisplayLoser = true;
}

// Player movement.
// Player Standing, walking, running, falling.
state PlayerWalking
{
ignores SeePlayer, HearNoise;

	function bool NotifyLanded(vector HitNormal)
	{
		if (DoubleClickDir == DCLICK_Active)
		{
			DoubleClickDir = DCLICK_Done;
			ClearDoubleClick();
			Pawn.Velocity *= Vect(0.1,0.1,1.0);
		}
		else
			DoubleClickDir = DCLICK_None;

		if ( Global.NotifyLanded(HitNormal) ) // jjs - moved this down here to fix dodge freezing bug
			return true;

		return false;
	}

	function ProcessMove(float DeltaTime, vector NewAccel, eDoubleClickDir DoubleClickMove, rotator DeltaRot)	
	{
		if ( (DoubleClickMove == DCLICK_Active) && (Pawn.Physics == PHYS_Falling) )
			DoubleClickDir = DCLICK_Active;
		else if ( (DoubleClickMove != DCLICK_None) && (DoubleClickMove < DCLICK_Active) )
		{
			if ( UnrealPawn(Pawn).Dodge(DoubleClickMove) )
				DoubleClickDir = DCLICK_Active;
		}

		Super.ProcessMove(DeltaTime,NewAccel,DoubleClickMove,DeltaRot);
	}
}

function ServerSpectate()
{
	GotoState('Spectating');
}

state Dead
{
ignores SeePlayer, HearNoise, KilledBy, SwitchWeapon;

	exec function Fire( optional float F )
	{
		if ( bFrozen )
		{
			if ( (TimerRate <= 0.0) || (TimerRate > 1.0) )
				bFrozen = false;
			return;
		}
		if ( PlayerReplicationInfo.bOutOfLives )
			ServerSpectate();
		else 
			Super.Fire(F);
	}
	
Begin:
    Sleep(3.0);
	if ( (ViewTarget == self) || (VSize(ViewTarget.Velocity) < 1.0) )
	{
		Sleep(1.0);
		myHUD.bShowScoreBoard = true;
	}
	else
		Goto('Begin');
}

defaultproperties
{
	 PathWhisps[0]=class'WillowWhisp'
	 PathWhisps[1]=class'WillowWhisp'
     FovAngle=+00085.000000
	 LocalMessageClass=class'LocalMessage'
	 PlayerReplicationInfoClass=Class'UnrealGame.TeamPlayerReplicationInfo'
}

