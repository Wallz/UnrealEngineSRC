/*=============================================================================
	UnGame.cpp: Unreal game engine.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

#include "EnginePrivate.h"
#include "UnNet.h"
#include "UnLinker.h"

#include "xForceFeedback.h" // jdf

/*-----------------------------------------------------------------------------
	Object class implementation.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(UGameEngine);
IMPLEMENT_CLASS(APrecacheHack);

const int XBoxRelaunch = 0; // sjs --- enables rebooting of xbox between level loads
extern int QueueScreenShot; // sjs - label hack
/*-----------------------------------------------------------------------------
	cleanup!!
-----------------------------------------------------------------------------*/

// gam ---
void UGameEngine::PaintProgress( AVignette* Vignette, FLOAT Progress )
{
	guard(PaintProgress);

    if( !Client || Client->Viewports.Num()==0 )
        return;

	UViewport* Viewport=Client->Viewports(0);

    if( !Viewport || !Viewport->Canvas || !Viewport->RI )
        return;

    if( !Viewport->Lock() ) // sjs - no render device available (minimized)
        return;

    Viewport->Canvas->Update();
	Viewport->RI->Clear(0,FColor(0,0,0),1,1.0f);

	if (Vignette)
		Vignette->eventDrawVignette( Client->Viewports(0)->Canvas, Progress );
	else
		Draw(Viewport);

    if ( Viewport->Canvas->pCanvasUtil )
        Viewport->Canvas->pCanvasUtil->Flush();

    Viewport->Unlock();
	Viewport->Present();

	unguard;
}
// --- gam

INT UGameEngine::ChallengeResponse( INT Challenge )
{
	guard(UGameEngine::ChallengeResponse);
	return (Challenge*237) ^ (0x93fe92Ce) ^ (Challenge>>16) ^ (Challenge<<16);
	unguard;
}

void UGameEngine::UpdateConnectingMessage()
{
	guard(UGameEngine::UpdateConnectingMessage);
	if( GPendingLevel && Client && Client->Viewports.Num() )
	{
		if( Client->Viewports(0)->Actor->ProgressTimeOut<Client->Viewports(0)->Actor->Level->TimeSeconds )
		{
			TCHAR Msg1[256], Msg2[256];
			if( GPendingLevel->DemoRecDriver )
			{
				appSprintf( Msg1, TEXT("") );
				appSprintf( Msg2, *GPendingLevel->URL.Map );
			}
			else
			{
				appSprintf( Msg1, LocalizeProgress(TEXT("ConnectingText"),TEXT("Engine")) );
				appSprintf( Msg2, LocalizeProgress(TEXT("ConnectingURL"),TEXT("Engine")), *GPendingLevel->URL.Host, *GPendingLevel->URL.Map );
			}

			if (!Client->Viewports(0)->GUIController->bActive)
				SetProgress( TEXT(""), Msg1, Msg2, 60.f );
		}
	}
	unguard;
}
void UGameEngine::BuildServerMasterMap( UNetDriver* NetDriver, ULevel* InLevel )
{
	guard(UGameEngine::BuildServerMasterMap);
	check(NetDriver);
	check(InLevel);
	BeginLoad();
	{
		// Init LinkerMap.
		check(InLevel->GetLinker());
		NetDriver->MasterMap->AddLinker( InLevel->GetLinker() );

		// Load server-required packages.
		for( INT i=0; i<ServerPackages.Num(); i++ )
		{
			debugf( TEXT("Server Package: %s"), *ServerPackages(i) );
			ULinkerLoad* Linker = GetPackageLinker( NULL, *ServerPackages(i), LOAD_NoFail, NULL, NULL );
			if( NetDriver->MasterMap->AddLinker( Linker )==INDEX_NONE )
				debugf( TEXT("   (server-side only)") );
		}

		// Add GameInfo's package to map.
		check(InLevel->GetLevelInfo());
		check(InLevel->GetLevelInfo()->Game);
		check(InLevel->GetLevelInfo()->Game->GetClass()->GetLinker());
		NetDriver->MasterMap->AddLinker( InLevel->GetLevelInfo()->Game->GetClass()->GetLinker() );

		// Precompute linker info.
		NetDriver->MasterMap->Compute();
	}
	EndLoad();
	unguard;
}

/*-----------------------------------------------------------------------------
	Game init and exit.
-----------------------------------------------------------------------------*/

//
// Construct the game engine.
//
UGameEngine::UGameEngine()
: LastURL(TEXT(""))
{}

//
// Initialize the game engine.
//
void UGameEngine::Init()
{
	guard(UGameEngine::Init);
	check(sizeof(*this)==GetClass()->GetPropertiesSize());

	// Call base.
	UEngine::Init();

	// Init variables.
	GLevel = NULL;

	// Delete temporary files in cache.
	appCleanFileCache();

	// If not a dedicated server.
	if( GIsClient )
	{	
		// Init client.
		UClass* ClientClass = StaticLoadClass( UClient::StaticClass(), NULL, TEXT("ini:Engine.Engine.ViewportManager"), NULL, LOAD_NoFail, NULL );
		Client = ConstructObject<UClient>( ClientClass );
		Client->Init( this );

		// Init Render Device
		UClass* RenDevClass = StaticLoadClass( URenderDevice::StaticClass(), NULL, TEXT("ini:Engine.Engine.RenderDevice"), NULL, LOAD_NoFail, NULL );
		GRenDev = ConstructObject<URenderDevice>( RenDevClass );
		GRenDev->Init();

        InitForceFeedback();
	}
    
#ifdef WITH_KARMA
    KInitGameKarma(); // Init Karma physics.
#endif

	// Load the entry level.
	FString Error;
	
	// Add code to load Packages.MD5
	
	UObject* MD5Package;

	MD5Package = LoadPackage( NULL, TEXT("Packages.md5"), 0 );

	if (!MD5Package)
        appErrorf(LocalizeError(TEXT("FailedMD5Load"),TEXT("Engine")), TEXT("Packages.md5"));
    else
    {
	    // Build the PackageValidation Array for quick lookup
	    for( FObjectIterator It; It; ++It )
	    {
		    UPackageCheckInfo *Info = (UPackageCheckInfo *) *It;
		    if(Info && Info->IsIn( MD5Package ) )
			    PackageValidation.AddItem(Info);
	    }
    }

	if( Client )
	{
		if( !LoadMap( FURL(TEXT("Entry")), NULL, NULL, Error ) )
			appErrorf( LocalizeError(TEXT("FailedBrowse"),TEXT("Engine")), TEXT("Entry"), *Error );
		Exchange( GLevel, GEntry );
	}

	// Create default URL.
	FURL DefaultURL;
	DefaultURL.LoadURLConfig( TEXT("DefaultPlayer"), TEXT("User") );

	// Enter initial world.
	TCHAR Parm[4096]=TEXT("");
	const TCHAR* Tmp = appCmdLine();
	UBOOL SkippedEntry = 1; // gam
	if
	(	!ParseToken( Tmp, Parm, ARRAY_COUNT(Parm), 0 )
	||	(appStricmp(Parm,TEXT("SERVER"))==0 && !ParseToken( Tmp, Parm, ARRAY_COUNT(Parm), 0 ))
	||	Parm[0]=='-' )
	{
		appStrcpy( Parm, *FURL::DefaultLocalMap );
		SkippedEntry = 0;
    }
	FURL URL( &DefaultURL, Parm, TRAVEL_Partial );
	if( !URL.Valid )
		appErrorf( LocalizeError(TEXT("InvalidUrl"),TEXT("Engine")), Parm );

	UBOOL Success = Browse( URL, NULL, Error );

	// If waiting for a network connection, go into the starting level.
	if( !Success && Error==TEXT("") && appStricmp( Parm, *FURL::DefaultNetBrowseMap )!=0 )
		Success = Browse( FURL(&DefaultURL,*FURL::DefaultNetBrowseMap,TRAVEL_Partial), NULL, Error );

	// Handle failure.
	if( !Success )
		appErrorf( LocalizeError(TEXT("FailedBrowse"),TEXT("Engine")), Parm, *Error );

	// Open initial Viewport.
	if( Client )
	{
		// Init input.!!Temporary
		UInput::StaticInitInput();

        // Create the InteractionMaster

		UClass* IMClass = StaticLoadClass(UInteractionMaster::StaticClass(), NULL, TEXT("engine.InteractionMaster"), NULL, LOAD_NoFail, NULL);
		Client->InteractionMaster = ConstructObject<UInteractionMaster>(IMClass);

		// Setup callback to the client

		Client->InteractionMaster->Client = Client;

		// Display Copyright Notice

		Client->InteractionMaster->DisplayCopyright();

		// Create viewport.
		UViewport* Viewport = Client->NewViewport( NAME_None );

		// Add code to Create the Menu System here				

		UInteraction *GUIController = Client->InteractionMaster->eventAddInteraction(TEXT("ini:Engine.Engine.GUIController"),NULL);

		if (GUIController)
		{

			if ( Cast<UBaseGUIController>(GUIController) != NULL )		
			{
				Cast<UBaseGUIController>(GUIController)->eventInitializeController();	// Initialize it
				Viewport->GUIController = Cast<UBaseGUIController>(GUIController);
			}
		}
		else
			debugf(TEXT("Could not spawn a GUI Controller!"));

		// Create the Console

		UInteraction *Console = Client->InteractionMaster->eventAddInteraction(TEXT("ini:Engine.Engine.Console"),NULL);

		if (Console)
			Client->InteractionMaster->Console = Console;

        Console->ViewportOwner = Viewport;	
		GUIController->ViewportOwner = Viewport;

		Viewport->Console = Console;

		// Initialize Audio.
		InitAudio();
		if( Audio )
			Audio->SetViewport( Viewport );

		// Spawn play actor.
		FString Error;

		APlayerController* PlayerController = GLevel->SpawnPlayActor( Viewport, ROLE_SimulatedProxy, URL, Error );

        if( !PlayerController)
			appErrorf( TEXT("%s"), *Error );

        // jdf ---
        if( GIsClient )
        {
            if( GForceFeedbackAvailable )
                PrecacheForceFeedback();
            else
                ExitForceFeedback();
        }
        // --- jdf

		Viewport->Input->Init( Viewport );
		Viewport->OpenWindow( 0, 0, (INT) INDEX_NONE, (INT) INDEX_NONE, (INT) INDEX_NONE, (INT) INDEX_NONE );
		GLevel->DetailChange(
			Viewport->RenDev->SuperHighDetailActors ? DM_SuperHigh :
			Viewport->RenDev->HighDetailActors ? DM_High :
			DM_Low
			);

	}
	
	// gam ---
	debugf( NAME_Init, TEXT("Game engine initialized") );

	unguard;
}

//
// Pre exit.
//
void UGameEngine::Exit()
{
	guard(UGameEngine::Exit);
	Super::Exit();

	// Exit net.
	if( GLevel->NetDriver )
	{
		delete GLevel->NetDriver;
		GLevel->NetDriver = NULL;
	}

	// Exit RenDev;
	if( GRenDev )
	{
		delete GRenDev;
		GRenDev = NULL;
	}

    ExitForceFeedback();

	unguard;
}

//
// Game exit.
//
void UGameEngine::Destroy()
{
	guard(UGameEngine::Destroy);

	// Game exit.
	if( GPendingLevel )
		CancelPending();
	GLevel = NULL;
	debugf( NAME_Exit, TEXT("Game engine shut down") );

#ifdef WITH_KARMA
    KGData->bShutdownPending = 1;
    KTermGameKarma();
#endif

	Super::Destroy();
	unguard;
}

//
// Progress text.
//

// gam ---
struct SetProgress_Params
{
    FString Str1;
    FString Str2;
};

void UGameEngine::SetProgress( const TCHAR* CmdStr,  const TCHAR* Str1, const TCHAR* Str2, FLOAT Seconds )
{
	guard(UGameEngine::SetProgress);

	if( !Client || !Client->Viewports.Num() )
	    return;
	APlayerController* Actor = Client->Viewports(0)->Actor;

	if( Seconds < 0 )
	    Seconds = ((APlayerController*)((APlayerController::StaticClass())->GetDefaultObject()))->ProgressTimeOut;

	if ( appStrcmp(CmdStr,TEXT("")) )
		Actor->eventProgressCommand(CmdStr,Str1,Str2);
	else
	{
		Actor->eventSetProgressMessage(0, Str1, FColor(255,255,255));
		Actor->eventSetProgressMessage(1, Str2, FColor(255,255,255));
		Actor->eventSetProgressTime(Seconds);
	}
	unguard;
}
// --- gam

