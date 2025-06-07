/*=============================================================================
	UnAudio.cpp: Unreal base audio.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Tim Sweeney
	* Wave modification code by Erik de Neve
=============================================================================*/

#include "EnginePrivate.h" 


/*-----------------------------------------------------------------------------
	USound implementation.
-----------------------------------------------------------------------------*/

void FSoundData::Load()
{
	guard(FSoundData::Load);

    // gam --- Prevent loading of procedural sounds
    checkSlow( Owner );
    checkSlow( Owner->GetClass() == USound::StaticClass() );
    // --- gam

	UBOOL Loaded = SavedPos>0;

	guard(0);
	TLazyArray<BYTE>::Load();
	unguard;

	if( Loaded && (Owner->FileType != FName(TEXT("PS2"))) )
	{
		// Calculate our duration.
		guard(1);
		guard(callingGetPerioid);
		Owner->Duration = GetPeriod();
		unguard;
		unguard;

		// Derive these from the exposed 'low quality' preference setting.
		INT Force8Bit = 0;
		INT ForceHalve = 0;
		guard(3);
		if( Owner->Audio && Owner->Audio->LowQualitySound() && !GIsEditor )
		{
			Force8Bit = 1;
			ForceHalve = 1;
		}
		unguard;

		// Frequencies below this sample rate will NOT be downsampled.
		DWORD FreqThreshold = 22050;

		// Reduce sound frequency and/or bit depth if required.
		if( Force8Bit || ForceHalve )
		{		
			// If ReadWaveInfo returns true, all relevant Wave chunks were found and 
			// all pointers in the WaveInfo structure have been successfully initialized.			
			guard(4);
			FWaveModInfo WaveInfo;
			if( WaveInfo.ReadWaveInfo(*this) && WaveInfo.SampleDataSize>4  ) 
			{				
				// Three main conversions:
				// * Halving the frequency -> simple 0.25, 0.50, 0.25 kernel. 
				// * Reducing bit-depth 8->16  
				// * Both in one sweep.  
				//
				// Important: Wave data ALWAYS padded to use 16-bit alignment even
				// though the number of bytes in pWaveDataSize may be odd.	
				UBOOL ReduceBits = ((Force8Bit) && (*WaveInfo.pBitsPerSample == 16));
				UBOOL ReduceFreq = ((ForceHalve) && (*WaveInfo.pSamplesPerSec >= FreqThreshold));
				if( ReduceBits && ReduceFreq )
				{
					// Convert 16-bit sample to 8 bit and halve the frequency too.
					guard(5);
					WaveInfo.HalveReduce16to8();
					unguard;
				}
				else if (ReduceBits && (!ReduceFreq))
				{	
					// Convert 16-bit sample down to 8-bit.
					guard(6);
					WaveInfo.Reduce16to8();
					unguard;
				}
				else if( ReduceFreq )
				{
					// Just halve the frequency. Separate cases for 16 and 8 bits.
					guard(7);
					WaveInfo.HalveData();
					unguard;
				}
				guard(8);
				WaveInfo.UpdateWaveData( *this );
				unguard;
			}
			unguard;
		}

		// Register it.
		guard(2);
		Owner->OriginalSize = Num();
		if( Owner->Audio && !GIsEditor )
			Owner->Audio->RegisterSound( Owner );
		unguard;
	} else if ( Loaded && (Owner->FileType == FName(TEXT("PS2"))) ) {
		// Register it.
		guard(RegisterSound);
		Owner->Duration = GetPeriod();
		Owner->OriginalSize = Num();
		if( Owner->Audio && !GIsEditor )
			Owner->Audio->RegisterSound( Owner );
		unguard;
	}
	unguard;
}

FLOAT FSoundData::GetPeriod()
{
	FLOAT Period = 0.f;
	if( Owner->FileType != FName(TEXT("PS2")) )
	{
		// Ensure the data is present.
		TLazyArray<BYTE>::Load();

		// Calculate the sound's duration.
		FWaveModInfo WaveInfo;
		if( WaveInfo.ReadWaveInfo(*this) )
		{
			#define DEFAULT_FREQUENCY (22050)
			INT DurDiv =  *WaveInfo.pChannels * *WaveInfo.pBitsPerSample  * *WaveInfo.pSamplesPerSec;  
			if ( DurDiv ) Period = *WaveInfo.pWaveDataSize * 8.f / (FLOAT)DurDiv;
		}	
		return Period;
	} else {
		// Ensure the data is present.
		TLazyArray<BYTE>::Load();

		// Get the period from the header.
		appMemcpy( &Period, ((BYTE*) Data) + sizeof(INT), sizeof(FLOAT) );
	}
	return Period;
}
// gam ---
USound::USound()
    : Data( this )
{
    // TODO: for some reason these aren't being read from the defaultproperties
    Duration=-1.0;
    Likelihood=1.0;
	Flags = 0; // sjs - per vogel
}

USound::USound( const TCHAR* InFilename, INT InFlags ) // sjs - per vogel
: Data( this )
{
	Filename	= InFilename;
	Flags		= InFlags | SF_Streaming;
	Duration	= 1.f;//dummy
	Likelihood	= 1.f;
}

USound* USound::RenderSoundPlay( FLOAT *Volume, FLOAT *Pitch )
{
    return this;
}

FLOAT USound::GetDuration()
{
	guard(USound::GetDuration);
	if ( Duration < 0.f )
		Duration = Data.GetPeriod();
	return Duration;
	unguard;
}
void USound::SetDuration(FLOAT Duration)
{
	guard(USound::SetDuration);
    this->Duration = Duration;
	unguard;
}
void USound::Serialize( FArchive& Ar )
{
	guard(USound::Serialize);
	Super::Serialize( Ar );
	Ar << FileType;

    if( Ar.LicenseeVer() >= 2 )
    	Ar << Likelihood;

	if( Ar.IsLoading() || Ar.IsSaving() )
	{
		Ar << Data;
	}
	else Ar.CountBytes( OriginalSize, OriginalSize );

    if( Ar.IsLoading() ) // gam
	    Flags = 0;

	unguard;
}

