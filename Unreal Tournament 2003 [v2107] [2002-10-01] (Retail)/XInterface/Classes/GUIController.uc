// ====================================================================
//  Class:  Engine.GUIController
// 
//  The GUIController is a simple FILO menu stack.  You have 3 things
//  you can do.  You can Open a menu which adds the menu to the top of the
//  stack.  You can Replace a menu which replaces the current menu with the
//  new menu.  And you can close a menu, which returns you to the last menu
//  on the stack.  
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIController extends BaseGUIController
		Native;

#exec OBJ LOAD FILE=InterfaceContent.utx

cpptext
{
		void  NativeMessage(const FString Msg, FLOAT MsgLife);
		UBOOL NativeKeyType(BYTE& iKey, TCHAR Unicode );
		UBOOL NativeKeyEvent(BYTE& iKey, BYTE& State, FLOAT Delta );
		void  NativeTick(FLOAT DeltaTime);
		void  NativePreRender(UCanvas* Canvas);
		void  NativePostRender(UCanvas* Canvas);

		virtual void LookUnderCursor(FLOAT dX, FLOAT dY);
		UGUIComponent* UnderCursor(FLOAT MouseX, FLOAT MouseY);
		
		UBOOL virtual MousePressed(UBOOL IsRepeat);								
		UBOOL virtual MouseReleased();
		
		UBOOL HasMouseMoved();								

		void PlayInterfaceSound(USound* sound);
		void PlayClickSound(BYTE SoundNum);
}

		
var	editinline export	array<GUIPage>		MenuStack;			// Holds the stack of menus
var						GUIPage				ActivePage;			// Points to the currently active page
var editinline 			Array<GUIFont>		FontStack;			// Holds all the possible fonts
var 					Array<GUIStyles>	StyleStack;			// Holds all of the possible styles
var						Array<string>		StyleNames;			// Holds the name of all styles to use					
var editinline 			Array<Material>		MouseCursors;		// Holds a list of all possible mouse
var editinline			Array<vector>		MouseCursorOffset;  // Only X,Y used, between 0 and 1. 'Hot Spot' of cursor material.
var						Array<GUIPage>		PersistentStack;	// Holds the set of pages which are persistent across close/open

var						int					MouseX,MouseY;		// Where is the mouse currently located
var						int					LastMouseX, LastMouseY;
var						bool				ShiftPressed;		// Shift key is being held
var						bool				AltPressed;			// Alt key is being held
var						bool				CtrlPressed;		// Ctrl key is being held


var						float				DblClickWindow;			// How long do you have for a double click
var						float				LastClickTime;			// When did the last click occur
var						int					LastClickX,LastClickY;	// Who was the active component

var						float				ButtonRepeatDelay;		// The amount of delay for faking button repeats
var						byte				RepeatKey;				// Used to determine what should repeat
var						float				RepeatDelta; 			// Data var
var						float				RepeatTime;				// How long until the next repeat;
var						float				CursorFade;				// How visible is the cursor
var						int					CursorStep;				// Are we fading in or out

var						float				FastCursorFade;			// How visible is the cursor
var						int					FastCursorStep;			// Are we fading in or out

var						GUIComponent		FocusedControl;			// Top most Focused control
var						GUIComponent 		ActiveControl;			// Which control is currently active
var						GUIComponent		SkipControl;			// This control should be skipped over and drawn at the end
var						GUIComponent		MoveControl;			// Used for visual design

var						bool				bIgnoreNextRelease;				// Used to make sure discard errant releases.

var config				bool 				bModAuthor;				// Allows bDesign Mode
var 					bool				bDesignMode;			// Are we in design mode;
var						bool				bHighlightCurrent;		// Highlight the current control being edited


var						bool				bCurMenuInitialized;	// Has the current Menu Finished initialization

var						string				GameResolution;
var config				float				MenuMouseSens;									

var						bool				MainNotWanted;			// Set to true if you don't want main to appear.

// Sounds
var						sound				MouseOverSound;
var						sound				ClickSound;
var						sound				EditSound;
var						sound				UpSound;
var						sound				DownSound;

var						bool				bForceMouseCheck;		// HACK
var						bool				bIgnoreUntilPress;		// HACK

