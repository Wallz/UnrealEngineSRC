//=============================================================================
// Copyright 2001 Digital Extremes - All Rights Reserved.
// Confidential.
//=============================================================================

#include "Engine.h"
#include "XGame.h"

#define ENABLE_MUTATOR_SCANNING 1

#define MAX_NAME_SIZE (INT)(512)

#define DECO_TEXT_MAX_SIZE (INT)(16 * 1024)
#define DECO_TEXT_MAX_COLUMNS (INT)(128 - 1)
#define DECO_TEXT_EOL_MARKER '|'

static void ParseDecoText( UDecoText* DecoText, const TCHAR* Text, INT ColumnCount )
{
    guard( ParseDecoText );

    TCHAR Line[DECO_TEXT_MAX_COLUMNS + 1];

    const TCHAR* LineStart;
    const TCHAR* WordStart;
    const TCHAR* WordEnd;
    const TCHAR* PrevWordEnd;

    if( ColumnCount > DECO_TEXT_MAX_COLUMNS )
    {
        debugf( NAME_Error, TEXT("Couldn't parse DecoText into %d columns (only %d are supported)"), ColumnCount, DECO_TEXT_MAX_COLUMNS );
        ColumnCount = DECO_TEXT_MAX_COLUMNS;
    }
    else if( ColumnCount <= 0 )
    {
        debugf( NAME_Error, TEXT("Couldn't parse DecoText into %d columns"), ColumnCount );
        return;
    }

    LineStart = Text;
    WordStart = Text;
    WordEnd = Text;
    PrevWordEnd = Text;

    for(;;)
    {
        // Get the next word:

        while( appIsSpace( *WordEnd ) )
            WordEnd++;

        while( !appIsSpace( *WordEnd ) && ( *WordEnd != '\0' ) && ( *WordEnd != DECO_TEXT_EOL_MARKER ) )
            WordEnd++;

        if( (WordEnd - WordStart) > ColumnCount ) // The word itself is too long:
        {
            // Flush line so-far:
            if( (PrevWordEnd - LineStart) > 0 )
            {
                appStrncpy( Line, LineStart, PrevWordEnd - LineStart + 1 );
                Line[PrevWordEnd - LineStart] = '\0';
                new(DecoText->Rows)FString(Line);
            }

            // Flush the word:
            appStrncpy( Line, WordStart, WordEnd - WordStart + 1 );
            Line[WordEnd - WordStart] = '\0';
            new(DecoText->Rows)FString(Line);

            // Start a new line:

            while( appIsSpace( *WordEnd ) )
                WordEnd++;

            LineStart = WordEnd;
        }
        else if( ( WordEnd - LineStart ) > ColumnCount )
        {
            // Flush line so-far:
            if( (PrevWordEnd - LineStart) > 0 )
            {
                appStrncpy( Line, LineStart, PrevWordEnd - LineStart + 1 );
                Line[PrevWordEnd - LineStart] = '\0';
                new(DecoText->Rows)FString(Line);
            }

            WordEnd = PrevWordEnd;

            while( appIsSpace( *WordEnd ) )
                WordEnd++;

            // Start a new line:
            LineStart = WordEnd;
        }
        else if( ( *WordEnd == DECO_TEXT_EOL_MARKER ) || ( *WordEnd == '\0' ) ) 
        {
            // Flush line to word end:

            appStrncpy( Line, LineStart, WordEnd - LineStart + 1 );
            Line[WordEnd - LineStart] = '\0';
            new(DecoText->Rows)FString(Line);

            if( *WordEnd == '\0' )
                break;

            WordEnd++;

            // Start a new line:
            LineStart = WordEnd;
        }

        PrevWordEnd = WordEnd;
        WordStart = WordEnd;
        WordEnd = WordEnd;
    }

    unguard;
}

