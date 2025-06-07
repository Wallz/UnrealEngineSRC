//=============================================================================
// GameReplicationInfo.
//=============================================================================
class GameReplicationInfo extends ReplicationInfo
	native nativereplication exportstructs;

var string GameName;						// Assigned by GameInfo.
var string GameClass;						// Assigned by GameInfo.
var bool bTeamGame;							// Assigned by GameInfo.
var bool bStopCountDown;
var bool bMatchHasBegun;
var bool bTeamSymbolsUpdated;
var int  RemainingTime, ElapsedTime, RemainingMinute;
var float SecondCount;
var int GoalScore;
var int TimeLimit;
var int MaxLives;

var TeamInfo Teams[2];

var() globalconfig string ServerName;		// Name of the server, i.e.: Bob's Server.
var() globalconfig string ShortName;        // Abbreviated name of server, i.e.: B's Serv (stupid example)
var() globalconfig string AdminName;		// Name of the server admin.
var() globalconfig string AdminEmail;       // Email address of the server admin.
var() globalconfig int	  ServerRegion;		// Region of the game server.

var() globalconfig string MOTDLine1;		// Message
var() globalconfig string MOTDLine2;	    // Of
var() globalconfig string MOTDLine3;	    // The
var() globalconfig string MOTDLine4;	    // Day

var Actor Winner;			// set by gameinfo when game ends

var() texture TeamSymbols[2];
var() array<PlayerReplicationInfo> PRIArray;

// mc - localized PlayInfo descriptions & extra info
var private localized string GRIPropsDisplayText[8];

var vector FlagPos;	// replicated 2D position of one object
var EFlagState FlagState[2];

// stats
var int MatchID;

replication
{
	reliable if ( bNetDirty && (Role == ROLE_Authority) )
		RemainingMinute, bStopCountDown, Winner, Teams, FlagPos, FlagState, bMatchHasBegun, MatchID; 

	reliable if ( bNetInitial && (Role==ROLE_Authority) )
		GameName, GameClass, bTeamGame, 
		RemainingTime, ElapsedTime,MOTDLine1, MOTDLine2, 
		MOTDLine3, MOTDLine4, ServerName, ShortName, AdminName,
		AdminEmail, ServerRegion, GoalScore, MaxLives, TimeLimit, TeamSymbols; 
}

simulated function PostNetBeginPlay()
{
	local PlayerReplicationInfo PRI;
	
	ForEach DynamicActors(class'PlayerReplicationInfo',PRI)
		AddPRI(PRI);
	if ( Level.NetMode == NM_Client )
		TeamSymbolNotify();
}

simulated function TeamSymbolNotify()
{
	local Actor A;

	if ( TeamSymbols[0] == None )
		return;
	bTeamSymbolsUpdated = true;
	ForEach AllActors(class'Actor', A)
		A.SetGRI(self);
}

simulated function UpdatePrecacheMaterials()
{
	Level.AddPrecacheMaterial(TeamSymbols[0]);
	Level.AddPrecacheMaterial(TeamSymbols[1]);
}

simulated function PostBeginPlay()
{
	if( Level.NetMode == NM_Client )
	{
		// clear variables so we don't display our own values if the server has them left blank 
		ServerName = "";
		AdminName = "";
		AdminEmail = "";
		MOTDLine1 = "";
		MOTDLine2 = "";
		MOTDLine3 = "";
		MOTDLine4 = "";
	}

	SecondCount = Level.TimeSeconds;
	SetTimer(1, true);
}

/* Reset() 
reset actor to initial state - used when restarting level without reloading.
*/
function Reset()
{
	Super.Reset();
	Winner = None;
}

simulated function Timer()
{
	if ( Level.NetMode == NM_Client )
	{
		if (Level.TimeSeconds - SecondCount >= Level.TimeDilation)
		{
			ElapsedTime++;
			if ( RemainingMinute != 0 )
			{
				RemainingTime = RemainingMinute;
				RemainingMinute = 0;
			}
			if ( (RemainingTime > 0) && !bStopCountDown )
				RemainingTime--;
			SecondCount += Level.TimeDilation;
		}
		if ( !bTeamSymbolsUpdated )
			TeamSymbolNotify();
	}
}

simulated function AddPRI(PlayerReplicationInfo PRI)
{
    PRIArray[PRIArray.Length] = PRI;
}

simulated function RemovePRI(PlayerReplicationInfo PRI)
{
    local int i;

    for (i=0; i<PRIArray.Length; i++)
    {
        if (PRIArray[i] == PRI)
            break;
    }

    if (i == PRIArray.Length)
    {
        log("GameReplicationInfo::RemovePRI() pri="$PRI$" not found.", 'Error');
        return;
    }

    PRIArray.Remove(i,1);
}

simulated function GetPRIArray(out array<PlayerReplicationInfo> pris)
{
    local int i;
    local int num;

    pris.Remove(0, pris.Length);
    for (i=0; i<PRIArray.Length; i++)
    {
        if (PRIArray[i] != None)
            pris[num++] = PRIArray[i];
    }
}

static function FillPlayInfo(PlayInfo PlayInfo)
{
	local int i;

	Super.FillPlayInfo(PlayInfo);  // Always begin with calling parent

	PlayInfo.AddSetting("Server",  "ServerName", default.GRIPropsDisplayText[i++], 255, 100, "Text", "40");
	PlayInfo.AddSetting("Server",  "ShortName",  default.GRIPropsDisplayText[i++], 255, 101, "Text", "20");
	PlayInfo.AddSetting("Server",  "AdminName",  default.GRIPropsDisplayText[i++], 255, 102, "Text", "20");
	PlayInfo.AddSetting("Server",  "AdminEmail", default.GRIPropsDisplayText[i++], 255, 103, "Text", "20");
	PlayInfo.AddSetting("Server",  "MOTDLine1",  default.GRIPropsDisplayText[i++], 254, 200, "Text", "40");
	PlayInfo.AddSetting("Server",  "MOTDLine2",  default.GRIPropsDisplayText[i++], 254, 201, "Text", "40");
	PlayInfo.AddSetting("Server",  "MOTDLine3",  default.GRIPropsDisplayText[i++], 254, 202, "Text", "40");
	PlayInfo.AddSetting("Server",  "MOTDLine4",  default.GRIPropsDisplayText[i++], 254, 203, "Text", "40");
}

defaultproperties
{
	FlagState[0]=FLAG_Home
	FlagState[1]=FLAG_Home
	bStopCountDown=true
	RemoteRole=ROLE_SimulatedProxy
	bAlwaysRelevant=True
	ServerName="Another Server"
	ShortName="Server"
	MOTDLine1=""
	MOTDLine2=""
	MOTDLine3=""
	MOTDLine4=""
    GRIPropsDisplayText(0)="Server Name"
    GRIPropsDisplayText(1)="Short Server Name"
    GRIPropsDisplayText(2)="Admin Name"
    GRIPropsDisplayText(3)="Admin E-Mail"
    GRIPropsDisplayText(4)="Message"
    GRIPropsDisplayText(5)="of"
    GRIPropsDisplayText(6)="the"
    GRIPropsDisplayText(7)="day"
    bNetNotify=true
}
