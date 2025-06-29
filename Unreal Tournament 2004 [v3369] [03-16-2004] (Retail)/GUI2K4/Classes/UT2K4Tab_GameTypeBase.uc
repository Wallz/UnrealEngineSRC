//==============================================================================
//  Created on: 12/11/2003
//  Base class for GameType tab of the Instant Action & Host Multiplayer pages
//
//  Written by Ron Prestenback
//  � 2003, Epic Games, Inc. All Rights Reserved
//==============================================================================

class UT2K4Tab_GameTypeBase extends UT2K4GameTabBase;

struct SortHack
{
	var string GameClass;
	var byte Weight;
};

var array<SortHack>                GamePos;
var UT2K4Tab_MainBase              tp_Main;
var array<CacheManager.GameRecord> GameTypes;

// Components for left side
var automated GUISectionBackground sb_Games;
var automated GUIListBox           lb_Games;
var GUIList                        li_Games;

// Components for right side
var automated GUISectionBackground sb_Preview;
var automated GUILabel             l_NoPreview;
var automated GUIImage             i_GamePreview;
var automated AltSectionBackground i_Bk;
var automated GUIScrollTextBox     lb_GameDesc;

var localized string EpicGameCaption, CustomGameCaption;

delegate OnChangeGameType(bool bIsCustom);

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
    Super.InitComponent(MyController, MyOwner);

    li_Games = lb_Games.List;
    li_Games.bHotTrack = True;
    li_Games.OnTrack = InternalOnTrack;
    li_Games.CompareItem = GametypeSort;
    PopulateGameTypes();
    InitPreview();
    lb_GameDesc.MyScrollText.Style = lb_GameDesc.Style;

    i_BK.ManageComponent(lb_GameDesc);

}

function PopulateGameTypes()
{
    local int i,cnt;

    class'CacheManager'.static.GetGameTypeList(GameTypes);

	// Get the list of valid gametypes, and separate them into the appropriate lists
	// All official gametypes go into the large listbox
	// All custom gametypes go into the combo box (sorry guys!)
    for (i = 0; i < GameTypes.Length; i++)
    {
		if ( HasMaps(GameTypes[i]) )
		{
			if (GameTypes[i].GameTypeGroup < 3)
	            AddEpicGameType( GameTypes[i].GameName, GameTypes[i].MapListClassName);
			else
				cnt++;
		}
		else if (GameTypes[i].GameTypeGroup >= 3)
		{
			Log("Gametype"@GameTypes[i].ClassName@"found but it has no maps", 'Warning');
		}
    }

//	li_Games.SortList();

	if (cnt>0)
	{
	    li_Games.Add(CustomGameCaption,None,"",true);

	    for (i = 0; i < GameTypes.Length; i++)
	    {
			if ( HasMaps(GameTypes[i]) )
			{
				if (GameTypes[i].GameTypeGroup >= 3)
		            AddEpicGameType( GameTypes[i].GameName, GameTypes[i].MapListClassName);
		    }
	    }
		li_Games.Insert(0,EpicGameCaption,None,"",true,true);
		li_Games.SetIndex(1);
	}



}

function bool HasMaps( CacheManager.GameRecord TestRec )
{
    local array<CacheManager.MapRecord> Records;

    if ( TestRec.MapPrefix != "" )
    {
    	class'CacheManager'.static.GetMapList(Records, TestRec.MapPrefix);
    	return Records.Length > 0;
    }

    return false;
}

function InitPreview()
{
    local Material Screenie;
    local int Index;

    Index = FindRecordIndex(li_Games.Get(True));
    if ( Index >= 0 && Index < GameTypes.Length )
    {
	    Screenie = Material(DynamicLoadObject(GameTypes[Index].ScreenshotRef, class'Material'));
	    if ( GameTypes[Index].Description != "" )
	    	lb_GameDesc.SetContent(GameTypes[Index].Description);
	    else lb_GameDesc.SetContent(class'UT2K4Tab_MainBase'.default.MessageNoInfo);
	}
	else lb_GameDesc.SetContent(class'UT2K4Tab_MainBase'.default.MessageNoInfo);

	i_GamePreview.Image = Screenie;
	i_GamePreview.SetVisibility( Screenie != None );
	l_NoPreview.SetVisibility(Screenie == None);
	i_BK.Caption = li_Games.Get();
}

function int GametypeSort(GUIListElem ElemA, GUIListElem ElemB)
{
	local byte A,B;

	A = GetWeight(FindGameClass(ElemA.Item),ElemA.bSection);
	B = GetWeight(FindGameClass(ElemB.Item),ElemB.bSection);
	if (A==B)
	{
		if ( ElemA.Item > ElemB.Item )
			return 1;

		else if ( ElemA.Item < ElemB.Item )
			return -1;

		return 0;
	}
	else
		return A-B;


	return GetWeight(FindGameClass(ElemA.Item), ElemA.bSection) - GetWeight(FindGameClass(ElemB.Item), ElemA.bSection);
}

function byte GetWeight( string GameClass, bool bSection )
{
	local int i;

    if (bSection)
    	return 254;

	for ( i = 0; i < GamePos.Length; i++ )
	{
		if ( GamePos[i].GameClass ~= GameClass )
			return GamePos[i].Weight;
	}

	return 255;
}

function AddEpicGameType(string GameName, string MapList)
{
    li_Games.Add(GameName, None, MapList);
}


