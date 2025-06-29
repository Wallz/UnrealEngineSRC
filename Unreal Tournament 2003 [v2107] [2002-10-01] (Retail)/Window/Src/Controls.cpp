/*=============================================================================
	Controls.cpp: Control implementations
	Copyright 1997-2001 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Warren Marshall
=============================================================================*/

#pragma warning( disable : 4201 )
#define STRICT
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include "Engine.h"
#include "Window.h"

#define WPS_TABS	19900

/*-----------------------------------------------------------------------------
	WTabControl.
-----------------------------------------------------------------------------*/

void WTabControl::OpenWindow( UBOOL Visible, DWORD dwExtraStyle )
{
	guard(WTabControl::OpenWindow);
	PerformCreateWindowEx
	(
		0,
        NULL,
        WS_CHILD | (Visible?WS_VISIBLE:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

void WTabControl::AddTab( FString InText, INT InlParam )
{
	guard(WTabControl::AddTab);

	TCHAR Text[256] = TEXT("");
	appStrcpy( Text, *InText );

	TCITEM tci;

	tci.mask = TCIF_TEXT | TCIF_PARAM;
	tci.pszText = Text;
	tci.lParam = InlParam;

	SendMessageX( *this, TCM_INSERTITEM, GetCount(), (LPARAM)(const LPTCITEM)&tci );

	unguard;
}

void WTabControl::Empty()
{
	guard(WTabControl::Empty);
	SendMessageX( *this, TCM_DELETEALLITEMS, 0, 0 );
	unguard;
}

INT WTabControl::GetCount()
{
	guard(WTabControl::GetCount);
	return SendMessageX( *this, TCM_GETITEMCOUNT, 0, 0 );
	unguard;
}

INT WTabControl::SetCurrent( INT Index )
{
	guard(WTabControl::SetCurrent);
	return SendMessageX( *this, TCM_SETCURSEL, Index, 0 );
	unguard;
}

INT WTabControl::GetCurrent()
{
	guard(WTabControl::GetCurrent);
	return SendMessageX( *this, TCM_GETCURFOCUS, 0, 0 );
	unguard;
}

INT WTabControl::GetIndexFromlParam( INT InlParam )
{
	guard(WTabControl::GetIndexFromlParam);

	TCITEM tci;
	tci.mask = TCIF_PARAM;

	for( INT x = 0 ; x < GetCount() ; x++ )
	{
		SendMessageX( *this, TCM_GETITEM, x, (LPARAM)(LPTCITEM)&tci );
		if( tci.lParam == InlParam )
			return x;
	}

	return -1;
	unguard;
}

FString WTabControl::GetString( INT Index )
{
	guard(WTabControl::GetIndexFromlParam);

	TCHAR Buffer[256] = TEXT("");

	TCITEM tci;
	tci.mask = TCIF_TEXT;
	tci.pszText = Buffer;
	tci.cchTextMax = 256;

	SendMessageX( *this, TCM_GETITEM, Index, (LPARAM)(LPTCITEM)&tci );

	return FString( Buffer );
	unguard;
}

INT WTabControl::GetlParam( INT Index )
{
	guard(WTabControl::GetlParam);

	TCITEM tci;
	tci.mask = TCIF_PARAM;

	SendMessageX( *this, TCM_GETITEM, Index, (LPARAM)(LPTCITEM)&tci );

	return tci.lParam;
	unguard;
}

UBOOL WTabControl::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WTabControl::InterceptControlCommand);

	LastwParam = wParam;
	LastlParam = lParam;

	NMHDR* pnmhdr = (LPNMHDR)lParam;

	if( pnmhdr->code == TCN_SELCHANGE)
		SelectionChangeDelegate();

	return 0;
	unguard;
}

/*-----------------------------------------------------------------------------
	WLabel.
-----------------------------------------------------------------------------*/

void WLabel::OpenWindow( UBOOL Visible, UBOOL ClientEdge, DWORD dwExtraStyle )
{
	guard(WLabel::OpenWindow);
	PerformCreateWindowEx
	(
		(ClientEdge?WS_EX_CLIENTEDGE:0),
        NULL,
        WS_CHILD | (Visible?WS_VISIBLE:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

/*-----------------------------------------------------------------------------
	WGroupBox.
-----------------------------------------------------------------------------*/

void WGroupBox::OpenWindow( UBOOL Visible, DWORD dwExtraStyle )
{
	guard(WGroupBox::OpenWindow);
	PerformCreateWindowEx
	(
		0,
        NULL,
        BS_GROUPBOX | WS_CHILD | (Visible?WS_VISIBLE:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

/*-----------------------------------------------------------------------------
	WPropertyPage.
-----------------------------------------------------------------------------*/

void WPropertyPage::OpenWindow( INT InDlgId, HMODULE InHMOD )
{
	guard(WPropertyPage ::OpenWindow);
	MdiChild = 0;

	id = InDlgId;

	PerformCreateWindowEx
	(
		0,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0,
		0, 0,
		OwnerWindow ? OwnerWindow->hWnd : NULL,
		NULL,
		hInstance
	);
	SendMessageX( hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );

	if( InDlgId )
	{
		check(InHMOD);

		// Create an invisible dialog box from the specified resource id, and read the control
		// positions from it.  Put these positions into an array for future reference.
		char ResName[80] = "";
		strcpy( ResName, appToAnsi( *FString::Printf(TEXT("#%d"), InDlgId) ) );
		HRSRC hrsrc = FindResourceA( InHMOD, ResName, (LPSTR)RT_DIALOG );
		check(hrsrc);

		HGLOBAL hgbl = LoadResource( InHMOD, hrsrc );
		check(hgbl);

		DLGTEMPLATE* dlgtemplate = (DLGTEMPLATE*)LockResource(hgbl);
		check(dlgtemplate);

		HWND hwndDlg = ::CreateDialogIndirectA( hInstance, dlgtemplate, hWnd, NULL );
		check(hwndDlg);	// Specified ID is NOT a valid dialog resource.

		char TempCaption[80] = "";
		GetWindowTextA( hwndDlg, TempCaption, 80 );
		Caption = appFromAnsi( TempCaption );
		for( HWND ChildWnd = ::GetWindow( hwndDlg, GW_CHILD ) ; ChildWnd ; ChildWnd = ::GetNextWindow( ChildWnd, GW_HWNDNEXT ) )
		{
			RECT rect;
			::GetWindowRect( ChildWnd, &rect );
			::ScreenToClient( hwndDlg, (POINT*)&rect.left );
			::ScreenToClient( hwndDlg, (POINT*)&rect.right );

			INT id = GetDlgCtrlID( ChildWnd );

			::GetWindowTextA( ChildWnd, TempCaption, 80 );
			LONG Style, ExStyle;
			Style = GetWindowLongX( ChildWnd, GWL_STYLE );
			ExStyle = GetWindowLongX( ChildWnd, GWL_EXSTYLE );
			new( Ctrls )WPropertyPageCtrl( id, rect, appFromAnsi( TempCaption ), Style, ExStyle );
		}

		// Get the size of the dialog resource and use this as the default size of the property page.
		RECT rectDlg;
		::GetClientRect( hwndDlg, &rectDlg );
		::MoveWindow( hWnd, 0, 0, rectDlg.right, rectDlg.bottom, 1);

		UnlockResource( dlgtemplate );
	}
	unguard;
}

// Places a control based on the position that it previously read from the dialog template
void WPropertyPage::PlaceControl( WControl* InControl )
{
	guard(WPropertyPage::PlaceControl);

	for( INT x = 0 ; x < Ctrls.Num() ; x++ )
		if( Ctrls(x).id  == InControl->ControlId )
		{
			InControl->SetText( *Ctrls(x).Caption );
			SetWindowLongX( InControl->hWnd, GWL_STYLE, Ctrls(x).Style );
			SetWindowLongX( InControl->hWnd, GWL_EXSTYLE, Ctrls(x).ExStyle );
			::MoveWindow( InControl->hWnd, Ctrls(x).rect.left, Ctrls(x).rect.top, Ctrls(x).rect.right - Ctrls(x).rect.left, Ctrls(x).rect.bottom - Ctrls(x).rect.top, 1 );
			Ctrls.Remove(x);
			break;
		}

	unguard;
}

// Loops through the control list and for any controls that are left over, create static 
// controls for them and place them.
void WPropertyPage::Finalize()
{
	guard(WPropertyPage::Finalize);
	for( INT x = 0 ; x < Ctrls.Num() ; x++ )
	{
		WLabel* lbl = new WLabel( this, Ctrls(x).id );
		Labels.AddItem( lbl );
		lbl->OpenWindow( 1, 0 );
		RECT rect = Ctrls(x).rect;
		::MoveWindow( lbl->hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1 );
		lbl->SetText( *Ctrls(x).Caption );
		SetWindowLongX( lbl->hWnd, GWL_STYLE, Ctrls(x).Style );
		SetWindowLongX( lbl->hWnd, GWL_EXSTYLE, Ctrls(x).ExStyle );
	}
	unguard;
}

void WPropertyPage::Cleanup()
{
	guard(WPropertyPage::Cleanup);
	for( INT x = 0 ; x < Labels.Num() ; x++ )
	{
		DestroyWindow( Labels(x)->hWnd );
		delete Labels(x);
	}
	unguard;
}

FString WPropertyPage::GetCaption()
{
	guard(WPropertyPage::GetCaption);
	return Caption;
	unguard;
}

INT WPropertyPage::GetID()
{
	guard(WPropertyPage::GetID);
	return id;
	unguard;
}

void WPropertyPage::Refresh()
{
}

/*-----------------------------------------------------------------------------
	WPropertySheet.
-----------------------------------------------------------------------------*/

void WPropertySheet::OpenWindow( UBOOL Visible, UBOOL InResizable, DWORD dwExtraStyle )
{
	guard(WPropertySheet::OpenWindow);
	bResizable = InResizable;
	PerformCreateWindowEx
	(
		0,
        NULL,
        WS_CHILD | (Visible?WS_VISIBLE:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

void WPropertySheet::OnCreate()
{
	guard(WPropertySheet::OnCreate);
	WControl::OnCreate();

	Tabs = new WTabControl( this, WPS_TABS );
	Tabs->OpenWindow( 1, bMultiLine ? TCS_MULTILINE : 0 );
	Tabs->SelectionChangeDelegate = FDelegate(this, (TDelegate)OnTabsSelChange);

	unguard;
}

void WPropertySheet::OnSize( DWORD Flags, INT NewX, INT NewY )
{
	guard(WPropertySheet::OnSize);
	WControl::OnSize(Flags, NewX, NewY);

	RECT rect;
	::GetClientRect( *this, &rect );
	::MoveWindow( Tabs->hWnd, 4, 4, rect.right-8, rect.bottom-8, 1 );

	RefreshPages();

	InvalidateRect( hWnd, NULL, 1 );
	unguard;
}

void WPropertySheet::RefreshPages()
{
	guard(WPropertySheet::RefreshPages);

	if( !Pages.Num() ) return;

	INT idx = Tabs->GetCurrent();
	if( idx == -1 )	idx = 0;
	check( idx < Pages.Num() );

	RECT rect;
	::GetClientRect( *this, &rect );

	RECT tabrect;
	SendMessageX( *Tabs, TCM_GETITEMRECT, idx, (LPARAM)(&tabrect) );

	// Hide all pages, and show the current one.
	for( INT x = 0 ; x < Pages.Num() ; x++ )
	{
		if( x == idx )
			Pages(x)->Show( 1 );
		else
			Pages(x)->Show( 0 );
		Pages(x)->Refresh();

		// Set the pages size
		if( bResizable )
			::SetWindowPos( Pages(idx)->hWnd, HWND_TOP, 0, 0, rect.right-24, rect.bottom-24-tabrect.bottom, SWP_NOMOVE );
	}

	// Move the current page to the top
	::SetWindowPos( Pages(idx)->hWnd, HWND_TOP, 8, tabrect.bottom+8, 0,0, SWP_NOSIZE );

	unguard;
}

void WPropertySheet::Empty()
{
	guard(WPropertySheet::Empty);
	Tabs->Empty();
	unguard;
}

void WPropertySheet::AddPage( WPropertyPage* InPage )
{
	guard(WPropertySheet::AddPage);
	Pages.AddItem( InPage );
	Tabs->AddTab( InPage->GetCaption(), InPage->GetID() );
	unguard;
}

INT WPropertySheet::SetCurrent( INT Index )
{
	guard(WPropertySheet::SetCurrent);
	INT ret = Tabs->SetCurrent( Index );
	RefreshPages();
	if( ParentContainer )
		ParentContainer->RefreshControls();
	return ret;
	unguard;
}

INT WPropertySheet::SetCurrent( WPropertyPage* Page )
{
	guard(WPropertySheet::SetCurrent);
	INT newpage = Pages.FindItemIndex(Page);
	if( newpage != INDEX_NONE )
		return SetCurrent( newpage );
	return -1;
	unguard;
}

void WPropertySheet::OnTabsSelChange()
{
	guard(WPropertySheet::OnTabsSelChange);
	RefreshPages();
	if( ParentContainer )
		ParentContainer->RefreshControls();
	unguard;
}

/*-----------------------------------------------------------------------------
	WCustomLabel.
-----------------------------------------------------------------------------*/

void WCustomLabel::OnPaint()
{
	guard(WCustomLabel::OnPaint);
	PAINTSTRUCT PS;
	HDC hDC = BeginPaint( *this, &PS );

	FRect Rect = GetClientRect();
	FillRect( hDC, Rect, hBrushGrey );

	HFONT OldFont = (HFONT)SelectObject( hDC, GetStockObject(DEFAULT_GUI_FONT) );

	FString Caption = GetText();
	::SetBkMode( hDC, TRANSPARENT );
	DrawTextX( hDC, *Caption, Caption.Len(), Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	SelectObject( hDC, OldFont );

	EndPaint( *this, &PS );
	unguard;
}

/*-----------------------------------------------------------------------------
	WButton.
-----------------------------------------------------------------------------*/

void WButton::OpenWindow( UBOOL Visible, INT X, INT Y, INT XL, INT YL, const TCHAR* Text, UBOOL bBitmap, DWORD dwExtraStyle )
{
	guard(WButton::OpenWindow);
	PerformCreateWindowEx
	(
		0,
        NULL,
        WS_CHILD | BS_PUSHBUTTON | (bBitmap?BS_BITMAP:0) | dwExtraStyle,
        X, Y,
		XL, YL,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	SetText( Text );
	if( Visible )
		ShowWindow( *this, SW_SHOWNOACTIVATE );
	bOwnerDraw = (dwExtraStyle & BS_OWNERDRAW);
	unguard;
}

void WButton::SetVisibleText( const TCHAR* Text )
{
	guard(WButton::SetText);
	check(hWnd);
	if( Text )
		SetText( Text );
	Show( Text!=NULL );
	unguard;
}

void WButton::SetBitmap( HBITMAP Inhbm )
{
	guard(WButton::SetText);
	SendMessageA( hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Inhbm);
	hbm = Inhbm;
	InvalidateRect( hWnd, NULL, 1 );
	unguard;
}

void WButton::Clicked()
{
	guard(WButton::Clicked);
	SendMessageX( OwnerWindow->hWnd, WM_COMMAND, WM_PB_PUSH, ControlId );
	unguard;
}

void WButton::OnDrawItem( DRAWITEMSTRUCT* Item )
{
	guard(WButton::OnDrawItem);
	RECT R=Item->rcItem;
	UBOOL bPressed = (Item->itemState&ODS_SELECTED)!=0 || bChecked;

	FillRect( Item->hDC, &Item->rcItem, hBrushGrey );

	MyDrawEdge( Item->hDC, &R, !bPressed );

	HDC hdcMem;
	HBITMAP hbmOld;

	hdcMem = CreateCompatibleDC(Item->hDC);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
	BITMAP bitmap;
	GetObjectA( hbm, sizeof(BITMAP), (LPSTR)&bitmap );

	INT Adjust = (bPressed) ? 2 : 0;

	BitBlt(Item->hDC,
		(R.right / 2) - (bitmap.bmWidth / 2) + Adjust, (R.bottom / 2) - (bitmap.bmHeight / 2) + Adjust,
		bitmap.bmWidth, bitmap.bmHeight,
		hdcMem,
		0, 0,
		SRCCOPY);

	// If this button is checked, draw a light border around in the inside to make it obvious.
	if( bPressed )
	{
		HPEN penOld, pen = CreatePen( PS_SOLID, 1, RGB(145,210,198) );

		RECT rc = R;
		rc.left++;	rc.top++;	rc.right--;		rc.bottom--;

		penOld = (HPEN)SelectObject( Item->hDC, pen );
		::MoveToEx( Item->hDC, rc.left, rc.top, NULL );
		::LineTo( Item->hDC, rc.right, rc.top );
		::LineTo( Item->hDC, rc.right, rc.bottom );
		::LineTo( Item->hDC, rc.left, rc.bottom );
		::LineTo( Item->hDC, rc.left, rc.top );
		SelectObject( Item->hDC, penOld );

		DeleteObject( pen );
	}

	// Clean up.
	//
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	unguard;
}

BOOL WButton::IsChecked( void )
{
	guard(WButton::IsChecked);

	// Owner draw buttons require us to keep track of the checked status ourselves.
	if( bOwnerDraw )
		return bChecked;
	else
		return ( SendMessageLX( *this, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
	unguard;
}

void WButton::SetCheck( INT iCheck )
{
	guard(WButton::SetCheck);
	SendMessageLX( *this, BM_SETCHECK, (WPARAM)iCheck, 0 );
	bChecked = (iCheck == BST_CHECKED);
	InvalidateRect( hWnd, NULL, 1 );
	unguard;
}

UBOOL WButton::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WButton::InterceptControlCommand);
	if     ( HIWORD(wParam)==BN_CLICKED   )
	{
		Clicked();
		ClickDelegate();
		// This notification returns 0 instead of 1 because the editor sometimes wants
		// to know about clicks without using a delegate (i.e. dynamically created buttons)
		return 0;
	}
	else if( HIWORD(wParam)==BN_DBLCLK    ) {DoubleClickDelegate(); return 1;}
	else if( HIWORD(wParam)==BN_PUSHED    ) {PushDelegate();        return 1;}
	else if( HIWORD(wParam)==BN_UNPUSHED  ) {UnPushDelegate();      return 1;}
	else if( HIWORD(wParam)==BN_SETFOCUS  ) {SetFocusDelegate();    return 1;}
	else if( HIWORD(wParam)==BN_KILLFOCUS ) {UnPushDelegate();      return 1;}
	else return 0;
	unguard;
}

/*-----------------------------------------------------------------------------
	WBitmapButton.
-----------------------------------------------------------------------------*/

void WBitmapButton::OpenWindow( UBOOL Visible, DWORD InType, DWORD dwExtraStyle, HBITMAP InhbmSource, FRect InEnabledRect, FRect InDownRect, FRect InDisabledRect )
{
	guard(WBitmapButton::OpenWindow);
	bIsAutoCheckBox = (InType == BS_AUTOCHECKBOX);
	PerformCreateWindowEx
	(
		0,
        NULL,
        InType | BS_OWNERDRAW | WS_CHILD | WS_CLIPCHILDREN | (Visible?WS_VISIBLE:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	hbmSource = InhbmSource;
	EnabledRect = InEnabledRect;
	DownRect = InDownRect;
	DisabledRect = InDisabledRect;
	unguard;
}

void WBitmapButton::OnDestroy()
{
	guard(WBitmapButton::OnDestroy);
	WButton::OnDestroy();

	unguard;
}

void WBitmapButton::OnDrawItem( DRAWITEMSTRUCT* Item )
{
	guard(WBitmapButton::OnDrawItem);

	RECT R = Item->rcItem;
	UBOOL bPressed = (Item->itemState&ODS_SELECTED)!=0 || bChecked;

	FillRect( Item->hDC, &Item->rcItem, hBrushGrey );

	HDC hdcMem;
	HBITMAP hbmOld;

	FRect* ButtonRect = &EnabledRect;
	if( !IsWindowEnabled(hWnd) )
		ButtonRect = &DisabledRect;
	else if( bPressed )
		ButtonRect = &DownRect;

	INT Width, Height, Adjust;
	Width = EnabledRect.Max.X - EnabledRect.Min.X;
	Height = EnabledRect.Max.Y - EnabledRect.Min.Y;
	Adjust = (bPressed) ? 2 : 0;

	hdcMem = CreateCompatibleDC(Item->hDC);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmSource);

	BitBlt(Item->hDC,
		Adjust, Adjust,
		Width, Height,
		hdcMem,
		ButtonRect->Min.X, ButtonRect->Min.Y,
		SRCCOPY);

	MyDrawEdge( Item->hDC, &R, !bPressed );

	// If this button is checked, draw a light border around in the inside to make it obvious.
	if( bPressed )
	{
		HPEN penOld, pen = CreatePen( PS_SOLID, 1, UEDCOLOR_CYAN_HILIGHT );

		RECT rc = R;
		rc.left++;	rc.top++;	rc.right--;		rc.bottom--;

		penOld = (HPEN)SelectObject( Item->hDC, pen );
		::MoveToEx( Item->hDC, rc.left, rc.top, NULL );
		::LineTo( Item->hDC, rc.right, rc.top );
		::LineTo( Item->hDC, rc.right, rc.bottom );
		::LineTo( Item->hDC, rc.left, rc.bottom );
		::LineTo( Item->hDC, rc.left, rc.top );
		SelectObject( Item->hDC, penOld );

		DeleteObject( pen );
	}

	// Clean up.
	//
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	unguard;
}

void WBitmapButton::Clicked(void)
{
	guard(WBitmapButton::Clicked);
	if( bIsAutoCheckBox )
		bChecked = !bChecked;
	else
		bChecked = FALSE;
	InvalidateRect( hWnd, NULL, FALSE );
	unguard;
}

/*-----------------------------------------------------------------------------
	WColorButton.
-----------------------------------------------------------------------------*/

void WColorButton::OpenWindow( UBOOL Visible, DWORD dwExtraStyle )
{
	guard(WColorButton::OpenWindow);
	PerformCreateWindowEx
	(
		0,
        NULL,
        BS_PUSHBUTTON | BS_OWNERDRAW | WS_CHILD | WS_CLIPCHILDREN | (Visible?WS_VISIBLE:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	unguard;
}

void WColorButton::OnDrawItem( DRAWITEMSTRUCT* Item )
{
	guard(WColorButton::OnDrawItem);

	RECT rect = Item->rcItem;
	UBOOL bPressed = (Item->itemState&ODS_SELECTED)!=0;

	HBRUSH ColorBrush = CreateSolidBrush( RGB(R,G,B) ), OldBrush;
	FillRect( Item->hDC, &Item->rcItem, hBrushGrey );

	//DrawEdge( Item->hDC, &rect, !bPressed );
	DrawEdge( Item->hDC, &rect, bPressed ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT );

	OldBrush = (HBRUSH)SelectObject( Item->hDC, ColorBrush );
	INT Adjust = (bPressed) ? 2 : 0;
	rect.left += 2 + Adjust;
	rect.top += 2 + Adjust;
	rect.right -= 2 + Adjust;
	rect.bottom -= 2 + Adjust;

	Rectangle( Item->hDC, rect.left, rect.top, rect.right, rect.bottom );

	// Clean up.
	//
	SelectObject( Item->hDC, OldBrush );
	DeleteObject( ColorBrush );

	unguard;
}

void WColorButton::SetColor( INT InR, INT InG, INT InB )
{
	R = InR;
	G = InG;
	B = InB;
	InvalidateRect( hWnd, NULL, FALSE );
}

void WColorButton::GetColor( INT& InR, INT& InG, INT& InB )
{
	InR = R;
	InG = G;
	InB = B;
}

/*-----------------------------------------------------------------------------
	WToolTip.
-----------------------------------------------------------------------------*/

void WToolTip::OpenWindow()
{
	guard(WToolTip::OpenWindow);
	PerformCreateWindowEx
	(
		0,
        NULL,
        TTS_ALWAYSTIP,
        0, 0,
		0, 0,
        NULL,
        (HMENU)NULL,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

void WToolTip::AddTool( HWND InHwnd, FString InToolTip, INT InId, RECT* InRect )
{
	guard(WToolTip::AddTool);

	check(::IsWindow( hWnd ));

	RECT rect;
	::GetClientRect( InHwnd, &rect );

	TOOLINFO ti;
	TCHAR chToolTip[128] = TEXT("");
	appStrcpy( chToolTip, *InToolTip );
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = InHwnd;
	ti.hinst = hInstance;
	ti.uId = ControlId + InId;
	ti.lpszText = chToolTip;
	if( InRect )
		ti.rect = *InRect;
	else
		ti.rect = rect;

	SendMessageX(hWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
	unguard;
}

/*-----------------------------------------------------------------------------
	WPictureButton.
-----------------------------------------------------------------------------*/

void WPictureButton::OpenWindow()
{
	guard(WPictureButton::OpenWindow);
	MdiChild = 0;

	// HAVE to call "SetUp" before opening the window
	check(bHasBeenSetup);

	PerformCreateWindowEx
	(
		0,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		ClientPos.left,
		ClientPos.top,
		ClientPos.right,
		ClientPos.bottom,
		OwnerWindow ? OwnerWindow->hWnd : NULL,
		NULL,
		hInstance
	);
	SendMessageX( hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );

	ToolTipCtrl = new WToolTip(this);
	ToolTipCtrl->OpenWindow();

	unguard;
}

void WPictureButton::OnSize( DWORD Flags, INT NewX, INT NewY )
{
	guard(WPictureButton::OnSize);
	WWindow::OnSize(Flags, NewX, NewY);

	RECT rect;
	::GetClientRect( hWnd, &rect );

	if( ToolTipCtrl && ToolTipText.Len() )
		ToolTipCtrl->AddTool( hWnd, ToolTipText, 0 );

	unguard;
}

void WPictureButton::OnDestroy()
{
	guard(WPictureButton::OnDestroy);
	WWindow::OnDestroy();
	delete ToolTipCtrl;
	unguard;
}

void WPictureButton::OnPaint()
{
	guard(WPictureButton::OnPaint);
	PAINTSTRUCT PS;
	HDC hDC = BeginPaint( *this, &PS );

	RECT rc;
	::GetClientRect( hWnd, &rc );

	HDC hdcMem;
	HBITMAP hbmOld;

	hdcMem = CreateCompatibleDC(hDC);
	hbmOld = (HBITMAP)SelectObject(hdcMem, (bOn ? hbmOn : hbmOff));

	RECT BmpPos = (bOn ? BmpOnPos : BmpOffPos);

	BitBlt(hDC,
		(rc.right - BmpPos.right) / 2, (rc.bottom - BmpPos.bottom) / 2,
		BmpPos.right, BmpPos.bottom,
		hdcMem,
		BmpPos.left, BmpPos.top,
		SRCCOPY);

	// Clean up.
	//
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	EndPaint( *this, &PS );
	unguard;
}

void WPictureButton::OnLeftButtonDown()
{
	guard(WPictureButton::OnLeftButtonDown);
	SendMessageX( OwnerWindow->hWnd, WM_COMMAND, WM_PB_PUSH, ID );
	unguard;
}

void WPictureButton::SetUp( FString InToolTipText, INT InID, 
	INT InClientLeft, INT InClientTop, INT InClientRight, INT InClientBottom,
	HBITMAP InHbmOff, INT InBmpOffLeft, INT InBmpOffTop, INT InBmpOffRight, INT InBmpOffBottom,
	HBITMAP InHbmOn, INT InBmpOnLeft, INT InBmpOnTop, INT InBmpOnRight, INT InBmpOnBottom )
{
	guard(WPictureButton::SetUp);

	bHasBeenSetup = 1;

	ToolTipText = InToolTipText;
	ID = InID;
	ClientPos.left = InClientLeft;
	ClientPos.top = InClientTop;
	ClientPos.right = InClientRight;
	ClientPos.bottom = InClientBottom;
	hbmOff = InHbmOff;
	BmpOffPos.left = InBmpOffLeft;
	BmpOffPos.top = InBmpOffTop;
	BmpOffPos.right = InBmpOffRight;
	BmpOffPos.bottom = InBmpOffBottom;
	hbmOn = InHbmOn;
	BmpOnPos.left = InBmpOnLeft;
	BmpOnPos.top = InBmpOnTop;
	BmpOnPos.right = InBmpOnRight;
	BmpOnPos.bottom = InBmpOnBottom;

	unguard;
}

/*-----------------------------------------------------------------------------
	WCheckBox.
-----------------------------------------------------------------------------*/

void WCheckBox::OpenWindow( UBOOL Visible, INT X, INT Y, INT XL, INT YL, const TCHAR* Text, UBOOL InbAutocheck, UBOOL bBitmap, DWORD dwExtraStyle )
{
	guard(WCheckBox::OpenWindow);
	bAutocheck = InbAutocheck;
	PerformCreateWindowEx
	(
		0,
        NULL,
        WS_CHILD | BS_CHECKBOX | (bAutocheck?BS_AUTOCHECKBOX:0) | (bBitmap?BS_BITMAP:0) | dwExtraStyle,
        X, Y,
		XL, YL,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
	SetText( Text );
	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	if( Visible )
		ShowWindow( *this, SW_SHOWNOACTIVATE );
	bOwnerDraw = (dwExtraStyle & BS_OWNERDRAW);

	unguard;
}

void WCheckBox::OnCreate()
{
	guard(WCheckBox::OnCreate);
	WButton::OnCreate();
	bOwnerDraw = 0;
	unguard;
}

void WCheckBox::OnRightButtonDown()
{
	guard(WCheckBox::OnRightButtonDown);
	// This is just a quick hack so the parent window can know when the user right clicks on
	// one of the buttons in the UnrealEd button bar.
	SendMessageX( ::GetParent( hWnd ), WM_COMMAND, WM_BB_RCLICK, ControlId );
	unguard;
}

void WCheckBox::Clicked()
{
	guard(WCheckBox::Clicked);
	WButton::Clicked();
	if( bAutocheck )
		bChecked = !bChecked;
	else
		bChecked = FALSE;
	unguard;
}

/*-----------------------------------------------------------------------------
	WScrollBar.
-----------------------------------------------------------------------------*/

void WScrollBar::OpenWindow( UBOOL Visible, INT X, INT Y, INT XL, INT YL, UBOOL bVertical )
{
	guard(WScrollBar::OpenWindow);
	PerformCreateWindowEx
	(
		0,
        NULL,
        WS_CHILD | (bVertical ? SBS_VERT : SBS_HORZ),
        X, Y,
		XL, YL,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	if( Visible )
		ShowWindow( *this, SW_SHOWNOACTIVATE );
	unguard;
}

/*-----------------------------------------------------------------------------
	WTreeView.
-----------------------------------------------------------------------------*/

void WTreeView::OpenWindow( UBOOL Visible, INT X, INT Y, INT XL, INT YL, DWORD dwExtraStyle )
{
	guard(WTreeView::OpenWindow);
	PerformCreateWindowEx
	(
		WS_EX_CLIENTEDGE,
        NULL,
        WS_CHILD | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP | dwExtraStyle,
        X, Y,
		XL, YL,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	if( Visible )
		ShowWindow( *this, SW_SHOWNOACTIVATE );
	unguard;
}

void WTreeView::Empty( void )
{
	guard(WTreeView::Empty);
	SendMessageX( *this, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT );
	unguard;
}

HTREEITEM WTreeView::AddItem( const TCHAR* pName, HTREEITEM hti, UBOOL bHasChildren )
{
	guard(WTreeView::AddItem);

	TCHAR chName[256] = TEXT("\0");

	appStrcpy( chName, pName );

	TV_INSERTSTRUCT	InsertInfo;

	InsertInfo.hParent = hti;
	InsertInfo.hInsertAfter = TVI_LAST;

	InsertInfo.item.mask = TVIF_TEXT;
	InsertInfo.item.pszText = chName;

	if( bHasChildren )
	{
		InsertInfo.item.mask |= TVIF_CHILDREN;
		InsertInfo.item.cChildren = 1;
	}

	return (HTREEITEM)SendMessageX( *this, TVM_INSERTITEM, 0, (LPARAM) &InsertInfo );

	unguard;
}

void WTreeView::OnRightButtonDown()
{
	guard(WTreeView::OnRightButtonDown);
	PostMessageX( OwnerWindow->hWnd, WM_COMMAND, WM_TREEVIEW_RIGHT_CLICK, 0 );
	unguard;
}

UBOOL WTreeView::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WTreeView::InterceptControlCommand);

	LastwParam = wParam;
	LastlParam = lParam;

	NMTREEVIEW* pntv = (LPNMTREEVIEW)lParam;

	if( pntv->hdr.code==TVN_SELCHANGEDA
			|| pntv->hdr.code==TVN_SELCHANGEDW )
		SelChangedDelegate();
	if( pntv->hdr.code==TVN_ITEMEXPANDINGA
			|| pntv->hdr.code==TVN_ITEMEXPANDINGW )
		ItemExpandingDelegate();
	if( pntv->hdr.code==NM_DBLCLK )
	{
		DblClkDelegate();
		return 1;
	}

	return 0;
	unguard;
}

/*-----------------------------------------------------------------------------
	WCoolButton.
-----------------------------------------------------------------------------*/

void WCoolButton::OpenWindow( UBOOL Visible, INT X, INT Y, INT XL, INT YL, const TCHAR* Text )
{
	guard(WCoolButton::OpenWindow);
#ifndef JAPANESE
	PerformCreateWindowEx
	(
		0,
        NULL,
        WS_CHILD | BS_OWNERDRAW,
        X, Y,
		XL, YL,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
#else
	PerformCreateWindowEx
	(
		0,
        TEXT("BUTTON"),
        WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
        X, Y,
		XL, YL,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
#endif
	SetText( Text );

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	if( Visible )
		ShowWindow( *this, SW_SHOWNOACTIVATE );
	unguard;
}

void WCoolButton::OnDestroy()
{
	if( GlobalCoolButton==this )
		GlobalCoolButton=NULL;
	WButton::OnDestroy();
}

#ifndef JAPANESE
void WCoolButton::OnCreate()
{
	WButton::OnCreate();
	SetClassLongX( *this, GCL_STYLE, GetClassLongX(*this,GCL_STYLE) & ~CS_DBLCLKS);
}

void WCoolButton::UpdateHighlight( UBOOL TurnOff )
{
	guard(WCoolButton::UpdateHighlight);
	FPoint P;
	FRect R;
	::GetCursorPos((POINT*)&P);
	::GetWindowRect(*this,(RECT*)&R);
	UBOOL ShouldHighlight = (R.Contains(P) || GetCapture()==hWnd) && !TurnOff;
	if( (GlobalCoolButton==this) != (ShouldHighlight) )
	{
		if( GlobalCoolButton==this )
		{
			GlobalCoolButton = NULL;
			KillTimer( hWnd, 0 );
		}
		else
		{
			if( GlobalCoolButton )
				GlobalCoolButton->UpdateHighlight( 1 );
			GlobalCoolButton = this;
			SetTimer( hWnd, 0, 50, NULL );
		}
		InvalidateRect( *this, NULL, 1 );
		UpdateWindow( *this );
	}
	unguard;
}

void WCoolButton::OnTimer()
{
	guard(WCoolButton::OnTimer);
	UpdateHighlight(0);
	unguard;
}

INT WCoolButton::OnSetCursor()
{
	guard(WCoolButton::OnSetCursor);
	WButton::OnSetCursor();
	UpdateHighlight(0);
	if( FrameFlags & CBFF_UrlStyle )
		SetCursor(LoadCursorIdX(hInstanceWindow,IDC_Hand));
	return 1;
	unguard;
}

void WCoolButton::OnDrawItem( DRAWITEMSTRUCT* Item )
{
	guard(WCoolButton::OnDrawItem);
	RECT R=Item->rcItem;
	UBOOL Pressed = (Item->itemState&ODS_SELECTED)!=0;
	FillRect( Item->hDC, &Item->rcItem, (HBRUSH)(COLOR_BTNFACE+1));
	if( GlobalCoolButton==this )
	{
		if( FrameFlags & CBFF_ShowOver )
			DrawEdge( Item->hDC, &Item->rcItem, Pressed?EDGE_SUNKEN:EDGE_RAISED, BF_RECT );
	}
	else
	{
		if( FrameFlags & CBFF_DimAway )
			DrawEdge( Item->hDC, &Item->rcItem, Pressed?BDR_SUNKENINNER:BDR_RAISEDINNER, BF_RECT );
		else if( FrameFlags & CBFF_ShowAway )
			DrawEdge( Item->hDC, &Item->rcItem, Pressed?EDGE_SUNKEN:EDGE_RAISED, BF_RECT );
	}
	R.left += Pressed;
	R.right += Pressed;
	R.top += Pressed;
	R.bottom += Pressed;
	if( hIcon )
	{
		ICONINFO II;
		GetIconInfo( hIcon, &II );
		DrawIcon( Item->hDC, R.left, R.top + (R.bottom-R.top)/2-II.yHotspot, hIcon );
		R.left += II.xHotspot * 2;
	}
	FString Text = GetText();
	DWORD TextFlags
	=	DT_END_ELLIPSIS
	|	DT_VCENTER
	|	DT_SINGLELINE
	|	((FrameFlags & CBFF_NoCenter) ? 0 : DT_CENTER);
	HFONT OldFont;
	if( FrameFlags & CBFF_UrlStyle )
	{
		R.left += 8;
		SetTextColor( Item->hDC, RGB(0,0,255) );
		OldFont = SelectObject( Item->hDC, hFontUrl );
	}
	else
		OldFont = SelectObject( Item->hDC, hFontText );
	SetBkMode( Item->hDC, TRANSPARENT );
	DrawTextX( Item->hDC, *Text, Text.Len(), &R, TextFlags );
	SelectObject( Item->hDC, OldFont );
	unguard;
}
#endif

/*-----------------------------------------------------------------------------
	WUrlButton.
-----------------------------------------------------------------------------*/

void WUrlButton::OnClick()
{
	guard(WUrlButton::OnClick);
	ShellExecuteX( GetActiveWindow(), TEXT("open"), *URL, TEXT(""), TEXT(""), SW_SHOWNORMAL );
	unguard;
}

/*-----------------------------------------------------------------------------
	WComboBox.
-----------------------------------------------------------------------------*/

void WComboBox::OpenWindow( UBOOL Visible, UBOOL Sort, UINT InListType )
{
	guard(WComboBox::OpenWindow);
	PerformCreateWindowEx
	(
		0,
        NULL,
        WS_CHILD | WS_VSCROLL | InListType | (Sort?CBS_SORT:0) | (Visible?WS_VISIBLE:0),
        0, 0,
		64, 384,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

LONG WComboBox::WndProc( UINT Message, UINT wParam, LONG lParam )
{
	if( Message==WM_KEYDOWN && (wParam==VK_UP || wParam==VK_DOWN) )
	{
		// Suppress arrow keys.
		if( Snoop )
			Snoop->SnoopKeyDown( this, wParam );
		return 1;
	}
	else return WControl::WndProc( Message, wParam, lParam );
}

UBOOL WComboBox::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WComboBox::InterceptControlCommand);
	if     ( HIWORD(wParam)==CBN_DBLCLK         ) {DoubleClickDelegate();        return 1;}
	else if( HIWORD(wParam)==CBN_DROPDOWN       )
	{
		INT ItemHeight		= SendMessage( *this, CB_GETITEMHEIGHT, 0, 0 );
		INT SelectionHeight = SendMessage( *this, CB_GETITEMHEIGHT, -1, 0 );
		ResizeWindow( GetWindowRect().Width(), Min((GetCount()+1)*ItemHeight+SelectionHeight, 384), 0 );
		DropDownDelegate();
		return 1;
	}
	else if( HIWORD(wParam)==CBN_CLOSEUP        ) {CloseComboDelegate();         return 1;}
	else if( HIWORD(wParam)==CBN_EDITCHANGE     ) {EditChangeDelegate();         return 1;}
	else if( HIWORD(wParam)==CBN_EDITUPDATE     ) {EditUpdateDelegate();         return 1;}
	else if( HIWORD(wParam)==CBN_SETFOCUS       ) {SetFocusDelegate();           return 1;}
	else if( HIWORD(wParam)==CBN_KILLFOCUS      ) {KillFocusDelegate();          return 1;}
	else if( HIWORD(wParam)==CBN_SELCHANGE      ) {SelectionChangeDelegate();    return 1;}
	else if( HIWORD(wParam)==CBN_SELENDOK       ) {SelectionEndOkDelegate();     return 1;}
	else if( HIWORD(wParam)==CBN_SELENDCANCEL   ) {SelectionEndCancelDelegate(); return 1;}
	else return 0;
	unguard;
}

void WComboBox::AddString( const TCHAR* Str )
{
	guard(WComboBox::AddString);
	SendMessageLX( *this, CB_ADDSTRING, 0, Str );
	unguard;
}

FString WComboBox::GetString( INT Index )
{
	guard(WComboBox::GetString);
	INT Length = SendMessageX( *this, CB_GETLBTEXTLEN, Index, 0 );
	if( Length==CB_ERR )
		return TEXT("");
#if UNICODE
	if( GUnicode && !GUnicodeOS )
	{
		ANSICHAR* Text = (ANSICHAR*)appAlloca((Length+1)*sizeof(ANSICHAR));
		SendMessageA( *this, CB_GETLBTEXT, Index, (LPARAM)Text );
		return appFromAnsi( Text );
	}
	else
#endif
	{
		TCHAR* Text = (TCHAR*)appAlloca((Length+1)*sizeof(TCHAR));
		SendMessage( *this, CB_GETLBTEXT, Index, (LPARAM)Text );
		return Text;
	}
	unguard;
}

INT WComboBox::GetCount()
{
	guard(WComboBox::GetCount);
	return SendMessageX( *this, CB_GETCOUNT, 0, 0 );
	unguard;
}

void WComboBox::SetCurrent( INT Index )
{
	guard(WComboBox::SetCurrent);
	SendMessageX( *this, CB_SETCURSEL, Index, 0 );
	unguard;
}

void WComboBox::SetCurrent( const TCHAR* String )
{
	guard(WComboBox::SetCurrent);
	INT idx = FindStringExact( String );
	SendMessageX( *this, CB_SETCURSEL, idx, 0 );
	unguard;
}

INT WComboBox::GetCurrent()
{
	guard(WComboBox::GetCurrent);
	return SendMessageX( *this, CB_GETCURSEL, 0, 0 );
	unguard;
}

INT WComboBox::FindString( const TCHAR* String )
{
	guard(WComboBox::FindString);
	INT Index = SendMessageLX( *this, CB_FINDSTRING, -1, String );
	return Index!=CB_ERR ? Index : -1;
	unguard;
}

INT WComboBox::FindStringExact( const TCHAR* String )
{
	guard(WComboBox::FindString);
	INT Index = SendMessageLX( *this, CB_FINDSTRINGEXACT, -1, String );
	return Index!=CB_ERR ? Index : -1;
	unguard;
}

void WComboBox::Empty()
{
	guard(WComboBox::Empty);
	SendMessageLX( *this, CB_RESETCONTENT, 0, 0 );
	unguard;
}

/*-----------------------------------------------------------------------------
	WEdit.
-----------------------------------------------------------------------------*/

void WEdit::OpenWindow( UBOOL Visible, UBOOL Multiline, UBOOL ReadOnly, UBOOL HorizScroll, UBOOL NoHideSel )
{
	guard(WEdit::OpenWindow);
	PerformCreateWindowEx
	(
		WS_EX_CLIENTEDGE,
        NULL,
        WS_CHILD | (HorizScroll?WS_HSCROLL:0) | (Visible?WS_VISIBLE:0) | ES_LEFT | (Multiline?(ES_MULTILINE|WS_VSCROLL):0) | ES_AUTOVSCROLL | ES_AUTOHSCROLL | (ReadOnly?ES_READONLY:0) | (NoHideSel?ES_NOHIDESEL:0),
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

UBOOL WEdit::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WEdit::InterceptControlCommand);
	if( HIWORD(wParam)==EN_CHANGE )
	{
		ChangeDelegate();
		return 1;
	}
    // gam ---
    if( HIWORD(wParam)==EN_KILLFOCUS )
	{
		BlurDelegate();
		return 1;
	}
    // --- gam
	else return 0;
	unguard;
}

UBOOL WEdit::GetReadOnly()
{
	guard(WEdit::GetReadOnly);
	check(hWnd);
	return (GetWindowLongX( *this, GWL_STYLE )&ES_READONLY)!=0;
	unguard;
}

void WEdit::SetReadOnly( UBOOL ReadOnly )
{
	guard(WEdit::SetReadOnly);
	check(hWnd);
	SendMessageX( *this, EM_SETREADONLY, ReadOnly, 0 );
	unguard;
}

INT WEdit::GetLineCount()
{
	guard(WEdit::GetLineCount);
	check(hWnd);
	return SendMessageX( *this, EM_GETLINECOUNT, 0, 0 );
	unguard;
}

INT WEdit::GetLineIndex( INT Line )
{
	guard(WEdit::GetLineIndex);
	check(hWnd);
	return SendMessageX( *this, EM_LINEINDEX, Line, 0 );
	unguard;
}

void WEdit::GetSelection( INT& Start, INT& End )
{
	guard(WEdit::GetSelection);
	check(hWnd);
	SendMessageX( *this, EM_GETSEL, (WPARAM)&Start, (LPARAM)&End );
	unguard;
}

void WEdit::SetSelection( INT Start, INT End )
{
	guard(WEdit::SetSelection);
	check(hWnd);
	SendMessageX( *this, EM_SETSEL, Start, End );
	unguard;
}

void WEdit::SetSelectedText( const TCHAR* Text )
{
	guard(WEdit::SetSelectedText);
	check(hWnd);
	SendMessageLX( *this, EM_REPLACESEL, 1, Text );
	unguard;
}

UBOOL WEdit::GetModify()
{
	guard(WEdit::GetModify);
	return SendMessageX( *this, EM_GETMODIFY, 0, 0 )!=0;
	unguard;
}

void WEdit::SetModify( UBOOL Modified )
{
	guard(WEdit::SetModify);
	SendMessageX( *this, EM_SETMODIFY, Modified, 0 );
	unguard;
}

void WEdit::ScrollCaret()
{
	guard(WEdit::ScrollCaret);
	SendMessageX( *this, EM_SCROLLCARET, 0, 0 );
	unguard;
}

/*-----------------------------------------------------------------------------
	WRichEdit.
-----------------------------------------------------------------------------*/

static char *GStreamPos;	// Pointer to the current position we are reading text from
static INT GiStreamSz, GiStreamPos;

static DWORD CALLBACK RichEdit_StreamIn(DWORD dwCookie, LPBYTE pbBuffer, LONG cb, LONG *pcb)
{
	if( GiStreamSz - GiStreamPos < cb )
	{
		::strncpy( (char*)pbBuffer, GStreamPos, GiStreamSz - GiStreamPos );
		*pcb = GiStreamSz - GiStreamPos;
	}
	else
	{
		::strncpy( (char*)pbBuffer, GStreamPos, cb );
		GStreamPos += cb;
		*pcb = cb;
	}

	return 0;
}

static DWORD CALLBACK RichEdit_StreamOut(DWORD dwCookie, LPBYTE pbBuffer, LONG cb, LONG *pcb)
{
	::strncat( GStreamPos, (char*)pbBuffer, cb );
	*pcb = cb;
	
	return 0;
}

void WRichEdit::StreamTextIn( char* _StreamSrc, INT _iSz )
{
	guard(WRichEdit::StreamTextIn);

	if(_iSz > 25000)		GWarn->BeginSlowTask( TEXT("Loading text..."), 1 );
	GStreamPos = _StreamSrc;
	GiStreamSz = _iSz;
	GiStreamPos = 0;

	EDITSTREAM es;
	es.dwCookie = 0;
	es.dwError = 0;
	es.pfnCallback = RichEdit_StreamIn;
	SendMessageX(hWnd, EM_STREAMIN, SF_RTF, (LPARAM) &es);
	if(_iSz > 25000)		GWarn->EndSlowTask();

	unguard;
}

void WRichEdit::StreamTextOut( char* _StreamDst, INT _iSz )
{
	guard(WRichEdit::StreamTextIn);

	if( _iSz > 25000 )	GWarn->BeginSlowTask( TEXT("Saving text..."), 1 );
	GStreamPos = _StreamDst;
	GiStreamSz = _iSz;
	GiStreamPos = 0;

	EDITSTREAM es;
	es.dwCookie = 0;
	es.dwError = 0;
	es.pfnCallback = RichEdit_StreamOut;
	SendMessageX(hWnd, EM_STREAMOUT, SF_TEXT, (LPARAM) &es);
	if( _iSz > 25000 )	GWarn->EndSlowTask();

	unguard;
}

void WRichEdit::OpenWindow( UBOOL Visible, UBOOL ReadOnly )
{
	guard(WRichEdit::OpenWindow);
	PerformCreateWindowEx
	(
		WS_EX_CLIENTEDGE,
		NULL,
		WS_CHILD | (Visible?WS_VISIBLE:0) | ES_MULTILINE | ES_NOHIDESEL | ES_SUNKEN | ES_SAVESEL | WS_HSCROLL | WS_VSCROLL,
		0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

UBOOL WRichEdit::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WRichEdit::InterceptControlCommand);
	if( HIWORD(wParam)==EN_CHANGE )
	{
		ChangeDelegate();
		return 1;
	}
	else return 0;
	unguard;
}

void WRichEdit::SetTextLimit( INT _Limit )
{
	guard(WRichEdit::SetTextLimit);
	SendMessageX( *this, EM_EXLIMITTEXT, _Limit, 0 );
	unguard;
}

void WRichEdit::SetReadOnly( UBOOL ReadOnly )
{
	guard(WRichEdit::SetReadOnly);
	check(hWnd);
	SendMessageX( *this, EM_SETREADONLY, ReadOnly, 0 );
	unguard;
}

/*-----------------------------------------------------------------------------
	WTrackBar.
-----------------------------------------------------------------------------*/

void WTrackBar::OpenWindow( UBOOL Visible, UBOOL InClientEdge )
{
	guard(WTrackBar::OpenWindow);
	PerformCreateWindowEx
	(
		(InClientEdge?WS_EX_CLIENTEDGE:0),
        NULL,
        WS_CHILD | TBS_HORZ | TBS_BOTTOM | (Visible?WS_VISIBLE:0) | (ManualTicks ? 0 : TBS_AUTOTICKS),
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
	unguard;
}

UBOOL WTrackBar::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WTrackBar::InterceptControlCommand);
	if     ( Message==WM_HSCROLL && LOWORD(wParam)==TB_THUMBTRACK ) {ThumbTrackDelegate();    return 1;}
	else if( Message==WM_HSCROLL                                  ) {ThumbPositionDelegate(); return 1;}
	else return 0;
	unguard;
}

void WTrackBar::SetTicFreq( INT TicFreq )
{
	guard(WTrackBar::SetTicFreq);
	SendMessageX( *this, TBM_SETTICFREQ, TicFreq, 0 );
	unguard;
}

void WTrackBar::SetTicks( INT* Ticks, INT TickCount )
{
	guard(WTrackBar::SetTicks);
	SendMessageX( *this, TBM_CLEARTICS, 1, 0 );
	for(INT i=0;i<TickCount;i++)
		SendMessageX( *this, TBM_SETTIC, 0, (LONG)Ticks[i] );
	unguard;
}

void WTrackBar::SetRange( INT Min, INT Max )
{
	guard(WTrackBar::SetRange);
	SendMessageX( *this, TBM_SETRANGE, 1, MAKELONG(Min,Max) );
	unguard;
}

void WTrackBar::SetPos( INT Pos )
{
	guard(WTrackBar::SetPos);
	SendMessageX( *this, TBM_SETPOS, 1, Pos );
	unguard;
}

INT WTrackBar::GetPos()
{
	guard(WTrackBar::GetPos);
	return SendMessageX( *this, TBM_GETPOS, 0, 0 );
	unguard;
}

/*-----------------------------------------------------------------------------
	WProgressBar.
-----------------------------------------------------------------------------*/

void WProgressBar::OpenWindow( UBOOL Visible )
{
	guard(WProgressBar::OpenWindow);
	PerformCreateWindowEx
	(
		WS_EX_CLIENTEDGE,
        NULL,
        WS_CHILD | (Visible?WS_VISIBLE:0),
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);
	SendMessageX( *this, PBM_SETRANGE, 0, 100 );
	unguard;
}

void WProgressBar::SetProgress( SQWORD InCurrent, SQWORD InMax ) // sjs INT overflowed!
{
	INT InPercent = InCurrent*100/Max<SQWORD>(InMax,1);
	if( InPercent!=Percent )
		SendMessageX( *this, PBM_SETPOS, InPercent, 0 );
	Percent = InPercent;
}

/*-----------------------------------------------------------------------------
	WListBox.
-----------------------------------------------------------------------------*/

void WListBox::OpenWindow( UBOOL Visible, UBOOL Integral, UBOOL MultiSel, UBOOL OwnerDrawVariable, UBOOL Sort, DWORD dwExtraStyle )
{
	guard(WListBox::OpenWindow);
	m_bMultiSel = MultiSel;
	PerformCreateWindowEx
	(
		WS_EX_CLIENTEDGE,
        NULL,
        WS_CHILD | WS_BORDER | WS_VSCROLL | WS_CLIPCHILDREN | LBS_NOTIFY | (Visible?WS_VISIBLE:0) | (Integral?0:LBS_NOINTEGRALHEIGHT) 
		| (MultiSel?(LBS_EXTENDEDSEL|LBS_MULTIPLESEL):0) | (OwnerDrawVariable?LBS_OWNERDRAWVARIABLE:0) | (Sort?LBS_SORT:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

UBOOL WListBox::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WListBox::InterceptControlCommand);
	if     ( HIWORD(wParam)==LBN_DBLCLK   ) {DoubleClickDelegate();     return 1;}
	else if( HIWORD(wParam)==LBN_SELCHANGE) {SelectionChangeDelegate(); return 1;}
	else if( HIWORD(wParam)==LBN_SELCANCEL) {SelectionCancelDelegate(); return 1;}
	else if( HIWORD(wParam)==LBN_SETFOCUS)  {SetFocusDelegate();        return 1;}
	else if( HIWORD(wParam)==LBN_KILLFOCUS) {KillFocusDelegate();       return 1;}
	else return 0;
	unguard;
}

void WListBox::OnRightButtonDown()
{
	FPoint pt = GetCursorPos();
	INT idx = ItemFromPoint( pt );
	SetCurrent( idx );

	RightClickDelegate();
}

FString WListBox::GetString( INT Index )
{
	guard(WListBox::GetText);
	INT Length = SendMessageX(*this,LB_GETTEXTLEN,Index,0);
	if( Length == LB_ERR )
		return TEXT("");
#if UNICODE
	if( GUnicode && !GUnicodeOS )
	{
		ANSICHAR* ACh = (ANSICHAR*)appAlloca((Length+1)*sizeof(ANSICHAR));
		SendMessageA( *this, LB_GETTEXT, Index, (LPARAM)ACh );
		ACh[Length] = 0;
		return appFromAnsi(ACh);
	}
	else
#endif
	{
		TCHAR* Ch = (TCHAR*)appAlloca((Length+1)*sizeof(TCHAR));
		SendMessage( *this, LB_GETTEXT, Index, (LPARAM)Ch );
		Ch[Length] = 0;
		return Ch;
	}
	unguard;
}

void* WListBox::GetItemData( INT Index )
{
	guard(WListBox::GetItemData);
	return (void*)SendMessageX( *this, LB_GETITEMDATA, Index, 0 );
	unguard;
}

void WListBox::SetItemData( INT Index, void* Value )
{
	guard(WListBox::SetItemData);
	SendMessageX( *this, LB_SETITEMDATA, Index, (LPARAM)Value );
	unguard;
}

void WListBox::SetItemData( INT Index, INT Value )
{
	guard(WListBox::SetItemData);
	SendMessageX( *this, LB_SETITEMDATA, Index, (LPARAM)Value );
	unguard;
}

INT WListBox::GetCurrent()
{
	guard(WListBox::GetCurrent);
	return SendMessageX( *this, LB_GETCARETINDEX, 0, 0 );
	unguard;
}

void WListBox::ClearSel()
{
	guard(WListBox::ClearSel);
	SendMessageX( *this, LB_SETSEL, FALSE, -1 );
	unguard;
}

INT WListBox::SetCurrent( INT Index, UBOOL bScrollIntoView )
{
	guard(WListBox::SetCurrent);
	INT ret;
	if( m_bMultiSel )
	{
		ClearSel();
		ret = SendMessageX( *this, LB_SETSEL, TRUE, Index );
		SendMessageX( *this, LB_SETCARETINDEX, Index, bScrollIntoView );
	}
	else
	{
		ret = SendMessageX( *this, LB_SETCURSEL, Index, 0 );
		SendMessageX( *this, LB_SETCARETINDEX, Index, bScrollIntoView );
	}
	return ret;
	unguard;
}

INT WListBox::GetTop()
{
	guard(WListBox::GetTop);
	return SendMessageX( *this, LB_GETTOPINDEX, 0, 0 );
	unguard;
}

void WListBox::SetTop( INT Index )
{
	guard(WListBox::SetTop);
	SendMessageX( *this, LB_SETTOPINDEX, Index, 0 );
	unguard;
}

void WListBox::DeleteString( INT Index )
{
	guard(WListBox::DeleteItem);
	SendMessageX( *this, LB_DELETESTRING, Index, 0 );
	unguard;
}

INT WListBox::GetCount()
{
	guard(WListBox::GetCount);
	return SendMessageX( *this, LB_GETCOUNT, 0, 0 );
	unguard;
}

INT WListBox::GetItemHeight( INT Index )
{
	guard(WListBox::GetItemHeight);
	return SendMessageX( *this, LB_GETITEMHEIGHT, Index, 0 );
	unguard;
}

INT WListBox::ItemFromPoint( FPoint P )
{
	guard(WListBox::ItemFromPoint);
	DWORD Result=SendMessageX( *this, LB_ITEMFROMPOINT, 0, MAKELPARAM(P.X,P.Y) );
	return HIWORD(Result) ? -1 : LOWORD(Result);
	unguard;
}

FRect WListBox::GetItemRect( INT Index )
{
	guard(WListBox::GetItemRect);
	RECT R; R.left=R.right=R.top=R.bottom=0;
	SendMessageX( *this, LB_GETITEMRECT, Index, (LPARAM)&R );
	return R;
	unguard;
}

void WListBox::Empty()
{
	guard(WListBox::Empty);
	SendMessageX( *this, LB_RESETCONTENT, 0, 0 );
	unguard;
}

UBOOL WListBox::GetSelected( INT Index )
{
	guard(WListBox::GetSelected);
	return SendMessageX( *this, LB_GETSEL, Index, 0 );
	unguard;
}

INT WListBox::GetSelectedItems( INT Count, INT* Buffer )
{
	guard(WListBox::GetSelectedItems);
	return SendMessageX( *this, LB_GETSELITEMS, (WPARAM)Count, (LPARAM)Buffer );
	unguard;
}

INT WListBox::GetSelectedCount()
{
	guard(WListBox::GetSelectedCount);
	return SendMessageX( *this, LB_GETSELCOUNT, 0, 0 );
	unguard;
}

INT WListBox::AddString( const TCHAR* C )
{
	guard(WListBox::AddString);
	return SendMessageLX( *this, LB_ADDSTRING, 0, C );
	unguard;
}

void WListBox::InsertString( INT Index, const TCHAR* C )
{
	guard(WListBox::InsertString);
	SendMessageLX( *this, LB_INSERTSTRING, Index, C );
	unguard;
}

INT WListBox::FindStringExact( const TCHAR* C )
{
	guard(WListBox::FindStringExact);
	return SendMessageLX( *this, LB_FINDSTRINGEXACT, -1, C );
	unguard;
}

INT WListBox::FindString( const TCHAR* C )
{
	guard(WListBox::FindString);
	return SendMessageLX( *this, LB_FINDSTRING, -1, C );
	unguard;
}

INT WListBox::FindStringChecked( const TCHAR* C )
{
	guard(WListBox::FindStringChecked);
	INT Result = SendMessageLX( *this, LB_FINDSTRING, -1, C );
	check(Result!=LB_ERR);
	return Result;
	unguard;
}

void WListBox::InsertStringAfter( const TCHAR* Existing, const TCHAR* New )
{
	guard(WListBox::InsertStringAfter);
	InsertString( FindStringChecked(Existing)+1, New );
	unguard;
}

INT WListBox::AddItem( const void* C )
{
	guard(WListBox::AddItem);
	return SendMessageX( *this, LB_ADDSTRING, 0, (LPARAM)C );
	unguard;
}

void WListBox::InsertItem( INT Index, const void* C )
{
	guard(WListBox::AddInsert);
	SendMessageX( *this, LB_INSERTSTRING, Index, (LPARAM)C );
	unguard;
}

INT WListBox::FindItem( const void* C )
{
	guard(WListBox::FindItem);
	return SendMessageX( *this, LB_FINDSTRING, -1, (LPARAM)C );
	unguard;
}

INT WListBox::FindItemChecked( const void* C )
{
	guard(WListBox::FindItemChecked);
	INT Result = SendMessageX( *this, LB_FINDSTRING, -1, (LPARAM)C );
	check(Result!=LB_ERR);
	return Result;
	unguard;
}

void WListBox::InsertItemAfter( const void* Existing, const void* New )
{
	guard(WListBox::InsertStringAfter);
	InsertItem( FindItemChecked(Existing)+1, New );
	unguard;
}

/*-----------------------------------------------------------------------------
	WCheckListBox.
-----------------------------------------------------------------------------*/

void WCheckListBox::OpenWindow( UBOOL Visible, UBOOL Integral, UBOOL MultiSel, UBOOL Sort, DWORD dwExtraStyle )
{
	guard(WCheckListBox::OpenWindow);
	m_bMultiSel = MultiSel;
	PerformCreateWindowEx
	(
		WS_EX_CLIENTEDGE,
        NULL,
        WS_CHILD | WS_BORDER | WS_VSCROLL | WS_CLIPCHILDREN | LBS_NOTIFY | (Visible?WS_VISIBLE:0) | (Integral?0:LBS_NOINTEGRALHEIGHT) 
			| (MultiSel?(LBS_EXTENDEDSEL|LBS_MULTIPLESEL):0) | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | (Sort?LBS_SORT:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

void WCheckListBox::OnDestroy()
{
	guard(WCheckListBox::OnDestroy);
	WListBox::OnDestroy();
	DeleteObject(hbmOff);
	DeleteObject(hbmOn);
	unguard;
}

void WCheckListBox::OnDrawItem( DRAWITEMSTRUCT* Item )
{
	guard(WCheckListBox::OnDrawItem);

	if(((LONG)(Item->itemID) >= 0)
		&& (Item->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		UBOOL bDisabled = !IsWindowEnabled(hWnd);

		COLORREF newTextColor = bDisabled ? RGB(128, 128, 128) : GetSysColor(COLOR_WINDOWTEXT);
		COLORREF oldTextColor = SetTextColor(Item->hDC, newTextColor);

		COLORREF newBkColor = GetSysColor(COLOR_WINDOW);
		COLORREF oldBkColor = SetBkColor(Item->hDC, newBkColor);

		if (newTextColor == newBkColor)
			newTextColor = RGB(192, 192, 192);

		if (!bDisabled && ((Item->itemState & ODS_SELECTED) != 0))
		{
			SetTextColor(Item->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor(Item->hDC, GetSysColor(COLOR_HIGHLIGHT));
		}

		FString strText = GetString(Item->itemID);
		ExtTextOutA(Item->hDC, Item->rcItem.left + 18,
			Item->rcItem.top + 2,
			ETO_OPAQUE, &(Item->rcItem), appToAnsi( *strText ), strText.Len(), NULL);

		SetTextColor(Item->hDC, oldTextColor);
		SetBkColor(Item->hDC, oldBkColor);

		// BITMAP
		//
		HDC hdcMem = CreateCompatibleDC(Item->hDC);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, ( (INT)GetItemData( Item->itemID ) ? hbmOn : hbmOff));

		BitBlt(Item->hDC,
			Item->rcItem.left + 2, Item->rcItem.top + 3,
			13, 13,
			hdcMem,
			0, 0,
			SRCCOPY);

		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);
	}

	if((Item->itemAction & ODA_FOCUS) != 0)
		DrawFocusRect(Item->hDC, &(Item->rcItem));
	unguard;
}

void WCheckListBox::OnLeftButtonDown()
{
	guard(WCheckListBox::OnLeftButtonDown);
	WListBox::OnLeftButtonDown();

	FPoint pt;
	::GetCursorPos((POINT*)pt);
	::ScreenToClient( hWnd, (POINT*)pt );

	if( pt.X <= 16 )
	{
		INT Item = ItemFromPoint( pt );
		INT Data = (INT)GetItemData( Item );
		Data = Data ? 0 : 1;
		SetItemData( Item, Data );
	}

	check(OwnerWindow);
	PostMessageX( OwnerWindow->hWnd, WM_COMMAND, WM_WCLB_UPDATE_VISIBILITY, 0L );
	InvalidateRect( hWnd, NULL, FALSE );
	unguard;
}

/*-----------------------------------------------------------------------------
	WItemBox
-----------------------------------------------------------------------------*/

void WItemBox::OnDrawItem( DRAWITEMSTRUCT* Info )
{
	if( Info->itemData )
	{
		((FTreeItemBase*)Info->itemData)->SetSelected( (Info->itemState & ODS_SELECTED)!=0 );
		((FTreeItemBase*)Info->itemData)->Draw( Info->hDC );
	}
}

void WItemBox::OnMeasureItem( MEASUREITEMSTRUCT* Info )
{
	if( Info->itemData )
		Info->itemHeight = ((FTreeItemBase*)Info->itemData)->GetHeight();
}

UBOOL WItemBox::OnEraseBkgnd()
{
	return 1;
}

/*-----------------------------------------------------------------------------
	WListView
-----------------------------------------------------------------------------*/

void WListView::OpenWindow( UBOOL Visible, DWORD dwExtraStyle )
{
	guard(WListView::OpenWindow);
	PerformCreateWindowEx
	(
		WS_EX_CLIENTEDGE,
        NULL,
        WS_CHILD | (Visible?WS_VISIBLE:0) | dwExtraStyle,
        0, 0,
		0, 0,
        *OwnerWindow,
        (HMENU)ControlId,
        hInstance
	);

	SendMessageX( *this, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(0,0) );
	unguard;
}

void WListView::Empty()
{
	guard(WListView::Empty);
	SendMessageX(hWnd, LVM_DELETEALLITEMS, 0, 0L);
	unguard;
}

UBOOL WListView::InterceptControlCommand( UINT Message, UINT wParam, LONG lParam )
{
	guard(WListView::InterceptControlCommand);

	LastwParam = wParam;
	LastlParam = lParam;

	NMTREEVIEW* pntv = (LPNMTREEVIEW)lParam;

	if( pntv->hdr.code==NM_DBLCLK )
	{
		DblClkDelegate();
		return 1;
	}
	if( pntv->hdr.code==LVN_ITEMCHANGED )
		SelChangedDelegate();

	return 0;
	unguard;
}

INT WListView::GetCurrent()
{
	guard(WListView::GetCurrent);
	return SendMessageX( hWnd, LVM_GETNEXTITEM, -1, LVNI_ALL | LVNI_SELECTED | LVNI_FOCUSED );
	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
