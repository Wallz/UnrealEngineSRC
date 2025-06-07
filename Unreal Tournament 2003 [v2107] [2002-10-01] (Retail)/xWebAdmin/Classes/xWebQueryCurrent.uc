// ====================================================================
//  Class:  XWebAdmin.xWebQueryCurrent
//  Parent: XWebAdmin.xWebQueryHandler
//
//  <Enter a description here>
// ====================================================================

class xWebQueryCurrent extends xWebQueryHandler;

var config string CurrentIndexPage;		// This is the page with the Menu
var config string CurrentPlayersPage;
var config string CurrentGamePage;
var config string CurrentConsolePage;
var config string CurrentConsoleLogPage;
var config string CurrentConsoleSendPage;
var config string CurrentMutatorsPage;
var config string CurrentBotsPage;
var config string CurrentRestartPage;
var config string DefaultSendText;		// TODO : Move to Defaults

var localized string NoteGamePage;
var localized string NotePlayersPage;
var localized string NoteConsolePage;
var localized string NoteMutatorsPage;
var localized string NoteBotsPage;

var StringArray	SpeciesNames;
var array<StringArray>  BotList;		// Sorted bot list by species

function bool Query(WebRequest Request, WebResponse Response)
{
	switch (Mid(Request.URI, 1))
	{
	case DefaultPage:			QueryCurrentFrame(Request, Response); return true;	// Done : General
	case CurrentIndexPage:		QueryCurrentMenu(Request, Response); return true;	// Done : General
	case CurrentPlayersPage:	if (!MapIsChanging()) QueryCurrentPlayers(Request, Response); return true;
	case CurrentGamePage:		if (!MapIsChanging()) QueryCurrentGame(Request, Response); return true;		// Done :
	case CurrentConsolePage: 	if (!MapIsChanging()) QueryCurrentConsole(Request, Response); return true;
	case CurrentConsoleLogPage:	if (!MapIsChanging()) QueryCurrentConsoleLog(Request, Response); return true;
	case CurrentConsoleSendPage:	QueryCurrentConsoleSend(Request, Response); return true;
	case CurrentMutatorsPage:	if (!MapIsChanging()) QueryCurrentMutators(Request, Response); return true;
	case CurrentBotsPage:		if (!MapIsChanging()) QueryCurrentBots(Request, Response); return true;
	case CurrentRestartPage:	if (!MapIsChanging()) QueryRestartPage(Request, Response); return true;
	}
	return false;
}		

//*****************************************************************************
function QueryCurrentFrame(WebRequest Request, WebResponse Response)
{
local String Page;
	
	// if no page specified, use the default
	Page = Request.GetVariable("Page", CurrentGamePage);

	Response.Subst("IndexURI", 	CurrentIndexPage$"?Page="$Page);
	Response.Subst("MainURI", 	Page);
	
	ShowFrame(Response, DefaultPage);
}

function QueryCurrentMenu(WebRequest Request, WebResponse Response)
{
	local String Page;
	
	Page = Request.GetVariable("Page", CurrentGamePage);
		
	// set background colors
	Response.Subst("DefaultBG", DefaultBG);	// for unused tabs

	Response.Subst("PlayersBG", DefaultBG);
	Response.Subst("GameBG", 	DefaultBG);
	Response.Subst("ConsoleBG",	DefaultBG);
	Response.Subst("MutatorsBG",DefaultBG);
	Response.Subst("RestartBG", DefaultBG);
	
	switch(Page) {
	case CurrentPlayersPage:
		Response.Subst("PlayersBG",	HighlightedBG); break;
	case CurrentGamePage:
		Response.Subst("GameBG", 	HighlightedBG); break;
	case CurrentConsolePage:
		Response.Subst("ConsoleBG",	HighlightedBG); break;
	case CurrentMutatorsPage:
		Response.Subst("MutatorsBG",HighlightedBG); break;
	case CurrentRestartPage:
		Response.Subst("RestartBG", HighlightedBG); break;
	}

	// Set URIs
	Response.Subst("PlayersURI", 	DefaultPage$"?Page="$CurrentPlayersPage);
	Response.Subst("GameURI",		DefaultPage$"?Page="$CurrentGamePage);
	Response.Subst("ConsoleURI", 	DefaultPage$"?Page="$CurrentConsolePage);
	Response.Subst("MutatorsURI", 	DefaultPage$"?Page="$CurrentMutatorsPage);
	Response.Subst("BotsURI", 		DefaultPage$"?Page="$CurrentBotsPage);
	Response.Subst("RestartURI", 	DefaultPage$"?Page="$CurrentRestartPage);
	
	ShowPage(Response, CurrentIndexPage);
}

