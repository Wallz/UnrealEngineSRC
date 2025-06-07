/*=============================================================================
	UnAudio.h: Unreal base audio.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
		* Wave modification code by Erik de Neve
=============================================================================*/

class USound;
class UStreamingSound;
class UAudioSubsystem;

/*-----------------------------------------------------------------------------
	Special Sony VAG Includes.
-----------------------------------------------------------------------------*/

#define HAVE_VAG 0 
#if HAVE_VAG
	#include "ENCVAG.h"
#endif

/*-----------------------------------------------------------------------------
	UAudioSubsystem.
-----------------------------------------------------------------------------*/

enum ESoundFlags
{
	SF_None				= 0,
	// reserve = 1 for special case
	SF_Looping			= 2,
	SF_Streaming		= 4,
	SF_Music			= 8,
	SF_No3D				= 16,
	SF_UpdatePitch		= 32,
	SF_NoUpdates		= 64,
	SF_RootMotion		= 128,
};

enum EFadeMode
{
	FADE_None			= 0,
	FADE_In,
	FADE_Out
};

//
// UAudioSubsystem is the abstract base class of
// the game's audio subsystem.
//
class ENGINE_API UAudioSubsystem : public USubsystem
{
	DECLARE_ABSTRACT_CLASS(UAudioSubsystem,USubsystem,CLASS_Config,Engine)
	NO_DEFAULT_CONSTRUCTOR(UAudioSubsystem)

	// UAudioSubsystem interface.
	virtual UBOOL Init()=0;
	virtual void SetViewport( UViewport* Viewport )=0;
	virtual UBOOL Exec( const TCHAR* Cmd, FOutputDevice& Ar=*GLog )=0;
	virtual void Update( FSceneNode* SceneNode )=0;
	virtual void RegisterSound( USound* Sound )=0;
	virtual void UnregisterSound( USound* Sound )=0;
	virtual UBOOL PlaySound( AActor* Actor, INT Id, USound* Sound, FVector Location, FLOAT Volume, FLOAT Radius, FLOAT Pitch, INT Flags, FLOAT FadeDuration, FLOAT Priority = 0.f )=0;
	virtual UBOOL StopSound( AActor* Actor, USound* Sound )=0;

	virtual INT PlayMusic( FString Song, FLOAT FadeInTime )=0;
	virtual UBOOL StopMusic( INT SongHandle, FLOAT FadeOutTime )=0;
	virtual INT StopAllMusic( FLOAT FadeOutTime )=0;

	virtual void ChangeVoiceChatter( DWORD IpAddr, DWORD ControllerPort, UBOOL Add )=0;
	virtual void EnterVoiceChat()=0;
	virtual void LeaveVoiceChat()=0;

	virtual UBOOL LowQualitySound()=0;

	virtual void NoteDestroy( AActor* Actor )=0;
	virtual UViewport* GetViewport()=0;
	virtual void CleanUp() {};
    virtual void Render( int flags ) = 0; // sjs
};


/*-----------------------------------------------------------------------------
	USound.
-----------------------------------------------------------------------------*/

//
// Sound data.
//
class ENGINE_API FSoundData : public TLazyArray<BYTE>
{
public:
	USound* Owner;
	void Load();
	FLOAT GetPeriod();
	FSoundData( USound* InOwner )
	: Owner( InOwner )
	{}
};

// gam ---

//
// A sound effect.
//
class ENGINE_API USound : public UObject
{
	DECLARE_CLASS(USound,UObject,CLASS_SafeReplace,Engine)

    public:

        // Constructor.
        USound();
        USound( const TCHAR* InFilename, INT InFlags ); // sjs - per vogel

        virtual USound* RenderSoundPlay( FLOAT *Volume, FLOAT *Pitch );