void USound::Destroy()
{
	guard(USound::Destroy);
	if( Audio )
		Audio->UnregisterSound( this );
	Super::Destroy();
	unguard;
}

void USound::PostLoad()
{
	guard(USound::PostLoad);
	Super::PostLoad();
	unguard;
}
bool USound::IsValid()
{
	guard(USound::IsValid);
    return (true);    
    unguard;
}
FSoundData &USound::GetData()
{
	guard(USound::GetData);
    return (Data);
    unguard;
}
FName USound::GetFileType()
{
	guard(USound::GetData);
    return (FileType);
    unguard;
}
void USound::SetFileType (FName FileType)
{
	guard(USound::SetFileType);
    this->FileType = FileType;
    unguard;
}
const TCHAR* USound::GetFilename()
{
	guard(USound::GetFilename);
    return *Filename;
    unguard;
}
INT USound::GetOriginalSize()
{
	guard(USound::GetOriginalSize);
    return (OriginalSize);
    unguard;
}
INT USound::GetHandle()
{
	guard(USound::GetHandle);
    return (Handle);
    unguard;
}
void USound::SetHandle(INT Handle)
{
	guard(USound::SetHandle);
    this->Handle = Handle;
    unguard;
}
INT USound::GetFlags()
{
	guard(USound::GetFlags);
    return (Flags);
    unguard;
}
FLOAT USound::GetRadius() // sjs
{
	guard(USound::GetRadius);
    return (BaseRadius);
    unguard;
}
FLOAT USound::GetVelocityScale() // gam
{
	guard(USound::GetVelocityScale);
    return (VelocityScale);
    unguard;
}

UAudioSubsystem* USound::Audio = NULL;
void USound::PS2Convert()
{
	guard(USound::PS2Convert);

	#if HAVE_VAG

	/*
	 * Convert a standard wave sound to PS2 format.
	 *
	 * 3 Steps:
	 * - Promote 8 bit sounds to 16 bit for ADPCM compression.
	 * - Perform VAG conversion.
	 */

	Data.Load();

	FWaveModInfo WavData;
	WavData.ReadWaveInfo( Data );

	// Check loop.
	INT SoundLoops = WavData.SampleLoopsNum;

	// GWarn->Logf( TEXT("Sound: %s Size: %i Depth: %i Rate: %i"), GetPathName(), WavData.SampleDataSize, *WavData.pBitsPerSample, *WavData.pSamplesPerSec );
	// Copy the sample data into a working buffer.
	// Convert 8 bit samples to 16 bit data as we go.
	TArray<BYTE> WorkData;

	INT NewDataSize = WavData.SampleDataSize;
	INT SoundRate = *WavData.pSamplesPerSec;

	FLOAT Period = 0.f;
	INT DurDiv =  *WavData.pChannels * *WavData.pBitsPerSample * *WavData.pSamplesPerSec;  
	if ( DurDiv )
		Period = *WavData.pWaveDataSize * 8.f / (FLOAT) DurDiv;

	guard(Promote8to16);
	if (*WavData.pBitsPerSample == 8)
	{
		WorkData.Add( NewDataSize*2 );
		for (INT i=0; i<NewDataSize; i++)
		{
			WorkData(i*2)	= 0;
			WorkData(i*2+1)	= WavData.SampleDataStart[i] + 128;
		}
	} else {
		WorkData.Add( NewDataSize );
		for (INT i=0; i<NewDataSize; i++)
			WorkData(i) = WavData.SampleDataStart[i];
	}
	unguard;

	EncVagInit( ENC_VAG_MODE_NORMAL );

	INT WorkSamples = WorkData.Num() / 2;
	INT NeededBlocks = WorkSamples / 28;
	if (WorkSamples % 28 != 0)
		NeededBlocks++;

	// Prepare output data.
	Data.Empty();

	// Add custom PS2 header.
	guard(CustomHeader);
	Data.Add( 16 );
	appMemcpy( &Data(0), &SoundRate, sizeof(INT) );
	appMemcpy( &Data(4), &Period, sizeof(FLOAT) );
	appMemcpy( &Data(8), &SoundLoops, sizeof(INT) );
	INT Pad=0;
	appMemcpy( &Data(12), &Pad, sizeof(INT) );
	unguard;

	// Encode one VAG block per every 28 samples.
	guard(EncodeVAGBlocks);
	INT ByteCount = 0;
	for (INT i=0; i<NeededBlocks; i++)
	{
		TArray<BYTE> WorkBlock;
		WorkBlock.Add( 28*2 ); // Add 28 samples worth of room.
		for (INT j=0; j<28*2; j++)
		{
			if (ByteCount < WorkData.Num())
				WorkBlock(j) = WorkData(ByteCount++);
			else
				WorkBlock(j) = 0; // Pad with zero.
		}
		INT BlockAttribute;
		if (SoundLoops)
		{
			if (i == 0)
				BlockAttribute = ENC_VAG_LOOP_START;
			else if (i+1 == NeededBlocks)
				BlockAttribute = ENC_VAG_LOOP_END;
			else
				BlockAttribute = ENC_VAG_LOOP_BODY;
		} else {
			if (i+1 == NeededBlocks)
				BlockAttribute = ENC_VAG_1_SHOT_END;
			else
				BlockAttribute = ENC_VAG_1_SHOT;
		}

		TArray<BYTE> EncodedBlock;
		EncodedBlock.Add( 16 );
		EncVag( (short*) &WorkBlock(0), (short*) &EncodedBlock(0), BlockAttribute );

		INT StartPosition = Data.Num();
		Data.Add( 16 );
		for (j=0; j<16; j++)
			Data(StartPosition+j) = EncodedBlock(j);
	}
	unguard;

	// Finish off the one shot conversion.
	if (SoundLoops == 0)
	{
		guard(FinishOneShot);
		INT StartPosition = Data.Num();
		Data.Add( 16 );
		EncVagFin( (short*) &Data(StartPosition) );
		unguard;
	}

	// Set the file type.
	FileType = FName( TEXT("PS2") );

	/*
	for (INT i=0; i<Data.Num()/16; i++)
	{
		GWarn->Logf( TEXT("%02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x %02x%02x"),
			Data(0 + i*16),  Data(1 + i*16),  Data(2 + i*16),  Data(3 + i*16),
			Data(4 + i*16),  Data(5 + i*16),  Data(6 + i*16),  Data(7 + i*16),
			Data(8 + i*16),  Data(9 + i*16),  Data(10 + i*16), Data(11 + i*16),
			Data(12 + i*16), Data(13 + i*16), Data(14 + i*16), Data(15 + i*16) );
	}
	*/

	#endif

	unguard;
}
IMPLEMENT_CLASS(USound);