function QueryCurrentPlayers(WebRequest Request, WebResponse Response)
{
local string Sort, PlayerListSubst, TempStr, TempTag, TempData, TeamName;
local string KickButtonText[3], TableHeaders;
local StringArray	PlayerList;
local Controller P;
local int i, Cols;
local string IP;
local bool bCanKick, bCanBan;

	Response.Subst("Section", "Player List");
	Response.Subst("PostAction", CurrentPlayersPage);

	if (CanPerform("Xp|Kp|Kb|Mb"))
	{	
		PlayerList = new(None) class'SortedStringArray';
	
		Sort = Request.GetVariable("Sort", "Name");
		Cols = 0;
	
		bCanKick = CanPerform("Kp|Mb");
		bCanBan = CanPerform("Kb");
		
		// TODO: Allow to kick Bots
		// Count the number of Columns allowed
		if (bCanKick || bCanBan)
		{
			for ( P=Level.ControllerList; P!=None; P=P.NextController )
			{
				if(		PlayerController(P) != None 
					&&	P.PlayerReplicationInfo != None
					&&	NetConnection(PlayerController(P).Player) != None)
				{
					if ( bCanBan && Request.GetVariable("BanPlayer"$string(P.PlayerReplicationInfo.PlayerID)) != "" )
						Level.Game.AccessControl.KickBanPlayer(PlayerController(P));
					else if ( bCanKick && Request.GetVariable("KickPlayer"$string(P.PlayerReplicationInfo.PlayerID)) != "" )
						P.Destroy();
				}
				else if ( PlayerController(P) == None && bCanKick
								&& Request.GetVariable("KickPlayer"$string(P.PlayerReplicationInfo.PlayerID)) != "")
				{	// Kick Bots
					P.Destroy();
				}
			}
			
			KickButtonText[0] = "Kick";
			KickButtonText[1] = "Ban";
			KickButtonText[2] = "Kick/Ban";
			if (bCanKick) Cols += 1;
			if (bCanBan) Cols += 2;
			Response.Subst("KickButton", "<input class=button type='submit' name='Kick' value='"$KickButtonText[Cols-1]$"'>");
			
			// Build of valid TableHeaders
			TableHeaders = "";
			if (bCanKick)
			{
				Response.Subst("HeadTitle", "Kick");
				TableHeaders = TableHeaders$WebInclude(CurrentPlayersPage$"_list_head");
			}
			
			if (bCanBan)
			{
				Response.Subst("HeadTitle", "Ban");
				TableHeaders = TableHeaders$WebInclude(CurrentPlayersPage$"_list_head");
			}
			Response.Subst("HeadTitle", "Name");
			TableHeaders = TableHeaders$WebInclude(CurrentPlayersPage$"_list_head_link");
			
			if (Level.Game.GameReplicationInfo.bTeamGame)
			{
				Response.Subst("HeadTitle", "Team");
				TableHeaders = TableHeaders$WebInclude(CurrentPlayersPage$"_list_head_link");
			}
			
			Response.Subst("HeadTitle", "Ping");
			TableHeaders = TableHeaders$WebInclude(CurrentPlayersPage$"_list_head_link");
			Response.Subst("HeadTitle", "Score");
			TableHeaders = TableHeaders$WebInclude(CurrentPlayersPage$"_list_head_link");
			Response.Subst("HeadTitle", "IP");
			TableHeaders = TableHeaders$WebInclude(CurrentPlayersPage$"_list_head");
			Response.Subst("TableHeaders", TableHeaders);
		}
		
		if (CanPerform("Ba"))	// TODO: Check with GamePlayInfo for ability to set the parameter
		{
			if ((Request.GetVariable("SetMinPlayers", "") != "") && UnrealMPGameInfo(Level.Game) != None)
			{
				UnrealMPGameInfo(Level.Game).MinPlayers = Min(Max(int(Request.GetVariable("MinPlayers", String(0))), 0), 32);
				Level.Game.SaveConfig();
			}
			Response.Subst("MinPlayers", string(UnrealMPGameInfo(Level.Game).MinPlayers));
			Response.Subst("MinPlayerPart", WebInclude(CurrentPlayersPage$"_minp"));
		}	
		
		for (P=Level.ControllerList; P!=None; P=P.NextController)
		{
			if (!P.bDeleteMe && P.bIsPlayer && P.PlayerReplicationInfo != None)
			{
				TempData = "<tr>";
				
				Response.Subst("PlayerId", string(P.PlayerReplicationInfo.PlayerID));
				if (CanPerform("Kp"))
					TempData = TempData$WebInclude(CurrentPlayersPage$"_kick_col");

				if (CanPerform("Kb"))
				{
					if (PlayerController(P) != None)
						TempData = TempData$WebInclude(CurrentPlayersPage$"_ban_col");
					else
						TempData = TempData$WebInclude(CurrentPlayersPage$"_empty_col");
				}

				// TODO: Improve State Display
				if (DeathMatch(Level.Game) != None && DeathMatch(Level.Game).bTournament && P.PlayerReplicationInfo.bReadyToPlay)
					TempStr = "&nbsp;(Ready)";
				else if (P.PlayerReplicationInfo.bIsSpectator)
					TempStr = "&nbsp;(Spectator)";
				else if (PlayerController(P) == None)
					TempStr = "&nbsp;(Bot)";
				else
					TempStr = "";

				if( PlayerController(P) != None )
				{
					IP = PlayerController(P).GetPlayerNetworkAddress();
					IP = Left(IP, InStr(IP, ":"));
				}
				else
					IP = "";

				TempData = TempData$"<td><div align=\"left\" nowrap>"$HtmlEncode(P.PlayerReplicationInfo.PlayerName)$TempStr$"</div></td>";
				if (P.PlayerReplicationInfo.Team != None && P.PlayerReplicationInfo.Team.TeamIndex < 4)
				{
					TeamName = "<span style='background-color: "$class'TeamInfo'.default.ColorNames[P.PlayerReplicationInfo.Team.TeamIndex]$"'>&nbsp;&nbsp;</span> "$HtmlEncode(P.PlayerReplicationInfo.Team.GetHumanReadableName());
					TempData = TempData$"<td nowrap align=center>"$TeamName$"&nbsp;</td>";
				}			
				TempData = TempData$"<td><div align=\"center\">"$P.PlayerReplicationInfo.Ping$"</div></td>";
				TempData = TempData$"<td><div align=\"center\">"$int(P.PlayerReplicationInfo.Score)$"</div></td>";
				TempData = TempData$"<td><div align=\"center\">"$IP$"</div></td></tr>";

				switch (Sort)
				{
					case "Name":
						TempTag = P.PlayerReplicationInfo.PlayerName; break;
					case "Team":	// Ordered by Team + TeamId
						TempTag = P.PlayerReplicationInfo.Team.TeamName$Right("00"$string(P.PlayerReplicationInfo.TeamID),3); break;
					case "Ping":
						TempTag = Right("0000"$string(P.PlayerReplicationInfo.Ping), 5); break;
					default:
						TempTag = Right("000"$string(int(P.PlayerReplicationInfo.Score)), 4); break;
				}

				PlayerList.Add(TempData, TempTag);
			}
		}
		PlayerListSubst = "";
		if (PlayerList.Count() > 0)
		{
			for ( i=0; i<PlayerList.Count(); i++)
			{
				if (Sort ~= "Score")
					PlayerListSubst = PlayerList.GetItem(i)$PlayerListSubst;
				else
					PlayerListSubst = PlayerListSubst$PlayerList.GetItem(i);
			}
		}
		else
			PlayerListSubst = "<tr align=\"center\"><td colspan=\"5\">** No Players Connected **</td></tr>";

		Response.Subst("PlayerList", PlayerListSubst);
		Response.Subst("Sort", Sort);
		Response.Subst("MinPlayers", string(UnrealMPGameInfo(Level.Game).MinPlayers));

		Response.Subst("PageHelp", NotePlayersPage);
		MapTitle(Response);
		ShowPage(Response, CurrentPlayersPage);
	}
	else
		AccessDenied(Response);	
}

