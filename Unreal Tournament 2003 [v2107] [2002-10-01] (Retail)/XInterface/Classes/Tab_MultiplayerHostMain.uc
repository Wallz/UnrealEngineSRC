// ====================================================================
//  Class:  XInterface.Tab_MultiplayerHostMain
//  Parent: XInterface.GUITabPanel
//
//  <Enter a description here>
// ====================================================================

class Tab_MultiplayerHostMain extends GUITabPanel;

var	config string LastGameType;			// Stores the last known settings
var	config string LastMap;

var moComboBox 			MyGameCombo;
var GUIImage   			MyMapImage;
var GUIScrollTextBox	MyMapScroll;
var GUIListBox 			MyFullMapList;
var GUIListBox 			MyCurMapList;
var GUILabel			MyMapName;

var localized string	MessageNoInfo;

delegate OnChangeGameType();

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	local string Entry, Desc;
	local int index;

	Super.Initcomponent(MyController, MyOwner);
	
	MyGameCombo 			= moComboBox(Controls[2]);
	MyMapImage  			= GUIImage(Controls[3]);
	MyMapScroll 			= GUIScrollTextBox(Controls[4]);
	MyFullMapList			= GUIListBox(Controls[6]);
	MyFullMapList.List.OnDblClick=MapAdd;
	MyCurMapList			= GUIListBox(Controls[13]);
	MyCurMapList.List.OnDblClick=MapRemove;
	MyMapName				= GUILabel(Controls[14]);

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

	ReadMapList(GetMapPrefix(),GetMapListClass());

	// Set the original map
	
	if ( (LastMap=="") || (MyFullMapList.List.Find(LastMap)=="") )
		MyFullMapList.List.SetIndex(0);

	Entry = MyFullMapList.List.Get();
	if (Entry=="")
	{
		Entry = MyCurMapList.List.GetItemAtIndex(0);
	}
	if (Entry!="")
		ReadMapInfo(Entry);
	
}

