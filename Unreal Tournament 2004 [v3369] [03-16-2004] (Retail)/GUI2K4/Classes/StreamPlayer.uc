//==============================================================================
//	Created on: 10/15/2003
//	Graphical User Interface for MP3 & OGG player control
//
//	Written by Ron Prestenback
//	� 2003, Epic Games, Inc.  All Rights Reserved
//==============================================================================
class StreamPlayer extends FloatingWindow;

var() Automated	GUIImage				Bk1,bk2;
var() Automated	GUISectionBackground 	sb_PlayList, sb_Management;

var() Automated GUIScrollTextBox    lb_SongInfo;
var() Automated GUILabel          	l_Time;
var() Automated GUIListBox        	lb_Playlist,lb_AllPlaylists;

var() Automated GUIButton			b_BotA, b_BotB, b_BotC;

var() Automated GUISlider         	sl_Volume;
var() Automated GUIGFXButton      	b_Play, b_Stop, b_NextTrack, b_PrevTrack, b_Playlist, b_Management;
var() Automated GUICheckboxButton 	ch_Shuffle, ch_ShuffleAll, ch_Repeat, ch_RepeatAll;

var() editconst noexport GUIList    li_Playlist, li_AllLists;

var() editconst noexport editinline StreamInteraction           Handler;
var() editconst noexport editinline StreamPlaylistManager       PlaylistManager;
var() editconst noexport editinline StreamPlaylist              CurrentPlaylist;

var() config string               	OptionsMenu;
var() config string               	ID3TagEditorMenu;
var() config string               	PlaylistEditorMenu;
var() config float					ExpandedHeight;
var() float                         ConstrictedHeight;

var() editconst noexport int      SongSeconds;
var() Material                    PauseImage, PlayImage, OpenFolder, ClosedFolder;

var() localized string CollapsePlaylistHint, ExpandPlaylistHint, PlayHint, PauseHint,
					 CollapseManagementHint, ExpandManagementHint;

var() localized string RenameCaption, NewCaption, PlaylistNameCaption;
var() localized string PLAdd,PLRemove,PLClear;
var() localized string MGNew,MGRemove,MGRename;

var() int DebugIndex;
var() bool bExpand, bConstrict;

var() editconst noexport GUIContextMenu  cm_Playlist, cm_AllLists;

enum epbWinMode     // Defines the various states of a component
{
    MODE_Compact,		// Compact, just the player controls
    MODE_PlayList,		// Play list is showing
    MODE_Manager,		// Play list manage is showing
};

var() epbWinMode WindowMode;

function InitComponent( GUIController MyController, GUIComponent MyOwner )
{
	Super.InitComponent(MyController, MyOwner);

	li_Playlist = lb_Playlist.List;
	li_Playlist.TextAlign = TXTA_Left;
	li_Playlist.OnDblClick = PlaylistDblClick;
	li_Playlist.OnChange = InternalOnChange;
	li_Playlist.bMultiSelect = True;
	li_Playlist.bDropSource = True;
	li_Playlist.bDropTarget = True;
	li_Playlist.OnDragDrop = PlaylistReceiveDrop;
	li_Playlist.OnEndDrag = PlaylistDragEnded;
	li_Playlist.ContextMenu = cm_Playlist;

	li_AllLists = lb_AllPlaylists.List;
	li_AllLists.TextAlign = TXTA_Left;
	li_AllLists.OnDblClick = PlaylistDblClick;
	li_AllLists.OnChange = InternalOnChange;
	li_Alllists.bMultiSelect = False;
	li_AllLists.bDropTarget = True;
	li_AllLists.bInitializeList = False;
	li_AllLists.ContextMenu = cm_AllLists;
	li_AllLists.OnDragDrop = AllListsReceiveDrop;

	sl_Volume.OnPreDrawCaption = SliderPreDrawCaption;

	sb_PlayList.ManageComponent(lb_PlayList);
	sb_Management.ManageComponent(lb_AllPlayLists);
}

function ResolutionChanged( int ResX, int ResY )
{
	if (WindowMode==MODE_Compact)
		Constrict();

	super.ResolutionChanged(ResX,ResY);
}

event Opened( GUIComponent Sender )
{
	Super.Opened(Sender);

	Constrict();
	if ( !SetPlaylistManager() )
		Warn("Error setting PlaylistManager!");

	sl_Volume.SetValue( float(PlayerOwner().ConsoleCommand("get ini:Engine.Engine.AudioDevice MusicVolume")) );
	RefreshPlaylistSelections();
	RefreshCurrentPlaylist();
	RefreshPlaybackOptions();

}

event Closed(GUIComponent Sender, bool bCancelled)
{
	local int i;

	i = Controller.FindMenuIndexByName(PlaylistEditorMenu);
	if ( Controller.bCurMenuInitialized && i != -1 )
		Controller.RemoveMenuAt(i,true);

	Super.Closed(Sender, bCancelled);
	PlaylistManager.Save();
	HideAll();
}

function SaveCurrentPosition()
{
	if (WindowMode!=MODE_Compact)
		ExpandedHeight = WinHeight;

	DefaultLeft = WinLeft;
	DefaultTop = WinTop;
	DefaultWidth = WinWidth;

	if ( Controller.ResX <= 640 )
		DefaultHeight = 0.32;
	else DefaultHeight = 0.27;

	SaveConfig();
}

function bool SetHandler()
{
	local int i;

	if ( Controller == None || Controller.ViewportOwner == None )
		return false;

	for ( i = 0; i < Controller.ViewportOwner.LocalInteractions.Length; i++ )
	{
		if ( StreamInteraction(Controller.ViewportOwner.LocalInteractions[i]) != None )
		{
			Handler = StreamInteraction(Controller.ViewportOwner.LocalInteractions[i]);
			Handler.OnStreamChanged = HandleStreamChange;
			Handler.OnStreamingStopped = HandleStreamStop;
			Handler.OnAdjustVolume = HandleAdjustVolume;
			return true;
		}
	}

	log("StreamPlayer.SetHandler() - no StreamInteractions found!",'MusicPlayer');
	return false;
}

