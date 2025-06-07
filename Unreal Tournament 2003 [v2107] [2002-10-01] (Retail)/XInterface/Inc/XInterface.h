//=============================================================================
// Copyright 2001 Digital Extremes - All Rights Reserved.
// Confidential.
//=============================================================================

// TODO: WHY CAN'T WE #include "Engine.h" HERE???
#include "UnIpDrv.h"
#include "XGame.h"

struct FGUIListElem
{
	FString item;
	UObject* ExtraData;
	FString ExtraStrData;
};

#include "XInterfaceClasses.h"
#include "XInterfaceNative.h"

XINTERFACE_API void CalcPivotCoords( EDrawPivot DrawPivot, FLOAT& ScreenX, FLOAT& ScreenY, FLOAT ScreenDX, FLOAT ScreenDY );
XINTERFACE_API bool MemoryIsZero( const void* Memory, size_t Size );

#define P_GET_WIDGET_REF(typ,var) GPropAddr=0; Stack.Step( Stack.Object, NULL ); checkSlow( GPropObject ); GPropObject->NetDirty(GProperty); typ* var = (typ *)GPropAddr;
