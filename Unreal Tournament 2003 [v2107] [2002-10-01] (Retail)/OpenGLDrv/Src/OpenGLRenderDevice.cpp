/*=============================================================================
	OpenGLRenderDevice.cpp: Unreal OpenGL support.
	Copyright 2002 Epic Games, Inc. All Rights Reserved.
	
	Revision history:
	* Created by Daniel Vogel

=============================================================================*/

#include "OpenGLDrv.h"

/*-----------------------------------------------------------------------------
	Constants & static stuff.
-----------------------------------------------------------------------------*/

#ifdef WIN32
#define GL_DLL (TEXT("OpenGL32.dll"))
#endif

// Static variables.
INT				UOpenGLRenderDevice::NumDevices		= 0;
INT				UOpenGLRenderDevice::LockCount		= 0;
BYTE*			UOpenGLRenderDevice::VARPointer		= 0;
INT				UOpenGLRenderDevice::VARIndex		= 0;
INT				UOpenGLRenderDevice::VARSize		= 0;

#ifdef WIN32
HGLRC			UOpenGLRenderDevice::hCurrentRC		= NULL;
HMODULE			UOpenGLRenderDevice::hModuleGLMain	= NULL;
HMODULE			UOpenGLRenderDevice::hModuleGLGDI	= NULL;
#else
UBOOL			UOpenGLRenderDevice::GLLoaded		= 0;
#endif

// OpenGL function pointers.
#define GL_EXT(name) UBOOL UOpenGLRenderDevice::SUPPORTS##name=0;
#define GL_PROC(ext,ret,func,parms) ret (STDCALL *UOpenGLRenderDevice::func)parms;
#include "OpenGLFuncs.h"
#undef GL_EXT
#undef GL_PROC

