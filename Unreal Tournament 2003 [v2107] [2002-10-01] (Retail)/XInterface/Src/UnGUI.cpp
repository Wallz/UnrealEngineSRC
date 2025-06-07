/*=============================================================================
	UnGUI.cpp: See .UC for for info
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Joe Wilcox
=============================================================================*/

#include "XInterface.h"

IMPLEMENT_CLASS(UGUIController);
IMPLEMENT_CLASS(UGUI);
IMPLEMENT_CLASS(UGUIComponent);
IMPLEMENT_CLASS(UGUIImage);
IMPLEMENT_CLASS(UGUIPage);
IMPLEMENT_CLASS(UGUIFont);
IMPLEMENT_CLASS(UGUILabel);
IMPLEMENT_CLASS(UGUIButton);
IMPLEMENT_CLASS(UGUIGFXButton);
IMPLEMENT_CLASS(UGUISpinnerButton);
IMPLEMENT_CLASS(UGUICheckBoxButton);
IMPLEMENT_CLASS(UGUIEditBox);
IMPLEMENT_CLASS(UGUINumericEdit);
IMPLEMENT_CLASS(UGUIMultiComponent);
IMPLEMENT_CLASS(UGUIScrollBarBase);
IMPLEMENT_CLASS(UGUIVertScrollZone);
IMPLEMENT_CLASS(UGUIVertScrollButton);
IMPLEMENT_CLASS(UGUIVertGripButton);
IMPLEMENT_CLASS(UGUIVertScrollBar);
IMPLEMENT_CLASS(UGUIHorzScrollZone);
IMPLEMENT_CLASS(UGUIHorzScrollButton);
IMPLEMENT_CLASS(UGUIHorzGripButton);
IMPLEMENT_CLASS(UGUIHorzScrollBar);
IMPLEMENT_CLASS(UGUIListBoxBase);
IMPLEMENT_CLASS(UGUIListBox);
IMPLEMENT_CLASS(UGUIMultiColumnListBox);
IMPLEMENT_CLASS(UGUIPanel);
IMPLEMENT_CLASS(UGUIListBase);
IMPLEMENT_CLASS(UGUIList);
IMPLEMENT_CLASS(UGUIMultiColumnList);
IMPLEMENT_CLASS(UGUIMultiColumnListHeader);
IMPLEMENT_CLASS(UGUIStyles);
IMPLEMENT_CLASS(UGUITabControl);
IMPLEMENT_CLASS(UGUITabButton);
IMPLEMENT_CLASS(UGUITabPanel);
IMPLEMENT_CLASS(UGUIComboBox);
IMPLEMENT_CLASS(UGUIMenuOption);
IMPLEMENT_CLASS(UGUITitleBar);
IMPLEMENT_CLASS(UGUISplitter);
IMPLEMENT_CLASS(UGUISlider);
IMPLEMENT_CLASS(UGUIVertList);
IMPLEMENT_CLASS(UGUIHorzList);
IMPLEMENT_CLASS(UGUICircularList);
IMPLEMENT_CLASS(UGUICharacterList);
IMPLEMENT_CLASS(UGUIScrollText);
IMPLEMENT_CLASS(UGUIScrollTextBox);

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIController
// =======================================================================================================================================================
// =======================================================================================================================================================

void  UGUIController::NativeMessage(const FString Msg, FLOAT MsgLife)
{
}

// NativeTick is responsible for faking mouse button repeats and setting the fade value for
// the text cursor.  

void  UGUIController::NativeTick(FLOAT DeltaTime)
{

	if ( (ViewportOwner==NULL) || (ViewportOwner->Actor==NULL) || (ViewportOwner->Actor->Level==NULL) )
		return;

	// Use real seconds for TimeBased
	// TODO: Check if other/all UInteraction need to use RealTime
	DeltaTime /= ViewportOwner->Actor->Level->TimeDilation;

	// Update all of the button arrays
	if (RepeatKey)
	{
		RepeatTime-=DeltaTime;
		if (RepeatTime<=0)
		{
			BYTE Hold=IST_Hold;
			NativeKeyEvent(RepeatKey, Hold, RepeatDelta);
			RepeatTime=ButtonRepeatDelay;
		}
	}

	CursorFade+= 255 * (DeltaTime) * CursorStep;
	if (CursorFade<=0.0)
	{
		CursorFade=0;
		CursorStep = 1;
	}
	else if (CursorFade>=255.0)
	{
		CursorFade=255;
		CursorStep = -1;
	}

	FastCursorFade+= 8192 * (DeltaTime) * FastCursorStep;
	if (FastCursorFade<=0.0)
	{
		FastCursorFade=0;
		FastCursorStep = 1;
	}
	else if (FastCursorFade>=255.0)
	{
		FastCursorFade=255;
		FastCursorStep = -1;
	}

	if (ActivePage != NULL)
		ActivePage->UpdateTimers( DeltaTime );
}

// Pre Render is used for sizing/positioning

void  UGUIController::NativePreRender(UCanvas* Canvas)
{

	guard(GUIController::NativePreRender);

	if ( !Canvas || !Master || !Master->Client || Master->Client->Viewports.Num()==0 || !Master->Client->Viewports(0) )
		return;

	// If we are using the Windows Mouse.. pass it along before any rendering

	if (Master->Client->Viewports(0)->bWindowsMouseAvailable)
	{

		LastMouseX = MouseX;
		LastMouseY = MouseY;

		MouseX = Master->Client->Viewports(0)->WindowsMouseX;
		MouseY = Master->Client->Viewports(0)->WindowsMouseY;
		Master->Client->Viewports(0)->bShowWindowsMouse = 1;
	}
	else
		Master->Client->Viewports(0)->bShowWindowsMouse = 0;

	// Move through the stack and Draw each Page.  Each page will then itterate though all of
	// it's components and render them

	for (INT i=0;i < MenuStack.Num(); i++)
	{
		if (MenuStack(i)!=NULL)
			MenuStack(i)->PreDraw(Canvas);
	}

	unguard;

}

// NativePostRender passes render control to each component.  It starts with all components
// on the Active page, and then continues until all components are rendered

void  UGUIController::NativePostRender(UCanvas* Canvas)
{

	guard(GUIController::NativePostRender);

	if ( !Canvas || !Master || !Master->Client || Master->Client->Viewports.Num()==0 || !Master->Client->Viewports(0) )
		return;

	Canvas->CurX = 0;
	Canvas->ColorModulate = FPlane (1.0, 1.0, 1.0, 1.0);	// Fix thier hack
	Canvas->Style=1;

	// Move through the stack and Draw each Page.  Each page will then itterate though all of
	// it's components and render them

	for (INT i=0;i < MenuStack.Num(); i++)
	{
		if (MenuStack(i)==NULL)
		{
			GWarn->Logf(NAME_Warning, TEXT("MenuStack out of sync"));
			break;
		}

		if (MenuStack(i)==ActivePage)
			Canvas->ColorModulate = FPlane (1.0, 1.0, 1.0, 1.0);
		else
			Canvas->ColorModulate = MenuStack(i)->InactiveFadeColor;
		
		MenuStack(i)->Draw(Canvas);
	}

	// Render the Mouse Cursor if we are not using the windows cusor.  Otherwise set the cursor index

	if (!Master->Client->Viewports(0)->bWindowsMouseAvailable)
	{
		
		UMaterial* MouseMat;
		FVector MouseOffset;

		if (ActiveControl!=NULL)
		{
			MouseMat = MouseCursors(ActiveControl->MouseCursorIndex);
			MouseOffset = MouseCursorOffset(ActiveControl->MouseCursorIndex);
		}
		else
		{
			MouseMat = MouseCursors(0);
			MouseOffset = MouseCursorOffset(0);
		}

		if (MouseMat!=NULL)
		{

			FPlane OldColorModulate;

			Canvas->Color = FColor(255,255,255,255);
			Canvas->Style = STY_Alpha;

			OldColorModulate = Canvas->ColorModulate;
			Canvas->ColorModulate = FPlane (1.0, 1.0, 1.0, 1.0);

			FLOAT MouseScale = Canvas->SizeX / 640;

			if (Canvas->SizeX<640)
				MouseScale=1.0;

			INT MX = (INT)(Clamp<INT>(MouseX, 0, Canvas->SizeX));
			INT MY = (INT)(Clamp<INT>(MouseY, 0, Canvas->SizeY));

			FLOAT MouseSizeX = MouseMat->MaterialUSize()*MouseScale;
			FLOAT MouseSizeY = MouseMat->MaterialVSize()*MouseScale;

			INT DrawMouseX = MX - (MouseOffset.X * MouseSizeX);
			INT DrawMouseY = MY - (MouseOffset.Y * MouseSizeY);


			// Clamp the software mouse
			LastMouseX = MouseX;
			LastMouseY = MouseY;

			MouseX = MX;
			MouseY = MY;

			// If the Window's mouse is available, add code here to use it instead		
			
			Canvas->DrawTile
			(
				MouseMat,
				DrawMouseX,
				DrawMouseY,
				MouseSizeX,
				MouseSizeY,
				0,
				0,
				MouseMat->MaterialUSize(),
				MouseMat->MaterialVSize(),
				Canvas->Z,
				Canvas->Color.Plane(),
				FPlane(0.0f,0.0f,0.0f,0.0f)
			);

			Canvas->ColorModulate = OldColorModulate;
		}
	}
	else
	{
		if (ActiveControl!=NULL)
		{
			ViewportOwner->SelectedCursor = ActiveControl->MouseCursorIndex;
		}
	}

	if (bDesignMode)
	{
		Canvas->CurX = 0;
		Canvas->CurY = 0;

		UGUIFont* MyFont = eventGetMenuFont(TEXT("UT2SmallTextFont"));
		if (MyFont!=NULL)
		{
			UFont* CFont = MyFont->eventGetFont(800);
			if (CFont==NULL)	
				return;

			Canvas->Font = CFont;
		}

		INT XL,YL;
		Canvas->ClippedStrLen( Canvas->Font, 1.0, 1.0, XL, YL, TEXT("W") );

		if (MoveControl!=NULL && !ShiftPressed)
		{
			Canvas->DrawTile(DefaultPens[0], 0,0,Canvas->SizeX,YL,0,0,32,32, 0.0f, FColor(0,0,0,255),FPlane(0,0,0,0));				
			Canvas->DrawTextJustified(2,0,0,Canvas->SizeX,YL,TEXT("%s  :Move"),MoveControl->GetFullName());
			
			
			if (bHighlightCurrent)
			{
				// Draw the bounding rectangle
				Canvas->DrawTileStretched(WhiteBorder, MoveControl->Bounds[0], MoveControl->Bounds[1],
								MoveControl->Bounds[2]-MoveControl->Bounds[0], MoveControl->Bounds[3]-MoveControl->Bounds[1]);
			}
		}

		if (ShiftPressed)
			return;

		Canvas->DrawTile(DefaultPens[0], 0,YL,Canvas->SizeX,YL,0,0,32,32, 0.0f, FColor(0,0,0,255),FPlane(0,0,0,0));				
		if (FocusedControl!=NULL)
			Canvas->DrawTextJustified(0,0,YL,Canvas->SizeX,YL*2, TEXT("%s (%i) :focus"),FocusedControl->GetFullName(),FocusedControl->bCaptureMouse);
		else
			Canvas->DrawTextJustified(0,0,YL,Canvas->SizeX,YL*2,TEXT("None  :Focus"));


		Canvas->DrawTile(DefaultPens[0], 0,Canvas->SizeY-YL,Canvas->SizeX,YL,0,0,32,32, 0.0f, FColor(0,0,0,255),FPlane(0,0,0,0));				
		if (ActiveControl!=NULL)
			Canvas->DrawTextJustified(0,0,Canvas->SizeY-YL,Canvas->SizeX,Canvas->SizeY, TEXT("%s (%i) (%i)  :Active"), ActiveControl->GetFullName(),ActiveControl->MenuState,ActiveControl->bCaptureMouse);
		else
			Canvas->DrawTextJustified(0,0,Canvas->SizeY-YL,Canvas->SizeX,Canvas->SizeY,TEXT("None  :Active"));

	}

	unguard;

}

void UGUIController::execResetKeyboard( FFrame& Stack, RESULT_DECL )
{
	guard(UGUIController::execResetKeyboard);

	P_FINISH;

	UViewport* Viewport = Cast<UViewport>(ViewportOwner);

	if( Viewport && Viewport->Input )
		ResetConfig(Viewport->Input->GetClass());

	unguard;
}

void UGUIController::execLoadDecoText(FFrame& Stack, RESULT_DECL )
{
    guard( UGUIController::execLoadDecoText );

	P_GET_STR(PackageName);
	P_GET_STR(KeyName);
	P_FINISH

    TCHAR FileNames[2][512];
    TCHAR Buffer[16384];

	// Find the key:

	appSprintf( FileNames[0], TEXT("%s.%s"), *PackageName, UObject::GetLanguage() );
	appSprintf( FileNames[1], TEXT("%s.%s"), *PackageName, TEXT("int") );

	if( !GConfig->GetString( TEXT("DecoText"), *KeyName, Buffer, ARRAY_COUNT( Buffer), FileNames[0] ) &&
		!GConfig->GetString( TEXT("DecoText"), *KeyName, Buffer, ARRAY_COUNT( Buffer), FileNames[1] ) )
	{
		debugf( NAME_Error, TEXT("Couldn't load DecoText %s.%s"), *PackageName, *KeyName );
		*(FString*)Result = FString::Printf(TEXT(""));
		return;
	}

	if (appStrlen(Buffer)>0)
		*(FString*)Result = FString::Printf(TEXT("%s"), Buffer);
	else
		*(FString*)Result = FString::Printf(TEXT(""));

	return;

    unguard;

}


void UGUIController::execGetMapList(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController::execGetMapList);

	P_GET_STR(Prefix);
	P_GET_OBJECT(UGUIList, List);
	P_FINISH

	TCHAR Wildcard[256];
	appSprintf( Wildcard, TEXT("*.%s"), *FURL::DefaultMapExt );

	List->Elements.Empty();

	for( INT DoCD=0; DoCD<1+(GCdPath[0]!=0); DoCD++ )
	{
		for( INT i=0; i<GSys->Paths.Num(); i++ )
		{
			if( appStrstr( *GSys->Paths(i), Wildcard ) )
			{
				TCHAR Tmp[256]=TEXT("");
				if( DoCD )
				{
					appStrcat( Tmp, GCdPath );
					appStrcat( Tmp, TEXT("System") PATH_SEPARATOR );
				}
				appStrcat( Tmp, *GSys->Paths(i) ); 
				*appStrstr( Tmp, Wildcard )=0;
				appStrcat( Tmp, *Prefix );
				appStrcat( Tmp, Wildcard );
				TArray<FString>	TheseNames = GFileManager->FindFiles(Tmp,1,0);
				for( INT j=0; j<TheseNames.Num(); j++ )
				{
					INT k=List->Elements.AddZeroed();
					List->Elements(k).item = TheseNames(j).Left(TheseNames(j).Len()-4);
				}
			}
		}
	}

	// Update list size
	List->ItemCount = List->Elements.Num();

	unguard;
}

