//=============================================================================
// DeathMatch
//=============================================================================
class DeathMatch extends UnrealMPGameInfo
    config;

#exec OBJ LOAD FILE=TeamSymbols_UT2003.utx
#exec OBJ LOAD FILE=TeamSymbols.utx				// needed right now for Link symbols, etc.
#exec OBJ LOAD File=AnnouncerMain.uax

var int NumRounds;
var globalconfig int NetWait;       // time to wait for players in netgames w/ bNetReady (typically team games)
var globalconfig int MinNetPlayers; // how many players must join before net game will start
var globalconfig int RestartWait;

// client game rendering options 
var globalconfig bool bNoCoronas;					// don't display team coronas on player shoulders
var globalconfig bool bForceDefaultCharacter;		// all characters shown using default (jugg male) mesh

var globalconfig bool bTournament;  // number of players must equal maxplayers for game to start
var config bool bPlayersMustBeReady;// players must confirm ready for game to start
var config bool bForceRespawn;
var config bool bAdjustSkill;
var config bool bAllowTaunts;
var bool	bAllowTrans;
var bool    bWaitForNetPlayers;     // wait until more than MinNetPlayers players have joined before starting match
var bool    bMustJoinBeforeStart;   // players can only spectate if they join after the game starts
var bool	bFirstBlood;
var bool	bQuickStart;
var bool	bStartedCountDown;
var bool	bFinalStartup;
var bool	bOverTimeBroadcast;
var bool	bEpicNames;
var bool	bKillBots;

var byte StartupStage;              // what startup message to display
var int RemainingTime, ElapsedTime;
var int CountDown;
var float AdjustedDifficulty;
var int PlayerKills, PlayerDeaths;
var class<SquadAI> DMSquadClass;    // squad class to use for bots in DM games (no team)
var class<LevelGameRules> LevelRulesClass;
var LevelGameRules LevelRules;		// level designer overriding of game settings (hook for mod authors)
var config float SpawnProtectionTime;
var UnrealTeamInfo EnemyRoster;
var string EnemyRosterName;
var string DefaultEnemyRosterClass;

// Bot related info
var     int         RemainingBots;
var     int         InitialBots;

var NavigationPoint LastPlayerStartSpot;    // last place player looking for start spot started from
var NavigationPoint LastStartSpot;          // last place any player started from

var     int         NameNumber;             // append to ensure unique name if duplicate player name change requested

var int             EndMessageWait;         // wait before playing which team won the match
var transient int   EndMessageCounter;      // end message counter
var Sound           EndGameSound[2];        // end game sounds
var Sound AltEndGameSound[2];
var int             SinglePlayerWait;       // single-player wait delay before auto-returning to menus

var globalconfig string NamePrefixes[10];		// for bots with same name
var globalconfig string NameSuffixes[10];		// for bots with same name

var actor EndGameFocus;
var PlayerController StandalonePlayer;

// mc - localized PlayInfo descriptions & extra info
var private localized string DMPropsDisplayText[9];

var() float ADR_Kill;
var() float ADR_MajorKill;
var() float ADR_MinorError;
var() float ADR_MinorBonus;
var() float ADR_KillTeamMate;

var string EpicNames[21];
var int EpicOffset, TotalEpic;

function PostBeginPlay()
{
	NameNumber = rand(10);
	EpicOffset = rand(10);

    Super.PostBeginPlay();
    GameReplicationInfo.RemainingTime = RemainingTime;
    InitTeamSymbols();
    GetBotTeam(InitialBots);
    if ( CurrentGameProfile == None )
		OverrideInitialBots();
}

function OverrideInitialBots()
{
	InitialBots = GetBotTeam().OverrideInitialBots(InitialBots,None);
}

event PreLogin
(
    string Options,
    string Address,
    out string Error,
    out string FailCode
)
{
	local controller C;
	
	Super.PreLogin(Options,Address,Error,FailCode);
	if ( (Error == "") && (FailCode == "") && (MaxLives > 0) )
	{
		// check that game isn't too far along
		for ( C=Level.ControllerList; C!=None; C=C.NextController )
		{
			if ( (C.PlayerReplicationInfo != None) && (C.PlayerReplicationInfo.NumLives > 1) )
			{
				Error = "LastManStanding game already in progress";
				FailCode = "GAMESTARTED";
				return;
			}
		}
	}
}
	
/* Reset() 
reset actor to initial state - used when restarting level without reloading.
*/
function Reset()
{
    Super.Reset();
    ElapsedTime = NetWait - 3;
    bWaitForNetPlayers = ( Level.NetMode != NM_StandAlone );
	bStartedCountDown = false;
	bFinalStartup = false;
    CountDown = Default.Countdown;
    RemainingTime = 60 * TimeLimit;
    //log("Reset() RemainingTime:"$RemainingTime$" TimeLimit: "$TimeLimit); // sjs
    GotoState('PendingMatch');
}

/* CheckReady()
If tournament game, make sure that there is a valid game winning criterion
*/
function CheckReady()
{
    if ( (GoalScore == 0) && (TimeLimit == 0) )
    {
        TimeLimit = 20;
        RemainingTime = 60 * TimeLimit;
    }
}

// Monitor killed messages for fraglimit
function Killed( Controller Killer, Controller Killed, Pawn KilledPawn, class<DamageType> damageType )
{
	local bool bEnemyKill;
	
	bEnemyKill = ( !bTeamGame || ((Killer != None) && (Killer != Killed) && (Killed != None)
								&& (Killer.PlayerReplicationInfo != None) && (Killed.PlayerReplicationInfo != None)
								&& (Killer.PlayerReplicationInfo.Team != Killed.PlayerReplicationInfo.Team)) );
	if ( UnrealPlayer(Killer) != None )
		UnrealPlayer(Killer).LogMultiKills(ADR_MajorKill, bEnemyKill);
   
	if ( (UnrealPawn(KilledPawn) != None) && (UnrealPawn(KilledPawn).Spree > 4) )
	{
		if ( bEnemyKill && (Killer != None) )
			Killer.AwardAdrenaline(ADR_MajorKill);
		EndSpree(Killer, Killed); 
	}
	if ( (Killer != None) && Killer.bIsPlayer && (Killed != None) && Killed.bIsPlayer )
	{
		DamageType.static.IncrementKills(Killer);

		if ( !bFirstBlood && (Killer != Killed) && bEnemyKill )
		{
			Killer.AwardAdrenaline(ADR_MajorKill);
			bFirstBlood = True;
			BroadcastLocalizedMessage( class'FirstBloodMessage', 0, Killer.PlayerReplicationInfo );

			if (GameStats!=None)	//cal Firstblood
				GameStats.SpecialEvent(Killer.PlayerReplicationInfo,"first_blood");		
		}
		if ( Killer == Killed )
			Killer.AwardAdrenaline(ADR_MinorError);
		else if ( bTeamGame && (Killed.PlayerReplicationInfo.Team == Killer.PlayerReplicationInfo.Team) )
			Killer.AwardAdrenaline(ADR_KillTeamMate);	
		else if ( UnrealPawn(Killer.Pawn) != None ) 
		{
			Killer.AwardAdrenaline(ADR_Kill);
			UnrealPawn(Killer.Pawn).Spree++;
			if ( UnrealPawn(Killer.Pawn).Spree > 4 )
				NotifySpree(Killer, UnrealPawn(Killer.Pawn).Spree);
		} 
	}
    Super.Killed(Killer, Killed, KilledPawn, damageType);
}

