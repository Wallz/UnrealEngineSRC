/*=============================================================================
	Launch.cpp: Game launcher.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Brandon Reinhart.
=============================================================================*/

#include "XLaunchPrivate.h"

/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

extern "C" {TCHAR GPackage[64]=TEXT("XLaunch");}

// Memory allocator.
#include "FMallocAnsi.h"
FMallocAnsi Malloc;

// Log file.
#include "FOutputDeviceFile.h"
FOutputDeviceFile Log;

// Error handler.
#include "FOutputDeviceAnsiError.h"
FOutputDeviceAnsiError Error;

// Feedback.
#include "FFeedbackContextAnsi.h"
FFeedbackContextAnsi Warn;

// File manager.
#include "FFileManagerLinux.h"
FFileManagerLinux FileManager;

// Config.
#include "FConfigCacheIni.h"

// X Windows
#include <X11/Xlib.h>
#include <X11/xpm.h>
Display* GDisplay;
Window SplashScreen;
Pixmap SplashPixmap;

/*-----------------------------------------------------------------------------
	Initialization
-----------------------------------------------------------------------------*/

//
// Connects to the X server.
//
static INT InitWindowing()
{
	// Get the display.
	GDisplay = XOpenDisplay(0);
	if (!GDisplay)
		appErrorf( TEXT("Can't open display. Unreal requires X windows.") );
}

//
// Clear out all the splash window messages.
// 
static void EmptySplashQueue()
{
	XEvent SplashEvent;
	while( XCheckWindowEvent(GDisplay, SplashScreen, 
		ExposureMask | StructureNotifyMask | VisibilityChangeMask, 
		&SplashEvent) )
	{
		// Do nothing.
	}
}

//
// Create a splash screen.
//
static void InitSplash()
{
	// Load the splash pixmap.
	XpmReadFileToPixmap( GDisplay, DefaultRootWindow(GDisplay), 
		"../Help/UT-Linux.Xpm", &SplashPixmap, NULL, NULL);

	// Find position to put the window.
	Window RootRoot;
	int r_x, r_y;
	unsigned int r_width, r_height, r_border, r_depth;
	XGetGeometry(
		GDisplay, DefaultRootWindow(GDisplay), &RootRoot,
		&r_x, &r_y, &r_width, &r_height, &r_border, &r_depth
	);
	
	// Create the splash window.
	XSetWindowAttributes swa;
	swa.background_pixmap = SplashPixmap;
	swa.colormap = DefaultColormap(GDisplay, DefaultScreen(GDisplay));
	swa.border_pixel = 0;
	swa.override_redirect = True;
	swa.event_mask = ExposureMask | StructureNotifyMask | VisibilityChangeMask;
	SplashScreen = XCreateWindow(
		GDisplay,
		DefaultRootWindow(GDisplay),
		(r_width - 510)/2, (r_height - 385)/2,
		510, 385,
		0, DefaultDepth(GDisplay, DefaultScreen(GDisplay)),
		InputOutput, DefaultVisual(GDisplay, DefaultScreen(GDisplay)),
		CWBackPixmap | CWBorderPixel | CWColormap | 
		CWEventMask | CWOverrideRedirect, &swa
	);

	// Show the window.
	XMapRaised(GDisplay, SplashScreen);

	// Empty the window message queue.
	EmptySplashQueue();
}

//
// Destroy the splash screen.
//
static void ExitSplash()
{
	// Destroy the splash screen.
	XDestroyWindow(GDisplay, SplashScreen);

	// Empty the window message queue.
	EmptySplashQueue();

	// Free the pixmap.
	XFreePixmap(GDisplay, SplashPixmap);
}

//
// Creates a UEngine object.
//
static UEngine* InitEngine()
{
	guard(InitEngine);
	DOUBLE LoadTime = appSeconds();

	// Set exec hook.
	GExec = NULL;

	// Update first-run.
	INT FirstRun=0;
	if (FirstRun<ENGINE_VERSION)
		FirstRun = ENGINE_VERSION;
	GConfig->SetInt( TEXT("FirstRun"), TEXT("FirstRun"), FirstRun );

	// Create the global engine object.
	UClass* EngineClass;
	EngineClass = UObject::StaticLoadClass(
		UGameEngine::StaticClass(), NULL, 
		TEXT("ini:Engine.Engine.GameEngine"), 
		NULL, LOAD_NoFail, NULL 
	);
	UEngine* Engine = ConstructObject<UEngine>( EngineClass );
	Engine->Init();
	debugf( TEXT("Startup time: %f seconds."), appSeconds()-LoadTime );

	return Engine;
	unguard;
}

/*-----------------------------------------------------------------------------
	Main Loop
-----------------------------------------------------------------------------*/