// WARNING: HACKS AHEAD!
//
// In order to make all existing "USound" code work I had to derive these
// classes from USound. The similarity ends there, though -- USound doesn't
// have an interface per say; it's really not much more than a struct. So,
// since we want our objects to be interchangable with USound objects, we
// don't want to call Super::Anything because we aren't really a USound.
//
// So, never do Super::Anything, do Super::Super::Anything instead.

UProceduralSound::UProceduralSound()
{
	guard(UProceduralSound::UProceduralSound);

    BaseSound = NULL;
    RenderedVolumeModification = 1.0F;
    RenderedPitchModification = 1.0F;

    unguard;
}

USound*  UProceduralSound::RenderSoundPlay( FLOAT *Volume, FLOAT *Pitch )
{
	guard(UProceduralSound::RenderSoundPlay);

    FLOAT NewPitch;

    if( !IsValid() )
        return this;

    RenderedPitchModification = 1.0F + (PitchModification / 100.0F) + (appSRand() * PitchVariance / 100.0F);
	NewPitch = ::Max(0.f,*Pitch * RenderedPitchModification);
    *Pitch = NewPitch;
    return BaseSound->RenderSoundPlay( Volume, Pitch );
    unguard;
}

bool UProceduralSound::IsValid()
{
	guard(UProceduralSound::IsValid);
    return ((BaseSound != NULL) && (BaseSound != this) && BaseSound->IsValid() );
    unguard;
}
FSoundData &UProceduralSound::GetData()
{
	guard(UProceduralSound::GetData);
    check (IsValid());
    return (BaseSound->GetData());
    unguard;
}
FName UProceduralSound::GetFileType()
{
	guard(UProceduralSound::GetFileType);
    check (IsValid());
    return (BaseSound->GetFileType());
    unguard;
}
void UProceduralSound::SetFileType (FName FileType)
{
	guard(UProceduralSound::SetFileType);
    check (IsValid());
    BaseSound->SetFileType (FileType);
    unguard;
}
const TCHAR* UProceduralSound::GetFilename()
{
	guard(UProceduralSound::GetFilename);
    check (IsValid());
    return (BaseSound->GetFilename());
    unguard;
}
INT UProceduralSound::GetOriginalSize()
{
	guard(UProceduralSound::GetOriginalSize);
    if (!IsValid())
        return (0);
    else
        return (BaseSound->GetOriginalSize());
    unguard;
}
INT UProceduralSound::GetHandle()
{
	guard(UProceduralSound::GetHandle);
    if (!IsValid())
        return (0);
    else
        return (BaseSound->GetHandle());
    unguard;
}
void UProceduralSound::SetHandle(INT Handle)
{
	guard(UProceduralSound::SetHandle);
    check (IsValid());
    BaseSound->SetHandle (Handle);
    unguard;
}
INT UProceduralSound::GetFlags()
{
	guard(UProceduralSound::GetFlags);
    check (IsValid());
    return (BaseSound->GetFlags());
    unguard;
}
FLOAT UProceduralSound::GetDuration()
{
	guard(UProceduralSound::GetDuration);
    if (!IsValid())
        return (0);
    else
	    return (BaseSound->GetDuration() * (1.0F / RenderedPitchModification));
	unguard;
}
FLOAT UProceduralSound::GetRadius()
{
	guard(UProceduralSound::GetDuration);
    if (!IsValid())
        return 0.0f;
    else
	    return BaseSound->GetRadius();
	unguard;
}
FLOAT UProceduralSound::GetVelocityScale() // gam
{
	guard(UProceduralSound::GetVelocityScale);
    if (!IsValid())
        return 0.0f;
    else
	    return BaseSound->GetVelocityScale();
    unguard;
}
void UProceduralSound::SetDuration(FLOAT Duration)
{
	guard(UProceduralSound::SetDuration);
    if (IsValid())
        BaseSound->SetDuration (Duration);
	unguard;
}
void UProceduralSound::PS2Convert()
{
	guard(UProceduralSound::PS2Convert);
	unguard;
}
void UProceduralSound::Load()
{
	guard(UProceduralSound::Load);
    if (IsValid())
        BaseSound->Load();
	unguard;
}

void UProceduralSound::Serialize( FArchive& Ar )
{
	guard(UProceduralSound::Serialize);

	Super::Super::Serialize( Ar );

  	Ar << Likelihood; // Manually serialize USound::Likelihood.

    Ar << BaseSound;

    Ar << PitchModification;
    Ar << VolumeModification;

    Ar << PitchVariance;
    Ar << VolumeVariance;

	unguard;
}