function InitLogging()
{
	// no stats logging with bots
	if ( MinPlayers > 1 )
		return;
		
	Super.InitLogging();
}

function AddGameSpecificInventory(Pawn p)
{
    if ( bAllowTrans )
        p.CreateInventory("XWeapons.TransLauncher");
    Super.AddGameSpecificInventory(p);
}

// Parse options for this game...
event InitGame( string Options, out string Error )
{
    local string InOpt;
    local bool bAutoNumBots;

    // find Level's LevelGameRules actor if it exists
    ForEach AllActors(class'LevelGameRules', LevelRules)
        break;
    if ( LevelRules != None )
		LevelRules.UpdateGame(self);
    
    Super.InitGame(Options, Error);

    SetGameSpeed(GameSpeed);
    MaxLives = Max(0,GetIntOption( Options, "MaxLives", MaxLives ));
    if ( MaxLives > 0 )
		bForceRespawn = true;
    GoalScore = Max(0,GetIntOption( Options, "GoalScore", GoalScore ));
    TimeLimit = Max(0,GetIntOption( Options, "TimeLimit", TimeLimit ));

	InOpt = ParseOption( Options, "Translocator");
    // For instant action, use map defaults
    if ( InOpt != "" )
    {
        log("Translocators: "$bool(InOpt));
        bAllowTrans = bool(InOpt);
    }
    InOpt = ParseOption( Options, "bAutoNumBots");
    if ( InOpt != "" )
    {
        log("bAutoNumBots: "$bool(InOpt));
        bAutoNumBots = bool(InOpt);
    }
    InOpt = ParseOption( Options, "AutoAdjust");
    if ( InOpt != "" )
    {
        bAdjustSkill = bool(InOpt);
        log("Adjust skill "$bAdjustSkill);
    }

	EnemyRosterName = ParseOption( Options, "DMTeam");

    // SP
    if (CurrentGameProfile != None)
    {
		MaxLives = 0;
		bAllowTrans = default.bAllowTrans;
        bAdjustSkill = false;
		// if numbots isn't set by the URL from LadderInfo, go with
		// auto number
		if ( GetIntOption( Options, "NumBots", -1) <= 0 )	
			bAutoNumBots = true;
    }
    
    if (bAutoNumBots)
    {
        MaxPlayers = Level.IdealPlayerCountMax;
        MinPlayers = GetMinPlayers();
        
        if ((MinPlayers & 1) == 1)
            MinPlayers++;
            
        if( MinPlayers < 2 )
            MinPlayers = 2;
        
        InitialBots = Max(0,MinPlayers - 1);
    }
    else
    {
        MinPlayers = Clamp(GetIntOption( Options, "MinPlayers", MinPlayers ),0,32);
        InitialBots = Clamp(GetIntOption( Options, "NumBots", InitialBots ),0,32);
    }

    RemainingTime = 60 * TimeLimit;
    
    InOpt = ParseOption( Options, "WeaponStay");
    if ( InOpt != "" )
    {
        log("WeaponStay: "$bool(InOpt));
        bWeaponStay = bool(InOpt);
    }

    bTournament = (GetIntOption( Options, "Tournament", 0 ) > 0);
    if ( bTournament ) 
        CheckReady();
    bWaitForNetPlayers = ( Level.NetMode != NM_StandAlone );
    
    InOpt = ParseOption(Options,"QuickStart");
    if ( InOpt != "" )
		bQuickStart = true;
		
    AdjustedDifficulty = GameDifficulty;
}

function Texture GetRandomTeamSymbol(int base)
{
    local string SymbolName;
    local int SymbolIndex;
    
    SymbolIndex = base + Rand(9);

    if (SymbolIndex < 9)
        SymbolName = "TeamSymbols_UT2003.Sym0" $ SymbolIndex+1;
    else
        SymbolName = "TeamSymbols_UT2003.Sym" $ SymbolIndex+1;

    return Texture(DynamicLoadObject(SymbolName, class'Texture'));
}

function int GetMinPlayers()
{
    if (CurrentGameProfile == None)
        return (Level.IdealPlayerCountMax + Level.IdealPlayerCountMin)/2;

    return Level.SinglePlayerTeamSize*2;
}

/* AcceptInventory()
Examine the passed player's inventory, and accept or discard each item
* AcceptInventory needs to gracefully handle the case of some inventory
being accepted but other inventory not being accepted (such as the default
weapon).  There are several things that can go wrong: A weapon's
AmmoType not being accepted but the weapon being accepted -- the weapon
should be killed off. Or the player's selected inventory item, active
weapon, etc. not being accepted, leaving the player weaponless or leaving
the HUD inventory rendering messed up (AcceptInventory should pick another
applicable weapon/item as current).
*/
function AcceptInventory(pawn PlayerPawn)
{
    while ( PlayerPawn.Inventory != None )
        PlayerPawn.Inventory.Destroy();

    PlayerPawn.Weapon = None;
    PlayerPawn.SelectedItem = None;
    AddDefaultInventory( PlayerPawn );
}

