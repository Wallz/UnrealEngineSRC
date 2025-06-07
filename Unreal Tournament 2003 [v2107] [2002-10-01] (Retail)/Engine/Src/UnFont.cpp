/*=============================================================================
	UnFont.cpp: Unreal font code.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

#include "EnginePrivate.h"
//#include "UnRender.h"

/*------------------------------------------------------------------------------
	UFont implementation.
------------------------------------------------------------------------------*/

UFont::UFont()
{}

void UFont::Serialize( FArchive& Ar )
{
	guard(UFont::Serialize);
	Super::Serialize( Ar );
	UBOOL GSavedLazyLoad = GLazyLoad;
#ifdef _XBOX
	GLazyLoad = 0;
#else
	GLazyLoad = 1;
#endif
	Ar << Pages << CharactersPerPage << Kerning; // gam
	// sjs rem'd check(!(CharactersPerPage&(CharactersPerPage-1)));
    // gam ---
	if( !GLazyLoad )
    {
		for( INT c=0,p=0; c<256; c+=CharactersPerPage,p++ )
        {
			if( p<Pages.Num() && Pages(p).Texture )
            {
				for( INT i=0; i<Pages(p).Texture->Mips.Num(); i++ )
					Pages(p).Texture->Mips(i).DataArray.Load();
            }
        }
    }
    // --- gam
	GLazyLoad = GSavedLazyLoad;
	if( Ar.Ver() >= 69 )
		Ar << CharRemap << IsRemapped;
	else
		IsRemapped = 0;
	unguardobj;
}
IMPLEMENT_CLASS(UFont);

/*------------------------------------------------------------------------------
	The End.
------------------------------------------------------------------------------*/