UDecoText* UxUtil::LoadDecoText( const TCHAR* TextName, INT ColumnCount )
{
    guard( UxUtil::LoadDecoText );

    TCHAR PackageName[MAX_NAME_SIZE];
    TCHAR KeyName[MAX_NAME_SIZE];

    TCHAR FileNames[2][MAX_NAME_SIZE];

    TCHAR Buffer[DECO_TEXT_MAX_SIZE];
    const TCHAR *BufferP;

    const TCHAR* p;
    TCHAR* q;
    INT i;

    UDecoText* DecoText = NULL;
	
	debugf(TEXT("LoadDecoText::TextName = [%s]"),TextName);

    if
    (
        appStrPrefix( TextName, TEXT("XMaps.") ) &&
        appStrPrefix( TextName, TEXT("XGame.") ) &&
        appStrPrefix( TextName, TEXT("XPlayers.") )
    )
    {
        BufferP = TextName;
    }
    else
    {
        // Parse out the package and key name from TextName

        p = TextName;
        q = PackageName;
        i = ARRAY_COUNT(PackageName) - 1;

        while( *p != '.' )
        {
            if( *p == '\0' )
                return( NULL );

            if( i <= 0 )
                return( NULL );

            *q = *p;

            p++;
            q++;
            i--;
        }

        *q = '\0';
        p++;

        appStrncpy( KeyName, p, ARRAY_COUNT(KeyName) );

        if( !appStrlen( KeyName ) )
            return( NULL );
        
        // Find the key:
		
        appSprintf( FileNames[0], TEXT("%s.%s"), PackageName, UObject::GetLanguage() );
        appSprintf( FileNames[1], TEXT("%s.%s"), PackageName, TEXT("int") );

		debugf(TEXT("LoadDecoText::All = [%s] [%s] [%s] [%s]"),PackageName, KeyName, FileNames[0], FileNames[1]);

        if( !GConfig->GetString( TEXT("DecoText"), KeyName, Buffer, ARRAY_COUNT( Buffer), FileNames[0] ) &&
            !GConfig->GetString( TEXT("DecoText"), KeyName, Buffer, ARRAY_COUNT( Buffer), FileNames[1] ) )
        {
            debugf( NAME_Error, TEXT("Couldn't load DecoText %s.%s"), PackageName, KeyName );
            return( NULL );
        }
        
        BufferP = Buffer;
    }
    
    DecoText = CastChecked<UDecoText>( UObject::StaticConstructObject( UDecoText::StaticClass(), UObject::GetTransientPackage(), NAME_None, RF_Public | RF_Transient ) );
    DecoText->TextName = TextName;

    ParseDecoText( DecoText, BufferP, ColumnCount );

    return( DecoText );

    unguard;
}

void UxUtil::GenerateMutatorList( const TCHAR* MutatorManifestFile )
{
    guard( UxUtil::GenerateMutatorList );

	if (CachedMutatorList)
	{
		debugf(TEXT("Filling Mutator list from Cache"));
		return;
	}

	debugf(TEXT("Scanning and building mutator list"));

//	CachedMutatorList = CacheGet<UCacheMutators>();
	CachedMutatorList = CastChecked<UCacheMutators>( UObject::StaticConstructObject( UCacheMutators::StaticClass(), UObject::GetTransientPackage(), NAME_None, RF_Public | RF_Transient ) );

    TArray<FRegistryObjectInfo> List;
    GetRegistryObjects( List, UClass::StaticClass(), AMutator::StaticClass(), 0 );

    for( INT i = 0; i < List.Num(); i++ )
    {
        FMutatorRecord Record;
        appMemzero( &Record, sizeof( Record ) );

        UClass* MutatorClass = Cast<UClass>( UObject::StaticLoadObject( UClass::StaticClass(), NULL, *List(i).Object, NULL, 0, NULL ) );

        if( !MutatorClass )
            continue;

        AMutator* Default = Cast<AMutator>( MutatorClass->GetDefaultObject() );

        if( !Default )
            continue;

        Record.ClassName = MutatorClass->GetPathName();
        Record.IconMaterialName = Default->IconMaterialName;
        Record.ConfigMenuClassName = Default->ConfigMenuClassName;
        Record.GroupName = Default->GroupName;
        Record.Description = Default->Description;
        Record.FriendlyName = Default->FriendlyName;

		CachedMutatorList->Records( CachedMutatorList->Records.AddZeroed() ) = Record;		
    }
    unguard;
}