function bool CheckEndGame(PlayerReplicationInfo Winner, string Reason)
{
    local Controller P;
    local PlayerController Player;
    local bool bLastMan;
    
    bLastMan = ( Reason ~= "LastMan" );

    if ( !bLastMan && (GameRulesModifiers != None) && !GameRulesModifiers.CheckEndGame(Winner, Reason) )
        return false;

	if ( Winner == None )
	{
		// find winner
		for ( P=Level.ControllerList; P!=None; P=P.nextController )
			if ( P.bIsPlayer && !P.PlayerReplicationInfo.bOutOfLives 
				&& ((Winner == None) || (P.PlayerReplicationInfo.Score >= Winner.Score)) )
			{
				Winner = P.PlayerReplicationInfo;
			}       
	}
			
    // check for tie
    if ( !bLastMan )
    {
		for ( P=Level.ControllerList; P!=None; P=P.nextController )
		{
			if ( P.bIsPlayer && 
				(Winner != P.PlayerReplicationInfo) && 
				(P.PlayerReplicationInfo.Score == Winner.Score) 
				&& !P.PlayerReplicationInfo.bOutOfLives )
			{
				if ( !bOverTimeBroadcast )
				{
					StartupStage = 7;
					PlayStartupMessage();
					bOverTimeBroadcast = true;
				}
				return false;
			}       
		}
	}
	
    EndTime = Level.TimeSeconds + 3.0;
    GameReplicationInfo.Winner = Winner;
    if ( CurrentGameProfile != None )
		CurrentGameProfile.bWonMatch = (PlayerController(Winner.Owner) != None);

    EndGameFocus = Controller(Winner.Owner).Pawn;
    if ( EndGameFocus != None )
		EndGameFocus.bAlwaysRelevant = true;
    for ( P=Level.ControllerList; P!=None; P=P.nextController )
    {
        Player = PlayerController(P);
        if ( Player != None )
        {
            PlayWinMessage(Player, (Player.PlayerReplicationInfo == Winner));
            Player.ClientSetBehindView(true);
            if ( EndGameFocus != None )
            {
				Player.ClientSetViewTarget(EndGameFocus);
                Player.SetViewTarget(EndGameFocus);
            }
            Player.ClientGameEnded();
        }
        P.GameHasEnded();
    }
    return true;
}

function PlayWinMessage(PlayerController Player, bool bWinner)
{
    UnrealPlayer(Player).PlayWinMessage(bWinner);
}

event PlayerController Login
(
    string Portal,
    string Options,
    out string Error
)
{
    local PlayerController NewPlayer;

    NewPlayer = Super.Login(Portal,Options,Error);
    if ( bMustJoinBeforeStart && GameReplicationInfo.bMatchHasBegun )
        UnrealPlayer(NewPlayer).bLatecomer = true;
  
	if ( Level.NetMode == NM_Standalone )
	{      
		if( NewPlayer.PlayerReplicationInfo.bOnlySpectator )
		{
			// Compensate for the space left for the player
			InitialBots++;
		}
		else
			StandalonePlayer = NewPlayer;
	}
	
    return NewPlayer;
}

event PostLogin( playercontroller NewPlayer )
{
    Super.PostLogin(NewPlayer);
    UnrealPlayer(NewPlayer).PlayStartUpMessage(StartupStage);
}

function ChangeLoadOut(PlayerController P, string LoadoutName)
{
    local class<UnrealPawn> NewLoadout;

    NewLoadout = class<UnrealPawn>(DynamicLoadObject(LoadoutName,class'Class'));
    if ( (NewLoadout != None) 
        && ((UnrealTeamInfo(P.PlayerReplicationInfo.Team) == None) || UnrealTeamInfo(P.PlayerReplicationInfo.Team).BelongsOnTeam(NewLoadout)) )
    {
        P.PawnClass = NewLoadout;
        if (P.Pawn!=None)
            P.ClientMessage("Your next class is "$P.PawnClass.Default.MenuName);
    }
}

function RestartPlayer( Controller aPlayer )    
{
    if ( bMustJoinBeforeStart && (UnrealPlayer(aPlayer) != None)
        && UnrealPlayer(aPlayer).bLatecomer )
        return;

    if ( aPlayer.PlayerReplicationInfo.bOutOfLives )
        return;

    if ( aPlayer.IsA('Bot') && TooManyBots(aPlayer) )
    {
        aPlayer.Destroy();
        return;
    } 
    Super.RestartPlayer(aPlayer);
}

function ForceAddBot()
{
    // add bot during gameplay
    if ( Level.NetMode != NM_Standalone )
        MinPlayers = Max(MinPlayers+1, NumPlayers + NumBots + 1);
    AddBot();
}

function bool AddBot(optional string botName)
{
    local Bot NewBot;

    NewBot = SpawnBot(botName);
    if ( NewBot == None )
    {
        warn("Failed to spawn bot.");
        return false;
    }
    // broadcast a welcome message.
    BroadcastLocalizedMessage(GameMessageClass, 1, NewBot.PlayerReplicationInfo);

    NewBot.PlayerReplicationInfo.PlayerID = CurrentID++;
    NumBots++;
    if ( Level.NetMode == NM_Standalone )
		RestartPlayer(NewBot);
	else
		NewBot.GotoState('Dead','MPStart');
        
    return true;
}

function AddDefaultInventory( pawn PlayerPawn )
{
    if ( UnrealPawn(PlayerPawn) != None )
        UnrealPawn(PlayerPawn).AddDefaultInventory();
    SetPlayerDefaults(PlayerPawn);
}

function bool CanSpectate( PlayerController Viewer, bool bOnlySpectator, actor ViewTarget )
{
    if ( (ViewTarget == None) || ViewTarget.IsA('Controller') )
        return false;
    return ( (Level.NetMode == NM_Standalone) || bOnlySpectator );
}

function bool ShouldRespawn(Pickup Other)
{
    return ( Other.ReSpawnTime!=0.0 );
}

function ChangeName(Controller Other, string S, bool bNameChange)
{
    local Controller APlayer,C;

    if ( S == "" )
        return;

    if (Other.PlayerReplicationInfo.playername~=S)
        return;

	S = Left(S,16);
	
	if ( bEpicNames && (Bot(Other) != None) )
	{
		if ( TotalEpic < 21 )
		{
			S = EpicNames[EpicOffset % 21];
			EpicOffset++;
			TotalEpic++;
		}
		else
		{
			S = NamePrefixes[NameNumber%10]$"CliffyB"$NameSuffixes[NameNumber%10]; 
			NameNumber++;
		}
	}
				 	
    for( APlayer=Level.ControllerList; APlayer!=None; APlayer=APlayer.nextController )
        if ( APlayer.bIsPlayer && (APlayer.PlayerReplicationInfo.playername~=S) )
        {
            if ( Other.IsA('PlayerController') )
            {
                PlayerController(Other).ReceiveLocalizedMessage( GameMessageClass, 8 );
				return;
			}
            else
            {
                S = NamePrefixes[NameNumber%10]$S$NameSuffixes[NameNumber%10];
                NameNumber++;
                break;
            }
        }
    
	if( bNameChange && GameStats!=None )
		GameStats.GameEvent("NameChange",s,Other.PlayerReplicationInfo);		

	if ( S ~= "CliffyB" )
		bEpicNames = true;
    Other.PlayerReplicationInfo.SetPlayerName(S);
    // notify local players
    if  ( bNameChange )
		for ( C=Level.ControllerList; C!=None; C=C.NextController )
			if ( (PlayerController(C) != None) && (Viewport(PlayerController(C).Player) != None) )
				PlayerController(C).ReceiveLocalizedMessage( class'GameMessage', 2, Other.PlayerReplicationInfo );          
}

