// ====================================================================
//  Class:  XInterface.Tab_InstantActionMain
//  Parent: XInterface.GUITabPanel
//
//  <Enter a description here>
// ====================================================================

class Tab_InstantActionMain extends GUITabPanel;

var	config string	LastGameType;			// Stores the last known settings
var config string	LastMap;
var	config int		LastBotSkill,
					LastBotCount;
var config bool		LastUseMapBots,
					LastUseCustomBots;

var GUIListBox 			MyMapList;
var moComboBox 			MyGameCombo;
var GUIImage   			MyMapImage;
var GUIScrollTextBox	MyMapScroll;
var	moComboBox			MyBotSkill;
var moCheckBox			MyUseMapBots;
var moCheckBox			MyUseCustomBots;
var moNumericEdit		MyBotCount;
var GUILabel			MyMapName;

var localized string	DifficultyLevels[8];
var localized string	MessageNoInfo;

delegate OnChangeGameType();
delegate OnChangeCustomBots();

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	local string Entry, Desc;
	local int index;

	Super.Initcomponent(MyController, MyOwner);

	MyBotCount				= moNumericEdit(Controls[1]);
	MyGameCombo 			= moComboBox(Controls[2]);
	MyMapList   			= GUIListBox(Controls[3]);
	MyMapImage  			= GUIImage(Controls[4]);
	MyMapScroll 			= GUIScrollTextBox(Controls[5]);
	MyBotSkill				= moComboBox(Controls[7]);
	MyUseMapBots			= moCheckBox(Controls[8]);
	MyUseCustomBots			= moCheckBox(COntrols[9]);
	MyMapName				= GUILabel(Controls[12]);

	MyMapList.List.OnChange = MapListChange;
	MyMapList.List.OnDblClick = MapListDblClick;
	
	Index = 0;
	PlayerOwner().GetNextIntDesc("GameInfo",Index,Entry,Desc);
	while (Entry != "") 
	{
		Desc = Entry$"|"$Desc;
		
		if ( ! ((PlayerOwner().Level.IsDemoBuild()) && (InStr (Desc, "xDoubleDom" ) >= 0) ) )
		{
			MyGameCombo.AddItem(ParseDescStr(Desc,2),,Desc);
			
			if ( (LastGameType=="") || (LastGameType==Entry) )
			{
				MyGameCombo.SetText(ParseDescStr(Desc,2));
				LastGameType=Entry;
			}
		}

		Index++;			
		PlayerOwner().GetNextIntDesc("GameInfo", Index, Entry, Desc);
	}

	MyGameCombo.ReadOnly(true);
	
	// Load Maps for the current GameType

	ReadMapList(GetMapPrefix());

	// Set the original map
	
	if ( (LastMap=="") || (MyMapList.List.Find(LastMap)=="") )
		MyMapList.List.SetIndex(0);

	ReadMapInfo(MyMapList.List.Get());
	
	for(index = 0;index < 8;index++)
		MyBotSkill.AddItem(DifficultyLevels[index]);
	MyBotSkill.ReadOnly(True);
	MyBotSkill.SetIndex(LastBotSkill);

	MyBotCount.SetValue(LastBotCount);
	MyUseCustomBots.Checked(LastUseCustomBots);
	MyUseMapBots.Checked(LastUseMapBots);
	
	if(LastUseMapBots)
	{
		MyUseCustomBots.Checked(false);
		MyUseCustomBots.MenuStateChange(MSAT_Disabled);
		MyBotCount.MenuStateChange(MSAT_Disabled);
	}
	else
	{
		MyUseCustomBots.MenuStateChange(MSAT_Blurry);

		if(LastUseCustomBots)
			MyBotCount.MenuStateChange(MSAT_Disabled);
		else
			MyBotCount.MenuStateChange(MSAT_Blurry);
	}
}

// Play is called when the play button is pressed.  It saves any releavent data and then
// returns any additions to the URL
function string Play()
{
	LastBotCount = MyBotCount.GetValue();
	SaveConfig();

	return "?Difficulty="$LastBotSkill$"?bAutoNumBots="$LastUseMapBots$"?NumBots="$LastBotCount;
}

function string ParseDescStr(string DescStr, int index)
{
	local string temp;
	local int p,i;
	
	i = 0;
	
	while (DescStr!="")
	{
		p = instr(DescStr,"|");
		if (p<0)
		{
			Temp = DescStr;
			DescStr = "";
		}
		else
		{
			Temp = Left(DescStr,p);
			DescStr = Right(DescStr,Len(DescStr)-p-1);
		} 
		if (i==Index)
		
			return Temp;
		
		i++;
	}
}

function string GetMapPrefix()
{
	return ParseDescStr(MyGameCombo.GetExtra(),1);
}

function string GetRulesClass()
{
	return ParseDescStr(MyGameCombo.GetExtra(),3);
}