/*-----------------------------------------------------------------------------
	Command line executor.
-----------------------------------------------------------------------------*/

//
// This always going to be the last exec handler in the chain. It
// handles passing the command to all other global handlers.
//
UBOOL UGameEngine::Exec( const TCHAR* Cmd, FOutputDevice& Ar )
{
	guard(UGameEngine::Exec);
	const TCHAR* Str=Cmd;

	if( ParseCommand( &Str, TEXT("OPEN") ) )
	{
		FString Error;

		if ( appStrnicmp(Str,TEXT("http://"),7) )
			Client->Viewports(0)->GUIController->eventCloseAll(false);

		if( Client && Client->Viewports.Num() )
			SetClientTravel( Client->Viewports(0), Str, 0, TRAVEL_Partial );
		else
		if( !Browse( FURL(&LastURL,Str,TRAVEL_Partial), NULL, Error ) && Error!=TEXT("") )
			Ar.Logf( TEXT("Open failed: %s"), *Error );
		return 1;
	}
    else if( ParseCommand( &Str, TEXT("NAMECOUNT") ) )
    {
        Ar.Logf( TEXT("Name count is: %d"), FName::GetMaxNames() );
        return 1;
    }
	else if( ParseCommand( &Str, TEXT("START") ) || ParseCommand( &Str, TEXT("MAP") )) // gam
	{
		FString Error;
		if( Client && Client->Viewports.Num() )
			SetClientTravel( Client->Viewports(0), Str, 0, TRAVEL_Absolute );
		else
		if( !Browse( FURL(&LastURL,Str,TRAVEL_Absolute), NULL, Error ) && Error!=TEXT("") )
			Ar.Logf( TEXT("Start failed: %s"), *Error );
		return 1;
	}
	else if( ParseCommand( &Str, TEXT("SERVERTRAVEL") ) && (GIsServer && !GIsClient) )
	{
		GLevel->GetLevelInfo()->eventServerTravel(Str,0);
		return 1;
	}
	else if( (GIsServer && !GIsClient) && ParseCommand( &Str, TEXT("SAY") ) )
	{
		GLevel->GetLevelInfo()->Game->eventBroadcast(NULL, Str, NAME_None);
		return 1;
	}
	else if( ParseCommand(&Str, TEXT("DISCONNECT")) )
	{
		FString Error;

        // gam ---		
//        FString FailedURL = TEXT("?failed");
		FString FailedURL = TEXT("?disconnect");		
        
        while( appIsSpace( *Str ) )
            Str++;
            
        if( *Str )
        {
            FailedURL += TEXT("?");
            FailedURL += Str;
        }
        
		if( GIsSoaking )
		    UObject::StaticExec( TEXT("OBJ LIST"), *GLog );
		
		if( Client && Client->Viewports.Num() )
		{
#ifdef _XBOX
            Client->Exec( TEXT("E3-WRITEPLAYER") ); // sjs - E3
#endif
			if( GLevel && GLevel->NetDriver && GLevel->NetDriver->ServerConnection && GLevel->NetDriver->ServerConnection->Channels[0] )
			{
				GLevel->NetDriver->ServerConnection->Channels[0]->Close();
				GLevel->NetDriver->ServerConnection->FlushNet();
			}
			if( GPendingLevel && GPendingLevel->NetDriver && GPendingLevel->NetDriver->ServerConnection && GPendingLevel->NetDriver->ServerConnection->Channels[0] )
			{
				GPendingLevel->NetDriver->ServerConnection->Channels[0]->Close();
				GPendingLevel->NetDriver->ServerConnection->FlushNet();
			}
			if ( GLevel->GetLevelInfo()->NetMode == NM_Standalone && GLevel->GetLevelInfo()->Game->CurrentGameProfile != NULL ) { 
				GLevel->GetLevelInfo()->Game->CurrentGameProfile->bInLadderGame = false;
			}
			SetClientTravel( Client->Viewports(0), *FailedURL, 0, TRAVEL_Absolute );
		}
		else
        {
		    if( !Browse( FURL(&LastURL,*FailedURL,TRAVEL_Absolute), NULL, Error ) && Error!=TEXT("") )
			    Ar.Logf( TEXT("Disconnect failed: %s"), *Error );
        }
		// --- gam
		return 1;
	}
	else if( ParseCommand(&Str, TEXT("RECONNECT")) )
	{
		FString Error;
		if( Client && Client->Viewports.Num() )
		{
			Client->Viewports(0)->GUIController->eventCloseAll(false);
			SetClientTravel( Client->Viewports(0), *LastURL.String(), 0, TRAVEL_Absolute );
		}
		else
		if( !Browse( FURL(LastURL), NULL, Error ) && Error!=TEXT("") )
			Ar.Logf( TEXT("Reconnect failed: %s"), *Error );
		return 1;
	}
	else if( ParseCommand(&Cmd,TEXT("EXIT")) || ParseCommand(&Cmd,TEXT("QUIT")))
	{
		if( GLevel && GLevel->NetDriver && GLevel->NetDriver->ServerConnection && GLevel->NetDriver->ServerConnection->Channels[0] )
		{
			GLevel->NetDriver->ServerConnection->Channels[0]->Close();
			GLevel->NetDriver->ServerConnection->FlushNet();
		}
		if( GPendingLevel && GPendingLevel->NetDriver && GPendingLevel->NetDriver->ServerConnection && GPendingLevel->NetDriver->ServerConnection->Channels[0] )
		{
			GPendingLevel->NetDriver->ServerConnection->Channels[0]->Close();
			GPendingLevel->NetDriver->ServerConnection->FlushNet();
		}

        // gam ---
        if( GLevel && GLevel->GetLevelInfo() && GLevel->GetLevelInfo()->NetMode != NM_Client )
            GLevel->GetLevelInfo()->Game->eventGameEnding();
        //if( GLevel )
        //    GLevel->SetActorCollision( 0, 0 );
        // --- gam
 
		Ar.Log( TEXT("Closing by request") );
		appRequestExit( 0 );
		return 1;
	}
	else if( ParseCommand( &Str, TEXT("GETCURRENTTICKRATE") ) )
	{
		Ar.Logf( TEXT("%f"), CurrentTickRate );
		return 1;
	}
	else if( ParseCommand( &Str, TEXT("GETMAXTICKRATE") ) )
	{
		Ar.Logf( TEXT("%f"), GetMaxTickRate() );
		return 1;
	}
	else if( ParseCommand( &Str, TEXT("GSPYLITE") ) )
	{
		FString Error;
		appLaunchURL( TEXT("GSpyLite.exe"), TEXT(""), &Error );
		return 1;
	}
	else if( ParseCommand(&Str,TEXT("SAVEGAME")) )
	{
		if( appIsDigit(Str[0]) )
			SaveGame( appAtoi(Str) );
		return 1;
	}
	else if( ParseCommand( &Cmd, TEXT("CANCEL") ) )
	{
		static UBOOL InCancel = 0;
		if( !InCancel )	
		{
			//!!Hack for broken Input subsystem.  JP.
			//!!Inside LoadMap(), ResetInput() is called,
			//!!which can retrigger an Exec call.
			InCancel = 1;
			if( GPendingLevel )
			{
				if( GPendingLevel->TrySkipFile() )
				{
					InCancel = 0;
					return 1;
				}
				SetProgress( *FString::Printf(TEXT("menu:%s"),*DisconnectMenuClass),LocalizeProgress(TEXT("CancelledConnect"),TEXT("Engine")), TEXT("") );
			}
			else
			{
				debugf(TEXT("Set Progress from UnGame::Cancel"));
				SetProgress( TEXT(""), TEXT(""), TEXT(""), 0.f );
			}

			CancelPending();
			InCancel = 0;
		}
		return 1;
	}
    // gam ---
    else if( ParseCommand(&Cmd,TEXT("SOUND_REBOOT"))  )
    {
        if( Audio )
        {
            UViewport* Viewport = Audio->GetViewport();
            delete Audio;
            InitAudio();
            if( Audio )
                Audio->SetViewport( Viewport );
        }
		return 1;
    }
    // --- gam
	else if( GLevel && GLevel->Exec( Cmd, Ar ) )
	{
		return 1;
	}
	else if( GLevel && GLevel->GetLevelInfo()->Game && GLevel->GetLevelInfo()->Game->ScriptConsoleExec(Cmd,Ar,NULL) )
	{
		return 1;
	}
	else
	{
		// disallow set of pawn property if network game
		INT AllowSet = 1;
		if ( GLevel && (GLevel->GetLevelInfo()->NetMode == NM_Client) )
		{
			const TCHAR *Str = Cmd;
			if ( ParseCommand(&Str,TEXT("SET")) )
			{
				TCHAR ClassName[256];
				UClass* Class;
				if
				(	ParseToken( Str, ClassName, ARRAY_COUNT(ClassName), 1 )
				&&	(Class=FindObject<UClass>( ANY_PACKAGE, ClassName))!=NULL )
				{
					TCHAR PropertyName[256];
					UProperty* Property;
					if
					(	ParseToken( Str, PropertyName, ARRAY_COUNT(PropertyName), 1 )
					&&	(Property=FindField<UProperty>( Class, PropertyName))!=NULL )
					{
						if ( Class->IsChildOf(AActor::StaticClass()) 
							&& !Class->IsChildOf(AGameInfo::StaticClass()) 
							&& !(Property->PropertyFlags & (CPF_Config|CPF_GlobalConfig)) )
						{
							AllowSet = 0;
							debugf(
								TEXT("Not allowing set command: %u %u %u"),
								Class->IsChildOf(AActor::StaticClass()),
								!Class->IsChildOf(AGameInfo::StaticClass()),
								!(Property->PropertyFlags & (CPF_Config|CPF_GlobalConfig))
								);
						}
					}
				}
			}
		}
		if( AllowSet && UEngine::Exec( Cmd, Ar ) )
			return 1;
		else
			return 0;
	}

	unguard;
}

/*-----------------------------------------------------------------------------
	Serialization.
-----------------------------------------------------------------------------*/

//
// Serializer.
//
void UGameEngine::Serialize( FArchive& Ar )
{
	guard(UGameEngine::Serialize);
	Super::Serialize( Ar );

	Ar << GLevel << GEntry << GPendingLevel;

	unguardobj;
}

/*-----------------------------------------------------------------------------
	Game entering.
-----------------------------------------------------------------------------*/

//
// Cancel pending level.
//
void UGameEngine::CancelPending()
{
	guard(UGameEngine::CancelPending);
	if( GPendingLevel )
	{
		if( GPendingLevel->NetDriver && GPendingLevel->NetDriver->ServerConnection && GPendingLevel->NetDriver->ServerConnection->Channels[0] )
		{
			GPendingLevel->NetDriver->ServerConnection->Channels[0]->Close();
			GPendingLevel->NetDriver->ServerConnection->FlushNet();
		}
		delete GPendingLevel;
		GPendingLevel = NULL;
	}
	unguard;
}

//
// Match Viewports to actors.
//
static void MatchViewportsToActors( UClient* Client, ULevel* Level, const FURL& URL, const TCHAR* MenuClassName )
{
	guard(MatchViewportsToActors);

	for( INT i=0; i<Client->Viewports.Num(); i++ )
	{
		FString Error;
		UViewport* Viewport = Client->Viewports(i);
		debugf( NAME_Log, TEXT("Spawning new actor for Viewport %s"), Viewport->GetName() );

		APlayerController* PlayerController = Level->SpawnPlayActor( Viewport, ROLE_SimulatedProxy, URL, Error );

        if( !PlayerController)
			appErrorf( TEXT("%s"), *Error );

		if ( appStrlen(MenuClassName) && !Viewport->GUIController->bActive )
			Viewport->GUIController->eventOpenMenu(MenuClassName,TEXT(""),TEXT(""));

	}
	unguardf(( TEXT("(%s)"), *Level->URL.Map ));
}