function Logout(controller Exiting)
{
    Super.Logout(Exiting);
    if ( Exiting.IsA('Bot') )
        NumBots--;
    if ( !bKillBots )
		RemainingBots++;
    if ( !NeedPlayers() || AddBot() )
        RemainingBots--;
}

function bool NeedPlayers()
{
    if ( Level.NetMode == NM_Standalone )
        return ( RemainingBots > 0 );
    if ( bMustJoinBeforeStart || (GameStats != None) )
        return false;
    return (NumPlayers + NumBots < MinPlayers);
}

//------------------------------------------------------------------------------
// Game Querying.

function GetServerDetails( out ServerResponseLine ServerState )
{
	local int i;

	Super.GetServerDetails( ServerState );

	i = ServerState.ServerInfo.Length;

	// goalscore
	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "goalscore";
	ServerState.ServerInfo[i++].Value = string(GoalScore);
	
	// timelimit
	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "timelimit";
	ServerState.ServerInfo[i++].Value = string(TimeLimit);

	// minplayers
	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "minplayers";
	ServerState.ServerInfo[i++].Value = string(MinPlayers);

	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "translocator";
	if( bAllowTrans )
		ServerState.ServerInfo[i++].Value = "true";
	else
		ServerState.ServerInfo[i++].Value = "false";
}

function InitGameReplicationInfo()
{
    Super.InitGameReplicationInfo();
    GameReplicationInfo.GoalScore = GoalScore;
    GameReplicationInfo.TimeLimit = TimeLimit;
}
 
function InitTeamSymbols()
{
    // default team textures (for banners, etc.)
    if ( GameReplicationInfo.TeamSymbols[0] == None )
		GameReplicationInfo.TeamSymbols[0] = GetRandomTeamSymbol(1);
    if ( GameReplicationInfo.TeamSymbols[1] == None )
		GameReplicationInfo.TeamSymbols[1] = GetRandomTeamSymbol(10);
	GameReplicationInfo.TeamSymbolNotify();
}

//------------------------------------------------------------------------------

function UnrealTeamInfo GetBotTeam(optional int TeamBots)
{
	local class<UnrealTeamInfo> RosterClass;
	
	if ( EnemyRoster != None )
		return EnemyRoster;
    if ( CurrentGameProfile != None )
	{
		RosterClass = class<UnrealTeamInfo>(DynamicLoadObject(CurrentGameProfile.EnemyTeam,class'Class'));
		if ( RosterClass == None)
			warn("NO ENEMY ROSTER FOR THIS SINGLE PLAYER MATCH WITH ENEMY TEAM "$CurrentGameProfile.EnemyTeam);
		else
			EnemyRoster = spawn(RosterClass);
	}
	else if ( EnemyRosterName != "" )
	{
		RosterClass = class<UnrealTeamInfo>(DynamicLoadObject(EnemyRosterName,class'Class'));
		if ( RosterClass != None)
			EnemyRoster = spawn(RosterClass);
	}
    if ( EnemyRoster == None )
    {
		RosterClass = class<UnrealTeamInfo>(DynamicLoadObject(DefaultEnemyRosterClass,class'Class'));
		if ( RosterClass != None)
			EnemyRoster = spawn(RosterClass);
	}
	EnemyRoster.Initialize(TeamBots);
	return EnemyRoster;
}

function PreLoadBot()
{
	EnemyRoster.AddRandomPlayer();
}

/* Spawn and initialize a bot
*/
function Bot SpawnBot(optional string botName)
{
    local Bot NewBot;
    local RosterEntry Chosen;
	local UnrealTeamInfo BotTeam;
	
	BotTeam = GetBotTeam();
    Chosen = BotTeam.ChooseBotClass(botName);

    if (Chosen.PawnClass == None)
        Chosen.Init(); //amb
    // log("Chose pawn class "$Chosen.PawnClass);
    NewBot = Bot(Spawn(Chosen.PawnClass.default.ControllerClass));

    if ( NewBot != None )
        InitializeBot(NewBot,BotTeam,Chosen);
    return NewBot;
}
    
/* Initialize bot
*/
function InitializeBot(Bot NewBot, UnrealTeamInfo BotTeam, RosterEntry Chosen)
{
    NewBot.InitializeSkill(AdjustedDifficulty);
 	Chosen.InitBot(NewBot);
    BotTeam.AddToTeam(NewBot);
	ChangeName(NewBot, Chosen.PlayerName, false);
	if ( bEpicNames && (NewBot.PlayerReplicationInfo.PlayerName ~= "The_Reaper") )
	{
		NewBot.Accuracy = 1;
		NewBot.StrafingAbility = 1;
		NewBot.Tactics = 1;
		NewBot.InitializeSkill(AdjustedDifficulty+2);
	}
	BotTeam.SetBotOrders(NewBot,Chosen);
}

/* initialize a bot which is associated with a pawn placed in the level
*/
function InitPlacedBot(Controller C, RosterEntry R)
{
    local UnrealTeamInfo BotTeam;

	log("Init placed bot "$C);
    
    BotTeam = FindTeamFor(C);
    if ( Bot(C) != None )
    {
		Bot(C).InitializeSkill(AdjustedDifficulty);
		if ( R != None )
			R.InitBot(Bot(C));
	}
	BotTeam.AddToTeam(C);
	if ( R != None )
		ChangeName(C, R.PlayerName, false);
}

function UnrealTeamInfo FindTeamFor(Controller C)
{
    return GetBotTeam();
}
//------------------------------------------------------------------------------
// Game States