// Temporary for Design Mode
var Material WhiteBorder;

native event GUIFont GetMenuFont(string FontName); 	// Finds a given font in the FontStack
native event GUIStyles GetStyle(string StyleName); 	// Find a style on the stack
native function string GetCurrentRes();				// Returns the current res as a string
native function string GetMainMenuClass();			// Returns GameEngine.MainMenuClass

// Utility functions for the UI

native function GetTeamSymbolList(array<string> SymbolNames, optional bool bNoSinglePlayer);
native function GetWeaponList(out array<class<Weapon> > WeaponClass, out array<string> WeaponDesc);
native function GetMapList(string Prefix, GUIList list);
native function GetProfileList(string Prefix, out array<string> ProfileList);
native function string LoadDecoText(string PackageName, string DecoTextName); 

native function ResetKeyboard();


delegate bool OnNeedRawKeyPress(byte NewKey);

// ================================================
// CreateMenu - Attempts to Create a menu.  Returns none if it can't

event GUIPage CreateMenu(string NewMenuName)
{ 
	local class<GUIPage> NewMenuClass;
	local GUIPage NewMenu;
	local int i;

	// Load the menu's package if needed

	NewMenuClass = class<GUIPage>(DynamicLoadObject(NewMenuName,class'class'));
	if (NewMenuClass != None)
	{
		// If it's persistent, try to find an instance in the PersistentStack.
		if( NewMenuClass.default.bPersistent )
		{
			for( i=0;i<PersistentStack.Length;i++ )
				if( PersistentStack[i].Class == NewMenuClass )
				{
					NewMenu = PersistentStack[i];
					break;
				}      
		}

		// Not found, spawn a new menu
		if( NewMenu == None )
		{
			NewMenu = new NewMenuClass;
		
			// Check for errors
			if (NewMenu == None)
			{
				log("Could not create requested menu"@NewMenuName);
				return None;
			}
			else
			if( NewMenuClass.default.bPersistent )
			{
				// Save in PersistentStack if it's persistent.
				i = PersistentStack.Length;
				PersistentStack.Length = i+1;
				PersistentStack[i] = NewMenu;
			}
		}

		return NewMenu;
	}
	else
	{
		log("Could not DLO menu '"$NewMenuName$"'");
		return none;
	}
	
}
// ================================================
// OpenMenu - Opens a new menu and places it on top of the stack



event bool OpenMenu(string NewMenuName, optional string Param1, optional string Param2)				
{
	local GUIPage NewMenu,CurMenu;

	// Sanity Check

	log("GUIController::OpenMenu - Attempt to open menu ["$NewMenuName$"]");
	log("GUIController::MenuMouseSens="$MenuMouseSens);
	
	NewMenu = CreateMenu(NewMenuName);
	bCurMenuInitialized=false;	
	if (NewMenu!=None)
	{
	
		CurMenu = ActivePage;

		NewMenu.ParentPage = CurMenu;
		
		// Add this menu to the stack and give it focus
		
		MenuStack.Length = MenuStack.Length+1;
		MenuStack[MenuStack.Length-1] = NewMenu;

		ActivePage = NewMenu;

		ResetFocus();
		
		// Initialize this Menu
		
		NewMenu.InitComponent(Self, none);
		
		// Remove focus from the last menu

		if (CurMenu!=None)
		{
			CurMenu.MenuState = MSAT_Blurry;
			CurMenu.OnDeActivate();
		}

		NewMenu.CheckResolution(false);
		NewMenu.OnOpen();	// Pass along the event
		NewMenu.MenuState = MSAT_Focused;
		NewMenu.OnActivate();
		NewMenu.PlayOpenSound();

		SetControllerStatus(true);
		bCurMenuInitialized=true;

		NewMenu.HandleParameters(Param1, Param2);
		
		bForceMouseCheck = true;
		
		return true;
	}
	else
	{
		log("Could not open menu"@NewMenuName);
		return false;
	}		
}

// ================================================
// Replaces a menu in the stack.  returns true if success

