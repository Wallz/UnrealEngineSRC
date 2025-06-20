/*=============================================================================
	Audio.h: Unreal audio public header file.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#ifndef _INC_AUDIO
#define _INC_AUDIO

/*----------------------------------------------------------------------------
	API.
----------------------------------------------------------------------------*/

#ifndef AUDIO_API
	#define AUDIO_API DLL_IMPORT
#endif

/*-----------------------------------------------------------------------------
	Dependencies.
-----------------------------------------------------------------------------*/
#include <dolphin.h>
#include <dolphin/ax.h>
#ifndef EMU
#include <dolphin/mix.h>
#endif

#include "Engine.h"
#include "UnRender.h"

/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Audio compiler specific includes.
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Audio public includes.
-----------------------------------------------------------------------------*/

//#include "AudioLibrary.h"
//#include "AudioMixer.h"
//#include "AudioCore.h"
#include "AudioSubsystem.h"
//#include "FormatWAV.h"

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
#endif