function StartMatch()
{
    local bool bTemp;
	local int Num;
	
    GotoState('MatchInProgress');
    if ( Level.NetMode == NM_Standalone )
        RemainingBots = InitialBots;
    else
        RemainingBots = 0;
    GameReplicationInfo.RemainingMinute = RemainingTime;
    Super.StartMatch();
    bTemp = bMustJoinBeforeStart;
    bMustJoinBeforeStart = false;
    while ( NeedPlayers() && (Num<16) )
    {
		if ( AddBot() )
			RemainingBots--;
		Num++;
    }
    bMustJoinBeforeStart = bTemp;
    log("START MATCH");
}

function EndGame(PlayerReplicationInfo Winner, string Reason )
{
    if ( (Reason ~= "triggered") || 
         (Reason ~= "LastMan")   || 
         (Reason ~= "TimeLimit") || 
         (Reason ~= "FragLimit") || 
         (Reason ~= "TeamScoreLimit") ) 
    {       
        Super.EndGame(Winner,Reason);
        if ( bGameEnded )
            GotoState('MatchOver');
    }
}

/* FindPlayerStart()
returns the 'best' player start for this player to start from.
*/
function NavigationPoint FindPlayerStart(Controller Player, optional byte InTeam, optional string incomingName)
{
    local NavigationPoint Best;

    if ( (Player != None) && (Player.StartSpot != None) )
        LastPlayerStartSpot = Player.StartSpot;

    Best = Super.FindPlayerStart(Player, InTeam, incomingName );
    if ( Best != None )
        LastStartSpot = Best;
    return Best;
}

function PlayEndOfMatchMessage()
{
	local controller C;
	
    if ( (PlayerReplicationInfo(GameReplicationInfo.Winner).Deaths == 0)
		&& (PlayerReplicationInfo(GameReplicationInfo.Winner).Score >= 5) )
    {
		for ( C = Level.ControllerList; C != None; C = C.NextController )
		{
			if ( C.IsA('PlayerController') )
			{
				if ( (C.PlayerReplicationInfo == GameReplicationInfo.Winner) || C.PlayerReplicationInfo.bOnlySpectator )
					PlayerController(C).PlayAnnouncement(AltEndGameSound[0],1,true);
				else
					PlayerController(C).PlayAnnouncement(AltEndGameSound[1],1,true);
			}
		}
	}        
    else
    {       
		for ( C = Level.ControllerList; C != None; C = C.NextController )
		{
			if ( C.IsA('PlayerController') && !C.PlayerReplicationInfo.bOnlySpectator )
			{
				if (C.PlayerReplicationInfo == GameReplicationInfo.Winner)
					PlayerController(C).PlayAnnouncement(EndGameSound[0],1,true);
				else
					PlayerController(C).PlayAnnouncement(EndGameSound[1],1,true);
			}
		}
	}
}

function PlayStartupMessage()
{
	local Controller P;

    // keep message displayed for waiting players
    for (P=Level.ControllerList; P!=None; P=P.NextController )
        if ( UnrealPlayer(P) != None )
            UnrealPlayer(P).PlayStartUpMessage(StartupStage);
}

auto State PendingMatch
{
	function RestartPlayer( Controller aPlayer ) 
	{
		if ( CountDown <= 0 )
			Super.RestartPlayer(aPlayer);
	}
 
    function bool AddBot(optional string botName)
    {
        if ( Level.NetMode == NM_Standalone )
            InitialBots++;
        PreLoadBot();
        return true;
    }
    
    function Timer()
    {
        local Controller P;
        local bool bReady;

        Global.Timer();

        // first check if there are enough net players, and enough time has elapsed to give people
        // a chance to join
        if ( NumPlayers == 0 )
			bWaitForNetPlayers = true;
        if ( bWaitForNetPlayers && (Level.NetMode != NM_Standalone) )
        {
             if ( NumPlayers > 0 )
                ElapsedTime++;
            else
                ElapsedTime = 0;
            if ( (NumPlayers == MaxPlayers) 
                || ((ElapsedTime > NetWait) && (NumPlayers >= MinNetPlayers)) )
            {
                bWaitForNetPlayers = false;
                CountDown = Default.CountDown;
            }
        }

        if ( (Level.NetMode != NM_Standalone) && (bWaitForNetPlayers || (bTournament && (NumPlayers < MaxPlayers))) )
        {
       		PlayStartupMessage();
            return;
		}
		
		// check if players are ready
        bReady = true;
        StartupStage = 1;
        if ( !bStartedCountDown && (bTournament || bPlayersMustBeReady || (Level.NetMode == NM_Standalone)) )
        {
            for (P=Level.ControllerList; P!=None; P=P.NextController )
                if ( P.IsA('PlayerController') && (P.PlayerReplicationInfo != None)
                    && P.bIsPlayer && P.PlayerReplicationInfo.bWaitingPlayer
                    && !P.PlayerReplicationInfo.bReadyToPlay )
                    bReady = false;
        }
        if ( bReady )
        {
			bStartedCountDown = true;
            CountDown--;
            if ( CountDown <= 0 )
                StartMatch();
            else
                StartupStage = 5 - CountDown;
        }
		PlayStartupMessage();
    }

    function beginstate()
    {
		bWaitingToStartMatch = true;
        StartupStage = 0;   
    }
    
Begin:
	if ( bQuickStart )
		StartMatch();
}

State MatchInProgress
{
    function Timer()
    {
        local Controller P;

        //log("*** MatchInProgress Timer! ***");

        Global.Timer();
		if ( !bFinalStartup )
		{
			bFinalStartup = true;
			PlayStartupMessage();
		}
        if ( bForceRespawn )
            For ( P=Level.ControllerList; P!=None; P=P.NextController )
            {
                if ( (P.Pawn == None) && P.IsA('PlayerController') && !P.PlayerReplicationInfo.bOnlySpectator )
                    PlayerController(P).ServerReStartPlayer();
            }
        if ( NeedPlayers() )
            AddBot();
        if ( !bOverTime && (TimeLimit > 0) )
        {
            //log("RemainingTime:"$RemainingTime$" TimeLimit: "$TimeLimit); // sjs
            GameReplicationInfo.bStopCountDown = false;
            RemainingTime--;
            GameReplicationInfo.RemainingTime = RemainingTime;
            if ( RemainingTime % 60 == 0 )
                GameReplicationInfo.RemainingMinute = RemainingTime;
            if ( RemainingTime <= 0 )
                EndGame(None,"TimeLimit");
        }
        ElapsedTime++;
        GameReplicationInfo.ElapsedTime = ElapsedTime;
    }

    function beginstate()
    {
		local PlayerReplicationInfo PRI;
		
		ForEach DynamicActors(class'PlayerReplicationInfo',PRI)
			PRI.StartTime = 0;
		ElapsedTime = 0;
		bWaitingToStartMatch = false;
        StartupStage = 5;   
        PlayStartupMessage();
        StartupStage = 6;   
    }
}

