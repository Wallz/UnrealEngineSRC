/*=============================================================================
	UnCDKey.cpp: CD Key validation
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Jack Porter
=============================================================================*/

/*-----------------------------------------------------------------------------
	Includes
-----------------------------------------------------------------------------*/

// This sucks, but I really don't want the code to read the CD key to be
// in a DLL export.  So we have to declare the registry stuff we need here.

#if _MSC_VER && !_XBOX
#include <windows.h>
#endif
#include "EnginePrivate.h"

/*-----------------------------------------------------------------------------
	Win32 Registry access
-----------------------------------------------------------------------------*/

#if _MSC_VER && !_XBOX
#define RegOpenKeyX(a,b,c)				TCHAR_CALL_OS(RegOpenKeyW(a,b,c),RegOpenKeyA(a,TCHAR_TO_ANSI(b),c))
static inline LONG RegQueryValueExX( HKEY hKey, LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData )
{
#if UNICODE
	if( !GUnicodeOS )
	{
		ANSICHAR* ACh = (ANSICHAR*)appAlloca(*lpcbData);
		LONG Result = RegQueryValueExA( hKey, TCHAR_TO_ANSI(lpValueName), lpReserved, lpType, (BYTE*)ACh, lpcbData );
		if( Result==ERROR_SUCCESS )
			MultiByteToWideChar( CP_ACP, 0, ACh, -1, (TCHAR*)lpData, *lpcbData );
		return Result;
	}
	else
#endif
	{
		return RegQueryValueEx( hKey, lpValueName, lpReserved, lpType, lpData, lpcbData );
	}
}
#endif

/*-----------------------------------------------------------------------------
	GetRealCDKey
-----------------------------------------------------------------------------*/

static FString GetRealCDKey()
{
	static FString cdkey;

	if( cdkey != TEXT("") )
		return cdkey;

#if _MSC_VER && !_XBOX
	// Read the cdkey from the registry.
	TCHAR Temp[256];
	FString Product;
	if( GConfig->GetString( TEXT("Engine.Engine"), TEXT("Product"), Temp, ARRAY_COUNT(Temp) ) )
		Product = Temp;
	HKEY hKey = NULL;
	if( RegOpenKeyX( HKEY_LOCAL_MACHINE, *FString::Printf(TEXT("Software\\Unreal Technology\\Installed Apps\\%s"),*Product), &hKey )==ERROR_SUCCESS )
	{
		TCHAR Buffer[4096]=TEXT("");
		DWORD Type=REG_SZ, BufferSize=sizeof(Buffer);
		if( RegQueryValueExX( hKey, TEXT("CDKey"), 0, &Type, (BYTE*)Buffer, &BufferSize )==ERROR_SUCCESS && Type==REG_SZ )
			cdkey = Buffer;
	}
#elif ((__linux__) || (__FreeBSD__))

  #if DEMOVERSION
    cdkey = TEXT("UT2DEM-UT2DEM-UT2DEM-UT2DEM");
  #else
    FArchive *in = GFileManager->CreateFileReader(TEXT("cdkey"));
    if (in == NULL)
    {
        debugf("Couldn't open cdkey file.");
    }

    else
    {
        char buf[128];
        int max = in->TotalSize();
        if (max > sizeof (buf))
            max = sizeof (buf);
        in->Serialize(buf, max);
        delete in;

        for (int i = 0; i < sizeof (buf); i++)
        {
            char ch = buf[i];
            if ( ! ( ((ch >= '0') && (ch <= '9')) ||
                     ((ch >= 'a') && (ch <= 'z')) ||
                     ((ch >= 'A') && (ch <= 'Z')) ||
                     ((ch == '-')) ) )
            {
                buf[i] = '\0';
                break;
            }
        }

        buf[sizeof (buf) - 1] = '\0';  // just in case.
        cdkey = buf;

        //debugf("cdkey is [%s].", *cdkey);
    }
  #endif
#else
#   error Please fill in CD Key stuff for your platform!
#endif

	return cdkey;
}

/*-----------------------------------------------------------------------------
	Internal MD5 processing
-----------------------------------------------------------------------------*/

static FString GetDigestString( BYTE* Digest )
{
	FString MD5;
	for( INT i=0; i<16; i++ )
		MD5 += FString::Printf(TEXT("%02x"), Digest[i]);	
	return MD5;
}

static FString MD5HashAnsiString( const TCHAR* String )
{
	const ANSICHAR* AnsiChallenge = appToAnsi( String );
	BYTE Digest[16];
	FMD5Context Context;
	appMD5Init( &Context );
	appMD5Update( &Context, (unsigned char*)AnsiChallenge, appStrlen( String ) );
	appMD5Final( Digest, &Context );
	return GetDigestString( Digest );
}

static INT HexDigit( TCHAR c )
{
	if( c>='0' && c<='9' )
		return c - '0';
	else if( c>='a' && c<='f' )
		return c + 10 - 'a';
	else if( c>='A' && c<='F' )
		return c + 10 - 'A';
	else
		return 0;
}

/*-----------------------------------------------------------------------------
	Global CD Key functions
-----------------------------------------------------------------------------*/

UBOOL ENGINE_API ValidateCDKey()
{
#if DEMOVERSION
	return 1;
#else
	return ValidateCDKey(*GetRealCDKey());
#endif
}

FString ENGINE_API GetCDKeyHash()
{
	return MD5HashAnsiString(*GetRealCDKey());
}

FString ENGINE_API GetCDKeyResponse( const TCHAR* Challenge )
{
	// Append challenge
	FString CDKeyChallenge = GetRealCDKey() + Challenge;

	// MD5
	return MD5HashAnsiString( *CDKeyChallenge );
}

FString ENGINE_API EncryptWithCDKeyHash( const TCHAR* String, const TCHAR* HashAppend )
{
	FString Result;
	FString Key = FString::Printf(TEXT("%s%s"), *GetRealCDKey(), HashAppend );
	FString Hash = MD5HashAnsiString( *Key );

    for( INT i=0;String[i];i++ )
		Result = Result + FString::Printf( TEXT("%02x"), (*Hash)[i%16]^String[i] );

	return Result;
}

FString ENGINE_API DecryptWithCDKeyHash( const TCHAR* String, const TCHAR* HashAppend, const TCHAR* InCDKey )
{
	FString Result, Hash;
	if( InCDKey )
	{
		FString Key = FString::Printf(TEXT("%s%s"), InCDKey, HashAppend );
		Hash = MD5HashAnsiString( *Key );
	}
	else
		Hash = HashAppend;

    for( INT i=0;String[i]&&String[i+1];i+=2 )
	{
		INT Ch = 16*HexDigit(String[i])+HexDigit(String[i+1]);
		Result = Result + FString::Printf( TEXT("%c"), (*Hash)[(i/2)%16]^Ch );
	}

	return Result;
}




/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

