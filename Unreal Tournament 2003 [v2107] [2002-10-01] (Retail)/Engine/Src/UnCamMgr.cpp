/*=============================================================================
	UnCamMgr.cpp: Unreal viewport manager, generic implementation.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

#include "EnginePrivate.h"
#include "UnRender.h"

/*-----------------------------------------------------------------------------
	UClient implementation.
-----------------------------------------------------------------------------*/

//
// Constructor.
//
UClient::UClient()
{
	guard(UClient::UClient);

	// Hook in.
	UTexture::__Client = this;

	unguard;
}
void UClient::PostEditChange()
{
	guard(UClient::PostEditChange);
	Super::PostEditChange();
	Brightness = Clamp(Brightness,0.f,1.f);
    
    // gam ---
	Contrast = Clamp( Contrast, 0.f, 2.f );
	Gamma = Clamp( Gamma, 0.5f, 2.5f );
	Engine->UpdateGamma();

	for(TObjectIterator<UTexture> It;It;++It)
        It->bRealtimeChanged = 1;

	SaveConfig();
    // --- gam

	unguard;
}
void UClient::StaticConstructor()
{
	guard(UClient::StaticConstructor);

    // gam ---
	PTextureDetail = new(GetClass(),TEXT("ETextureDetail"))UEnum( NULL );

	new(PTextureDetail->Names)FName( TEXT("UltraHigh")); // +4
	new(PTextureDetail->Names)FName( TEXT("VeryHigh") ); // +3
	new(PTextureDetail->Names)FName( TEXT("High")     ); // +2
	new(PTextureDetail->Names)FName( TEXT("Higher")   ); // +1
	new(PTextureDetail->Names)FName( TEXT("Normal")   ); // No-change
	new(PTextureDetail->Names)FName( TEXT("Lower")    ); // -1
	new(PTextureDetail->Names)FName( TEXT("Low")      ); // -2 
	new(PTextureDetail->Names)FName( TEXT("VeryLow")  ); // -3 
	new(PTextureDetail->Names)FName( TEXT("UltraLow") ); // -4
    // --- gam

	new(GetClass(),TEXT("WindowedViewportX"),	RF_Public)UIntProperty  (CPP_PROPERTY(WindowedViewportX     ), TEXT("Client"),  CPF_Config );
	new(GetClass(),TEXT("WindowedViewportY"),	RF_Public)UIntProperty  (CPP_PROPERTY(WindowedViewportY     ), TEXT("Client"),  CPF_Config );
	new(GetClass(),TEXT("FullscreenViewportX"),	RF_Public)UIntProperty  (CPP_PROPERTY(FullscreenViewportX   ), TEXT("Client"),  CPF_Config );
	new(GetClass(),TEXT("FullscreenViewportY"),	RF_Public)UIntProperty  (CPP_PROPERTY(FullscreenViewportY	), TEXT("Client"),  CPF_Config );
	new(GetClass(),TEXT("MenuViewportX"),	    RF_Public)UIntProperty  (CPP_PROPERTY(MenuViewportX         ), TEXT("Client"),  CPF_Config );
	new(GetClass(),TEXT("MenuViewportY"),	    RF_Public)UIntProperty  (CPP_PROPERTY(MenuViewportY	        ), TEXT("Client"),  CPF_Config );
	new(GetClass(),TEXT("Brightness"),			RF_Public)UFloatProperty(CPP_PROPERTY(Brightness			), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("Contrast"),			RF_Public)UFloatProperty(CPP_PROPERTY(Contrast				), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("Gamma"),				RF_Public)UFloatProperty(CPP_PROPERTY(Gamma					), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("CaptureMouse"),		RF_Public)UBoolProperty (CPP_PROPERTY(CaptureMouse			), TEXT("Display"), CPF_Config );

    // gam ---
    new(GetClass(),TEXT("TextureDetailWorld"),              RF_Public)UByteProperty (CPP_PROPERTY(TextureLODSet[LODSET_World]           ), TEXT("Display"), CPF_Config, PTextureDetail );
    new(GetClass(),TEXT("TextureDetailPlayerSkin"),         RF_Public)UByteProperty (CPP_PROPERTY(TextureLODSet[LODSET_PlayerSkin]      ), TEXT("Display"), CPF_Config, PTextureDetail );
    new(GetClass(),TEXT("TextureDetailWeaponSkin"),         RF_Public)UByteProperty (CPP_PROPERTY(TextureLODSet[LODSET_WeaponSkin]      ), TEXT("Display"), CPF_Config, PTextureDetail );
    new(GetClass(),TEXT("TextureDetailTerrain"),            RF_Public)UByteProperty (CPP_PROPERTY(TextureLODSet[LODSET_Terrain]         ), TEXT("Display"), CPF_Config, PTextureDetail );
    new(GetClass(),TEXT("TextureDetailInterface"),          RF_Public)UByteProperty (CPP_PROPERTY(TextureLODSet[LODSET_Interface]       ), TEXT("Display"), CPF_Config, PTextureDetail );
    new(GetClass(),TEXT("TextureDetailRenderMap"),          RF_Public)UByteProperty (CPP_PROPERTY(TextureLODSet[LODSET_RenderMap]       ), TEXT("Display"), CPF_Config, PTextureDetail );
    new(GetClass(),TEXT("TextureDetailLightmap"),           RF_Public)UByteProperty (CPP_PROPERTY(TextureLODSet[LODSET_Lightmap]        ), TEXT("Display"), CPF_Config, PTextureDetail );
    // --- gam

	new(GetClass(),TEXT("ScreenFlashes"),		RF_Public)UBoolProperty (CPP_PROPERTY(ScreenFlashes			), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("NoLighting"),			RF_Public)UBoolProperty (CPP_PROPERTY(NoLighting			), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("MinDesiredFrameRate"),	RF_Public)UFloatProperty(CPP_PROPERTY(MinDesiredFrameRate	), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("Decals"),				RF_Public)UBoolProperty (CPP_PROPERTY(Decals				), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("NoDynamicLights"),		RF_Public)UBoolProperty (CPP_PROPERTY(NoDynamicLights		), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("NoFractalAnim"),		RF_Public)UBoolProperty (CPP_PROPERTY(NoFractalAnim 		), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("Coronas"),				RF_Public)UBoolProperty (CPP_PROPERTY(Coronas		 		), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("DecoLayers"),			RF_Public)UBoolProperty (CPP_PROPERTY(DecoLayers	 		), TEXT("Display"), CPF_Config );	
	new(GetClass(),TEXT("Projectors"),			RF_Public)UBoolProperty (CPP_PROPERTY(Projectors	 		), TEXT("Display"), CPF_Config );	
	new(GetClass(),TEXT("ReportDynamicUploads"),RF_Public)UBoolProperty (CPP_PROPERTY(ReportDynamicUploads	), TEXT("Display"), CPF_Config );
	new(GetClass(),TEXT("ScaleHUDX"),			RF_Public)UFloatProperty(CPP_PROPERTY(ScaleHUDX				), TEXT("Display"), CPF_Config );

	unguard;
}
void UClient::Destroy()
{
	guard(UClient::Destroy);
	UTexture::__Client = NULL;
	Super::Destroy();
	unguard;
}