//
// X game message loop.
//
static void MainLoop( UEngine* Engine )
{
	guard(MainLoop);
	check(Engine);

	// Loop while running.
	GIsRunning = 1;
	DOUBLE OldTime = appSeconds();
	DOUBLE SecondStartTime = OldTime;
	INT TickCount = 0;
	while( GIsRunning && !GIsRequestingExit )
	{
		// Update the world.
		guard(UpdateWorld);
		DOUBLE NewTime   = appSeconds();
		FLOAT  DeltaTime = NewTime - OldTime;
		Engine->Tick( DeltaTime );
		if( GWindowManager )
			GWindowManager->Tick( DeltaTime );
		OldTime = NewTime;
		TickCount++;
		if( OldTime > SecondStartTime + 1 )
		{
			Engine->CurrentTickRate = (FLOAT)TickCount / (OldTime - SecondStartTime);
			SecondStartTime = OldTime;
			TickCount = 0;
		}
		unguard;

		// Enforce optional maximum tick rate.
		guard(EnforceTickRate);
		FLOAT MaxTickRate = Engine->GetMaxTickRate();
		if( MaxTickRate>0.0 )
		{
			FLOAT Delta = (1.0/MaxTickRate) - (appSeconds()-OldTime);
			appSleep( Max(0.f,Delta) );
		}
		unguard;
	}
	GIsRunning = 0;

	unguard;
}

/*-----------------------------------------------------------------------------
	Main.
-----------------------------------------------------------------------------*/

//
// Simple copy.
// 

void SimpleCopy(TCHAR* fromfile, TCHAR* tofile)
{
	INT c;
	FILE* from;
	FILE* to;
	from = fopen(fromfile, "r");
	if (from == NULL)
		return;
	to = fopen(tofile, "w");
	if (to == NULL)
	{
		printf("Can't open or create %s", tofile);
		return;
	}
	while ((c = getc(from)) != EOF)
		putc(c, to);
	fclose(from);
	fclose(to);
}

//
// Exit wound.
//
int CleanUpOnExit(int ErrorLevel)
{
	// Clean up our mess.
	if( GDisplay )
		XCloseDisplay(GDisplay);
	GFileManager->Delete(TEXT("Running.ini"),0,0);
	debugf( NAME_Title, LocalizeGeneral("Exit") );
	appPreExit();
	GIsGuarded = 0;

	// Shutdown.
	appExit();
	GIsStarted = 0;

	// Restore the user's configuration.
	TCHAR baseconfig[PATH_MAX] = TEXT("");
	getcwd(baseconfig, sizeof(baseconfig));
	appStrcat(baseconfig, "/User.ini");

	TCHAR userconfig[PATH_MAX] = TEXT("");
	sprintf(userconfig, "~/.utconf");

	TCHAR exec[PATH_MAX] = TEXT("");
	sprintf(exec, "cp -f %s %s", baseconfig, userconfig);
	//system( exec );

	return ErrorLevel;
}

//
// Entry point.
//
int main( int argc, char* argv[] )
{
	__Context::StaticInit();

	try
	{
	guard(main);
	
	INT ErrorLevel = 0;
	GIsStarted	   = 1;

	// Set module name.
	appStrcpy( GModule, argv[0] );

	// Set the package name.
	appStrcpy( GPackage, appPackage() );	

	// Get the command line.
	TCHAR CmdLine[1024], *CmdLinePtr=CmdLine;
	*CmdLinePtr = 0;
	for( INT i=1; i<argc; i++ )
	{
		if( i>1 )
			appStrcat( CmdLine, " " );
		appStrcat( CmdLine, argv[i] );
	}

	// Take care of .ini swapping.
	TCHAR userconfig[PATH_MAX] = TEXT("");
	sprintf(userconfig, "~/.utconf");

	TCHAR baseconfig[PATH_MAX] = TEXT("");
	getcwd(baseconfig, sizeof(baseconfig));
	appStrcat(baseconfig, "/User.ini");

	TCHAR exec[PATH_MAX] = TEXT("");
	sprintf(exec, "cp -f %s %s", userconfig, baseconfig);
	//system( exec );

	//SimpleCopy( userconfig, baseconfig );

	// Init core.
	GIsClient = 1; 
	GIsGuarded = 1;
	appInit( TEXT("UnrealTournament"), CmdLine, &Malloc, &Log, &Error, &Warn, &FileManager, FConfigCacheIni::Factory, 1 );

	// Init mode.
	GIsServer		= 1;
	GIsClient		= !ParseParam(appCmdLine(), TEXT("SERVER"));
	GIsEditor		= 0;
	GIsScriptable	= 1;
	GLazyLoad		= !GIsClient || ParseParam(appCmdLine(), TEXT("LAZY"));

	// Init windowing.
	InitWindowing();

	// Init splash screen.
	InitSplash();
	
	// Init console log.
	if (ParseParam(CmdLine, TEXT("LOG")))
	{
		Warn.AuxOut = GLog;
		GLog		= &Warn;
	}

	// Init engine.
	UEngine* Engine = InitEngine();
	if( Engine )
	{
		debugf( NAME_Title, LocalizeGeneral("Run") );

		// Remove splash screen.
		ExitSplash();

		// Optionally Exec and exec file.
		FString Temp;
		if( Parse(CmdLine, TEXT("EXEC="), Temp) )
		{
			Temp = FString(TEXT("exec ")) + Temp;
			if( Engine->Client && Engine->Client->Viewports.Num() && Engine->Client->Viewports(0) )
				Engine->Client->Viewports(0)->Exec( *Temp, *GLog );
		}

		// Start main engine loop.
		debugf( TEXT("Entering main loop.") );
		if ( !GIsRequestingExit )
			MainLoop( Engine );
	}

	// Finish up.
	return CleanUpOnExit(ErrorLevel);

	unguard;
	}
	catch (...)
	{
		// Chained abort.  Do cleanup.
		printf("%s\n", GErrorHist);
		return CleanUpOnExit(1);
	}
}