function QueryCurrentGame(WebRequest Request, WebResponse Response)
{
local StringArray	ExcludeMaps, IncludeMaps, MovedMaps;
local class<GameInfo> GameClass;
local string NewGameType, SwitchButtonName;
local bool bMakeChanges;

	if (CanPerform("Mt|Mm"))
	{
		if (Request.GetVariable("SwitchGameTypeAndMap", "") != "")
		{
			if (CanPerform("Mt"))
			{
				ServerChangeMap(Request, Response, Request.GetVariable("MapSelect"), Request.GetVariable("GameTypeSelect"));
			}
			else
				AccessDenied(Response);
			
			return;
		}
		else if (Request.GetVariable("SwitchMap", "") != "")
		{
			if (CanPerform("Mm|Mt"))
			{
				// TODO: Add a check to see if already changing
				Level.ServerTravel(Request.GetVariable("MapSelect")$"?game="$Level.Game.Class$"?mutator="$UsedMutators(), false);
				ShowMessage(Response, "Please Wait", "The server is now switching to map '"$Request.GetVariable("MapSelect")$"'.    Please allow 10-15 seconds while the server changes levels.");
			}
			else
				AccessDenied(Response);
			
			return;
		}

		bMakeChanges = (Request.GetVariable("ApplySettings", "") != "");

		if (CanPerform("Mt") && (bMakeChanges || Request.GetVariable("SwitchGameType", "") != ""))
		{
			NewGameType = Request.GetVariable("GameTypeSelect");
			GameClass = class<GameInfo>(DynamicLoadObject(NewGameType, class'Class'));
		}
		else
			GameClass = None;
		
		if (GameClass == None)
		{
			GameClass = Level.Game.Class;
			NewGameType = String(GameClass);
		}
		
		ExcludeMaps = ReloadExcludeMaps(NewGameType);
		IncludeMaps = ReloadIncludeMaps(ExcludeMaps, NewGameType);

		if (GameClass == Level.Game.Class)
		{
			SwitchButtonName="SwitchMap";
			MovedMaps = New(None) Class'SortedStringArray';
			MovedMaps.CopyFromId(IncludeMaps, IncludeMaps.FindItemId(Left(string(Level), InStr(string(Level), "."))$".unr"));
		}
		else
			SwitchButtonName="SwitchGameTypeAndMap";
	
		if (CanPerform("Mt"))
		{
			Response.Subst("GameTypeSelect", "<select name=\"GameTypeSelect\">"$GenerateGameTypeOptions(NewGameType)$"</select>");
			Response.Subst("GameTypeButton", "<input class=button type='submit' name='SwitchGameType' value='Switch'>");
		}
		else
			Response.Subst("GameTypeSelect", Level.Game.Default.GameName);
		
		Response.Subst("MapButton", "<input class=button type='submit' name='"$SwitchButtonName$"' value='Switch'>");
		Response.Subst("MapSelect", GenerateMapListSelect(IncludeMaps, MovedMaps ));
		Response.Subst("PostAction", CurrentGamePage);
	
		Response.Subst("Section", "Current Game");
		Response.Subst("PageHelp", NoteGamePage);
		MapTitle(Response);
		ShowPage(Response, CurrentGamePage);
	}
	else
		AccessDenied(Response);
}