//
// Command line.
//
UBOOL UClient::Exec( const TCHAR* Cmd, FOutputDevice& Ar )
{
	guard(UClient::Exec);
	if( ParseCommand(&Cmd,TEXT("BRIGHTNESS")) )
	{
		if( *Cmd == '+' )
			Brightness = Brightness >= 0.9f ? 0.f : Brightness + 0.1f;
		else
		if( *Cmd )
			Brightness = Clamp<FLOAT>( appAtof(Cmd), 0.f, 1.f );
		else
			Brightness = 0.5f;
		Engine->UpdateGamma();
		SaveConfig();
		if( Viewports.Num() && Viewports(0)->Actor )//!!ugly
			Viewports(0)->Actor->eventClientMessage( *FString::Printf(TEXT("Brightness %i"), (INT)(Brightness*10)), NAME_None );
		return 1;
	}
	else
	if( ParseCommand(&Cmd,TEXT("CONTRAST")) )
	{
		if( *Cmd == '+' )
			Contrast = Contrast >= 0.9f ? 0.f : Contrast + 0.1f;
		else
		if( *Cmd )
			Contrast = Clamp<FLOAT>( appAtof(Cmd), 0.f, 2.f );
		else
			Contrast = 0.5f;
		Engine->UpdateGamma();
		SaveConfig();
		if( Viewports.Num() && Viewports(0)->Actor )//!!ugly
			Viewports(0)->Actor->eventClientMessage( *FString::Printf(TEXT("Contrast %i"), (INT)(Contrast*10)), NAME_None );
		return 1;
	}
	else
	if( ParseCommand(&Cmd,TEXT("GAMMA")) )
	{
		if( *Cmd == '+' )
			Gamma = Gamma >= 2.4f ? 0.5f : Gamma + 0.1f;
		else
		if( *Cmd )
			Gamma = Clamp<FLOAT>( appAtof(Cmd), 0.5f, 2.5f );
		else
			Gamma = 1.7f;
		Engine->UpdateGamma();
		SaveConfig();
		if( Viewports.Num() && Viewports(0)->Actor )//!!ugly
			Viewports(0)->Actor->eventClientMessage( *FString::Printf(TEXT("Gamma %1.1f"), Gamma), NAME_None );
		return 1;
	}
	else return 0;
	unguard;
}

//
// Init.
//
void UClient::Init( UEngine* InEngine )
{
	guard(UClient::Init);
	Engine = InEngine;
	unguard;
}

//
// Flush.
//
void UClient::Flush( UBOOL AllowPrecache )
{
	guard(UClient::Flush);

	for( INT i=0; i<Viewports.Num(); i++ )
		if( Viewports(i)->RenDev )
			Viewports(i)->RenDev->Flush( Viewports(i));

	unguard;
}

//
// Update Gamma.
//
void UClient::UpdateGamma()
{
	guard(UClient::UpdateGamma);

	for( INT i=0; i<Viewports.Num(); i++ )
		if( Viewports(i)->RenDev )
			Viewports(i)->RenDev->UpdateGamma( Viewports(i) );
	
	unguard;
}

//
// Restore Gamma.
//
void UClient::RestoreGamma()
{
	guard(UClient::RestoreGamma);

	for( INT i=0; i<Viewports.Num(); i++ )
		if( Viewports(i)->RenDev )
			Viewports(i)->RenDev->RestoreGamma();
	
	unguard;
}

//
// Serializer.
//
void UClient::Serialize( FArchive& Ar )
{
	guard(UClient::Serialize);
	Super::Serialize( Ar );

	// Only serialize objects, since this can't be loaded or saved.
	Ar << Viewports;

	unguard;
}

// gam ---
INT UClient::GetTextureLODBias (ELODSet LODSet)
{
	guard(UClient::GetTextureLODBias);
    check (LODSet < LODSET_MAX);
    check (PTextureDetail);

    return (TextureLODSet[LODSet] - (PTextureDetail->Names.Num() / 2));

	unguard;
}
// --- gam

IMPLEMENT_CLASS(UClient);

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

