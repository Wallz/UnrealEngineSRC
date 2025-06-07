/*=============================================================================
	UnPackageCheckInfo : Stores allowable MD5s for each package
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Joe Wilcox
	
=============================================================================*/

#include "EnginePrivate.h"

IMPLEMENT_CLASS(UPackageCheckInfo);

UBOOL UPackageCheckInfo::VerifyID(FString CurrentId)
{
	// Look for this id in the chain

	for ( int i=0; i<AllowedIDs.Num(); i++ )
	{
		if ( CurrentId == AllowedIDs(i) )
			return true;
	}

	return false;
}

void UPackageCheckInfo::Serialize( FArchive& Ar )
{

	guard(UPackageCheckInfo::Serialize);

	INT IDCount;

	if( Ar.IsLoading() )		// Load in the block
	{
		FString TmpStr;
		
		Ar << PackageID;		// Get the Package's GUID
		Ar << IDCount;			// Get the # of IDs for this package

		AllowedIDs.Empty();		// Clear the array
	
		for (int i=0; i<IDCount; i++) // Load in all available id's for this package
		{
			Ar << TmpStr;
			new(AllowedIDs)FString(TmpStr);
		}

		Native = true;

	}
	else						// Save out the block
	{

		if (Native)		// Only Native packages are serialized
		{

			Ar << PackageID;
			IDCount = AllowedIDs.Num();
			Ar << IDCount;
			for (int i=0; i<AllowedIDs.Num(); i++)
				Ar << AllowedIDs(i);
		}
	}

	Super::Serialize( Ar );

	unguard;

}


//void UPackageCheckInfo::PostLoad()
//{
//}