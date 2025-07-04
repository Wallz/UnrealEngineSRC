class UTServerAdmin extends WebApplication config;


var() class<UTServerAdminSpectator> SpectatorType;
var array< class<xWebQueryHandler> >		QueryHandlerClasses;
var config array<xWebQueryHandler>			QueryHandlers;

// Authentication items
var UTServerAdminSpectator Spectator;	// Used to get console messages
var xAdminUser CurAdmin;				// Currently logged admin (not thread safe)
var PlayInfo		GamePI;

var array<xUtil.MutatorRecord> AllMutators;
var StringArray			AGameType;		// All available Game Types
var StringArray			AExcMutators;	// All available Mutators (Excluded)
var StringArray			AIncMutators;	// All Mutators currently in play

var WebResponse		Resp;

var config string RootFrame;			// This is the master frame divided in 2: Top = Header, bottom = frame page
var config string HeaderPage;			// This is the header menu
var config string MessagePage;			// Name of the file containing the message template
var config string RestartPage;			// This is the page that users will be transferred to when restarting the server

var config string DefaultBG;
var config string HighlightedBG;

var config string AdminRealm;

var string htm;

event Init()
{
	// TODO: Destroy if not a server
	Super.Init();
	
	if (SpectatorType != None)
		Spectator = Level.Spawn(SpectatorType);
	else
		Spectator = Level.Spawn(class'UTServerAdminSpectator');

	if (Spectator != None)
		Spectator.Server = self;
	
	// won't change as long as the server is up and the map hasnt changed
	LoadQueryHandlers();
	LoadGameTypes();
	LoadMutators();
	
	Log(class@"Initialized on Port"@WebServer.ListenPort);
}

function LoadQueryHandlers()
{
local int i, j, cnt;
local xWebQueryHandler	QH;
local class<xWebQueryHandler> QHC;

	cnt = 0;
	for (i=0; i<QueryHandlerClasses.Length; i++)
	{
		QHC = QueryHandlerClasses[i];
		// Skip invalid classes;
		if (QHC != None)
		{
			// Make sure we dont have duplicate instance of the same class
			for (j=0;j<QueryHandlers.Length; j++)
			{
				if (QueryHandlers[j].Class == QHC)
				{
					QHC = None;
					break;
				}
			}
			
			if (QHC != None)
			{
				QH = new QHC;
				if (QH != None)
				{
					if (QH.Init())
					{
						QueryHandlers.Length = QueryHandlers.Length+1;
						QueryHandlers[QueryHandlers.Length - 1] = QH;
					}
					else
					{
						Log("WebQueryHandler:"@QHC@"could not be initialized");
					}
				}
			}
		}
	}
}

event bool PreQuery(WebRequest Request, WebResponse Response)
{
	if (Level == None || Level.Game == None || Level.Game.AccessControl == None)
	{
		ShowMessage(Response, "Error", "Exception Occured During Authentication!");
		return false;
	}

	if (Spectator == None)
	{
		if (SpectatorType != None)
			Spectator = Level.Spawn(SpectatorType);
		else
			Spectator = Level.Spawn(class'UTServerAdminSpectator');

		if (Spectator != None)
			Spectator.Server = self;
	}
	if (Spectator == None)
	{
		ShowMessage(Response, "Error", "Exception Occured During Authentication!");
		return false;
	}
	// Check authentication:
	// TODO: Simply use Spectator.WebLogin() or something like that
	if (!Level.Game.AccessControl.AdminLogin(Spectator, Request.Username, Request.Password))
	{
		Response.FailAuthentication(AdminRealm);
		return false;
	}
	CurAdmin = Level.Game.AccessControl.GetLoggedAdmin(Spectator);
	if (CurAdmin == None)
	{
		ShowMessage(Response, "Error", "Exception Occured During Authentication!");
		Level.Game.AccessControl.AdminLogout(Spectator);
		return false;
	}
	Resp = Response;
	return true;
}

event PostQuery(WebRequest Request, WebResponse Response)
{
	Resp = None;
	CurAdmin = None;
	Level.Game.AccessControl.AdminLogout(Spectator);
}

//*****************************************************************************
//
//        Begin Query Functions
//
//*****************************************************************************