State MatchOver
{
	function RestartPlayer( Controller aPlayer ) {} 
	function ScoreKill(Controller Killer, Controller Other) {}
	function int ReduceDamage( int Damage, pawn injured, pawn instigatedBy, vector HitLocation, vector Momentum, class<DamageType> DamageType )
	{
		return 0;
	}
		   
	function bool ChangeTeam(Controller Other, int num, bool bNewTeam) 
	{
		return false;
	}

    function Timer()
    {
		local Controller C;

        Global.Timer();

        if ( Level.TimeSeconds > EndTime + RestartWait )
            RestartGame();

		if ( EndGameFocus != None )
		{
			EndGameFocus.bAlwaysRelevant = true;
			for ( C = Level.ControllerList; C != None; C = C.NextController )
				if ( PlayerController(C) != None )
					PlayerController(C).ClientSetViewtarget(EndGameFocus);
		}
		
         // play end-of-match message for winner/losers (for single and muli-player)
        EndMessageCounter++;
        if ( EndMessageCounter == EndMessageWait )
	         PlayEndOfMatchMessage();
	}

    
    function bool NeedPlayers()
    {
        return false;
    }
    
    function BeginState()
    {
		local Controller C;
		local PlayerController P;
		
		GameReplicationInfo.bStopCountDown = true;
        if ( CurrentGameProfile != None )
        {
			EndTime = Level.TimeSeconds + SinglePlayerWait;
			for ( c=Level.ControllerList; c!=None; c=c.NextController )
			{
				P = PlayerController(C);
				if ( P != None )
					break;
			}
			P.myHUD.bShowScoreboard=true;	
			CurrentGameProfile.RegisterGame(self,P.PlayerReplicationInfo);
            SavePackage(CurrentGameProfile.PackageName);
        }
	}
}

/* Rate whether player should choose this NavigationPoint as its start
*/
function float RatePlayerStart(NavigationPoint N, byte Team, Controller Player)
{
    local PlayerStart P;
    local float Score, NextDist;
    local Controller OtherPlayer;

    P = PlayerStart(N);

    if ( (P == None) || !P.bEnabled || P.PhysicsVolume.bWaterVolume )
        return -10000000;

    //assess candidate
    if ( P.bPrimaryStart )
		Score = 10000000;
	else
		Score = 5000000;
    if ( (N == LastStartSpot) || (N == LastPlayerStartSpot) )
        Score -= 10000.0;
    else
        Score += 3000 * FRand(); //randomize

	if ( Level.TimeSeconds - P.LastSpawnCampTime < 30 )
		Score = Score - (30 - P.LastSpawnCampTime + Level.TimeSeconds) * 1000;
	
    for ( OtherPlayer=Level.ControllerList; OtherPlayer!=None; OtherPlayer=OtherPlayer.NextController)  
        if ( OtherPlayer.bIsPlayer && (OtherPlayer.Pawn != None) )
        {
            if ( OtherPlayer.Pawn.Region.Zone == N.Region.Zone )
                Score -= 1500;
            NextDist = VSize(OtherPlayer.Pawn.Location - N.Location);
            if ( NextDist < OtherPlayer.Pawn.CollisionRadius + OtherPlayer.Pawn.CollisionHeight )
                Score -= 1000000.0;
            else if ( (NextDist < 3000) && FastTrace(N.Location, OtherPlayer.Pawn.Location) )
                Score -= (10000.0 - NextDist);
            else if ( NumPlayers + NumBots == 2 )
            {
                Score += 2 * VSize(OtherPlayer.Pawn.Location - N.Location);
                if ( FastTrace(N.Location, OtherPlayer.Pawn.Location) )
                    Score -= 10000;
            }
        }
    return FMax(Score, 5);
}

// check if all other players are out
function bool CheckMaxLives(PlayerReplicationInfo Scorer)
{
    local Controller C;
    local PlayerReplicationInfo Living;
    local bool bNoneLeft;

    if ( MaxLives > 0 )
    {
		if ( (Scorer != None) && !Scorer.bOutOfLives )
			Living = Scorer;
        bNoneLeft = true;
        for ( C=Level.ControllerList; C!=None; C=C.NextController )
            if ( (C.PlayerReplicationInfo != None) && C.bIsPlayer
                && !C.PlayerReplicationInfo.bOutOfLives )
            {
				if ( Living == None )
					Living = C.PlayerReplicationInfo;
				else if (C.PlayerReplicationInfo != Living) 
			   	{
    	        	bNoneLeft = false;
	            	break;
				}
            } 
        if ( bNoneLeft )
        {
			if ( Living != None )
				EndGame(Living,"LastMan");
			else
				EndGame(Scorer,"LastMan");
			return true;
		}
    }   
    return false;
}

/* CheckScore()
see if this score means the game ends
*/
function CheckScore(PlayerReplicationInfo Scorer)
{
	if ( CheckMaxLives(Scorer) )
		return;

    if ( (GameRulesModifiers != None) && GameRulesModifiers.CheckScore(Scorer) )
        return;

    if ( (Scorer != None) && (bOverTime || (GoalScore > 0)) && (Scorer.Score >= GoalScore) )
        EndGame(Scorer,"fraglimit");

}

function ScoreObjective(PlayerReplicationInfo Scorer, Int Score)
{
    if ( Scorer != None )
    {
        Scorer.Score += Score;
		
		if (GameStats!=None)
			GameStats.ScoreEvent(Scorer,Score,"ObjectiveScore");
    }

    if ( GameRulesModifiers != None )
        GameRulesModifiers.ScoreObjective(Scorer,Score);
    CheckScore(Scorer);
}