event bool ReplaceMenu(string NewMenuName, optional string Param1, optional string Param2)
{
	local GUIPage NewMenu,CurMenu;

	NewMenu = CreateMenu(NewMenuName);
	bCurMenuInitialized=false;
	if (NewMenu!=None)
	{
		CurMenu = ActivePage;

		// Add this menu to the stack and give it focus
		
		NewMenu.MenuState = MSAT_Focused;
		
		if (CurMenu==None)
			MenuStack.Length = MenuStack.Length+1;

		MenuStack[MenuStack.Length-1] = NewMenu;
		ActivePage = NewMenu;
		NewMenu.ParentPage = CurMenu.ParentPage;
		
		ResetFocus();

		NewMenu.InitComponent(Self, None); 
		
		NewMenu.CheckResolution(false);
		NewMenu.OnOpen();				// Pass along the event
		NewMenu.MenuState = MSAT_Focused;
		NewMenu.OnActivate();
		NewMenu.PlayOpenSound();

		SetControllerStatus(true);
		bCurMenuInitialized=true;
		
		NewMenu.HandleParameters(Param1, Param2);

		bForceMouseCheck = true;
				
		return true;
	}
	else
		return false;
}
event bool CloseMenu(optional bool bCanceled)	// Close the top menu.  returns true if success.
{

	local GUIPage CurMenu;
	local int 	  CurIndex;

	if (MenuStack.Length <= 0)
	{
		log("Attempting to close a non-existing menu page");
		return false;
	}

	
	
	CurIndex = MenuStack.Length-1;
	CurMenu = MenuStack[CurIndex];

	log("GUIController::CloseMenu - "@CurMenu);
	
	// Remove the menu from the stack
	MenuStack.Remove(MenuStack.Length-1,1);

	// Look for the resolution switch
	
	CurMenu.PlayCloseSound();		// Play the closing sound
	CurMenu.OnClose(bCanceled);

	CurMenu.MenuOwner=None;
	CurMenu.Controller=None;
	
	MoveControl = None;
	SkipControl = None;
	
	// Gab the next page on the stack
	bCurMenuInitialized=false;	
	if (MenuStack.Length>0)	// Pass control back to the previous menu
	{
		ActivePage = MenuStack[MenuStack.Length-1];
		ActivePage.MenuState = MSAT_Focused;
		ActivePage.CheckResolution(true);	

		ActivePage.OnReOpen();
		ActivePage.OnActivate();
		
		ActiveControl = none;
		
		ActivePage.FocusFirst(None,true);
	}
	else
	{
	
		if (!CurMenu.bAllowedAsLast)
		{
			OpenMenu(GetMainMenuClass());
			return true;
		}
				
		ActivePage = None;
 		SetControllerStatus(false);
	}
	
	bCurMenuInitialized=true;	

	bForceMouseCheck = true;
	
	return true;	
}

function GUIPage TopPage()
{
	return ActivePage;
}

function SetControllerStatus(bool On)
{

	Log("GUIController::SetControllerStatus:"@On);

	bActive = On;
	bVisible = On;
	bRequiresTick=On;

	// Attempt to Pause as well as show the windows mouse cursor.
	
//	ViewportOwner.Actor.Level.Game.SetPause(On, ViewportOwner.Actor);		
	ViewportOwner.bShowWindowsMouse=On;

	// Add code to pause/unpause/hide/etc the game here.

	if (On)
		bIgnoreUntilPress = true;
	
}


event CloseAll(bool bCancel)
{
	local int i;
	
	log("GUIController::CloseAll "@GameResolution);
	
	// Close the current menu manually before we clean up the stack.
	if( MenuStack.Length >= 0 )
	{
		if ( !CloseMenu(bCancel) )
			return;
	}
	
	for (i=0;i<MenuStack.Length;i++)
	{
		MenuStack[i].CheckResolution(true);
		MenuStack[i].Controller = None;
		MenuStack[i] = None;
	}

	log("GUIController::CloseAll - After Menu closing "@GameResolution);
	
	if (GameResolution!="")
	{
		ViewportOwner.Actor.ConsoleCommand("SETRES"@GameResolution);
		GameResolution="";
	}
		
	
	MenuStack.Remove(0,MenuStack.Length);
	SetControllerStatus(false);
	
	log("GUIController::CloseAll - All Closed!");
	
}