//
// Browse to a specified URL, relative to the current one.
//
UBOOL UGameEngine::Browse( FURL URL, const TMap<FString,FString>* TravelInfo, FString& Error )
{
	guard(UGameEngine::Browse);
	Error = TEXT("");
	const TCHAR* Option;

	// Tear down voice chat.
	if( Audio )
		Audio->LeaveVoiceChat();

	// Convert .unreal link files.
	const TCHAR* LinkStr = TEXT(".unreal");//!!
	if( appStrstr(*URL.Map,LinkStr)-*URL.Map==appStrlen(*URL.Map)-appStrlen(LinkStr) )
	{
		debugf( TEXT("Link: %s"), *URL.Map );
		FString NewUrlString;
		if( GConfig->GetString( TEXT("Link")/*!!*/, TEXT("Server"), NewUrlString, *URL.Map ) )
		{
			// Go to link.
			URL = FURL( NULL, *NewUrlString, TRAVEL_Absolute );//!!
		}
		else
		{
			// Invalid link.
			guard(InvalidLink);
			Error = FString::Printf( LocalizeError(TEXT("InvalidLink"),TEXT("Engine")), *URL.Map );
			unguard;
			return 0;
		}
	}

	// Crack the URL.
	debugf( TEXT("Browse: %s"), *URL.String() );

	// Handle it.
	if( !URL.Valid )
	{
		// Unknown URL.
		guard(UnknownURL);
		Error = FString::Printf( LocalizeError(TEXT("InvalidUrl"),TEXT("Engine")), *URL.String() );
		unguard;
		return 0;
	}
	else if ( URL.HasOption(TEXT("failed")) || URL.HasOption(TEXT("disconnect")) || URL.HasOption(TEXT("closed")) || URL.HasOption(TEXT("entry")) )
	{
		// Handle failure URL.
		guard(FailedURL);
		
	    // gam ---
	    for( INT i=LastURL.Op.Num()-1; i>=0; i-- )
	    {
		    if( !appStrPrefix( *LastURL.Op(i), TEXT("SpectatorOnly=") ) )
		    {
			    LastURL.Op.Remove( i );
			    break;
			}
	    }
	    for( INT i=URL.Op.Num()-1; i>=0; i-- )
	    {
		    if( !appStrPrefix( *URL.Op(i), TEXT("SpecatorOnly=") ) )
		    {
			    URL.Op.Remove( i );
			    break;
			}
	    }
        // --- gam
		
		debugf( NAME_Log, LocalizeError(TEXT("AbortToEntry"),TEXT("Engine")) );
		if( GLevel && GLevel!=GEntry )
        {
            GLevel->SetActorCollision( 0, 1 ); // gam
			ResetLoaders( GLevel->GetOuter(), 1, 0 );
        }
		NotifyLevelChange();
		GLevel = GEntry;
        if( GLevel )
        {
		    GLevel->GetLevelInfo()->LevelAction = LEVACT_None;
		    check(Client && Client->Viewports.Num());

			
			if (GPendingLevel)
				debugf(TEXT("GP=TRUE"));
			else
				debugf(TEXT("GP=FALSE"));

			if ( !URL.HasOption(TEXT("closed")) || !GPendingLevel )
			{
					MatchViewportsToActors( Client, GLevel, URL, *MainMenuClass );
			}
			else
			    MatchViewportsToActors( Client, GLevel, URL, TEXT("") );

			// --- gam

		    if( Audio )
			    Audio->SetViewport( Audio->GetViewport() );
    		
#if 0 // sjs - questionable Xbox memory reclaimation, alot of crashing here, but could be legit...
		    {for( TObjectIterator<AActor> It; It; ++It )
			    if( It->IsIn(GLevel->GetOuter()) )
				    It->SetFlags( RF_EliminateObject );}
		    {for( INT i=0; i<GLevel->Actors.Num(); i++ )
			    if( GLevel->Actors(i) )
				    GLevel->Actors(i)->ClearFlags( RF_EliminateObject );}
		    CollectGarbage( RF_Native );
		    Flush(0);
		    debugf(TEXT("Done flush."));
#endif
        }

		//CollectGarbage( RF_Native ); // Causes texture corruption unless you flush.
		if( URL.HasOption(TEXT("failed")) )
		{
			if ( ( !GPendingLevel ) && (!Client->Viewports(0)->GUIController->bActive) )
			{
				SetProgress( *FString::Printf(TEXT("menu:%s"),*DisconnectMenuClass),LocalizeError(TEXT("ConnectionFailed"),TEXT("Engine")), TEXT("") );
			}
		}
		unguard;
		return 1;
	}
	else if( URL.HasOption(TEXT("pop")) )
	{
		// Pop the hub.
		guard(PopURL);
		if( GLevel && GLevel->GetLevelInfo()->HubStackLevel>0 )
		{
			TCHAR Filename[256], SavedPortal[256];
			appSprintf( Filename, TEXT("%s") PATH_SEPARATOR TEXT("Game%i.usa"), *GSys->SavePath, GLevel->GetLevelInfo()->HubStackLevel-1 );
			appStrcpy( SavedPortal, *URL.Portal );
			URL = FURL( &URL, Filename, TRAVEL_Partial );
			URL.Portal = SavedPortal;
		}
		else return 0;
		unguard;
	}
	else if( URL.HasOption(TEXT("restart")) )
	{
		// Handle restarting.
		guard(RestartURL);
		URL = LastURL;
		unguard;
	}
	else if( (Option=URL.GetOption(TEXT("load="),NULL))!=NULL )
	{
		// Handle loadgame.
		guard(LoadURL);
		FString Error, Temp=FString::Printf( TEXT("%s") PATH_SEPARATOR TEXT("Save%i.usa?load"), *GSys->SavePath, appAtoi(Option) );
		if( LoadMap(FURL(&LastURL,*Temp,TRAVEL_Partial),NULL,NULL,Error) )
		{
			// Copy the hub stack.
			INT i;
			for( i=0; i<GLevel->GetLevelInfo()->HubStackLevel; i++ )
			{
				TCHAR Src[256], Dest[256];//!!
				appSprintf( Src, TEXT("%s") PATH_SEPARATOR TEXT("Save%i%i.usa"), *GSys->SavePath, appAtoi(Option), i );
				appSprintf( Dest, TEXT("%s") PATH_SEPARATOR TEXT("Game%i.usa"), *GSys->SavePath, i );
				GFileManager->Copy( Src, Dest );
			}
			while( 1 )
			{
				Temp = FString::Printf( TEXT("%s") PATH_SEPARATOR TEXT("Game%i.usa"), *GSys->SavePath, i++ );
				if( GFileManager->FileSize(*Temp)<=0 )
					break;
				GFileManager->Delete( *Temp );
			}
			LastURL = GLevel->URL;
			
			// gam ---
	        for( INT i=LastURL.Op.Num()-1; i>=0; i-- )
	        {
		        if( !appStrPrefix( *LastURL.Op(i), TEXT("Menu=") ) )
		        {
			        LastURL.Op.Remove( i );
			        break;
			    }
	        }
            // --- gam
            
			return 1;
		}
		else return 0;
		unguard;
	}

	// Handle normal URL's.
	if( URL.IsLocalInternal() )
	{
		// Local map file.
		guard(LocalMapURL);
#ifdef _XBOX // sjs - reboot the Xbox when launching a new map
		if ( !XBoxRelaunch || appStrnicmp( *URL.Map, TEXT("Entry"), 5 ) == 0 || GLevel == NULL )
		{
		return LoadMap( URL, NULL, TravelInfo, Error )!=NULL;
		}
		else
		{
			FString launch = FString::Printf(TEXT("RELAUNCH %s"), *URL.String() );
			return Exec( *launch );
		}
#else
		return LoadMap( URL, NULL, TravelInfo, Error )!=NULL;
#endif
		unguard;
	}
	else if( URL.IsInternal() && GIsClient )
	{
		// Network URL.
		guard(NetworkURL);
		if( GPendingLevel )
			CancelPending();
		GPendingLevel = new UNetPendingLevel( this, URL );
		if( !GPendingLevel->NetDriver )
		{
			SetProgress( *FString::Printf(TEXT("menu:%s"),*DisconnectMenuClass), TEXT("Networking Failed"), *GPendingLevel->Error );
			delete GPendingLevel;
			GPendingLevel = NULL;
		}
		return 0;
		unguard;
	}
	else if( URL.IsInternal() )
	{
		// Invalid.
		guard(InvalidURL);
		Error = LocalizeError(TEXT("ServerOpen"),TEXT("Engine"));
		unguard;
		return 0;
	}
	else
	{
		// External URL.
		guard(ExternalURL);
		//Client->Viewports(0)->Exec(TEXT("ENDFULLSCREEN"));
		appLaunchURL( *URL.String(), TEXT(""), &Error );
		unguard;
		return 0;
	}
	unguard;
}

//
// Notify that level is changing
//
void UGameEngine::NotifyLevelChange()
{
	guard(UGameEngine::NotifyLevelChange);

	// Make sure cinematic view is turned off when the level changes

    if( Client && Client->Viewports.Num() )
		for( INT x = 0 ; x < Client->Viewports.Num() ; x++ )
			Client->Viewports(x)->bRenderCinematics = 0;

	// gam ---
    if( Client && Client->Viewports.Num() && Client->Viewports(0)->Console )
		CastChecked<UConsole>(Client->Viewports(0)->Console)->eventNotifyLevelChange();
    // --- gam

    // sjs ---
    if ( Audio )
        Audio->Exec(TEXT("StopMusic"));
    // --- sjs

	unguard;	
}

