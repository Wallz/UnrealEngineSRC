/*=============================================================================
	UnEngine.cpp: Unreal engine main.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

#include "EnginePrivate.h"
#include "UnLinker.h"
/*-----------------------------------------------------------------------------
	Object class implementation.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(UEngine);
IMPLEMENT_CLASS(URenderDevice);

/*-----------------------------------------------------------------------------
	Engine init and exit.
-----------------------------------------------------------------------------*/

//
// Construct the engine.
//
UEngine::UEngine()
{
	guard(UEngine::UEngine);
	unguard;
}

//
// Init class.
//
void UEngine::StaticConstructor()
{
	guard(UEngine::StaticConstructor);

	new(GetClass(),TEXT("CacheSizeMegs"),      RF_Public)UIntProperty (CPP_PROPERTY(CacheSizeMegs      ), TEXT("Settings"), CPF_Config );

	unguard;
}

// Register things.
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name) ENGINE_API FName ENGINE_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name) IMPLEMENT_FUNCTION(cls,idx,name)
#include "EngineClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NAMES_ONLY

// sjs --- import natives
#define NATIVES_ONLY
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name)
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#include "EngineClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NATIVES_ONLY
#undef NAMES_ONLY
// --- sjs

//
// Init audio.
//
void UEngine::InitAudio()
{
	guard(UEngine::InitAudio);
	if
	(	UseSound
	&&	GIsClient
	&&	!ParseParam(appCmdLine(),TEXT("NOSOUND")) )
	{
		UClass* AudioClass = StaticLoadClass( UAudioSubsystem::StaticClass(), NULL, TEXT("ini:Engine.Engine.AudioDevice"), NULL, LOAD_NoFail, NULL );
		Audio = ConstructObject<UAudioSubsystem>( AudioClass );
		if( !Audio->Init() )
		{
			debugf( NAME_Error, TEXT("Audio initialization failed.") ); // gam
			delete Audio;
			Audio = NULL;
		}
	}
	unguard;
}

//
// Initialize the engine.
//
void UEngine::Init()
{
	guard(UEngine::Init);

	// Add the intrinsic names.
	#define NAMES_ONLY
	#define AUTOGENERATE_NAME(name) ENGINE_##name = FName(TEXT(#name),FNAME_Intrinsic);
	#define AUTOGENERATE_FUNCTION(cls,idx,name)
	#include "EngineClasses.h"
	#undef AUTOGENERATE_FUNCTION
	#undef AUTOGENERATE_NAME
	#undef NAMES_ONLY

	// Subsystems.
	FURL::StaticInit();
	GEngineMem.Init( 65536 );
#ifdef _XBOX
	GCache.Init( 1024 * 1024 * 1, 4096 );
#else
	GCache.Init( 1024 * 1024 * CacheSizeMegs, 4096 );
#endif
	GEngineStats.Init();

	// Initialize random number generator.
	if( GIsBenchmarking )
		appRandInit( 0 );
	else
		appRandInit( appCycles() );
	
	if(!GStatGraph)
		GStatGraph = new FStatGraph();

	if(!GTempLineBatcher)
		GTempLineBatcher = new FTempLineBatcher();

	// Objects.
	Cylinder = new UPrimitive;

	// Add to root.
	AddToRoot();

	// Create GGlobalTempObjects
	GGlobalTempObjects = new UGlobalTempObjects;

	// Ensure all native classes are loaded.
	for( TObjectIterator<UClass> It ; It ; ++It )
		if( !It->GetLinker() )
			LoadObject<UClass>( It->GetOuter(), It->GetName(), NULL, LOAD_Quiet|LOAD_NoWarn, NULL );

	debugf( NAME_Init, TEXT("Unreal engine initialized") );
	unguard;
}

//
// Pre shutdown.
//
void UEngine::Exit()
{
	guard(UEngine::Exit);

	// Exit sound.
	guard(ExitSound);
	if( Audio )
	{
		delete Audio;
		Audio = NULL;
	}
	unguard;

	unguard;
}

//
// Exit the engine.
//
void UEngine::Destroy()
{
	guard(UEngine::Destroy);

	// Remove from root.
	RemoveFromRoot();

	// Shut down all subsystems.
	Audio	 = NULL;
	Client	 = NULL;
	FURL::StaticExit();
	GEngineMem.Exit();
	GCache.Exit( 1 );
	if(GStatGraph)
	{
		delete GStatGraph;
		GStatGraph = NULL;
	}
	
	if(GTempLineBatcher)
	{
		delete GTempLineBatcher;
		GTempLineBatcher = NULL;
	}
	
	Super::Destroy();
	unguard;
}