event InitializeController()
{
	local int i;
	local class<GUIStyles> NewStyleClass;

	for (i=0;i<StyleNames.Length;i++)
	{
		NewStyleClass = class<GUIStyles>(DynamicLoadObject(StyleNames[i],class'class'));
		if (NewStyleClass != None)
			if (!RegisterStyle(NewStyleClass))
				log("Could not create requested style"@StyleNames[i]);

	}		
}

function bool RegisterStyle(class<GUIStyles> StyleClass)
{
local GUIStyles NewStyle;

	if (StyleClass != None && !StyleClass.default.bRegistered)
	{
		NewStyle = new StyleClass;

		// Check for errors

		if (NewStyle != None)
		{
			// Dynamic Array Auto Sizes StyleStack.
			StyleStack[StyleStack.Length] = NewStyle;
			NewStyle.Controller = self;
			NewStyle.Initialize();
			return true;
		}
	}
	return false;
}

event ChangeFocus(GUIComponent Who)
{
	return;
}
		
function ResetFocus()
{

	if (ActiveControl!=None)
	{
		ActiveControl.MenuStateChange(MSAT_Blurry);
		ActiveControl=None;
	}

	RepeatKey=0;
	RepeatTime=0;
		
}

event MoveFocused(GUIComponent Ctrl, int bmLeft, int bmTop, int bmWidth, int bmHeight, float ClipX, float ClipY)
{
	local float val;
	
	
	if (AltPressed)
		val = 5;
	else
		val = 1;
	
	if (bmLeft!=0)
	{
		if (Ctrl.WinLeft<1)
			Ctrl.WinLeft = Ctrl.WinLeft + ( (Val/ClipX) * bmLeft);
		else
			Ctrl.WinLeft += (Val*bmLeft);
	}
	
	if (bmTop!=0) 
	{
		if (Ctrl.WinTop<1)
			Ctrl.WinTop = Ctrl.WinTop + ( (Val/ClipY) * bmTop);
		else
			Ctrl.WinTop+= (Val*bmTop);
	}
	
	if (bmWidth!=0)
	{		
		if (Ctrl.WinWidth<1)
			Ctrl.WinWidth = Ctrl.WinWidth + ( (Val/ClipX) * bmWidth);
		else
			Ctrl.WinWidth += (Val*bmWidth);
	}	
			
	if (bmHeight!=0)
	{		
		if (Ctrl.WinHeight<1)
			Ctrl.WinHeight = Ctrl.WinHeight + ( (Val/ClipX) * bmHeight);
		else
			Ctrl.WinHeight += (Val*bmHeight);
	}
}	

function bool HasMouseMoved()
{
	if (MouseX==LastMouseX && MouseY==LastMouseY)
		return false;
	else
		return true;
}

event bool NeedsMenuResolution()
{

	if ( (ActivePage!=None) && (ActivePage.bRequire640x480) )
		return true;
	else
		return false;
}

event SetRequiredGameResolution(string GameRes)
{
	GameResolution = GameRes;
}