void UProceduralSound::Destroy()
{
	guard(UProceduralSound::Destroy);
	Super::Super::Destroy();
	unguard;
}

void UProceduralSound::PostLoad()
{
	guard(UProceduralSound::PostLoad);
	Super::Super::PostLoad();
	unguard;
}

void UProceduralSound::PostEditChange()
{
	guard(UProceduralSound::PostEditChange);

	Super::Super::PostEditChange();

    if( PitchModification < -100.0 )
        PitchModification = -100.0;

    if( VolumeModification < -100.0 )
        VolumeModification = -100.0;

    if( PitchVariance < -100.0 )
        PitchVariance = -100.0;

    if( VolumeVariance < -100.0 )
        VolumeVariance = -100.0;

	unguard;
}

IMPLEMENT_CLASS(UProceduralSound);

USoundGroup::USoundGroup()
{
    RenderedSound = NULL;
}

void USoundGroup::RefreshGroup( const FString &Package )
{
	guard(USoundGroup::RefreshGroup);
	
    FString PackageName;
    INT i;

    debugf( NAME_Warning, TEXT("USoundGroup::RefreshGroup() being called!") );

    UObject *PackageObject;

    Sounds.Empty();

	if( Package.Len() <= 0 )
		return;

    i = Package.InStr( TEXT(".") );

    if( i > 0 )
        PackageName = Package.Left( i );
    else
        PackageName = Package;

    PackageObject = Cast<UPackage>( LoadPackage( NULL, *PackageName, 0 ) );

    if( !PackageObject )
    {
        GWarn->Logf( NAME_Warning, TEXT("Can't load package %s"), *PackageName );
        return;
    }

    PackageObject = FindObject<UPackage>( NULL, *Package );

    if( !PackageObject )
    {
        GWarn->Logf( NAME_Warning, TEXT("Can't find package %s"), *Package );
        return;
    }
    
	for( TObjectIterator<USound> It; It; ++It )
	{
        USound *Sound = *It;
        USoundGroup *SoundGroup;

        if( !Sound->IsIn( PackageObject ) )
            continue;

        Sounds.AddItem( Sound );
        
        SoundGroup = Cast<USoundGroup>( Sound );
    }

    RefreshGroup();

    unguard;
}

void USoundGroup::RefreshGroup( UBOOL Force )
{
	guard(USoundGroup::RefreshGroup);

    if( !Force && TotalLikelihood < 0 )
        return;

    TotalLikelihood = 0;

	for( INT i = 0; i < Sounds.Num(); i++ )
	{
        USound *Sound = Sounds(i);
        USoundGroup *SoundGroup;
        
        SoundGroup = Cast<USoundGroup>( Sound );
        
        if( SoundGroup )
		{
            SoundGroup->RefreshGroup();
			TotalLikelihood += Sound->Likelihood;
		}
    }

	unguard;
}

USound* USoundGroup::RenderSoundPlay( FLOAT *Volume, FLOAT *Pitch )
{
	guard(USoundGroup::RenderSoundPlay);

    if( Sounds.Num() <= 0 )
    {
        debugf( NAME_Warning, TEXT("SoundGroup %s has no members!"), GetName() );
        return this;
    }
	TotalLikelihood = 0.f;
    for( INT i = 0; i < Sounds.Num(); i++)
 		TotalLikelihood += Sounds(i)->Likelihood;

    FLOAT r = appSRand() * TotalLikelihood;
    RenderedSound = Sounds(0);
    FLOAT Sum = 0.f;
    for( INT i = 0; i < Sounds.Num(); i++)
    {
		Sum += Sounds(i)->Likelihood;
        if( r < Sum )
        {
            RenderedSound = Sounds(i); 
            break;
        }
    }
    return RenderedSound->RenderSoundPlay( Volume, Pitch );
	unguard;
}

bool USoundGroup::IsValid()
{
	guard(USoundGroup::IsValid);
    return ((RenderedSound != NULL) && (RenderedSound->IsValid()));
    unguard;
}
FSoundData &USoundGroup::GetData()
{
	guard(USoundGroup::GetData);
    check (RenderedSound);
    return (RenderedSound->GetData());
    unguard;
}
FName USoundGroup::GetFileType()
{
	guard(USoundGroup::GetFileType);
    check (RenderedSound);
    return (RenderedSound->GetFileType());
    unguard;
}
void USoundGroup::SetFileType (FName FileType)
{
	guard(USoundGroup::SetFileType);
    check (RenderedSound);
    RenderedSound->SetFileType (FileType);
    unguard;
}
const TCHAR* USoundGroup::GetFilename()
{
	guard(USoundGroup::GetFilename);
    check (RenderedSound);
    return (RenderedSound->GetFilename());
    unguard;
}
INT USoundGroup::GetOriginalSize()
{
	guard(USoundGroup::GetOriginalSize);
    if (!RenderedSound)
        return (0);
    else
        return (RenderedSound->GetOriginalSize());
    unguard;
}
INT USoundGroup::GetHandle()
{
	guard(USoundGroup::GetHandle);
    if (!RenderedSound)
        return (0);
    else
        return (RenderedSound->GetHandle());
    unguard;
}
void USoundGroup::SetHandle(INT Handle)
{
	guard(USoundGroup::SetHandle);
    check (RenderedSound);
    RenderedSound->SetHandle (Handle);
    unguard;
}
INT USoundGroup::GetFlags()
{
	guard(USoundGroup::GetFlags);
    check (RenderedSound);
    return (RenderedSound->GetFlags());
    unguard;
}
FLOAT USoundGroup::GetDuration()
{
	guard(USoundGroup::GetDuration);
    check( Sounds.Num() );
    check( Sounds(0) );
	return Sounds(0)->GetDuration();
	unguard;
}
FLOAT USoundGroup::GetRadius()
{
	guard(USoundGroup::GetRadius);
    if( !RenderedSound )
    {
        if( Sounds.Num() <= 0 )
            return( 0.f );

        return Sounds(0)->GetRadius();
    }
	return RenderedSound->GetRadius();
	unguard;
}
FLOAT USoundGroup::GetVelocityScale()
{
	guard(USoundGroup::GetVelocityScale);
    if( !RenderedSound )
    {
        if( Sounds.Num() <= 0 )
            return( 0.f );

        return Sounds(0)->GetVelocityScale();
    }
	return RenderedSound->GetVelocityScale();
	unguard;
}
void USoundGroup::SetDuration(FLOAT Duration)
{
	guard(USoundGroup::SetDuration);
    check (RenderedSound);
    RenderedSound->SetDuration (Duration);
	unguard;
}
void USoundGroup::PS2Convert()
{
	guard(USoundGroup::PS2Convert);
	unguard;
}