function QueryCurrentConsole(WebRequest Request, WebResponse Response)
{
local String SendStr, OutStr;

	if (CanPerform("Xc"))
	{
		SendStr = Request.GetVariable("SendText", "");
		if (SendStr != "" && !(Left(SendStr, 6) ~= "debug " || SendStr ~= "debug"))
		{
			if (Left(SendStr, 4) ~= "say ")
				Level.Game.Broadcast(Spectator, "WebAdmin: "$Mid(SendStr, 4), 'Say');
			else if (SendStr ~= "dump")
				Spectator.Dump();
			else
			{
				OutStr = Level.ConsoleCommand(SendStr);
				if (OutStr != "")
					Spectator.AddMessage(None, OutStr, 'Console');
			}
		}
		
		Response.Subst("LogURI", CurrentConsoleLogPage);
		Response.Subst("SayURI", CurrentConsoleSendPage);
		ShowPage(Response, CurrentConsolePage);
	}
	else
		AccessDenied(Response);
}

function QueryCurrentConsoleLog(WebRequest Request, WebResponse Response)
{
local String LogSubst, LogStr;
local int i;


	if (CanPerform("Xc"))
	{
		Response.Subst("Section", "Server Console");
		Response.Subst("SubTitle", Level.Game.GameReplicationInfo.GameName$" in "$Level.Title);
//		Spectator.Dump();
		i = Spectator.LastMessage();
		LogStr = HtmlEncode(Spectator.NextMessage(i));
		while (LogStr  != "")
		{
			LogSubst = LogSubst$"&gt; "$LogStr$"<br>";
			LogStr = HtmlEncode(Spectator.NextMessage(i));
		}
		Response.Subst("RefreshMeta", "<meta http-equiv=\"refresh\" CONTENT=\"5; URL="$WebServer.ServerURL$Path$"/"$CurrentConsoleLogPage$"#END\">");
		Response.Subst("LogText", LogSubst);
		Response.Subst("PageHelp", NoteConsolePage);
		MapTitle(Response);
		ShowPage(Response, CurrentConsoleLogPage);
	}
	else
		AccessDenied(Response);
}

