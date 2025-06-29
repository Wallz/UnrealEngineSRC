class MasterServerUplink extends MasterServerLink
    config
	native;

cpptext
{
	// AActor interface
	void Destroy();
	void PostScriptDestroyed();
	// AServerQueryLink interface
	UBOOL Poll( INT WaitTime );
}

enum EServerToMaster
{
	STM_ClientResponse,
	STM_GameState,
	STM_Stats,
	STM_ClientDisconnectFailed,
};

enum EMasterToServer
{
	MTS_ClientChallenge,
	MTS_ClientAuthFailed,
	MTS_Shutdown,
	MTS_MatchID,
};

enum EHeartbeatType
{
	HB_QueryInterface,
	HB_GamePort,
	HB_GamespyQueryPort,
};

var GameInfo.ServerResponseLine ServerState;
var MasterServerGameStats GameStats;
var UdpLink	GamespyQueryLink;
var const int MatchID;
var float ReconnectTime;
var bool bReconnectPending;

// config
var globalconfig bool DoUplink;
var globalconfig bool UplinkToGamespy;
var globalconfig bool SendStats;
var globalconfig bool ServerBehindNAT;
var globalconfig bool DoLANBroadcast;

// sorry, no code for you!
native function Reconnect();

event BeginPlay()
{
	local UdpGamespyQuery  GamespyQuery;
	local UdpGamespyUplink GamespyUplink;

	if( DoUplink )
	{
		// if we're uplinking to gamespy, also spawn the gamespy actors.
		if( UplinkToGamespy )
		{
			GamespyQuery  = Spawn( class'UdpGamespyQuery' );
			GamespyUplink = Spawn( class'UdpGamespyUplink' );		

			// FMasterServerUplink needs this for NAT.
			GamespyQueryLink = GamespyQuery;
		}

		// If we're sending stats, 
		if( SendStats )
		{
			foreach AllActors(class'MasterServerGameStats', GameStats )
			{
				if( GameStats.Uplink == None )
					GameStats.Uplink = Self;
				else
					GameStats = None;
				break;
			}		
			if( GameStats == None )
				Log("MasterServerUplink: MasterServerGameStats not found - stats uploading disabled.");
		}
	}

	Reconnect();
}

// Called when the connection to the master server fails or doesn't connect.
event ConnectionFailed( bool bShouldReconnect )
{
	Log("Master server connection failed");
	bReconnectPending = bShouldReconnect;
	ReconnectTime = 0;
}

// Called when we should refresh the game state
event Refresh()
{
	Level.Game.GetServerInfo(ServerState);
	Level.Game.GetServerDetails(ServerState);
	Level.Game.GetServerPlayers(ServerState);
}

// Call to log a stat line
native event bool LogStatLine( string StatLine );

// Handle disconnection.
simulated function Tick( float Delta )
{
	Super.Tick(Delta);
	ReconnectTime = ReconnectTime + Delta;
	if( bReconnectPending )
	{
		if( ReconnectTime > 10.0 )
		{
			Log("Attempting to reconnect to master server");
			bReconnectPending = False;
			Reconnect();
		}
	}
}

defaultproperties
{
	DoUplink=True
	UplinkToGamespy=True
	SendStats=True
	MatchID=0
}