// Fixup a map
// hack to post release fix map actor problems without breaking compatibility
void UGameEngine::FixUpLevel( )
{
	debugf(TEXT("Level is %s"), GLevel->GetFullName());
}
//
// Load a map.
//
ULevel* UGameEngine::LoadMap( const FURL& URL, UPendingLevel* Pending, const TMap<FString,FString>* TravelInfo, FString& Error )
{
	guard(UGameEngine::LoadMap);

    if( GEntry ) GEntry->CleanupDestroyed( 1 ); // gam

	Error = TEXT("");
	debugf( NAME_Log, TEXT("LoadMap: %s"), *URL.String() );
	GInitRunaway();

	// Remember current level's stack level.
	INT SavedHubStackLevel = GLevel ? GLevel->GetLevelInfo()->HubStackLevel : 0;

	// Get network package map.
	UPackageMap* PackageMap = NULL;
	if( Pending )
		PackageMap = Pending->GetDriver()->ServerConnection->PackageMap;

	// Verify that we can load all packages we need.
	UObject* MapParent = NULL;
	guard(VerifyPackages);
	try
	{
		BeginLoad();
		if( Pending )
		{
			// Verify that we can load everything needed for client in this network level.
			for( INT i=0; i<PackageMap->List.Num(); i++ )
				PackageMap->List(i).Linker = GetPackageLinker
				(
					PackageMap->List(i).Parent,
					NULL,
					LOAD_Verify | LOAD_Throw | LOAD_NoWarn | LOAD_NoVerify,
					NULL,
					&PackageMap->List(i).Guid
				);
			for( INT i=0; i<PackageMap->List.Num(); i++ )
				VerifyLinker( PackageMap->List(i).Linker );
			if( PackageMap->List.Num() )
				MapParent = PackageMap->List(0).Parent;
		}
		LoadObject<ULevel>( MapParent, TEXT("MyLevel"), *URL.Map, LOAD_Verify | LOAD_Throw | LOAD_NoWarn, NULL );
		EndLoad();

	#if DEMOVERSION
			// If we are a demo, prevent third party maps from being loaded.
			if( !Pending )
			{
				FString FileName(FString(TEXT("../Maps/"))+URL.Map);
				if( FileName.Right(4).Caps() != TEXT(".UT2"))
					FileName = FileName + TEXT(".ut2");
				INT FileSize = GFileManager->FileSize( *FileName );
				//debugf(TEXT("Looking for file: %s %d"), *FileName, FileSize);
				if( ( FileName.Caps() != TEXT("../MAPS/DM-ANTALUS.UT2") || FileSize != 3803652 ) &&
					( FileName.Caps() != TEXT("../MAPS/DM-ASBESTOS.UT2") || FileSize != 12895376 ) &&
					( FileName.Caps() != TEXT("../MAPS/BR-ANUBIS.UT2") || FileSize != 17761876 ) &&
					( FileName.Caps() != TEXT("../MAPS/CTF-CITADEL.UT2") || FileSize != 6363943 ) &&
					( FileName.Caps() != TEXT("../MAPS/NVIDIALOGO.UT2") || FileSize != 462273 ) &&
					( FileName.Caps() != TEXT("../MAPS/ENTRY.UT2") || FileSize != 334613 ) )
				{
					Error = TEXT("Sorry, only the retail version of UT2003 can load third party maps.");
//					SetProgress( LocalizeError(TEXT("UrlFailed"),TEXT("Core")), *Error, TEXT("") );
					SetProgress( *FString::Printf(TEXT("menu:%s"),*DisconnectMenuClass), *Error, TEXT("") );
					return NULL;
				}
			}
	#endif
	}

	#if UNICODE
	catch( TCHAR* CatchError )
	#else
	catch( char* CatchError )
	#endif
	{
		// Safely failed loading.
		EndLoad();
		Error = CatchError;
        #if UNICODE
        TCHAR *e = CatchError;
        #else
        TCHAR *e = ANSI_TO_TCHAR(CatchError);
        #endif

		if (GLevel==GEntry)
			SetProgress( *FString::Printf(TEXT("menu:XInterface.UT2StatusMsg"),*MainMenuClass), LocalizeError(TEXT("UrlFailed"),TEXT("Core")), e );
		else if (Pending && Pending->NetDriver && Pending->NetDriver->ServerConnection)
			SetProgress( *FString::Printf(TEXT("menu:%s"),*DisconnectMenuClass), LocalizeError(TEXT("UrlFailed"),TEXT("Core")), e );
		else
			SetProgress( TEXT(""), LocalizeError(TEXT("UrlFailed"),TEXT("Core")), e );

		// Clear the partially initialized level loader from memory.
		if(Pending && PackageMap->List.Num() && PackageMap->List(0).Parent)
			ResetLoaders(PackageMap->List(0).Parent,0,1);
		return NULL;
	}
	unguard;

	// Display loading screen (gam -- moved after package map verification)
	guard(LoadingScreen);
	if( Client && Client->Viewports.Num() && GLevel && !URL.HasOption(TEXT("quiet")) )
	{
		GLevel->GetLevelInfo()->LevelAction = LEVACT_Loading;
		GLevel->GetLevelInfo()->Pauser = NULL;

        // gam --- My dream is to have this not be an actor but perhaps a UObject that we don't destroy immediately:
        // the idea would be we're spawn in here and it'd get updates throughout the life of this function call. Then,
        // before we return, we'd destroy it.
        // Also, perhaps through some of that object registry wizardry we'd pick a random vignette each time instead
        // of going to TestVignette every time.

        AVignette* Vignette = NULL;
		const TCHAR *gametype, *TeamScreen;
		gametype = URL.GetOption(TEXT("Game="),TEXT(""));
		TeamScreen = URL.GetOption(TEXT("TeamScreen="),TEXT("True"));

		if ( GLevel->GetLevelInfo()->NetMode == NM_Standalone && 
			 !(Client->Viewports(0)->SizeX < 640 ) &&		// hardcoded force for menu resolution, see D3DRenderDevice.cpp
			 !(Client->Viewports(0)->SizeY < 480 ) &&		// hardcoded force for menu resolution, see D3DRenderDevice.cpp
			 GLevel->GetLevelInfo()->Game->CurrentGameProfile != NULL && 
			 GLevel->GetLevelInfo()->Game->CurrentGameProfile->bInLadderGame &&
			 //(appStrcmp (gametype, TEXT(""))) &&
			 (appStricmp (gametype, TEXT("xGame.xDeathmatch"))) &&
			 (appStricmp (gametype, TEXT("xGame.BossDM"))) &&
			 (appStricmp (TeamScreen, TEXT("false"))) )
		{ 
			UClass* VignetteClass = StaticLoadClass( AVignette::StaticClass(), NULL, TEXT("XInterface.UT2SP_LadderLoading"), NULL, LOAD_NoFail, NULL );
			Vignette = CastChecked<AVignette>( GLevel->SpawnActor( VignetteClass ) );
			Vignette->MapName = URL.Map;
			Vignette->eventInit();
		} else if( URL.Map.Left (4) != TEXT("Menu") )
        {
            UClass* VignetteClass = StaticLoadClass( AVignette::StaticClass(), NULL, TEXT("XInterface.TestVignette"), NULL, LOAD_NoFail, NULL );
            Vignette = CastChecked<AVignette>( GLevel->SpawnActor( VignetteClass ) );
            Vignette->MapName = URL.Map;
            Vignette->eventInit();
        }

        PaintProgress( Vignette, 1.0F );

        if( Vignette )
            GLevel->DestroyActor( Vignette );

        // --- gam

		if( Audio )
			Audio->SetViewport( Audio->GetViewport() );
		GLevel->GetLevelInfo()->LevelAction = LEVACT_None;
	}
	unguard;

	// Notify of the level change, before we dissociate Viewport actors
	guard(NotifyLevelChange);
	if( GLevel )
		NotifyLevelChange();
	unguard;

	// Dissociate Viewport actors.
	guard(DissociateViewports);
	if( Client )
	{
		for( INT i=0; i<Client->Viewports.Num(); i++ )
		{
			APlayerController* Actor    = Client->Viewports(i)->Actor;
			ULevel*      Level          = Actor->GetLevel();
			Actor->Player               = NULL;
			Client->Viewports(i)->Actor = NULL;
			if ( Actor->Pawn )
				Level->DestroyActor(Actor->Pawn);
			Level->DestroyActor( Actor );
		}
	}
	unguard;

	// Clean up game state.
	guard(ExitLevel);
	if( GLevel )
	{
		// Shut down.
        GLevel->SetActorCollision( 0, 1 ); // gam
		ResetLoaders( GLevel->GetOuter(), 1, 0 );
		if( GLevel->NetDriver )
		{
			delete GLevel->NetDriver;
			GLevel->NetDriver = NULL;
		}
		if( GLevel->DemoRecDriver )
		{
			delete GLevel->DemoRecDriver;
			GLevel->DemoRecDriver = NULL;
		}
		if( URL.HasOption(TEXT("push")) )
		{
			// Save the current level minus players actors.
			GLevel->CleanupDestroyed( 1 );
			TCHAR Filename[256];
			appSprintf( Filename, TEXT("%s") PATH_SEPARATOR TEXT("Game%i.usa"), *GSys->SavePath, SavedHubStackLevel );
			SavePackage( GLevel->GetOuter(), GLevel, 0, Filename, GLog );
		}

#ifdef WITH_KARMA
		if(!GIsEditor) // dont need to do this in editor - no Karma runs.
		{
			// To save memory, we remove Karma from all actors before loading the new level.
			for( INT iActor=0; iActor<GLevel->Actors.Num(); iActor++ )
			{
				AActor* actor = GLevel->Actors(iActor);
				if(actor)
				{
					KTermActorKarma(actor);
				}
			}
		}
#endif
		GLevel = NULL;
	}
	unguard;

	// sjs ---
#ifdef _XBOX
	guard(CleanupXbox);
	if( GLevel && appStricmp(GLevel->GetOuter()->GetName(),TEXT("Entry"))!=0 )
	{
		Flush(0);
		{for( TObjectIterator<AActor> It; It; ++It )
			if( It->IsIn(GLevel->GetOuter()) )
				It->SetFlags( RF_EliminateObject );}
		{for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) )
				GLevel->Actors(i)->ClearFlags( RF_EliminateObject );}
		CollectGarbage( RF_Native );
	}
	unguard;
#endif
	// --- sjs

	// Load the level and all objects under it, using the proper Guid.
	guard(LoadLevel);
	GLevel = LoadObject<ULevel>( MapParent, TEXT("MyLevel"), *URL.Map, LOAD_NoFail, NULL );
	unguard;

	// If pending network level.
	if( Pending )
	{
		// If playing this network level alone, ditch the pending level.
		if( Pending && Pending->LonePlayer )
			Pending = NULL;

		// Setup network package info.
		PackageMap->Compute();
		for( INT i=0; i<PackageMap->List.Num(); i++ )
			if( PackageMap->List(i).LocalGeneration!=PackageMap->List(i).RemoteGeneration )
				Pending->GetDriver()->ServerConnection->Logf( TEXT("HAVE GUID=%s GEN=%i"), PackageMap->List(i).Guid.String(), PackageMap->List(i).LocalGeneration );
	}

	// Verify classes.
	guard(VerifyClasses);
	VERIFY_CLASS_OFFSET( A, Actor,       Owner         );
	VERIFY_CLASS_OFFSET( A, Actor,       TimerCounter  );
	VERIFY_CLASS_OFFSET( A, PlayerController,  Player  );
	VERIFY_CLASS_OFFSET( A, Pawn,  Health );

#if 0
    // test some IsA hacks!-----------------------
    INT iterCycles = 0;
    clock(iterCycles);
    for( int i=0; i<10000; i++ )
    {
        for( TFieldFlagIterator<UProperty,CLASS_IsAUProperty> It(APlayerController::StaticClass()); It; ++It )
        {

        }
    }
    unclock(iterCycles);

    INT isAIterCycles = 0;
    clock(isAIterCycles);
    for( int i=0; i<10000; i++ )
    {
        for( TFieldIterator<UProperty> It(APlayerController::StaticClass()); It; ++It )
        {

        }
    }
    unclock(isAIterCycles);

    debugf(TEXT("*** Iteration cycles: %d"), iterCycles );
    debugf(TEXT("*** IsA       cycles: %d"), isAIterCycles );
    // test some IsA hacks!-----------------------
#endif


    // gam ---
    VERIFY_CLASS_SIZE( UCanvas );
    VERIFY_CLASS_SIZE( UCubemap );
    VERIFY_CLASS_SIZE( UEngine );
    VERIFY_CLASS_SIZE( UGameEngine );
    VERIFY_CLASS_SIZE( UPalette );
    VERIFY_CLASS_SIZE( UPlayer );
    VERIFY_CLASS_SIZE( UTexture );
    // --- gam

	unguard;

	// Get LevelInfo.
	check(GLevel);
	ALevelInfo* Info = GLevel->GetLevelInfo();
	Info->ComputerName = appComputerName();

	// Handle pushing.
	guard(ProcessHubStack);
	Info->HubStackLevel
	=	URL.HasOption(TEXT("load")) ? Info->HubStackLevel
	:	URL.HasOption(TEXT("push")) ? SavedHubStackLevel+1
	:	URL.HasOption(TEXT("pop" )) ? Max(SavedHubStackLevel-1,0)
	:	URL.HasOption(TEXT("peer")) ? SavedHubStackLevel
	:	                              0;
	unguard;

	// Handle pending level.
	guard(ActivatePending);
	if( Pending )
	{
		check(Pending==GPendingLevel);

		// Hook network driver up to level.
		GLevel->NetDriver = Pending->NetDriver;
		if( GLevel->NetDriver )
			GLevel->NetDriver->Notify = GLevel;

		// Hook demo playback driver to level
		GLevel->DemoRecDriver = Pending->DemoRecDriver;
		if( GLevel->DemoRecDriver )
			GLevel->DemoRecDriver->Notify = GLevel;

		// Setup level.
		GLevel->GetLevelInfo()->NetMode = NM_Client;
	}
	else check(!GLevel->NetDriver);
	unguard;

	// Set level info.
	guard(InitLevel);
	if( !URL.GetOption(TEXT("load"),NULL) )
		GLevel->URL = URL;
	Info->EngineVersion = FString::Printf( TEXT("%i"), ENGINE_VERSION );
	Info->MinNetVersion = FString::Printf( TEXT("%i"), ENGINE_MIN_NET_VERSION );
	GLevel->Engine = this;
	if( TravelInfo )
		GLevel->TravelInfo = *TravelInfo;
	unguard;

	// Remove cubemaps.
#if 1
	if( GLevel->Engine->GRenDev && GLevel->Engine->GRenDev->Is3dfx )
	{
		for( TObjectIterator<UModifier> It; It; ++It )
		{
			if( It->Material && It->Material->IsA(UCubemap::StaticClass()) )
				It->Material = NULL;
		}

		for( TObjectIterator<UCombiner> It; It; ++It )
		{
			if( It->Material1 && It->Material1->IsA(UCubemap::StaticClass()) )
			{
				It->Material1 = It->Material2;
				It->Material2 = NULL;
			}
			if( It->Material2 && It->Material2->IsA(UCubemap::StaticClass()) )
				It->Material2 = NULL;
		}

		for( TObjectIterator<UShader> It; It; ++It )
		{
			if( It->Diffuse && It->Diffuse->IsA(UCubemap::StaticClass()) )
				It->Diffuse = NULL;

			if( It->Opacity && It->Opacity->IsA(UCubemap::StaticClass()) )
				It->Opacity = NULL;

			if( It->Specular && It->Specular->IsA(UCubemap::StaticClass()) )
				It->Specular = NULL;

			if( It->SpecularityMask && It->SpecularityMask->IsA(UCubemap::StaticClass()) )
				It->SpecularityMask = NULL;

			if( It->SelfIllumination && It->SelfIllumination->IsA(UCubemap::StaticClass()) )
				It->SelfIllumination = NULL;

			if( It->SelfIlluminationMask && It->SelfIlluminationMask->IsA(UCubemap::StaticClass()) )
				It->SelfIlluminationMask = NULL;

			if( It->Detail && It->Detail->IsA(UCubemap::StaticClass()) )
				It->Detail = NULL;
		}
	}