        virtual bool IsValid();
        virtual FSoundData &GetData();
        virtual FName GetFileType();
        virtual void SetFileType (FName FileType);
        virtual const TCHAR* GetFilename();
        virtual INT GetOriginalSize();
        virtual INT GetHandle();
        virtual void SetHandle(INT Handle);
        virtual INT GetFlags();
        virtual FLOAT GetDuration();
        virtual FLOAT GetRadius(); // sjs
        virtual FLOAT GetVelocityScale(); // gam
        virtual void SetDuration(FLOAT Duration);
        virtual void PS2Convert();
        virtual void Load() { GetData().Load(); } //amb

        // UObject interface.
        void Serialize( FArchive& Ar );
        void Destroy();
        void PostLoad();

        friend class FSoundData;

        static      UAudioSubsystem *Audio;

    FLOAT       Likelihood; // Used in a USoundGroup to distribute random sounds.

	protected:

	// Variables.
	FSoundData	Data;
	FName		FileType;
	FString		Filename;
	INT			OriginalSize;
	FLOAT       Duration;
	INT			Handle;
	INT			Flags;
    FLOAT       BaseRadius; // sjs
    FLOAT       VelocityScale; // gam
};

//
// A modified and dynamic instance of a sound.
//
class ENGINE_API UProceduralSound : public USound
{
    DECLARE_CLASS(UProceduralSound,USound,CLASS_SafeReplace,Engine)
   
    public:


        // Constructor.
        UProceduralSound();

        // USound interface.
        USound* RenderSoundPlay( FLOAT *Volume, FLOAT *Pitch );

        bool IsValid();
        FSoundData &GetData();
        FName GetFileType();
        void SetFileType (FName FileType);
        const TCHAR* GetFilename();
        INT GetOriginalSize();
        INT GetHandle();
        void SetHandle(INT Handle);
        INT GetFlags();
        FLOAT GetDuration();
        FLOAT GetRadius(); // sjs
        FLOAT GetVelocityScale(); // gam
        void SetDuration(FLOAT Duration);
        void PS2Convert();
        void Load(); // gam

        // UObject interface.
        void Serialize( FArchive& Ar );
        void Destroy();
        void PostLoad();
        void PostEditChange();

    protected:

        USound *BaseSound;

        // Modifications and Variances are in % and are bounded by [-100, +oo]
        FLOAT PitchModification;
        FLOAT VolumeModification;

        FLOAT PitchVariance; 
        FLOAT VolumeVariance;

        FLOAT RenderedPitchModification;
        FLOAT RenderedVolumeModification;
	};

//
// A group of sounds.
//
class ENGINE_API USoundGroup : public USound
{
    DECLARE_CLASS(USoundGroup,USound,CLASS_SafeReplace,Engine)

    public:

        USoundGroup();
        void RefreshGroup( UBOOL Force = 0 );

        // USound interface.
        USound* RenderSoundPlay( FLOAT *Volume, FLOAT *Pitch );

        bool IsValid();
        FSoundData &GetData();
        FName GetFileType();
        void SetFileType (FName FileType);
        const TCHAR* GetFilename();
        INT GetOriginalSize();
        INT GetHandle();
        void SetHandle(INT Handle);
        INT GetFlags();
        FLOAT GetDuration();
        FLOAT GetRadius(); // sjs
        FLOAT GetVelocityScale(); // gam
        void SetDuration(FLOAT Duration);
        void PS2Convert();
        void Load(); //amb

	// UObject interface.
        void Serialize( FArchive& Ar );
        void Destroy();
        void PostLoad();
        void PostEditChange();

        FStringNoInit Package; // deprecated

    protected:

        void RefreshGroup( const FString &Package ); // OBSOLETE!

        TArrayNoInit<USound*> Sounds;
        FLOAT TotalLikelihood;
        USound *RenderedSound;
};

// --- gam

/*-----------------------------------------------------------------------------
	FWaveModInfo. 
-----------------------------------------------------------------------------*/

//  Macros to convert 4 bytes to a Riff-style ID DWORD.
//  Todo: make these endian independent !!!

#undef MAKEFOURCC

#define MAKEFOURCC(ch0, ch1, ch2, ch3)\
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |\
    ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

#define mmioFOURCC(ch0, ch1, ch2, ch3)\
    MAKEFOURCC(ch0, ch1, ch2, ch3)

