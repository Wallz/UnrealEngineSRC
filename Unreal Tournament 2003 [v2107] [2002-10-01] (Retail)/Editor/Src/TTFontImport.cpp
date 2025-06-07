/*=============================================================================
	TTFontImport.cpp: True-type Font Importing
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#include "EditorPrivate.h"

// Windows includes.
#define STRICT
#undef TEXT
#undef HANDLE
#undef HINSTANCE
#include <windows.h>

/*------------------------------------------------------------------------------
	UTrueTypeFontFactory.
------------------------------------------------------------------------------*/

INT FromHex( TCHAR Ch )
{
	if( Ch>='0' && Ch<='9' )
		return Ch-'0';
	else if( Ch>='a' && Ch<='f' )
		return 10+Ch-'a';
	else if( Ch>='A' && Ch<='F' )
		return 10+Ch-'A';
	appErrorf(TEXT("Expecting digit, got character %i"),Ch);
	return 0;
}
void UTrueTypeFontFactory::StaticConstructor()
{
	guard(UTrueTypeFontFactory::StaticConstructor);

	SupportedClass		= UFont::StaticClass();
	bCreateNew			= 1;
	bShowPropertySheet	= 1;
	bShowCategories		= 0;
	AutoPriority        = -1;
	Description			= TEXT("Font Imported From TrueType");
	InContextCommand	= TEXT("Import TrueType Font");
	OutOfContextCommand	= TEXT("Import TrueType Font");
	FontName			= TEXT("MS Sans Serif");
	Height				= 16.0;
	USize				= 256;
	VSize				= 256;
	XPad				= 1;
	YPad				= 1;
	CharactersPerPage	= 64;
	Gamma				= 0.7f;
	Count				= 256;
	AntiAlias			= 0;
	Chars				= TEXT("");
	Wildcard			= TEXT("");
	Path				= TEXT("");
	// amb --- added support for bold, italic, underline
	Style	            = FW_NORMAL;
	Italic	            = 0;
	Underline           = 0;
	new(GetClass(),TEXT("Style"),             RF_Public)UIntProperty  (CPP_PROPERTY(Style            ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Italic"),            RF_Public)UBoolProperty (CPP_PROPERTY(Italic           ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Underline"),         RF_Public)UBoolProperty (CPP_PROPERTY(Underline        ), TEXT(""), CPF_Edit );
    // --- amb
	new(GetClass(),TEXT("FontName"),          RF_Public)UStrProperty  (CPP_PROPERTY(FontName         ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Height"),            RF_Public)UFloatProperty(CPP_PROPERTY(Height           ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("USize"),             RF_Public)UIntProperty  (CPP_PROPERTY(USize            ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("VSize"),             RF_Public)UIntProperty  (CPP_PROPERTY(VSize            ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("XPad"),              RF_Public)UIntProperty  (CPP_PROPERTY(XPad             ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("YPad"),              RF_Public)UIntProperty  (CPP_PROPERTY(YPad             ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("CharactersPerPage"), RF_Public)UIntProperty  (CPP_PROPERTY(CharactersPerPage), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Count"),             RF_Public)UIntProperty  (CPP_PROPERTY(Count            ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Gamma"),             RF_Public)UFloatProperty(CPP_PROPERTY(Gamma            ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Chars"),             RF_Public)UStrProperty  (CPP_PROPERTY(Chars            ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("AntiAlias"),         RF_Public)UBoolProperty (CPP_PROPERTY(AntiAlias        ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("List"),              RF_Public)UStrProperty  (CPP_PROPERTY(List             ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Path"),              RF_Public)UStrProperty  (CPP_PROPERTY(Path             ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("Wildcard"),          RF_Public)UStrProperty  (CPP_PROPERTY(Wildcard         ), TEXT(""), CPF_Edit );

    // gam ---
    Kerning = 0;
	new(GetClass(),TEXT("Kerning"),           RF_Public)UIntProperty  (CPP_PROPERTY(Kerning          ), TEXT(""), CPF_Edit );

    DropShadowX = 0;
    DropShadowY = 0;
	new(GetClass(),TEXT("DropShadowX"),       RF_Public)UIntProperty  (CPP_PROPERTY(DropShadowX      ), TEXT(""), CPF_Edit );
	new(GetClass(),TEXT("DropShadowY"),       RF_Public)UIntProperty  (CPP_PROPERTY(DropShadowY      ), TEXT(""), CPF_Edit );
    // --- gam

	unguard;
}
UTrueTypeFontFactory::UTrueTypeFontFactory()
{
	guard(UTrueTypeFontFactory::UTrueTypeFontFactory);
	unguard;
}
UObject* UTrueTypeFontFactory::FactoryCreateNew
(
	UClass*				Class,
	UObject*			InParent,
	FName				Name,
	DWORD				Flags,
	UObject*			Context,
	FFeedbackContext*	Warn
)
{
	guard(UTrueTypeFontFactory::FactoryCreateNew);
	check(Class==UFont::StaticClass());
	INT i, j, n;

	// Create font and its texture.
	UFont* Font = new( InParent, Name, Flags )UFont;

	Font->CharactersPerPage = CharactersPerPage;
	Font->Kerning = Kerning; // gam
	Font->IsRemapped = 0;
	TMap<TCHAR,TCHAR> InverseMap;
	
	TArray<BYTE> ChList;
	if( Wildcard!=TEXT("") && Path!=TEXT("") )
	{
		// find all characters in specified path/wildcard
		// remap characters optimally.
		Font->IsRemapped = 1;

		// Map (ASCII)
		ChList.AddUniqueItem(0);
		for( TCHAR c=0;c<256;c++ )
		{
			Font->CharRemap.Set( c, c );
			InverseMap.Set( c, c );
		}
		TArray<FString> Files = GFileManager->FindFiles( *(Path*Wildcard),1,0 );
		BYTE* Chars = (BYTE*)appMalloc(65536*sizeof(BYTE), TEXT("UnicodeCheck") );
		appMemzero(Chars,sizeof(65536*sizeof(BYTE)));

		for( TArray<FString>::TIterator it(Files); it; ++it )
		{
			FString S;
			verify(appLoadFileToString(S,*(Path * *it)));
			Warn->Logf(TEXT("Checking %s"), *(Path * *it));
			for( INT i=0; i<S.Len(); i++ )
				Chars[(*S)[i]] = 1;
		}
		
		INT j=256;
		for( i=256; i<65536; i++ )
			if( Chars[i] )
			{
				Font->CharRemap.Set( i, j );
				InverseMap.Set( j++, i );
			}
		for( i=1; i<=(j/256); i++)
		{
			Warn->Logf(TEXT("Using font page %d"), i);
			ChList.AddUniqueItem(i);
		}
		Count=65536;
		appFree(Chars);	
	}
	else
	if( List!=TEXT("") )
	{
		Warn->Logf(TEXT("List <%s>:"),*List);
		const TCHAR* C=*List;
		while( *C )
		{
			INT Current = FromHex(C[0])*16 + FromHex(C[1]);
			C+=2;
			ChList.AddUniqueItem(Current);
			if( *C=='-' )
			{
				C++;
				INT Next=FromHex(C[0])*16 + FromHex(C[1]);
				C+=2;
				check(Next>Current);
				for( INT i=Current+1; i<=Next; i++ )
					ChList.AddUniqueItem(i);
			}
			if( *C==' ' )
				C++;
			else
				check(*C==0);
		}
		for( INT i=0; i<ChList.Num(); i++ )
			Warn->Logf(TEXT("   %02X"),ChList(i));
		Count=65536;
	}

    // gam --- Find out if it should be remapped for case translation
    
    // If all upper case chars have lower case char counterparts no mapping is required.
    
    if( !Font->IsRemapped )
    {
        bool NeedToRemap = false;
        
        for( const TCHAR* p = *Chars; *p; p++ )
        {
            TCHAR c;
            
            if( !appIsAlpha( *p ) )
                continue;
            
            if( appIsUpper( *p ) )
                c = appToLower( *p );
            else
                c = appToUpper( *p );

            if( appStrchr(*Chars, c) )
                continue;
            
            NeedToRemap = true;
            break;
        }
        
        if( NeedToRemap )
        {
            Font->IsRemapped = 1;

            for( const TCHAR* p = *Chars; *p; p++ )
		    {
                TCHAR c;

                if( !appIsAlpha( *p ) )
                {
			        Font->CharRemap.Set( *p, *p );
			        InverseMap.Set( *p, *p );
                    continue;
                }
                
                if( appIsUpper( *p ) )
                    c = appToLower( *p );
                else
                    c = appToUpper( *p );

			    Font->CharRemap.Set( *p, *p );
			    InverseMap.Set( *p, *p );

                if( !appStrchr(*Chars, c) )
			        Font->CharRemap.Set( c, *p );
		    }
        }
    }
    // --- gam

	// Create the font.

	// gam ---
	HDC tempDC = CreateCompatibleDC( NULL );
	INT nHeight = -appRound(Height * (FLOAT)GetDeviceCaps(tempDC, LOGPIXELSY) / 72.0);
	DeleteDC( tempDC );

	HFONT F = TCHAR_CALL_OS(
		CreateFont(nHeight, 0, 0, 0, Style, Italic, Underline, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, AntiAlias ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY, VARIABLE_PITCH, *FontName), // sjs
		CreateFontA(nHeight, 0, 0, 0, Style, Italic, Underline, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, AntiAlias ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY, VARIABLE_PITCH, TCHAR_TO_ANSI(*FontName)) ); // sjs
	if( !F )
	{
		Warn->Logf( NAME_Error, TEXT("CreateFont failed: %s"), appGetSystemErrorMessage() );
		return NULL;
	}
	// --- gam

	// Create palette.
	UPalette* Palette = new( Font, NAME_None, RF_Public )UPalette;
	Palette->Colors.Empty();
	Palette->Colors.AddZeroed( NUM_PAL_COLORS );
	for( i=0; i<256; i++ )
	{
		Palette->Colors(i).R = AntiAlias ? i : i ? 255 : 0;
		Palette->Colors(i).G = AntiAlias ? i : i ? 255 : 0;
		Palette->Colors(i).B = AntiAlias ? i : i ? 255 : 0;
		Palette->Colors(i).A = AntiAlias ? i : i ? 255 : 0;
	}

	// Render all font characters into texture.
	INT      N         = 1+65536/Font->CharactersPerPage;
	INT*     X         =(INT    *)appAlloca(N*sizeof(INT    ));
	INT*     Y         =(INT    *)appAlloca(N*sizeof(INT    ));
	INT*     RowHeight =(INT    *)appAlloca(N*sizeof(INT    ));
	HBITMAP* B         =(HBITMAP*)appAlloca(N*sizeof(HBITMAP));
	HDC*     dc        =(HDC    *)appAlloca(N*sizeof(HDC    ));
	for( i=0; i<N; i++ )
	{
		X [i] = Y[i] = RowHeight[i] = 0;
		B [i] = NULL;
		dc[i] = NULL;
	}
	for( INT Ch=0; Ch<Count; Ch++ )
	{
		// Skip if this character isn't desired.
		if( Chars!=TEXT("") && (!Ch || !appStrchr(*Chars, Ch)) )
			continue;
		if( ChList.Num() && ChList.FindItemIndex(Ch/256)==INDEX_NONE )
			continue;

		// Create font page if needed.
		INT Page  = Ch / Font->CharactersPerPage;
		INT Index = Ch - Font->CharactersPerPage * Page;
		if( Page >= Font->Pages.Num() )
			Font->Pages.AddZeroed( 1 + Page - Font->Pages.Num() );
		FFontPage* PageInfo = &Font->Pages(Page);
		if( Index >= PageInfo->Characters.Num() )
			PageInfo->Characters.AddZeroed( 1 + Index - PageInfo->Characters.Num() );

		// Create Windows resources.
		if( !B[Page] )
		{
			dc[Page] = CreateCompatibleDC( NULL );
			if( !dc[Page] )
			{
				Warn->Logf( NAME_Error, TEXT("CreateDC failed: %s"), appGetSystemErrorMessage() ); // gam
				return NULL;
			}
			if( AntiAlias )
			{
				BITMAPINFO* pBI = (BITMAPINFO*)appMalloc(sizeof(BITMAPINFO), TEXT("FontImport"));

				pBI->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
				pBI->bmiHeader.biWidth         = USize;
				pBI->bmiHeader.biHeight        = VSize;
				pBI->bmiHeader.biPlanes        = 1;      //  Must be 1
				pBI->bmiHeader.biBitCount      = 24;
				pBI->bmiHeader.biCompression   = BI_RGB; 
				pBI->bmiHeader.biSizeImage     = 0;      
				pBI->bmiHeader.biXPelsPerMeter = 0;      
				pBI->bmiHeader.biYPelsPerMeter = 0;      
				pBI->bmiHeader.biClrUsed       = 0;      
				pBI->bmiHeader.biClrImportant  = 0;      

				void* pvBmp;
				B [Page] = CreateDIBSection((HDC)NULL, 
										pBI,
										DIB_RGB_COLORS,
										&pvBmp,
										NULL,
										0);  
				appFree( pBI );
			}
			else
			{
				B [Page] = CreateBitmap( USize, VSize, 1, 1, NULL);
			}

			if( !B[Page] )
			{
				Warn->Logf( NAME_Error, TEXT("CreateBitmap failed: %s"), appGetSystemErrorMessage() ); // gam
				return NULL;
			}

            // gam -- AntiAlias fix from UnProg
			SelectObject( dc[Page], F );
			SelectObject( dc[Page], B[Page] );
			SetTextColor( dc[Page], 0x00ffffff );
			SetBkColor( dc[Page], 0x00000000 );
			HBRUSH Black = CreateSolidBrush(0x00000000);
			RECT   r     = {0, 0, USize, VSize};
			FillRect( dc[Page], &r, Black );
            // --- gam
		}

		// Get text size.
		SIZE Size;
		TCHAR Tmp[5];
		if( Font->IsRemapped )
		{
			TCHAR *p = InverseMap.Find(Ch);
			Tmp[0] = p ? *p : 32;
		}
		else
			Tmp[0] = Ch;
		Tmp[1] = 0;
		GetTextExtentPoint32( dc[Page], Tmp, 1, &Size );

        // gam ---
        Size.cx += DropShadowX;
        Size.cy += DropShadowY;
        // --- gam

		// If it doesn't fit right here, advance to next line.
		if( Size.cx + X[Page] + 2 > USize)
		{
			Y[Page]         = Y[Page] + RowHeight[Page] + YPad;
			RowHeight[Page] = 0;
			X[Page]         = 0;
		}

		// Set font character information.
		PageInfo->Characters(Index).StartU = X[Page];
		PageInfo->Characters(Index).StartV = Y[Page]+1;
		PageInfo->Characters(Index).USize  = Size.cx;
		PageInfo->Characters(Index).VSize  = Size.cy;

		// Draw character into font and advance.
		if( Size.cy > RowHeight[Page] )
			RowHeight[Page] = Size.cy;
		if( Y[Page]+RowHeight[Page]>VSize )
		{
			Warn->Logf( TEXT("Font vertical size exceeded maximum of %i at character %i"), VSize, Index );
			return NULL;
		}
		TextOut( dc[Page], X[Page], Y[Page], Tmp, 1 );
		X[Page] = X[Page] + Size.cx + XPad;
	}

	// Create textures.
	for( n=0; n<Font->Pages.Num(); n++ )
	{
		FFontPage* PageInfo = &Font->Pages(n);
		if( PageInfo->Characters.Num() )
		{
			// Create texture for page.
			PageInfo->Texture = new(Font)UTexture;
            PageInfo->Texture->Format = TEXF_RGBA8; // gam
			PageInfo->Texture->Init( USize, 1<<appCeilLogTwo(Y[n]+RowHeight[n]) );

			if( AntiAlias )
				PageInfo->Texture->bAlphaTexture = 1;
			else
				PageInfo->Texture->bMasked = 1;

			PageInfo->Texture->PostLoad();
			PageInfo->Texture->Palette = Palette;

            // gam ---
            FString TextureString = FString::Printf( TEXT("%s_Page%c"), *Name, 'A' + n );
            if( !StaticFindObject( NULL, PageInfo->Texture->GetOuter(), *TextureString ) )
                PageInfo->Texture->Rename( *TextureString );
            else
		        Warn->Logf( TEXT("A texture named %s already exists!"), *TextureString );

			// Copy bitmap data to texture page.

			if( !AntiAlias )
            {
			for( i=0; i<PageInfo->Texture->USize; i++ )
			{
				for( j=0; j<PageInfo->Texture->VSize; j++ )
				{
                        INT CharAlpha = GetRValue( GetPixel( dc[n], i, j ) );
                        INT DropShadowAlpha;

                        if( (i >= DropShadowX) && (j >= DropShadowY) )
                            DropShadowAlpha = GetRValue( GetPixel( dc[n], i - DropShadowX, j - DropShadowY ) );
                        else
                            DropShadowAlpha = 0;

                        if( CharAlpha )
                        {
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 0) = 0xFF;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 1) = 0xFF;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 2) = 0xFF;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 3) = 0xFF;
                        }
                        else if( DropShadowAlpha )
                        {
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 0) = 0x00;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 1) = 0x00;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 2) = 0x00;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 3) = 0xFF;
                        }
                        else
					{
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 0) = 0x00;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 1) = 0x00;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 2) = 0x00;
						    PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 3) = 0x00;
                        }
				    }
			    }
					}
					else
					{
			    for( i=0; i<PageInfo->Texture->USize; i++ )
			    {
				    for( j=0; j<PageInfo->Texture->VSize; j++ )
				    {
                        INT CharAlpha = GetRValue( GetPixel( dc[n], i, j ) );
                        float fCharAlpha = float( CharAlpha ) / 255.0f;

                        INT DropShadowAlpha;

                        if( (i >= DropShadowX) && (j >= DropShadowY) )
                            DropShadowAlpha = GetRValue( GetPixel( dc[n], i - DropShadowX, j - DropShadowY ) );
                        else
                            DropShadowAlpha = 0;

                        // Dest = (White * fCharAlpha) + (1.0 - fCharAlpha) * (Black * fDropShadowAlpha)                        // Dest = (White * CharAlpha) + (1.0 - CharAlpha) * (Black * DropShadowAlpha)
                        // --------------------------------------------------------------------------
                        // Dest.[RGB] = (255 * fCharAlpha)
                        // Dest.A = (255 * fCharAlpha) + (1.0 - fCharAlpha) * (255 * fDropShadowAlpha)

						PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 0) = CharAlpha;						PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 0) = CharAlpha;
						PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 1) = CharAlpha;
						PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 2) = CharAlpha;

						PageInfo->Texture->Mips(0).DataArray(4 * (i + j * PageInfo->Texture->USize) + 3)
                            = (BYTE)( CharAlpha + ( 1.0f - fCharAlpha ) * ( (float)DropShadowAlpha ) );
				    }
					}
				}

            FString TempFileName = FString (PageInfo->Texture->GetName()) + TEXT("_temp.tga");
            FString DestFileName = FString (PageInfo->Texture->GetName()) + TEXT(".tga");

            TArray<BYTE> TempFileData;
            TArray<BYTE> DestFileData;

            GEditor->Exec( *FString::Printf( TEXT("OBJ EXPORT TYPE=TEXTURE FILE=\"%s\" NAME=\"%s\""),
                *TempFileName, PageInfo->Texture->GetName()) );

            appLoadFileToArray( TempFileData, *TempFileName );
            appLoadFileToArray( DestFileData, *DestFileName );

            if( (TempFileData.Num() != DestFileData.Num()) || (appMemcmp (TempFileData.GetData(), DestFileData.GetData(), TempFileData.Num()) != 0 ) )
            {
        		if( !appSaveArrayToFile( TempFileData, *DestFileName ) )
		            Warn->Logf( NAME_Error, TEXT("Couldn't update %s!"), *DestFileName );
			}

            GFileManager->Delete ( *TempFileName );

            FString newName = FString::Printf(TEXT("%sDXT"), PageInfo->Texture->GetName() );

            GEditor->Exec( *FString::Printf( TEXT("TEXTURE IMPORT FILE=\"%s.tga\" NAME=\"%s\" PACKAGE=\"%s\" GROUP=\"%s\" MIPS=0 ALPHA=1 DXT=5"),
                PageInfo->Texture->GetName(), *newName, Font->GetOuter()->GetName(), PageInfo->Texture->GetOuter()->GetName()) );

            UTexture* dxtTexture = Cast<UTexture>(StaticFindObject( UTexture::StaticClass(), ANY_PACKAGE, *newName ));
            delete PageInfo->Texture;
            check( dxtTexture );
            PageInfo->Texture = dxtTexture;
            // --- gam
		}
		if( dc[i] )
			DeleteDC( dc[i] );
		if( B[i] )
			DeleteObject( B[i] );
	}

	// Success.
	return Font;

	unguard;
}
IMPLEMENT_CLASS(UTrueTypeFontFactory);

/*------------------------------------------------------------------------------
	The end.
------------------------------------------------------------------------------*/