void UGUIController::execGetWeaponList(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController::execGetWeaponList);

	P_GET_TARRAY_REF(WeaponClass, UClass*);
	P_GET_TARRAY_REF(WeaponDesc, FString);
	P_FINISH

	WeaponClass->Empty();
	WeaponDesc->Empty();

	TArray<FRegistryObjectInfo> RegList;
	GetRegistryObjects( RegList, UClass::StaticClass(), AWeapon::StaticClass(), 0 );

	for(INT i=0; i<RegList.Num(); i++)
	{
		UClass* weapClass = (UClass*)(StaticLoadObject(UClass::StaticClass(), NULL, *(RegList(i).Object), NULL, LOAD_NoWarn | LOAD_Quiet, NULL ));

		if(!weapClass)
			continue;

		AWeapon* w = (AWeapon*)weapClass->GetDefaultObject();

		if(!w || w->bNotInPriorityList)
			continue;

		// Add class and description to output arrays
		WeaponClass->AddItem( weapClass );

		int k = WeaponDesc->AddZeroed();
		(*WeaponDesc)(k) = RegList(i).Description;
	}

	unguard;
}

void UGUIController::execGetTeamSymbolList(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController::execGetTeamSymbolList);

	P_GET_TARRAY_REF(SymbolNames, FString);
	P_GET_UBOOL_OPTX(bNoSinglePlayer, 0);
	P_FINISH

	SymbolNames->Empty();

	TArray<FRegistryObjectInfo> RegList;	
	GetRegistryObjects( RegList, UTexture::StaticClass(), NULL, 0 );
	
	for(INT i=0; i<RegList.Num(); i++)
	{
		FRegistryObjectInfo* Info = &RegList(i);

		// Only pick up 'TeamSymbol's
		if( appStrncmp(*Info->Object, TEXT("TeamSymbols_"), 12) != 0 )
			continue;

		// Ignore 'single player' symbols if desired
		if( bNoSinglePlayer && appStrstr( *Info->Description, TEXT("SP") ) )
			continue;

		// Add team symbol name to array
		int k = SymbolNames->AddZeroed();
		(*SymbolNames)(k) = Info->Object;
	}

	unguard;
}

// returns a list of profiles on the disk
void UGUIController::execGetProfileList(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController::execGetProfileList);

	P_GET_STR(Prefix);
	P_GET_TARRAY_REF(ProfileNames, FString);
	P_FINISH

	TCHAR Wildcard[256]; 
	appSprintf( Wildcard, TEXT("*.uvx") );  // defined in xDataObject.cpp

	ProfileNames->Empty();

	for( INT DoCD=0; DoCD<1+(GCdPath[0]!=0); DoCD++ )
	{
		for( INT i=0; i<GSys->Paths.Num(); i++ )
		{
			if( appStrstr( *GSys->Paths(i), Wildcard ) )
			{
				TCHAR Tmp[256]=TEXT("");
				if( DoCD )
				{
					appStrcat( Tmp, GCdPath );
					appStrcat( Tmp, TEXT("System") PATH_SEPARATOR );
				}
				appStrcat( Tmp, *GSys->Paths(i) ); 
				*appStrstr( Tmp, Wildcard )=0;
				appStrcat( Tmp, *Prefix );
				appStrcat( Tmp, Wildcard );
				TArray<FString>	TheseNames = GFileManager->FindFiles(Tmp,1,0);
				for( INT j=0; j<TheseNames.Num(); j++ )
				{
					INT k=ProfileNames->AddZeroed();
					(*ProfileNames)(k) = TheseNames(j).Left(TheseNames(j).Len()-4);
				}
			}
		}
	}

	unguard;
}

void UGUIController::execGetCurrentRes(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController::execGetCurrentRes);

	P_FINISH;

	if ( Master==NULL || Master->Client==NULL || Master->Client->Viewports.Num()==0 || Master->Client->Viewports(0)==NULL)
		*(FString*)Result = FString::Printf(TEXT(""));
	else
		*(FString*)Result = FString::Printf(TEXT("%ix%i"), Master->Client->Viewports(0)->SizeX,Master->Client->Viewports(0)->SizeY);

	unguard;
}

void UGUIController::execGetMainMenuClass(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController::execMainMenuClass);

	P_FINISH;

	*(FString*)Result = FString::Printf(TEXT("Xinterface.UT2MainMenu"));//, *Cast<UGameEngine>(Master->Client->Engine)->MainMenuClass );

	unguard;
}
void UGUIController::execGetMenuFont(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController::execGetMenuFont);

	P_GET_STR(FontName);
	P_FINISH;

	if (FontStack.Num()==0)
	{
		*(UObject**) Result = NULL;
		return;
	}

	for (INT i=0;i<FontStack.Num();i++)
	{
		if ( FontName == FontStack(i)->KeyName )
		{
			*(UObject**) Result = FontStack(i);
			return;
		}
	}

	*(UObject**) Result = NULL;

	unguard;
}

void UGUIController::execGetStyle(FFrame& Stack, RESULT_DECL )
{
	guard(GUIController:execGetStyle);

	P_GET_STR(StyleName);
	P_FINISH;

	if (StyleStack.Num()==0)
	{
		*(UObject**) Result = NULL;
		return;
	}

	for (INT i=0;i<StyleStack.Num();i++)
	{
		if ( StyleName == StyleStack(i)->KeyName )
		{
			*(UObject**) Result = StyleStack(i);
			return;
		}
	}

	*(UObject**) Result = NULL;

	unguard;
}

// -- NOTE: The UGUIController always swallows input when it's active;

UBOOL UGUIController::NativeKeyEvent(BYTE& iKey, BYTE& State, FLOAT Delta )
{

	if (Master==NULL || Master->Client==NULL || Master->Client->Viewports.Num()==0 || Master->Client->Viewports(0)==NULL)
		return false;

	FLOAT ClipX = Master->Client->Viewports(0)->SizeX;
	FLOAT ClipY = Master->Client->Viewports(0)->SizeY;

	if ( DELEGATE_IS_SET(OnNeedRawKeyPress) )
	{
		if (State==IST_Release)
			delegateOnNeedRawKeyPress(iKey);

		return true;
	}

	if (iKey==IK_Enter && AltPressed)
	{
		return true;
	}

	// Do we need to switch to the console?

	if (ViewportOwner!=NULL && ViewportOwner->Console!=NULL)
	{

		BYTE ConsoleHotKey = Cast<UConsole>(ViewportOwner->Console)->ConsoleHotKey;

		if (iKey==ConsoleHotKey && State==IST_Release)
		{
			if (Cast<UConsole>(ViewportOwner->Console) != NULL)
				Cast<UConsole>(ViewportOwner->Console)->eventNativeConsoleOpen();
			
			return true;
		}
	
	}

	// The UGUI control also tracks Shift, Ctrl, and Alt but doesn't swallow them

	if (iKey==IK_Shift)
	{
		ShiftPressed = State==IST_Press;
	}

	if (iKey==IK_Alt)
	{
		AltPressed = State==IST_Press;
	}

	if (iKey==IK_Ctrl)
	{
		CtrlPressed = State==IST_Press;
	}

	// If we are in design mode, allow components to be moved around

	if (bModAuthor && CtrlPressed && AltPressed && iKey==IK_D && State==IST_Press)
	{
		bDesignMode = !bDesignMode;
		MoveControl = NULL;
	}

	if (bDesignMode)
	{
		if ( CtrlPressed && iKey==IK_H && State==IST_Release )
			bHighlightCurrent = !bHighlightCurrent;
		
		if ( (CtrlPressed) && (iKey==IK_LeftMouse) && (State==IST_Press) && (ActivePage!=NULL) )
		{
			UGUIComponent *OldMoveCtrl = MoveControl;

			// Clear MoveControl if not in Bounds anymore.
			if (MoveControl != NULL)
			{

				MoveControl = NULL;
				OldMoveCtrl->SpecialHit();
				if (MoveControl != OldMoveCtrl)
					MoveControl = NULL;
			}

			// Search for the next Control
			if (!ActivePage->SpecialHit() || MoveControl == OldMoveCtrl)	// Find the component under the cursor, special
				MoveControl=NULL;
			return true;
		}

		if ( (MoveControl!=NULL) && (CtrlPressed) && (State==IST_Press) )
		{
			// Handle positioning

			if		(iKey==IK_Left)		{ eventMoveFocused(MoveControl, -1,0,0,0,ClipX,ClipY);	return true; }
			else if (iKey==IK_Right)	{ eventMoveFocused(MoveControl, 1,0,0,0,ClipX,ClipY);	return true; }
			else if (iKey==IK_Up)		{ eventMoveFocused(MoveControl, 0,-1,0,0,ClipX,ClipY);	return true; }
			else if (iKey==IK_Down)		{ eventMoveFocused(MoveControl, 0,1,0,0,ClipX,ClipY);	return true; }
			else if (iKey==IK_GreyPlus)	{ eventMoveFocused(MoveControl, 0,0,1,0,ClipX,ClipY);	return true; }
			else if (iKey==IK_GreyMinus){ eventMoveFocused(MoveControl, 0,0,-1,0,ClipX,ClipY);	return true; }
			else if (iKey==IK_Equals)	{ eventMoveFocused(MoveControl, 0,0,0,1,ClipX,ClipY);	return true; }
			else if (iKey==IK_Minus)	{ eventMoveFocused(MoveControl, 0,0,0,-1,ClipX,ClipY);	return true; }
			else if (iKey==IK_MouseWheelUp)
			{
				if (MoveControl->MenuOwner && MoveControl != ActivePage)
						MoveControl = MoveControl->MenuOwner;
			}
		}

		if ( ( CtrlPressed) && (iKey==IK_C) )
			appClipboardCopy( *(FString::Printf(TEXT("\t\tWinWidth=%f\n\t\tWinHeight=%f\n\t\tWinLeft=%f\n\t\tWinTop=%f\n"), MoveControl->WinWidth, MoveControl->WinHeight, MoveControl->WinLeft, MoveControl->WinTop)));
	}

	// The UGUIControl automatically tracks Mouse X/Y changes

	if ( (iKey==IK_MouseX) || (iKey==IK_MouseY) || bForceMouseCheck )
	{

		bForceMouseCheck = false;

		FLOAT dX=0.0f;
		FLOAT dY=0.0f;

		if (iKey==IK_MouseX)
		{
			dX = appFloor(Delta);
			MouseX+= (dX*MenuMouseSens);

			// Handle moving of controls if in design mode

			if ( (bDesignMode) && (MoveControl!=NULL) && (CtrlPressed) && (AltPressed) )
			{
				if (Delta>0)
					eventMoveFocused(MoveControl,1,0,0,0,ClipX, ClipY);
				else
					eventMoveFocused(MoveControl,-1,0,0,0,ClipX, ClipY);
				return true;
			}
		}
		else 
		{
			dY = appFloor(Delta);
			MouseY-= (dY * MenuMouseSens);

			// Handle moving of controls if in design mode

			if ( (bDesignMode) && (MoveControl!=NULL) && (CtrlPressed) && (AltPressed) )
			{
				if (Delta*-1 >0)
					eventMoveFocused(MoveControl,0,1,0,0,ClipX, ClipY);
				else
					eventMoveFocused(MoveControl,0,-1,0,0,ClipX, ClipY);
				return true;
			}
		}

		// As long as we are not capturing the mouse, make sure the Active Control is up to date

		LookUnderCursor(dX,dY);
	}

	// Handle Key Repeat

	if ( (iKey == RepeatKey) && (State!=IST_Press && State!=IST_Hold) )
	{
		RepeatKey=0;
		RepeatTime=0;
		RepeatDelta=0.0f;
	}

	else if ( (iKey!=RepeatKey) && (State==IST_Press) )
	{
		if (iKey!=IK_Shift && iKey!=IK_Ctrl && iKey!=IK_Alt)
		{

			RepeatKey=iKey;
			RepeatDelta=Delta;
			RepeatTime=ButtonRepeatDelay*3;
		}
	}

	if (iKey==IK_LeftMouse)
	{
		if (State==IST_Press)			// Process Mouse Presses
		{
			MousePressed(0);
		}
		else if (State==IST_Hold)		// Process Mouse Repeat
		{
			if (!bIgnoreNextRelease)
			{
				MousePressed(1);
			}
		}
		else if (State==IST_Release)	// Process Mouse Release
		{
			if (!bIgnoreNextRelease)
			{
				MouseReleased();
			}
			bIgnoreNextRelease=false;
		}

		// @@Note: Add code to handle double-click here

		return true;

	}

	// Pass key events along to the pages.  Start with the active page, then
	// proceed upward in the stack
	
	if ( ( ActivePage!=NULL) && ( ActivePage->NativeKeyEvent(iKey, State, Delta) ) )
		return true;

	// The Active Page didn't swallow it.. 		pass it along
	
	 for (INT i=MenuStack.Num()-1;i>=0; i--)
	{
		if (MenuStack(i) != ActivePage) 
		{
			if ( MenuStack(i)->NativeKeyEvent(iKey, State, Delta) )
				return true;
		}
	}
	
	return true;
}
					   
UBOOL UGUIController::NativeKeyType(BYTE& iKey, TCHAR Unicode )
{

	guard(GUIController::NativeKeyType);

	RepeatKey=0;
	RepeatTime=0;
	RepeatDelta=0.0f;

	if (ActivePage==NULL)
			return true;

	if ( !ActivePage->NativeKeyType(iKey, Unicode) )
	{
		// The Active Page didn't swallow it.. pass it along
		
		for (INT i=MenuStack.Num()-1; i>=0; i--)
		{
			if (MenuStack(i) != ActivePage)
			{
				if ( MenuStack(i)->NativeKeyType(iKey, Unicode) )
					return true;
			}
		}
	}

	return true;

	unguard;
}

// The Mouse Button was pressed

UBOOL UGUIController::MousePressed(UBOOL IsRepeat)
{
	guard(UGUIController::MousePressed);

	bIgnoreUntilPress = false;

	// Pass MousePresses to the Active Control

	if ( (ActiveControl!=NULL) && (ActiveControl != ActivePage) )
	{
		return ( ActiveControl->MousePressed(IsRepeat) );
	}
	else if (ActivePage!=NULL)
		ActivePage->NativeInvalidate();

	return 0;

	unguard;

}

// The Mouse Button was released

UBOOL UGUIController::MouseReleased()
{
	guard(UGUIController::MouseReleased);

	if (bIgnoreUntilPress)
		return false;

	if (ActiveControl!=NULL)
	{
		return (ActiveControl->MouseReleased() );
	}

	return false;

	unguard;
}

UGUIComponent* UGUIController::UnderCursor(FLOAT MouseX, FLOAT MouseY)
{
	if (ActivePage!=NULL)
	{
		return ActivePage->UnderCursor(MouseX, MouseY);
	}
	return NULL;
}

void UGUIController::PlayClickSound(BYTE SoundNum)
{
	USound* s = NULL;

	switch(SoundNum)
	{
	case CS_Click:
		s = ClickSound;
		break;
	case CS_Edit:
		s = EditSound;
		break;
	case CS_Up:
		s = UpSound;
		break;
	case CS_Down:
		s = DownSound;
		break;
	}

	if(!s)
		return;

	PlayInterfaceSound(s);
}