// Play is called when the play button is pressed.  It saves any releavent data and then
// returns any additions to the URL
function string Play()
{
	local MapList ML;
	local class<MapList> MLClass;
	local int i;
	
	MLClass = class<MapList>(DynamicLoadObject(GetMapListClass(), class'class'));
	if (MLClass!=None)
	{
		ML = PlayerOwner().spawn(MLClass);
		if (ML!=None)
		{
			ML.Maps.Remove(0,ML.Maps.Length);
			for (i=0;i<MyCurMapList.ItemCount();i++)
				ML.Maps[i]=MyCurMapList.List.GetItemAtIndex(i);

			ML.SaveConfig();				
			ML.Destroy();
		}
	}
	
//	return "?Difficulty="$LastBotSkill$"?AutoAdjust="$LastAutoAdjust;
	return "";
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

function bool GetIsTeamGame()
{
	return bool(ParseDescStr(MyGameCombo.GetExtra(),5));
}

function string GetMapListClass()
{
	return ParseDescStr(MyGameCombo.GetExtra(),4);
}

function string GetGameClass()
{
	return ParseDescStr(MyGameCombo.GetExtra(),0);
}

function ReadMapInfo(string MapName)
{
	local material Screenshot;
	local LevelSummary L;
	local string s,mName;
	local int p;

	if(MapName == "")
		return;

	L = LevelSummary(DynamicLoadObject(MapName$".LevelSummary", class'LevelSummary'));

	if(L != None)
		MyMapName.Caption = L.Title;

	Screenshot = Material(DynamicLoadObject(MapName$".Screenshot", class'Material'));
	if (Screenshot==None)
	{
		if (L.ScreenShot==None)
			MyMapImage.Image = material'InterfaceContent.Menu.NoLevelPreview';
		else
			MyMapImage.Image = L.Screenshot;
	}
	else
		MyMapImage.Image = Screenshot;
		
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
		
}

function ReadMapList(string MapPreFix, string MapListClass)
{
	local MapList ML;
	local class<MapList> MLClass;
	local int i,j;
	
	MyFullMapList.List.Clear();
	MyCurMapList.List.Clear();
	
	Controller.GetMapList(MapPrefix,MyFullMapList.List);
	MyFullMapList.List.SetIndex(0);

	MLClass = class<MapList>(DynamicLoadObject(MapListClass, class'class'));
	if (MLClass!=None)
	{
		ML = PlayerOwner().spawn(MLClass);
		if (ML!=None)
		{
			for (i=0;i<ML.Maps.Length;i++)
			{
				for (j=0;j<MyFullMapList.ItemCount();j++)
				{
					if (MyFullMapList.List.GetItemAtIndex(j) ~= ML.Maps[i])
					{
						MyCurMapList.List.Add(ML.Maps[i]);
						MyFullmapList.List.Remove(j,1);
						break;
					}
				}				
			}
			ML.Destroy();
		}
	}
}

function bool MapAdd(GUIComponent Sender)
{
	if ( (MyFullMapList.ItemCount()==0) || (MyFullMapList.List.Index<0) )
		return true;
		
	MyCurMapList.List.Add(MyFullMapList.List.Get());
	MyFullMapList.List.Remove(MyFullMapList.List.Index,1);

	return true;
}

function bool MapRemove(GUIComponent Sender)
{
	if ( (MyCurMapList.ItemCount()==0) || (MyCurMapList.List.Index<0) )
		return true;
		
	MyFullMapList.List.Add(MyCurMapList.List.Get());
	MyFullMapList.List.SortList();
	MyCurMapList.List.Remove(MyCurMapList.List.Index,1);
	MapListChange(MyFullMapList);

	return true;
}

function bool MapAll(GUIComponent Sender)
{
	if (MyFullMapList.ItemCount()==0) 
		return true;

	MyCurMapList.List.LoadFrom(MyFullMapList.List,false);
	MyFullMapList.List.Clear();
	
	return true;
}

function bool MapClear(GUIComponent Sender)
{
	if (MyCurMapList.ItemCount()==0) 
		return true;

	MyFullMapList.List.LoadFrom(MyCurMapList.List,false);
	MyCurMapList.List.Clear();
	MapListChange(MyFullMapList);

	return true;
}

function bool MapUp(GUIComponent Sender)
{
	local int index;
	if ( (MyCurMapList.ItemCount()==0) || (MyCurMapList.List.Index<0) )
		return true;

	index = MyCurMapList.List.Index;
	if (index>0)
	{
		MyCurMapList.List.Swap(index,index-1);
		MyCurMapList.List.Index = index-1; 
	}
		
	return true;
}

function bool MapDown(GUIComponent Sender)
{
	local int index;
	if ( (MyCurMapList.ItemCount()==0) || (MyCurMapList.List.Index<0) )
		return true;

	index = MyCurMapList.List.Index;
	if (index<MyCurMapList.ItemCount()-1)
	{
		MyCurMapList.List.Swap(index,index+1);
		MyCurMapList.List.Index = index+1; 
	}

	return true;		
}

function GameTypeChanged(GUIComponent Sender)
{
	local string Desc;

	if (!Controller.bCurMenuInitialized)
		return;
	
	// Load Maps for the current GameType

	Desc = MyGameCombo.GetExtra();
	ReadMapList(GetMapPrefix(),GetMapListClass());	

	LastGameType = ParseDescStr(Desc,0);
	SaveConfig();
	
	OnChangeGameType();
}


function MapListChange(GUIComponent Sender)
{

	local string map;

	if (!Controller.bCurMenuInitialized)
		return;

	Map = GUIListBox(Sender).List.Get();
	
	if (Map=="")
		return;
	else
		LastMap = Map;

	SaveConfig();
	ReadMapInfo(LastMap);
}			

defaultproperties  
{

	Begin Object class=GUIImage Name=MPHostBK1
		WinWidth=0.957500
		WinHeight=0.107188
		WinLeft=0.016758
		WinTop=0.024687
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(0)=GUIImage'MPHostBK1'

	Begin Object class=GUIImage Name=MPHostBK2
		WinWidth=0.451172
		WinHeight=0.416016
		WinLeft=0.022695
		WinTop=0.160885
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(1)=GUIImage'MPHostBK2'

	Begin Object class=moComboBox Name=MPHost_GameType
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
	Controls(2)=GUIMenuOption'MPHost_GameType'		
	
	Begin Object Class=GUIImage Name=MPHost_MapImage
		WinWidth=0.444063
		WinHeight=0.406562
		WinLeft=0.518984
		WinTop=0.165573
		Image=material'InterfaceContent.Menu.NoLevelPreview'
		ImageColor=(R=255,G=255,B=255,A=255);
		ImageStyle=ISTY_Scaled
		ImageRenderStyle=MSTY_Normal
	End Object
	Controls(3)=GUIImage'MPHost_MapImage'		

	Begin Object Class=GUIScrollTextBox Name=MPHost_MapScroll
		WinWidth=0.430274
		WinHeight=0.302539
		WinLeft=0.030859
		WinTop=0.255209
		CharDelay=0.0025
		EOLDelay=0.5
		StyleName="NoBackground"
	End Object
	Controls(4)=GUIScrollTextBox'MPHost_MapScroll'		

	Begin Object class=GUIImage Name=MPHostBK3
		WinWidth=0.450664
		WinHeight=0.416758
		WinLeft=0.515781
		WinTop=0.160104
		Image=Material'InterfaceContent.Menu.BorderBoxA1'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Normal
		ImageStyle=ISTY_Stretched
	End Object
	Controls(5)=GUIImage'MPHostBK3'
	
	Begin Object Class=GUIListBox Name=MPHostListFullMapList
		WinWidth=0.392773
		WinHeight=0.386486
		WinLeft=0.021875
		WinTop=0.604115
		bVisibleWhenEmpty=true
		StyleName="SquareButton"
		Hint="Select the map to play"
		OnChange=MapListChange
	End Object
	Controls(6)=GUIListBox'MPHostListFullMapList'

	Begin Object Class=GUIButton Name=MPHostListAdd
		Caption="Add"
		Hint="Add this map to your map list"
		WinWidth=0.123828
		WinHeight=0.050000
		WinLeft=0.433203
		WinTop=0.679530
		OnClick=MapAdd
		OnClickSound=CS_Up
	End Object
	Controls(7)=GUIButton'MPHostListAdd'

	Begin Object Class=GUIButton Name=MPHostListRemove
		Caption="Remove"
		Hint="Remove this map from your map list"
		WinWidth=0.123828
		WinHeight=0.050000
		WinLeft=0.433203
		WinTop=0.862864
		OnClick=MapRemove
		OnClickSound=CS_Down
	End Object
	Controls(8)=GUIButton'MPHostListRemove'

	Begin Object Class=GUIButton Name=MPHostListUp
		Caption="Up"
		Hint="Move this Map Higher up in the list"
		WinWidth=0.123828
		WinHeight=0.05
		WinLeft=0.433203
		WinTop=0.610259
		OnClick=MapUp
		OnClickSound=CS_Up
	End Object
	Controls(9)=GUIButton'MPHostListUp'
	
	
	Begin Object Class=GUIButton Name=MPHostListAll
		Caption="Add All"
		Hint="All all maps to your map list"
		WinWidth=0.123828
		WinHeight=0.050000
		WinLeft=0.433203
		WinTop=0.733697
		OnClick=MapAll
		OnClickSound=CS_Up
	End Object
	Controls(10)=GUIButton'MPHostListAll'

	Begin Object Class=GUIButton Name=MPHostListClear
		Caption="Remove All"
		Hint="Remove all maps from your map list"
		WinWidth=0.123828
		WinHeight=0.050000
		WinLeft=0.433203
		WinTop=0.808697
		OnClick=MapClear
		OnClickSound=CS_Down
	End Object
	Controls(11)=GUIButton'MPHostListClear'

	Begin Object Class=GUIButton Name=MPHostListDown
		Caption="Down"
		Hint="Move this Map lower down in the list"
		WinWidth=0.123828
		WinHeight=0.050000
		WinLeft=0.433203
		WinTop=0.932135
		OnClick=MapDown
		OnClickSound=CS_Down
	End Object
	Controls(12)=GUIButton'MPHostListDown'

	Begin Object Class=GUIListBox Name=MPHostListCurMapList
		WinWidth=0.391796
		WinHeight=0.386486
		WinLeft=0.574610
		WinTop=0.604115
		bVisibleWhenEmpty=true
		StyleName="SquareButton"
		Hint="Select the map to play"
		OnChange=MapListChange
	End Object
	Controls(13)=GUIListBox'MPHostListCurMapList'

	Begin Object class=GUILabel Name=MPHostMapName
		Caption="Testing"
		TextALign=TXTA_Center
		TextFont="UT2HeaderFont"
		TextColor=(R=220,G=180,B=0,A=255)
		WinWidth=0.382813
		WinHeight=32.000000
		WinLeft=0.057617
		WinTop=0.175468
	End Object
	Controls(14)=GUILabel'MPHostMapName'
			

	MessageNoInfo="No information available!"

	WinTop=0.15
	WinLeft=0
	WinWidth=1
	WinHeight=0.77
	bAcceptsInput=false		
}