function bool SetPlaylistManager()
{
	if ( PlaylistManager != None )
	{
		if ( Handler == None && !SetHandler() )
			return false;

		return true;
	}

	if ( Handler == None && !SetHandler() )
		return false;

	PlaylistManager = Handler.PlaylistManager;
	PlaylistManager.ChangedActivePlaylist = ActivePlaylistChanged;

	return PlaylistManager != None;
}

// Reload the values for shuffle, repeat, and volume
function RefreshPlaybackOptions()
{
	sl_Volume.SetValue( Handler.GetStreamVolume() );
	ch_Shuffle.bChecked = PlaylistManager.GetShuffle();
	ch_ShuffleAll.bChecked = PlaylistManager.GetShuffleAll();
	ch_Repeat.bChecked = PlaylistManager.GetRepeat();
	ch_RepeatAll.bChecked = PlaylistManager.GetRepeatAll();
}

function RefreshPlaylistSelections()
{
	local StreamPlaylist List;
	local int i;
	local string str;

	if ( PlaylistManager == None )
		return;

	// Save the selected item so we can reselect it if it still exists after the refresh
	str = li_AllLists.Get();

	li_AllLists.Clear();
	for ( i = 0; i < PlaylistManager.GetPlaylistCount(); i++ )
	{
		List = PlaylistManager.GetPlaylistAt(i);
		li_AllLists.Add(List.GetTitle(), List);
	}

	i = li_AllLists.FindIndex(str);

	// If couldn't find the previously selected list, attempt to set index to current playlist
	if ( i == -1 && CurrentPlaylist != None )
		i = PlaylistManager.FindPlaylistIndex(CurrentPlaylist);

	// If that didn't work, don't worry about it
	li_AllLists.SilentSetIndex(i);
}

function ActivePlaylistChanged( StreamPlaylist NewList )
{
	RefreshCurrentPlaylist();
	if ( WindowMode == MODE_Manager )
		ShowPlaylist();
}

function RefreshCurrentPlaylist()
{
	if ( CurrentPlaylist != None )
		CurrentPlaylist.OnPlaylistChanged = None;

	CurrentPlaylist = PlaylistManager.GetCurrentPlaylist();
	if ( CurrentPlaylist == None )
	{
		Warn("Invalid playlist selected!");
		return;
	}

	LoadCurrentPlaylist();
	li_Playlist.SetIndex( li_Playlist.FindIndex("",,,CurrentPlaylist.GetCurrentStream()) );
	li_AllLists.SilentSetIndex( li_AllLists.FindIndex("",,,CurrentPlaylist) );
}

function LoadCurrentPlaylist()
{
	local int i;
	local array<Stream> Songs;

	li_Playlist.Clear();
	if ( CurrentPlaylist == None )
		return;

	CurrentPlaylist.GetSongs( Songs );
	for ( i = 0; i < Songs.Length; i++ )
		li_Playlist.Add( Songs[i].GetSongTitle(), Songs[i], Songs[i].GetPath() );

	CurrentPlaylist.OnPlaylistChanged = HandlePlaylistChange;
}

function HandlePlaylistChange()
{
	LoadCurrentPlaylist();
	PlaylistManager.Save();
}

// Called when a new stream has begun playing
function HandleStreamChange( string NewStreamFileName )
{
	local Stream S;
	local int i;

	i = CurrentPlaylist.FindIndexByName(NewStreamFileName);
	S = CurrentPlaylist.GetStreamAt(i);

	// Set timer for seconds display
	ResetSongCounter();

	if ( S != None )
		CurrentPlaylist.SetCurrent(i);
	else
	{
		li_Playlist.SetIndex(-1);

		// Attempt to load an id3 tag for this file
		S = Handler.FileManager.CreateStream(NewStreamFileName);
	}

	ReadStream(S);
	PlaylistManager.Save();
}

// Called when a song is stopped
function HandleStreamStop()
{
	UpdateSongTimeDisplay();
	UpdatePlayButton();
	li_Playlist.SetIndex(-1);
	DisableComponent(b_Stop);

	PlaylistManager.Save();
}

function HandleAdjustVolume( float NewVolume )
{
	sl_Volume.SetValue(NewVolume);
}

function bool InternalOnClick( GUIComponent C )
{
	local Stream Obj;

	switch ( C )
	{
	case b_Play:
		if ( !li_Playlist.IsValid() && !Handler.IsPlaying() )
		{
			li_Playlist.SilentSetIndex(0);
			Play();
		}

		Obj = Stream(li_Playlist.GetObject());
		if ( Obj == None || Obj != Handler.CurrentStreamAttachment )
		{
			Play();
			return true;
		}

		PauseCurrent();
		return true;

	case b_Stop:
		StopCurrent();
		return true;

	case b_NextTrack:
		NextTrack();
		return true;

	case b_PrevTrack:
		PrevTrack();
		return true;


	case b_Playlist:
		TogglePlaylist();
		return true;

	case b_Management:
		ToggleManagement();
		return true;

	case b_BotA:
		if (WindowMode==MODE_PlayList)
			return Controller.OpenMenu( PlaylistEditorMenu, CurrentPlaylist.GetTitle() );
		else if (WindowMode==MODE_Manager)
			NewPlaylist();

		return true;

	case b_BotB:
		if (WindowMode==MODE_PlayList)
			CurrentListRemove();
		else if (WindowMode==MODE_Manager)
			RemovePlayList();

		return true;

	case b_BotC:
		if (WindowMode==MODE_PlayList)
			CurrentListClear();
		else if (WindowMode==MODE_Manager)
			RenamePlayList();

	}

	return true;
}



function int GetPlaylistManagerIndex( int ListIndex )
{
	if ( PlaylistManager == None || li_AllLists == None )
		return -1;

	if ( !li_AllLists.IsValidIndex(ListIndex) )
		ListIndex = li_AllLists.Index;

	return PlaylistManager.FindNameIndex( li_AllLists.GetItemAtIndex(ListIndex) );
}