static void CacheParseLine( UCachePlayers& Cache, const TCHAR* Key, const TCHAR* Value, const TCHAR* FileName )
{
    guard( CacheParseLine );

    if( appStricmp( Key, TEXT("Player") ) != 0 )
        return;

    FPlayerRecord Record;
    appMemzero( &Record, sizeof( Record ) );

    TCHAR tmp [MAX_NAME_SIZE];
    if( !Parse( Value, TEXT("DefaultName="), tmp, ARRAY_COUNT(tmp) ) )
        appStrcpy( tmp, TEXT("Player") );
    Record.DefaultName = tmp;

    if( Parse( Value, TEXT("Species="), tmp, ARRAY_COUNT(tmp) ) )
		Record.Species = UObject::StaticLoadClass( USpeciesType::StaticClass(), NULL, tmp, NULL, 0, NULL );

    Parse( Value, TEXT("Mesh="), Record.MeshName );
    Parse( Value, TEXT("BodySkin="), Record.BodySkinName );
    Parse( Value, TEXT("FaceSkin="), Record.FaceSkinName );
    Parse( Value, TEXT("Voice="), Record.VoiceClassName );
    Parse( Value, TEXT("Sex="), Record.Sex );

    UMaterial* mat = NULL;
    if( Parse( Value, TEXT("Portrait="), tmp, ARRAY_COUNT(tmp) ) )
        mat = Cast<UMaterial>( UObject::StaticLoadObject( UMaterial::StaticClass(), NULL, tmp, NULL, 0, NULL ) );
    if( !mat )
        mat = Cast<UMaterial>( UObject::StaticLoadObject( UMaterial::StaticClass(), NULL, TEXT("PlayerPictures.cDefault"), NULL, LOAD_NoWarn, NULL ) );
    Record.Portrait = mat;

    Parse( Value, TEXT("Accuracy="), Record.Accuracy );
    Parse( Value, TEXT("Aggressiveness="), Record.Aggressiveness );
    Parse( Value, TEXT("StrafingAbility="), Record.StrafingAbility );
    Parse( Value, TEXT("CombatStyle="), Record.CombatStyle );
    Parse( Value, TEXT("Tactics="), Record.Tactics );
    Parse( Value, TEXT("bJumpy="), Record.bJumpy );
    Parse( Value, TEXT("FavoriteWeapon="), Record.FavoriteWeapon );
    Parse( Value, TEXT("Menu="), Record.Menu );

    if( !Parse( Value, TEXT("Text="), tmp, ARRAY_COUNT(tmp) ) )
        appStrcpy( tmp, TEXT("XPlayers.Default") );
    Record.TextName = tmp;

    INT index = Cache.Records.AddZeroed();
    Record.RecordIndex = index;
    Cache.Records( index ) = Record;

    unguard;
}