void UGUIController::PlayInterfaceSound(USound*  sound)
{
	guard(UGUIController::PlayInterfaceSound);

	APlayerController* pc = ViewportOwner->Actor;
	if( pc && pc->GetLevel() && pc->GetLevel()->Engine && pc->GetLevel()->Engine->Audio )
		pc->GetLevel()->Engine->Audio->PlaySound( NULL, 2*SLOT_None, sound, FVector(0, 0, 0), 1.0, 4096.0, 1.0, SF_No3D, 0.f );

	unguard;
}

void UGUIController::LookUnderCursor(FLOAT dX, FLOAT dY)
{
	
	UGUIComponent* Under = UnderCursor(MouseX, MouseY);

	// Handle moving over new control
	if (ActiveControl!=NULL) 
	{
		if ( (ActiveControl->MenuState!=MSAT_Pressed) || (!ActiveControl->bCaptureMouse) ) 
		{
			// Remove the first one


			if ( ActiveControl != Under) 
			{
				if (ActiveControl->MenuState==MSAT_Watched)
					ActiveControl->eventMenuStateChange(MSAT_Blurry);
			}
		
			if ( (Under!=NULL) && (Under->MenuState==MSAT_Blurry) )
			{
					Under->eventMenuStateChange(MSAT_Watched);

					// Mouse over sound
					if( Under->bMouseOverSound && (ActiveControl != Under) )
						PlayInterfaceSound(MouseOverSound);
			}

			ActiveControl = Under;
			
			if (ActivePage!=NULL)
			{
				if (ActiveControl==NULL) 
					ActivePage->eventChangeHint(TEXT(""));
				else
					ActivePage->eventChangeHint(ActiveControl->Hint);
			}

			// Call MouseHover on the thing the mouse is on.
			if(ActiveControl != NULL)
				ActiveControl->MouseHover();
		}
		else
			ActiveControl->MouseMove(dX,dY);
	}
	else if (Under!=NULL)
	{
		ActiveControl = Under;

		if ( ActiveControl->MenuState==MSAT_Blurry )
		{
			ActiveControl->eventMenuStateChange(MSAT_Watched);

			// Mouse over sound
			if(ActiveControl->bMouseOverSound)
				PlayInterfaceSound(MouseOverSound);
		}

		if (ActivePage!=NULL)
		{
			if (ActiveControl==NULL) 
				ActivePage->eventChangeHint(TEXT(""));
			else
				ActivePage->eventChangeHint(ActiveControl->Hint);
		}

		if(ActiveControl != NULL)
			ActiveControl->MouseHover();
	}
}