function QueryCurrentConsoleSend(WebRequest Request, WebResponse Response)
{
	if (CanPerform("Xc"))
	{
		Response.Subst("DefaultSendText", DefaultSendText);
		Response.Subst("PostAction", CurrentConsolePage);
		ShowPage(Response, CurrentConsoleSendPage);
	}
	else
		AccessDenied(Response);
}

function QueryCurrentMutators(WebRequest Request, WebResponse Response)
{
local int i, j, k, l, id;
local bool bHasSelected;
local string selectedmutes, lastgroup, nextgroup, thisgroup;
local string GrpValue;
local StringArray	GroupedMutators;

	if (CanPerform("Mu"))
	{
		GroupedMutators = new(None) class'SortedStringArray';
		if (Request.GetVariable("SetMutes", "") != "")
		{
			AIncMutators.Reset();
			lastgroup = "";
			for (i = 0; i<AExcMutators.Count(); i++)
			{
				j = int(AExcMutators.GetItem(i));
				thisgroup = AllMutators[j].GroupName;

				if (Request.GetVariable(AExcMutators.GetTag(i), "") != "" || Request.GetVariable(thisgroup) == AllMutators[j].ClassName)
					AIncMutators.Add(AExcMutators.GetItem(i), AExcMutators.GetTag(i));
			}
		}

		// Make a list sorted by groupname.classname
		for (i = 0; i<AExcMutators.Count(); i++)
		{
			j = int(AExcMutators.GetItem(i));
			GroupedMutators.Add(string(j), AllMutators[j].GroupName$"."$AllMutators[j].ClassName);
		}

		// First, Display Selected Mutators, 1 per line
		selectedmutes = "";
		for (i = 0; i<AIncMutators.Count(); i++)
		{
		j = int(AIncMutators.GetItem(i));

			Response.Subst("MutatorName", HtmlEncode(AllMutators[j].FriendlyName));
			Response.Subst("MutatorDesc", HtmlEncode(AllMutators[j].Description));

			selectedmutes = selectedmutes$WebInclude("current_mutators_selected");
		}
		if (selectedmutes != "")
		{
			Response.Subst("TableTitle", "Selected Mutators");
			Response.Subst("TableRows", selectedmutes);
			Response.Subst("SelectedTable", WebInclude("current_mutators_table"));
		}


		// Then, Display All mutators with CheckBoxes/Radio Buttons based on Groups
		lastgroup = "";
		selectedmutes="";
		for (i = 0; i<GroupedMutators.Count(); i++)
		{
			j = int(GroupedMutators.GetItem(i));

			if ( (i + 1) == GroupedMutators.Count())
				nextgroup = "";
			else
			{
				k = int(GroupedMutators.GetItem(i + 1));
				nextgroup = AllMutators[k].GroupName;
			}

			thisgroup = AllMutators[j].GroupName;

			Response.Subst("GroupName", nextgroup);
			if (lastgroup != thisgroup && thisgroup == nextgroup)
			{
				bHasSelected = false;
				GrpValue = "None";
				// See if any items in group is selected
				for (k = i; k<GroupedMutators.Count(); k++)
				{
					l = int(GroupedMutators.GetItem(k));
					if (AllMutators[l].GroupName != nextgroup)
						break;

					id = AIncMutators.FindTagId(AllMutators[l].ClassName);

					if (id != -1)
					{
						GrpValue = AllMutators[l].ClassName;
						bHasSelected = true;
						break;
					}
				}

				// Output Group Row + None Row
				Response.Subst("GroupValue", GrpValue);
				if (GrpValue == "None")
					Response.Subst("Checked", "checked");

				selectedmutes = selectedmutes$WebInclude("current_mutators_group");
			}

			id = AIncMutators.FindTagId(AllMutators[j].ClassName);
			Response.Subst("Checked", "");
			if (id >= 0)
				Response.Subst("Checked", "checked");

			Response.Subst("MutatorClass", AllMutators[j].ClassName);
			Response.Subst("MutatorName", AllMutators[j].FriendlyName);
			Response.Subst("MutatorDesc", AllMutators[j].Description);
			if (nextgroup == thisgroup || thisgroup == lastgroup)
				selectedmutes = selectedmutes$WebInclude("current_mutators_group_row");
			else
				selectedmutes = selectedmutes$WebInclude("current_mutators_row");

			lastgroup = thisgroup;
		}
		Response.Subst("TableTitle", "Pick your Mutators");
		Response.Subst("TableRows", selectedmutes);
		Response.Subst("ChooseTable", WebInclude("current_mutators_table"));


		MapTitle(Response);
		Response.Subst("Section", "Mutators");
		Response.Subst("PageHelp", NoteMutatorsPage);
		Response.Subst("PostAction", CurrentMutatorsPage);
		ShowPage(Response, CurrentMutatorsPage);
	}
	else
		AccessDenied(Response);
}