function string GetMapListClass()
{
	return ParseDescStr(MyGameCombo.GetExtra(),4);
}

function bool GetIsTeamGame()
{
	return bool(ParseDescStr(MyGameCombo.GetExtra(),5));
}

function string GetGameClass()
{
	return ParseDescStr(MyGameCombo.GetExtra(),0);
}

//function ReadMapList(class<GameInfo> GameClass)
function ReadMapList(string MapPreFix)
{
	MyMapList.List.Clear();
	
	Controller.GetMapList(MapPrefix,MyMapList.List);

	MyMapList.List.SetIndex(0);
}

function ReadMapInfo(string MapName)
{
	local LevelSummary L;
	local string s,mName;
	local int p;

	if(MapName == "")
		return;

	L = LevelSummary(DynamicLoadObject(MapName$".LevelSummary", class'LevelSummary'));

	if ( L != none ) 
		MyMapName.Caption = L.Title;

	MyMapImage.Image = Material(DynamicLoadObject(MapName$".Screenshot", class'Material'));
	if (MyMapImage.Image==None)
	{
		if (L == none || L.ScreenShot==None)
			MyMapImage.Image = material'InterfaceContent.Menu.NoLevelPreview';
		else
			MyMapImage.Image = L.Screenshot;
	}
		
	p = instr(MapName,"-");
	if (p<0)
		mName = MapName;
	else
		mName = Right(MapName,Len(MapName)-p-1); 		
				
	s = Controller.LoadDecoText("XMaps",mName);
	if (s!="")
	{
		MyMapScroll.SetContent(S);
		return;
	}
	
	MyMapScroll.SetContent(MessageNoInfo);
		
/*	
	if(L != None)
	{
		MapTitle = L.Title;
		MapAuthor = L.Author;
		IdealPlayerCount = L.IdealPlayerCount;
	}
	else
	{
		MapTitle = "";
		MapAuthor = "";
		IdealPlayerCount = "";
	}


*/
}

function GameTypeChanged(GUIComponent Sender)
{
	local string Desc;

	if (!Controller.bCurMenuInitialized)
		return;
	
	// Load Maps for the current GameType

	Desc = MyGameCombo.GetExtra();
	ReadMapList(ParseDescStr(Desc,1));	

	LastGameType = ParseDescStr(Desc,0);
	SaveConfig();
	
	OnChangeGameType();
}


function MapListChange(GUIComponent Sender)
{
	if (!Controller.bCurMenuInitialized)
		return;

	LastMap = MyMapList.List.Get();
	SaveConfig();

	ReadMapInfo(LastMap);
}		

function BotSkillChanged(GUIComponent Sender)
{
	if (!Controller.bCurMenuInitialized)
		return;

	LastBotSkill=MyBotSkill.GetIndex();
	SaveConfig();
}

function ChangeMapBots(GUIComponent Sender)
{
	if(!Controller.bCurMenuInitialized)
		return;

	LastUseMapBots = MyUseMapBots.IsChecked();
	SaveConfig();

	if(LastUseMapBots)
	{
		MyUseCustomBots.Checked(false);
		MyUseCustomBots.MenuStateChange(MSAT_Disabled);
		MyBotCount.MenuStateChange(MSAT_Disabled);
	}
	else
	{
		MyUseCustomBots.MenuStateChange(MSAT_Blurry);
		MyBotCount.MenuStateChange(MSAT_Blurry);
	}
}

function ChangeCustomBots(GUIComponent Sender)
{
	if(!Controller.bCurMenuInitialized)
		return;

	LastUseCustomBots = MyUseCustomBots.IsChecked();
	SaveConfig();

	if(LastUseCustomBots)
		MyBotCount.MenuStateChange(MSAT_Disabled);
	else
		MyBotCount.MenuStateChange(MSAT_Blurry);

	OnChangeCustomBots();
}

function bool MapListDblClick(GUIComponent Sender)
{
	UT2InstantActionPage(Controller.ActivePage).PlayButtonClick(self);
	return true;
}	