function InternalOnChange( GUIComponent C )
{
	switch ( C )
	{
	case sl_Volume:
		Handler.SetMusicVolume(sl_Volume.Value);
		PlayerOwner().ConsoleCommand("set ini:Engine.Engine.AudioDevice MusicVolume"@sl_Volume.Value);
		PlayerOwner().ConsoleCommand("SetMusicVolume"@ sl_Volume.Value);
		break;

	case ch_Shuffle:
		PlaylistManager.SetShuffle(ch_Shuffle.bChecked);
		ch_ShuffleAll.bChecked = PlaylistManager.GetShuffleAll();
		break;

	case ch_ShuffleAll:
		PlaylistManager.SetShuffleAll(ch_ShuffleAll.bChecked);
		ch_Shuffle.bChecked = PlaylistManager.GetShuffle();
		break;

	case ch_Repeat:
		PlaylistManager.SetRepeat( ch_Repeat.bChecked );
		ch_RepeatAll.bChecked = PlaylistManager.GetRepeatAll();
		break;

	case ch_RepeatAll:
		PlaylistManager.SetRepeatAll( ch_RepeatAll.bChecked );
		ch_Repeat.bChecked = PlaylistManager.GetRepeat();
		break;

	case li_Playlist:
		if ( !Handler.IsPlaying() && li_Playlist.IsValid() )
			ReadTagInfo( Stream(li_Playlist.GetObject()) );

		break;
	}
}

// Just return true because I don't want a caption drawn for this slider
function bool SliderPreDrawCaption( out float X, out float Y, out float XL, out float YL, out ETextAlign Justification )
{
	return true;
}

function bool AllListsReceiveDrop(GUIComponent Sender)
{
	local int i, idx;
	local array<GUIListElem> Elems;
	local array<string> Items;
	local Stream str;
	local StreamPlaylist List;

	if ( Controller.DropSource == li_Playlist )
	{
		List = PlaylistManager.GetPlaylistAt( GetPlaylistManagerIndex(li_AllLists.DropIndex) );
		if ( List == None || List == CurrentPlaylist )
			return false;

		List.InitializePlaylist(Handler.FileManager);
		Elems = li_Playlist.GetPendingElements(True);
		for ( i = Elems.Length - 1; i >= 0; i-- )
		{
			str = Stream(Elems[i].ExtraData);
			if ( str != None )
				List.AddStream(-1, str, True);
		}

		return true;
	}
	else if ( DirectoryTreeList(Controller.DropSource) != None )
	{
		Items = DirectoryTreeList(Controller.DropSource).GetPendingItems();
		idx = GetPlaylistManagerIndex(li_AllLists.DropIndex);
		List = PlaylistManager.GetPlaylistAt( idx );
		if ( List == None )
			return False;

		for ( i = Items.Length - 1; i >= 0; i-- )
			PlaylistManager.InsertInPlaylist( idx, -1, Items[i], i > 0);

		return True;
	}

	return false;
}

// Check which type of file we're dropping, and perform the appropriate action
function bool PlaylistReceiveDrop(GUIComponent Sender)
{
	local array<string> Items;
	local int i, idx;

	// If this was a drag-n-drop between the same list, perform the default behavior
	if ( Sender == li_Playlist )
	{
		if ( Controller.DropSource != li_AllLists && li_Playlist.InternalOnDragDrop(Sender) )
			return true;

		// if source is the directory list, then figure what type of file we're attempting to drop
		if ( DirectoryTreeList(Controller.DropSource) != None )
		{
			Items = DirectoryTreeList(Controller.DropSource).GetPendingItems();
			idx = li_Playlist.DropIndex;

			if ( !li_Playlist.IsValidIndex(idx) )
				idx = li_Playlist.Elements.Length;

			for ( i = Items.Length - 1; i >= 0; i-- )
				PlaylistManager.InsertInPlaylist(PlaylistManager.GetCurrentIndex(), idx, Items[i], i > 0);

			li_Playlist.SetIndex(idx);
			return true;
		}
	}
	return false;
}

function PlaylistDragEnded(GUIComponent Accepting, bool bAccepted)
{
	local int i;
	local array<Stream> Streams;

	li_Playlist.InternalOnEndDrag(Accepting, bAccepted);
	if ( bAccepted )
	{
		if ( Accepting == li_Playlist )
		{
			Streams.Length = li_Playlist.Elements.Length;
			for ( i = 0; i < li_Playlist.Elements.Length; i++ )
				Streams[i] = Stream(li_Playlist.GetObjectAtIndex(i));

			CurrentPlaylist.InitializePlaylist(Handler.FileManager);
			CurrentPlaylist.SetSongs(Streams);
		}
	}
}

function Play()
{
	// The stream knows where it's located on the disk
	Handler.PlaySong(li_Playlist.GetExtra(),0.0);
	UpdatePlayButton();
}
function StopCurrent()
{
	Handler.StopSong();
	UpdatePlayButton();
}

function NextTrack()
{
	Handler.NextSong();
	UpdatePlayButton();
}

function PrevTrack()
{
	Handler.PrevSong();
}

function PauseCurrent()
{
	if ( !Handler.IsPlaying() )
		Play();
	else
	{
		Handler.PauseSong();
		UpdatePlayButton();
	}
}

function bool PlaylistDblClick( GUIComponent C )
{
	local int i;

	if ( C == li_Playlist )
	{
		Play();
		return true;
	}

	else if ( C == li_AllLists )
	{
		i = PlaylistManager.FindPlaylistIndex( StreamPlaylist(li_AllLists.GetObject()) );
		PlaylistManager.ActivatePlaylist(i);
//		ShowPlayList(); -- this should be called by ActivatePlaylist
		return true;
	}

	return false;
}

function ReadTagInfo( Stream StreamObj )
{
	local string TagText;
	local StreamTag sTag;

	sTag = StreamObj.GetTag();

	// Get tag info
	TagText = StreamObj.GetSongTitle();
	if ( sTag != None )
	{
		if ( TagText != "" )
			TagText $= "|";

		TagText $= sTag.Artist.FieldValue;
		if ( TagText != "" )
			TagText $= "|";

		TagText $= sTag.Album.FieldValue;
/*
	// if this song only had an ID3v1 tag associated with it, it isn't possible to get the song duration
	// until the audio code actually loads the mp3/ogg file, so if the duration is 0.0, request an update
		if ( float(sTag.Duration.FieldValue) <= 0.0 )
			sTag.Duration.FieldValue = string(Handler.GetStreamDuration() * 1000);
*/
		// Add any other desired tag fields here
	}
	lb_SongInfo.SetContent(TagText);
}