event Query(WebRequest Request, WebResponse Response)
{
local int i;

	Log("URL:"@Request.URI);

	Response.Subst("BugAddress", "utbugs"$Level.EngineVersion$"@epicgames.com");

	// Match query function.  checks URI and calls appropriate input/output function
	switch (Mid(Request.URI, 1))
	{
	case "":
	case RootFrame:		QueryRootFrame(Request, Response); return;
	case HeaderPage:	QueryHeaderPage(Request, Response); return;
	case RestartPage:	if (!MapIsChanging()) QuerySubmitRestartPage(Request, Response); return;
	case "ut2003.css":	Response.SendCachedFile(Path$"/"$Mid(Request.URI, 1), "text/css"); return;
	}
	
	for (i=0; i<QueryHandlers.Length; i++)
	{
		if (QueryHandlers[i].Query(Request, Response))
			return;
	}
	ShowMessage(Response, "Error", "Page not found!");
	return;
}

function QueryRootFrame(WebRequest Request, WebResponse Response)
{
local String GroupPage;
	
	if (QueryHandlers.Length > 0)
		GroupPage = QueryHandlers[0].DefaultPage;

	GroupPage = Request.GetVariable("Group", GroupPage);
	
	Response.Subst("HeaderURI", HeaderPage$"?Group="$GroupPage);
	Response.Subst("BottomURI", GroupPage);
	
	ShowFrame(Response, RootFrame);
}

function QueryHeaderPage(WebRequest Request, WebResponse Response)
{
local int i;
local string menu, GroupPage, Dis, CurPageTitle;

	Response.Subst("AdminName", CurAdmin.UserName);

	if (QueryHandlers.Length > 0)
	{
		GroupPage = Request.GetVariable("Group", QueryHandlers[0].DefaultPage);
		// We build a multi-column table for each QueryHandler
		menu = "";
		CurPageTitle = "";
		for (i=0; i<QueryHandlers.Length; i++)
		{
			if (QueryHandlers[i].DefaultPage == GroupPage)
				CurPageTitle = QueryHandlers[i].Title;
			
			Dis = "";
			if (QueryHandlers[i].NeededPrivs != "" && !CanPerform(QueryHandlers[i].NeededPrivs))
				Dis = "d";

			Response.Subst("MenuLink", RootFrame$"?Group="$QueryHandlers[i].DefaultPage);
			Response.Subst("MenuTitle", QueryHandlers[i].Title);
			menu = menu$WebInclude(HeaderPage$"_item"$Dis);
		}
		Response.Subst("Location", CurPageTitle);
		Response.Subst("HeaderMenu", menu);
	}
	// Set URIs
	ShowPage(Response, HeaderPage);
}

function QueryRestartPage(WebRequest Request, WebResponse Response)
{
	ServerChangeMap(Request, Response, Level.GetURLMap(), String(Level.Game.Class));
}

function QuerySubmitRestartPage(WebRequest Request, WebResponse Response)
{
	ServerChangeMap(Request, Response, Request.GetVariable("MapName"), Request.GetVariable("GameType"));
}

