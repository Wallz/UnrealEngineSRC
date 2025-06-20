/*=============================================================================
	AGameStats.cpp: Unreal Tournament 2003 Stats control
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Joe Wilcox
=============================================================================*/

#include "EnginePrivate.h"
#include "UnNet.h"

IMPLEMENT_CLASS(AGameStats);

void AGameStats::execGetMapFileName( FFrame& Stack, RESULT_DECL )
{
	guard(AGameStats::execGetMapFileName);
	P_FINISH;

	*(FString*)Result = XLevel->URL.Map;

	unguardexec;
}

void AGameStats::execGetStatsIdentifier( FFrame& Stack, RESULT_DECL )
{
	guard(AGameStats::GetStatsIdentifier);
	P_GET_OBJECT( AController, Controller );
	P_FINISH;
  
	APlayerController* P;
	if( (P=Cast<APlayerController>(Controller))!=NULL )
	{
		// remote player
		UNetConnection* Conn;
		if( (Conn=Cast<UNetConnection>(P->Player))!=NULL )
		{
			*(FString*)Result = FString::Printf(TEXT("%s\t%s\t%s"), *Conn->CDKeyHash, *Conn->EncStatsUsername, *Conn->EncStatsPassword );
			return;
		}

		// local player on listen server
		if( Cast<UViewport>(P->Player)!=NULL )
		{
			*(FString*)Result = FString::Printf(TEXT("%s\t%s\t%s"), *GetCDKeyHash(), *P->StatsUsername, *EncryptWithCDKeyHash( *P->StatsPassword, TEXT("STATS") ) );
			return;
		}
	}

	*(FString*)Result = TEXT("\t\t");

	unguardexec;
}