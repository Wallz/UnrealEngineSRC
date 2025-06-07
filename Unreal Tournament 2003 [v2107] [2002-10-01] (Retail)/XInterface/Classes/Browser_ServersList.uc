class Browser_ServersList extends GUIMultiColumnList;

var() array<GameInfo.ServerResponseLine> Servers;

var Browser_ServerListPageBase MyPage;
var Browser_RulesList	MyRulesList;
var Browser_PlayersList	MyPlayersList;
var GUIStyles SelStyle;
var array<int> OutstandingPings;
var globalconfig int MaxSimultaneousPings;
var int UseSimultaneousPings;
var int PingStart;
var int NumReceivedPings;
var int NumPlayers;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	local int i;

	// let rules/players know about us
	MyPlayersList.MyServersList = Self;
	MyRulesList.MyServersList = Self;

	// set delegates
	OnDrawItem	= MyOnDrawItem;
	OnChange	= MyOnChange;
	OnDblClick  = MyOnDblClick;

	Super.Initcomponent(MyController, MyOwner);
	SelStyle = Controller.GetStyle("SquareButton");

	if( MaxSimultaneousPings == 0 )
	{
		i = class'Player'.default.ConfiguredInternetSpeed;
		if (i<=2600)
			UseSimultaneousPings = 10;
		else if (i<=5000)
			UseSimultaneousPings = 15;
		else if (i<=10000)
			UseSimultaneousPings = 20;
		else
			UseSimultaneousPings = 35;
	}
	else
		UseSimultaneousPings = MaxSimultaneousPings;
}

function Clear()
{
	PingStart = 0;
	StopPings();
	Servers.Remove(0,Servers.Length);
	ItemCount = 0;
	Super.Clear();
}

function Connect(bool Spectator)
{
	local string URL;
	if( Index >= 0 )
	{
		//!!
		URL = "unreal://"$Servers[SortData[Index].SortItem].IP$":"$string(Servers[SortData[Index].SortItem].Port);
		if( Spectator )
			URL = URL $ "?SpectatorOnly=True";
		if( MyPage.ConnectLAN )
			URL = URL $ "?LAN";

		Controller.CloseAll(false);
		PlayerOwner().ClientTravel( URL, TRAVEL_Absolute, false );    
	}
}

function AddFavorite( ServerBrowser Browser )
{
	if( Index >= 0 )
		Browser.OnAddFavorite( Servers[SortData[Index].SortItem] );
}

function bool MyOnDblClick(GUIComponent Sender)
{
    Connect(false);
	return true;
}

function bool InternalOnKeyEvent(out byte Key, out byte State, float delta)
{
	if( Super.InternalOnKeyEvent(Key, State, delta) )
		return true;

	if( State==1 )
	{
		switch(Key)
		{
		case 0x0D: //IK_Enter
			Connect(false);
			return true;
			break;
		case 0x74: //IK_F5
			MyPage.RefreshList();
			return true;
			break;
		}	
	}
	return false;
}	

function MyOnReceivedServer( GameInfo.ServerResponseLine s )
{
	local int i;

	i = Servers.Length;
	Servers.Length=i+1;
	Servers[i] = s;
	if( Servers[i].Ping == 0 )
		Servers[i].Ping = 9999;
	ItemCount++;
	AddedItem();
}

function MyPingTimeout( int listid, ServerQueryClient.EPingCause PingCause  )
{
	local int i;

	if(listid >= Servers.Length)
		return;

	if( Servers[listid].Ping == 9999 )
	{
		Servers[listid].Ping = 10000;
	    UpdatedItem(listid);
	}

	// remove from the outstanding ping list
	for( i=0;i<OutstandingPings.Length;i++ )
		if( OutstandingPings[i] == listid )
		{
			OutstandingPings.Remove(i,1);
            break;
		}

	// continue pinging
	if( PingCause == PC_AutoPing )
	{
		NumReceivedPings++;
		NeedsSorting = True;
		AutoPingServers();
	}
}

function MyReceivedPingInfo( int listid, ServerQueryClient.EPingCause PingCause, GameInfo.ServerResponseLine s )
{
	local int i;

	if( listid < 0 )
	{
		PingStart = 0;
		return;
	}

    Servers[listid] = s;

	// see if we can move the ping start marker down
	for( i=PingStart;i<listid && i<Servers.Length;i++ )
        if( Servers[i].Ping == 9999 )
			break;
	if( i < listid )
		PingStart = listid;

	// remove from the outstanding ping list
	for( i=0;i<OutstandingPings.Length;i++ )
		if( OutstandingPings[i] == listid )
		{
			OutstandingPings.Remove(i,1);
            break;
		}

    UpdatedItem(listid);

	// update rules/player info
	if( Index>=0 && listid == SortData[Index].SortItem )
		MyOnChange(None);

	// continue pinging
	if( PingCause == PC_AutoPing )
	{
		NumReceivedPings++;
		NumPlayers += s.CurrentPlayers;
		NeedsSorting = True;
		AutoPingServers();
	}
}

function RePingServers()
{
	InvalidatePings();
	AutoPingServers();
}

function MyQueryFinished( MasterServerClient.EResponseInfo ResponseInfo, int Info )
{
	if( ResponseInfo == RI_Success )
	{
		RePingServers();
	}
}

function InvalidatePings()
{
	local int i;
	StopPings();
	PingStart = 0;
	NumReceivedPings=0;
	NumPlayers=0;
	for( i=0;i<Servers.Length;i++ )
	{
		Servers[i].Ping = 9999;
	    UpdatedItem(i);
	}
}