//
// Flush all caches.
//
void UEngine::Flush( UBOOL AllowPrecache )
{
	guard(UEngine::Flush);

	GCache.Flush();
	if( Client )
		Client->Flush( AllowPrecache );

	unguard;
}

//
// Update Gamma/Brightness/Contrast settings.
//
void UEngine::UpdateGamma()
{
	guard(UEngine::UpdateGamma);

	if( Client )
		Client->UpdateGamma();

	unguard;
}

//
// Restore Gamma/Brightness/Contrast settings.
//
void UEngine::RestoreGamma()
{
	guard(UEngine::RestoreGamma);

	if( Client )
		Client->RestoreGamma();

	unguard;
}

//
// Tick rate.
//
FLOAT UEngine::GetMaxTickRate()
{
	guard(UEngine::GetMaxTickRate);
	return ( GIsEditor ? 30 : 0);
	unguard;
}

//
// Progress indicator.
//
void UEngine::SetProgress( const TCHAR* CmdStr,  const TCHAR* Str1, const TCHAR* Str2, FLOAT Seconds )
{
	guard(UEngine::SetProgress);
	unguard;
}

//
// Serialize.
//
void UEngine::Serialize( FArchive& Ar )
{
	guard(UGameEngine::Serialize);

	Super::Serialize( Ar );
	Ar << Cylinder << Client << Audio << GRenDev;

	unguardobj;
}

/*-----------------------------------------------------------------------------
	Input.
-----------------------------------------------------------------------------*/

struct FTextureSizeSort
{
	UBitmapMaterial*	Texture;
	INT					Size;

	FTextureSizeSort(UBitmapMaterial* InTexture)
	{
		Texture = InTexture;
		Size = 0;

		FTexture*	RenderInterface = Texture->GetRenderInterface()->GetTextureInterface();

		for(INT MipIndex = RenderInterface->GetFirstMip();MipIndex < RenderInterface->GetNumMips();MipIndex++)
			Size += GetBytesPerPixel(RenderInterface->GetFormat(),(RenderInterface->GetWidth() >> MipIndex) * (RenderInterface->GetHeight() >> MipIndex));
	}

	friend INT Compare(FTextureSizeSort& A,FTextureSizeSort& B)
	{
		return A.Size - B.Size;
	}
};