#endif

	// Purge unused objects and flush caches.
	guard(Cleanup);
	if( appStricmp(GLevel->GetOuter()->GetName(),TEXT("Entry"))!=0 )
	{
		Flush(0);
		{for( TObjectIterator<AActor> It; It; ++It )
			if( It->IsIn(GLevel->GetOuter()) )
				It->SetFlags( RF_EliminateObject );}
		{for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) )
				GLevel->Actors(i)->ClearFlags( RF_EliminateObject );}
		CollectGarbage( RF_Native );
	}
	unguard;

	// Tell the audio driver to clean up.
	if( Audio )
		Audio->CleanUp();

	// Init collision.
	GLevel->SetActorCollision( 1 );

	// Setup zone distance table for sound damping. Fast enough: Approx 3 msec.
	guard(SetupZoneTable);
	QWORD OldConvConn[64];
	QWORD ConvConn[64];
	for( INT i=0; i<64; i++ )
	{
		for( INT j=0; j<64; j++ )
		{
			OldConvConn[i] = GLevel->Model->Zones[i].Connectivity;
			if( i == j )
				GLevel->ZoneDist[i][j] = 0;
			else
				GLevel->ZoneDist[i][j] = 255;
		}
	}
	for( INT i=1; i<64; i++ )
	{
		for( INT j=0; j<64; j++ )
			for( INT k=0; k<64; k++ )
				if( (GLevel->ZoneDist[j][k] > i) && ((OldConvConn[j] & ((QWORD)1 << k)) != 0) )
					GLevel->ZoneDist[j][k] = i;
		for( INT j=0; j<64; j++ )
			ConvConn[j] = 0;
		for( INT j=0; j<64; j++ )
			for( INT k=0; k<64; k++ )
				if( (OldConvConn[j] & ((QWORD)1 << k)) != 0 )
					ConvConn[j] = ConvConn[j] | OldConvConn[k];
		for( INT j=0; j<64; j++ )
			OldConvConn[j] = ConvConn[j];
	}
	unguard;

	// Update the LevelInfo's time.
	GLevel->UpdateTime(Info);

	// Init the game info.
	TCHAR Options[1024]=TEXT("");
	TCHAR GameClassName[256]=TEXT("");
	TCHAR MenuClassName[256]=TEXT(""); // gam
	FString Error=TEXT("");
	guard(InitGameInfo);
	for( INT i=0; i<URL.Op.Num(); i++ )
	{
		appStrcat( Options, TEXT("?") );
		appStrcat( Options, *URL.Op(i) );
		Parse( *URL.Op(i), TEXT("GAME="), GameClassName, ARRAY_COUNT(GameClassName) );
		Parse( *URL.Op(i), TEXT("MENU="), MenuClassName, ARRAY_COUNT(MenuClassName) ); // gam
	}
	if( GLevel->IsServer() && !Info->Game )
	{
		// Get the GameInfo class.
		UClass* GameClass=NULL;
		if ( GameClassName[0] )
			GameClass = StaticLoadClass( AGameInfo::StaticClass(), NULL, GameClassName, NULL, 0, PackageMap );
		if( !GameClass && Info->DefaultGameType.Len() > 0 ) // sjs
			GameClass = StaticLoadClass( AGameInfo::StaticClass(), NULL, *(Info->DefaultGameType), NULL, 0, PackageMap );
		if( !GameClass && appStricmp(GLevel->GetOuter()->GetName(),TEXT("Entry"))==0 ) // sjs
			GameClass = AGameInfo::StaticClass();
		if ( !GameClass ) // sjs
			GameClass = StaticLoadClass( AGameInfo::StaticClass(), NULL, Client ? TEXT("ini:Engine.Engine.DefaultGame") : TEXT("ini:Engine.Engine.DefaultServerGame"), NULL, 0, PackageMap ); // gam
        if ( !GameClass ) // sjs
			GameClass = AGameInfo::StaticClass();

        check(GameClass);

		// Spawn the GameInfo.
		debugf( NAME_Log, TEXT("Game class is '%s'"), GameClass->GetName() );
		Info->Game = (AGameInfo*)GLevel->SpawnActor( GameClass );
		check(Info->Game!=NULL);
	}
	unguard;

	// Listen for clients.
	guard(Listen);
	if( !Client || URL.HasOption(TEXT("Listen")) )
	{
		if( GPendingLevel )
		{
			guard(CancelPendingForListen);
			check(!Pending);
			delete GPendingLevel;
			GPendingLevel = NULL;
			unguard;
		}
		FString Error;
		if( !GLevel->Listen( Error ) )
			appErrorf( LocalizeError(TEXT("ServerListen"),TEXT("Engine")), *Error );
	}
	unguard;

	// Init detail.
	Info->DetailMode = DM_SuperHigh;
	if(Client && Client->Viewports.Num() && Client->Viewports(0)->RenDev)
	{
		if(Client->Viewports(0)->RenDev->SuperHighDetailActors)
			Info->DetailMode = DM_SuperHigh;
		else if(Client->Viewports(0)->RenDev->HighDetailActors)
			Info->DetailMode = DM_High;
		else
			Info->DetailMode = DM_Low;
	}

	// Clear any existing stat graphs.
	if(GStatGraph)
		GStatGraph->Reset();

	// Init level gameplay info.
	guard(BeginPlay);
	GLevel->iFirstDynamicActor = 0;
	if( !Info->bBegunPlay )
	{
        appResetTimer(); // sjs

		// fix up level problems
		FixUpLevel();

		// Check that paths are valid
		if ( !GLevel->GetLevelInfo()->bPathsRebuilt )
			debugf( NAME_Warning, TEXT("Paths may not be valid.")); // gam
		// Lock the level.
		debugf( NAME_Log, TEXT("Bringing %s up for play (%i) appSeconds: %f..."), GLevel->GetFullName(), appRound(GetMaxTickRate()), appSeconds() ); // sjs
		GLevel->TimeSeconds = 0;
		GLevel->GetLevelInfo()->TimeSeconds = 0;
		GLevel->GetLevelInfo()->GetDefaultPhysicsVolume()->bNoDelete = true;

		// Kill off actors that aren't interesting to the client.
		if( !GLevel->IsServer() )
		{
			for( INT i=0; i<GLevel->Actors.Num(); i++ )
			{
				AActor* Actor = GLevel->Actors(i);
				if( Actor )
				{
					if( Actor->bStatic || Actor->bNoDelete || Actor->IsA(AxEmitter::StaticClass()) ) // sjs
						Exchange( Actor->Role, Actor->RemoteRole );
					else
						GLevel->DestroyActor( Actor );
				}
			}
		}

		// Init touching actors & clear LastRenderTime
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) )
			{
				GLevel->Actors(i)->LastRenderTime = 0.f;
				GLevel->Actors(i)->Touching.Empty();
				GLevel->Actors(i)->PhysicsVolume = GLevel->GetLevelInfo()->GetDefaultPhysicsVolume();
			}


		// Init scripting.
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) )
				GLevel->Actors(i)->InitExecution();

		// Enable actor script calls.
		Info->bBegunPlay = 1;
		Info->bStartup = 1;

		// Init the game.
		if( Info->Game )
			Info->Game->eventInitGame( Options, Error );

		// Send PreBeginPlay.
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) && !GLevel->Actors(i)->bScriptInitialized )
				GLevel->Actors(i)->eventPreBeginPlay();

		// Set BeginPlay.
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) && !GLevel->Actors(i)->bScriptInitialized )
				GLevel->Actors(i)->eventBeginPlay();

		// Set zones.
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) && !GLevel->Actors(i)->bScriptInitialized )
				GLevel->Actors(i)->SetZone( 1, 1 );

		// set appropriate volumes for each actor
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) && !GLevel->Actors(i)->bScriptInitialized )
				GLevel->Actors(i)->SetVolumes();

		// Post begin play.
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) && !GLevel->Actors(i)->bScriptInitialized )
			{
				GLevel->Actors(i)->eventPostBeginPlay();

				if(GLevel->Actors(i))
					GLevel->Actors(i)->PostBeginPlay();
			}

		// Post net begin play.
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) && !GLevel->Actors(i)->bScriptInitialized )
				GLevel->Actors(i)->eventPostNetBeginPlay();

		// Begin scripting.
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
			if( GLevel->Actors(i) && !GLevel->Actors(i)->bScriptInitialized )
				GLevel->Actors(i)->eventSetInitialState();

		// Find bases
		for( INT i=0; i<GLevel->Actors.Num(); i++ )
		{
			if( GLevel->Actors(i) ) 
			{
				if ( GLevel->Actors(i)->AttachTag != NAME_None )
				{
					//find actor to attach self onto
					for( INT j=0; j<GLevel->Actors.Num(); j++ )
					{
						if( GLevel->Actors(j) 
							&& ((GLevel->Actors(j)->Tag == GLevel->Actors(i)->AttachTag) || (GLevel->Actors(j)->GetFName() == GLevel->Actors(i)->AttachTag))  )
						{
							GLevel->Actors(i)->SetBase(GLevel->Actors(j), FVector(0,0,1), 0);
							break;
						}
					}
				}
				else if( GLevel->Actors(i)->bCollideWorld && GLevel->Actors(i)->bShouldBaseAtStartup
				 &&	((GLevel->Actors(i)->Physics == PHYS_None) || (GLevel->Actors(i)->Physics == PHYS_Rotating)) )
				{
					 GLevel->Actors(i)->FindBase();
				}
			}
		}

		for( INT i=0; i<GLevel->Actors.Num(); i++ ) 
		{
			if(GLevel->Actors(i))
			{
				if( GLevel->Actors(i)->IsA(AProjector::StaticClass())) // sjs - why is this needed?!!
				{
					GLevel->Actors(i)->PostEditChange();
				}

#ifdef WITH_KARMA
				AActor* actor = GLevel->Actors(i);

				if(actor->Physics != PHYS_Karma || !actor->KParams || !actor->KParams->IsA(UKarmaParams::StaticClass()))
					continue;

				UKarmaParams* kparams = Cast<UKarmaParams>(actor->KParams);

				// If running below HighDetailPhysics, turn off karma dynamics for actors with bHighDetailOnly set true.
				if(	GLevel->GetLevelInfo()->PhysicsDetailLevel < PDL_High && kparams->bHighDetailOnly )
					KTermActorDynamics(actor);

				// If dedicated server, turn off karma for actors with bHighDetailOnly or bClientsOnly
				if(	GLevel->GetLevelInfo()->NetMode == NM_DedicatedServer && (kparams->bHighDetailOnly || kparams->bClientOnly) )					
					KTermActorDynamics(actor);
#endif
			}
		}


		// Preprocess Index/Vertex buffers for skeletal actors currently in memory.
		// #DEBUG - currently disabled.
#if (0)
		for( TObjectIterator<USkeletalMesh> It; It; ++It )
		{
			USkeletalMesh* SkelMesh = *It;				
			debugf(TEXT("D3D-predigesting skeletal mesh: %s"),SkelMesh->GetFullName());
			SkelMesh->RenderPreProcess( Client->HardwareSkinning );
		}
#endif


		Info->bStartup = 0;
	}
	else GLevel->TimeSeconds = GLevel->GetLevelInfo()->TimeSeconds;
	unguard;

	// Rearrange actors: static first, then others.
	guard(Rearrange);
	TArray<AActor*> Actors;
	Actors.AddItem(GLevel->Actors(0));
	Actors.AddItem(GLevel->Actors(1));
	for( INT i=2; i<GLevel->Actors.Num(); i++ )
		if( GLevel->Actors(i) && GLevel->Actors(i)->bStatic && !GLevel->Actors(i)->bAlwaysRelevant )
			Actors.AddItem( GLevel->Actors(i) );
	GLevel->iFirstNetRelevantActor=Actors.Num();
	for( INT i=2; i<GLevel->Actors.Num(); i++ )
		if( GLevel->Actors(i) && GLevel->Actors(i)->bStatic && GLevel->Actors(i)->bAlwaysRelevant )
			Actors.AddItem( GLevel->Actors(i) );
	GLevel->iFirstDynamicActor=Actors.Num();
	for( INT i=2; i<GLevel->Actors.Num(); i++ )
		if( GLevel->Actors(i) && !GLevel->Actors(i)->bStatic )
			Actors.AddItem( GLevel->Actors(i) );
	GLevel->Actors.Empty();
	GLevel->Actors.Add( Actors.Num() );
	for( INT i=0; i<Actors.Num(); i++ )
		GLevel->Actors(i) = Actors(i);

	// create AntiPortal volume list
	GLevel->AntiPortals.Empty();
	for( INT i=0; i<GLevel->Actors.Num(); i++ )
	{
		AAntiPortalActor *A = Cast<AAntiPortalActor>(GLevel->Actors(i));
		if ( A )
			GLevel->AntiPortals.AddItem(A);
	}
	debugf(TEXT("%d ANTIPORTALS"),GLevel->AntiPortals.Num());

	unguard;

	// Cleanup profiling.