function InternalOnChange(GUIComponent Sender)
{
    local int Index;

    Index = FindRecordIndex(li_Games.Get());
    if ( Index < 0 || Index >= GameTypes.Length )
    	return;

    if ( Controller.LastGameType == "" || GameTypes[Index].ClassName != Controller.LastGameType )
    {
    	InitPreview();
    	Controller.LastGameType = GameTypes[Index].ClassName;
    }

    if (Sender == lb_Games)
    {

		if ( li_Games.IsSection() )
			return;

		OnChangeGameType(False);
    }
}

function InternalOnTrack(GUIComponent Sender, int OldIndex)
{
    if (!li_Games.IsValid())
        return;

	if (li_Games.IsSection())
	{
		li_Games.Index = OldIndex;
		return;
	}

	InitPreview();
}

function int FindRecordIndex(string GameName)
{
    local int i;

    for (i = 0; i < GameTypes.Length; i++)
        if (GameTypes[i].GameName ~= GameName)
            return i;

    return -1;
}

function string FindGameClass(string GameName)
{
	local int i;

	for ( i = 0; i < GameTypes.Length; i++ )
	{
		if ( GameTypes[i].GameName ~= GameName )
			return GameTypes[i].ClassName;
	}

	return "";
}

event SetVisibility( bool bIsVisible )
{
	Super.SetVisibility(bIsVisible);

	i_GamePreview.SetVisibility( i_GamePreview.Image != None );
	l_NoPreview.SetVisibility( i_GamePreview.Image == None );
}


DefaultProperties
{
	// Left Component Group
	Begin Object Class=GUISectionBackground Name=GameTypeLeftGroup
		WinWidth=0.482500
        WinHeight=0.941490
		WinLeft=0.023750
        WinTop=0.043125
        TabOrder=0
    	TopPadding=0.025
    	BottomPadding=0.025
        Caption="Available Game Types"
    End Object
    sb_Games=GameTypeLeftGroup

    Begin Object Class=GUIListBox Name=UT2004Games
		WinWidth=0.438457
		WinHeight=0.796982
		WinLeft=0.045599
		WinTop=0.144225
		bSorted=True
        bBoundToParent=True
        bScaleToParent=True
        bVisibleWhenEmpty=True
        OnChange=InternalOnChange
        StyleName="NoBackground"
        SelectedStyleName="ListSelection"
        FontScale=FNS_Large
        TabOrder=0
    End Object
    lb_Games=UT2004Games

	// Right component group
	Begin Object Class=GUISectionBackground Name=GameTypeRightGroup
        WinWidth=0.464649
        WinHeight=0.941490
        WinLeft=0.513243
        WinTop=0.043125
        Caption="Preview"
	End Object
	sb_Preview=GameTypeRightGroup

    Begin Object Class=GUILabel Name=NoPreview
		WinWidth=0.411862
		WinHeight=0.316545
		WinLeft=0.539224
		WinTop=0.142826
        TextFont="UT2HeaderFont"
        TextAlign=TXTA_Center
        VertAlign=TXTA_Center
        bMultiline=True
        bTransparent=False
        TextColor=(R=247,G=255,B=0,A=255)
        Caption="No Preview Available"
    End Object
    l_NoPreview=NoPreview

    Begin Object Class=GUIImage Name=GameTypePreview
		WinWidth=0.411862
		WinHeight=0.316545
		WinLeft=0.539224
		WinTop=0.142826
        ImageColor=(R=255,G=255,B=255,A=255)
        ImageStyle=ISTY_Scaled
        ImageRenderStyle=MSTY_Normal
        RenderWeight=0.2
    End Object
    i_GamePreview=GameTypePreview

    Begin Object Class=GUIScrollTextBox Name=GameTypeDescription
		WinWidth=0.362056
		WinHeight=0.325716
		WinLeft=0.565270
		WinTop=0.556774
        CharDelay=0.0025
        EOLDelay=0.5
        bNeverFocus=true
        StyleName="NoBackground"
        bNoTeletype=True
        bTabStop=False
    End Object
    lb_GameDesc=GameTypeDescription

	Begin Object class=AltSectionBackground name=Bk1
		WinWidth=0.419030
		WinHeight=0.474455
		WinLeft=0.535622
		WinTop=0.478553
		RenderWeight=0.3
	End Object
	i_BK=BK1


    GamePos(0)=(GameClass="UT2K4Assault.ASGameInfo",Weight=0)
    GamePos(1)=(GameClass="Onslaught.ONSOnslaughtGame",Weight=1)
    GamePos(2)=(GameClass="xGame.xDeathMatch",Weight=2)
    GamePos(3)=(GameClass="xGame.xCTFGame",Weight=3)
    GamePos(4)=(GameClass="xGame.xTeamGame",Weight=4)
    GamePos(5)=(GameClass="xGame.xDoubleDom",Weight=5)
    GamePos(6)=(GameClass="xGame.xBombingRun",Weight=6)
    GamePos(7)=(GameClass="BonusPack.xMutantGame",Weight=7)
    GamePos(8)=(GameClass="SkaarjPack.Invasion",Weight=8)
    GamePos(9)=(GameClass="BonusPack.xLastManStandingGame",Weight=9)
	GamePos(10)=(GameClass="xGame.xVehicleCTFGame",Weight=10)
	GamePos(11)=(GameClass="xGame.InstagibCTF",Weight=11)

	EpicGameCaption="Default Game Types"
	CustomGameCaption="Custom Game Type"

}