void USoundGroup::Serialize( FArchive& Ar )
{
	guard(USoundGroup::Serialize);
	Super::Super::Serialize( Ar );

    if( Ar.LicenseeVer() < 0x1B )
    {
        Ar << Package;

	    if( !Ar.IsLoading() && !Ar.IsSaving() )
	    {
		    for( INT i = 0; i < Sounds.Num(); i++ )
			    Ar << Sounds(i);
	    }
	}
	else
	{
	    Ar << Sounds;
	    
	    if( Ar.IsLoading() )
	        TotalLikelihood = -1.0;
	}
	
	unguard;
}

void USoundGroup::Destroy()
{
	guard(USoundGroup::Destroy);
	Super::Super::Destroy();
	unguard;
}

void USoundGroup::PostLoad()
{
	guard(USoundGroup::PostLoad);
	Super::Super::PostLoad();
	
	if( Package.Len() )
	    RefreshGroup( Package );
	    
	unguard;
}
void USoundGroup::PostEditChange()
{
	guard(USoundGroup::PostEditChange);
	Super::Super::PostEditChange();
	RefreshGroup();
	unguard;
}

void USoundGroup::Load()
{
    guard(USoundGroup::Load);
    for (int i=0; i<Sounds.Num(); i++)
        Sounds(i)->Load();
    unguard;
}

IMPLEMENT_CLASS(USoundGroup);

// --- gam

/*-----------------------------------------------------------------------------
	WaveModInfo implementation - downsampling of wave files.
-----------------------------------------------------------------------------*/