//
// This always going to be the last exec handler in the chain. It
// handles passing the command to all other global handlers.
//
UBOOL UEngine::Exec( const TCHAR* Cmd, FOutputDevice& Ar )
{
	guard(UEngine::Exec);

	// See if any other subsystems claim the command.
	if( GSys    && GSys->Exec		(Cmd,Ar) ) return 1;
#ifdef _XBOX
	//!!vogel: use xbdbsmon to read output 
	if( UObject::StaticExec			(Cmd,*GLog) ) return 1;
#else
	if( UObject::StaticExec			(Cmd,Ar) ) return 1;
#endif
	if( GCache.Exec					(Cmd,Ar) ) return 1;
	if( GExec   && GExec->Exec      (Cmd,Ar) ) return 1;
	if( Client  && Client->Exec		(Cmd,Ar) ) return 1;
	if( Audio   && Audio->Exec		(Cmd,Ar) ) return 1;
	if( GStatGraph && GStatGraph->Exec(Cmd,Ar) ) return 1;

	// Handle engine command line.
	if( ParseCommand(&Cmd,TEXT("FLUSH")) )
	{
		Flush(1);
		Ar.Log( TEXT("Flushed engine caches") );
		return 1;
	}

	else if( ParseCommand(&Cmd,TEXT("CLOCK")) )
	{
		GIsClocking = !GIsClocking;
		return 1;
	}
	else if( ParseCommand(&Cmd,TEXT("STAT")) )
	{
		INT Result = 0;
		if( ParseCommand(&Cmd,TEXT("ANIM")) )
		{
			bShowAnimStats = !bShowAnimStats;
			Result = 1;
		} 
		else if( ParseCommand(&Cmd,TEXT("LIGHT")) )
		{
			bShowLightStats = !bShowLightStats;
			Result = 1;
		}
		else if( ParseCommand(&Cmd,TEXT("DEFAULT")) || ParseCommand(&Cmd,TEXT("RESET")))
		{
			bShowAnimStats		= 0;
			bShowRenderStats	= 0;
			bShowHardwareStats	= 0;
			bShowMatineeStats	= 0;
			bShowGameStats		= 0;
			bShowAudioStats		= 0;
			bShowNetStats		= 0;
			bShowHistograph		= 0;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("FPS")))
		{
			bShowFrameRate = !bShowFrameRate;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("RENDER")))
		{
			bShowRenderStats = !bShowRenderStats;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("HARDWARE")))
		{
			bShowHardwareStats = !bShowHardwareStats;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("GAME")))
		{
			bShowGameStats = !bShowGameStats;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("HISTOGRAPH")))
		{
			bShowHistograph = !bShowHistograph;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("XBOXMEM")))
		{
			bShowXboxMemStats = !bShowXboxMemStats;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("MATINEE")))
		{
			bShowMatineeStats = !bShowMatineeStats;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("AUDIO")))
		{
			bShowAudioStats = !bShowAudioStats;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("NET")))
		{
			bShowNetStats		= !bShowNetStats;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("ALL")))
		{
			//bShowAnimStats	= 1;
			bShowFrameRate		= 1;
			bShowRenderStats	= 1;
			bShowHardwareStats	= 1;
			bShowMatineeStats	= 1;
			bShowGameStats		= 1;
			bShowAudioStats		= 1;
			bShowNetStats		= 1;
			Result = 1;
		}
		else if(ParseCommand(&Cmd,TEXT("NONE")))
		{
			bShowAnimStats		= 0;
			bShowFrameRate		= 0;
			bShowRenderStats	= 0;
			bShowHardwareStats	= 0;
			bShowMatineeStats	= 0;
			bShowGameStats		= 0;
			bShowAudioStats		= 0;
			bShowNetStats		= 0;
			Result = 1;
		}

		if ( Result )
		{
			if ( bShowAnimStats || bShowRenderStats || bShowHardwareStats || bShowMatineeStats
				|| bShowGameStats || bShowAudioStats || bShowNetStats )
			{
				GIsClocking = 1;
			}
			else
				GIsClocking = 0;
			return 1;
		}
		else
			return 0;
	}
	else if(ParseCommand(&Cmd,TEXT("texstats")))
	{
		// Dump texture stats in video memory.

		TArray<FTextureSizeSort>	SizeSortedTextures;

		for(TObjectIterator<UBitmapMaterial> It;It;++It)
		{
			FTexture*	RenderInterface = It->GetRenderInterface() ? It->GetRenderInterface()->GetTextureInterface() : NULL;

			if(RenderInterface && GRenDev->ResourceCached(RenderInterface->GetCacheId()))
				SizeSortedTextures.AddItem(FTextureSizeSort(*It));
		}

		Sort(&SizeSortedTextures(0),SizeSortedTextures.Num());

		INT	TotalTextureSize = 0;

		for(INT TextureIndex = 0;TextureIndex < SizeSortedTextures.Num();TextureIndex++)
		{
			FTexture*	RenderInterface = SizeSortedTextures(TextureIndex).Texture->GetRenderInterface()->GetTextureInterface();

			Ar.Logf(
				TEXT("%u bytes\t%ux%u\t%u mips\t%u BPP: %s"),
				SizeSortedTextures(TextureIndex).Size,
				RenderInterface->GetWidth() >> RenderInterface->GetFirstMip(),
				RenderInterface->GetHeight() >> RenderInterface->GetFirstMip(),
				RenderInterface->GetNumMips() - RenderInterface->GetFirstMip(),
				GetBytesPerPixel(RenderInterface->GetFormat(),8),
				SizeSortedTextures(TextureIndex).Texture->GetFullName()
				);

			TotalTextureSize += SizeSortedTextures(TextureIndex).Size;
		}

		Ar.Logf(
			TEXT("Total texture size: %u bytes"),
			TotalTextureSize
			);

		return 1;
	}
    else if( ParseCommand(&Cmd,TEXT("SHIP")) )
    {
        GShowBuildLabel = !GShowBuildLabel;
        return 1;
    }
    // --- gam
	else if( ParseCommand(&Cmd,TEXT("CRACKURL")) )
	{
		FURL URL(NULL,Cmd,TRAVEL_Absolute);
		if( URL.Valid )
		{
			Ar.Logf( TEXT("     Protocol: %s"), *URL.Protocol );
			Ar.Logf( TEXT("         Host: %s"), *URL.Host );
			Ar.Logf( TEXT("         Port: %i"), URL.Port );
			Ar.Logf( TEXT("          Map: %s"), *URL.Map );
			Ar.Logf( TEXT("   NumOptions: %i"), URL.Op.Num() );
			for( INT i=0; i<URL.Op.Num(); i++ )
				Ar.Logf( TEXT("     Option %i: %s"), i, *URL.Op(i) );
			Ar.Logf( TEXT("       Portal: %s"), *URL.Portal );
			Ar.Logf( TEXT("       String: '%s'"), *URL.String() );
		}
		else Ar.Logf( TEXT("BAD URL") );
		return 1;
	}
	else if( ParseCommand(&Cmd,TEXT("RENDEREMULATE")) )
	{
		if( ParseCommand(&Cmd,TEXT("gf1")) )
			GRenDev->SetEmulationMode( HEM_GeForce1 );
		else
		if( ParseCommand(&Cmd,TEXT("gf2")) )
			GRenDev->SetEmulationMode( HEM_GeForce1 );
		else
		if( ParseCommand(&Cmd,TEXT("xbox")) )
			GRenDev->SetEmulationMode( HEM_XBox );
		else
			GRenDev->SetEmulationMode( HEM_None );
		return 1;
	}
	else return 0;
	unguard;
}