function AutoPingServers()
{
	local int i, j;

	for( i=PingStart;i<Servers.Length && OutstandingPings.Length < UseSimultaneousPings;i++ )
	{
        if( Servers[i].Ping == 9999 )
		{
			// see if we already have an outstanding ping for this server.
			for( j=0;j<OutstandingPings.Length;j++ )
				if( OutstandingPings[j] == i )
					break;

			if( j == OutstandingPings.Length )
			{
				// ping
				MyPage.PingServer( i, PC_AutoPing, Servers[i] );

				// add out outstanding ping list
				j = OutstandingPings.Length;
				OutstandingPings.Length = j+1;
				OutstandingPings[j] = i;
			}
		}
	}

	if(OutstandingPings.Length == 0)
		NumReceivedPings = Servers.Length;
}

function StopPings()
{
	OutstandingPings.Remove(0,OutstandingPings.Length);
	MyPage.CancelPings();     
}

event Timer()
{
	if( Index >= 0 )
		MyOnChange( Self );
}

function MyOnChange(GUIComponent Sender)
{
	MyRulesList.Clear();
	MyPlayersList.Clear();
	if( Index >= 0 )
	{
		// when changing selected servers, get their rules
		if( Sender != None )
			MyPage.PingServer( SortData[Index].SortItem, PC_Clicked, Servers[SortData[Index].SortItem] );

		MyRulesList.ItemCount	= Servers[SortData[Index].SortItem].ServerInfo.Length;
		MyRulesList.ListItem	= SortData[Index].SortItem;
        MyPlayersList.ItemCount = Servers[SortData[Index].SortItem].PlayerInfo.Length;
		MyPlayersList.ListItem	= SortData[Index].SortItem;
		SetTimer( 5, false );
	}
	else
	{
		MyRulesList.ItemCount	= 0;
		MyPlayersList.ItemCount = 0;
	}
}

function int RemoveCurrentServer()
{
	local int OldItem;
	if( Index >= 0 )
	{
		OldItem = SortData[Index].SortItem;

		Servers.Remove( OldItem, 1 );
		ItemCount--;

		MyRulesList.ItemCount	= 0;
		MyPlayersList.ItemCount = 0;

		// tell sort lists.
		RemovedCurrent();

		return OldItem;
	}
	return -1;
}

function MyOnDrawItem(Canvas Canvas, int i, float X, float Y, float W, float H, bool bSelected)
{
	local float CellLeft, CellWidth;
	local string Ping;

	if( bSelected )
		SelStyle.Draw(Canvas,MSAT_Pressed, X, Y-2, W, H+2 );

	GetCellLeftWidth( 0, CellLeft, CellWidth );
	Style.DrawText( Canvas, MenuState, X+CellLeft, Y, CellWidth, H, TXTA_Left, Servers[SortData[i].SortItem].ServerName );

	GetCellLeftWidth( 1, CellLeft, CellWidth );
	Style.DrawText( Canvas, MenuState, X+CellLeft, Y, CellWidth, H, TXTA_Left, Servers[SortData[i].SortItem].MapName );

	GetCellLeftWidth( 2, CellLeft, CellWidth );
	if( Servers[SortData[i].SortItem].CurrentPlayers>0 || Servers[SortData[i].SortItem].MaxPlayers>0 )
		Style.DrawText( Canvas, MenuState, X+CellLeft, Y, CellWidth, H, TXTA_Left, string(Servers[SortData[i].SortItem].CurrentPlayers)$"/"$string(Servers[SortData[i].SortItem].MaxPlayers) );

	GetCellLeftWidth( 3, CellLeft, CellWidth );
	if( Servers[SortData[i].SortItem].Ping == 9999 )
		Ping = "?";
	else
	if( Servers[SortData[i].SortItem].Ping == 10000 )
		Ping = "N/A";
	else
		Ping = string(Servers[SortData[i].SortItem].Ping);
	Style.DrawText( Canvas, MenuState, X+CellLeft, Y, CellWidth, H, TXTA_Left, Ping );

//	GetCellLeftWidth( 4, CellLeft, CellWidth );
//	Style.DrawText( Canvas, MenuState, X+CellLeft, Y, CellWidth, H, TXTA_Left, SortData[i].SortString );
}

function string GetSortString( int i )
{
	local string s, t;

	switch( SortColumn )
	{
	case 0:
		s = Left(caps(Servers[i].ServerName), 8);
		break;
	case 1:
		s = Left(caps(Servers[i].MapName), 8);
		break;
	case 2:
		s = string(Servers[i].CurrentPlayers);
		while(Len(s) < 4 )
			s = "0"$s;
		t = string(Servers[i].MaxPlayers);
		while(Len(t) < 4 )
			t = "0"$t;
		s = s $ t;
		break;
	case 3:
		s = string(Servers[i].Ping);
		while(Len(s) < 5 )
			s = "0"$s;
		break;
	default:
		s = string(Servers[i].Ping);
		while(Len(s) < 5 )
			s = "0"$s;
		break;
	}
	return s;
}

defaultproperties
{
	ColumnHeadings(0)="Server Name"
	ColumnHeadings(1)="Map"
	ColumnHeadings(2)="Players"
	ColumnHeadings(3)="Ping"

	InitColumnPerc(0)=0.45
	InitColumnPerc(1)=0.25
	InitColumnPerc(2)=0.15
	InitColumnPerc(3)=0.15

	SortColumn=3
	SortDescending=False
	MaxSimultaneousPings=0
}