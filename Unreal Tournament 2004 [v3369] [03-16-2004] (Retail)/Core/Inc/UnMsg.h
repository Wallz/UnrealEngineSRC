/*=============================================================================
	UnMsg.h: Just a place to dump all user defined messages
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#define WM_APP_EXEC								(WM_APP +  1)
#define WM_APP_SETPROP							(WM_APP +  2)
#define WM_APP_GETPROP							(WM_APP +  3)
#define WM_VIEWPORT_UPDATEWINDOWFRAME			(WM_APP +  4)
#define WM_EDC_BROWSE							(WM_APP +  5)
#define WM_EDC_USECURRENT						(WM_APP +  6)
#define WM_EDC_CURTEXCHANGE						(WM_APP +  7)
#define WM_EDC_SELPOLYCHANGE					(WM_APP +  8)
#define WM_EDC_SELCHANGE						(WM_APP +  9)
#define WM_EDC_RTCLICKTEXTURE					(WM_APP + 10)
#define WM_EDC_RTCLICKPOLY						(WM_APP + 11)
#define WM_EDC_RTCLICKACTOR						(WM_APP + 12)
#define WM_EDC_RTCLICKWINDOW					(WM_APP + 13)
#define WM_EDC_RTCLICKWINDOWCANADD				(WM_APP + 14)
#define WM_EDC_MAPCHANGE						(WM_APP + 15)
#define WM_EDC_VIEWPORTUPDATEWINDOWFRAME		(WM_APP + 16)
#define WM_EDC_SURFPROPS						(WM_APP + 17)
#define WM_EDC_SAVEMAP							(WM_APP + 18)
#define WM_EDC_LOADMAP							(WM_APP + 19)
#define WM_EDC_PLAYMAP							(WM_APP + 20)
#define WM_MC_SHOW								(WM_APP + 21)
#define WM_MC_SHOW_COND							(WM_APP + 22)
#define WM_MC_CLEAR								(WM_APP + 23)
#define WM_MC_ADD								(WM_APP + 24)
#define WM_EDC_CAMMODECHANGE					(WM_APP + 25)
#define WM_EDC_RTCLICKACTORSTATICMESH			(WM_APP + 26)
#define WM_BB_RCLICK							(WM_APP + 27)
#define WM_REDRAWALLVIEWPORTS					(WM_APP + 28)
#define WM_PB_PUSH								(WM_APP + 29)
#define WM_TREEVIEW_RIGHT_CLICK					(WM_APP + 30)
#define WM_EXPANDBUTTON_CLICKED					(WM_APP + 31)
#define WM_ADD_MRU								(WM_APP + 32)
#define WM_WCLB_UPDATE_VISIBILITY				(WM_APP + 33)
#define WM_BROWSER_DOCK							(WM_APP + 34)
#define WM_BROWSER_UNDOCK						(WM_APP + 35)
#define WM_EDC_SAVEMAPAS						(WM_APP + 36)
#define WM_EDC_VIEWPORTSDISABLEREALTIME			(WM_APP + 37)
#define WM_EDC_ACTORPROPERTIESCHANGE			(WM_APP + 38)
#define WM_EDC_RTCLICKTERRAINLAYER				(WM_APP + 39)
#define WM_EDC_DISPLAYLOADERRORS				(WM_APP + 40)
#define WM_EDC_REFRESHEDITOR					(WM_APP + 41)
#define WM_REFRESH								(WM_APP + 42)
#define WM_EDC_CURSTATICMESHCHANGE				(WM_APP + 43)
#define WM_EDC_RTCLICKSTATICMESH				(WM_APP + 44)
#define WM_EDC_RTCLICKMATINEETIMESLIDER			(WM_APP + 45)
#define WM_EDC_RTCLICKMATSCENE					(WM_APP + 46)
#define WM_MAT_PREVIEW_CLOSING					(WM_APP + 47)
#define WM_EDC_RTCLICKANIMSEQ					(WM_APP + 48)
#define WM_DLGTEXPROP_CLOSING					(WM_APP + 49)
#define WM_REDRAWCURRENTVIEWPORT				(WM_APP + 50)
#define WM_EDC_MATERIALTREECLICK				(WM_APP + 51)
#define WM_UDN_GETHELPTOPIC						(WM_APP + 52)
#define WM_POSITIONCHILDCONTROLS				(WM_APP + 53)
#define WM_MC_HIDE								(WM_APP + 54)
#define WM_EDC_RTCLICKMATACTION					(WM_APP + 55)

#ifdef WITH_LIPSINC
#define WM_EDC_LIPSINCANIMLISTRTCLICK			(WM_APP + 56)
#define WM_EDC_LIPSINC_CANPASTE					(WM_APP + 57)
#define WM_EDC_LIPSINC_NOPASTE					(WM_APP + 58)
#endif

// gam ---
#define WM_APPLY_DRAW_SCALE    				    (WM_APP + 200)
// --- gam

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