function QueryCurrentBots(WebRequest Request, WebResponse Response)
{
local array<xUtil.PlayerRecord> PlayerRecords;
local string OutStr, Lists[2], BotName;
local int i, j, BotCount, maxbots;
local xBot	B;
local bool oldstate, newstate, bInMatch;
local DeathMatch	DM;

	if (!CanPerform("Mb"))
	{
		AccessDenied(Response);
		return;
	}

	DM = DeathMatch(Level.Game);
	if (DM == None)
	{
		ShowMessage(Response, "Unsupported Game Type", "The Game Type '"$Level.Game.Class$"' does not use standard bots");
		return;
	}

	// Disable any type of Bots controls when stats are on
	if (DM.bEnableStatLogging)
	{
		ShowMessage(Response, "Bots Unavailable", "You cannot set bots while World Stats Logging is set to true");
		return;
	}

	// Make a sorted list of all species and group bots
	if (SpeciesNames == None)
	{
		class'xUtil'.static.GetPlayerList(PlayerRecords);
		SpeciesNames = new(None) class'SortedStringArray';
		for (i = 0; i<PlayerRecords.Length; i++)
			SpeciesNames.Add(PlayerRecords[i].Species.default.SpeciesName, PlayerRecords[i].Species.default.SpeciesName, true);

		BotList.Length = SpeciesNames.Count();	// Preset Bot list size

		for (i = 0; i<PlayerRecords.Length; i++)
		{
			j = SpeciesNames.FindTagId(PlayerRecords[i].Species.default.SpeciesName);
			if (BotList[j] == None)
				BotList[j] = new(None) class'SortedStringArray';

			// Add the player record to the BotList
			BotList[j].Add(PlayerRecords[i].DefaultName, PlayerRecords[i].DefaultName);
		}
	}

	bInMatch = Level.Game.IsInState('MatchInProgress');

	if (Request.GetVariable("addbotnum", "") != "")
	{
		BotCount = int(Request.GetVariable("addnum", "0"));
		if (Request.GetVariable("BotAction", "") == "Add")
		{
			maxbots = 32-(DM.NumPlayers + DM.NumBots);
			
			BotCount = Clamp(BotCount, 0, maxbots);
			for (i=0;i<BotCount; i++)
				DM.ForceAddBot();
	
			// Save the change
			if (BotCount == 0)
				StatusError(Response, "No Bots were Added");
			else if (BotCount == 1)
				StatusOk(Response, "1 Bot was Added");
			else				
				StatusOk(Response, BotCount@"Bots were added");
		}
		else if (Request.GetVariable("BotAction", "") == "Remove")
		{
			BotCount = Clamp(BotCount, 0, DM.NumBots);
		
			DM.MinPlayers = DM.NumPlayers + DM.NumBots - BotCount;
			if (BotCount == 0)
				StatusError(Response, "No Bots Were Removed");
			else if (BotCount == 1)
				StatusOk(Response, "1 Bot was Removed, it will quit soon");
			else
				StatusOk(Response, BotCount@"Bots were Removed, They will quit soon");
		}
	}
	else if (Request.GetVariable("selectbots", "") != "" && bInMatch)
	{
		// Read as many bot infos as available
		for (i = 0; i<SpeciesNames.Count(); i++)
		{
			for (j = 0; j<BotList[i].Count(); j++)
			{
				oldstate = Request.GetVariable("BotX"$i$"."$j, "") != "";
				newstate = Request.GetVariable("Bot"$i$"."$j, "") != "";
				BotName = BotList[i].GetItem(j);
				if (oldstate != newstate)
				{
					if (oldstate)	// remove the bot
					{
						B = FindPlayingBot(BotName);
						if (B != None)
						{
							Log("Removing Bot"@BotName);
							DM.MinPlayers = DM.NumPlayers + DM.NumBots - 1;
							B.Destroy();
						}
					}
					else
					{
						Log("Adding Bot"@BotName);
						DM.MinPlayers = DM.NumPlayers + DM.NumBots;
						DM.AddNamedBot(BotName);
					}
				}
			}
		}
	}

	// Build our BotList
	if (SpeciesNames != None)
	{
		Lists[0] = "";
		Lists[1] = "";
		for (i = 0; i<SpeciesNames.Count(); i++)
		{
			OutStr = "";	// Why i need this beats me.
			Response.Subst("SpeciesName", SpeciesNames.GetItem(i));
			OutStr = Response.LoadParsedUHTM(Path$"/"$CurrentBotsPage$"_species.inc");
			for (j = 0; j<BotList[i].Count(); j++)
			{
				Response.Subst("BotChecked", "");
				Response.Subst("BotIndex", String(i)$"."$String(j));
				Response.Subst("BotName", BotList[i].GetItem(j));
				B = FindPlayingBot(BotList[i].GetItem(j));

				Response.Subst("DisabledBots", "");
				if (!bInMatch)
					Response.Subst("DisabledBots", " DISABLED");
				// The following are set only if bot is currently in the game
				if (B != None)
				{
					Response.Subst("BotColor", GetTeamColor(B.PlayerReplicationInfo.Team));
					Response.Subst("BotTeamName", GetTeamName(B.PlayerReplicationInfo.Team));
					OutStr = OutStr$Response.LoadParsedUHTM(Path$"/"$CurrentBotsPage$"_row_sel.inc");
				}
				else
				{
					OutStr = OutStr$Response.LoadParsedUHTM(Path$"/"$CurrentBotsPage$"_row.inc");
				}

			}
			Lists[i % 2] = Lists[i % 2]$OutStr;
		}
	}
	
	// If not in match, make sure that the bots selection button is disabled
	Response.Subst("DisabledBots", "");
	if (!bInMatch)
		Response.Subst("DisabledBots", " DISABLED");
	MapTitle(Response);
	Response.Subst("PageHelp", NoteBotsPage);
	Response.Subst("LeftBotList", Lists[0]);
	Response.Subst("RightBotList", Lists[1]);
	Response.Subst("Section", "Bots");
	ShowPage(Response, CurrentBotsPage);
}