function ReadStream( Stream StreamObj )
{
	if ( StreamObj == None )
	{
		log("ReadStream() called with StreamObj == None",'MusicPlayer');
		return;
	}

	ReadTagInfo(StreamObj);
	EnableComponent(b_Stop);

	li_Playlist.SilentSetIndex( li_Playlist.FindIndex("",,,StreamObj) );
	Handler.SetStreamAttachment(StreamObj);
}

function ReadStreamAt( int Index )
{
	local Stream StreamObj;

	StreamObj = Stream(lb_Playlist.List.GetObjectAtIndex(Index));
	if ( StreamObj == None )
	{
		log("ReadStreamInfo couldn't find stream object at index"@index,'MusicPlayer');
		return;
	}

	ReadStream(StreamObj);
}

event Timer()
{
	if ( Handler != None )
		UpdateSongTimeDisplay();
}

function ResetSongCounter( optional bool bNoRestart )
{
	SongSeconds = 0;

	if ( bNoRestart ) KillTimer();
	else SetTimer(1.0, True);

	Timer();
}

protected function UpdateSongTimeDisplay()
{
	SongSeconds = int( Handler.GetStreamPosition() );
	l_Time.Caption = class'StreamBase'.static.FormatTimeDisplay(SongSeconds);

}

function UpdatePlayButton()
{
	if ( Handler == None )
		return;

	if ( Handler.IsPlaying() && !Handler.IsPaused() )
	{
		b_Play.Graphic = PauseImage;
		b_Play.SetHint(PauseHint);
	}
	else
	{
		b_Play.SetHint(PlayHint);
		b_Play.Graphic = PlayImage;
	}
}

// ===========================================================
// Context Menu
// ===========================================================

function PlayerMenuClick( GUIContextMenu Menu, int Index )
{
	if ( Index < 0 || Index >= Menu.ContextItems.Length )
		return;

	if ( Menu.ContextItems[Index] == "-" )
		return;

	switch ( Index )
	{
	case 0:
		Controller.OpenMenu(OptionsMenu);
		return;
	}
}

function RemovePlayList()
{
	PlaylistManager.RemovePlaylist(li_AllLists.Get());
	RefreshPlaylistSelections();
}

function CurrentListClear()
{
	PlaylistManager.ClearCurrentPlaylist();
	LoadCurrentPlaylist();
}

function PlaylistMenuClick(  GUIContextMenu Menu, int Index )
{
	local int i;

	if ( Index < 0 || Index >= Menu.ContextItems.Length )
		return;

	if ( Menu.ContextItems[Index] == "-" )
		return;

	switch ( Index )
	{
	case 0:	// Activate
		i = PlaylistManager.FindPlaylistIndex( StreamPlaylist(li_AllLists.GetObject()) );
		PlaylistManager.ActivatePlaylist(i);
//		ShowPlayList();
		break;

	case 1:	// Rename
		RenamePlayList();
		break;

	case 2: // Create
		NewPlayList();
		break;

	case 4: // Remove
		RemovePlayList();
		break;

	case 5: // Clear
		CurrentListClear();
		break;
	}
}

function CurrentListRemove()
{
	local array<GUIListElem> Paths;
	local int i;

	Paths = li_Playlist.GetPendingElements(True);
	for ( i = 0; i < Paths.Length; i++ )
		PlaylistManager.RemoveFromCurrentPlaylist(Paths[i].ExtraStrData, i < Paths.Length -1 );
}

function SongMenuClick( GUIContextMenu Menu, int Index )
{
	if ( Index < 0 || Index >= Menu.ContextItems.Length )
		return;

	if ( Menu.ContextItems[Index] == "-" )
		return;

	switch ( Index )
	{
	case 0: // Play
		Play(); break;

	case 1: // Editor
		Controller.OpenMenu(PlaylistEditorMenu); break;

//	case 2: // Tag
//		if ( li_Playlist.IsValid() && Controller.OpenMenu(ID3TagEditorMenu,li_Playlist.GetExtra()) )
//			Controller.ActivePage.HandleObject(li_Playlist.GetObject());
//		break;

	case 3: // Remove
		CurrentListRemove();
		break;

	case 4: // Clear
		CurrentListClear();
		break;
	}

	li_Playlist.ClearPendingElements();
}

// =====================================================================================================================

function NewPlaylist()
{
	if ( Controller.OpenMenu(Controller.RequestDataMenu, NewCaption, PlaylistNameCaption) )
		Controller.ActivePage.OnClose = NewPlaylistClosed;
}

function RenamePlayList()
{
	if ( Controller.OpenMenu(Controller.RequestDataMenu, RenameCaption, PlaylistNameCaption) )
	{
		Controller.ActivePage.SetDataString( li_Alllists.Get() );
		Controller.ActivePage.OnClose = RenameClosed;
	}
}

function NewPlaylistClosed( bool bCancelled )
{
	local string PlaylistName;

	if ( !bCancelled )
	{
		PlaylistName = Controller.ActivePage.GetDataString();
		if ( PlaylistManager.ActivatePlaylist(PlaylistManager.AddPlaylist(PlaylistName), true) )
			RefreshPlaylistSelections();
	}
}

function RenameClosed( bool bCancelled )
{
	local string PlaylistName;

	if ( !bCancelled )
	{
		PlaylistName = Controller.ActivePage.GetDataString();
		if ( PlaylistManager.RenamePlaylist(GetPlaylistManagerIndex(-1), PlaylistName) )
			RefreshPlaylistSelections();
	}
}

