//=============================================================================
// XInterface - Native Interface Package
// Copyright 2001 Digital Extremes - All Rights Reserved.
// Confidential.
//=============================================================================

#include "XInterface.h"

IMPLEMENT_PACKAGE(XInterface);

#define NAMES_ONLY
#define AUTOGENERATE_NAME(name) XINTERFACE_API FName XINTERFACE_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name) IMPLEMENT_FUNCTION(cls,idx,name)
#include "XInterfaceClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NAMES_ONLY

// sjs --- import natives
#define NATIVES_ONLY
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name)
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#include "XInterfaceClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NATIVES_ONLY
#undef NAMES_ONLY
// --- sjs

#if __STATIC_LINK
    void RegisterNamesXInterface()
    {
	    #define NAMES_ONLY
	    #define AUTOGENERATE_NAME(name) extern XINTERFACE_API FName XINTERFACE_##name; XINTERFACE_##name=FName(TEXT(#name),FNAME_Intrinsic);
	    #define AUTOGENERATE_FUNCTION(cls,idx,name)
	    #include "XInterfaceClasses.h"
	    #undef DECLARE_NAME
	    #undef NAMES_ONLY
    }
#else
    struct FInitManager
    {
        FInitManager()
        {
	        #define NAMES_ONLY
	        #define AUTOGENERATE_NAME(name) extern XINTERFACE_API FName XINTERFACE_##name; XINTERFACE_##name=FName(TEXT(#name),FNAME_Intrinsic);
	        #define AUTOGENERATE_FUNCTION(cls,idx,name)
	        #include "XInterfaceClasses.h"
	        #undef DECLARE_NAME
	        #undef NAMES_ONLY
        }
    } InitManager;
#endif

XINTERFACE_API void CalcPivotCoords( EDrawPivot DrawPivot, FLOAT& ScreenX, FLOAT& ScreenY, FLOAT ScreenDX, FLOAT ScreenDY )
{
	guardSlow(CalcPivotCoords);

    switch (DrawPivot)
    {
        case DP_UpperLeft:
            break;

        case DP_UpperMiddle:
            ScreenX -= ScreenDX * 0.5f;
            break;

        case DP_UpperRight:
            ScreenX -= ScreenDX;
            break;

        case DP_MiddleRight:
            ScreenX -= ScreenDX;
            ScreenY -= ScreenDY * 0.5f;
            break;

        case DP_LowerRight:
            ScreenX -= ScreenDX;
            ScreenY -= ScreenDY;
            break;

        case DP_LowerMiddle:
            ScreenX -= ScreenDX * 0.5f;
            ScreenY -= ScreenDY;
            break;

        case DP_LowerLeft:
            ScreenY -= ScreenDY;
            break;

        case DP_MiddleLeft:
            ScreenY -= ScreenDY * 0.5f;
            break;

        case DP_MiddleMiddle:
            ScreenX -= ScreenDX * 0.5f;
            ScreenY -= ScreenDY * 0.5f;
            break;

        default:
            debugf( NAME_Error, TEXT("CalcPivotCoords() -- Unknown DrawPivot"));
            break;
    }

    unguardSlow;
}

XINTERFACE_API bool MemoryIsZero( const void* Memory, size_t Size )
{
	guard(MemoryIsZero);

    const BYTE* Ptr = (const BYTE*)Memory;

    while( Size > 0 )
    {
        if( *Ptr != 0x00 )
            return( false );

        Size--;
        Ptr++;
    }    

    return( true );

    unguard;
};