template<class CacheClass> void CacheLoad( CacheClass& Cache )
{
    TCHAR Filename[1024];
    TArray<FString> Files;
    DOUBLE Time;

    guard(AMenuBase::CacheLoad);

    Time = appSeconds();

    Cache.Records.Empty();

    for( INT i = 0; i < GSys->Paths.Num(); i++ )
    {
        Files.Empty();

        appSprintf( Filename, TEXT("%s%s"), appBaseDir(), *GSys->Paths(i) );
        TCHAR* Tmp = appStrstr( Filename, TEXT("*.") );

        if( !Tmp )
            continue;

        appSprintf( Tmp, TEXT("*.upl") );

        Files = GFileManager->FindFiles(Filename, 1, 0 );

	    TCHAR *Buffer = 0;		// update buffersize based on full file-size; can be overkill since
								// [Public] section is equal or smaller than file
		INT buffersize = 0;
        for( INT j = 0; j < Files.Num(); j++ )
        {
			buffersize = GFileManager->FileSize(*(Files(j))) + 1;
			if(Buffer)
				Buffer = (TCHAR*)appRealloc(Buffer, sizeof(TCHAR) * buffersize, TEXT("CacheLoad Buffer"));
			else
				Buffer = (TCHAR*)appMalloc(sizeof(TCHAR) * buffersize, TEXT("CacheLoad Buffer"));

            Files(j) = Files(j).LeftChop(4);
            appSprintf( Tmp, TEXT("%s%s"), appBaseDir(), *Files(j) );
            TCHAR* End = Tmp + appStrlen( Tmp );
            appSprintf( End, TEXT(".upl") );

			// note, buffer is at least as large as buffersize * sizeof(TCHAR)
            UBOOL Success = GConfig->GetSection( TEXT("Public"), Buffer, buffersize, Tmp );
            if( !Success )
                continue;

            TCHAR* Next;
            for( TCHAR* Key = Buffer; *Key; Key=Next )
            {
                Next = Key + appStrlen(Key) + 1;
                TCHAR* Value = appStrstr(Key,TEXT("="));

                if( !Value )
                    continue;

                *Value++ = 0;
                if( *Value=='(' )
                    *Value++ = 0;
                if( *Value && Value[appStrlen(Value)-1]==')' )
                    Value[appStrlen(Value)-1] = 0;

                CacheParseLine( Cache, Key, Value, *Files(j) );
            }
        }
		if (Buffer) 
			appFree(Buffer);
    }
    
    Time = appSeconds() - Time;
    
    debugf( NAME_Log, TEXT("xUtil::CacheLoad %s (%f seconds)"), Cache.GetName(), Time );

    unguard;
};

template<class CacheClass> CacheClass* CacheGet()
{
    guard(AMenuBase::CacheGet);

    CacheClass* Cache = CastChecked<CacheClass>( UObject::StaticConstructObject( CacheClass::StaticClass(), UObject::GetTransientPackage(), NAME_None, RF_Public | RF_Transient ) );
    CacheLoad( *Cache );

    return( Cache );

    unguard;
}

void UxUtil::GetPlayerList()
{
    if (!CachedPlayerList)
        CachedPlayerList = CacheGet<UCachePlayers>();
}

void UxUtil::execGetPlayerList( FFrame& Stack, RESULT_DECL )
{
    guard(UxUtil::execGetPlayerList);

    P_GET_TARRAY_REF( PlayerRecords, FPlayerRecord );
    P_FINISH;

    GetPlayerList();
    *PlayerRecords = CachedPlayerList->Records;

    unguard;
}

void UxUtil::execGetPlayerRecord( FFrame& Stack, RESULT_DECL )
{
    guard(UxUtil::execGetPlayerRecord);

    P_GET_INT(prIdx);
    P_FINISH;

    GetPlayerList();

    if( CachedPlayerList->Records.Num()==0 )
    {
        FPlayerRecord empty;
        appMemzero( &empty, sizeof( empty ) );
        *(FPlayerRecord*)Result = empty;
        return;
    }

    *(FPlayerRecord*)Result = CachedPlayerList->Records(prIdx);

    unguard;
}

void UxUtil::execFindPlayerRecord( FFrame& Stack, RESULT_DECL )
{
    guard(UxUtil::execFindPlayerRecord);
    P_GET_STR(charName);
    P_FINISH;

    GetPlayerList();
    for (INT i=0; i<CachedPlayerList->Records.Num(); i++)
    {
        if (charName == CachedPlayerList->Records(i).DefaultName)
        {
            *(FPlayerRecord*)Result = CachedPlayerList->Records(i);
            return;
        }
    }
    
    FPlayerRecord empty;
    appMemzero( &empty, sizeof( empty ) );
    *(FPlayerRecord*)Result = empty;

    unguard;
}

void UxUtil::execGetMutatorList( FFrame& Stack, RESULT_DECL )
{
    guard(UxUtil::execGetMutatorList);

    P_GET_TARRAY_REF( MutatorRecords, FMutatorRecord );
    P_FINISH;

    GenerateMutatorList();
	*MutatorRecords = CachedMutatorList->Records;

    unguard;
}