defaultproperties  
{

	Begin Object class=GUIImage Name=IAMainBK1
		WinWidth=0.957500
		WinHeight=0.107188
		WinLeft=0.021641
		WinTop=0.024687
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Normal
		ImageStyle=ISTY_Stretched
	End Object
	Controls(0)=GUIImage'IAMainBK1'

	Begin Object class=moNumericEdit Name=IAMain_BotCount
		WinWidth=0.450000
		WinHeight=0.060000
		WinLeft=0.038476
		WinTop=0.919531
		Caption="Number of Bots"
		Hint="Choose the number of bots you wish to play with."
		CaptionWidth=0.5
		MinValue=0
		MaxValue=16
	End Object
	Controls(1)=moNumericEdit'IAMain_BotCount'

	Begin Object class=moComboBox Name=IAMain_GameType
		WinWidth=0.500000
		WinHeight=0.060000
		WinLeft=0.25
		WinTop=0.047917
		Caption="Game Type:"
		Hint="Select the type of game you wish to play."
		CaptionWidth=0.3
		OnChange=GameTypeChanged
		ComponentJustification=TXTA_Left
	End Object
	Controls(2)=GUIMenuOption'IAMain_GameType'		

	Begin Object Class=GUIListBox Name=IAMain_MapList
		WinWidth=0.478984
		WinHeight=0.463633
		WinLeft=0.021641
		WinTop=0.161406
		bVisibleWhenEmpty=true
		StyleName="SquareButton"
		Hint="Select the map to play"
	End Object
	Controls(3)=GUIListBox'IAMain_MapList'
	
	Begin Object Class=GUIImage Name=IAMain_MapImage
		WinWidth=0.443750
		WinHeight=0.405312
		WinLeft=0.531796
		WinTop=0.163489
		Image=material'InterfaceContent.Menu.NoLevelPreview'
		ImageColor=(R=255,G=255,B=255,A=255);
		ImageStyle=ISTY_Scaled
		ImageRenderStyle=MSTY_Normal
	End Object
	Controls(4)=GUIImage'IAMain_MapImage'		

	Begin Object Class=GUIScrollTextBox Name=IAMain_MapScroll
		WinWidth=0.435000
		WinHeight=0.300000
		WinLeft=0.534569
		WinTop=0.689325
		CharDelay=0.0025
		EOLDelay=0.5
		bNeverFocus=true
		StyleName="NoBackground"
	End Object
	Controls(5)=GUIScrollTextBox'IAMain_MapScroll'		

	Begin Object class=GUIImage Name=IAMainBK3
		WinWidth=0.480469
		WinHeight=0.348633
		WinLeft=0.020742
		WinTop=0.650469
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(6)=GUIImage'IAMainBK3'

	Begin Object class=moComboBox Name=IAMain_BotSkill
		WinWidth=0.450000
		WinHeight=0.060000
		WinLeft=0.038476
		WinTop=0.689531
		Caption="Bot Skill"
		Hint="Choose the skill of the bots you wish to play with."
		OnChange=BotSkillChanged
		CaptionWidth=0.5		
	End Object
	Controls(7)=moComboBox'IAMain_BotSkill'

	Begin Object class=moCheckBox Name=IAMain_UseMapBots
		WinWidth=0.450000
		WinHeight=0.060000
		WinLeft=0.038476
		WinTop=0.769531
		Caption="Use Map Bot Count"
		Hint="When enabled, an appropriate number of bots for the map will be used."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
		OnChange=ChangeMapBots
	End Object
	Controls(8)=moCheckBox'IAMain_UseMapBots'

	Begin Object class=moCheckBox Name=IAMain_UseCustomBots
		WinWidth=0.450000
		WinHeight=0.060000
		WinLeft=0.038476
		WinTop=0.849531
		Caption="Use Custom Bots"
		Hint="When enabled, you may use the Bot tab to choose bots to play with."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
		OnChange=ChangeCustomBots
	End Object
	Controls(9)=moCheckBox'IAMain_UseCustomBots'

	Begin Object class=GUIImage Name=IAMainBK2
		WinWidth=0.45
		WinHeight=0.41
		WinLeft=0.527891
		WinTop=0.161406
		Image=Material'InterfaceContent.Menu.BorderBoxA1'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Normal
		ImageStyle=ISTY_Stretched
	End Object
	Controls(10)=GUIImage'IAMainBK2'

	Begin Object class=GUIImage Name=IAMain_DescBack
		WinWidth=0.450000
		WinHeight=0.410000
		WinLeft=0.527891
		WinTop=0.591354
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Normal
		ImageStyle=ISTY_Stretched
	End Object
	Controls(11)=GUIImage'IAMain_DescBack'

	Begin Object class=GUILabel Name=IAMain_MapName
		Caption="Testing"
		TextALign=TXTA_Center
		TextFont="UT2HeaderFont"
		TextColor=(R=220,G=180,B=0,A=255)
		WinWidth=0.382813
		WinHeight=32.000000
		WinLeft=0.562304
		WinTop=0.596822
	End Object
	Controls(12)=GUILabel'IAMain_MapName'

	WinTop=0.15
	WinLeft=0
	WinWidth=1
	WinHeight=0.77
	bAcceptsInput=false		

	DifficultyLevels(0)="Novice"
	DifficultyLevels(1)="Average"
	DifficultyLevels(2)="Experienced"
	DifficultyLevels(3)="Skilled"
	DifficultyLevels(4)="Adept"
	DifficultyLevels(5)="Masterful"
	DifficultyLevels(6)="Inhuman"
	DifficultyLevels(7)="Godlike"

	MessageNoInfo="No information available!"
}