// =====================================================================================================================
// =====================================================================================================================
//  Page/Component Animation
// =====================================================================================================================
// =====================================================================================================================
/*
function SetDefaultPosition()
{
	local float F;

	F = MinPageHeight;
	MinPageHeight = default.MinPageHeight;

	Super.SetDefaultPosition();

	MinPageHeight = F;
}
*/
event SetVisibility( bool bIsVisible )
{
	local int i;

	if ( !bIsVisible )
	{
		Super.SetVisibility(bIsVisible);
		return;
	}

	Super(GUIComponent).SetVisibility(bIsVisible);

    if ( !PropagateVisibility )
    	return;

    for ( i = 0; i < Controls.Length; i++ )
    {
    	// If the playlist is supposed to be hidden, skip it
    	if ( Controls[i] == sb_Playlist && WindowMode!=MODE_Playlist ||
		     Controls[i] == sb_Management && WindowMode!=MODE_Manager )
    		continue;

    	Controls[i].SetVisibility(True);
    }
}


function Expand()
{
	bResizeHeightAllowed = True;
	if ( ExpandedHeight < WinHeight )
		ExpandedHeight=WinHeight+0.1;

	WinHeight = RelativeHeight(ExpandedHeight);
	MinPageHeight = 0.4;

	CheckBounds();
}

function Constrict()
{
	bResizeHeightAllowed = False;
	WindowMode = MODE_Compact;

	if ( Controller.ResX <= 640 )
	{
		MinPageHeight = 0.32;
		WinHeight= 0.32;
	}

	else
	{
		MinPageHeight = 0.27;
		WinHeight = 0.27;
	}
}

function HideAll()
{
	Constrict();
	sb_PlayList.SetVisibility(false);
	sb_Management.SetVisibility(false);
	b_BotA.SetVisibility(false);
	b_BotB.SetVisibility(false);
	b_BotC.SetVisibility(false);

}

function TogglePlaylist()
{
	if (WindowMode!=MODE_PlayList)
		ShowPlayList();
	else
		HidePlayList();
}

function ShowPlayList()
{
	if (WindowMode==MODE_Compact)	// Expand the Window up
		Expand();

	sb_PlayList.Caption = PlayListManager.GetCurrentTitle();

	b_BotA.SetVisibility(true); b_BotA.Caption = plAdd;
	b_BotB.SetVisibility(true); b_BotB.Caption = plRemove;
	b_BotC.SetVisibility(true); b_BotC.Caption = plClear;

	sb_PlayList.SetVisibility(true);
	sb_Management.SetVisibility(false);

	WindowMode = MODE_PlayList;

	b_PlayList.Graphic=OpenFolder;

}

function HidePlayList()
{
	Constrict();

	b_BotA.SetVisibility(false);
	b_BotB.SetVisibility(false);
	b_BotC.SetVisibility(false);

	sb_PlayList.SetVisibility(false);
	b_PlayList.Graphic=ClosedFolder;
}

function ToggleManagement()
{
	if (WindowMode!=MODE_Manager)
		ShowManagement();
	else
    	HideManagement();

	return;
}

function ShowManagement()
{
	if (WindowMode==MODE_Compact)	// Expand the Window up
		Expand();

	b_BotA.SetVisibility(true); b_BotA.Caption = mgNew;
	b_BotB.SetVisibility(true); b_BotB.Caption = mgRemove;
	b_BotC.SetVisibility(true); b_BotC.Caption = mgRename;

	sb_PlayList.SetVisibility(false);
	sb_Management.SetVisibility(true);
	WindowMode = MODE_Manager;
	b_Playlist.Graphic = ClosedFolder;
}

function HideManagement()
{
	Constrict();

	b_BotA.SetVisibility(false);
	b_BotB.SetVisibility(false);
	b_BotC.SetVisibility(false);

	sb_Management.SetVisibility(false);
}


function ManageDragOver( GUIComponent Sender )
{
	if ( bAnimating )
		return;

	if ( Sender == b_Management && WindowMode != MODE_Manager )
		ShowManagement();

	else if ( Sender == b_Playlist && WindowMode != MODE_Playlist )
		ShowPlaylist();
}

