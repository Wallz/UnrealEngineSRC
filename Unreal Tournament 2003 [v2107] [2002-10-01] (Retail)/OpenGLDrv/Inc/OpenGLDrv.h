/*=============================================================================
	OpenGLDrv.h: Unreal OpenGL support header.
	Copyright 2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Daniel Vogel
		
=============================================================================*/

/*-----------------------------------------------------------------------------
	Includes.
-----------------------------------------------------------------------------*/
#ifndef HEADER_OPENGLDRV
#define HEADER_OPENGLDRV

#ifdef WIN32
#include <windows.h>
#else
#include "SDL.h"
#endif

#include "GL/gl.h"
#include "glext.h"

#ifdef WIN32
#include "wglext.h"
#endif

#include "Engine.h"

#include "OpenGLResource.h"
#include "OpenGLRenderInterface.h"
#include "OpenGLRenderDevice.h"

#endif

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