//
//	Figure out the WAVE file layout.
//
UBOOL FWaveModInfo::ReadWaveInfo( TArray<BYTE>& WavData )
{
	guard(FWaveModInfo::ReadWaveInfo);

	FFormatChunk* FmtChunk;
	FRiffWaveHeader* RiffHdr = (FRiffWaveHeader*)&WavData(0);
	WaveDataEnd = &WavData(0) + WavData.Num();	
	
	// Verify we've got a real 'WAVE' header.
	if( RiffHdr->wID != ( mmioFOURCC('W','A','V','E') )  )
		return 0;

	pMasterSize = &RiffHdr->ChunkLen;

	// The appMemcpy indirect accessing is necessary to avoid PSX2 read alignment problems but
	// somehow causes a crash on PCs in "Look for a 'smpl' chunk."

#if __PSX2_EE__

	FRiffChunkOld RiffChunk;
	FRiffChunkOld* RiffChunkPtr = (FRiffChunkOld*)&WavData(3*4);
	appMemcpy( &RiffChunk, RiffChunkPtr, sizeof(FRiffChunkOld) );

	// Look for the 'fmt ' chunk.
	while( ( ((BYTE*)RiffChunkPtr + 8) < WaveDataEnd)  && ( RiffChunk.ChunkID != mmioFOURCC('f','m','t',' ') ) )
	{
		// Go to next chunk.
		RiffChunkPtr = (FRiffChunkOld*) ( (BYTE*)RiffChunkPtr + Pad16Bit(RiffChunk.ChunkLen) + 8);
		appMemcpy( &RiffChunk, RiffChunkPtr, sizeof(FRiffChunkOld) );
	}
	// Chunk found ?
	if( RiffChunk.ChunkID != mmioFOURCC('f','m','t',' ') )
		return 0;

	FmtChunk = (FFormatChunk*)((BYTE*)RiffChunkPtr + 8);
	pBitsPerSample  = &FmtChunk->wBitsPerSample;
	pSamplesPerSec  = &FmtChunk->nSamplesPerSec;
	pAvgBytesPerSec = &FmtChunk->nAvgBytesPerSec;
	pBlockAlign		= &FmtChunk->nBlockAlign;
	pChannels       = &FmtChunk->nChannels;

	// re-initalize the RiffChunk pointer
	RiffChunkPtr = (FRiffChunkOld*)&WavData(3*4);
	appMemcpy( &RiffChunk, RiffChunkPtr, sizeof(FRiffChunkOld) );

	// Look for the 'data' chunk.
	while( ( ((BYTE*)RiffChunkPtr + 8) < WaveDataEnd) && ( RiffChunk.ChunkID != mmioFOURCC('d','a','t','a') ) )
	{
		// Go to next chunk.
		RiffChunkPtr = (FRiffChunkOld*) ( (BYTE*)RiffChunkPtr + Pad16Bit(RiffChunk.ChunkLen) + 8); 
		appMemcpy( &RiffChunk, RiffChunkPtr, sizeof(FRiffChunkOld) );
	} 
	// Chunk found ?
	if( RiffChunk.ChunkID != mmioFOURCC('d','a','t','a') )
		return 0;

	SampleDataStart = (BYTE*)RiffChunkPtr + 8;
	pWaveDataSize   = &RiffChunkPtr->ChunkLen;
	SampleDataSize  =  RiffChunk.ChunkLen;
	OldBitsPerSample = FmtChunk->wBitsPerSample;
	SampleDataEnd   =  SampleDataStart+SampleDataSize;

	NewDataSize	= SampleDataSize;

	// Re-initalize the RiffChunk pointer
	RiffChunkPtr = (FRiffChunkOld*)&WavData(3*4);
	appMemcpy( &RiffChunk, RiffChunkPtr, sizeof(FRiffChunkOld) );

	// Look for a 'smpl' chunk.
	while( ( (((BYTE*)RiffChunkPtr) + 8) < WaveDataEnd) && ( RiffChunk.ChunkID != mmioFOURCC('s','m','p','l') ) )
	{
		// Go to next chunk.
		RiffChunkPtr = (FRiffChunkOld*) ( (BYTE*)RiffChunkPtr + Pad16Bit(RiffChunk.ChunkLen) + 8); 
		appMemcpy( &RiffChunk, RiffChunkPtr, sizeof(FRiffChunkOld) );
	} 

	// Chunk found ? smpl chunk is optional.
	// Find the first sample-loop structure, and the total number of them.
	if( (INT)RiffChunkPtr+4<(INT)WaveDataEnd && RiffChunk.ChunkID == mmioFOURCC('s','m','p','l') )
	{
		FSampleChunk pSampleChunk;
		appMemcpy(&pSampleChunk, (BYTE*)RiffChunkPtr + 8, sizeof(FSampleChunk));
		SampleLoopsNum = pSampleChunk.cSampleLoops;
		pSampleLoop = (FSampleLoop*) ((BYTE*)RiffChunkPtr + 8 + sizeof(FSampleChunk)); 
		/*
		FSampleChunk* pSampleChunk =  (FSampleChunk*)( (BYTE*)RiffChunk + 8);
		SampleLoopsNum  = pSampleChunk->cSampleLoops; // Number of tSampleLoop structures.
		// First tSampleLoop structure starts right after the tSampleChunk.
		pSampleLoop = (FSampleLoop*) ((BYTE*)pSampleChunk + sizeof(FSampleChunk)); 
		*/		
	}

#else
	
	FRiffChunkOld* RiffChunk = (FRiffChunkOld*)&WavData(3*4);
	// Look for the 'fmt ' chunk.
	while( ( ((BYTE*)RiffChunk + 8) < WaveDataEnd)  && ( RiffChunk->ChunkID != mmioFOURCC('f','m','t',' ') ) )
	{
		// Go to next chunk.
		RiffChunk = (FRiffChunkOld*) ( (BYTE*)RiffChunk + Pad16Bit(RiffChunk->ChunkLen) + 8); 
	}
	// Chunk found ?
	if( RiffChunk->ChunkID != mmioFOURCC('f','m','t',' ') )
		return 0;

	FmtChunk = (FFormatChunk*)((BYTE*)RiffChunk + 8);
	pBitsPerSample  = &FmtChunk->wBitsPerSample;
	pSamplesPerSec  = &FmtChunk->nSamplesPerSec;
	pAvgBytesPerSec = &FmtChunk->nAvgBytesPerSec;
	pBlockAlign		= &FmtChunk->nBlockAlign;
	pChannels       = &FmtChunk->nChannels;

	//GWarn->Logf( TEXT("look for data chunk") );
	// re-initalize the RiffChunk pointer
	RiffChunk = (FRiffChunkOld*)&WavData(3*4);
	// Look for the 'data' chunk.
	while( ( ((BYTE*)RiffChunk + 8) < WaveDataEnd) && ( RiffChunk->ChunkID != mmioFOURCC('d','a','t','a') ) )
	{
		// Go to next chunk.
		RiffChunk = (FRiffChunkOld*) ( (BYTE*)RiffChunk + Pad16Bit(RiffChunk->ChunkLen) + 8); 
	} 
	// Chunk found ?
	if( RiffChunk->ChunkID != mmioFOURCC('d','a','t','a') )
		return 0;

	SampleDataStart = (BYTE*)RiffChunk + 8;
	pWaveDataSize   = &RiffChunk->ChunkLen;
	SampleDataSize  =  RiffChunk->ChunkLen;
	OldBitsPerSample = FmtChunk->wBitsPerSample;
	SampleDataEnd   =  SampleDataStart+SampleDataSize;

	NewDataSize	= SampleDataSize;

	//GWarn->Logf( TEXT("look for smpl chunk 0x%x"), RiffChunk );
	// Re-initalize the RiffChunk pointer
	RiffChunk = (FRiffChunkOld*)&WavData(3*4);
	// Look for a 'smpl' chunk.
	while( ( (((BYTE*)RiffChunk) + 8) < WaveDataEnd) && ( RiffChunk->ChunkID != mmioFOURCC('s','m','p','l') ) )
	{
		// Go to next chunk.
		//GWarn->Logf( TEXT("go to next\n") );
		RiffChunk = (FRiffChunkOld*) ( (BYTE*)RiffChunk + Pad16Bit(RiffChunk->ChunkLen) + 8); 
		//GWarn->Logf( TEXT("riff: 0x%x\n"), RiffChunk );
		//GWarn->Logf( TEXT("%i"), RiffChunk->ChunkID );
	} 

	// Chunk found ? smpl chunk is optional.
	// Find the first sample-loop structure, and the total number of them.
	// GWarn->Logf( TEXT("find sample-loop 0x%x"), RiffChunk );
	if( (INT)RiffChunk+4<(INT)WaveDataEnd && RiffChunk->ChunkID == mmioFOURCC('s','m','p','l') )
	{
		//GWarn->Logf( TEXT("loop") );
		FSampleChunk pSampleChunk;
		appMemcpy(&pSampleChunk, (BYTE*)RiffChunk + 8, sizeof(FSampleChunk));
		SampleLoopsNum = pSampleChunk.cSampleLoops;
		pSampleLoop = (FSampleLoop*) ((BYTE*)RiffChunk + 8 + sizeof(FSampleChunk)); 
		/*
		FSampleChunk* pSampleChunk =  (FSampleChunk*)( (BYTE*)RiffChunk + 8);
		SampleLoopsNum  = pSampleChunk->cSampleLoops; // Number of tSampleLoop structures.
		// First tSampleLoop structure starts right after the tSampleChunk.
		pSampleLoop = (FSampleLoop*) ((BYTE*)pSampleChunk + sizeof(FSampleChunk)); 
		*/
	}
#endif
	
	return 1;
	unguard;
}