function xBot FindPlayingBot(string BotName) // Returns -1 on failure, or index for team/color
{
local Controller C;
local xBot B;

	for (C = Level.ControllerList; C != None; C = C.NextController)
	{
		B = xBot(C);
		if (B != None)
			if (B.PlayerReplicationInfo.PlayerName == BotName)
				return B;
	}
	return None;
}

function string GetTeamColor(TeamInfo Team)
{
	if (Team == None)
		return "";

	if (Team.TeamIndex < 4)
		return Team.ColorNames[Team.TeamIndex];

	return "#CCCCCC";
}

function string GetTeamName(TeamInfo Team)
{
	if (Team == None)
		return "";

	return Team.GetHumanReadableName();
}


defaultproperties
{
	Title="Current"
	DefaultPage="currentframe"
    CurrentIndexPage="current_menu"
    CurrentPlayersPage="current_players"
    CurrentGamePage="current_game"
    CurrentConsolePage="current_console"
    CurrentConsoleLogPage="current_console_log"
    CurrentConsoleSendPage="current_console_send"
    CurrentMutatorsPage="current_mutators"
	CurrentBotsPage="current_bots"
    CurrentRestartPage="current_restart"
    DefaultSendText="say "
	NoteGamePage="Choose a game type and/or maps then click on the corresponding change button."
	NotePlayersPage="Shows you who is currently playing. You can kick bots and players but only ban players. Use the checkboxes to choose who gets evicted."
	NoteConsolePage="Here you you tell the players when you intend to make some changes or restart with a different map. You can also see what the players are saying, as long as its not team only messages."
	NoteMutatorsPage="Select which mutators you want to be used when you hit the Restart Server Link"
	NoteBotsPage="Need to get more bots in ? Want some specific bots to join the game or want to have those too good bots out of your player's way ? You have found the right place."
}