function bool FloatingPreDraw( Canvas C )
{

	local float X,Y,xl,yl, BK1L, BK1T, BK1W, BK1H;
	local float l,t,w,h;
	local float bl,bt,bw,bh,bs;
	local bool b;

	if ( bInit )
		Constrict();

	b = super.FloatingPreDraw(c);

	// dock the songinfo box to the title bar
	t = t_WindowTitle.ActualTop()+ t_WindowTitle.ActualHeight();

	l = ActualLeft();
	w = ActualWidth();

	l_Time.Style.TextSize(C,MenuState,"100:00",XL,YL,l_Time.FontScale);
	h = YL*3;

	// Align the controls.

	x = l + w - (C.SizeX*0.01);
	y = t + (C.SizeY*0.01);

	// Timer Box
	BK1.WinTop = BK1.RelativeTop(y);
	BK1.WinHeight = BK1.RelativeHeight(3*YL);
	BK1.WinWidth = BK1.RelativeWidth(XL + (XL*0.32));
	BK1.WinLeft = BK1.RelativeLeft(X - BK1.ActualWidth());

	BK1T = BK1.ActualTop();
	BK1W = BK1.ActualWidth();
	BK1H = BK1.ActualHeight();
	BK1L = BK1.ActualLeft();

	l_Time.SetPosition(BK1L, BK1T, BK1W, BK1H, True);

	X = BK1L - (C.SizeX*0.01);
	lb_SongInfo.WinTop    = lb_SongInfo.RelativeTop(y - C.SizeY * 0.01);
	lb_SongInfo.WinLeft   = lb_SongInfo.RelativeLeft(l + (C.SizeX*0.02));
	lb_SongInfo.WinWidth  = lb_SongInfo.RelativeWidth(X - (lb_SongInfo.ActualLeft()));

	// Do the buttons

	bh = ActualHeight(ch_Shuffle.StandardHeight);
	bw = bh;
	bt = BK1T + BK1H - bh;
	bs = C.SizeX*0.01;
	bl = x - bw;


	MoveButton(ch_ShuffleAll,BL,BT,BW,BH,BS);
	MoveButton(ch_Shuffle,BL,BT,BW,BH,BS);
	MoveButton(ch_RepeatAll,BL,BT,BW,BH,BS);
	MoveButton(ch_Repeat,BL,BT,BW,BH,BS);
	MoveButton(b_NextTrack,BL,BT,BW,BH,BS);
	MoveButton(b_Play,BL,BT,BW,BH,BS);
	MoveButton(b_Stop,BL,BT,BW,BH,BS);
	MoveButton(b_PrevTrack,BL,BT,BW,BH,BS);

	sl_Volume.WinLeft = sl_Volume.RelativeLeft(lb_SongInfo.ActualLeft());
	sl_Volume.WinTop = sl_Volume.RelativeTop(bt);
	sl_Volume.WinHeight = sl_Volume.RelativeHeight(bh);
	sl_Volume.WinWidth = sl_Volume.RelativeWidth(bl - sl_Volume.ActualLeft());

	lb_SongInfo.WinHeight = lb_SongInfo.RelativeHeight( (sl_Volume.ActualTop() - (C.SizeY * 0.01)) - lb_SongInfo.ActualTop() );
	t = sl_Volume.ActualTop() + sl_Volume.ActualHeight() + (C.SizeY *0.01);
	l = sl_Volume.ActualLeft();

	b_PlayList.SetPosition(l,t,bw,bh,true);
	b_Management.SetPosition(l+bw+bs,t,bw,bh,true);

	l = l+(bw*2)+(bs*2);
	bt = t + bh*0.32;

	bk2.WinLeft = bk2.RelativeLeft(l);
	bk2.WinTop = bk2.RelativeTop(bt);
	bk2.WinHeight = bk2.RelativeHeight(bh*0.5);
	bk2.WinWidth = bk2.RelativeWidth((BK1L+BK1W-bw) - bk2.ActualLeft());

	l = ActualLeft();
	w = ActualWidth();
	t = bt + (bh*1.5);

	if (WindowMode==MODE_PlayList)
	{
		sb_PlayList.WinLeft   = sb_Playlist.RelativeLeft(l);
		sb_PlayList.WinWidth  = sb_Playlist.RelativeWidth(w);
		sb_PlayList.WinTop    = sb_Playlist.RelativeTop(t);
		sb_PlayList.WinHeight = sb_Playlist.RelativeHeight(ActualTop() + ActualHeight() - t);
		T = t + sb_PlayList.ActualHeight() - 32;
		SetBottomButtons(C,sb_PlayList.ActualLeft(),T,sb_PlayList.ActualWidth(), 32);
	}

	if (WindowMode==MODE_Manager)
	{
		sb_Management.WinLeft   = sb_Management.RelativeLeft(l);
		sb_Management.WinWidth  = sb_Management.RelativeWidth(w);
		sb_Management.WinTop    = sb_Management.RelativeTop(t);
		sb_Management.WinHeight = sb_Management.RelativeHeight(ActualTop() + ActualHeight() - t);
		T = t + sb_Management.ActualHeight() - 32;
		SetBottomButtons(C,sb_Management.ActualLeft(),T, sb_Management.ActualWidth(), 32);
	}

	return b;
}

function MoveButton(GUIButton B, out float L, float T, float W, float H, float S)
{
	B.SetPosition(l,t,w,h,true);
	L -= w+S;
}

function SetBottomButtons(Canvas C, float Left, float Top, float Width, float Height)
{
	local int i;
	local float xsize,xl,yl,s;
	local GUIButton B;

	s = Controller.ResX*0.01;
	for (i=0;i<Controls.Length;i++)
		if ( Controls[i].Tag>0 )
		{
			B = GUIButton(Controls[i]);
			b.Style.TextSize(C,b.MenuState,b.Caption,XL,YL,b.FontScale);
			xsize += XL + S;
			Controls[i].WinWidth = Controls[i].RelativeWidth(XL + S);
		}


	Left = Left + (Width/2) - (xsize/2);
	for (i=0;i<Controls.Length;i++)
		if ( Controls[i].Tag>0 )
		{
			Controls[i].WinTop =   Controls[i].RelativeTop(Top);
			Controls[i].WinLeft =  Controls[i].RelativeLeft(Left);
			Controls[i].WinHeight= Controls[i].RelativeHeight(Height);
			Left += Controls[i].ActualWidth();
		}
}

// =====================================================================================================================
// =====================================================================================================================
//  Notifications
// =====================================================================================================================
// =====================================================================================================================
function ResizedBoth()
{
	ExpandedHeight = RelativeHeight();
	Super.ResizedBoth();
}

function ResizedHeight()
{
	ExpandedHeight = RelativeHeight();
	Super.ResizedHeight();
}