#if DO_GUARD_SLOW
	guard(CleanupProfiling);
	for( TObjectIterator<UFunction> It; It; ++It )
		It->Calls = It->Cycles=0;
	GTicks=1;
	unguard;
#endif

	// Client init.
	guard(ClientInit);
	if( Client )
	{
		// Match Viewports to actors.
		MatchViewportsToActors( Client, GLevel->IsServer() ? GLevel : GEntry, URL, TEXT("")); //MenuClassName ); // gam

		// Set up audio.
		if( Audio )
			Audio->SetViewport( Audio->GetViewport() );

		// Reset viewports.
		for( INT i=0; i<Client->Viewports.Num(); i++ )
		{
			UViewport* Viewport = Client->Viewports(i);
			Viewport->Input->ResetInput();
			if( Viewport->RenDev )
				Viewport->RenDev->Flush(Viewport);
		}
	}
	unguard;

	// Init detail.
	GLevel->DetailChange( (EDetailMode)Info->DetailMode );

	// Remember the URL.
	guard(RememberURL);
	LastURL = URL;
	// gam ---
	for( INT i=LastURL.Op.Num()-1; i>=0; i-- )
	{
		if( !appStrPrefix( *LastURL.Op(i), TEXT("Menu=") ) )
		{
			LastURL.Op.Remove( i );
			break;
		}
	}
    // --- gam
	unguard;

	// Remember DefaultPlayer options.
	if( GIsClient )
	{
		URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("Name" ), TEXT("User") );
		URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("Team" ), TEXT("User") );
        URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("Character" ), TEXT("User") ); // sjs
#ifdef _XBOX
        URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("VoiceMask" ), TEXT("User") ); // sjs
#endif
		URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("Class"), TEXT("User") );
		URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("Skin" ), TEXT("User") );
		URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("Face" ), TEXT("User") );
		URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("Voice" ), TEXT("User") );
		URL.SaveURLConfig( TEXT("DefaultPlayer"), TEXT("OverrideClass" ), TEXT("User") );
	}

    // amb --- Load Sounds
    if (GLevel)
        GLevel->LoadSounds();
    // --- amb

#ifdef WITH_KARMA
	// Pre-allocate pool of KarmaTriListData structs
	// Don't bother if bKNoInit is false
	ALevelInfo* lInfo = GLevel->GetLevelInfo();
	if(!lInfo->bKNoInit)
	{
		for(INT i=0; i<lInfo->MaxRagdolls; i++)
		{
			KarmaTriListData* list = (KarmaTriListData*)appMalloc(sizeof(KarmaTriListData), TEXT("RAGDOLL TRILIST"));
			KarmaTriListDataInit(list);
			GLevel->TriListPool.AddItem(list);
		}
	}
#endif

	// Successfully started local level.
	return GLevel;
	unguard;
}

/*-----------------------------------------------------------------------------
	Game Viewport functions.
-----------------------------------------------------------------------------*/

//
// Draw a global view.
//
void UGameEngine::Draw( UViewport* Viewport, UBOOL Blit, BYTE* HitData, INT* HitSize )
{
	guard(UGameEngine::Draw);

	// If not up and running yet, don't draw.
	if(!GIsRunning)
		return;

	clock(GStats.DWORDStats( GEngineStats.STATS_Frame_RenderCycles ));

	// Determine the camera actor, location and rotation.
	AActor*		CameraActor		= Viewport->Actor;
	FVector		CameraLocation	= CameraActor->Location;
	FRotator	CameraRotation	= CameraActor->Rotation;

	Viewport->Actor->eventPlayerCalcView(CameraActor,CameraLocation,CameraRotation);

	if(!CameraActor)
	{
		debugf(TEXT("Warning: NULL CameraActor returned from PlayerCalcView for %s"),Viewport->Actor->GetPathName());
		CameraActor = Viewport->Actor;
	}

	if(Viewport->Actor->XLevel != GLevel)
		return;

	// Render the level.
	UpdateConnectingMessage();

	UBOOL Ugly3dfxHack = Viewport->RenDev->Is3dfx && (appStricmp( Viewport->Actor->XLevel->GetPathName(), TEXT("ut2-intro.mylevel") ) == 0); 

	BYTE SavedAction = Viewport->Actor->Level->LevelAction;
	
	if( Viewport->RenDev->PrecacheOnFlip ) // && !Viewport->Actor->Level->bNeverPrecache )
		Viewport->Actor->Level->LevelAction = LEVACT_Precaching;

	// Present the last frame.
	if( !Ugly3dfxHack && Viewport->PendingFrame && (!(Viewport->RenDev->PrecacheOnFlip) )  )  // && (!Viewport->Actor->Level->bNeverPrecache) )
			Viewport->Present();

	// Precache now if desired.
	if( Viewport->RenDev->PrecacheOnFlip )//&& !Viewport->Actor->Level->bNeverPrecache )
	{
		debugf(TEXT("Precaching: %s"), Viewport->Actor->Level->GetPathName() );
/*
		// Display loading screen (gam -- moved after package map verification)
		guard(LoadingScreen);
		if( Client && Client->Viewports.Num() )
		{
			GLevel->GetLevelInfo()->LevelAction = LEVACT_Loading;
			GLevel->GetLevelInfo()->Pauser = NULL;

			// gam --- My dream is to have this not be an actor but perhaps a UObject that we don't destroy immediately:
			// the idea would be we're spawn in here and it'd get updates throughout the life of this function call. Then,
			// before we return, we'd destroy it.
			// Also, perhaps through some of that object registry wizardry we'd pick a random vignette each time instead
			// of going to TestVignette every time.

			AVignette* Vignette = NULL;
			UClass* VignetteClass = StaticLoadClass( AVignette::StaticClass(), NULL, TEXT("XInterface.TestVignette"), NULL, LOAD_NoFail, NULL );
			Vignette = CastChecked<AVignette>( GLevel->SpawnActor( VignetteClass ) );
			Vignette->MapName = GLevel->URL.Map;
			Vignette->eventInit();

			PaintProgress( Vignette, 1.0F );

			if( Vignette )
				GLevel->DestroyActor( Vignette );

			// --- gam

			if( Audio )
				Audio->SetViewport( Audio->GetViewport() );
			GLevel->GetLevelInfo()->LevelAction = LEVACT_None;
		}
		unguard;
*/
		Viewport->RenDev->PrecacheOnFlip = 0;

		// Request script to fill in dynamic stuff like player skins.
		Viewport->Actor->Level->eventFillPrecacheMaterialsArray();
		Viewport->Actor->Level->eventFillPrecacheStaticMeshesArray();

		Viewport->Precaching = 1;

		DOUBLE StartTime = appSeconds();

		if(Viewport->Lock(HitData,HitSize))
		{
			FPlayerSceneNode	SceneNode(Viewport,&Viewport->RenderTarget,CameraActor,CameraLocation,CameraRotation,Viewport->Actor->FovAngle);

			Viewport->LodSceneNode = &SceneNode;
			Viewport->RI->SetPrecacheMode( PRECACHE_VertexBuffers );
			if( !Ugly3dfxHack )	
				SceneNode.Render(Viewport->RI);

			// Precache "dynamic" static meshes (e.g. weapon effects).
			for( INT i=0; i<Viewport->Actor->Level->PrecacheStaticMeshes.Num(); i++ )
			{
				// Set vertex and index buffers.
				UStaticMesh*	StaticMesh = Viewport->Actor->Level->PrecacheStaticMeshes(i);
				if( !StaticMesh )
					continue;

				FVertexStream*	VertexStreams[9] = { &StaticMesh->VertexStream };
				INT				NumVertexStreams = 1;

				if( !StaticMesh->VertexStream.Vertices.Num() )
					continue;

				if( StaticMesh->UseVertexColor )
				{
					if( StaticMesh->ColorStream.Colors.Num() )
						VertexStreams[NumVertexStreams++] = &StaticMesh->ColorStream;
				}
				else
				{
					if( StaticMesh->AlphaStream.Colors.Num() )
						VertexStreams[NumVertexStreams++] = &StaticMesh->AlphaStream;
				}

				for(INT UVIndex=0; UVIndex<StaticMesh->UVStreams.Num(); UVIndex++ )
					VertexStreams[NumVertexStreams++] = &StaticMesh->UVStreams(UVIndex);

				Viewport->RI->SetVertexStreams(VS_FixedFunction,VertexStreams,NumVertexStreams);
				Viewport->RI->SetIndexBuffer(&StaticMesh->IndexBuffer,0);

				for( INT MatIndex=0; MatIndex<StaticMesh->Materials.Num(); MatIndex++ )		
					Viewport->Actor->Level->PrecacheMaterials.AddItem( StaticMesh->Materials(MatIndex).Material );
			}
			Viewport->Actor->Level->PrecacheStaticMeshes.Empty();

			Viewport->LodSceneNode = NULL;

			Viewport->Unlock();
		}

		debugf(TEXT("Finished precaching geometry in %5.3f seconds"), (FLOAT) (appSeconds() - StartTime));
		StartTime = appSeconds();

		if(Viewport->Lock(HitData,HitSize))
		{
			FPlayerSceneNode SceneNode(Viewport,&Viewport->RenderTarget,CameraActor,CameraLocation,CameraRotation,Viewport->Actor->FovAngle);

			Viewport->LodSceneNode = &SceneNode;
			Viewport->RI->SetPrecacheMode( PRECACHE_All );
			if( !Ugly3dfxHack )	
				SceneNode.Render(Viewport->RI);

			// Precache dynamic materials (e.g. player skins).
			for( INT i=0; i<Viewport->Actor->Level->PrecacheMaterials.Num(); i++ )
				Viewport->RI->SetMaterial( Viewport->Actor->Level->PrecacheMaterials(i) );
			Viewport->Actor->Level->PrecacheMaterials.Empty();

			Viewport->LodSceneNode = NULL;

			Viewport->Unlock();
		}

		debugf(TEXT("Finished precaching textures in %5.3f seconds"), (FLOAT) (appSeconds() - StartTime));
		Viewport->Precaching	= 0;
		Viewport->PendingFrame	= 0;
	}
	else if(Viewport->Lock(HitData,HitSize))
	{
		if( Viewport->Actor->UseFixedVisibility )
		{
			FMatrix& WorldToCamera = Viewport->Actor->RenderWorldToCamera;
			WorldToCamera = FTranslationMatrix(-CameraLocation);

			if(!Viewport->IsOrtho())
				WorldToCamera = WorldToCamera * FInverseRotationMatrix(CameraRotation);

			if(Viewport->Actor->RendMap == REN_OrthXY)
				WorldToCamera = WorldToCamera * FMatrix(
											FPlane(Viewport->ScaleX,	0,					0,					0),
											FPlane(0,					-Viewport->ScaleY,	0,					0),
											FPlane(0,					0,					-1,					0),
											FPlane(0,					0,					-CameraLocation.Z,	1));
			else if(Viewport->Actor->RendMap == REN_OrthXZ)
				WorldToCamera = WorldToCamera * FMatrix(
											FPlane(Viewport->ScaleX,	0,					0,					0),
											FPlane(0,					0,					-1,					0),
											FPlane(0,					Viewport->ScaleY,	0,					0),
											FPlane(0,					0,					-CameraLocation.Y,	1));
			else if(Viewport->Actor->RendMap == REN_OrthYZ)
				WorldToCamera = WorldToCamera * FMatrix(
											FPlane(0,					0,					1,					0),
											FPlane(Viewport->ScaleX,	0,					0,					0),
											FPlane(0,					Viewport->ScaleY,	0,					0),
											FPlane(0,					0,					CameraLocation.X,	1));
			else
				WorldToCamera = WorldToCamera * FMatrix(
											FPlane(0,					0,					1,	0),
											FPlane(Viewport->ScaleX,	0,					0,	0),
											FPlane(0,					Viewport->ScaleY,	0,	0),
											FPlane(0,					0,					0,	1));

			CameraLocation = Viewport->Actor->FixedLocation;
			CameraRotation = Viewport->Actor->FixedRotation;

			Viewport->RI->Clear();
		}

		FPlayerSceneNode	SceneNode(Viewport,&Viewport->RenderTarget,CameraActor,CameraLocation,CameraRotation,Viewport->Actor->FovAngle);
	
		Viewport->LodSceneNode = &SceneNode;

		Viewport->RI->Clear(0,FColor(0,0,0),1,1.0f,1,~DEPTH_COMPLEXITY_MASK(Viewport));

		// Update level audio.
		if(Audio)
		{
			clock(GStats.DWORDStats(GEngineStats.STATS_Game_AudioTickCycles));
			Audio->Update(&SceneNode);
			unclock(GStats.DWORDStats(GEngineStats.STATS_Game_AudioTickCycles));
		}

		if( !Ugly3dfxHack )	
			SceneNode.Render(Viewport->RI);

		if ( Viewport->Canvas->pCanvasUtil ) // sjs
            Viewport->Canvas->pCanvasUtil->Flush();

		if(Audio) // sjs
		{
            Audio->Render(0);
            if ( Viewport->Canvas->pCanvasUtil ) // sjs
                Viewport->Canvas->pCanvasUtil->Flush();
        }

		Viewport->Precaching = 1;

		// Precache "dynamic" static meshes (e.g. weapon effects).
		for( INT i=0; i<Viewport->Actor->Level->PrecacheStaticMeshes.Num(); i++ )
		{
			// Set vertex and index buffers.
			UStaticMesh*	StaticMesh = Viewport->Actor->Level->PrecacheStaticMeshes(i);
			if( !StaticMesh )
				continue;

			FVertexStream*	VertexStreams[9] = { &StaticMesh->VertexStream };
			INT				NumVertexStreams = 1;

			if( !StaticMesh->VertexStream.Vertices.Num() )
				continue;

			if( StaticMesh->UseVertexColor )
			{
				if( StaticMesh->ColorStream.Colors.Num() )
					VertexStreams[NumVertexStreams++] = &StaticMesh->ColorStream;
			}
			else
			{
				if( StaticMesh->AlphaStream.Colors.Num() )
					VertexStreams[NumVertexStreams++] = &StaticMesh->AlphaStream;
			}

			for(INT UVIndex = 0;UVIndex < StaticMesh->UVStreams.Num();UVIndex++)
				VertexStreams[NumVertexStreams++] = &StaticMesh->UVStreams(UVIndex);

			Viewport->RI->SetVertexStreams(VS_FixedFunction,VertexStreams,NumVertexStreams);
			Viewport->RI->SetIndexBuffer(&StaticMesh->IndexBuffer,0);

			for( INT MatIndex=0; MatIndex<StaticMesh->Materials.Num(); MatIndex++ )
				Viewport->Actor->Level->PrecacheMaterials.AddItem( StaticMesh->Materials(MatIndex).Material );
		}
		Viewport->Actor->Level->PrecacheStaticMeshes.Empty();

		// Precache dynamic materials (e.g. player skins).
		for( INT i=0; i<Viewport->Actor->Level->PrecacheMaterials.Num(); i++ )
			Viewport->RI->SetMaterial( Viewport->Actor->Level->PrecacheMaterials(i) );
		Viewport->Actor->Level->PrecacheMaterials.Empty();		

		Viewport->Precaching = 0;

		Viewport->LodSceneNode = NULL;
		Viewport->Unlock();
		Viewport->PendingFrame = Blit ? 1 : 0;
	}

	Viewport->Actor->Level->LevelAction = SavedAction;

	unclock(GStats.DWORDStats( GEngineStats.STATS_Frame_RenderCycles ));

    if( QueueScreenShot ) // sjs - label hack
    {
        FMemMark Mark(GMem);
		FColor* Buf = new(GMem,Viewport->SizeX*Viewport->SizeY)FColor;
		Viewport->RenDev->ReadPixels( Viewport, Buf );
		appCreateBitmap( TEXT("Shot"), Viewport->SizeX, Viewport->SizeY, (DWORD*) Buf, GFileManager );
		Mark.Pop();
        QueueScreenShot = 0;
    }

	unguard;
}