defaultproperties
{
	Begin Object Class=UT2MenuFont Name=GUIMenuFont
		bFixedSize=false
		Name="UT2MenuFont"
	End Object
	FontStack(0)=UT2MenuFont'GUIMenuFont'

	Begin Object Class=UT2DefaultFont Name=GUIDefaultFont
		bFixedSize=true
		Name="GUIDefaultFont"
	End Object
	FontStack(1)=UT2DefaultFont'GUIDefaultFont'

	Begin Object Class=UT2LargeFont Name=GUILargeFont
		bFixedSize=false
		Name="GUILargeFont"
	End Object
	FontStack(2)=UT2LargeFont'GUILargeFont'
	
	Begin Object Class=UT2HeaderFont Name=GUIHeaderFont
		bFixedSize=false
		Name="GUIHeaderFont"
	End Object
	FontStack(3)=UT2HeaderFont'GUIHeaderFont'
	
	Begin Object Class=UT2SmallFont Name=GUISmallFont
		bFixedSize=false
		Name="GUISmallFont"
	End Object
	FontStack(4)=UT2SmallFont'GUISmallFont'
	
	Begin Object Class=UT2MidGameFont Name=GUIMidGameFont
		bFixedSize=false
		Name="GUIMidGameFont"
	End Object
	FontStack(5)=UT2MidGameFont'GUIMidGameFont'
	
	Begin Object Class=UT2SmallHeaderFont Name=GUISmallHeaderFont
		bFixedSize=false
		Name="GUISmallHeaderFont"
	End Object
	FontStack(6)=UT2SmallHeaderFont'GUISmallHeaderFont'

	Begin Object Class=UT2ServerListFont Name=GUIServerListFont
		bFixedSize=false
		Name="GUIServerListFont"
	End Object
	FontStack(7)=UT2ServerListFont'GUIServerListFont'

	StyleNames(0)="xinterface.STY_RoundButton"
	StyleNames(1)="xinterface.STY_RoundScaledButton"
	StyleNames(2)="xinterface.STY_SquareButton"
	StyleNames(3)="xinterface.STY_ListBox"
	StyleNames(4)="xinterface.STY_ScrollZone"
	StyleNames(5)="xinterface.STY_TextButton"
	StyleNames(6)="xinterface.STY_Page"
	StyleNames(7)="xinterface.STY_Header"
	StyleNames(8)="xinterface.STY_Footer"
	StyleNames(9)="xinterface.STY_TabButton"
	StyleNames(10)="xinterface.STY_CharButton"
	StyleNames(11)="xinterface.STY_ArrowLeft"
	StyleNames(12)="xinterface.STY_ArrowRight"
	StyleNames(13)="xinterface.STY_ServerBrowserGrid"
	StyleNames(14)="xinterface.STY_NoBackground"
	StyleNames(15)="xinterface.STY_ServerBrowserGridHeader"
	StyleNames(16)="xinterface.STY_SliderCaption"
	StyleNames(17)="xinterface.STY_LadderButton"
	StyleNames(18)="xinterface.STY_LadderButtonHi"
	StyleNames(19)="XInterface.STY_LadderButtonActive"
	StyleNames(20)="xinterface.STY_BindBox"
	StyleNames(21)="xinterface.STY_SquareBar"
	StyleNames(22)="xinterface.STY_MidGameButton"
	StyleNames(23)="xinterface.STY_TextLabel"
	StyleNames(24)="xinterface.STY_ComboListBox"
	StyleNames(25)="xinterface.STY_SquareMenuButton"
	
	// See Player.uc IDC_ definitions
	MouseCursors(0)=material'InterfaceContent.MouseCursor'			// Arrow
	MouseCursors(1)=material'InterfaceContent.SplitterCursor'		// SizeAll
	MouseCursors(2)=material'InterfaceContent.SplitterCursor'		// Size NE SW
	MouseCursors(3)=material'InterfaceContent.SplitterCursorVert'	// Size NS
	MouseCursors(4)=material'InterfaceContent.SplitterCursor'		// Size NW SE
	MouseCursors(5)=material'InterfaceContent.SplitterCursor'		// Size WE
	MouseCursors(6)=material'InterfaceContent.MouseCursor'			// Wait

	MouseCursorOffset(0)=(X=0,Y=0,Z=0)
	MouseCursorOffset(1)=(X=0.5,Y=0.5,Z=0)
	MouseCursorOffset(2)=(X=0.5,Y=0.5,Z=0)
	MouseCursorOffset(3)=(X=0.5,Y=0.5,Z=0)
	MouseCursorOffset(4)=(X=0.5,Y=0.5,Z=0)
	MouseCursorOffset(5)=(X=0.5,Y=0.5,Z=0)
	MouseCursorOffset(6)=(X=0,Y=0,Z=0)


	ButtonRepeatDelay=0.25
	CursorStep=1
	FastCursorStep=1
	// Design Mode stuff
	WhiteBorder=Material'InterfaceContent.Menu.WhiteBorder'
 	DblClickWindow=0.5

	MouseOverSound=sound'MenuSounds.MS_MouseOver'
	ClickSound=sound'MenuSounds.MS_Click'
	EditSound=sound'MenuSounds.MS_Edit'
	UpSound=sound'MenuSounds.MS_ListChangeUp'
	DownSound=sound'MenuSounds.MS_ListChangeDown'
	
	MenuMouseSens=1.0	
	bHighlightCurrent=true
}