defaultproperties
{
	ConstrictedHeight=0.32
	WinHeight=0.32
 	ExpandedHeight=0.5

	MinPageWidth=0.75
	MinPageHeight=0.32
	MaxPageWidth=0.9

	InactiveFadeColor=(R=255,B=255,G=255,A=255)
	bPersistent=True
	bAllowedAsLast=True
	bResizeHeightAllowed=False
	WindowName="Music Player"

	PlaylistEditorMenu="GUI2K4.StreamPlaylistEditor"
	ID3TagEditorMenu="GUI2K4.ID3TagEditor"

	PauseHint="Pause"
	PlayHint="Play"
	PlayImage=Material'2K4Menus.mp3.play'
	PauseImage=Material'2K4Menus.mp3.pause'
	OpenFolder=material'2k4menus.mp3.closeplaylist'
	ClosedFolder=material'2k4Menus.mp3.viewplaylist'


	ExpandManagementHint="Show Playlist Options"
	CollapseManagementHint="Hide Playlist Options"
	ExpandPlaylistHint="Show Current Playlist"
	CollapsePlaylistHint="Hide Current Playlist"
	// Components

	Begin Object Class=GUIContextMenu Name=SongRCMenu
		ContextItems(0)="Play Selected"
		ContextItems(1)="Open Playlist Editor"
		ContextItems(2)="-"
		ContextItems(3)="Remove From Playlist"
		ContextItems(4)="Clear Playlist"
		OnSelect=SongMenuClick
	End Object
	cm_Playlist=SongRCMenu

	Begin Object Class=GUIContextMenu Name=PlaylistRCMenu
		ContextItems(0)="Activate Playlist"
		ContextItems(1)="Rename Playlist"
		ContextItems(2)="Create new Playlist"
		ContextItems(3)="-"
		ContextItems(4)="Delete Playlist"
		ContextItems(5)="Clear Playlist"
		OnSelect=PlaylistMenuClick
	End Object
	cm_AllLists=PlaylistRCMenu

	Begin Object Class=GUIContextMenu Name=PlayerRCMenu
		ContextItems(0)="Change Settings"
		OnSelect=PlayerMenuClick
	End Object
	ContextMenu=PlayerRCMenu

	Begin Object Class=GUIScrollTextbox Name=SongInfoBox
		WinWidth=0.2
		WinHeight=0.2
		WinLeft=0
		WinTop=0
		bTabStop=False
		FontScale=FNS_Medium
		bNoTeleType=True
		bNeverFocus=True
		bAcceptsInput=False
	End Object
	lb_SongInfo=SongInfoBox

	Begin Object Class=GUILabel Name=SongTime
		WinWidth=0.241172
		WinHeight=0.550191
		WinLeft=0.2
		WinTop=-0.2
		Caption="0:00"
		StyleName="TextLabel"
		FontScale=FNS_Large
		TextAlign=TXTA_Center
		VertAlign=TXTA_Center
	End Object
	l_Time=SongTime

	Begin Object Class=GUISlider Name=StreamVolume
		WinWidth=0.544922
		WinHeight=1.000000
		WinLeft=0.0
		WinTop=0.000000
		MinValue=0.0
		MaxValue=1.0
		bIntSlider=False
		TabOrder=0
		Hint="Volume"
		OnChange=InternalOnChange
	End Object
	sl_Volume=StreamVolume

	Begin Object Class=GUIGFXButton Name=PrevTrackButton
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.542109
		WinTop=0.0
		OnClick=InternalOnClick
		Graphic=Material'2K4Menus.mp3.PreviousTrack'
		TabOrder=1
		bTabStop=True
		Position=ICP_Scaled
		Hint="Previous"
		bRepeatClick=False
		StyleName="TextLabel"
	End Object
	b_PrevTrack=PrevTrackButton

	Begin Object Class=GUIGFXButton Name=StopButton
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.605859
		WinTop=0.0
		OnClick=InternalOnClick
		Graphic=Material'2K4Menus.mp3.stop'
		TabOrder=2
		bTabStop=True
		Position=ICP_Scaled
		Hint="Stop"
		bRepeatClick=False
		StyleName="TextLabel"
	End Object
	b_Stop=StopButton

	Begin Object Class=GUIGFXButton Name=NextTrackButton
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.660860
		WinTop=0.0
		OnClick=InternalOnClick
		Graphic=Material'2K4Menus.mp3.NextTrack'
		TabOrder=3
		bTabStop=True
		Position=ICP_Scaled
		Hint="Next"
		bRepeatClick=False
		StyleName="TextLabel"
	End Object
	b_NextTrack=NextTrackButton

	Begin Object Class=GUIGFXButton Name=PlayButton
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.717110
		WinTop=0.0
		OnClick=InternalOnClick
		Graphic=Material'2K4Menus.mp3.Play'
		TabOrder=4
		bTabStop=True
		Position=ICP_Scaled
		bRepeatClick=False
		StyleName="TextLabel"
	End Object
	b_Play=PlayButton


	Begin Object Class=GUICheckboxButton Name=RepeatCheck
		Hint="Repeat"
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.76
		WinTop=0.666667
		OnChange=InternalOnChange
		TabOrder=0
		CheckedOverlay(0)=material'2k4menus.mp3.RepeatOnceWatched'
		CheckedOverlay(1)=material'2k4menus.mp3.RepeatOnceWatched'
		CheckedOverlay(2)=material'2k4menus.mp3.RepeatOnceWatched'
		CheckedOverlay(3)=material'2k4menus.mp3.RepeatOnceWatched'
		CheckedOverlay(4)=material'2k4menus.mp3.RepeatOnceWatched'
		CheckedOverlay(5)=material'2k4menus.mp3.RepeatOnce'
		CheckedOverlay(6)=material'2k4menus.mp3.RepeatOnce'
		CheckedOverlay(7)=material'2k4menus.mp3.RepeatOnce'
		CheckedOverlay(8)=material'2k4menus.mp3.RepeatOnce'
		CheckedOverlay(9)=material'2k4menus.mp3.RepeatOnce'
		bAllOverlay=true
	End Object
	ch_Repeat=RepeatCheck

	Begin Object Class=GUICheckboxButton Name=RepeatAllCheck
		Hint="Repeat All"
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.8
		WinTop=0.666667
		OnChange=InternalOnChange
		bAllOverlay=true
		TabOrder=1
		CheckedOverlay(0)=material'2k4menus.mp3.RepeatAllWatched'
		CheckedOverlay(1)=material'2k4menus.mp3.RepeatAllWatched'
		CheckedOverlay(2)=material'2k4menus.mp3.RepeatAllWatched'
		CheckedOverlay(3)=material'2k4menus.mp3.RepeatAllWatched'
		CheckedOverlay(4)=material'2k4menus.mp3.RepeatAllWatched'
		CheckedOverlay(5)=material'2k4menus.mp3.RepeatAll'
		CheckedOverlay(6)=material'2k4menus.mp3.RepeatAll'
		CheckedOverlay(7)=material'2k4menus.mp3.RepeatAll'
		CheckedOverlay(8)=material'2k4menus.mp3.RepeatAll'
		CheckedOverlay(9)=material'2k4menus.mp3.RepeatAll'
	End Object
	ch_RepeatAll=RepeatAllCheck

	Begin Object Class=GUICheckboxButton Name=ShuffleCheck
		Hint="Shuffle"
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.84
		WinTop=0.666667
		OnChange=InternalOnChange
		TabOrder=2
		bAllOverlay=true
		CheckedOverlay(0)=material'2k4menus.mp3.ShuffleOnceWatched'
		CheckedOverlay(1)=material'2k4menus.mp3.ShuffleOnceWatched'
		CheckedOverlay(2)=material'2k4menus.mp3.ShuffleOnceWatched'
		CheckedOverlay(3)=material'2k4menus.mp3.ShuffleOnceWatched'
		CheckedOverlay(4)=material'2k4menus.mp3.ShuffleOnceWatched'
		CheckedOverlay(5)=material'2k4menus.mp3.ShuffleOnce'
		CheckedOverlay(6)=material'2k4menus.mp3.ShuffleOnce'
		CheckedOverlay(7)=material'2k4menus.mp3.ShuffleOnce'
		CheckedOverlay(8)=material'2k4menus.mp3.ShuffleOnce'
		CheckedOverlay(9)=material'2k4menus.mp3.ShuffleOnce'
	End Object
	ch_Shuffle=ShuffleCheck

	Begin Object Class=GUICheckboxButton Name=ShuffleAllCheck
		Hint="Shuffle All"
		WinWidth=0.04
		WinHeight=0.04
		WinLeft=0.88
		WinTop=0.666667
		OnChange=InternalOnChange
		TabOrder=3
		bAllOverlay=true
		CheckedOverlay(0)=material'2k4menus.mp3.ShuffleAllWatched'
		CheckedOverlay(1)=material'2k4menus.mp3.ShuffleAllWatched'
		CheckedOverlay(2)=material'2k4menus.mp3.ShuffleAllWatched'
		CheckedOverlay(3)=material'2k4menus.mp3.ShuffleAllWatched'
		CheckedOverlay(4)=material'2k4menus.mp3.ShuffleAllWatched'
		CheckedOverlay(5)=material'2k4menus.mp3.ShuffleAll'
		CheckedOverlay(6)=material'2k4menus.mp3.ShuffleAll'
		CheckedOverlay(7)=material'2k4menus.mp3.ShuffleAll'
		CheckedOverlay(8)=material'2k4menus.mp3.ShuffleAll'
		CheckedOverlay(9)=material'2k4menus.mp3.ShuffleAll'
	End Object
	ch_ShuffleAll=ShuffleAllCheck

	Begin Object Class=GUIGFXButton Name=ShowPlaylistButton
		WinWidth=0.052422
		WinHeight=0.925000
		WinLeft=0.82547
		WinTop=0.000000
		OnClick=InternalOnClick
		Graphic=material'2k4menus.mp3.ViewPlayList'
		bTabStop=True
		TabOrder=5
		StyleName="RoundScaledButton"
		bRepeatClick=False
		OnDragOver=ManageDragOver
	End Object
	b_Playlist=ShowPlaylistButton

	Begin Object Class=GUIGFXButton Name=ManagePlaylistsButton
		WinWidth=0.052422
		WinHeight=0.925000
		WinLeft=0.883360
		WinTop=0.000000
		OnClick=InternalOnClick
		Graphic=material'2k4menus.mp3.add'
		bTabStop=True
		TabOrder=6
		StyleName="RoundScaledButton"
		bRepeatClick=False
		Hint="Manage Playlists"
		OnDragOver=ManageDragOver
	End Object
	b_Management=ManagePlaylistsButton

	Begin Object Class=GUIListBox Name=PlaylistListBox
		WinWidth=0.977425
		WinHeight=0.588941
		WinLeft=0.010859
		WinTop=0.300000
		bVisibleWhenEmpty=True
		bVisible=False
		TabOrder=6
		RenderWeight=0.503
		Hint="Current Playlist"
	End Object
	lb_Playlist=PlaylistListBox

	Begin Object Class=GUIListBox Name=SelectPlaylistListBox
		WinWidth=0.977425
		WinHeight=0.717500
		WinLeft=0.010859
		WinTop=0.235000
		bVisible=False
		TabOrder=1
		Hint="Select A Playlist"
		bBoundToParent=True
		bVisibleWhenEmpty=True
	End Object
	lb_AllPlaylists=SelectPlaylistListBox


	Begin Object class=GUIImage name=Img1
		Image=material'2k4menus.controls.display99'
		ImageStyle=ISTY_Stretched
		WinWidth=0.2
		WinHeight=0.2
		WinLeft=0.2
		WinTop=0.2
	End Object
	bk1=img1

	Begin Object class=GUIImage name=Img2
		Image=Material'2K4Menus.NewControls.NewSliderBar'
		ImageStyle=ISTY_Scaled
		WinWidth=0.2
		WinHeight=0.2
		WinLeft=0.2
		WinTop=0.2
	End Object
	bk2=img2

	Begin Object class=altSectionBackground name=sbManagement
		WinWidth=0.2
		WinHeight=0.2
		WinLeft=0.2
		WinTop=0.2
		Caption="Play List Manager"
		LeftPadding=0
		RightPadding=0
		bVisible=false
		bFillClient=true
	End Object
	sb_Management=sbManagement

	Begin Object class=altSectionBackground name=sbPlayList
		WinWidth=0.2
		WinHeight=0.2
		WinLeft=0.2
		WinTop=0.2
		Caption="Current Playlist"
		bFillClient=true
		bVisible=false
		LeftPadding=0
		RightPadding=0
	End Object
	sb_PlayList=sbPlayList

	Begin Object class=GUIButton name=bBotA
		Caption="Add"
		bVisible=false;
		StyleName="FooterButton"
		Tag=1
		OnClick=InternalOnClick
	End Object
	b_BotA=bBotA

	Begin Object class=GUIButton name=bBotB
		Caption="Remove"
		StyleName="FooterButton"
		OnClick=InternalOnClick
		bVisible=false;
		Tag=1
	End Object
	b_BotB=bBotB

	Begin Object class=GUIButton name=bBotC
		Caption="Clear"
		StyleName="FooterButton"
		bVisible=false;
		Tag=1
		OnClick=InternalOnClick
	End Object
	b_BotC=bBotC

	RenameCaption="Rename Playlist"
	NewCaption="Create New Playlist"
	PlaylistNameCaption="Name: "

	PLAdd="Add"
	PLRemove="Remove"
	PLClear="Clear"
	MGNew="New"
	MGRemove="Remove"
	MGRename="Rename"

}