//
// Update internal variables and shrink the data fields.
//
UBOOL FWaveModInfo::UpdateWaveData( TArray<BYTE>& WavData )
{
	guard(FWaveModInfo::UpdateWaveData);
	if( NewDataSize < SampleDataSize )
	{		
		// Shrinkage of data chunk in bytes -> chunk data must always remain 16-bit-padded.
		INT ChunkShrinkage = Pad16Bit(SampleDataSize)  - Pad16Bit(NewDataSize);

		// Update sizes.
		*pWaveDataSize  = NewDataSize;
		*pMasterSize   -= ChunkShrinkage;

		// Refresh all wave parameters depending on bit depth and/or sample rate.
		*pBlockAlign    =  *pChannels * (*pBitsPerSample >> 3); // channels * Bytes per sample
		*pAvgBytesPerSec = *pBlockAlign * *pSamplesPerSec; //sample rate * Block align

		// Update 'smpl' chunk data also, if present.
		if (SampleLoopsNum)
		{
			FSampleLoop* pTempSampleLoop = pSampleLoop;
			INT SampleDivisor = ( (SampleDataSize *  *pBitsPerSample) / (NewDataSize ) );
			for (INT SL = 0; SL<SampleLoopsNum; SL++)
			{
				pTempSampleLoop->dwStart = pTempSampleLoop->dwStart  * OldBitsPerSample / SampleDivisor;
				pTempSampleLoop->dwEnd   = pTempSampleLoop->dwEnd  * OldBitsPerSample / SampleDivisor;
				pTempSampleLoop++; // Next TempSampleLoop structure.
			}	
		}		
			
		// Now shuffle back all data after wave data by SampleDataSize/2 (+ padding) bytes 
		// INT SampleShrinkage = ( SampleDataSize/4) * 2;
		BYTE* NewWaveDataEnd = SampleDataEnd - ChunkShrinkage;

		for ( INT pt = 0; pt< ( WaveDataEnd -  SampleDataEnd); pt++ )
		{ 
			NewWaveDataEnd[pt] =  SampleDataEnd[pt];
		}			

		// Resize the dynamic array.
		WavData.Remove( WavData.Num() - ChunkShrinkage, ChunkShrinkage );
		
		/*
		static INT SavedBytes = 0;
		SavedBytes += ChunkShrinkage;
		debugf(NAME_Log," Audio shrunk by: %i bytes, total savings %i bytes.",ChunkShrinkage,SavedBytes);
		debugf(NAME_Log," New BitsPerSample: %i ", *pBitsPerSample);
		debugf(NAME_Log," New SamplesPerSec: %i ", *pSamplesPerSec);
		debugf(NAME_Log," Olddata/NEW*wav* sizes: %i %i ", SampleDataSize, *pMasterSize);
		*/
	}

	// Noise gate filtering assumes 8-bit sound.
	// Warning: While very useful on SOME sounds, it erased too many low-volume sound fx
	// in its current form - even when 'noise' level scaled to average sound amplitude.
	// if (NoiseGate) NoiseGateFilter();

	return 1;
	unguard;
}

//
// Reduce bit depth and halve the number of samples simultaneously.
//
void FWaveModInfo::HalveReduce16to8()
{
	guard(FWaveModInfo::HalveReduce16to8);

	DWORD SampleWords =  SampleDataSize >> 1;
	INT OrigValue,NewValue;
	INT ErrorDiff = 0;

	DWORD SampleBytes = SampleWords >> 1;

	INT NextSample0 = (INT)((SWORD*) SampleDataStart)[0];
	INT NextSample1,NextSample2;

	BYTE* SampleData =  SampleDataStart;
	for (DWORD T=0; T<SampleBytes; T++)
	{
		NextSample1 = (INT)((SWORD*)SampleData)[T*2];
		NextSample2 = (INT)((SWORD*)SampleData)[T*2+1];
		INT Filtered16BitSample = 32768*4 + NextSample0 + NextSample1 + NextSample1 + NextSample2;
		NextSample0 = NextSample2;

		// Error diffusion works in '18 bit' resolution.
		OrigValue = ErrorDiff + Filtered16BitSample;
		// Rounding: "+0.5"
		NewValue  = (OrigValue + 512) & 0xfffffc00;
		if (NewValue > 0x3fc00) NewValue = 0x3fc00;

		INT NewSample = NewValue >> (8+2);
		SampleData[T] = (BYTE)NewSample;	// Output byte.

		ErrorDiff = OrigValue - NewValue;   // Error cycles back into input.
	}

	NewDataSize = SampleBytes;

	*pBitsPerSample  = 8;
	*pSamplesPerSec  = *pSamplesPerSec >> 1;

	NoiseGate = true;
	unguard;
}

