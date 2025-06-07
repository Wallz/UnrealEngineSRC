/*=============================================================================
	UnVehicle.cpp: Vehicle physics implementation

	Copyright 2000 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Steven Polge 7/00
=============================================================================*/

#include "EnginePrivate.h"

void AVehicle::performPhysics(FLOAT DeltaSeconds)
{
	guard(AVehicle::performPhysics);

	FVector OldVelocity = Velocity;
	OldZ = Location.Z;	// used for eyeheight smoothing

	bSimulateGravity = false;

	if ( bActivated )
	{
		// change position
		switch (Physics)
		{
			case PHYS_Falling: physFalling(DeltaSeconds, 0); bSimulateGravity = true; break;
			case PHYS_Flying: physFlying(DeltaSeconds, 0); break;
		}

		// rotate
		if ( Controller  && !bInterpolating
			&& (bCrawler || IsHumanControlled() || (Rotation != DesiredRotation) || (RotationRate.Roll > 0)) ) 
				physicsRotation(DeltaSeconds, OldVelocity);
	}
	//else if ( Physics != PHYS_None )
	//	setPhysics(PHYS_None);

	AvgPhysicsTime = 0.8f * AvgPhysicsTime + 0.2f * DeltaSeconds;

	if ( PendingTouch )
	{
		PendingTouch->eventPostTouch(this);
		if ( PendingTouch )
		{
			AActor *OldTouch = PendingTouch;
			PendingTouch = PendingTouch->PendingTouch;
			OldTouch->PendingTouch = NULL;
		}
	}
	unguard;
}

void AVehicle::physicsRotation(FLOAT deltaTime, FVector OldVelocity)
{
	guard(AVehicle::physicsRotation);

	// Accumulate a desired new rotation.
	FRotator NewRotation = Rotation;	

	int deltaYaw = (INT) (RotationRate.Yaw * deltaTime);
	bRotateToDesired = 1; //Pawns always have a "desired" rotation
	bFixedRotationDir = 0;

	if ( !IsHumanControlled() )
	{
		//YAW 
		if ( DesiredRotation.Yaw != NewRotation.Yaw )
			NewRotation.Yaw = fixedTurn(NewRotation.Yaw, DesiredRotation.Yaw, deltaYaw);

		//PITCH

		if ( !bCrawler && (DesiredRotation.Pitch != NewRotation.Pitch) )
			NewRotation.Pitch = fixedTurn(NewRotation.Pitch, DesiredRotation.Pitch, deltaYaw);
	}

	//ROLL
	if ( Base )
		NewRotation = FindSlopeRotation(Floor,NewRotation);
	else if ( RotationRate.Roll > 0 ) 
	{
		FVector RealAcceleration = (Velocity - OldVelocity)/deltaTime;
		if ( bCanStrafe )
			RealAcceleration = 0.5 * (RealAcceleration + Acceleration);

		if ( RealAcceleration.SizeSquared() > 100000.f ) 
		{
			FLOAT MaxRoll = 16384.f;
			NewRotation.Roll = 0;
			FCoords Coords = GMath.UnitCoords / Rotation;

			if ((RealAcceleration | Coords.YAxis) > 0) 
				NewRotation.Roll = Min(RotationRate.Roll, (int)((RealAcceleration | Coords.YAxis) * MaxRoll/AccelRate)); 
			else
				NewRotation.Roll = ::Max(65536 - RotationRate.Roll, (int)(65536.f + (RealAcceleration | Coords.YAxis) * MaxRoll/AccelRate));
			
			//smoothly change rotation
			Rotation.Roll = Rotation.Roll & 65535;
			if (NewRotation.Roll > 32768)
			{
				if (Rotation.Roll < 32768)
					Rotation.Roll += 65536;
			}
			else if (Rotation.Roll > 32768)
				Rotation.Roll -= 65536;

			FLOAT SmoothRoll = Min(1.f, 2.f * deltaTime);
			NewRotation.Roll = (INT) (NewRotation.Roll * SmoothRoll + Rotation.Roll * (1 - SmoothRoll));
		}
		else
		{
			FLOAT SmoothRoll = Min(1.f, deltaTime);
			if (NewRotation.Roll < 32768)
				NewRotation.Roll = (INT) (NewRotation.Roll * (1 - SmoothRoll));
			else
				NewRotation.Roll = (INT) (NewRotation.Roll + (65536 - NewRotation.Roll) * SmoothRoll);
		}
	}
	else
		NewRotation.Roll = 0;

	// Set the new rotation.
	if( NewRotation != Rotation )
	{
		FCheckResult Hit(1.f);
		GetLevel()->MoveActor( this, FVector(0,0,0), NewRotation, Hit );
	}
	unguard;
}