//
// Key handler.
//
UBOOL UEngine::Key( UViewport* Viewport, EInputKey Key, TCHAR Unicode )
{
	guard(UEngine::Key);

	// Allow the Interaction Master to process the event.  If it doesn't for right now
	// we continue down the orignal road until I get the console recoding completed :)
	if( !GIsRunning )
		return false;
	else if(Client->InteractionMaster)
		return Client->InteractionMaster->MasterProcessKeyType(Key,Unicode); 
	else
		return false;

	unguard;
}

//
// Input event handler.
//
UBOOL UEngine::InputEvent( UViewport* Viewport, EInputKey iKey, EInputAction State, FLOAT Delta )
{
	guard(UEngine::InputEvent);


	// Allow the Interaction Master to process the event.  If it doesn't for right now
	// we continue down the orignal road until I get the console recoding completed :)

    // sjs - merge_hack NOTE: I reverted this func... all firing/jumping aliased input was not working... why?

	if( !GIsRunning )
	{
		return 0;
	}
	else
	{
		if ( (Client->InteractionMaster) && ( Client->InteractionMaster->MasterProcessKeyEvent(iKey, State, Delta ) ) )
		{
			if (State==IST_Release)
			{
				Viewport->Input->PreProcess(iKey, State, Delta);
			}
			return 1;
		}
		else if ( Viewport->Input->PreProcess( iKey, State, Delta ) && Viewport->Input->Process( *GLog, iKey, State, Delta ) )
		{
			// Input system handled it.
			return 1;
		}
	}

	return 0;

	unguard;
}

INT UEngine::ChallengeResponse( INT Challenge )
{
	guard(UEngine::ChallengeResponse);
	return 0;
	unguard;
}

/*-----------------------------------------------------------------------------
	UServerCommandlet.
-----------------------------------------------------------------------------*/

void UServerCommandlet::StaticConstructor()
{
	guard(UServerCommandlet::StaticConstructor);

	LogToStdout = 1;
	IsClient    = 0;
	IsEditor    = 0;
	IsServer    = 1;
	LazyLoad    = 1;

	unguard;
}

#include <float.h> // sjs test!

INT UServerCommandlet::Main( const TCHAR* Parms )
{
	guard(UServerCommandlet::Main);

	// Language.
	TCHAR Temp[256];
	if( GConfig->GetString( TEXT("Engine.Engine"), TEXT("Language"), Temp, ARRAY_COUNT(Temp) ) )
	UObject::SetLanguage( Temp );

    appResetTimer(); // sjs

	// Create the editor class.
	UClass* EngineClass = UObject::StaticLoadClass( UEngine::StaticClass(), NULL, TEXT("ini:Engine.Engine.GameEngine"), NULL, LOAD_NoFail, NULL );
	UEngine* Engine = ConstructObject<UEngine>( EngineClass );
	Engine->Init();

	// Main loop.
	GIsRunning = 1;
	DOUBLE OldTime = appSeconds();
	DOUBLE SecondStartTime = OldTime;
	INT TickCount = 0;

	while( GIsRunning && !GIsRequestingExit )
	{
		// Clear stats (will also update old stats).
		GStats.Clear();

		// Update the world.
		guard(UpdateWorld);
		DOUBLE NewTime = appSeconds();
		Engine->Tick( NewTime - OldTime );
		// sjs --- engine::tick may load a new map and cause the timing to be reset (this is a good thing)
		if( appSeconds() < NewTime )
            SecondStartTime = NewTime = appSeconds();
		// --- sjs
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
		if( MaxTickRate>0.f )
		{
			FLOAT Delta = (1.f/MaxTickRate) - (appSeconds()-OldTime);
			appSleep( Max(0.f,Delta) );
		}
		unguard;
	}
	GIsRunning = 0;
	return 0;
	unguard;
}