static void ExportTravel( FOutputDevice& Out, AActor* Actor )
{
	guard(ExportTravel);
	debugf( TEXT("Exporting travelling actor of class %s"), Actor->GetClass()->GetPathName() );//!!xyzzy
	check(Actor);
	if( !Actor->bTravel )
		return;
	Out.Logf( TEXT("Class=%s Name=%s\r\n{\r\n"), Actor->GetClass()->GetPathName(), Actor->GetName() );
	for( TFieldFlagIterator<UProperty,CLASS_IsAUProperty> It(Actor->GetClass()); It; ++It )
	{
		for( INT Index=0; Index<It->ArrayDim; Index++ )
		{
			TCHAR Value[1024];
			if
			(	(It->PropertyFlags & CPF_Travel)
			&&	It->ExportText( Index, Value, (BYTE*)Actor, &Actor->GetClass()->Defaults(0), 0 ) )
			{
                Out.Log( It->GetName() );
				if( It->ArrayDim!=1 )
					Out.Logf( TEXT("[%i]"), Index );
				Out.Log( TEXT("=") );
				UObjectProperty* Ref = Cast<UObjectProperty>( *It );
				if( Ref && Ref->PropertyClass->IsChildOf(AActor::StaticClass()) )
				{
					UObject* Obj = *(UObject**)( (BYTE*)Actor + It->Offset + Index*It->ElementSize );
					Out.Logf( TEXT("%s\r\n"), Obj ? Obj->GetName() : TEXT("None") );
				}
				Out.Logf( TEXT("%s\r\n"), Value );
			}
		}
	}
	Out.Logf( TEXT("}\r\n") );
	unguard;
}

//
// Jumping viewport.
//
void UGameEngine::SetClientTravel( UPlayer* Player, const TCHAR* NextURL, UBOOL bItems, ETravelType TravelType )
{
	guard(UGameEngine::SetClientTravel);
	check(Player);

    // sjs --- E3
#ifdef _XBOX 
    if( Client ) // only do this if we're at entry right now
		Client->Exec(TEXT("E3-GETPLAYER"), *GLog);
#endif
    // --- sjs

	UViewport* Viewport    = CastChecked<UViewport>( Player );
	Viewport->TravelURL    = NextURL;
	Viewport->TravelType   = TravelType;
	Viewport->bTravelItems = bItems;

	unguard;
}

/*-----------------------------------------------------------------------------
	Tick.
-----------------------------------------------------------------------------*/

//
// Get tick rate limitor.
//
FLOAT UGameEngine::GetMaxTickRate()
{
	guard(UGameEngine::GetMaxTickRate);
	static UBOOL LanPlay = ParseParam(appCmdLine(),TEXT("lanplay"));
	if( GLevel && GLevel->NetDriver && !GIsClient )
		return Clamp( LanPlay ? GLevel->NetDriver->LanServerMaxTickRate : GLevel->NetDriver->NetServerMaxTickRate, 10, 120 );
	else if( GLevel && GLevel->DemoRecDriver && !GLevel->DemoRecDriver->ServerConnection )
		return Clamp( LanPlay ? GLevel->NetDriver->LanServerMaxTickRate : GLevel->DemoRecDriver->NetServerMaxTickRate, 10, 120 );
	else
		return 0;
	unguard;
}

//
// @@Cheat Protection - Check a given GUID/MD5 against the PackageValidation database and return if it's ok
//

UBOOL UGameEngine::ValidatePackage(const TCHAR* GUID, const TCHAR* MD5)
{
	guard(UGameEngine::ValidatePackage);

	UBOOL ValidPackage = false;
	UPackageCheckInfo *Info;
	FGuid Tester;
	for (INT i=0;i<PackageValidation.Num();i++)
	{
		Info = PackageValidation(i);

		Tester.A = Info->PackageID.C;
		Tester.B = Info->PackageID.D;
		Tester.C = Info->PackageID.B;
		Tester.D = Info->PackageID.A;


//		if ( !appStrcmp(GUID,Info->PackageID.String()) )
		if ( !appStrcmp(GUID,Tester.String()) )
		{
			for (INT j=0;j<Info->AllowedIDs.Num();j++)
			{
				if ( !appStrcmp(MD5,*Info->AllowedIDs(j)) )
				{
					ValidPackage = true;
					break;
				}
			}
			break;
		}
	}
	
	return ValidPackage;

	unguard;
}

//
// @@Cheat Protection - check all loaded packages against the package map in GPendingLevel.  If they are not allowed, try to clean
// them up
//

UBOOL UGameEngine::CheckForRogues()
{

	guard(UGameEngine::CheckForRogues);

	// Check to see if any "Non-Approved" packages are laying around

	TArray<UObject*> ObjLoaders = UObject::GetLoaderList(); 

	UPackageMap* ServerPackageMap = NULL;

	if( GLevel )
		ServerPackageMap = GLevel->NetDriver->ServerConnection->PackageMap;

	UBOOL bNeedsGC=false;	// By default, we don't need collection
	UBOOL bRemovePackage;
	for( INT i=0; i<ObjLoaders.Num(); i++ )
	{

		ULinker* Linker = CastChecked<ULinker>( ObjLoaders(i) );

		if ( Linker->LinksToCode() )
		{
			bRemovePackage = true;

			for( TArray<FPackageInfo>::TIterator It(ServerPackageMap->List); It; ++It )
			{
				if (Linker->Summary.Guid == It->Guid)
				{
					bRemovePackage = false;
					break;
				}
			}
		}
		else
			bRemovePackage=false;

		if (bRemovePackage)
		{
			debugf(TEXT("There is a need to remove %s"),Linker->LinkerRoot->GetName());
			bNeedsGC = true;
		}
	}

	return bNeedsGC;
	unguard;

}

// 
// @@Cheat Protection - Grab all of the client's MD5's and add them to the list.
//

void UGameEngine::AuthorizeClient(ULevel* Level)
{

	guard(UGameEngine::AuthorizeClient);

	// Check to see if any "Non-Approved" packages are laying around

	TArray<UObject*> ObjLoaders = UObject::GetLoaderList(); 

	
	FGuid Tester;
	INT Cnt = 0;

	for( INT i=0; i<ObjLoaders.Num(); i++ )
	{
		ULinker* Linker = CastChecked<ULinker>( ObjLoaders(i) );
		if ( Linker->LinksToCode() )
			Cnt++;
	}

	Level->NetDriver->ServerConnection->Logf( TEXT("CRITOBJCNT %i"),Cnt);

	for( INT i=0; i<ObjLoaders.Num(); i++ )
	{
		ULinker* Linker = CastChecked<ULinker>( ObjLoaders(i) );
		if ( Linker->LinksToCode() )
		{
			Tester.A = Linker->Summary.Guid.C;
			Tester.B = Linker->Summary.Guid.D;
			Tester.C = Linker->Summary.Guid.B;
			Tester.D = Linker->Summary.Guid.A;

			Level->NetDriver->ServerConnection->Logf( TEXT("GAMESTATE %s%s"),Tester.String(),*Linker->QuickMD5());
		}
	}
	unguard;
}