// Main Riff-Wave header.
struct FRiffWaveHeader
{ 
	DWORD	rID;			// Contains 'RIFF'
	DWORD	ChunkLen;		// Remaining length of the entire riff chunk (= file).
	DWORD	wID;			// Form type. Contains 'WAVE' for .wav files.
};

// General chunk header format.
struct FRiffChunkOld
{
	DWORD	ChunkID;		  // General data chunk ID like 'data', or 'fmt ' 
	DWORD	ChunkLen;		  // Length of the rest of this chunk in bytes.
};

// ChunkID: 'fmt ' ("WaveFormatEx" structure ) 
struct FFormatChunk
{
    _WORD   wFormatTag;        // Format type: 1 = PCM
    _WORD   nChannels;         // Number of channels (i.e. mono, stereo...).
    DWORD   nSamplesPerSec;    // Sample rate. 44100 or 22050 or 11025  Hz.
    DWORD   nAvgBytesPerSec;   // For buffer estimation  = sample rate * BlockAlign.
    _WORD   nBlockAlign;       // Block size of data = Channels times BYTES per sample.
    _WORD   wBitsPerSample;    // Number of bits per sample of mono data.
    _WORD   cbSize;            // The count in bytes of the size of extra information (after cbSize).
};

// ChunkID: 'smpl'
struct FSampleChunk
{
	DWORD   dwManufacturer;
	DWORD   dwProduct;
	DWORD   dwSamplePeriod;
	DWORD   dwMIDIUnityNote;
	DWORD   dwMIDIPitchFraction;
	DWORD	dwSMPTEFormat;		
	DWORD   dwSMPTEOffset;		//
	DWORD   cSampleLoops;		// Number of tSampleLoop structures following this chunk
	DWORD   cbSamplerData;		// 
};
 
struct FSampleLoop				// Immediately following cbSamplerData in the SMPL chunk.
{
	DWORD	dwIdentifier;		//
	DWORD	dwType;				//
	DWORD	dwStart;			// Startpoint of the loop in samples
	DWORD	dwEnd;				// Endpoint of the loop in samples
	DWORD	dwFraction;			// Fractional sample adjustment
	DWORD	dwPlayCount;		// Play count
};

//
// Structure for in-memory interpretation and modification of WAVE sound structures.
//
class ENGINE_API FWaveModInfo
{
public:

	// Pointers to variables in the in-memory WAVE file.
	DWORD* pSamplesPerSec;
	DWORD* pAvgBytesPerSec;
	_WORD* pBlockAlign;
	_WORD* pBitsPerSample;
	_WORD* pChannels;

	DWORD  OldBitsPerSample;

	DWORD* pWaveDataSize;
	DWORD* pMasterSize;
	BYTE*  SampleDataStart;
	BYTE*  SampleDataEnd;
	DWORD  SampleDataSize;
	BYTE*  WaveDataEnd;

	INT	   SampleLoopsNum;
	FSampleLoop*  pSampleLoop;

	DWORD  NewDataSize;
	UBOOL  NoiseGate;

	// Constructor.
	FWaveModInfo()
	{
		NoiseGate   = false;
		SampleLoopsNum = 0;
	}
	
	// 16-bit padding.
	DWORD Pad16Bit( DWORD InDW )
	{
		return ((InDW + 1)& ~1);
	}

	// Read headers and load all info pointers in WaveModInfo. 
	// Returns 0 if invalid data encountered.
	// UBOOL ReadWaveInfo( TArray<BYTE>& WavData );
	UBOOL ReadWaveInfo( TArray<BYTE>& WavData );
	
	// Handle RESIZING and updating of all variables needed for the new size:
	// notably the (possibly multiple) loop structures.
	UBOOL UpdateWaveData( TArray<BYTE>& WavData);

	// Wave size and/or bitdepth reduction.
	void Reduce16to8();
	void HalveData();
	void HalveReduce16to8(); 

	// Filters.
	void NoiseGateFilter(); 
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