/*-----------------------------------------------------------------------------
	OpenGLRenderDevice.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(UOpenGLRenderDevice);

//
// UOpenGLRenderDevice::UOpenGLRenderDevice
//
UOpenGLRenderDevice::UOpenGLRenderDevice() :
	RenderInterface(this)
{
	guard(UOpenGLRenderDevice::UOpenGLRenderDevice);
	
	LockedViewport		= NULL;
	IsVAR				= 0;

	unguard;
}

//
// UOpenGLRenderDevice::StaticConstructor
//
void UOpenGLRenderDevice::StaticConstructor()
{
	guard(UOpenGLRenderDevice::StaticConstructor);

	new(GetClass(),TEXT("DetailTexMipBias"),    RF_Public)UFloatProperty( CPP_PROPERTY(DetailTexMipBias	   ), TEXT("Options"), CPF_Config );
	new(GetClass(),TEXT("MaxTextureUnits"),     RF_Public)UIntProperty  ( CPP_PROPERTY(MaxTextureUnits	   ), TEXT("Options"), CPF_Config );
	new(GetClass(),TEXT("VARSize"),             RF_Public)UIntProperty  ( CPP_PROPERTY(ConfigVARSize	   ), TEXT("Options"), CPF_Config );
	new(GetClass(),TEXT("UseTrilinear"),        RF_Public)UBoolProperty ( CPP_PROPERTY(UseTrilinear        ), TEXT("Options"), CPF_Config );
	GIsOpenGL=1;
	
	SupportsCubemaps	= 1;
	SupportsZBIAS		= 1;

	unguard;
}


//
// UOpenGLRenderDevice::FindExt
//
UBOOL UOpenGLRenderDevice::FindExt( const TCHAR* Name )
{
	guard(UOpenGLRenderDevice::FindExt);
	
	// Can't use appStrFind as extension string might be > 1024 characters.
	UBOOL Result = strstr( (char*) glGetString(GL_EXTENSIONS), appToAnsi(Name) ) != NULL;
	if( Result )
		debugf( NAME_Init, TEXT("Device supports: %s"), Name );
	return Result;
	
	unguard;
}


//
// UOpenGLRenderDevice::FindProc
//
void UOpenGLRenderDevice::FindProc( void*& ProcAddress, char* Name, char* SupportName, UBOOL& Supports, UBOOL AllowExt )
{
	guard(UOpenGLRenderDevice::FindProc);

#ifdef __LINUX__
	if( !ProcAddress )
		ProcAddress = (void*) SDL_GL_GetProcAddress( Name );
#else
	if( !ProcAddress )
		ProcAddress = appGetDllExport( hModuleGLMain, appFromAnsi(Name) );
	if( !ProcAddress )
		ProcAddress = appGetDllExport( hModuleGLGDI, appFromAnsi(Name) );
	if( !ProcAddress && AllowExt && Supports )
		ProcAddress = wglGetProcAddress( Name );
	if( !ProcAddress && AllowExt && Supports )
	{
		char Dummy[1024];
		strcpy( Dummy, Name );
		strcat( Dummy, "EXT" );
		ProcAddress = wglGetProcAddress( Dummy );
	}
#endif

	if( !ProcAddress )
	{
		if( Supports )
			debugf( TEXT("   Missing function '%s' for '%s' support"), appFromAnsi(Name), appFromAnsi(SupportName) );
		Supports = 0;
	}

	unguard;
}


//
// UOpenGLRenderDevice::FindProcs
//
void UOpenGLRenderDevice::FindProcs( UBOOL AllowExt )
{
	guard(UOpenGLDriver::FindProcs);

	#define GL_EXT(name) if( AllowExt ) SUPPORTS##name = FindExt( TEXT(#name)+1 );
	#define GL_PROC(ext,ret,func,parms) FindProc( *(void**)&func, #func, #ext, SUPPORTS##ext, AllowExt );
	#include "OpenGLFuncs.h"
	#undef GL_EXT
	#undef GL_PROC

	unguard;
}


//
// UOpenGLRenderDevice::MakeCurrent
//
void UOpenGLRenderDevice::MakeCurrent()
{
	guard(UOpenGLRenderDevice::MakeCurrent);

#ifdef WIN32
	check(hRC);
	check(hDC);
	if( hCurrentRC!=hRC )
	{
		verify(wglMakeCurrent(hDC,hRC));
		hCurrentRC = hRC;
	}
#endif

	unguard;
}


//
// UOpenGLRenderDevice::GLError
//
void UOpenGLRenderDevice::GLError( TCHAR* Tag )
{
	guard(UOpenGLRenderDevice::GLError);

	GLenum Error;
	while( (Error=glGetError()) != GL_NO_ERROR )
	{
		const TCHAR* Msg;
		switch( Error )
		{
			case GL_INVALID_ENUM:
				Msg = TEXT("GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				Msg = TEXT("GL_INVALID_VALUE");
				break;
			case GL_INVALID_OPERATION:
				Msg = TEXT("GL_INVALID_OPERATION");
				break;
			case GL_STACK_OVERFLOW:
				Msg = TEXT("GL_STACK_OVERFLOW");
				break;
			case GL_STACK_UNDERFLOW:
				Msg = TEXT("GL_STACK_UNDERFLOW");
				break;
			case GL_OUT_OF_MEMORY:
				Msg = TEXT("GL_OUT_OF_MEMORY");
				break;
			default :
				Msg = TEXT("UNKNOWN");
		};
		debugf( TEXT("OpenGL Error: %s (%s)"), Msg, Tag );
	}

	unguard;
}


//
// UOpenGLRenderDevice::Init
//
UBOOL UOpenGLRenderDevice::Init()
{
	guard(UOpenGLRenderDevice::Init);

	debugf( NAME_Init, TEXT("Initializing OpenGLDrv...") );

#ifdef __LINUX__

	// Init global GL.
	if( NumDevices==0 )
	{
		// Bind the library.
		FString OpenGLLibName = TEXT("libGL.so.1");

		if ( !GLLoaded )
		{
			// Only call it once as succeeding calls will 'fail'.
			debugf( TEXT("binding %s"), *OpenGLLibName );
			if ( SDL_GL_LoadLibrary( appToAnsi(*OpenGLLibName) ) == -1 )
				appErrorf( appFromAnsi(SDL_GetError()) );
			GLLoaded = true;
		}

		SUPPORTS_GL = 1;
		FindProcs( 0 );
		if( !SUPPORTS_GL )
			appErrorf(TEXT("Missing symbols - aborting."));
	}

#else

	// Get list of device modes.
	for( INT i=0; ; i++ )
	{
#if UNICODE
		if( !GUnicodeOS )
		{
			DEVMODEA DisplayMode;
			appMemzero( &DisplayMode, sizeof(DisplayMode) );
			DisplayMode.dmSize = sizeof(DisplayMode);
			
			if( !EnumDisplaySettingsA( NULL, i, &DisplayMode) )
				break;
			
			Modes.AddUniqueItem( FPlane(	DisplayMode.dmPelsWidth, 
											DisplayMode.dmPelsHeight, 
											DisplayMode.dmBitsPerPel, 
											DisplayMode.dmDisplayFrequency
										));
		}
		else
#endif
		{
			DEVMODE DisplayMode;
			appMemzero( &DisplayMode, sizeof(DisplayMode) );
			DisplayMode.dmSize = sizeof(DisplayMode);
			
			if( !EnumDisplaySettings( NULL, i, &DisplayMode) )
				break;
			
			Modes.AddUniqueItem( FPlane(	DisplayMode.dmPelsWidth, 
											DisplayMode.dmPelsHeight, 
											DisplayMode.dmBitsPerPel, 
											DisplayMode.dmDisplayFrequency
										));
		}
	}

	// Init global GL.
	if( NumDevices==0 )
	{
		// Find DLL's.
		hModuleGLMain = (HMODULE) appGetDllHandle( GL_DLL );
		if( !hModuleGLMain )
		{
			debugf( NAME_Init, TEXT("Couldn't locate %s"), GL_DLL );
			return 0;
		}
		hModuleGLGDI = (HMODULE) appGetDllHandle( TEXT("GDI32.dll") );
		check(hModuleGLGDI);

		// Find functions.
		SUPPORTS_GL = 1;
		FindProcs( 0 );
		if( !SUPPORTS_GL )
			appErrorf(TEXT("Missing symbols - aborting."));
	}

#endif

	// Scratch buffer used by e.g. resource management.
	ScratchBuffer.Empty();
	ScratchBuffer.Add( 65536 );

	NumDevices++;

	return 1;
	unguard;
}


//
// UOpenGLRenderDevice::Exit
//
void UOpenGLRenderDevice::Exit(UViewport* Viewport)
{
	guard(UOpenGLRenderDevice::Exit);

	check(NumDevices>0);

	// Shut down RC.
	Flush( Viewport );

	UnSetRes();

	// UClient::Destroy is called before this gets called so hDC is invalid.
	// SetDeviceGammaRamp( GetDC( GetDesktopWindow() ), &OriginalRamp );
	RestoreGamma();

#ifdef WIN32
	// Shut down this GL context. May fail if window was already destroyed.
	if( hDC )
		ReleaseDC(hWnd,hDC);

	// Shut down global GL.
	if( --NumDevices==0 )
	{
		// Free modules.
		if( hModuleGLMain )
			appFreeDllHandle( hModuleGLMain );
		if( hModuleGLGDI )
			appFreeDllHandle( hModuleGLGDI );
	}
#endif

	unguard;
}


//
// UOpenGLRenderDevice::SetRes
//
UBOOL UOpenGLRenderDevice::SetRes(UViewport* Viewport,INT NewX,INT NewY,UBOOL Fullscreen,INT ColorBytes,UBOOL bSaveSize)
{
	guard(UOpenGLRenderDevice::SetRes);

	// Returning 0 causes infinite loops and I'd rather catch those bugs at the root.
	if( !GIsEditor && LockedViewport )
		appErrorf(TEXT("Can't change resolution while render device is locked!"));

	debugf(TEXT("Enter SetRes: %dx%d Fullscreen %d"), NewX, NewY, Fullscreen );

	switch( ColorBytes )
	{
	case 0:
		break;
	case 2:
		Use16bit = 1;
		break;
	case 4:
		Use16bit = 0;
	}
	ColorBytes = Use16bit ? 2 : 4;

	Flush( Viewport );

#ifdef __LINUX__

	UnSetRes();

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, ColorBytes<=2 ? 5 : 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, ColorBytes<=2 ? 5 : 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, ColorBytes<=2 ? 5 : 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, ColorBytes<=2 ? 16 : 24 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// Change window size.
	Viewport->ResizeViewport( Fullscreen ? (BLIT_Fullscreen|BLIT_OpenGL) : (BLIT_OpenGL), NewX, NewY );

#else

	// Get hWnd & hDC.
	hWnd		= (HWND) Viewport->GetWindow();
	check(hWnd);
	hDC			= GetDC( hWnd );
	check(hDC);

#if 0 
	// Print all PFD's exposed
	INT Count = DescribePixelFormat( hDC, 0, 0, NULL );
	for( i=1; i<Count; i++ )
		PrintFormat( hDC, i );
#endif

	// Exit res.
	if( hRC )
		UnSetRes();

	// Change display settings.
	if( Fullscreen )
	{
		INT FindX		= NewX, 
			FindY		= NewY, 
			BestError	= MAXINT;
		
		for( INT i=0; i<Modes.Num(); i++ )
		{
			if( Modes(i).Z == ColorBytes*8 )
			{
				INT Error
				=	(Modes(i).X-FindX)*(Modes(i).X-FindX)
				+	(Modes(i).Y-FindY)*(Modes(i).Y-FindY);
				if( Error < BestError )
				{
					NewX		= Modes(i).X;
					NewY		= Modes(i).Y;
					BestError	= Error;
				}
			}
		}

#if UNICODE
		if( !GUnicodeOS )
		{
			DEVMODEA DisplayMode;
			ZeroMemory( &DisplayMode, sizeof(DisplayMode) );
			DisplayMode.dmSize       = sizeof(DisplayMode);
			DisplayMode.dmPelsWidth  = NewX;
			DisplayMode.dmPelsHeight = NewY;
			DisplayMode.dmBitsPerPel = ColorBytes * 8;
#if 0
			if( RefreshRate )
			{
				DisplayMode.dmDisplayFrequency = RefreshRate;
				DisplayMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_BITSPERPEL;
			}
			else
#endif
			{
				DisplayMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
			}
			if( ChangeDisplaySettingsA( &DisplayMode, CDS_FULLSCREEN )!=DISP_CHANGE_SUCCESSFUL )
			{
				debugf( TEXT("ChangeDisplaySettingsA failed: %ix%ix%i"), NewX, NewY, ColorBytes );
				return 0;
			}
		}
		else
#endif
		{
			DEVMODE DisplayMode;
			ZeroMemory( &DisplayMode, sizeof(DisplayMode) );
			DisplayMode.dmSize       = sizeof(DisplayMode);
			DisplayMode.dmPelsWidth  = NewX;
			DisplayMode.dmPelsHeight = NewY;
			DisplayMode.dmBitsPerPel = ColorBytes * 8;
#if 0
			if ( RefreshRate )
			{
				DisplayMode.dmDisplayFrequency = RefreshRate;
				DisplayMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_BITSPERPEL;
			}
			else
#endif
			{
				DisplayMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
			}
			if( ChangeDisplaySettings( &DisplayMode, CDS_FULLSCREEN )!=DISP_CHANGE_SUCCESSFUL )
			{
				debugf( TEXT("ChangeDisplaySettings failed: %ix%i"), NewX, NewY );
				return 0;
			}
		}
	}

	// OpenGL renderer doesn't support stencil and can't run the Editor. 
	check( !GIsEditor );
	UseStencil = 0;

	// Set resolution.
	INT DesiredColorBits   = ColorBytes<=2 ? 16 : 32;
	INT DesiredStencilBits = 0;//ColorBytes<=2 ?  0 : 8;
	INT DesiredDepthBits   = ColorBytes<=2 ? 16 : 24;
	
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		DesiredColorBits,
		0,0,0,0,0,0,
		0,0,
		0,0,0,0,0,
		DesiredDepthBits,
		DesiredStencilBits,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};
	
	INT nPixelFormat = ChoosePixelFormat( hDC, &pfd );
	Parse( appCmdLine(), TEXT("PIXELFORMAT="), nPixelFormat );
	check(nPixelFormat);

	debugf( NAME_Init, TEXT("Using pixel format %i"), nPixelFormat );
	
	verify( SetPixelFormat( hDC, nPixelFormat, &pfd ) );
	
	hRC = wglCreateContext( hDC );
	check(hRC);

	// Change window size.
	UBOOL Result = Viewport->ResizeViewport( (Fullscreen ? BLIT_Fullscreen : 0) | BLIT_OpenGL, NewX, NewY );
	if( !Result )
	{
		if( Fullscreen )
			TCHAR_CALL_OS(ChangeDisplaySettings(NULL,0),ChangeDisplaySettingsA(NULL,0));
		return 0;
	}

#endif

	MakeCurrent();

	debugf( NAME_Init, TEXT("GL_VENDOR     : %s"), appFromAnsi((ANSICHAR*)glGetString(GL_VENDOR))	);
	debugf( NAME_Init, TEXT("GL_RENDERER   : %s"), appFromAnsi((ANSICHAR*)glGetString(GL_RENDERER)) );
	debugf( NAME_Init, TEXT("GL_VERSION    : %s"), appFromAnsi((ANSICHAR*)glGetString(GL_VERSION))	);

	FindProcs( 1 );

	INT DepthBits,
		StencilBits,
		RedBits,
		GreenBits,
		BlueBits,
		AlphaBits;

	glGetIntegerv( GL_DEPTH_BITS,	&DepthBits		);
	glGetIntegerv( GL_STENCIL_BITS,	&StencilBits	);
	glGetIntegerv( GL_RED_BITS,		&RedBits		);
	glGetIntegerv( GL_GREEN_BITS,	&GreenBits		);
	glGetIntegerv( GL_BLUE_BITS,	&BlueBits		);
	glGetIntegerv( GL_ALPHA_BITS,	&AlphaBits		 );


	debugf( NAME_Init, TEXT("C%i RGB%i%i%i Z%i S%i"), ColorBytes * 8, RedBits, GreenBits, BlueBits, DepthBits, StencilBits );

	// Check for old extension.
	SUPPORTS_GL_ATI_texture_env_combine3 |= SUPPORTS_GL_ATIX_texture_env_combine3;
	
	//!!TODO: it actually is faster to not use it and live with the implied memcpy.
	SUPPORTS_GL_ATI_map_object_buffer = 0;

	// NVIDIA doesn't expose the crossbar extension but supports it.
	SUPPORTS_GL_ARB_texture_env_crossbar |= SUPPORTS_GL_NV_texture_env_combine4;

	// Only enable VAR if VAR2 is exposed as well.
	SUPPORTS_GL_NV_vertex_array_range &= SUPPORTS_GL_NV_vertex_array_range2;

	// Disable usage if VAR size <= 0.
	if( ConfigVARSize <= 0 )
		SUPPORTS_GL_NV_vertex_array_range = 0;

	// Don't bother with texture conversion for OpenGL renderer.
	if( !SUPPORTS_GL_EXT_texture_compression_s3tc )
		appErrorf( TEXT("OpenGL renderer relies on DXTC/S3TC support.") );

	//!!TODO: check for OpenGL version >= 1.3.

	// Complain about lack of combine3/4 extensions.
	if( !SUPPORTS_GL_ATI_texture_env_combine3 && !SUPPORTS_GL_NV_texture_env_combine4 )
		debugf( NAME_Init, TEXT("OpenGL: WARNING: no support for combine3/4 extensions -> not all blend modes supported") );

	// Lame hack to ensure glDrawRangeElements is present.
	if( !SUPPORTS_GL_EXT_bgra )
		appErrorf(TEXT("GL_EXT_bgra not supported - bailing out."));

	// Set permanent state.
	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glDepthMask( GL_TRUE );
	glDepthFunc( GL_LEQUAL );
	glBlendFunc( GL_ONE, GL_ZERO );
	glEnable( GL_BLEND );
	glFogi( GL_FOG_MODE, GL_LINEAR );
	
	glEnable( GL_NORMALIZE );
	glEnable( GL_RESCALE_NORMAL );

	glEnable( GL_POLYGON_OFFSET_FILL );
	glDisable( GL_DITHER );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );

	glEnableClientState( GL_VERTEX_ARRAY );

	// Get number of texture units and clamp according to .ini setting.
	if( MaxTextureUnits < 1 )
		MaxTextureUnits = 8;

	glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &NumTextureUnits );
	NumTextureUnits = Min( NumTextureUnits, Clamp( MaxTextureUnits, 2, 8 ) );

	for( INT i=0; i<NumTextureUnits; i++ )
	{
		glActiveTextureARB( GL_TEXTURE0 + i );
		glDisable( GL_TEXTURE_2D );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
	}
	glActiveTextureARB( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );

	FLOAT ColorOne[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	FLOAT ColorZero[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE , (GLfloat*) ColorOne  );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT , (GLfloat*) ColorOne  );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) ColorZero );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat*) ColorZero );

	glColorMaterial( GL_FRONT_AND_BACK, GL_EMISSION );
	glEnable( GL_COLOR_MATERIAL );

	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
	
	for( INT i=0; i<8; i++ )
		glLightfv( GL_LIGHT0 + i, GL_SPECULAR, ColorZero );

	glViewport( 0, 0, NewX, NewY );
	
	appMemzero( HardwareState, sizeof( HardwareState) );
	
	// Create dummy white texture.
	FColor Data[8*8];
	for( INT i=0; i<ARRAY_COUNT(Data); i++ )
		Data[i] = FColor(0,0,0,0);
	glGenTextures( 1, &NullTextureID );
	glBindTexture( GL_TEXTURE_2D, NullTextureID );
	for( INT Level=0; 8>>Level; Level++ )
		glTexImage2D( GL_TEXTURE_2D, Level, 4, 8>>Level, 8>>Level, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data );

	// Vertex array range.
	VARPointer	= NULL;
	VARSize		= 0;
	VARIndex	= 0;
	if( SUPPORTS_GL_NV_vertex_array_range )
	{
		VARSize		= ConfigVARSize * 1024 * 1024;
		//!!vogel: TODO: find out how to get AGP memory.
#ifdef WIN32
		VARPointer	= (BYTE*) wglAllocateMemoryNV( VARSize, 0.f, 0.0f, 0.5f );
#else
		VARPointer	= (BYTE*) glXAllocateMemoryNV( VARSize, 0.f, 0.0f, 0.5f );
#endif
		if( VARPointer == NULL )
		{	
#ifdef _DEBUG
			check(VARPointer);
#else
			debugf( NAME_Init, TEXT("WARNING: Couldn't allocate AGP memory - turning off support for GL_NV_vertex_array_range"));
			debugf( NAME_Init, TEXT("WARNING: This has a serious impact on performance."));
			SUPPORTS_GL_NV_vertex_array_range = 0;
#endif
		}
	}
	SupportsCubemaps	= SUPPORTS_GL_ARB_texture_cube_map;
	SupportsZBIAS		= 1;

	UpdateGamma( Viewport );

	Viewport->PendingFrame = 0;

	return 1;

	unguard;
}


//
// UOpenGLRenderDevice::UnSetRes
//
void UOpenGLRenderDevice::UnSetRes()
{
	guard(UOpenGLRenderDevice::UnSetRes);

#ifdef WIN32
	if( hRC )
#endif
		glDeleteTextures( 1, &NullTextureID );

	// Vertex array range.
	if( SUPPORTS_GL_NV_vertex_array_range && VARPointer )
#ifdef WIN32
		wglFreeMemoryNV( VARPointer );
#else
		glXFreeMemoryNV( VARPointer );
#endif
	VARPointer	= NULL;
	VARSize		= 0;
	VARIndex	= 0;

#ifdef WIN32
	if( hRC )
	{
		hCurrentRC = NULL;
		verify(wglMakeCurrent( NULL, NULL ));
		verify(wglDeleteContext( hRC ));
		hRC = NULL;
	}
	//!!if( WasFullscreen )
	//!!	TCHAR_CALL_OS(ChangeDisplaySettings(NULL,0),ChangeDisplaySettingsA(NULL,0));
#endif

	unguard;
}


//
//	UOpenGLRenderDevice::GetCachedResource
//
FOpenGLResource* UOpenGLRenderDevice::GetCachedResource(QWORD CacheId)
{
	guard(UOpenGLRenderDevice::GetCachedResource);

	INT					HashIndex	= GetResourceHashIndex(CacheId);
	FOpenGLResource*	ResourcePtr = ResourceHash[HashIndex];

	while(ResourcePtr)
	{
		if(ResourcePtr->CacheId == CacheId)
			return ResourcePtr;

		ResourcePtr = ResourcePtr->HashNext;
	};

	return NULL;

	unguard;
}


//
//	UOpenGLRenderDevice::FlushResource
//
void UOpenGLRenderDevice::FlushResource(QWORD CacheId)
{
	guard(UOpenGLRenderDevice::GetCachedResource);

	FOpenGLResource*	CachedResource = GetCachedResource(CacheId);

	if(CachedResource)
		delete CachedResource;

	unguard;
}


//
//	UOpenGLRenderDevice::GetVertexShader
//
FOpenGLVertexShader* UOpenGLRenderDevice::GetVertexShader(EVertexShader Type,FShaderDeclaration& Declaration)
{
	guard(UOpenGLRenderDevice::GetVertexShader);

	// Find an existing vertex shader with the same type/declaration.
	FOpenGLVertexShader*	ShaderPtr = VertexShaders;

	while(ShaderPtr)
	{
		if(ShaderPtr->Type == Type && ShaderPtr->Declaration == Declaration)
			return ShaderPtr;

		ShaderPtr = ShaderPtr->NextVertexShader;
	};

	// Create a new vertex shader.
	if(Type == VS_FixedFunction)
		return new FOpenGLFixedVertexShader(this,Declaration);
	else
		return NULL;

	unguard;
}


//
// UOpenGLRenderDevice::ResourceCached
//
UBOOL UOpenGLRenderDevice::ResourceCached(QWORD CacheId)
{
	 FOpenGLResource*	Resource = GetCachedResource(CacheId);

	 if(!Resource)
		 return 0;

#if 0
	 FOpenGLTexture*	Texture = Resource->GetTexture();

	 if(!Texture)
		 return 1;

	 if(Texture->Direct3DTexture8 || Texture->Direct3DCubeTexture8)
		 return 1;
#else
	 return 1;
#endif

	 return 0;
}


//
// UOpenGLRenderDevice::Flush
//
void UOpenGLRenderDevice::Flush(UViewport* Viewport)
{
	guard(UOpenGLRenderDevice::Flush);

	DynamicVertexStream	= NULL;
	DynamicIndexBuffer	= NULL;

	guard(ResourceList);
	while(ResourceList)
		delete ResourceList;
	unguard;

	guard(VertexShaders);
	while(VertexShaders)
		delete VertexShaders;
	unguard;

	PrecacheOnFlip = 1;//UsePrecaching;

	unguard;
}


//
// UOpenGLRenderDevice::UpdateGamma
//
void UOpenGLRenderDevice::UpdateGamma(UViewport* Viewport)
{
	guard(UOpenGLRenderDevice::UpdateGamma);

	FLOAT	Gamma		= Viewport->GetOuterUClient()->Gamma,
			Brightness	= Viewport->GetOuterUClient()->Brightness,
			Contrast	= Viewport->GetOuterUClient()->Contrast;

	struct
	{
		_WORD red[256];
		_WORD green[256];
		_WORD blue[256];
	} Ramp;

	for(INT i=0; i<256; i++)
		Ramp.red[i] = Ramp.green[i] = Ramp.blue[i] = Clamp<INT>( appRound( (Contrast+0.5f)*appPow(i/255.f,1.0f/Gamma)*65535.f + (Brightness-0.5f)*32768.f - Contrast*32768.f + 16384.f ), 0, 65535 );

#ifdef __LINUX__
	SDL_SetGammaRamp( Ramp.red, Ramp.green, Ramp.blue );
    // (this commented code is a fallback if SDL_SetGammaRamp() is hosed.)
	//FLOAT G = Brightness + Gamma;
	//SDL_SetGamma( G, G, G );
#else
	SetDeviceGammaRamp( hDC, &Ramp );
#endif	    

	unguard;
}


//
// UOpenGLRenderDevice::RestoreGamma
//
void UOpenGLRenderDevice::RestoreGamma()
{
	guard(UOpenGLRenderDevice::RestoreGamma);

	struct
	{
		_WORD red[256];
		_WORD green[256];
		_WORD blue[256];
	} Ramp;

	for(INT i=0; i<256; i++)
		Ramp.red[i] = Ramp.green[i] = Ramp.blue[i] = i << 8;

#ifdef __LINUX__
	SDL_SetGammaRamp( Ramp.red, Ramp.green, Ramp.blue );
    // If SDL_SetGammaRamp is buggy, try this:
	//SDL_SetGamma( 1, 1, 1 );
#else
	SetDeviceGammaRamp( GetDC( GetDesktopWindow() ), &Ramp );
#endif	    

	unguard;
}


//
// UOpenGLRenderDevice::Exec
//
UBOOL UOpenGLRenderDevice::Exec(const TCHAR* Cmd,FOutputDevice& Ar)
{
	guard(UOpenGLRenderDevice::Exec);
	
	if( ParseCommand(&Cmd,TEXT("RESOURCES") ))
	{
		// Calculate resource usage
		INT StatTextureBytes=0, StatVertexStreamBytes=0, StatIndexBufferBytes=0, StatOtherBytes=0;
		INT StatNumTextures=0, StatNumVertexStreams=0, StatNumIndexBuffers=0, StatNumOther=0;
		for( FOpenGLResource* Resource = ResourceList;Resource;Resource = Resource->NextResource )
		{
			if(Resource->GetTexture())
			{
				StatTextureBytes += Resource->CalculateFootprint();
				StatNumTextures++;
			}
			else if(Resource->GetVertexStream())
			{
				StatVertexStreamBytes += Resource->CalculateFootprint();
				StatNumVertexStreams++;
			}
			else if(Resource->GetIndexBuffer())
			{
				StatIndexBufferBytes += Resource->CalculateFootprint();
				StatNumIndexBuffers++;
			}
			else
			{
				StatOtherBytes += Resource->CalculateFootprint();
				StatNumOther++;
			}
		}

		Ar.Logf( TEXT("OpenGL Resource Usage"));
		Ar.Logf( TEXT(""));
		Ar.Logf( TEXT("Resource Type      Count   Total Bytes"));
		Ar.Logf( TEXT("--------------------------------------"));
		Ar.Logf( TEXT("Textures            %4d      %8d"), StatNumTextures, StatTextureBytes );
		Ar.Logf( TEXT("Vertex Streams      %4d      %8d"), StatNumVertexStreams, StatVertexStreamBytes );
		Ar.Logf( TEXT("Index Buffers       %4d      %8d"), StatNumIndexBuffers, StatIndexBufferBytes );
		Ar.Logf( TEXT("Other               %4d      %8d"), StatNumOther, StatOtherBytes );
		Ar.Logf( TEXT("") );

		return 1;
	}
	else if( ParseCommand(&Cmd,TEXT("SUPPORTEDRESOLUTION")) )
	{
		INT		Width		= 0,
				Height		= 0,
				BitDepth	= 0;
		UBOOL	Supported	= 0;

		if( Parse(Cmd,TEXT("WIDTH="),Width) && Parse(Cmd,TEXT("HEIGHT="),Height) && Parse(Cmd,TEXT("BITDEPTH="),BitDepth) )
		{
#if 0
			for(INT ModeIndex = 0;ModeIndex < DisplayModes.Num();ModeIndex++)
			{
				D3DDISPLAYMODE&	DisplayMode = DisplayModes(ModeIndex);

				if(DisplayMode.Width == Width && DisplayMode.Height == Height && GetFormatBPP(DisplayMode.Format) == BitDepth)
				{
					Supported = 1;
					break;
				}
			}
#else
			Supported = 1;
#endif		
		}

		Ar.Logf(TEXT("%u"),Supported);

		return 1;
	}

	return 0;

	unguard;
}


//
// UOpenGLRenderDevice::Lock
//
FRenderInterface* UOpenGLRenderDevice::Lock(UViewport* Viewport,BYTE* InHitData,INT* InHitSize)
{
	guard(UOpenGLRenderDevice::Lock);
	
	MakeCurrent();

	FrameCounter++;

	check( Viewport );
	LockedViewport = Viewport;

	glViewport( 0, 0, Viewport->SizeX, Viewport->SizeY );

	// Create the render interface.
	RenderInterface.Locked( Viewport, InHitData, InHitSize );

	return &RenderInterface;	
	
	unguard;
}


//
// UOpenGLRenderDevice::Unlock
//
void UOpenGLRenderDevice::Unlock(FRenderInterface* RI)
{
	guard(UOpenGLRenderDevice::Unlock);
	
	RenderInterface.Unlocked();
	LockedViewport = NULL;

	GLError(TEXT("UOpenGLRenderDevice::Unlock"));
	
	unguard;
}


//
// UOpenGLRenderDevice::Present
//
void UOpenGLRenderDevice::Present(UViewport* Viewport)
{
	guard(UOpenGLRenderDevice::Present);

#ifdef __LINUX__
	SDL_GL_SwapBuffers();
#else
	verify( SwapBuffers( hDC ) );
#endif

	unguard;
}


//
// UOpenGLRenderDevice::ReadPixels
//
void UOpenGLRenderDevice::ReadPixels(UViewport* Viewport,FColor* Pixels)
{
	guard(UOpenGLRenderDevice::ReadPixels);

	if( Viewport && Pixels )
	{
		glReadPixels( 0, 0, Viewport->SizeX, Viewport->SizeY, GL_BGRA, GL_UNSIGNED_BYTE, Pixels );
		for( INT i=0; i<Viewport->SizeY/2; i++ )
			for( INT j=0; j<Viewport->SizeX; j++ )
				Exchange( Pixels[j+i*Viewport->SizeX], Pixels[j+(Viewport->SizeY-1-i)*Viewport->SizeX] );
	}

	unguard;
}


//
// UOpenGLRenderDevice::GetRenderCaps
//
FRenderCaps* UOpenGLRenderDevice::GetRenderCaps()
{
	guard(UOpenGLRenderDevice::GetRenderCaps);

	RenderCaps.HardwareTL					= 1;
	RenderCaps.MaxSimultaneousTerrainLayers	= 1;
	RenderCaps.PixelShaderVersion			= 0;
	return &RenderCaps;
	
	unguard;
}


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