//
// Reduce bit depth.
//
void FWaveModInfo::Reduce16to8()
{
	guard(FWaveModInfo::Reduce16to8);

	DWORD SampleBytes =  SampleDataSize >> 1;
	INT OrigValue,NewValue;
	INT ErrorDiff = 0;
	BYTE* SampleData = SampleDataStart;

	for (DWORD T=0; T<SampleBytes; T++)
	{
		// Error diffusion works in 16 bit resolution.
		OrigValue = ErrorDiff + 32768 + (INT)((SWORD*)SampleData)[T];
		// Rounding: '+0.5', then mask off low 2 bits.
		NewValue  = (OrigValue + 127 ) & 0xffffff00;  // + 128
		if (NewValue > 0xff00) NewValue = 0xff00;

		INT NewSample = NewValue >> 8;
		SampleData[T] = NewSample;

		ErrorDiff = OrigValue - NewValue;  // Error cycles back into input.
	}				

	NewDataSize = SampleBytes;
	*pBitsPerSample  = 8;
	NoiseGate = true;

	unguard;
}

//
// Halve the number of samples.
//
void FWaveModInfo::HalveData()
{
	guard(FWaveModInfo::HalveData);
	if( *pBitsPerSample == 16)
	{						
		DWORD SampleWords =  SampleDataSize >> 1;
		INT OrigValue,NewValue;
		INT ErrorDiff = 0;

		DWORD ScaledSampleWords = SampleWords >> 1; 

		INT NextSample0 = (INT)((SWORD*) SampleDataStart)[0];
		INT NextSample1, NextSample2;

		BYTE* SampleData =  SampleDataStart;
		for (DWORD T=0; T<ScaledSampleWords; T++)
		{
			NextSample1 = (INT)((SWORD*)SampleData)[T*2];
			NextSample2 = (INT)((SWORD*)SampleData)[T*2+1];
			INT Filtered18BitSample = 32768*4 + NextSample0 + NextSample1 + NextSample1 + NextSample2;
			NextSample0 = NextSample2;

			// Error diffusion works with '18 bit' resolution.
			OrigValue = ErrorDiff + Filtered18BitSample;
			// Rounding: '+0.5', then mask off low 2 bits.
			NewValue  = (OrigValue + 2) & 0x3fffc;
			if (NewValue > 0x3fffc) NewValue = 0x3fffc;
			((SWORD*)SampleData)[T] = (NewValue >> 2) - 32768;  // Output SWORD.
			ErrorDiff = OrigValue - NewValue; // Error cycles back into input.
		}				
		NewDataSize = (ScaledSampleWords * 2);
		*pSamplesPerSec  = *pSamplesPerSec >> 1;
	}
	else if( *pBitsPerSample == 8 )
	{									
		INT OrigValue,NewValue;
		INT ErrorDiff = 0;
	
		DWORD SampleBytes = SampleDataSize >> 1;  
		BYTE* SampleData = SampleDataStart;

		INT NextSample0 = SampleData[0];
		INT NextSample1, NextSample2;

		for (DWORD T=0; T<SampleBytes; T++)
		{
			NextSample1 =  SampleData[T*2];
			NextSample2 =  SampleData[T*2+1];
			INT Filtered10BitSample =  NextSample0 + NextSample1 + NextSample1 + NextSample2;
			NextSample0 =  NextSample2;

			// Error diffusion works with '10 bit' resolution.
			OrigValue = ErrorDiff + Filtered10BitSample;
			// Rounding: '+0.5', then mask off low 2 bits.
			NewValue  = (OrigValue + 2) & 0x3fc;
			if (NewValue > 0x3fc) NewValue = 0x3fc;
			SampleData[T] = (BYTE)(NewValue >> 2);	// Output BYTE.
			ErrorDiff = OrigValue - NewValue;		// Error cycles back into input.
		}				
		NewDataSize = SampleBytes; 
		*pSamplesPerSec  = *pSamplesPerSec >> 1;
	}
	unguard;
}

//
//	Noise gate filter. Hard to make general-purpose without hacking up some (soft) sounds.
//
void FWaveModInfo::NoiseGateFilter()
{
	guard(FWaveModInfo::NoiseGateFilter);

	BYTE* SampleData  =  SampleDataStart;
	INT   SampleBytes = *pWaveDataSize; 
	INT	  MinBlankSize = 860 * ((*pSamplesPerSec)/11025); // 600-800...

	// Threshold sound amplitude. About 18 seems OK.
	INT Amplitude	 = 18;

	// Ignore any over-threshold signals if under this size. ( 32 ?)
	INT GlitchSize	 = 32 * ((*pSamplesPerSec)/11025);
	INT StartSilence =  0;
	INT EndSilence	 =  0;
	INT LastErased   = -1;

	for( INT T = 0; T< SampleBytes; T++ )
	{
		UBOOL Loud;
		if	( Abs(SampleData[T]-128) >= Amplitude )
		{
			Loud = true;							
			if (StartSilence > 0)
			{
				if ( (T-StartSilence) < GlitchSize ) Loud = false;
			}							
		}
		else Loud = false;

		if( StartSilence == 0 )
		{
			if( !Loud )
				StartSilence = T;
		}
		else
		{													
			if( ((EndSilence == 0) && (Loud)) || (T ==(SampleBytes-1) ) )
			{
				EndSilence = T;
				//
				// Erase an area of low-amplitude sound ( noise... ) if size >= MinBlankSize.
				//
				// Todo: try erasing smoothly - decay, create some attack, 
				// proportional to the size of the area. ?
				//
				if	(( EndSilence - StartSilence) >= MinBlankSize )
				{					
					for ( INT Er = StartSilence; Er< EndSilence; Er++ )
					{
						SampleData[Er] = 128;
					}
				}
				LastErased = EndSilence-1;
				StartSilence = 0;
				EndSilence   = 0;
			}
		}										
	}
	unguard;
}

/*-----------------------------------------------------------------------------
	UAudioSubsystem implementation.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(UAudioSubsystem);


/*-----------------------------------------------------------------------------
	UI3DL2Listener implementation.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(UI3DL2Listener);
void UI3DL2Listener::PostEditChange()
{
	Super::PostEditChange();
	Updated = true;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

