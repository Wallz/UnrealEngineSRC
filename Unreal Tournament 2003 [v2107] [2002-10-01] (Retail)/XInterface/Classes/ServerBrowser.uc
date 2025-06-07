// ====================================================================
//  Written by Jack Porter
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class ServerBrowser extends GUIPage;

#exec OBJ LOAD FILE=InterfaceContent.utx

var Browser_Page MOTDPage;
var Browser_Page DMServerListPage;
var Browser_Page TDMServerListPage;
var Browser_Page CTFServerListPage;
var Browser_Page BRServerListPage;
var Browser_Page DomServerListPage; 
var Browser_Page FavoritesPage;
var Browser_Page LANPage;
var Browser_Page PrefsPage;
var Browser_Page BuddiesPage;

var bool bCreatedQueryTabs;
var bool bCreatedStandardTabs;

// Filtering options
var() config bool bOnlyShowStandard;
var() config bool bOnlyShowNonPassword;
var() config bool bDontShowFull;
var() config bool bDontShowEmpty;
var() config string DesiredMutator;
var() config string CustomQuery;

var() config enum EStatsServerView
{    
	SSV_Any,
    SSV_OnlyStatsEnabled,
    SSV_NoStatsEnabled,
} StatsServerView;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	GUITitleBar(Controls[0]).DockedTabs = GUITabControl(Controls[1]);		

	// delegates
	OnClose = InternalOnClose;

	// Add pages
	if(!bCreatedStandardTabs)
	{
		AddBrowserPage(MOTDPage);
		AddBrowserPage(PrefsPage);
		AddBrowserPage(LANPage);	
		AddBrowserPage(FavoritesPage);

		bCreatedStandardTabs=true;
	}
}

function MOTDVerified()
{
	if( bCreatedQueryTabs )
		return;

	bCreatedQueryTabs = true;
	AddBrowserPage(DMServerListPage);
	AddBrowserPage(TDMServerListPage);
	if(!PlayerOwner().Level.IsDemoBuild())
		AddBrowserPage(DomServerListPage);	
	AddBrowserPage(CTFServerListPage);
	AddBrowserPage(BRServerListPage);
	AddBrowserPage(BuddiesPage);
}

function AddBrowserPage( Browser_Page NewPage )
{
	local GUITabControl TabC;

	NewPage.Browser = Self;
	TabC = GUITabControl(Controls[1]);
	TabC.AddTab(NewPage.PageCaption,"", NewPage);
}

delegate OnAddFavorite( GameInfo.ServerResponseLine Server );

function InternalOnClose(optional Bool bCanceled)
{
	local int i;
	local GUITabControl TabC;

	TabC = GUITabControl(Controls[1]);

	for( i=0;i<TabC.TabStack.Length;i++ )
		Browser_Page(TabC.TabStack[i].MyPanel).OnCloseBrowser();

	Super.OnClose(bCanceled);
}

defaultproperties
{
	Begin Object Class=Browser_MOTD Name=MyMOTDPage
		PageCaption="News"
	End Object
	MOTDPage=MyMOTDPage

	Begin Object Class=Browser_ServerListPageMS Name=MyDMServerListPage
		GameType="xDeathMatch"
		PageCaption="DM"
	End Object
	DMServerListPage=MyDMServerListPage

	Begin Object Class=Browser_ServerListPageMS Name=MyTDMServerListPage
		GameType="xTeamGame"
		PageCaption="Team DM"
	End Object
	TDMServerListPage=MyTDMServerListPage

	Begin Object Class=Browser_ServerListPageMS Name=MyCTFServerListPage
		GameType="xCTFGame"
		PageCaption="CTF"
	End Object
	CTFServerListPage=MyCTFServerListPage

	Begin Object Class=Browser_ServerListPageMS Name=MyBRServerListPage
		GameType="xBombingRun"
		PageCaption="Bombing Run"
	End Object
	BRServerListPage=MyBRServerListPage

	Begin Object Class=Browser_ServerListPageMS Name=MyDomServerListPage
		GameType="xDoubleDom"
		PageCaption="Double Dom"
	End Object
	DomServerListPage=MyDomServerListPage

	Begin Object Class=Browser_ServerListPageFavorites Name=MyFavoritesPage
		PageCaption="Favorites"
	End Object
	FavoritesPage=MyFavoritesPage

	Begin Object Class=Browser_ServerListPageLAN Name=MyLANPage
		PageCaption="LAN"
	End Object
	LANPage=MyLANPage

	Begin Object Class=Browser_ServerListPageBuddy Name=MyBuddiesPage
		GameType="xBombingRun"
		PageCaption="Buddies"
	End Object
	BuddiesPage=MyBuddiesPage

	Begin Object Class=Browser_Prefs Name=MyPrefsPage
		PageCaption="Filter"
	End Object
	PrefsPage=MyPrefsPage	

	Begin Object class=GUITitleBar name=ServerBrowserHeader
		Caption="Server Browser"
		StyleName="Header"
		WinWidth=1
		WinHeight=46.000000
		WinLeft=0
		WinTop=0.036406
	End Object
	Controls(0)=GUITitleBar'ServerBrowserHeader'

	Begin Object Class=GUITabControl Name=ServerBrowserTabs
		WinWidth=1.0
		WinLeft=0
		WinTop=0.25
		WinHeight=48
		TabHeight=0.04
		bFillSpace=False
		bAcceptsInput=true
		bDockPanels=true
	End Object
	Controls(1)=GUITabControl'ServerBrowserTabs'

	Background=Material'InterfaceContent.Backgrounds.bg10'
	WinWidth=1.0
	WinHeight=1.0
	WinTop=0.0
	WinLeft=0.0
	bCheckResolution=true
	bCreatedQueryTabs=False
	bPersistent=true

	//Filtering
	StatsServerView=SSV_Any
	bOnlyShowStandard=false
	bOnlyShowNonPassword=false
	bDontShowFull=false
	bDontShowEmpty=false
	DesiredMutator="Any"
	CustomQuery=""
}