IMPLEMENT_CLASS(UServerCommandlet)


/*-----------------------------------------------------------------------------
	UMasterMD5Commandlet.
-----------------------------------------------------------------------------*/

void UMasterMD5Commandlet::StaticConstructor()
{
	guard(UMasterMD5Commandlet::StaticConstructor);

	LogToStdout     = 1;
	IsClient        = 1;
	IsEditor        = 1;
	IsServer        = 1;
	LazyLoad        = 1;
	ShowErrorCount  = 1;

	unguard;
}

// Process a directory for requested files

INT UMasterMD5Commandlet::ProcessDirectory(FString Directory, const TCHAR* Parms)
{
	guard(UMasterMD5Commandlet::ProcessDirectory);

	FString Wildcard;		
	FString SearchPath;
	if( !ParseToken(Parms,Wildcard,0) )
		appErrorf(TEXT("Source file(s) not specified"));
	do
	{
		SearchPath.Empty();
		SearchPath = FString::Printf(TEXT("%s%s"),*Directory,*Wildcard);
		TArray<FString> FilesFound = GFileManager->FindFiles( *SearchPath, 1, 0 );
		for (INT i=0; i<FilesFound.Num(); i++)
		{
			FString Pkg = FilesFound(i);
			BeginLoad();
			ULinkerLoad* Linker = GetPackageLinker( NULL, *Pkg, LOAD_NoWarn | LOAD_Throw, NULL, NULL );
			if (Linker != NULL)
			{
				//PackageValidation

				INT Index=PackageValidation.Num();

				PackageValidation.AddItem(ConstructObject<UPackageCheckInfo>(UPackageCheckInfo::StaticClass(),OutputPackage,NAME_None,RF_Public));
				
				//PackageValidation.AddItem(new(OutputPackage,NAME_None,RF_Public)UPackageCheckInfo));
				PackageValidation(Index)->PackageID = Linker->Summary.Guid;

				debugf(TEXT("Saving %s %s"),Linker->Summary.Guid.String(),*Linker->QuickMD5());

				new(PackageValidation(Index)->AllowedIDs)FString(Linker->QuickMD5());
				PackageValidation(Index)->Native = true;
			}
		}

		//UObject::CollectGarbage(RF_Native);

	}
	while( ParseToken(Parms,Wildcard,0) );

	return 0;

	unguard;
}

INT UMasterMD5Commandlet::Main( const TCHAR* Parms )
{
	guard(UMasterMD5Commandlet::Main);

	OutputPackage = CreatePackage(NULL,TEXT("Packages.md5"));
	if (OutputPackage==NULL)
	{
		GWarn->Logf(TEXT("Could not create [Packages.md5]"));
		GIsRequestingExit=1;
		return 0;
	}
	
	PackageValidation.Empty();

	TArray<FString> DirsFound = GFileManager->FindFiles( TEXT("..\\*.*"), 0, 1 );
	for (INT i=0; i<DirsFound.Num(); i++)
	{
		FString ThisDir=FString::Printf(TEXT("..\\%s\\"),*DirsFound(i));
		ProcessDirectory(ThisDir,Parms);
	}

	GWarn->Logf( TEXT("=================================================="));
	GWarn->Logf( TEXT(" No of Packages in Array: %i"),PackageValidation.Num());
	GWarn->Logf( TEXT("=================================================="));

	FString Text=TEXT("");

	for (INT i=0; i< PackageValidation.Num(); i++)
	{
		UPackageCheckInfo* P = PackageValidation(i);
	
		Text += FString::Printf(TEXT("[%s]\n"),P->PackageID.String());
		Text += FString::Printf(TEXT("MD5=%s\n\n"),*P->AllowedIDs(0));
		
		GWarn->Logf(TEXT("  Package GUID: %s   MD5: %s"),P->PackageID.String(),*P->AllowedIDs(0));

	}

	appSaveStringToFile(Text,TEXT("Packages.txt"), GFileManager);

	SavePackage(OutputPackage,NULL,RF_Public,TEXT("Packages.md5"),GWarn,NULL);
	GIsRequestingExit=1;
	return 0;

	unguard;
}

IMPLEMENT_CLASS(UMasterMD5Commandlet);

/*-----------------------------------------------------------------------------
	UGlobalTempObjects.
-----------------------------------------------------------------------------*/

ENGINE_API UGlobalTempObjects* GGlobalTempObjects = NULL;
IMPLEMENT_CLASS(UGlobalTempObjects)

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