UBOOL UGUIController::HasMouseMoved()
{
	if (MouseX==LastMouseX && MouseY==LastMouseY)
		return false;	
	else
		return true;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIFonts 
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIFont::execGetFont(FFrame& Stack, RESULT_DECL )
{
	guard(GUIFont::execGetFont);

	P_GET_INT(XRes);
	P_FINISH;
	
	INT Index;
	if ( (XRes<800) || (bFixedSize) )
		Index = 0;
	else if (XRes<1024)
		Index = 1;
	else if (XRes<1280)
		Index = 2;
	else if (XRes<1600)
		Index = 3;
	else
		Index = 4;

	*(UObject**) Result = FontArray.Num() ? FontArray( Min<INT>(Index,FontArray.Num()-1) ) : NULL;

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIStyles
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIStyles::execDraw(FFrame& Stack, RESULT_DECL )	// UScript stubs
{
	guard(UGUIStyles::execDraw);

	P_GET_OBJECT(UCanvas, pCanvas);
	P_GET_BYTE(State);
	P_GET_FLOAT(Left);
	P_GET_FLOAT(Top);
	P_GET_FLOAT(Width);
	P_GET_FLOAT(Height);
	P_FINISH;

	Draw(pCanvas,State,Left,Top,Width,Height);

	unguard;
}

void UGUIStyles::execDrawText(FFrame& Stack, RESULT_DECL ) // UScript Stubs
{
	guard(UGUIStyles::execDrawText);

	P_GET_OBJECT(UCanvas, pCanvas);
	P_GET_BYTE(State);
	P_GET_FLOAT(Left);
	P_GET_FLOAT(Top);
	P_GET_FLOAT(Width);
	P_GET_FLOAT(Height);
	P_GET_BYTE(Just);
	P_GET_STR(Text)
	P_FINISH;

	DrawText(pCanvas,State,Left,Top,Width,Height,Just, *Text);

	unguard;
}


// Draw outputs the Style object to the canvas using the current menu state

void UGUIStyles::Draw(UCanvas* Canvas, BYTE MenuState, FLOAT Left, FLOAT Top, FLOAT Width, FLOAT Height)
{

	guard(IGUIStyles::Draw);

	if ( (MenuState<0) || (MenuState>5) || (delegateOnDraw(Canvas,MenuState,Left,Top,Width,Height) ) || (Images[MenuState]==NULL)  )
		return;


	FColor OldColor = Canvas->Color;
	if (MenuState==MSAT_Disabled)
		Canvas->Color = FColor(128,128,128,255);
	else
		Canvas->Color = FColor(255,255,255,255);
	
	UMaterial* Image = Images[MenuState];

	if (Image==NULL)
		return;

	FLOAT mW = Image->MaterialUSize();
	FLOAT mH = Image->MaterialVSize();

	Canvas->Style = RStyles[MenuState];
	Canvas->Color = ImgColors[MenuState];

	switch (ImgStyle[MenuState])
	{
		case 0 :	Canvas->DrawTile(Image,Left,Top,mW,mH,0,0,mW,mH,0,FColor(255,255,255,255),FPlane(0,0,0,0)); break;
		case 1 :	Canvas->DrawTileStretched(Image, Left,Top, Width, Height); break;
		case 2 :	Canvas->DrawTileScaleBound(Image, Left, Top, Width, Height); break;
	}

	unguard;
}

// Draw Text outputs text of the current style to the canvas using the current menu state
void UGUIStyles::DrawText(UCanvas* Canvas, BYTE MenuState, FLOAT Left, FLOAT Top, FLOAT Width, FLOAT Height, BYTE Just, const TCHAR* Text)
{
	guard(IGUIStyles::DrawText)

	BYTE OldStyle   = Canvas->Style;
	FColor OldColor = Canvas->Color;
	FPlane OldMod   = Canvas->ColorModulate;

	UFont* OldFont  = Canvas->Font;

	// Snap location and size to integer amounts in screen space.
	Left = appRound(Left);
	Top = appRound(Top);
	Width = appRound(Width);
	Height = appRound(Height);

	if ( (MenuState<0) || 
		 (MenuState>5) || 
		 (Fonts[MenuState]==NULL) || 
		 (delegateOnDrawText(Canvas,MenuState,Left,Top,Width,Height,Just,FString::Printf(TEXT("%c"), Text)))  )
		return;
	
	INT FontClipX = Canvas->SizeX;

	Canvas->Style	= RStyles[MenuState];
	Canvas->Color	= FontColors[MenuState];

	if (MenuState==MSAT_Disabled)
		Canvas->ColorModulate = FPlane(0.5,0.5,0.5,1.0);

	Canvas->Font = Fonts[MenuState]->eventGetFont(FontClipX);
	if (Canvas->Font!=NULL)
		Canvas->DrawTextJustified(Just, Left, Top, Left+Width, Top+Height, TEXT("%s"), Text);

	Canvas->Style = OldStyle;
	Canvas->Color = OldColor;
	Canvas->Font  = OldFont;

	Canvas->ColorModulate = OldMod;

	unguard;
}

void UGUIStyles::TextSize(UCanvas* Canvas, BYTE MenuState, const TCHAR* Test, INT& XL, INT& YL)
{
	guard(UGUIStyles::TextSize);

	INT FontClipX = Canvas->SizeX;

	Canvas->Font	= Fonts[MenuState]->eventGetFont(FontClipX);
	if (Canvas->Font!=NULL)
	{
		if (*Test == '\0')
		{
			Canvas->ClippedStrLen( Canvas->Font, 1.0, 1.0, XL, YL, TEXT("W") );
			XL = 0;
		}
		else
			Canvas->ClippedStrLen( Canvas->Font,1.0,1.0,XL,YL, Test);
	}

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIComponent
// =======================================================================================================================================================
// =======================================================================================================================================================

// PreDraw - Updates the Bounding box depending on resolution

void UGUIComponent::PreDraw(UCanvas* Canvas)
{
	guard(GUIComponent::PreDraw);
	UpdateBounds();
	if( DELEGATE_IS_SET(OnPreDraw) )
		delegateOnPreDraw(Canvas);
	unguard;
}

// Draw should always be subclassed in the individual components, and they should ALWAYS call their super first to
// obtain their state.

void UGUIComponent::Draw(UCanvas* Canvas)
{
	guard(GUIComponent::Draw);

	if (!bVisible)
			return;

	Canvas->CurX = ActualLeft();
	Canvas->CurY = ActualTop();

	if (Style!=NULL)
	{
		Canvas->Color = Style->FontColors[MenuState];
		Canvas->Style = Style->RStyles[MenuState];

		if (Style->Fonts[MenuState]!=NULL)
			Canvas->Font  = Style->Fonts[MenuState]->eventGetFont(Controller->Master->Client->Viewports(0)->SizeX);
	}

	unguard;
}

void UGUIComponent::UpdateBounds()
{

	guard(GUIComponent::UpdateBounds);

	// if Scaled, calculate the bounds

	Bounds[0] = ActualLeft();
	Bounds[1] = ActualTop();
	Bounds[2] = ActualLeft()+ActualWidth();
	Bounds[3] = ActualTop()+ActualHeight();

	if (Style!=NULL)
	{
		ClientBounds[0] = Bounds[0]+Style->BorderOffsets[0];
		ClientBounds[1] = Bounds[1]+Style->BorderOffsets[1];
		ClientBounds[2] = Bounds[2]-Style->BorderOffsets[2];
		ClientBounds[3] = Bounds[3]-Style->BorderOffsets[3];
	}
	else
	{
		ClientBounds[0] = Bounds[0];
		ClientBounds[1] = Bounds[1];
		ClientBounds[2] = Bounds[2];
		ClientBounds[3] = Bounds[3];
	}
	
	unguard;
}


// PerformHitTest checks the current Mouse X/Y position to see if it's touching this
// component.  

UBOOL UGUIComponent::PerformHitTest(INT MouseX, INT MouseY)
{

	guard(GUIComponent::PerformHitTest);

	if ( (!bVisible) || (!bAcceptsInput) || (MenuState==MSAT_Disabled) )	// If never accepting input, leave this
		return 0;

	if (DELEGATE_IS_SET(OnHitTest) )
		delegateOnHitTest(MouseX,MouseY);

	// Perform simple box collision

	if ( ( (MouseX >= Bounds[0]) && (MouseX <= Bounds[2]) ) && ( (MouseY >= Bounds[1]) && (MouseY <= Bounds[3]) ) )
		return 1;
	else
		return 0;

	unguard;

}

// Returns the actual width (including scaling) of a component
FLOAT UGUIComponent::ActualWidth()
{
	guard(UGUIComponent::ActualWidth);

	if ( !Controller || !Controller->Master || !Controller->Master->Client || 
			Controller->Master->Client->Viewports.Num()==0 || !Controller->Master->Client->Viewports(0) )
		return 0.0;

	if (WinWidth <=1)
	{
		if ( (bScaleToParent) && (MenuOwner!=NULL) )
			return MenuOwner->ActualWidth() * WinWidth;
		else
			return Controller->Master->Client->Viewports(0)->SizeX * WinWidth;
	}
	else
		return WinWidth;	

	unguard;
}

void UGUIComponent::execActualWidth(FFrame& Stack, RESULT_DECL )
{
	guard(GUIComponent::execActualWidth);
	P_FINISH;

	*(FLOAT*)Result = ActualWidth();
	
	unguard;
}


// Returns the actual height (including scaling) of a component
FLOAT UGUIComponent::ActualHeight()
{
	guard(UGUIComponent::ActualHeight);

	if ( !Controller || !Controller->Master || !Controller->Master->Client || 
			Controller->Master->Client->Viewports.Num()==0 || !Controller->Master->Client->Viewports(0) )
		return 0.0;

	if (WinHeight<=1)
	{
		if ( (bScaleToParent) && (MenuOwner!=NULL) ) 
			return MenuOwner->ActualHeight() * WinHeight;
		else
			return Controller->Master->Client->Viewports(0)->SizeY * WinHeight;
	}
	else
		return WinHeight;

	unguard;
}

void UGUIComponent::execActualHeight(FFrame& Stack, RESULT_DECL )
{
	guard(GUIComponent::execActualHeight);
	P_FINISH;

	*(FLOAT*)Result = ActualHeight();
	
	unguard;
}


// Returns the actual left (including scaling) of a component
FLOAT UGUIComponent::ActualLeft()
{
	guard(UGUIComponent::ActualLeft);

	if ( !Controller || !Controller->Master || !Controller->Master->Client || 
			Controller->Master->Client->Viewports.Num()==0 || !Controller->Master->Client->Viewports(0) )
		return 0.0;

	if (WinLeft<=1)
	{
		if ( (bBoundToParent) && (MenuOwner!=NULL) )
			return MenuOwner->ActualLeft() + (MenuOwner->ActualWidth() * WinLeft);
		else
			return Controller->Master->Client->Viewports(0)->SizeX * WinLeft;
	}
	else
	{
		if ( (bBoundToParent) && (MenuOwner!=NULL) )
			return MenuOwner->ActualLeft() + WinLeft;
		else
			return WinLeft;	
	}

	unguard;
}

void UGUIComponent::execActualLeft(FFrame& Stack, RESULT_DECL )
{
	guard(GUIComponent::execActualLeft);
	P_FINISH;

	*(FLOAT*)Result = ActualLeft();
	
	unguard;
}


// Returns the actual top (including scaling) of a component
FLOAT UGUIComponent::ActualTop()
{
	guard(UGUIComponent::ActualTop);

	if ( !Controller || !Controller->Master || !Controller->Master->Client || 
			Controller->Master->Client->Viewports.Num()==0 || !Controller->Master->Client->Viewports(0) )
		return 0.0;

	if (WinTop<=1)
	{
		if ( (bBoundToParent) && (MenuOwner!=NULL) )
			return MenuOwner->ActualTop() + (MenuOwner->ActualHeight() * WinTop);
		else
			return Controller->Master->Client->Viewports(0)->SizeY * WinTop;
	}
	else
	{		
		if ( (bBoundToParent) && (MenuOwner!=NULL) )
			return MenuOwner->ActualTop() + WinTop;
		else
			return WinTop;
	}

	unguard;
}

void UGUIComponent::execActualTop(FFrame& Stack, RESULT_DECL )
{
	guard(GUIComponent::execActualTop);
	P_FINISH;

	*(FLOAT*)Result = ActualTop();
	
	unguard;
}


// - SaveCanvasState and RestoreCanvasState are called before and after the actual component is rendered to make sure the
//   canvas is kept in the proper state for any other action.

void UGUIComponent::SaveCanvasState(UCanvas* Canvas)
{
	SaveX	  = Canvas->CurX;
	SaveY	  = Canvas->CurY;
	SaveColor = Canvas->Color;
	SaveStyle = Canvas->Style;
	SaveFont  = Canvas->Font;
}
	
void UGUIComponent::RestoreCanvasState(UCanvas* Canvas)
{
	Canvas->CurX  = SaveX;
	Canvas->CurY  = SaveY;
	Canvas->Color = SaveColor;
	Canvas->Style = SaveStyle;
	Canvas->Font  = SaveFont;
}

// It's useful for each component to be able to act upon mouse/presses and releases 

UBOOL UGUIComponent::MousePressed(UBOOL IsRepeat)	
{
	guard(GUIComponet::MousePressed);

	// Attempt to SetFocus to this control

	eventSetFocus(NULL);
	eventMenuStateChange(MSAT_Pressed);

	delegateOnMousePressed(this,IsRepeat);

	// If we never focus we want this to act as a click

	if (bRepeatClick) //  && !Controller->CtrlPressed)	//FIXME! ctrl is here for handiness
	{
		Controller->PlayClickSound(OnClickSound);

		delegateOnClick(this);
	}

	return true;

	unguard;
}		

UBOOL UGUIComponent::MouseReleased()
{
	guard(GUIComponet::MouseReleased);

	if ( (bRequireReleaseClick) || (PerformHitTest(Controller->MouseX, Controller->MouseY)) )
	{
		if (bNeverFocus)
			eventMenuStateChange(MSAT_Watched);
		else
			eventSetFocus(NULL);

		if (!bRepeatClick) // && !Controller->CtrlPressed)  //FIXME! ctrl is here for handiness
		{
			// Play sound _before_ executing action
			Controller->PlayClickSound(OnClickSound);

			// Check for Double Click

			if ( (DELEGATE_IS_SET(OnDblClick) ) && (appSeconds() <Controller->LastClickTime+Controller->DblClickWindow) && (Controller->LastClickX == Controller->MouseX && Controller->LastClickY == Controller->MouseY) )
			{
				delegateOnDblClick(this);
				Controller->LastClickX=-1;
				Controller->LastClickY=-1;
				Controller->LastClickTime=0.0f;
			}
			else
			{
				delegateOnClick(this);
				Controller->LastClickX = Controller->MouseX;
				Controller->LastClickY = Controller->MouseY;
				Controller->LastClickTime = appSeconds();
			}
		}
	}
	else
	{
		if ( (MenuOwner!=NULL) && (Cast<UGUIMultiComponent>(MenuOwner)!=NULL) )
		{
			if (Cast<UGUIMultiComponent>(MenuOwner)->FocusedControl == this)
				eventMenuStateChange(MSAT_Focused);
			else
				eventMenuStateChange(MSAT_Blurry);
		}
		else
			eventMenuStateChange(MSAT_Blurry);
	}

	delegateOnMouseRelease(this);

	Controller->LookUnderCursor(0,0);

	return true;

	unguard;
}

// By Default, when the mouse is moved, components need to be searched to see if they are effected.  If this function returns true, then
// This component has used the input so go no further.  It should always be assumed that 

UBOOL UGUIComponent::MouseMove(INT XDelta, INT YDelta)
{
	guard(GUIComponent::MouseMove)
	
	if ( (MenuState==MSAT_Pressed) && (bCaptureMouse) )
		return delegateOnCapturedMouseMove(XDelta, YDelta);

	return false;

	unguard;
}

// When moving over things that have not captured the mouse, we might still want to know (ie changing cursor shape)
UBOOL UGUIComponent::MouseHover()
{
	guard(GUIComponent::MouseHover)
	
	return false;

	unguard;
}


// Input events should be handled by the subclasses

UBOOL UGUIComponent::NativeKeyEvent(BYTE& iKey, BYTE& State, FLOAT Delta )
{
	guard(GUIComponent::NativeKeyEvent);

	if ( iKey == IK_Tab && Controller->CtrlPressed && (State==1 || State==2) )
	{
		if (Controller->ShiftPressed)
		{
			if (eventPrevPage())
				return true;
		}
		else if (eventNextPage())
			return true;
	}
	return delegateOnKeyEvent(iKey, State, Delta);

	unguard;
}

UBOOL UGUIComponent::NativeKeyType(BYTE& iKey, TCHAR Unicode )
{
	guard(GUIComponent::NativeKeyType);

	if ( (iKey==IK_Tab) && (!bCaptureTabs) )
	{
		if (Controller->ShiftPressed)
			eventPrevControl(NULL);
		else
			eventNextControl(NULL);
		
		return true;
	}

	return delegateOnKeyType(iKey,FString::Printf(TEXT("%c"), Unicode));

	unguard;
}

void UGUIComponent::CloneDims(UGUIComponent* From)
{
	guard(GUIComponent::CloneDims)

	if (From!=NULL)
	{
		WinWidth  = From->ActualWidth();
		WinHeight = From->ActualHeight();
		WinLeft	  = From->ActualLeft();
		WinTop	  = From->ActualTop();
	}

	unguard;
}

void  UGUIComponent::SetDims(FLOAT Width, FLOAT Height, FLOAT Left, FLOAT Top)
{
	guard(GUIComponent::SetDims);

	WinWidth = Width;
	WinHeight = Height;
	WinLeft = Left;
	WinTop = Top;

	unguard;
}

UBOOL UGUIComponent::SpecialHit()
{
	guard(UGUIComponent::SpecialHit);

	if ( (!bVisible) || (!Controller) )
		return false;

	if (this == Controller->ActivePage)
		return false;

	if ( ( (Controller->MouseX >= Bounds[0]) && (Controller->MouseX <= Bounds[2]) ) && ( (Controller->MouseY >= Bounds[1]) && (Controller->MouseY <= Bounds[3]) ) )
	{
		if (Controller->MoveControl == this)
			Controller->MoveControl = NULL;
		else if (Controller->MoveControl == NULL)
		{
			Controller->MoveControl = this;
			return true;
		}

	}
	return false;
	unguard;
}

UGUIComponent* UGUIComponent::UnderCursor(FLOAT MouseX, FLOAT MouseY)
{
	guard(UGUIComponent::UnderCursor);
	if ((bVisible) && (MenuState!=MSAT_Disabled) && ( PerformHitTest(MouseX,MouseY) ))
		return this;

	return NULL;
	unguard;
}

void UGUIComponent::NativeInvalidate()
{
	delegateOnInvalidate();
}

void UGUIComponent::execSetTimer(FFrame& Stack, RESULT_DECL )
{
	guard(UGUIComponent::execSetTimer);
	// Verify that the control is really on the good page
	P_GET_FLOAT(Interval);
	P_GET_UBOOL_OPTX(bRepeat, false);
	P_FINISH;

	// Find Component's Page
	UGUIComponent *OuterOwner = this;
	while (OuterOwner->MenuOwner != NULL)
		OuterOwner = OuterOwner->MenuOwner;

	UGUIPage *Page = CastChecked<UGUIPage>(OuterOwner);

	if (TimerIndex < 0 && Interval > 0.0f)
		TimerIndex = Page->Timers.AddItem(this);

	TimerInterval = Interval;
	TimerCountdown = Interval;
	bTimerRepeat = false;
	if (TimerInterval > 0.0f)
		bTimerRepeat = bRepeat;

	unguard;
}

void UGUIComponent::execKillTimer(FFrame& Stack, RESULT_DECL )
{
	guard(UGUIComponent::execKillTimer);
	P_FINISH;

	TimerInterval=0.0;
	TimerCountdown=0.0;
	bTimerRepeat=false;

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIMultiComponent
// =======================================================================================================================================================
// =======================================================================================================================================================

UBOOL UGUIMultiComponent::PerformHitTest(INT MouseX, INT MouseY)
{
	guard(GUIMultiComponent::PerformHitTest);

	if (!bVisible)
		return false;

	for (INT i=0;i<Controls.Num();i++)
		if ( Controls(i)->PerformHitTest(MouseX, MouseY) )
			return true;

	return Super::PerformHitTest(MouseX, MouseY);

	unguard;
}

void UGUIMultiComponent::PreDraw(UCanvas* Canvas)
{
	guard(GUIMultiComponent::PreDraw);

	Super::PreDraw(Canvas);

	for (INT i=0;i<Controls.Num();i++)
	{
		if ( Controls(i) )
			Controls(i)->PreDraw(Canvas);
	}
	unguard;
}

void UGUIMultiComponent::Draw(UCanvas* Canvas)
{
	guard(GUIMultiComponent::Draw);

	if (!bVisible) 
			return;

	Super::Draw(Canvas);

	INT FocusedIndex=-1;
	for (INT i=0;i<Controls.Num();i++)
	{
		if ( Controls(i) )
		{
			if (Controls(i) == FocusedControl)
				FocusedIndex = i;
			else
			{
				Controls(i)->SaveCanvasState(Canvas);
				Controls(i)->Draw(Canvas);
				Controls(i)->RestoreCanvasState(Canvas);
			}
		}
	}

	if ( (FocusedIndex!=-1) && (FocusedIndex<Controls.Num()) && ( Controls(FocusedIndex) ) )
	{
		Controls(FocusedIndex)->SaveCanvasState(Canvas);
		Controls(FocusedIndex)->Draw(Canvas);
		Controls(FocusedIndex)->RestoreCanvasState(Canvas);
	}

	unguard;
}

UBOOL UGUIMultiComponent::NativeKeyEvent(BYTE& iKey, BYTE& State, FLOAT Delta )
{
	guard(UGUIMultiComponent::NativeKeyEvent)

	if ( (FocusedControl!=NULL) && (FocusedControl->NativeKeyEvent(iKey, State, Delta)) )
		return true;

	if (MenuState==MSAT_Focused)
		return Super::NativeKeyEvent(iKey, State, Delta);

	return false;

	unguard;
}
				 
UBOOL UGUIMultiComponent::NativeKeyType(BYTE& iKey, TCHAR Unicode )
{

	guard(UGUIMultiComponent::NativeKeyType);

	if ( (FocusedControl!=NULL) && (FocusedControl->NativeKeyType(iKey, Unicode)) )
		return true;

	if (MenuState==MSAT_Focused)
		return Super::NativeKeyType(iKey, Unicode);

	return false;

	unguard;
}

UGUIComponent* UGUIMultiComponent::UnderCursor(FLOAT MouseX, FLOAT MouseY)
{

	guard(UGUIMultiComponent::UnderCursor);
	UGUIComponent* tComp;

	if (FocusedControl!=NULL)
	{
		tComp = FocusedControl->UnderCursor(MouseX,MouseY);
		if (tComp!=NULL)
			return tComp;
	}

	for (INT i=Controls.Num();i>0;i--)
	{
		if ( (Controls(i-1)->bVisible) && (Controls(i-1)->MenuState!=MSAT_Disabled) )
		{
			tComp = Controls(i-1)->UnderCursor(MouseX, MouseY);
			if (tComp!=NULL)
				return tComp;
		}
	}
	return Super::UnderCursor( MouseX, MouseY );

	unguard;
}

void UGUIMultiComponent::NativeInvalidate()
{
	Super::NativeInvalidate();

	if (FocusedControl!=NULL)
		FocusedControl->NativeInvalidate();
}

UBOOL UGUIMultiComponent::SpecialHit()
{

	guard(UGUIMultiComponent::SpecialHit);

	if (Super::SpecialHit())
		return true;

	for (INT i=0;i<Controls.Num();i++)
	{
		if ( Controls(i) && Controls(i)->SpecialHit() )
			return true;
	}

	return false;

	unguard;

}


// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIPage
// =======================================================================================================================================================
// =======================================================================================================================================================

UBOOL UGUIPage::PerformHitTest(INT MouseX, INT MouseY)
{
	guard(GUIPage::PerformHitTest);
	
	return UGUIComponent::PerformHitTest(MouseX, MouseY);

	unguard;
}

void UGUIPage::Draw(UCanvas* Canvas)
{
	guard(GUIPage::Draw);

	if (!bVisible)
		return;

	SaveCanvasState(Canvas);	// Save Canvas State

	// Draw the background

	if (Background!=NULL)
	{
		Canvas->Color = BackgroundColor;
		Canvas->Style = BackgroundRStyle;
		Canvas->DrawTileScaleBound( Background, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight() );
	}

	// Pass it along;

	Super::Draw(Canvas);

	delegateOnDraw(Canvas);

	RestoreCanvasState(Canvas);		// Restore the Canvas state

	unguard;
}

UBOOL UGUIPage::NativeKeyEvent(BYTE& iKey, BYTE& State, FLOAT Delta )
{
	guard(UGUIPage::NativeKeyEvent)
		
	if ( Super::NativeKeyEvent(iKey, State, Delta) )
		return true;

	// Handle Close	-- Add support for aborting

	if (iKey==IK_Escape)
	{
		if ( (State==IST_Release) && ( delegateOnCanClose(true)) )
			Controller->eventCloseMenu(true);

		return true;
	}

	return true;

	unguard;
}

void UGUIPage::UpdateTimers(float DeltaTime)
{
	guard(UGUIPage::UpdateTimers);

	// We want to ignore any new timer created during this cycle
	INT TimerCnt = Timers.Num();

	for (INT i = 0; i<TimerCnt; i++)
	{
	UGUIComponent *Timer = Timers(i);

		Timer->TimerCountdown -= DeltaTime;
		Timer->TimerIndex = i;

		if (Timer->TimerCountdown <= 0.0 && Timer->TimerInterval > 0.0)
		{
			Timer->eventTimer();

			if (Timer->bTimerRepeat)
			{
				Timer->TimerCountdown += Timer->TimerInterval;
				// In case TimerInterval is too small
				if (Timer->TimerCountdown <= 0.0)
					Timer->TimerCountdown = Timer->TimerInterval;
			}
			else
			{
				Timer->TimerCountdown = 0.0;
				Timer->TimerInterval = 0.0;
			}
		}
		// Remove any defunct timer 
		if (Timer->TimerInterval <= 0.0)
		{
			Timer->TimerIndex = -1;
			Timers.Remove(i);
			i--;
			TimerCnt--;		// Make sure we dont trigger timers created in this timer cycle
		}
	}
	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIImage
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIImage::Draw(UCanvas* Canvas)
{
	guard(GUIImage::Draw);

	if ( (!bVisible) || (!Image) )
			return;

	// Pass it along;

	Super::Draw(Canvas);

	// Draw the background starting in the upper left of the image

	Canvas->Style = ImageRenderStyle;
	Canvas->Color = ImageColor;

	switch (ImageStyle)
	{
		case ISTY_Normal:
		{
			FLOAT X,Y, XL, YL;

			if (X1!=-1)
				X = X1;
			else
				X = 0;

			if (Y1!=-1)
				Y = Y1;
			else
				Y = 0;

			if (X2==-1)
				XL = Min<FLOAT>(ActualWidth(), Image->MaterialUSize());
			else
				XL = (X2-X1);

			if (Y2==-1)
				YL = Min<FLOAT>(ActualHeight(), Image->MaterialVSize());
			else
				YL = (Y2-Y1);

			if (ImageAlign==IMGA_Center)
				Canvas->DrawTile(Image, ActualLeft()-(XL/2), ActualTop()-(YL/2), XL, YL,X,Y, XL, YL, 0, Canvas->Color, FPlane(0,0,0,0));
			else
				Canvas->DrawTile(Image, ActualLeft(), ActualTop(), XL, YL,X,Y, XL, YL, 0, Canvas->Color, FPlane(0,0,0,0));
				
			break;
		}

		case ISTY_Stretched:
		{
			if ( X1 < 0 && X2 < 0 && Y1 < 0 && Y2 < 0 ) 
			{
				Canvas->DrawTileStretched(Image, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight() ); 
			} 
			else
			{
				FLOAT X,Y, XL, YL;

				if (X1!=-1)
					X = X1;
				else
					X = 0;

				if (Y1!=-1)
					Y = Y1;
				else
					Y = 0;

				if (X2==-1)
					XL = Min<FLOAT>(ActualWidth(), Image->MaterialUSize());
				else
					XL = (X2-X1);

				if (Y2==-1)
					YL = Min<FLOAT>(ActualHeight(), Image->MaterialVSize());
				else
					YL = (Y2-Y1);

				if (ImageAlign==IMGA_Center)
					Canvas->DrawTile(Image, ActualLeft()-(XL/2), ActualTop()-(YL/2), ActualWidth(), ActualHeight(), X,Y, XL, YL, 0, Canvas->Color, FPlane(0,0,0,0));
				else
					Canvas->DrawTile(Image, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight(), X,Y, XL, YL, 0, Canvas->Color, FPlane(0,0,0,0));
					
				break;
			}
			break;
		}
		case ISTY_Scaled:
			Canvas->DrawTileScaleBound(Image, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight() );
			break;

		case ISTY_Bound:
			Canvas->DrawTileBound(Image, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight() );
			break;

		case ISTY_Justified:
			Canvas->DrawTileJustified(Image, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight(), ImageAlign );
			break;
	}

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUILabel
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUILabel::Draw(UCanvas* Canvas)
{
	guard(GUILabel::Draw);

	if (!bVisible) return;

	// Pass it along;

	Super::Draw(Canvas);

	// Grab the font

	UGUIFont* MyFont = Controller->eventGetMenuFont(TextFont);
	if (MyFont!=NULL)
	{
		UFont* CFont = MyFont->eventGetFont(Canvas->SizeX);
		if (CFont==NULL)	
			return;

		Canvas->Font = CFont;

	}
	else
		return;

	if (!bTransparent)  // Draw the Background if required
		Canvas->DrawTile(Controller->DefaultPens[0], ActualLeft(), ActualTop(), ActualWidth(), ActualHeight(),0,0,32,32, 0.0f, BackColor,FPlane(0,0,0,0));

	FPlane OldMod = Canvas->ColorModulate;

	if (MenuState==MSAT_Disabled)
		Canvas->ColorModulate = FPlane(0.5,0.5,0.5,1.0);

	if (!bMultiLine)
	{
		if (Style==NULL)
		{
			Canvas->Color=TextColor;
			Canvas->Style=TextStyle;
			Canvas->DrawTextJustified(TextAlign, 
				appRound(ActualLeft()), 
				appRound(ActualTop()), 
				appRound(ActualLeft()+ActualWidth()), 
				appRound(ActualTop()+ActualHeight()), 
				TEXT("%s"),
				*Caption);
		}
		else
			Style->DrawText(Canvas, MenuState, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight(), TextAlign, *Caption);
	}
	else	// Here we handle MultiLine Labels
	{
	TArray<FString> Lines;
	INT XL, YL;
	FLOAT Left, Top, Width, Bottom;

		// TODO: Use Bounds or ClientBounds ???
		Left = ClientBounds[0];
		Top = ClientBounds[1];
		Width = ClientBounds[2] - ClientBounds[0];
		Bottom = ClientBounds[3];

		// TODO: Eventually offer Vertical Alignment
		// TODO: Make Split Char configurable in GUILabel
		// First get the Height of the font
		Canvas->ClippedStrLen(NULL, 1.0, 1.0, XL, YL, TEXT("W"));
		Canvas->WrapStringToArray(*Caption, &Lines, Width, NULL, '|');
		if (Style==NULL)
		{
			Canvas->Color=TextColor;
			Canvas->Style=TextStyle;

			for (INT i=0; i<Lines.Num(); i++)
			{
				Canvas->DrawTextJustified(TextAlign, 
					appRound(Left), 
					appRound(Top), 
					appRound(Left + Width), 
					appRound(Top + YL), 
					TEXT("%s"),
					*Lines(i));

				Top += YL;
				if ((Top+YL) >= Bottom)
					break;
			}
		}
		else
		{
			for (INT i=0; i<Lines.Num(); i++)
			{
				Style->DrawText(Canvas, MenuState, Left, Top, Width, YL, TextAlign, *Lines(i));
				Top += YL;
				if ((Top+YL) >= Bottom)
					break;
			}
		}
	}
	Canvas->ColorModulate = OldMod;

	unguard;
}
// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIButton
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIButton::Draw(UCanvas* Canvas)
{
	guard(GUIButton::Draw);

	if ( (!bVisible) || (Style==NULL) ) 	// Buttons require a style
		return;

	Super::Draw(Canvas);

	// Draw the button using it's style object

	Style->Draw(Canvas,MenuState, ActualLeft(),ActualTop(),ActualWidth(),ActualHeight());
	Style->DrawText(Canvas, MenuState, ActualLeft(),ActualTop(),ActualWidth(),ActualHeight(), 1, *Caption );
	
	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIGFXButton
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIGFXButton::Draw(UCanvas* Canvas)
{
	guard(GUIGFXButton::Draw);

	if (!bVisible)
			return;

	// Draw the button

	Super::Draw(Canvas);

	if ( (Graphic==NULL) || ((bCheckBox) && (!bChecked)) )
		return;

	// Figure out the X pixels = Y % of screen so we can scale the border

	FLOAT mW = Graphic->MaterialUSize();
	FLOAT mH = Graphic->MaterialVSize();

	if (bCheckBox)
	{
		if (MenuState==MSAT_Disabled)
			Canvas->Color = FColor(200,200,200,200);
		else
			Canvas->Color = FColor(255,255,255,255);

		Canvas->Style = 5;
	}
	FLOAT cW, cH, CX, CY;
	if (bClientBound)
	{
		cW = ClientBounds[2] - ClientBounds[0];
		cH = ClientBounds[3] - ClientBounds[1];
		CX = ClientBounds[0];
		CY = ClientBounds[1];
	}
	else
	{
		cW = ActualWidth();
		cH = ActualHeight();
		CX = ActualLeft();
		CY = ActualTop();
	}
	FLOAT dW = Min<FLOAT>(cW, mW);
	FLOAT dH = Min<FLOAT>(cH, mH);

	// MC: Clip to ClientBounds
	if (Position==ICP_Center)
		Canvas->DrawTile(Graphic, CX + ((cW - dW)/2), CY + ((cH - dH)/2), dW, dH, (mW-dW)/2, (mH-dH)/2, dW, dH, 0, Canvas->Color, FPlane(0,0,0,0));

	else if (Position==ICP_Stretched)
		Canvas->DrawTileStretched(Graphic, CX, CY, cW, cH ) ;			

	else if (Position==ICP_Scaled)
		Canvas->DrawTileScaleBound(Graphic, CX, CY, cW, cH ) ;			

	else if (Position==ICP_Bound)
		Canvas->DrawTileBound(Graphic, CX, CY, cW, cH ) ;			

	else
		Canvas->DrawTile(Graphic, CX, CY, dW, dH, 0, 0, dW, dH, 0, Canvas->Color, FPlane(0,0,0,0));

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIEditBox
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIEditBox::Draw(UCanvas* Canvas)
{
	guard(GUIEditBox::Draw);

	if ( (!bVisible) || (Style==NULL) )
			return;

	Super::Draw(Canvas);

	UpdateBounds();

	FString Storage,FinalDraw;
	Storage = TextStr;

	if ( (bMaskText) && (Storage.Len()>0) )
	{
		for (INT MaskIndex=0;MaskIndex<Storage.Len();MaskIndex++)
			Storage[MaskIndex] = *TEXT("#");
	}

	INT OldClipX = Canvas->ClipX;
	INT OldClipY = Canvas->ClipY;

	Canvas->Color = Style->FontColors[MenuState];

	INT XL,YL,BoxWidth=ClientBounds[2]-ClientBounds[0];
	
	if ( (Storage.Len() != LastLength) || (CaretPos!=LastCaret) )
	{
		// Recaculate FirstVis

		if (CaretPos<=FirstVis)
			FirstVis = Max(0,CaretPos-1);
		else
		{
			FinalDraw = Storage.Mid(FirstVis,CaretPos-FirstVis);
			Style->TextSize(Canvas,MenuState,*FinalDraw,XL,YL);

			while ( (XL>BoxWidth) && (FirstVis<Storage.Len()) )
			{
				FirstVis++;
				FinalDraw = Storage.Mid(FirstVis,CaretPos-FirstVis);
				Style->TextSize(Canvas,MenuState,*FinalDraw,XL,YL);
			}
		}

	}
	LastLength = Storage.Len();

	if (bReadOnly)
			FirstVis = 0;

	FinalDraw = Storage.Mid(FirstVis,Storage.Len()-FirstVis);

	// Display Cursor/select-all block. Behind text.

	if (!bReadOnly && bHasFocus)
	{

		if ( (FirstVis==CaretPos) || (FinalDraw.Len()==0) )
		{
			Canvas->ClippedStrLen( Canvas->Font, 1, 1, XL, YL, TEXT("W") );
			XL = 0;
			bAllSelected=false;
		}
		else
		{
			FString TmpString = FinalDraw.Mid(0,CaretPos-FirstVis);
			Canvas->ClippedStrLen( Canvas->Font, 1, 1, XL, YL, *TmpString );
		}

		FLOAT CursorY = ActualTop() + (ActualHeight()/2) - (YL/2);

		Canvas->Style=5;	// Alpha

		if(bAllSelected)
			Canvas->DrawTile(Controller->DefaultPens[0],ClientBounds[0],CursorY,XL,YL,0,0,3,1,0.0,FColor(255,255,255,0.5f * Controller->CursorFade),FPlane(0,0,0,0));
		else
			Canvas->DrawTile(Controller->DefaultPens[0],ClientBounds[0]+XL,CursorY,3,YL,0,0,3,1,0.0,FColor(255,255,255,Controller->CursorFade),FPlane(0,0,0,0));
	}


	Canvas->CurX   = ClientBounds[0];
	Canvas->CurY   = Bounds[1];
	Canvas->ClipX  = ClientBounds[2];
	Canvas->ClipY  = Bounds[3];

	Style->DrawText(Canvas, MenuState, ClientBounds[0], Bounds[1], ClientBounds[2]-ClientBounds[0], Bounds[3] - Bounds[1], 0, *FinalDraw);

	Canvas->ClipX = OldClipX;
	Canvas->ClipY = OldClipY;


	if (!bHasFocus)
		return;

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUINumericEdit
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUINumericEdit::Draw(UCanvas* Canvas)
{
	guard(GUINumericEdit::Draw);

	if (!bVisible)
			return;

	// Layout the controls

	FLOAT SpinDim = ActualHeight() / 2;

	// Setup the actual widths/heights of all of the controls.

	Controls(1)->WinWidth = SpinDim;
	Controls(2)->WinWidth = SpinDim;
	Controls(1)->WinHeight = SpinDim;
	Controls(2)->WinHeight = SpinDim;

	if (bLeftJustified)
	{
		Controls(1)->WinLeft = ActualLeft();
		Controls(2)->WinLeft = ActualLeft();
		Controls(1)->WinTop  = ActualTop();
		Controls(2)->WinTop  = ActualTop() + SpinDim;
		
		Controls(0)->WinWidth = ActualWidth() - SpinDim;
		Controls(0)->WinHeight = ActualHeight();
		Controls(0)->WinLeft = ActualLeft() + SpinDim;
		Controls(0)->WinTop = ActualTop();
	}
	else
	{

		Controls(0)->WinWidth = ActualWidth() - SpinDim;
		Controls(0)->WinHeight = ActualHeight();
		Controls(0)->WinLeft = ActualLeft();
		Controls(0)->WinTop = ActualTop();

		Controls(1)->WinLeft = ActualLeft()+Controls(0)->WinWidth;
		Controls(2)->WinLeft = ActualLeft()+Controls(0)->WinWidth;
		Controls(1)->WinTop  = ActualTop();
		Controls(2)->WinTop  = ActualTop() + SpinDim;
	}

	// Draw the controls

	if (!bVisible)
		return;

	Super::Draw(Canvas);
	
	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIVertScrollZone
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIVertScrollZone::Draw(UCanvas* Canvas)
{
	guard(UGUIVertScrollZone::Draw);

	if ( (!bVisible) || (Style==NULL) || ActualWidth()<=1 || ActualHeight()<=1 )
			return;

	Super::Draw(Canvas);

	Style->Draw(Canvas, MenuState, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight());

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIVertScrollBar
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIVertScrollBar::PreDraw(UCanvas* Canvas)
{
	guard(UGUIVertScrollBar::PreDraw);

	Super::PreDraw(Canvas);

	if ( (!bVisible) || (MyList==NULL) || (MyList->ItemCount==0) ) 
		return;
	
	// Position the two tick buttons

	FLOAT AWidth  = ActualWidth();
	FLOAT AHeight = ActualHeight();

	Controls(0)->SetDims(AWidth,AHeight-(2*AWidth),ActualLeft(),ActualTop()+AWidth);
	Controls(1)->SetDims(AWidth,AWidth,ActualLeft(),ActualTop());
	Controls(2)->SetDims(AWidth,AWidth,ActualLeft(),ActualTop()+AHeight-AWidth);

	// Calculate the grip

	FLOAT ZoneHeight = Controls(0)->ActualHeight();
	
	GripHeight = ZoneHeight * ( MyList->ItemsPerPage / FLOAT(MyList->ItemCount) );

	if (GripHeight<12)
		GripHeight=12;
	
	ZoneHeight -= GripHeight;

	FLOAT GripY = Controls(0)->ActualTop() + appFloor( ZoneHeight * GripTop );
	Controls(3)->SetDims(AWidth,GripHeight,ActualLeft(),GripY);

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIListBoxBase
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIListBoxBase::PreDraw(UCanvas* Canvas)
{

	guard(UGUIListBoxBase::PreDraw);
	
	Super::PreDraw(Canvas);

	if (!bVisible) return;

	UGUIListBase* List = CastChecked<UGUIListBase>(Controls(0));
	if ( (List->ItemsPerPage<=0) || ( List->ItemCount <= List->ItemsPerPage ) )
		Controls(1)->bVisible = false;
	else
		Controls(1)->bVisible = true;

	Controls(0)->SetDims( ActualWidth() - (Controls(1)->bVisible ? Controls(1)->ActualWidth() : 0), ActualHeight(), ActualLeft(), ActualTop() );
	Controls(1)->SetDims( Controls(1)->ActualWidth(), ActualHeight(), ActualLeft() + Controls(0)->ActualWidth(), ActualTop() );

	unguard;
}

void UGUIListBoxBase::Draw(UCanvas* Canvas)
{
	guard(UGUListBox::Draw);

	Super::Draw(Canvas);

	return;

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIPanel
// =======================================================================================================================================================
// =======================================================================================================================================================

UBOOL UGUIPanel::PerformHitTest(INT MouseX, INT MouseY)
{
	guard(GUIPanel::PerformHitTest);
	
	return UGUIComponent::PerformHitTest(MouseX, MouseY);

	unguard;
}


void UGUIPanel::Draw(UCanvas* Canvas)
{
	guard(UGUPanel::Draw);

	if (!bVisible || delegateOnDraw(Canvas))
			return;

	if (MenuOwner==NULL)
	{
		debugf(TEXT("%s has no MenuOwner"),GetName() );
		return;
	}

	if (Background!=NULL)
		Canvas->DrawTileScaleBound( Background, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight() );

	Super::Draw(Canvas);

	unguard;

}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIVertList
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIVertList::PreDraw(UCanvas* Canvas)
{
	guard(UGUIVertList::PreDraw);
	if ( (!bVisible) || (Style==NULL) )
		return;
	INT XL, YL;
	Canvas->Font = Style->Fonts[MenuState]->eventGetFont(Canvas->SizeX);

	if (!Canvas || !Canvas->Font)
			return;

	Canvas->ClippedStrLen( Canvas->Font, 1, 1, XL, YL, TEXT("WQ,2") );
	ItemHeight = YL;
	ItemsPerPage = appFloor( (ClientBounds[3] - ClientBounds[1]) / ItemHeight );
	Super::PreDraw(Canvas);
	unguard;
}

void UGUIVertList::Draw(UCanvas* Canvas)
{
	guard(UGUIVertList::Draw);

	if ( (!bVisible) || (Style==NULL) || (ItemCount==0 && !bVisibleWhenEmpty)  )
		return;

	Style->Draw(Canvas,MenuState, ActualLeft(),ActualTop(),ActualWidth(),ActualHeight());
	INT Y = ClientBounds[1];
	BYTE TempMenuState;

	if ( ( bHotTrack ) && (Controller->HasMouseMoved()) && (PerformHitTest(Controller->MouseX, Controller->MouseY) ) )
	{
		Index = Top + appFloor( (Controller->MouseY - ClientBounds[1]) / ItemHeight);
	}

	for (INT i=0;i<ItemsPerPage;i++)
	{
		
		TempMenuState = MenuState;

		if ( Top+i<ItemCount )	// Draw the text
		{
			// Check if this is the selected image

			UBOOL bIsSelected = (Top+i)==Index;

			if( DELEGATE_IS_SET(OnDrawItem) )
			{
				delegateOnDrawItem( Canvas, Top+i, ClientBounds[0], Y, ClientBounds[2]-ClientBounds[0], ItemHeight, bIsSelected );
			}
			else
			{
				if (bIsSelected) 
				{
					if ( (MenuState==MSAT_Focused)  || (MenuState==MSAT_Pressed) )
					{
						if (SelectedImage==NULL)
							Canvas->DrawTile(Controller->DefaultPens[0], ClientBounds[0],Y, ClientBounds[2] - ClientBounds[0],ItemHeight,0,0,32,32, 0.0f, SelectedBKColor,FPlane(0,0,0,0));
						else
						{
							Canvas->Color = SelectedBKColor;
							Canvas->DrawTileStretched(SelectedImage,ClientBounds[0],Y,ClientBounds[2] - ClientBounds[0],ItemHeight);
						}
					}

					MenuState=MSAT_Pressed;
				}
				else if( MenuState==MSAT_Pressed ) // Never draw unselected items as pressed
					MenuState=MSAT_Focused;

				DrawItem( Canvas, Top+i, ClientBounds[0], Y, ClientBounds[2]-ClientBounds[0], ItemHeight );
			}
		}
		MenuState=TempMenuState;
		Y+=ItemHeight;
	}

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIHorzList
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIHorzList::Draw(UCanvas* Canvas)
{
	guard(UGUIHorzList::Draw);

	if ( (!bVisible) || (Style==NULL) || (ItemCount==0) )
		return;

	Style->Draw(Canvas,MenuState, ActualLeft(),ActualTop(),ActualWidth(),ActualHeight());

	ItemsPerPage = appFloor( (ClientBounds[2] - ClientBounds[0]) / ItemWidth );

	INT X = ClientBounds[0];
	BYTE TempMenuState;

	FLOAT SelIndex = Index;

	if ( bHotTrack && PerformHitTest(Controller->MouseX, Controller->MouseY) )
		SelIndex = Top + appFloor( (Controller->MouseX - ClientBounds[0]) / ItemWidth);

	for (INT i=0;i<ItemsPerPage;i++)
	{
		
		TempMenuState = MenuState;

		FLOAT DrawIndex = (Top+i);

		if ( Top+i<ItemCount )	// Draw the text
		{
			// Check if this is the selected image

			UBOOL bIsSelected = DrawIndex==SelIndex;

			if( DELEGATE_IS_SET(OnDrawItem) )
				delegateOnDrawItem( Canvas, DrawIndex, X, ClientBounds[1], ItemWidth, ClientBounds[3]-ClientBounds[1], bIsSelected );
			else
			{
				if (bIsSelected) 
				{
					if (MenuState==MSAT_Focused) 
					{
						if (SelectedImage==NULL)
							Canvas->DrawTile(Controller->DefaultPens[0], X, ClientBounds[1], ItemWidth,  ClientBounds[3] - ClientBounds[1],0,0,32,32, 0.0f, SelectedBKColor,FPlane(0,0,0,0));
						else
							Canvas->DrawTileStretched(SelectedImage,X, ClientBounds[1],ItemWidth, ClientBounds[3] - ClientBounds[1]);
					}

					MenuState=MSAT_Watched;
				}
				DrawItem( Canvas, DrawIndex, X, ClientBounds[1], ItemWidth, ClientBounds[3]-ClientBounds[1]);
			}
		}
		MenuState=TempMenuState;
		X+=ItemWidth;
	}

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUICircularList
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUICircularList::Draw(UCanvas* Canvas)
{
	guard(UGUICircularList::Draw);

	if ( (!bVisible) || (Style==NULL) || (ItemCount==0) || (!Controller) )
		return;

	Style->Draw(Canvas,MenuState, ActualLeft(),ActualTop(),ActualWidth(),ActualHeight());

	if (FixedItemsPerPage>0)
		ItemsPerPage = FixedItemsPerPage;
	else
		ItemsPerPage = appFloor( (ClientBounds[2] - ClientBounds[0]) / ItemWidth );

	INT X = ClientBounds[0];
	INT Width = ClientBounds[2] - ClientBounds[0];

	if ( (bCenterInBounds) && (ItemsPerPage*ItemWidth<Width) )
		X += (Width - (ItemsPerPage*ItemWidth))/2;

	FLOAT xMod=0.0f;
	if (bFillBounds)
		xMod = (Width - (ItemsPerPage*ItemWidth)) / ItemsPerPage;

	BYTE TempMenuState;

	FLOAT SelIndex = Index;

	if ( bHotTrack && PerformHitTest(Controller->MouseX, Controller->MouseY) )
		SelIndex = Top + appFloor( (Controller->MouseX - ClientBounds[0]) / ItemWidth);

	for (INT i=0;i<ItemsPerPage;i++)
	{
		
		TempMenuState = MenuState;

		FLOAT DrawIndex = (Top+i) % ItemCount;

		UBOOL bIsSelected = DrawIndex==SelIndex;

		if( DELEGATE_IS_SET(OnDrawItem) )
			delegateOnDrawItem( Canvas, DrawIndex, X, ClientBounds[1], ItemWidth, ClientBounds[3]-ClientBounds[1], bIsSelected );
		else
		{
			if (bIsSelected)
			{
				if (MenuState==MSAT_Focused) 
				{
					if (SelectedImage==NULL)
						Canvas->DrawTile(Controller->DefaultPens[0], X, ClientBounds[1], ItemWidth,  ClientBounds[3] - ClientBounds[1],0,0,32,32, 0.0f, SelectedBKColor,FPlane(0,0,0,0));
					else
						Canvas->DrawTileStretched(SelectedImage,X, ClientBounds[1],ItemWidth, ClientBounds[3] - ClientBounds[1]);
				}
				MenuState=MSAT_Watched;
			}
			DrawItem( Canvas, DrawIndex, X, ClientBounds[1], ItemWidth, ClientBounds[3]-ClientBounds[1]);
		}

		MenuState=TempMenuState;

		if (!bFillBounds)
			X+=ItemWidth;
		else
			X+=ItemWidth+xMod;
	}

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIList
// =======================================================================================================================================================
// =======================================================================================================================================================
void UGUIList::DrawItem(UCanvas* Canvas, INT Item, FLOAT X, FLOAT Y, FLOAT W, FLOAT H)
{
	guard(UGUIList::DrawItem)
	Style->DrawText(Canvas, MenuState, X,Y,W,H, TextAlign, *(Elements(Item).item));
	unguard;
}

// EEEUUUCCHHH!!!
static UGUIList* GSList;

static INT Compare( FGUIListElem& A, FGUIListElem& B )
{
	if(GSList)
		return GSList->delegateCompareItem(A, B);
	else
		return appStricmp( *(A.item), *(B.item) );
}

void UGUIList::execSortList( FFrame& Stack, RESULT_DECL )
{
	guard(UGUIList::SortList);

	P_FINISH;

	// GSList being set indicates we should use the script delegate to compare elements.
	// If no delegate is set - we sort alphabetically.
	if(DELEGATE_IS_SET(CompareItem))
		GSList=this;
	else
		GSList=NULL;

	
	Sort( &Elements(0), Elements.Num() );

	unguard;
}



// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIComboBox
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIComboBox::PreDraw(UCanvas* Canvas)
{

	guard(UGUIComboBox::PreDraw);
	
	Super::PreDraw(Canvas);

	if (!bVisible) return;

	FLOAT AWidth  = ActualWidth();
	FLOAT AHeight = ActualHeight();

	Controls(0)->SetDims( AWidth - AHeight, ActualHeight(), ActualLeft(), ActualTop() );
	Controls(1)->SetDims( AHeight, ActualHeight(), ActualLeft() + Controls(0)->ActualWidth(), ActualTop() );

	if (List)
	{
		// Figure out how many items fit in the list
		int ListItems = List->Elements.Num();
		if (ListItems > MaxVisibleItems)
			ListItems = MaxVisibleItems;
		// We need a default value
		if (ListItems < 0)
			ListItems = 8;

		if (ListItems > 0 && List->Style)
		{
		int Wh = List->ItemHeight * ListItems + List->Style->BorderOffsets[0] + List->Style->BorderOffsets[2];
		int top = ActualTop() + ActualHeight();

			if ((top + Wh) > Canvas->ClipY)
				top = ActualTop() - Wh - 1; 

			Controls(2)->SetDims( ActualWidth(), Wh , ActualLeft(), top );
		}
	}

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUITabControl
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUITabControl::PreDraw(UCanvas* Canvas)
{
	guard(GUITabControl::PreDraw);

	Super::PreDraw(Canvas);

	INT StartIndex,RowX,RowY,ExtraPerButton;
	INT Count,XL,YL;

	FLOAT ATabHeight = TabHeight;

	if (ATabHeight<1)
		ATabHeight = Controller->Master->Client->Viewports(0)->SizeY * ATabHeight;

	FLOAT CompSize, TotalRowSize, RowWidth = ActualWidth();

	// Account for Style

	if (BackgroundStyle!=NULL)
		RowWidth = RowWidth - BackgroundStyle->BorderOffsets[0] - BackgroundStyle->BorderOffsets[2];

	WinHeight = 0;

	TotalRowSize = 0;
	StartIndex = 0;

	FLOAT BottomRow = 0;

	if (BackgroundStyle!=NULL)
		RowY = BackgroundStyle->BorderOffsets[1];
	else
		RowY = 0;

	Count = 0;

	for (INT i=0;i<TabStack.Num();i++)
	{
		TabStack(i)->Style->TextSize(Canvas,TabStack(i)->MenuState,*(TabStack(i)->Caption),XL,YL);
		CompSize = 10 +  XL + TabStack(i)->Style->BorderOffsets[0] + TabStack(i)->Style->BorderOffsets[2];
        
		if ( ( TotalRowSize + CompSize > RowWidth ) )	// If we will overflow, or we are the lsat control
		{
			// Align and position this row.
			ExtraPerButton = bFillSpace ? appFloor( (RowWidth - TotalRowSize) / Count ) : 0;
			RowX = BackgroundStyle ? BackgroundStyle->BorderOffsets[0] : 0;

			while (Count>0)
			{
				TabStack(StartIndex)->Style->TextSize(Canvas,TabStack(StartIndex)->MenuState,*(TabStack(StartIndex)->Caption),XL,YL);
				FLOAT ThisCompSize = 10 +  XL + + TabStack(StartIndex)->Style->BorderOffsets[0] + TabStack(StartIndex)->Style->BorderOffsets[2] + ExtraPerButton;
				TabStack(StartIndex)->SetDims(ThisCompSize,ATabHeight,RowX,RowY);
				RowX+=ThisCompSize;
				Count--;
				StartIndex++;
				BottomRow = ActualTop() + RowY + ATabHeight;
			}

			RowY += ATabHeight-3;
			WinHeight+=ATabHeight-3;		
			TotalRowSize=0;
		}

		Count++;
		TotalRowSize+=CompSize;
	}

	if (Count > 0)
	{
		// Align and position last row.
		ExtraPerButton = bFillSpace ? appFloor( (RowWidth - TotalRowSize) / Count ) : 0;
		RowX = BackgroundStyle ? BackgroundStyle->BorderOffsets[0] : 0;

		while (Count>0)
		{
			TabStack(StartIndex)->Style->TextSize(Canvas,TabStack(StartIndex)->MenuState,*(TabStack(StartIndex)->Caption),XL,YL);
			FLOAT ThisCompSize = 10 +  XL + + TabStack(StartIndex)->Style->BorderOffsets[0] + TabStack(StartIndex)->Style->BorderOffsets[2] + ExtraPerButton;
			TabStack(StartIndex)->SetDims(ThisCompSize,ATabHeight,RowX,RowY);
			RowX+=ThisCompSize;
			Count--;
			StartIndex++;
			BottomRow = ActualTop() + RowY + ATabHeight;
		}
		RowY += ATabHeight-3;
		WinHeight+=ATabHeight-3;		
	}

	if ( (bDockPanels) && (ActiveTab!=NULL) && (ActiveTab->MyPanel!=NULL) )	
		ActiveTab->MyPanel->WinTop = BottomRow;

	for (INT i=0;i<TabStack.Num();i++)
		TabStack(i)->PreDraw(Canvas);

	if (BackgroundStyle!=NULL)
		WinHeight = WinHeight + BackgroundStyle->BorderOffsets[0] + BackgroundStyle->BorderOffsets[2];

	if (FocusedControl!=NULL)
		FocusedControl->PreDraw(Canvas);

	unguard;
}


void UGUITabControl::Draw(UCanvas* Canvas)
{
	guard(GUITabControl::Draw);

	if (!bVisible)
		return;

	if (BackgroundStyle!=NULL)
	{
		if ( (MenuState==MSAT_Focused) && (FocusedControl==NULL) )
			BackgroundStyle->Draw(Canvas,MenuState, ActualLeft(),ActualTop(),ActualWidth(),ActualHeight());
		else
			BackgroundStyle->Draw(Canvas,MSAT_Blurry, ActualLeft(),ActualTop(),ActualWidth(),ActualHeight());
	}

	if (BackgroundImage!=NULL)
		Canvas->DrawTileScaleBound(BackgroundImage, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight() );

	for (INT i=0;i<TabStack.Num();i++)
	{
		if (TabStack(i)!=ActiveTab)
		{
			if ( TabStack(i) != Controller->ActiveControl)
				TabStack(i)->MenuState=MSAT_Blurry;
			else
				TabStack(i)->MenuState=MSAT_Watched;

			TabStack(i)->SaveCanvasState(Canvas);
			TabStack(i)->Draw(Canvas);
			TabStack(i)->RestoreCanvasState(Canvas);
		}
	}

	if (ActiveTab!=NULL)
	{
		if (FocusedControl==NULL)
			ActiveTab->MenuState=MSAT_Focused;
		else
			ActiveTab->MenuState=MSAT_Pressed;

		ActiveTab->SaveCanvasState(Canvas);
		ActiveTab->Draw(Canvas);
		ActiveTab->RestoreCanvasState(Canvas);
		ActiveTab->MyPanel->Draw(Canvas);
		
		ActiveTab->MenuState = MenuState;

	}

	unguard;
}
				 
UGUIComponent* UGUITabControl::UnderCursor(FLOAT MouseX, FLOAT MouseY)
{

	guard(UGUITabControl::UnderCursor);
	UGUIComponent* tComp;

	if (ActiveTab!=NULL)
	{
		tComp = ActiveTab->UnderCursor(MouseX,MouseY);
		if (tComp!=NULL)
			return tComp;
		else if ( (ActiveTab->MyPanel) != NULL)
		{
			tComp = ActiveTab->MyPanel->UnderCursor(MouseX,MouseY);
			if (tComp!=NULL)
				return tComp;
		}
	}

	for (INT i=TabStack.Num();i>0;i--)
	{
		tComp = TabStack(i-1)->UnderCursor(MouseX, MouseY);
		if (tComp!=NULL)
			return tComp;
	}
	
	return Super::UnderCursor(MouseX, MouseY);

	unguard;
}

UBOOL UGUITabControl::SpecialHit()
{
	guard(UGUITabControl::SpecialHit);

	if (Super::Super::SpecialHit())
		return true;

	if ( (ActiveTab!=NULL) && (ActiveTab->MyPanel!=NULL) )
		return ActiveTab->MyPanel->SpecialHit();

	return false;
	unguard;
}


// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIMenuOption
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIMenuOption::PreDraw(UCanvas* Canvas)
{
	guard(GUIMenuOption::PreDraw);

	Super::PreDraw(Canvas);

	if ( (MyLabel==NULL) || (MyComponent==NULL) )
		return;

	if (bVerticalLayout)
	{
		// This is a quickly hacked in vertical split MenuOption
		if (CaptionWidth <=1)
			MyLabel->WinHeight = ActualHeight() * CaptionWidth;
		else
			MyLabel->WinHeight = CaptionWidth;

		MyLabel->WinTop = ActualTop();
		MyLabel->WinLeft = ActualLeft();
		MyLabel->WinWidth = ActualWidth();

		// Figure out MyComponent new Height
		MyComponent->WinHeight = ActualHeight() - MyLabel->WinHeight;
		MyComponent->WinTop = ActualTop() + MyLabel->WinHeight;
		MyComponent->WinLeft = MyLabel->WinLeft;
		MyComponent->WinWidth = MyLabel->WinWidth;
	}
	else
	{
		if (CaptionWidth<=1)
			MyLabel->WinWidth = ActualWidth() * CaptionWidth;
		else
			MyLabel->WinWidth = CaptionWidth;
	
		MyLabel->WinHeight = ActualHeight();
		MyLabel->WinTop = ActualTop();
		
		MyComponent->WinHeight = ActualHeight();
	
		if (!bSquare)
			MyComponent->WinWidth = ActualWidth() - MyLabel->WinWidth;
		else	
			MyComponent->WinWidth = MyComponent->WinHeight;
	
		MyComponent->WinTop = ActualTop();
	
		// Figure out the Lefts.
	
		if (bFlipped)
		{
			MyLabel->WinLeft = ActualLeft()+MyComponent->WinWidth;
	
			if (ComponentJustification==TXTA_Left)
				MyComponent->WinLeft = ActualLeft();
			
			else 
				MyComponent->WinLeft = MyLabel->WinLeft - MyComponent->WinWidth;
	
		}
		else
		{
			MyLabel->WinLeft = ActualLeft();
	
			if (ComponentJustification==TXTA_Left)
				MyComponent->WinLeft = ActualLeft() + MyLabel->WinWidth;
			
			else 
				MyComponent->WinLeft = ActualLeft() + ActualWidth() - MyComponent->WinWidth;
		}
	
		if (ComponentWidth>=0)
			MyComponent->WinWidth*=ComponentWidth;
	
}
	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUITitleBar
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUITitleBar::PreDraw(UCanvas* Canvas)
{
	guard(GUITitleBar::PreDraw);

	// Calculate the Height

	if (Style==NULL)	return;

	INT XL,YL;

	if (bUseTextHeight)
	{
		Style->TextSize(Canvas, 0, TEXT("QWz,1"),XL,YL);
		WinHeight = YL+Style->BorderOffsets[1]+Style->BorderOffsets[3];
	}

	Super::PreDraw(Canvas);

	if (DockedTabs!=NULL)
	{
		if (bDockTop)
			DockedTabs->WinTop = ActualTop() + DockedTabs->ActualHeight()-3;
		else
		{
			DockedTabs->WinLeft = ActualLeft()+Style->BorderOffsets[0];
			DockedTabs->WinWidth = ActualWidth() - Style->BorderOffsets[0] - Style->BorderOffsets[2];
			DockedTabs->WinTop = ActualTop() + ActualHeight()-2;
		}
	}

	unguard;
}

void UGUITitleBar::Draw(UCanvas* Canvas)
{
	guard(GUITitleBar::Draw);

	if (Style==NULL)	return;

	Super::Draw(Canvas);

	// Draw the border

	Style->Draw(Canvas, 0, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight() );

	// Draw the effect if any..

	Style->DrawText(Canvas, 1, ActualLeft()+Style->BorderOffsets[0],ActualTop(),ActualWidth(),ActualHeight(), Justification, *Caption );


	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUISplitter
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUISplitter::Draw(UCanvas* Canvas)
{
	guard(UGUISplitter::Draw);

	if ( (!bVisible) || (Style==NULL) ) 	// Splitters require a style
		return;

	// Draw the splitters subcomponents.  Have to turn off style.
	UGUIStyles* TempStyle = Style;
	Style = NULL;
	UGUIMultiComponent::Draw(Canvas);
	Style = TempStyle;
	UGUIComponent::Draw(Canvas);


	// Draw the splitter using it's style object
	switch(SplitOrientation)
	{
	case SPLIT_Vertical:
		{
			FLOAT Y = ActualTop() + SplitPosition*(ActualHeight()-SplitAreaSize);
			Style->Draw(Canvas, MenuState, ActualLeft(), Y, ActualWidth(), SplitAreaSize );
		}
		break;
	case SPLIT_Horizontal:
		{
			FLOAT X = ActualLeft() + SplitPosition*(ActualWidth()-SplitAreaSize);
			Style->Draw(Canvas, MenuState, X, ActualTop(), SplitAreaSize, ActualHeight() );
		}
		break;
	}

	unguard;
}

void UGUISplitter::PreDraw(UCanvas* Canvas)
{
	guard(UGUISplitter::PreDraw);
	Super::PreDraw(Canvas);
	SplitterUpdatePositions();
	unguard;
}

void UGUISplitter::execSplitterUpdatePositions(FFrame& Stack, RESULT_DECL )
{
	P_FINISH;
	SplitterUpdatePositions();
}

void UGUISplitter::SplitterUpdatePositions()
{
	guard(UGUISplitter::SplitterUpdatePositions);

	if (!bVisible) return;
	if( Controls.Num() < 2 || !Controls(0) || !Controls(1) )
		return;

	FLOAT AWidth  = ActualWidth();
	FLOAT AHeight = ActualHeight();
    
	switch(SplitOrientation)
	{
	case SPLIT_Vertical:
		{
			FLOAT H = SplitPosition*(AHeight-SplitAreaSize);
			Controls(0)->SetDims( AWidth, H, 0, 0 );
			Controls(1)->SetDims( AWidth, AHeight-H-SplitAreaSize, 0, H+SplitAreaSize );
		}
		break;
	case SPLIT_Horizontal:
		{
			FLOAT W = SplitPosition*(AWidth-SplitAreaSize);
			Controls(0)->SetDims( W, AHeight, 0, 0 );
			Controls(1)->SetDims( AWidth-W-SplitAreaSize, AHeight, W+SplitAreaSize, 0 );
		}
		break;
	}
	unguard;
}

UBOOL UGUISplitter::MousePressed(UBOOL IsRepeat)
{
	guard(UGUISplitter::MousePressed);

	if (!Controller)
		return true;

	Super::MousePressed(IsRepeat);
	if( !IsRepeat )
	{
		FLOAT X1, X2, Y1, Y2;
		switch(SplitOrientation)
		{
		case SPLIT_Vertical:
			X1 = ActualLeft();
			X2 = X1 + ActualWidth();
			Y1 = ActualTop() + SplitPosition*(ActualHeight()-SplitAreaSize);
			Y2 = Y1 + SplitAreaSize;
			break;
		case SPLIT_Horizontal:
			X1 = ActualLeft() + SplitPosition*(ActualWidth()-SplitAreaSize);
			X2 = X1 + SplitAreaSize;
			Y1 = ActualTop();
			Y2 = Y1 + ActualHeight();
			break;
		default:
			return true;		// for warning
		}
		if( Controller->MouseX >= X1 && Controller->MouseX <= X2 && Controller->MouseY >= Y1 && Controller->MouseY <= Y2 )
			bCaptureMouse = 1;
		else
			bCaptureMouse = 0;
	}
	return true;
	unguard;
}

UBOOL UGUISplitter::MouseMove(INT XDelta, INT YDelta)
{
	guard(UGUISplitter::MouseMove);
	Super::MouseMove(XDelta,YDelta);

	if (!Controller)
		return true;

	if( MenuState==MSAT_Pressed )
	{
		switch(SplitOrientation)
		{
		case SPLIT_Vertical:
			SplitPosition = Clamp<FLOAT>( ((FLOAT)Controller->MouseY - ActualTop()) / ActualHeight(), 0.f, 1.f );
			break;
		case SPLIT_Horizontal:
			SplitPosition = Clamp<FLOAT>( ((FLOAT)Controller->MouseX - ActualLeft()) / ActualWidth(), 0.f, 1.f );
			break;
		}
		SplitterUpdatePositions();
	}
	else
		bCaptureMouse = 0;
	return false;
	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUISlider
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUISlider::Draw(UCanvas* Canvas)
{

	guard(UGUISlider::Draw);

	if ( (Style==NULL) || (!bVisible) || ((MinValue==0) && (MaxValue==0)) )
		return;

	Super::Draw(Canvas);

	FLOAT MWidth  = Canvas->SizeX * (16.0f/640.0f);

	Style->Draw(Canvas,0,ActualLeft()+(MWidth/2), ActualTop() + (ActualHeight()/4), ActualWidth()-MWidth, ActualHeight()/2);

	// Draw the marker

	FLOAT MLeft   = ( ActualLeft() + (MWidth/2) + ( (ActualWidth() - MWidth) * ( (Value-MinValue) / (MaxValue - MinValue) ))) - (MWidth / 2);
	FLOAT MTop    = ActualTop();
	FLOAT MHeight = ActualHeight();

	Style->Draw(Canvas,MenuState,MLeft, MTop, MWidth, MHeight);

	if (CaptionStyle==NULL)
		return;

	INT XL,YL;

	CaptionStyle->TextSize(Canvas,MSAT_Blurry, TEXT("WQ,"),XL,YL);
	CaptionStyle->DrawText(Canvas, MSAT_Blurry, ActualLeft(), MTop+MHeight, ActualWidth(), YL,1,*delegateOnDrawCaption());

	unguard;

}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUITabButton
// =======================================================================================================================================================
// =======================================================================================================================================================

UBOOL UGUITabButton::MousePressed(UBOOL IsRepeat)	
{
	guard(GUITabButton::MousePressed);

	// Attempt to SetFocus to this control

	if (!IsRepeat)
	{
		Controller->PlayClickSound(OnClickSound);

		eventSetFocus(NULL);
		eventMenuStateChange(MSAT_Pressed);
		delegateOnClick(this);
	}

	return true;

	unguard;
}		

UBOOL UGUITabButton::MouseReleased()
{
	guard(GUITabButton::MouseReleased);

	// HACK! Avoid playing mouse click sound on mouse released for tabs
	BYTE SoundNum = OnClickSound;
	OnClickSound = 0;
	Super::MouseReleased();
	OnClickSound = SoundNum;

//Controller->LookUnderCursor(0,0);
	return true;
	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIHorzScrollZone
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIHorzScrollZone::Draw(UCanvas* Canvas)
{
	guard(UGUIHorzScrollZone::Draw);

	if ( (!bVisible) || (Style==NULL) )
			return;

	Super::Draw(Canvas);

	Style->Draw(Canvas, MenuState, ActualLeft(), ActualTop(), ActualWidth(), ActualHeight());

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIHorzScrollBar
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIHorzScrollBar::PreDraw(UCanvas* Canvas)
{
	guard(UGUIHorzScrollBar::PreDraw);

	Super::PreDraw(Canvas);

	if ( (!bVisible) || (MyList==NULL) || (MyList->ItemCount==0) ) 
		return;
	
	// Position the two tick buttons

	FLOAT AWidth  = ActualWidth();
	FLOAT AHeight = ActualHeight();

	Controls(0)->SetDims(AWidth-(2*AHeight),AHeight,ActualLeft()+AHeight,ActualTop());
	Controls(1)->SetDims(AWidth,AWidth,ActualLeft(),ActualTop());
	Controls(2)->SetDims(AHeight,AHeight,ActualLeft()+AHeight-AWidth,ActualTop());

	// Calculate the grip

	FLOAT ZoneWidth = Controls(0)->ActualWidth();
	
	GripWidth = ZoneWidth * ( MyList->ItemsPerPage / FLOAT(MyList->ItemCount) );

	if (GripWidth<12)
		GripWidth=12;
	
	ZoneWidth -= GripWidth;

	FLOAT GripX = Controls(0)->ActualLeft() + appFloor( ZoneWidth * GripLeft );
	Controls(3)->SetDims(GripWidth,AHeight,GripX,ActualTop());

	unguard;
}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUICharacterList
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUICharacterList::PreDraw(UCanvas* Canvas)
{
	guard(UGUICharacterList::PreDraw);
	if (!bVisible) 
		return;

	Super::PreDraw(Canvas);

	ItemHeight = ActualHeight();
	ItemWidth = ItemHeight / 2;
	ItemsPerPage = appFloor( (ClientBounds[2] - ClientBounds[0]) / ItemWidth );

	unguard;
}

void UGUICharacterList::DrawItem(UCanvas* Canvas, INT Item, FLOAT X, FLOAT Y, FLOAT W, FLOAT H)
{
	guard(UGUICharacterList::DrawItem)

	UMaterial* Mat = PlayerList(Item).Portrait;
	if (!Mat)
		Mat = DefaultPortrait;

	FColor Color;
	if (Item == Index)
	{
		Color = FColor(255,255,255,255);
		Canvas->DrawTile(Controller->DefaultPens[0], X + (W/2) - (ItemWidth/2), Y + (H/2) - (ItemHeight/2), ItemWidth, ItemHeight, 0, 0, 256, 512, 0, FColor(255,255,0,255), FPlane(0,0,0,0));
	}
	else
		Color = FColor(128,128,128,255);

	Canvas->DrawTile(Mat, X + (W/2) - (ItemWidth/2)+1, Y + (H/2)+1 - (ItemHeight/2), ItemWidth-2, ItemHeight-2, 0, 0, 256, 512, 0, Color, FPlane(0,0,0,0));

	unguard;
}
// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIMultiColumnListBox
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIMultiColumnListBox::PreDraw(UCanvas* Canvas)
{

	guard(UGUIMultiColumnListBox::PreDraw);
	
	Super::PreDraw(Canvas);

	if (!bVisible) return;

	FLOAT HeaderSize = Controls(2)->ActualHeight();
	Controls(1)->WinWidth = HeaderSize;

	Controls(0)->SetDims( ActualWidth() - (Controls(1)->bVisible ? HeaderSize : 0), ActualHeight()-HeaderSize, ActualLeft(), ActualTop()+HeaderSize );
	Controls(0)->bVisible = Controls(0)->WinHeight > 0;
	Controls(1)->SetDims( HeaderSize, ActualHeight()-HeaderSize, ActualLeft() + Controls(0)->ActualWidth(), ActualTop()+HeaderSize );

	Controls(2)->bVisible = ActualHeight() > HeaderSize;
	Controls(2)->SetDims( ActualWidth(), HeaderSize, ActualLeft(), ActualTop() );

	unguard;
}
// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIMultiColumnListHeader
// =======================================================================================================================================================
// =======================================================================================================================================================
void UGUIMultiColumnListHeader::PreDraw(UCanvas* Canvas)
{
	guard(UGUIMultiColumnListHeader::PreDraw);
	Super::PreDraw(Canvas);
	if( Style )
	{
		INT XL, YL;
		Style->TextSize(Canvas, MenuState, TEXT("A"), XL, YL);
		WinHeight = YL * 1.2;
	}
	unguard;
}

UBOOL UGUIMultiColumnListHeader::MousePressed(UBOOL IsRepeat)
{
	guard(UGUIMultiColumnListHeader::MousePressed);
	Super::MousePressed(IsRepeat);
	if( !IsRepeat )
	{
		ClickingCol = -1;
		SizingCol = -1;
		FLOAT x = ActualLeft();
		for( INT i=0;i<MyList->ColumnWidths.Num();i++ )
		{
			x += MyList->ColumnWidths(i);
			if( x >= (FLOAT)Controller->MouseX-2.f && x <= (FLOAT)Controller->MouseX+2.f )
			{
				bCaptureMouse = 1;
				SizingCol = i;
			}
			else
			if( SizingCol >= 0 )
				return true;
		}

		x = ActualLeft();
		for( INT i=0;i<MyList->ColumnWidths.Num();i++ )
		{
			if( (FLOAT)Controller->MouseX >= x+5 && (FLOAT)Controller->MouseX <= x+MyList->ColumnWidths(i)-5 && MyList->ColumnWidths(i) > 10 )
			{
				ClickingCol = i;

				if( MyList->SortColumn == i )
				{
					MyList->SortDescending = !MyList->SortDescending;

					if(MyList->SortDescending == 0)
						Controller->PlayClickSound(CS_Up);
					else
						Controller->PlayClickSound(CS_Down);
				}
				else if(MyList->SortColumn != -1)
				{
					MyList->SortColumn = i;
					MyList->SortDescending = 0;
					Controller->PlayClickSound(CS_Up);
				}

				MyList->eventOnSortChanged();
				return true;
			}
			x += MyList->ColumnWidths(i);
		}
	}
	return true;
	unguard;
}

UBOOL UGUIMultiColumnListHeader::MouseReleased()
{
	guard(UGUIMultiColumnListHeader::MousePressed);
	Super::MouseReleased();
	ClickingCol = -1;
	SizingCol = -1;
	return true;
	unguard;
}

UBOOL UGUIMultiColumnListHeader::MouseMove(INT XDelta, INT YDelta)
{
	guard(UGUIMultiColumnListHeader::MouseMove);
	Super::MouseMove(XDelta,YDelta);
	
	if( MenuState==MSAT_Pressed && SizingCol >= 0 && SizingCol<MyList->ColumnWidths.Num() )
	{
		FLOAT x = ActualLeft();
		for( INT i=0;i<SizingCol;i++ )
			x += MyList->ColumnWidths(i);
		MyList->ColumnWidths(SizingCol) = Clamp<FLOAT>( (FLOAT)Controller->MouseX - x, 0, ActualLeft() + ActualWidth() - x );
		MyList->eventOnColumnSized(SizingCol);
	}
	else
	{
		bCaptureMouse = 0;
		SizingCol = -1;
	}
	return false;
	unguard;
}

UBOOL UGUIMultiColumnListHeader::MouseHover()
{
	guard(UGUIMultiColumnListHeader::MouseMove);
	Super::MouseHover();

	// Update curosr if we are just waving the cursor over the thing. Dont care which one it is.
	INT OnColSize = -1;
	FLOAT x = ActualLeft();
	for( INT i=0; i<MyList->ColumnWidths.Num() && OnColSize == -1; i++ )
	{
		x += MyList->ColumnWidths(i);
		if( x >= (FLOAT)Controller->MouseX-2.f && x <= (FLOAT)Controller->MouseX+2.f )
		{
			OnColSize = i;
		}
	}

	if(OnColSize != -1)
		MouseCursorIndex = 5;
	else
		MouseCursorIndex = 0;

	return false;

	unguard;
}


void UGUIMultiColumnListHeader::Draw(UCanvas* Canvas)
{
	guard(UGUIMultiColumnListHeader::Draw);

	if ( (!bVisible) || (Style==NULL) )
		return;

	Super::Draw(Canvas);

	FLOAT x = ActualLeft();
    //for( INT i=0;i<MyList->ColumnWidths.Num() && i<MyList->ColumnHeadings.Num();i++ )
    for( INT i=0;i<MyList->ColumnWidths.Num();i++ )  // assumes that ColumnHeadings size >= ColumnWidths.Num()
	{
		FLOAT w = Min<FLOAT>( MyList->ColumnWidths(i), ActualWidth()+ActualLeft()-x );
		if( w > 0 )
		{
			BYTE DrawState;		
			if( i == MyList->SortColumn )
				DrawState = MSAT_Focused;
			else
			if( i == ClickingCol )
				DrawState = MSAT_Pressed;
			else
				DrawState = MSAT_Blurry;			

			Style->Draw( Canvas, DrawState, x, ActualTop(), w, ActualHeight() );
			Style->DrawText( Canvas, DrawState, x+Style->BorderOffsets[0], ActualTop()+Style->BorderOffsets[1], w-Style->BorderOffsets[0]-Style->BorderOffsets[2], ActualHeight()-Style->BorderOffsets[1]-Style->BorderOffsets[3], 1, *MyList->ColumnHeadings[i] );
			x += w;
		}
    }
	FLOAT x2 = ActualWidth()+ActualLeft();
	if( x < x2 )
		Style->Draw( Canvas, MSAT_Blurry, x, ActualTop(), x2 - x, ActualHeight() );
	unguard;
}
// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIMultiColumnList
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIMultiColumnList::execAddedItem(FFrame& Stack, RESULT_DECL )
{
    guard(UGUIMultiColumnList::execAddedItem);
	P_FINISH;
	INT j = SortData.AddZeroed();
	InvSortData.Add();
	SortData(j).SortString = eventGetSortString(j) + appItoa(j);
	SortData(j).SortItem = j;
	if( j < InvSortData.Num() )	// this should always be true
		InvSortData(j) = j;
	unguard;
}

void UGUIMultiColumnList::execUpdatedItem(FFrame& Stack, RESULT_DECL )
{
    guard(UGUIMultiColumnList::execUpdatedItem);
	P_GET_INT(listitem);
	P_FINISH;
	if( listitem>=0 && listitem<InvSortData.Num() )
	{
		INT i = InvSortData(listitem);
		if( i>=0 && i<SortData.Num() )
			SortData(i).SortString = eventGetSortString(listitem) + appItoa(listitem);
	}
	unguard;
}

void UGUIMultiColumnList::execChangeSortOrder(FFrame& Stack, RESULT_DECL )
{
    guard(UGUIMultiColumnList::execChangeSortOrder);
	P_FINISH;

	for( INT i=0;i<SortData.Num();i++ )
	{
		SortData(i).SortItem = i;
		SortData(i).SortString = eventGetSortString(SortData(i).SortItem) + appItoa(SortData(i).SortItem);
	}

	unguard;
}

static INT MLSortOrder=1;
static INT Compare( FMultiColumnSortData& A, FMultiColumnSortData& B )
{
	return MLSortOrder*appStricmp( *(A.SortString), *(B.SortString) );
}

void UGUIMultiColumnList::execSortList(FFrame& Stack, RESULT_DECL )
{
    guard(UGUIMultiColumnList::execSortList);
	P_FINISH;

	if( SortColumn == -1 )
		return;

	if( SortDescending )
		MLSortOrder = -1;
	else
		MLSortOrder = 1;

	Sort( &SortData(0), SortData.Num() );

	// update reverse map
	for( INT i=0;i<SortData.Num();i++ )
	{
		INT SortItem = SortData(i).SortItem;
		if( SortItem >= 0 && SortItem < InvSortData.Num() )
			InvSortData(SortItem) = i;
	}

	unguard;
}


// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIScrollText
// =======================================================================================================================================================
// =======================================================================================================================================================


//
// Get a character's dimensions.
//
static inline void GetCharSize( UFont* Font, TCHAR InCh, INT& Width, INT& Height )
{
	guardSlow(GetCharSize);
	Width = 0;
	Height = 0;
	INT Ch    = (TCHAR)Font->RemapChar(InCh);
	INT Page  = Ch / Font->CharactersPerPage;
	INT Index = Ch - Page * Font->CharactersPerPage;
	if( Page<Font->Pages.Num() && Index<Font->Pages(Page).Characters.Num() )
	{
		FFontCharacter& Char = Font->Pages(Page).Characters(Index);
		Width = Char.USize;
		Height = Char.VSize;
	}
	unguardSlow;
}

void UGUIScrollText::PreDraw(UCanvas* Canvas)
{
	guard(UGUIScrollText::PreDraw);
	if ( (!bVisible) || (Style == NULL) )
		return;

	ItemCount = Elements.Num();

	Super::PreDraw(Canvas);

	INT cWidth = Bounds[2] - Bounds[0] - Style->BorderOffsets[0] - Style->BorderOffsets[2];
	// Prepare our array of strings
	if (bNewContent || cWidth != oldWidth)
	{
	UFont *Font = Style->Fonts[MSAT_Blurry]->eventGetFont(Canvas->SizeX);

		// Carve big string into seperate strings of the right length
		TArray<FString> Strings;
		Canvas->WrapStringToArray(*Content, &Strings, cWidth, Font, **Separator);

		// Then fill array of elements with each string
		Elements.Empty();
		for(INT i=0; i<Strings.Num(); i++)
		{
			Elements.AddZeroed();
			Elements(i).item = Strings(i);
		}
		ItemCount = Elements.Num();

		if (ItemCount == 0)
			VisibleLines = -1;
		else
			VisibleLines=0;
		VisibleChars=0;
		oldWidth = cWidth;
		bNewContent = false;
	}
	unguard;
}

void UGUIScrollText::Draw(UCanvas *Canvas)
{
	guard(UGUIScrollText::Draw);

	if ( (!bVisible) || (Style==NULL) )
		return;

	if (!bStopped)
	{
//		bForceHideSB = true;

		// Find our best Top
		INT NewTop = Max(VisibleLines - ItemsPerPage + 1, 0);
		if (NewTop>=0)
		{
			if (Top != NewTop)
			{
				Top = NewTop;
				if (MyScrollBar && Elements.Num()>0)
					MyScrollBar->eventAlignThumb();
			}
		}

		INT     oldCnt = ItemCount;
		ItemCount = VisibleLines+1;
		Index = -1;

		Super::Draw(Canvas);

		ItemCount = oldCnt;
	}
	else
	{
		ItemCount = Elements.Num();
		Index = -1;
		Super::Draw(Canvas);
	}
//	if (bForceHideSB && MyScrollBar)
//			MyScrollBar->bVisible = false;

	unguard;
}

void UGUIScrollText::DrawItem(UCanvas* Canvas, INT Item, FLOAT X, FLOAT Y, FLOAT W, FLOAT H)
{
INT XL, YL;

	guard(UGUIScrollText::DrawItem)
	if (!bStopped && Item == VisibleLines)
	{
		Style->DrawText(Canvas, MenuState, X,Y,W,H, TextAlign, *(Elements(Item).item.Left(VisibleChars)));
		// Display a simple large caret
		Style->TextSize( Canvas, MenuState, *(Elements(Item).item.Left(VisibleChars)), XL, YL );
		switch(TextAlign)
		{
		case TXTA_Left:	X += XL;				break;
		case TXTA_Center: X += ((W + XL) / 2); 	break;
		case TXTA_Right: X += W;				break;
		}
		Canvas->Style=5;	// Alpha
		Canvas->DrawTile(Controller->DefaultPens[0], X, Y, YL/2,YL,0,0,1,1,0.0,FColor(255,255,255,Controller->FastCursorFade),FPlane(0,0,0,0));
	}
	else
		Style->DrawText(Canvas, MenuState, X,Y,W,H, TextAlign, *(Elements(Item).item));
	unguard;

}

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUITabPanel
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUITabPanel::PreDraw(UCanvas* Canvas)
{
	guard(UGUTabPanel::PreDraw);

	if (bFillHeight)
	{
		WinHeight = Canvas->ClipY - ActualTop();
	}

	Super::PreDraw(Canvas);

	unguard;
}