//
// Update everything.
//
void UGameEngine::Tick( FLOAT DeltaSeconds )
{
	guard(UGameEngine::Tick);
	INT LocalTickCycles=0;
	clock(LocalTickCycles);

    if( DeltaSeconds < 0.0f ) // sjs temp
        appErrorf(TEXT("Negative delta time!"));

	// If all viewports closed, time to exit.
	if( Client && Client->Viewports.Num()==0 )
	{
		debugf( TEXT("All Windows Closed") );
		appRequestExit( 0 );
		return;
	}

	// If game is paused, release the cursor.
	static UBOOL WasPaused=1;
	if
	(	Client
	&&	Client->Viewports.Num()==1
	&&	GLevel
	&&	!Client->Viewports(0)->IsFullscreen() )
	{
		UBOOL IsPaused
		=	GLevel->IsPaused()
		||	Client->Viewports(0)->bShowWindowsMouse;
		if( IsPaused && !WasPaused )
			Client->Viewports(0)->SetMouseCapture( 0, 0, 0 );
		else if( WasPaused && !IsPaused && Client->CaptureMouse )
			Client->Viewports(0)->SetMouseCapture( 1, 1, 1 );
		WasPaused = IsPaused;
	}
	else WasPaused=0;

	// Update subsystems.
	UObject::StaticTick();				
	GCache.Tick();

	// Update the level.
	guard(TickLevel);
	GameCycles=0;
	clock(GameCycles);
	if( GLevel )
	{
		// Decide whether to drop high detail because of frame rate
		if ( Client )
		{
			GLevel->GetLevelInfo()->bDropDetail = (DeltaSeconds > 1.f/Clamp(Client->MinDesiredFrameRate,1.f,100.f)) && !GIsBenchmarking;
			GLevel->GetLevelInfo()->bAggressiveLOD = (DeltaSeconds > 1.f/Clamp(Client->MinDesiredFrameRate - 5.f,1.f,100.f)) && !GIsBenchmarking;
		}
		// tick the level
		GLevel->Tick( LEVELTICK_All, DeltaSeconds );
	}
	//if( GEntry && GEntry!=GLevel )
	//	GEntry->Tick( LEVELTICK_All, DeltaSeconds );
	if( Client && Client->Viewports.Num() && Client->Viewports(0)->Actor->GetLevel()!=GLevel )
		Client->Viewports(0)->Actor->GetLevel()->Tick( LEVELTICK_All, DeltaSeconds );
	unclock(GameCycles);
	unguard;

	// Handle server travelling.
	guard(ServerTravel);
	if( GLevel && GLevel->GetLevelInfo()->NextURL!=TEXT("") )
	{
		if( (GLevel->GetLevelInfo()->NextSwitchCountdown-=DeltaSeconds) <= 0.f )
		{
			// Travel to new level, and exit.
			TMap<FString,FString> TravelInfo;
			if( GLevel->GetLevelInfo()->NextURL==TEXT("?RESTART") )
			{
				TravelInfo = GLevel->TravelInfo;
			}
			else if( GLevel->GetLevelInfo()->bNextItems )
			{
				TravelInfo = GLevel->TravelInfo;
				for( INT i=0; i<GLevel->Actors.Num(); i++ )
				{
					APlayerController* P = Cast<APlayerController>( GLevel->Actors(i) );
					if( P && P->Player && P->Pawn )
					{
						// Export items and self.
						FStringOutputDevice PlayerTravelInfo;
						ExportTravel( PlayerTravelInfo, P->Pawn );
						for( AActor* Inv=P->Pawn->Inventory; Inv; Inv=Inv->Inventory )
							ExportTravel( PlayerTravelInfo, Inv );
						TravelInfo.Set( *P->PlayerReplicationInfo->PlayerName, *PlayerTravelInfo );

						// Prevent local ClientTravel from taking place, since it will happen automatically.
						if( Cast<UViewport>( P->Player ) )
							Cast<UViewport>( P->Player )->TravelURL = TEXT("");
					}
				}
			}
			debugf( TEXT("Server switch level: %s"), *GLevel->GetLevelInfo()->NextURL );
			FString Error;
            // amb ---
            FString nextURL = GLevel->GetLevelInfo()->NextURL;
			GLevel->GetLevelInfo()->NextURL = TEXT("");
            // --- amb
			Browse( FURL(&LastURL,*nextURL,TRAVEL_Relative), &TravelInfo, Error );
			return;
		}
	}
	unguard;

	// Handle client travelling.
	guard(ClientTravel);
	if( Client && Client->Viewports.Num() && Client->Viewports(0)->TravelURL!=TEXT("") )
	{
		// Travel to new level, and exit.
		UViewport* Viewport = Client->Viewports( 0 );
		TMap<FString,FString> TravelInfo;

        if( GLevel && GLevel->GetLevelInfo() && GLevel->GetLevelInfo()->NetMode != NM_Client )
            GLevel->GetLevelInfo()->Game->eventGameEnding(); // gam

		// Export items.
		if( appStricmp(*Viewport->TravelURL,TEXT("?RESTART"))==0 )
		{
			TravelInfo = GLevel->TravelInfo;
		}
		else if( Viewport->bTravelItems )
		{
			debugf( TEXT("Export travel for: %s"), *Viewport->Actor->PlayerReplicationInfo->PlayerName );
			FStringOutputDevice PlayerTravelInfo;
			ExportTravel( PlayerTravelInfo, Viewport->Actor->Pawn );
			for( AActor* Inv=Viewport->Actor->Pawn->Inventory; Inv; Inv=Inv->Inventory )
				ExportTravel( PlayerTravelInfo, Inv );
			TravelInfo.Set( *Viewport->Actor->PlayerReplicationInfo->PlayerName, *PlayerTravelInfo );
		}
		FString Error;
		Browse( FURL(&LastURL,*Viewport->TravelURL,Viewport->TravelType), &TravelInfo, Error );
		Viewport->TravelURL=TEXT("");

		return;
	}
	unguard;

	// Update the pending level.
	guard(TickPending);
	if( GPendingLevel )
	{
		GPendingLevel->Tick( DeltaSeconds );
		if( GPendingLevel->Error!=TEXT("") )
		{
			// Pending connect failed.
			guard(PendingFailed);
			SetProgress( *FString::Printf(TEXT("menu:%s"),*DisconnectMenuClass), LocalizeError(TEXT("ConnectionFailed"),TEXT("Engine")), *GPendingLevel->Error );
			debugf( NAME_Log, LocalizeError(TEXT("Pending"),TEXT("Engine")), *GPendingLevel->URL.String(), *GPendingLevel->Error );
			delete GPendingLevel;
			GPendingLevel = NULL;
			unguard;
		}
		else if( GPendingLevel->Success && !GPendingLevel->FilesNeeded && !GPendingLevel->SentJoin )
		{
			// Attempt to load the map.
			FString Error;
			guard(AttemptLoadPending);
			LoadMap( GPendingLevel->URL, GPendingLevel, NULL, Error );
			if( Error!=TEXT("") )
			{
				SetProgress( *FString::Printf(TEXT("menu:%s"),*DisconnectMenuClass), LocalizeError(TEXT("ConnectionFailed"),TEXT("Engine")), *Error );
			}
			else if( !GPendingLevel->LonePlayer )
			{

				//@@Cheat Protection

				// Once the new map is loaded, check for anything loaded in memory that shouldn't be here.  If something
				// is loaded, remove it and perform garbage collection.
	
				// Then check again.  If something refused to get removed then consider it a hostile client.
/*
				if ( CheckForRogues() )
				{
					CollectGarbage(RF_Native | RF_Standalone);				
					if ( CheckForRogues() )
					{
						debugf(TEXT("Secondary Rogue Check Failure"));
						// gam --- TEMP DISABLE SetProgress( LocalizeError(TEXT("ConnectionFailed"),TEXT("Engine")), *Error );
					}						
				}

				// Authorize this client by sending a list of all loaded packages and their GUIDs back to the
				// server.
*/
				AuthorizeClient(GLevel);


				// Show connecting message, cause precaching to occur.
				GLevel->GetLevelInfo()->LevelAction = LEVACT_Connecting;
				GEntry->GetLevelInfo()->LevelAction = LEVACT_Connecting;
				if( Client )
					Client->Tick();

				// Send join.
				GPendingLevel->SendJoin();
				GPendingLevel->NetDriver = NULL;
				GPendingLevel->DemoRecDriver = NULL;

			}

			unguard;

			// Kill the pending level.
			guard(KillPending);
			delete GPendingLevel;
			GPendingLevel = NULL;
			unguard;
		}
	}
	unguard;

	// Render everything.
	guard(ClientTick);
	INT LocalClientCycles=0;
	if( Client )
	{
		clock(LocalClientCycles);
		Client->Tick();
		unclock(LocalClientCycles);
	}
	ClientCycles=LocalClientCycles;
	unguard;

	unclock(LocalTickCycles);
	TickCycles=LocalTickCycles;
	GTicks++;
	unguard;
}

/*-----------------------------------------------------------------------------
	Saving the game.
-----------------------------------------------------------------------------*/

//
// Save the current game state to a file.
//
void UGameEngine::SaveGame( INT Position )
{
	guard(UGameEngine::SaveGame);

	TCHAR Filename[256];
	GFileManager->MakeDirectory( *GSys->SavePath, 0 );
	appSprintf( Filename, TEXT("%s") PATH_SEPARATOR TEXT("Save%i.usa"), *GSys->SavePath, Position );
	GLevel->GetLevelInfo()->LevelAction=LEVACT_Saving;

    // gam --- My dream is to have this not be an actor but perhaps a UObject that we don't destroy immediately:
    // the idea would be we're spawn in here and it'd get updates throughout the life of this function call. Then,
    // before we return, we'd destroy it.
    // Also, perhaps through some of that object registry wizardry we'd pick a random vignette each time instead
    // of going to TestVignette every time.

    AVignette* Vignette = NULL;
    UClass* VignetteClass = StaticLoadClass( AVignette::StaticClass(), NULL, TEXT("XInterface.TestVignette"), NULL, LOAD_NoFail, NULL );
    Vignette = CastChecked<AVignette>( GLevel->SpawnActor( VignetteClass ) );
    Vignette->eventInit();
    PaintProgress( Vignette, 1.0F );
    GLevel->DestroyActor( Vignette );

    // --- gam

	GWarn->BeginSlowTask( LocalizeProgress(TEXT("Saving"),TEXT("Engine")), 1);
	GLevel->CleanupDestroyed( 1 );
	if( SavePackage( GLevel->GetOuter(), GLevel, 0, Filename, GLog ) )
	{
		// Copy the hub stack.
		INT i;
		for( i=0; i<GLevel->GetLevelInfo()->HubStackLevel; i++ )
		{
			TCHAR Src[256], Dest[256];
			appSprintf( Src, TEXT("%s") PATH_SEPARATOR TEXT("Game%i.usa"), *GSys->SavePath, i );
			appSprintf( Dest, TEXT("%s") PATH_SEPARATOR TEXT("Save%i%i.usa"), *GSys->SavePath, Position, i );
			GFileManager->Copy( Src, Dest );
		}
		while( 1 )
		{
			appSprintf( Filename, TEXT("%s") PATH_SEPARATOR TEXT("Save%i%i.usa"), *GSys->SavePath, Position, i++ );
			if( GFileManager->FileSize(Filename)<=0 )
				break;
			GFileManager->Delete( Filename );
		}
	}
	for( INT i=0; i<GLevel->Actors.Num(); i++ )
		if( Cast<AMover>(GLevel->Actors(i)) )
			Cast<AMover>(GLevel->Actors(i))->SavedPos = FVector(-1,-1,-1);
	GWarn->EndSlowTask();
	GLevel->GetLevelInfo()->LevelAction=LEVACT_None;
	GCache.Flush();

	unguard;
}

/*-----------------------------------------------------------------------------
	Mouse feedback.
-----------------------------------------------------------------------------*/

void UGameEngine::MouseWheel( UViewport* Viewport, DWORD Buttons, INT Delta )
{
}

//
// Mouse delta while dragging.
//
void UGameEngine::MouseDelta( UViewport* Viewport, DWORD ClickFlags, FLOAT DX, FLOAT DY )
{
	guard(UGameEngine::MouseDelta);
	if
	(	(ClickFlags & MOUSE_FirstHit)
	&&	Client
	&&	Client->Viewports.Num()==1
	&&	GLevel
	&&	!Client->Viewports(0)->IsFullscreen()
	&&	!GLevel->IsPaused()
	&&  !Viewport->bShowWindowsMouse )
	{
		Viewport->SetMouseCapture( 1, 1, 1 );
	}
	else if( (ClickFlags & MOUSE_LastRelease) && !Client->CaptureMouse )
	{
		Viewport->SetMouseCapture( 0, 0, 0 );
	}
	unguard;
}

//
// Absolute mouse position.
//
void UGameEngine::MousePosition( UViewport* Viewport, DWORD ClickFlags, FLOAT X, FLOAT Y )
{
	guard(UGameEngine::MousePosition);

	if( Viewport )
	{
		Viewport->WindowsMouseX = X;
		Viewport->WindowsMouseY = Y;
	}

	unguard;
}

//
// Mouse clicking.
//
void UGameEngine::Click( UViewport* Viewport, DWORD ClickFlags, FLOAT X, FLOAT Y )
{
	guard(UGameEngine::Click);
	unguard;
}

void UGameEngine::UnClick( UViewport* Viewport, DWORD ClickFlags, INT MouseX, INT MouseY )
{
	guard(UGameEngine::UnClick);
	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