function ServerChangeMap(WebRequest Request, WebResponse Response, string MapName, string GameType)
{
local int i;
local bool bConflict;
local string Conflicts, Str, ShortName;


	if (Level.NextURL != "")
	{
		ShowMessage(Response, "Please Wait", "The server is now restarting.  Please allow 10-15 seconds while the server changes levels.");
	}

	if (Request.GetVariable("Save", "") != "")
	{
		// All we need to do is override settings as required
		for (i = 0; i<GamePI.Settings.Length; i++)
		{
			ShortName = Level.GetItemName(GamePI.Settings[i].SettingName);

			if (Request.GetVariable(GamePI.Settings[i].SettingName, "") != "")
				Level.UpdateURL(ShortName, GamePI.Settings[i].Value, false);
		}
	}
	else
	{
		bConflict = false;
		Conflicts = "";

		// Make sure we have a GamePI with the right GameType selected
		GameType = SetGamePI(GameType);

		// Check each parameter and see if it conflicts with the settings on the command line
		for (i = 0; i<GamePI.Settings.Length; i++)
		{
			// Case sensitive ???
			if (HasURLOption(GamePI.Settings[i].SettingName, Str) && !(GamePI.Settings[i].Value ~= Str))
			{
				// We have a conflicting setting, prepare a table row for it.
				Response.Subst("SettingName", GamePI.Settings[i].SettingName);
				Response.Subst("SettingText", GamePI.Settings[i].DisplayName);
				Response.Subst("DefVal", GamePI.Settings[i].Value);
				Response.Subst("URLVal", Str);
				Response.Subst("MapName", MapName);
				Response.Subst("GameType", GameType);
				Conflicts = Conflicts $ WebInclude(RestartPage$"_row");
				bConflict = true;
			}
		}
		
		if (bConflict)
		{
			// Conflicts exist .. show the RestartPage
			Response.Subst("Conflicts", Conflicts);
			Response.Subst("PostAction", RestartPage);
			Response.Subst("Section", "Restart Conflicts");
			Response.Subst("SubmitValue", "Accept");

			ShowPage(Response, RestartPage);
			return;
		}
	}
	// TODO: Add more checks for Mutators
	Level.ServerTravel(MapName$"?game="$GameType$"?mutator="$UsedMutators(), false);
	ShowMessage(Response, "Please Wait", "The server is now restarting.  Please allow 10-15 seconds while the server changes levels.");
}

function MapTitle(WebResponse Response)
{
local string str, smap;

	str = Level.Game.GameReplicationInfo.GameName$" in ";
	if (Level.Title ~= "untitled")
	{
		smap = Level.GetURLMap();
		if (Right(smap, 4) ~= ".ut2")
			str = str $ Left(smap, Len(smap) - 4);
		else
			str = str $ smap;
	}
	else
		str = str $ Level.Title;

	Response.Subst("SubTitle", str);
}