function ScoreKill(Controller Killer, Controller Other)
{
	local PlayerReplicationInfo OtherPRI;
	
	OtherPRI = Other.PlayerReplicationInfo;
    if ( OtherPRI != None )
    {
        OtherPRI.NumLives++;
        if ( (MaxLives > 0) && (OtherPRI.NumLives >=MaxLives) )
            OtherPRI.bOutOfLives = true;
    }

	if ( bAllowTaunts && (Killer != None) && (Killer != Other) && Killer.AutoTaunt() && (Killer.PlayerReplicationInfo.VoiceType != None) )
	{
		if( Killer.IsA('PlayerController') )
			Killer.SendMessage(OtherPRI, 'AUTOTAUNT', Killer.PlayerReplicationInfo.VoiceType.static.PickRandomTauntFor(Killer, false, false), 10, 'GLOBAL');
		else
			Killer.SendMessage(OtherPRI, 'AUTOTAUNT', Killer.PlayerReplicationInfo.VoiceType.static.PickRandomTauntFor(Killer, false, true), 10, 'GLOBAL');
	}
    Super.ScoreKill(Killer,Other);

    if ( (killer == None) || (Other == None) )
        return;

    if ( bAdjustSkill && (killer.IsA('PlayerController') || Other.IsA('PlayerController')) )
    {
        if ( killer.IsA('AIController') )
            AdjustSkill(AIController(killer), PlayerController(Other),true);
        if ( Other.IsA('AIController') )
            AdjustSkill(AIController(Other), PlayerController(Killer),false);
    }
}

function AdjustSkill(AIController B, PlayerController P, bool bWinner)
{
    local float BotSkill;

    BotSkill = B.Skill;

    if ( bWinner )
    {
        PlayerKills += 1;
        AdjustedDifficulty = FMax(0, AdjustedDifficulty - 2/Min(PlayerKills, 10));
        if ( BotSkill > AdjustedDifficulty )
            B.Skill = AdjustedDifficulty;
    }
    else
    {
        PlayerDeaths += 1;
        AdjustedDifficulty = FMin(7,AdjustedDifficulty + 2/Min(PlayerDeaths, 10));
        if ( BotSkill < AdjustedDifficulty )
            B.Skill = AdjustedDifficulty;
    }
    if ( abs(AdjustedDifficulty - GameDifficulty) >= 1 )
    {
        GameDifficulty = AdjustedDifficulty;
        SaveConfig();
    }
}

function int ReduceDamage( int Damage, pawn injured, pawn instigatedBy, vector HitLocation, vector Momentum, class<DamageType> DamageType )
{
    local float InstigatorSkill;

	if ( (instigatedBy != None) && (InstigatedBy != Injured) && (Level.TimeSeconds - injured.SpawnTime < SpawnProtectionTime) 
		&& (class<WeaponDamageType>(DamageType) != None) )
		return 0;
		
    Damage = Super.ReduceDamage( Damage, injured, InstigatedBy, HitLocation, Momentum, DamageType );

    if ( instigatedBy == None)
        return Damage;

    if ( Level.Game.GameDifficulty <= 3 )
    {
        if ( injured.IsPlayerPawn() && (injured == instigatedby) && (Level.NetMode == NM_Standalone) )
            Damage *= 0.5;

        //skill level modification
        if ( AIController(instigatedBy.Controller) != None )
        {
            InstigatorSkill = AIController(instigatedBy.Controller).Skill;
            if ( (InstigatorSkill <= 3) && injured.IsHumanControlled() )
			{
				if ( ((instigatedBy.Weapon != None) && instigatedBy.Weapon.bMeleeWeapon) 
					|| ((injured.Weapon != None) && injured.Weapon.bMeleeWeapon && (VSize(injured.location - instigatedBy.Location) < 600)) )
						Damage = Damage * (0.76 + 0.08 * InstigatorSkill);
				else
						Damage = Damage * (0.25 + 0.15 * InstigatorSkill);
            }
        }
    } 
    return (Damage * instigatedBy.DamageScaling);
}

// Add one or num bots
exec function AddNamedBot(string botname)
{
    if (Level.NetMode != NM_Standalone)
        MinPlayers = Max(MinPlayers + 1, NumPlayers + NumBots + 1);
    AddBot(botName);
}

exec function AddBots(int num)
{
	if ( GameStats != None )
		return;
    num = Clamp(num, 0, 32 - (NumPlayers + NumBots));

    while (--num >= 0)
    {
        if ( Level.NetMode != NM_Standalone )
            MinPlayers = Max(MinPlayers + 1, NumPlayers + NumBots + 1);
        AddBot();
    }
}

// Kill all or num bots
exec function KillBots(int num)
{
    local Controller c, nextC;

    if (num == 0)
        num = NumBots;

    c = Level.ControllerList;
    if ( Level.NetMode != NM_Standalone )
		MinPlayers = 0;
    bKillBots = true;
    while (c != None && num > 0)
    {
        nextC = c.NextController;
        if (KillBot(c))
            --num;
        c = nextC;
    }
    bKillBots = false;
}

function bool KillBot(Controller c)
{
    local Bot b;

    b = Bot(c);
    if (b != None)
    {
        if (Level.NetMode != NM_Standalone)
            MinPlayers = Max(MinPlayers - 1, NumPlayers + NumBots - 1);
        
        if (b.Pawn != None)
            b.Pawn.KilledBy( b.Pawn );
		if (b != None)
			b.Destroy();
        return true;
    }
    return false;
}

function ReviewJumpSpots(name TestLabel)
{
	local NavigationPoint StartSpot;
	local controller C;
	local Pawn P;
	local Bot B;
	local class<Pawn> PawnClass;
		
	B = spawn(class'Bot');
	B.Squad = spawn(class'DMSquad');
    startSpot = FindPlayerStart(B, 0);
    PawnClass = class<Pawn>( DynamicLoadObject(DefaultPlayerClassName, class'Class') );
    P = Spawn(PawnClass,,,StartSpot.Location,StartSpot.Rotation);
	if ( P == None )
	{
		log("Failed to spawn pawn to reviewjumpspots");
		return;
	}
	B.Possess(P);
	B.GoalString = "TRANSLOCATING";
	
	for ( C=Level.ControllerList; C!=None; C=C.NextController )
		if ( PlayerController(C) != None )
		{
			PlayerController(C).bBehindView = true;
			PlayerController(C).SetViewTarget(P);
			UnrealPlayer(C).ShowAI();
			break;
		}
			
	// first, check translocation	
    p.GiveWeapon("XWeapons.TransLauncher");
    if ( TestLabel == '' )
		TestLabel = 'Begin';
	else
		B.bSingleTestSection = true;
	B.GotoState('Testing',TestLabel);
}

static function FillPlayInfo(PlayInfo PlayInfo)
{
local int i;

	Super.FillPlayInfo(PlayInfo);  // Always begin with calling parent

	PlayInfo.AddSetting("Server", "NetWait",             default.DMPropsDisplayText[i++], 2,  40, "Text", "3;0:60");
	PlayInfo.AddSetting("Game",   "MinNetPlayers",       default.DMPropsDisplayText[i++], 1, 100, "Text", "3;0:32");
	PlayInfo.AddSetting("Server", "RestartWait",         default.DMPropsDisplayText[i++], 2,  42, "Text", "3;0:60");
	PlayInfo.AddSetting("Game",   "bTournament",         default.DMPropsDisplayText[i++], 1,  10, "Check");
	PlayInfo.AddSetting("Game",   "bPlayersMustBeReady", default.DMPropsDisplayText[i++], 1,  20, "Check");
	PlayInfo.AddSetting("Game",   "bForceRespawn",       default.DMPropsDisplayText[i++], 0,  40, "Check");
	PlayInfo.AddSetting("Bots",   "bAdjustSkill",        default.DMPropsDisplayText[i++], 0,  30, "Check");
	PlayInfo.AddSetting("Game",   "bAllowTaunts",        default.DMPropsDisplayText[i++], 0,  50, "Check");
	PlayInfo.AddSetting("Server", "SpawnProtectionTime", default.DMPropsDisplayText[i++], 2,  44, "Text", "8;0.0:30.0");
}

function NotifySpree(Controller Other, int num)
{
	local Controller C;

	if ( num == 5 )
		num = 0;
	else if ( num == 10 )
		num = 1;
	else if ( num == 15 )
		num = 2;
	else if ( num == 20 )
		num = 3;
	else if ( num == 25 )
		num = 4;
	else if ( num == 30 )
		num = 5;
	else
		return;

	if ( GameStats!=None && Other!=None )	//cal sprees
		GameStats.SpecialEvent(Other.PlayerReplicationInfo,"spree_"$(num+1));		

	Other.AwardAdrenaline( ADR_MajorKill );

	for ( C=Level.ControllerList; C!=None; C=C.NextController )
		if ( PlayerController(C) != None )
			PlayerController(C).ReceiveLocalizedMessage( class'KillingSpreeMessage', Num, Other.PlayerReplicationInfo );
}

function EndSpree(Controller Killer, Controller Other)
{
	local Controller C;

	if ( !Other.bIsPlayer )
		return;
	for ( C=Level.ControllerList; C!=None; C=C.NextController )
		if ( PlayerController(C) != None )
		{
			if ( (Killer == Other) || (Killer == None) || !Killer.bIsPlayer )
				PlayerController(C).ReceiveLocalizedMessage( class'KillingSpreeMessage', 1, None, Other.PlayerReplicationInfo );
			else
				PlayerController(C).ReceiveLocalizedMessage( class'KillingSpreeMessage', 0, Other.PlayerReplicationInfo, Killer.PlayerReplicationInfo );
		}
}

defaultproperties
{
    NumRounds=1
    DMSquadClass=class'UnrealGame.DMSquad'
    GoalScore=25
    bLoggingGame=true
	GameStatsClass=class'MasterServerGameStats'
    bAllowTaunts=true
    bTournament=false
    CountDown=4
    GameName="DeathMatch"
    InitialBots=0 
    bRestartLevel=False
    bPauseable=False
    bPlayersMustBeReady=false
    AutoAim=1.000000
    MapPrefix="DM"
    BeaconName="DM"
    MaxPlayers=32
    NetWait=2
    RestartWait=30    
    bDelayedStart=true
    MutatorClass="UnrealGame.DMMutator"
    MinNetPlayers=1
    bWaitForNetPlayers=true
    SpawnProtectionTime=+2.0
    DefaultPlayerClassName="XGame.xPawn"
    PlayerControllerClassName="XGame.XPlayer"
    EndMessageWait=2

    HUDType="XInterface.HudBDeathMatch"
    ScoreBoardType="XInterface.ScoreBoardDeathMatch"
    MapListType="XInterface.MapListDeathMatch"
     
    NamePrefixes(0)="Mr_"
    NamePrefixes(1)=""
    NamePrefixes(2)="The_Real_"
    NamePrefixes(3)="Evil_"
    NamePrefixes(4)=""
    NamePrefixes(5)="Owns_"
    NamePrefixes(6)=""
    NamePrefixes(7)="Evil_"
    
    NameSuffixes(0)=""
    NameSuffixes(1)="_is_lame"
    NameSuffixes(2)=""
    NameSuffixes(3)=""
    NameSuffixes(4)="_sucks"
    NameSuffixes(5)=""
    NameSuffixes(6)="_OwnsYou"
    NameSuffixes(7)=""
	NameSuffixes(8)="_jr"
	NameSuffixes(9)="'s_clone"
					
    DMPropsDisplayText(0)="Net Start Delay"
    DMPropsDisplayText(1)="Min. Net Players"
    DMPropsDisplayText(2)="Restart Delay"
    DMPropsDisplayText(3)="Tournament Game"
    DMPropsDisplayText(4)="Players Must Be Ready"
    DMPropsDisplayText(5)="Force Respawn"
    DMPropsDisplayText(6)="Adjust Bots Skill"
    DMPropsDisplayText(7)="Allow Taunts"
    DMPropsDisplayText(8)="Spawn Protection Time"

    EndGameSound[0]=Sound'AnnouncerMain.You_Have_Won_the_Match'
    EndGameSound[1]=Sound'AnnouncerMain.You_Have_Lost_the_Match'
    AltEndGameSound[0]=Sound'AnnouncerMain.Flawless_victory'
    AltEndGameSound[1]=Sound'AnnouncerMain.Humiliating_defeat'
    
    ADR_Kill=5.0
    ADR_MajorKill=10.0
    ADR_MinorError=-2.0
    ADR_MinorBonus=5.0
    ADR_KillTeamMate=-5.0
    
    EpicNames[0]="Eepers"
    EpicNames[1]="Bellheimer"
    EpicNames[2]="Shanesta"
    EpicNames[3]="EpicBoy"
    EpicNames[4]="Turtle"
    EpicNames[5]="Talisman"
    EpicNames[6]="BigSquid"
    EpicNames[7]="Ced"
    EpicNames[8]="Andrew"
    EpicNames[9]="DrSin"
    EpicNames[10]="The_Reaper"
    EpicNames[11]="ProfessorDeath"
    EpicNames[12]="DarkByte"
    EpicNames[13]="Jack"
    EpicNames[14]="Lankii"
    EpicNames[15]="MarkRein"
    EpicNames[16]="Perninator"
    EpicNames[17]="SteveG"
    EpicNames[18]="Cpt.Pinhead"
    EpicNames[19]="Christoph"
    EpicNames[20]="Tim"
}

   