function bool MapIsChanging()
{
	if (Level.NextURL != "")
	{
		ShowMessage(Resp, "Please Wait", "The server is now restarting.  Please allow 10-15 seconds while the server changes levels.");
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
//
//
//		BEGIN HELPER FUNCTIONS
//
//

function string WebInclude(string file)
{
	return Resp.LoadParsedUHTM(Path$"/"$file$".inc");
}

function LoadGameTypes()
{
local class<GameInfo>	TempClass;
local String 			NextGame;
local int				i;

	Log("Loading Game Types");
	// reinitialize list if needed
	AGameType = New(None) class'SortedStringArray';
	
	// Compile a list of all gametypes.
	TempClass = class'Engine.GameInfo';
	NextGame = Level.GetNextInt("Engine.GameInfo", 0); 
	while (NextGame != "")
	{
		TempClass = class<GameInfo>(DynamicLoadObject(NextGame, class'Class'));
		if (TempClass != None)
			AGameType.Add(NextGame, TempClass.Default.GameName);

		NextGame = Level.GetNextInt("Engine.GameInfo", ++i);
	}
}

function LoadMutators()
{
local int NumMutatorClasses;
local class<Mutator> MClass;
local Mutator M;
local int i, id;

	AExcMutators = New(None) class'SortedStringArray';
	AIncMutators = New(None) class'StringArray';


	// Load All mutators
	class'xUtil'.static.GetMutatorList(AllMutators);
	if (Level.IsDemoBuild())
	{
		for (i=AllMutators.Length - 1; i>=0; i--)
		{
			if (AllMutators[i].ClassName ~= "xGame.MutZoomInstaGib" || AllMutators[i].ClassName ~= "UnrealGame.MutLowGrav")
				continue;

			AllMutators.Remove(i,1);
		}
	}

	for (i = 0; i<AllMutators.Length; i++)
	{
		MClass = class<Mutator>(DynamicLoadObject(AllMutators[i].ClassName, class'Class'));
		if (MClass != None)
		{
			AExcMutators.Add(string(i), AllMutators[i].ClassName);
			NumMutatorClasses++;
		}
	}
	
	// Check Current Mutators
	for (M = Level.Game.BaseMutator; M != None; M = M.NextMutator)
	{
		if (M.bUserAdded)
		{
			id = AExcMutators.FindTagId(String(M.Class));
			if (id >= 0)
			{
				AIncMutators.Add(AExcMutators.GetItem(id), AExcMutators.GetTag(id));
			}
			else
				log("Unknown Mutator in use: "@String(M.Class));
		}
	}
}

function String UsedMutators()
{
local int i;
local String OutStr;
	
	if(AIncMutators.Count() == 0)
		return "";

	OutStr = AIncMutators.GetItem(i);
	for (i=1; i<AIncMutators.Count(); i++)
	{
		OutStr = OutStr$","$AIncMutators.GetItem(i);
	}
	
	return OutStr;
}

function String PadLeft(String InStr, int Width, String PadStr)
{
	local String OutStr;
	
	if (Len(PadStr) == 0)
		PadStr = " ";
		
	for (OutStr=InStr; Len(OutStr) < Width; OutStr=PadStr$OutStr);
	
	return Right(OutStr, Width); // in case PadStr is more than one character
}

function ApplyMapList(StringArray ExcludeMaps, StringArray IncludeMaps, String GameType, String MapListType)
{
local MapList List;
local int IncludeCount, i, id;
	
	List = Level.Game.GetMapList(MapListType);
	if (List != None)
	{
		ExcludeMaps = ReloadExcludeMaps(GameType);
		IncludeMaps.Reset();
	
		IncludeCount = List.Maps.Length;
		for(i=0; i<IncludeCount; i++)
		{
			if (ExcludeMaps.Count() > 0)
			{
				id = ExcludeMaps.FindTagId(List.Maps[i]);
				if (id >= 0)
				{
					IncludeMaps.Add(ExcludeMaps.GetItem(i), ExcludeMaps.GetTag(i));
					ExcludeMaps.Remove(i);
				}
				else
					Log("*** Unknown map in Map List: "$List.Maps[i]);
			}
			else
				Log("*** Empty exclude list, i="$i);
		}
		List.Destroy();
	}
	else
		Log("Invalid Map List Type : '"$MapListType$"'");
}

// TODO: Make use of xUtil.GetMapList ?
function StringArray ReloadExcludeMaps(String GameType)
{
local class<GameInfo>	GameClass;
local string NextMap, Tag;
local StringArray	AMaps;
local array<string> Maps;
local int i;

	GameClass = class<GameInfo>(DynamicLoadObject(GameType, class'Class'));
	
	AMaps = New(None) class'SortedStringArray';

	if(GameClass.Default.MapPrefix != "")
	{
		GameClass.static.LoadMapList(GameClass.Default.MapPrefix, Maps);

		for (i = 0; i<Maps.Length; i++)
		{
			NextMap = Maps[i];
			if (ValidDemoMap(NextMap))
			{
				if(!(Left(NextMap, Len(NextMap) - 4) ~= (GameClass.Default.MapPrefix$"-TUT")))
				{
					if(Right(NextMap, 4) ~= ".ut2")
						Tag = Left(NextMap, Len(NextMap) - 4);
					else
						Tag = NextMap;
					// Add the map.
					AMaps.Add(NextMap, Tag);
				}
			}
		}
	}
	return AMaps;
}

function bool ValidDemoMap(string MapName)
{
	if (Right(MapName, 4) ~= ".ut2")
		MapName = Left(MapName, Len(MapName) - 4);

	return ( !Level.IsDemoBuild() || (MapName ~= "DM-Antalus") || (MapName ~= "DM-Asbestos") || (MapName ~= "CTF-Citadel") || (MapName ~= "BR-Anubis") );
}

function StringArray ReloadIncludeMaps(StringArray ExMaps, String GameType)
{
local class<GameInfo> GameClass;
local MapList	List;
local int i;
local string map, Tag;
local StringArray AMaps;

	GameClass = class<GameInfo>(DynamicLoadObject(GameType, class'Class'));
	
	AMaps = New(None) class'StringArray';

	if (GameClass != None)
	{	
		List = Level.Game.GetMapList(GameClass.Default.MapListType);
		if (List != None)
		{
			for (i=0; i<List.Maps.Length; i++)
			{
				map = List.Maps[i];
			
				// Build the Item Tag
				if(Right(map, 4) ~= ".ut2")
					Tag = Left(map, Len(map) - 4);
				else
					Tag = map;

				// Remove from Exclude List
				ExMaps.Remove(ExMaps.FindTagId(Tag));
				
				// Add the data to the Include List
				AMaps.Add(map, Tag);
			}
		}
	}
	return AMaps;
}

function UpdateDefaultMaps(String GameType, StringArray Maps)
{
local class<GameInfo> GameClass;
local MapList List;
local int i;
	
	GameClass = class<GameInfo>(DynamicLoadObject(GameType, class'Class'));
	if (GameClass != None && GameClass.Default.MapListType != "")
	{
		List = Level.Game.GetMapList(GameClass.Default.MapListType);
		if (List != None)
		{
			List.Maps.Length = 0;
			for (i=0; i<Maps.Count(); i++)
				if (ValidDemoMap(Maps.GetItem(i)))
					List.Maps[i] = Maps.GetItem(i);

			List.MapNum = 0;
			List.SaveConfig();
			List.Destroy();
		}
	}
}

function String GenerateGameTypeOptions(String CurrentGameType)
{
local int i;
local string SelectedStr, OptionStr;

	for (i=0; i < AGameType.Count(); i++)
	{
		if (CurrentGameType ~= AGameType.GetItem(i))
			SelectedStr = " selected";
		else
			SelectedStr = "";
		
		OptionStr = OptionStr$"<option value=\""$AGameType.GetItem(i)$"\""$SelectedStr$">"$AGameType.GetTag(i)$"</option>";
	}
	return OptionStr;
}

function String GenerateMapListOptions(String GameType, String MapListType)
{
	local class<GameInfo> GameClass;
	local String DefaultBaseClass, NextDefault, NextDesc, SelectedStr, OptionStr;
	local int NumDefaultClasses;
	
	GameClass = class<GameInfo>(DynamicLoadObject(GameType, class'Class'));
	if(GameClass == None)
		return "";

	DefaultBaseClass = GameClass.Default.MapListType;
	
	if(DefaultBaseClass == "")
		return "";

	NextDefault = "Custom";
	NextDesc = "Custom";
	
	if(DynamicLoadObject(DefaultBaseClass, class'Class') == None)
		return "";
	while( (NextDefault != "") && (NumDefaultClasses < 50) )
	{
		if (MapListType ~= NextDefault)
			SelectedStr = " selected";
		else
			SelectedStr = "";
			
		OptionStr = OptionStr$"<option value=\""$NextDefault$"\""$SelectedStr$">"$NextDesc$"</option>";
			
		Level.GetNextIntDesc(DefaultBaseClass, NumDefaultClasses++, NextDefault, NextDesc);
	}				

	return OptionStr;
}

function String GenerateMapListSelect(StringArray MapList, StringArray MovedMaps)
{
local int i;
local String ResponseStr, SelectedStr;

	if (MapList.Count() == 0)
		return "<option value=\"\">*** None ***</option>";
		
	for (i = 0; i<MapList.Count(); i++)
	{
		if (ValidDemoMap(MapList.GetTag(i)))
		{
			SelectedStr = "";
			if (MovedMaps != None && MovedMaps.FindTagId(MapList.GetTag(i)) >= 0)
				SelectedStr = " selected";
			ResponseStr = ResponseStr$"<option value=\""$MapList.GetTag(i)$"\""$SelectedStr$">"$MapList.GetTag(i)$"</option>";
		}
	}
	
	return ResponseStr;
}

function string SetGamePI(string GameType)
{
local class<GameInfo> GameClass;

	GameClass = class<GameInfo>(DynamicLoadObject(GameType, class'Class'));
	if (GameClass == None)
		GameClass = Level.Game.Class;

	if (GamePI == None)
	{
		GamePI = new(None) class'PlayInfo';
		GameClass.static.FillPlayInfo(GamePI);
		Level.Game.AccessControl.FillPlayInfo(GamePI);
	}
	else if (GamePI.InfoClasses.Length>0 && GameClass != GamePI.InfoClasses[0])
	{
		GamePI.Clear();
		GameClass.static.FillPlayInfo(GamePI);
		Level.Game.AccessControl.FillPlayInfo(GamePI);
	}

	return string(GameClass);
}

function bool HasURLOption(string ParamName, out string Value)
{
local string Param;
local int i;

	Param = ParamName;
	while (true)
	{
		i = Instr(Param, ".");
		if (i < 0)
			break;

		Param = Mid(Param, i+1);
	}

	Value = Level.GetUrlOption(Param);
	return Value != "";
}

// Replaces any occurences of '<' or '>' with '&lt;' and '&gt;' 
function string HtmlEncode(string src)
{
local string Encoded;
local int p;

	// First, "<"
	Encoded = "";
	for (p=Instr(src, "<"); p != -1; p=Instr(src, "<"))
	{
		Encoded = Left(src, p) $ "&lt;";
		src = Mid(src, p + 1);
	}
	src = Encoded $ src;

	// Then, ">"
	Encoded = "";
	for (p=Instr(src, ">"); p != -1; p=Instr(src, ">"))
	{
		Encoded = Left(src, p) $ "&gt;";
		src = Mid(src, p + 1);
	}
	src = Encoded $ src;

	return src;
}

///////////////////////////////////////////////////////////////////
// Page Display functions
//
//

function bool ShowFrame(WebResponse Response, string Page)
{
	Response.IncludeUHTM(Path$"/"$Page$htm);
	return true;
}

function bool ShowPage(WebResponse Response, string Page)
{
	Response.IncludeUHTM(Path$"/"$Page$htm);
	Response.ClearSubst();
	return true;
}

function StatusError(WebResponse Response, string Message)
{
	if (Left(Message,1) == "@")
		Message = Mid(Message,1);
		
	Response.Subst("Status", "<font color='Yellow'>"$Message$"</font><br>");
}

function StatusOk(WebResponse Response, string Message)
{
	Response.Subst("Status", "<font color='#33cc66'>"$Message$"</font>");
}

function bool StatusReport(WebResponse Response, string ErrorMessage, string SuccessMessage)
{
	if (ErrorMessage == "")
		StatusOk(Response, SuccessMessage);
	else
		StatusError(Response, ErrorMessage);

	return ErrorMessage=="";
} 

function ShowMessage(WebResponse Response, string Title, string Message)
{
	Response.Subst("Section", Title);
	Response.Subst("Message", Message);
	Response.IncludeUHTM(Path$"/"$MessagePage$htm);
}

function AccessDenied(WebResponse Response)
{
	ShowMessage(Response, "Access Denied", "Your privileges disallow you from viewing this page");
}

function string HyperLink(string url, string text, bool bEnabled, optional string target)
{
local string hlink;

	if (bEnabled)
	{
		hlink = "<a href='"$url$"'";
		if (target != "")
			hlink = hlink@"target='"$target$"'";
		hlink = hlink$">"$text$"</a>";
		return hlink;
	}
	return text;
}

// Admin & Groups Management functions
function string StringIf(bool bCond, string iftrue, string iffalse)
{
  if (bCond)
	return iftrue;

  return iffalse;
}

function string Checkbox(string tag, bool bChecked)
{
local string str;

	str = "<input type='Checkbox' name='"$tag$"' value='1'";
	if (bChecked)
		str = str@"checked";
	return str$">";
}

function string NextPriv(out string privs)
{
local int pos;
local string priv;

	pos = Instr(privs, "|");
	if (pos == -1)
	{
		priv = privs;
		privs = "";
	}
	else
	{
		priv = Left(privs,pos);
		privs = Mid(privs,pos+1);
	}
	return priv;
}

function bool CanPerform(string privs)
{
local string priv;

	priv = NextPriv(privs);
	while (priv != "")
	{
		if (Level.Game.AccessControl.AllowPriv(priv) && CurAdmin.HasPrivilege(priv))
			return true;
			
		priv = NextPriv(privs);
	}
	return false;
}

defaultproperties
{
    SpectatorType=Class'UTServerAdminSpectator'
	
	QueryHandlerClasses(0)=class'xWebAdmin.xWebQueryCurrent'
	QueryHandlerClasses(1)=class'xWebAdmin.xWebQueryDefaults'
	QueryHandlerClasses(2)=class'xWebAdmin.xWebQueryAdmins'
	
    AdminRealm="UT Remote Admin Server"
    RootFrame="rootframe"
    HeaderPage="mainmenu"
    MessagePage="message"
	RestartPage="server_restart"
	
    DefaultBG="#aaaaaa"
    HighlightedBG="#3a7c8c"
	
	htm=".htm"
}
