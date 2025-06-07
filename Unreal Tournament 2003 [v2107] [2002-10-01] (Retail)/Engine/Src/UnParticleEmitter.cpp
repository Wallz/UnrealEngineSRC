/*=============================================================================
	UnParticleEmitter.cpp: Unreal Particle Emitter
	Copyright 2001 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Daniel Vogel
=============================================================================*/

#include "EnginePrivate.h"
#include <math.h>

/*-----------------------------------------------------------------------------
	UParticleEmitter.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(UParticleEmitter);

//
// UnrealScript interface.
//
void UParticleEmitter::execSpawnParticle( FFrame& Stack, RESULT_DECL )
{
	guard(UParticleEmitter::execSpawnParticle);
	P_GET_INT(Amount);
	P_FINISH;
	DeferredParticles += Amount;
	unguard;
}

void UParticleEmitter::execTrigger( FFrame& Stack, RESULT_DECL )
{
	guard(UParticleEmitter::execTrigger);
	P_FINISH;
	
	if( TriggerDisabled )
		Disabled = !Disabled;
	if( ResetOnTrigger )
		Reset();
	CurrentSpawnOnTrigger += SpawnOnTriggerRange.GetRand();
	if( CurrentSpawnOnTrigger )	
	{
		AllParticlesDead	= false;
		Inactive			= false;
	}

	unguard;
}


//
// General helper function.
//
static inline FVector RandomNormal()
{
	FLOAT Size;
	FVector V;
	do
	{
		V		= FVector(1-2*appFrand(), 1-2*appFrand(), 1-2*appFrand()); 
		Size	= V.Size();
	} while ( (Size > 1.0f) || (Size < 0.1f) );
	return V / Size;
}


// UObject functions.

// In our case equivalent to Spawned for Actors.
void UParticleEmitter::PostLoad()
{
	guard(UParticleEmitter::PostLoad);
	Super::PostLoad();
	Initialize( MaxParticles );
	unguard;
}

void UParticleEmitter::Destroy()
{
	guard(UParticleEmitter::Destroy);
	CleanUp();
	Super::Destroy();
	unguard;
}

void UParticleEmitter::PostEditChange()
{
	guard(UParticleEmitter::PostEditChange);
	if ( (Particles.Num() != MaxParticles) || ResetAfterChange )
	{
		CleanUp();
		Initialize( MaxParticles );
	}
	// Make sure collision data is up- to date.
	if ( UseCollisionPlanes )
		for (INT i=0; i<CollisionPlanes.Num(); i++)
			CollisionPlanes(i).Normalize();
	unguard;
}

// Helper functions.

void UParticleEmitter::Reset()
{
	guard(UParticleEmitter::Reset);
	ActiveParticles		= 0;
	ParticleIndex		= 0;
	OtherIndex			= 0;
	AllParticlesDead	= false;
	WarmedUp			= false;
	Owner				= NULL;
	InitialDelay		= InitialDelayRange.GetRand();
	TimeTillReset		= AutoResetTimeRange.GetRand();
	unguard;
}

void UParticleEmitter::Initialize( INT InMaxParticles )
{
	guard(UParticleEmitter::Initialized);
	Particles.Empty();
	Particles.Add( InMaxParticles );
	Reset();
	RealExtentMultiplier= ExtentMultiplier;
	RealMeshNormal		= MeshNormal.SafeNormal();
	MaxActiveParticles	= InMaxParticles;
	Initialized			= true;
	unguard;
}

void UParticleEmitter::CleanUp()
{
	guard(UParticleEmitter::CleanUp);
	Particles.Empty();
	ActiveParticles		= 0;
	ParticleIndex		= 0;
	Initialized			= false;
	unguard;
}

void UParticleEmitter::Scale( FLOAT ScaleFactor )
{
	guard(UParticleEmitter::Scale);
	StartLocationRange			*= ScaleFactor;
	StartSizeRange				*= ScaleFactor;
	StartVelocityRange			*= ScaleFactor;
	MaxAbsVelocity				*= ScaleFactor;
	StartLocationPolarRange.Z	*= ScaleFactor;
	unguard;
}


// Particle functions.

// Spawn one particle.
void UParticleEmitter::SpawnParticle( INT Index, FLOAT SpawnTime, INT Flags, INT SpawnFlags, const FVector& LocalLocationOffset )
{
	guard(UParticleEmitter::SpawnParticle);
	if( !MaxParticles || !Owner || KillPending || (LifetimeRange.Max <= 0) )
		return;

	FParticle& Particle = Particles(Index);

	Particle.Location	= StartLocationOffset;
	Particle.Velocity	= StartVelocityRange.GetRand();

	// Handle Shape.
	UBOOL ApplyAll = StartLocationShape == PTLS_All;
	if( ApplyAll || StartLocationShape == PTLS_Box )
		Particle.Location += StartLocationRange.GetRand();
	if( ApplyAll || StartLocationShape == PTLS_Sphere )
		Particle.Location += SphereRadiusRange.GetRand() * RandomNormal();
	if( ApplyAll || StartLocationShape == PTLS_Polar )	
	{
		FVector Polar = StartLocationPolarRange.GetRand();
		FLOAT X,Y,Z;
		X = Polar.Z * GMath.CosFloat(Polar.X * PI/32768.f)*GMath.SinFloat(Polar.Y * PI/32768.f);
		Y = Polar.Z * GMath.SinFloat(Polar.X * PI/32768.f)*GMath.SinFloat(Polar.Y * PI/32768.f);
		Z = Polar.Z * GMath.CosFloat(Polar.Y * PI/32768.f);
		Particle.Location += FVector(X,Y,Z);
	}

	// Handle spawning from mesh.
	Particle.ColorMultiplier = FVector(1.f,1.f,1.f);
	if( (MeshSpawning != PTMS_None) && MeshSpawningStaticMesh )
	{
		guard(MeshSpawning);
		INT MaxIndex = MeshSpawningStaticMesh->VertexStream.Vertices.Num() - 1;
		if( MaxIndex > 0 )
		{
			INT VertexIndex = (MeshSpawning == PTMS_Linear) ? CurrentMeshSpawningIndex++ : (INT) (appSRand() * MaxIndex);
			VertexIndex %= MaxIndex;
			VertexIndex = Clamp<INT>( VertexIndex, 0, MaxIndex );

			if( SpawnOnlyInDirectionOfNormal )
			{
				FVector Normal = MeshSpawningStaticMesh->VertexStream.Vertices(VertexIndex).Normal;	
				if( (Normal | RealMeshNormal) < (1.f - 2.f * MeshNormalThresholdRange.GetRand()) ) 
				{	
					Particle.Flags &= ~PTF_Active;
					return;
				}
			}

			FVector LocationScale = MeshScaleRange.GetRand();
			FVector Location = MeshSpawningStaticMesh->VertexStream.Vertices(VertexIndex).Position;	
			Particle.Location += UniformMeshScale ? Location * LocationScale.X : Location * LocationScale;

			if( VelocityFromMesh )
			{
				FVector VelocityScale = VelocityScaleRange.GetRand();
				FVector Velocity = MeshSpawningStaticMesh->VertexStream.Vertices(VertexIndex).Normal;	
				Particle.Velocity += UniformVelocityScale ? Velocity * VelocityScale.X : Velocity * VelocityScale;
			}

			if( UseColorFromMesh )
			{
				FColor Color = MeshSpawningStaticMesh->ColorStream.Colors(VertexIndex);
				Particle.ColorMultiplier.X = Color.R / 255.f;
				Particle.ColorMultiplier.Y = Color.G / 255.f;
				Particle.ColorMultiplier.Z = Color.B / 255.f;
			}
		}
		unguard;
	}

	// Handle Skeletal mesh spawning.
	INT NumBones = MeshVertsAndNormals.Num();
	Particle.BoneIndex = Clamp<INT>(RelativeBoneIndexRange.GetRand() * NumBones, 0.f, NumBones - 1);
	if( UseSkeletalLocationAs != PTSU_None && NumBones )
	{
		Particle.OldMeshLocation = MeshVertsAndNormals( Particle.BoneIndex ) * SkeletalScale;
		Particle.Location += Particle.OldMeshLocation;
	}


	OtherIndex++;
	if ( AddLocationFromOtherEmitter >= 0 )
	{
		UParticleEmitter* OtherEmitter = Owner->Emitters(AddLocationFromOtherEmitter);
		if (OtherEmitter->ActiveParticles > 0)
			Particle.Location += OtherEmitter->Particles(OtherIndex % OtherEmitter->ActiveParticles).Location - Owner->Location;
	}
		
	// Handle Rotation.
	switch ( UseRotationFrom )
	{
	case PTRS_Actor:
		Particle.Location = Particle.Location.TransformVectorBy(GMath.UnitCoords*RotationOffset*Owner->Rotation);
		break;
	case PTRS_Offset:
		Particle.Location = Particle.Location.TransformVectorBy(GMath.UnitCoords*RotationOffset);
		break;
	case PTRS_Normal:
		{
		FRotator Rotator  = RotationNormal.Rotation();
		// Map Z to -X if effect was created along the Z axis instead of negative X
		if ( EffectAxis == PTEA_PositiveZ )
			Rotator.Pitch -= 16384;
		Particle.Location = Particle.Location.TransformVectorBy(GMath.UnitCoords*Rotator);
		}
	default:
		break;
	}
	
	Particle.RevolutionCenter		= RevolutionCenterOffsetRange.GetRand();
	Particle.RevolutionsPerSecond	= RevolutionsPerSecondRange.GetRand();

	switch ( CoordinateSystem )
	{
	case PTCS_Independent :
		if ( !(SpawnFlags & PSF_NoOwnerLocation) )
			Particle.Location += Owner->Location + SpawnTime * Owner->Velocity;	
		Particle.RevolutionCenter += Owner->Location;
		break;
	case PTCS_Relative :
	case PTCS_Absolute :
	default:
		break;
	}

	if ( !(SpawnFlags & PSF_NoGlobalOffset) )
		Particle.Location += GlobalOffset;

	Particle.Location	   += LocalLocationOffset;

	Particle.OldLocation	= Particle.Location;
	Particle.StartLocation	= Particle.Location;
	Particle.ColorMultiplier *= ColorMultiplierRange.GetRand();
	Particle.MaxLifetime	= LifetimeRange.GetRand();
	Particle.Time			= SpawnTime + InitialTimeRange.GetRand();
	Particle.HitCount		= 0;
	Particle.Flags			= PTF_Active | Flags;
	Particle.Mass			= StartMassRange.GetRand();
	Particle.StartSize		= StartSizeRange.GetRand();
	if ( UniformSize )
	{
		Particle.StartSize.Y = Particle.StartSize.X;
		Particle.StartSize.Z = Particle.StartSize.X;
	}
	Particle.Size			= Particle.StartSize;
	Particle.Velocity		+= Acceleration * SpawnTime;
	Particle.VelocityMultiplier		= FVector(1.f,1.f,1.f);
	Particle.RevolutionsMultiplier	= FVector(1.f,1.f,1.f);
	
	// Adjust velocity.
	switch ( UseRotationFrom )
	{
	case PTRS_Actor:
		Particle.Velocity	= Particle.Velocity.TransformVectorBy(GMath.UnitCoords*RotationOffset*Owner->Rotation);
		break;
	case PTRS_Offset:
		Particle.Velocity	= Particle.Velocity.TransformVectorBy(GMath.UnitCoords*RotationOffset);
		break;
	case PTRS_Normal:
		Particle.Velocity	= Particle.Velocity.TransformVectorBy(GMath.UnitCoords*RotationNormal.Rotation());
	default:
		break;
	}

	if( GetVelocityDirectionFrom != PTVD_None )
	{
		FVector Direction;

		if( CoordinateSystem == PTCS_Relative )
			Direction = Particle.Location.SafeNormal();
		else
			Direction = (Owner->Location - Particle.Location).SafeNormal();

		switch( GetVelocityDirectionFrom )
		{
		case PTVD_StartPositionAndOwner:
			Particle.Velocity = -Particle.Velocity * Direction;
			break;
		case PTVD_OwnerAndStartPosition:
			Particle.Velocity = Particle.Velocity * Direction;
			break;
		case PTVD_AddRadial:
			Particle.Velocity += StartVelocityRadialRange.GetRand() * Direction;
		default:
			break;
		}
	}

	if ( AddVelocityFromOtherEmitter >= 0 )
	{
		UParticleEmitter* OtherEmitter = Owner->Emitters(AddVelocityFromOtherEmitter);
		if (OtherEmitter->ActiveParticles > 0)
			Particle.Velocity += AddVelocityMultiplierRange.GetRand() * OtherEmitter->Particles(OtherIndex % OtherEmitter->ActiveParticles).Velocity;
	}

	Particle.Location += SpawnTime * Particle.Velocity;

	// Mass is stored as one over mass internally.
	if ( Particle.Mass )
		Particle.Mass = 1.f / Particle.Mass;
	
	Particle.StartSpin		= StartSpinRange.GetRand();
	Particle.SpinsPerSecond = SpinsPerSecondRange.GetRand();

	// Determine spin.
	if ( SpinCCWorCW.X > appFrand() )
		Particle.SpinsPerSecond.X *= -1;
	if ( SpinCCWorCW.Y > appFrand() )
		Particle.SpinsPerSecond.Y *= -1;
	if ( SpinCCWorCW.Z > appFrand() )
		Particle.SpinsPerSecond.Z *= -1;

	Particle.StartSpin		*= 0xFFFF;
	Particle.SpinsPerSecond *= 0xFFFF;

	if ( UseRandomSubdivision )
	{
		if ( SubdivisionEnd )
			Particle.Subdivision = (INT) ((SubdivisionEnd - SubdivisionStart) * appFrand() + SubdivisionStart);
		else
			Particle.Subdivision = (INT) (appFrand() * TextureUSubdivisions * TextureVSubdivisions); 
	}
	else
		Particle.Subdivision = -1;

	if ( (Particle.Time > Particle.MaxLifetime) && Particle.MaxLifetime )
		SpawnParticle( Index, fmod(Particle.Time,Particle.MaxLifetime) );

	// Play sound on spawning.
	if( (SpawningSound != PTSC_None) && Owner->GetLevel()->Engine->Audio && Sounds.Num() )
	{
		INT SoundIndex = 0;
		switch ( SpawningSound )
		{
		case PTSC_LinearGlobal:
		case PTSC_LinearLocal:		
			SoundIndex = CurrentSpawningSoundIndex++;
			break;
		case PTSC_Random:
			SoundIndex = (INT) (1000 * appSRand());
			break;
		}
			
		SoundIndex %= (INT) (SpawningSoundIndex.Size() ? SpawningSoundIndex.Size() + 1 : Sounds.Num());
		SoundIndex += (INT) SpawningSoundIndex.GetMin();
		SoundIndex = Clamp( SoundIndex, 0, Sounds.Num() - 1); 

		if( appSRand() <= (Sounds(SoundIndex).Probability.GetRand() * SpawningSoundProbability.GetRand()) )
				Owner->GetLevel()->Engine->Audio->PlaySound( Owner, SLOT_None, Sounds(SoundIndex).Sound, Particle.Location, Owner->TransientSoundVolume*Sounds(SoundIndex).Volume.GetRand(), Sounds(SoundIndex).Radius.GetRand(), Sounds(SoundIndex).Pitch.GetRand(), SF_NoUpdates, 0.f );
	}

	// Make sure we get ticked.
	AllParticlesDead = false;

	unguard;
}


// Spawn continuosly.
FLOAT UParticleEmitter::SpawnParticles( FLOAT OldLeftover, FLOAT Rate, FLOAT DeltaTime )
{
	guard(UParticleEmitter::SpawnParticles);
	
	if (Rate <= 0 || !Owner) 
		return 0;
 
	FLOAT NewLeftover = OldLeftover + DeltaTime * Rate;
	INT Number = appFloor(NewLeftover);
	NewLeftover = NewLeftover - Number;

	FLOAT Increment = 1.f / Rate;
	FLOAT StartTime = DeltaTime + OldLeftover * Increment - Increment;

	Number = Clamp(Number, 0, MaxActiveParticles);
	if( CurrentSpawnOnTrigger )
	{
		Number = Clamp( Number, 0, CurrentSpawnOnTrigger );
		CurrentSpawnOnTrigger -= Number;
	}

	for (INT i=0; i<Number; i++)
	{
		SpawnParticle( ParticleIndex, StartTime - i * Increment, PTF_InitialSpawn );
		ActiveParticles = Max(ActiveParticles, ParticleIndex+1);
		++ParticleIndex %= MaxActiveParticles;
	}

	return NewLeftover;
	unguard;
}


// Tick each individual particle.
INT UParticleEmitter::UpdateParticles( FLOAT DeltaTime )
{
	BoundingBox.Init();
	INT	DeadParticles = 0;
	
	// Verify range of critical variables.
	if ( Owner )
	{
		if ( AddLocationFromOtherEmitter >= 0 )
			AddLocationFromOtherEmitter = Clamp(AddLocationFromOtherEmitter, 0, Owner->Emitters.Num() - 1 );
		if ( AddVelocityFromOtherEmitter >= 0 )
			AddVelocityFromOtherEmitter = Clamp(AddVelocityFromOtherEmitter, 0, Owner->Emitters.Num() -1 );
		if ( SpawnFromOtherEmitter >= 0 )
			SpawnFromOtherEmitter = Clamp(SpawnFromOtherEmitter, 0, Owner->Emitters.Num() - 1 );
	}
	else
		return 0;

	// Skeletal mesh stuff.
	INT NumBones = 0;
	if( SkeletalMeshActor && UseSkeletalLocationAs != PTSU_None )
	{
		guard(SkeletalMeshGet);
		if( !SkeletalMeshActor->bDeleteMe && SkeletalMeshActor->Mesh && SkeletalMeshActor->Mesh->IsA(USkeletalMesh::StaticClass()) )
		{
			USkeletalMeshInstance* SkeletalMeshInstance  = (USkeletalMeshInstance*)SkeletalMeshActor->Mesh->MeshGetInstance(SkeletalMeshActor);    
			NumBones = SkeletalMeshInstance->GetMeshJointsAndNormals( SkeletalMeshActor, &MeshVertsAndNormals);
		}
		unguard
	}

	// Spawning.
	FLOAT Rate;
	if ( ActiveParticles < MaxActiveParticles )
	{
		// Either spawn them continously...
		if ( AutomaticInitialSpawning )
		{
			Rate = MaxActiveParticles / LifetimeRange.GetCenter();
		}
		// ... or at a fixed rate.
		else
		{
			Rate = InitialParticlesPerSecond;
		}
	}
	else
	{
		Rate = ParticlesPerSecond; //!! TODO: PPSLightFactor
		//Rate = ParticlesPerSecond * PPSLightFactor * LightBrightness / 255.f;
	}

	// Spawning on trigger.
	if( CurrentSpawnOnTrigger )
		Rate += SpawnOnTriggerPPS;

	// Actually spawn them.
	if ( Rate > 0 && !KillPending )
		PPSFraction = SpawnParticles( PPSFraction, Rate, DeltaTime );

	// Deferred spawning.
	if( !KillPending )
	{
		INT Amount = Clamp(DeferredParticles, 0, MaxActiveParticles);
		for (INT i=0; i<Amount; i++)
		{
			SpawnParticle( ParticleIndex, 0 );
			ActiveParticles = Max(ActiveParticles, ParticleIndex+1);
			++ParticleIndex %= MaxActiveParticles;
		}
		DeferredParticles = 0;
	}

	// Only particles 0..Min(MaxActiveParticles, ActiveParticles) are active.
	for (INT Index=0; Index<Min(MaxActiveParticles, ActiveParticles); Index++)
	{
		FParticle& Particle = Particles(Index);
		
		if ( !(Particle.Flags & PTF_Active) )
		{
			DeadParticles++;
			continue;
		}
		// Don't tick particles with PTF_NoTick
		UBOOL TickParticle = !(Particle.Flags & PTF_NoTick) || (CoordinateSystem == PTCS_Relative);
		// Don't tick particle if it just got spawned via initial spawning.
		if ( Particle.Flags & PTF_InitialSpawn )
		{
			TickParticle = false;
			Particle.Flags &= ~PTF_InitialSpawn;
		}
		else
			Particle.Time += DeltaTime;
		if ( Particle.Time > Particle.MaxLifetime )
		{
			if ( !RespawnDeadParticles )
			{
				Particle.Flags &= ~PTF_Active;
				DeadParticles++;
				continue;
			}
			FLOAT NewTime = Particle.Time - Particle.MaxLifetime + InitialTimeRange.GetRand();
			if( Particle.MaxLifetime )
				NewTime = fmod( NewTime, Particle.MaxLifetime );
			else
				NewTime = 0.f;
			SpawnParticle( Index, NewTime );
		}
		else if ( TickParticle )
		{
			Particle.Velocity	+= Acceleration * DeltaTime;
			Particle.OldLocation = Particle.Location;
			Particle.Location	+= DeltaTime * (Particle.Velocity * Particle.VelocityMultiplier);

			if( NumBones && UseSkeletalLocationAs == PTSU_Location )
			{
				FVector NewMeshLocation = MeshVertsAndNormals( Particle.BoneIndex ) * SkeletalScale;
				Particle.Location += NewMeshLocation - Particle.OldMeshLocation;
				Particle.OldMeshLocation = NewMeshLocation;
			}

			if( UseRevolution )
			{
				FVector RevolutionCenter	= Particle.RevolutionCenter;
				FVector Location			= Particle.Location - RevolutionCenter;
				Location = Location.RotateAngleAxis( Particle.RevolutionsPerSecond.X * Particle.RevolutionsMultiplier.X * DeltaTime * 0xFFFF, FVector(1,0,0) );
				Location = Location.RotateAngleAxis( Particle.RevolutionsPerSecond.Y * Particle.RevolutionsMultiplier.Y * DeltaTime * 0xFFFF, FVector(0,1,0) );
				Location = Location.RotateAngleAxis( Particle.RevolutionsPerSecond.Z * Particle.RevolutionsMultiplier.Z * DeltaTime * 0xFFFF, FVector(0,0,1) );
				Particle.Location = Location + RevolutionCenter;
			}
		}

		// Colission detection.
		UBOOL	Collided	= 0;
		FVector HitNormal	= FVector(0,0,0);
		FVector HitLocation	= FVector(0,0,0);
		if ( TickParticle && (CoordinateSystem != PTCS_Relative) )
		{
			if ( UseCollision )
			{
				FCheckResult Hit;
				FVector Direction = (Particle.Location - Particle.OldLocation).SafeNormal() * RealExtentMultiplier * Particle.Size;
				// JG: This was TRACE_AllBlocking, but this seems a better default... 
				// Should pass in flags for this.
				if ( !Owner->GetLevel()->SingleLineCheck( 
					Hit, 
					Owner, 
					Particle.Location + Direction, 
					Particle.OldLocation, 
					TRACE_LevelGeometry | TRACE_Level
					)
				)
				{
					HitNormal   = Hit.Normal;
					HitLocation = Hit.Location - Direction;
					Collided	= true;
				}
			}
			if ( UseCollisionPlanes && !Collided )
			{
				for (INT i=0; i<CollisionPlanes.Num(); i++)
				{
					FPlane& Plane = CollisionPlanes(i);
					if ( Sgn(Plane.PlaneDot(Particle.Location)) !=  Sgn(Plane.PlaneDot(Particle.OldLocation)) )
					{
						HitLocation = FLinePlaneIntersection( Particle.Location, Particle.OldLocation, Plane );
						HitNormal	= Plane;
						Collided	= true;
						break;
					}
				}
			}
		}
		
		// Handle collided particle.
		if ( Collided )
		{
			// Play sound on collision
			if( (CollisionSound != PTSC_None) && Owner->GetLevel()->Engine->Audio )
			{
				INT SoundIndex = 0;
				switch ( CollisionSound )
				{
				case PTSC_LinearGlobal:
					SoundIndex = CurrentCollisionSoundIndex++;
					break;
				case PTSC_LinearLocal:
					SoundIndex = Particle.HitCount;
					break;
				case PTSC_Random:
					SoundIndex = (INT) (1000 * appSRand());
					break;
				}
				
				SoundIndex %= (INT) (CollisionSoundIndex.Size() ? CollisionSoundIndex.Size() + 1 : Sounds.Num());
				SoundIndex += (INT) CollisionSoundIndex.Min;
				SoundIndex = Clamp( SoundIndex, 0, Sounds.Num() ); 

				if( appSRand() <= (Sounds(SoundIndex).Probability.GetRand() * CollisionSoundProbability.GetRand()) )
					Owner->GetLevel()->Engine->Audio->PlaySound( Owner, SLOT_None, Sounds(SoundIndex).Sound, HitLocation, Owner->TransientSoundVolume*Sounds(SoundIndex).Volume.GetRand(), Sounds(SoundIndex).Radius.GetRand(), Sounds(SoundIndex).Pitch.GetRand(), SF_NoUpdates, 0.f );
			}

			// Update.
			if ( UseMaxCollisions && (Particle.HitCount+1 >= appFloor(MaxCollisions.GetRand())))
			{
				if( RespawnDeadParticles )
					SpawnParticle( Index, 0.5 * DeltaTime ); //!! HACK
				else
				{
					Particle.Flags &= ~PTF_Active;
					DeadParticles++;
					continue;
				}
			}			
			else
			{
				Particle.Velocity -= Acceleration * DeltaTime * 0.5f; //!! HACK
				Particle.Velocity  = Particle.Velocity.MirrorByVector( HitNormal );
				Particle.Velocity *= DampingFactorRange.GetRand();
				if (DampRotation)
					Particle.SpinsPerSecond *= RotationDampingFactorRange.GetRand();
				Particle.Location = HitLocation + HitNormal * 0.01f;
				Particle.OldLocation = Particle.Location;
				++Particle.HitCount;
			}
			
			// Spawn particle in another emitter on collision.
			if ( SpawnFromOtherEmitter >= 0 )
			{
				UParticleEmitter* OtherEmitter = Owner->Emitters(SpawnFromOtherEmitter);
				if ( OtherEmitter->Initialized && OtherEmitter->Owner && DeltaTime )
				{
					for (INT i=0; i<SpawnAmount; i++)
					{
						OtherEmitter->SpawnParticle( OtherEmitter->ParticleIndex, DeltaTime, 0, PSF_NoGlobalOffset | PSF_NoOwnerLocation, HitLocation + HitNormal * 0.01f );
						if ( UseSpawnedVelocityScale )
						{
							FParticle& OtherParticle = OtherEmitter->Particles(OtherEmitter->ParticleIndex);
							OtherParticle.Velocity += HitNormal * SpawnedVelocityScaleRange.GetRand();
						}
						OtherEmitter->ActiveParticles = Max(OtherEmitter->ActiveParticles, OtherEmitter->ParticleIndex+1);
						(++OtherEmitter->ParticleIndex) %= OtherEmitter->MaxActiveParticles;
					}		   
				}
			}
		}

		// Scaling over time.
		FLOAT TimeFactor	= 1.0f;
		FLOAT Time			= Particle.Time;
		FLOAT RelativeTime;
		FColor Color		= FColor(255,255,255,255);

		if ( Particle.MaxLifetime )
			RelativeTime	= Clamp( Time / Particle.MaxLifetime, 0.f, 1.f );
		else
			RelativeTime	= 0.f;

		// Size scale.
		if ( UseSizeScale )
		{
			if ( UseRegularSizeScale )
				TimeFactor = TimeFactor / ( 1.f + Particle.Time );
			else if	( Particle.MaxLifetime )
			{
				FLOAT SizeRelativeTime = appFractional( RelativeTime * (SizeScaleRepeats+1) );
				for (INT n=0; n<SizeScale.Num(); n++)
				{
					if ( SizeScale(n).RelativeTime >= SizeRelativeTime )
					{
						FLOAT S1, R1;
						FLOAT S2 = SizeScale(n).RelativeSize;
						FLOAT R2 = SizeScale(n).RelativeTime;
						if (n)
						{
							S1 = SizeScale(n-1).RelativeSize;
							R1 = SizeScale(n-1).RelativeTime;
						}
						else
						{
							S1 = 1.f;
							R1 = 0.f;
						}
						FLOAT A;
						if ( R2 )
							A = (SizeRelativeTime - R1) / (R2 - R1);
						else
							A = 1;

						// Interpolate between two scales.
						TimeFactor = Lerp(S1, S2, A);			
						break;
					}	
				}
			}	
		}
		Particle.Size = TimeFactor * Particle.StartSize;	

		// Velocity scale.
		if ( UseVelocityScale )
		{
			if	( Particle.MaxLifetime )
			{
				FLOAT VelocityRelativeTime = appFractional( RelativeTime * (VelocityScaleRepeats+1) );
				for (INT n=0; n<VelocityScale.Num(); n++)
				{
					if ( VelocityScale(n).RelativeTime >= VelocityRelativeTime )
					{
						FVector V1,
								V2 = VelocityScale(n).RelativeVelocity;
						FLOAT	R1,
								R2 = VelocityScale(n).RelativeTime;
						if (n)
						{
							V1 = VelocityScale(n-1).RelativeVelocity;
							R1 = VelocityScale(n-1).RelativeTime;
						}
						else
						{
							V1 = FVector(1.f,1.f,1.f);
							R1 = 0.f;
						}
						FLOAT A;
						if ( R2 )
							A = (VelocityRelativeTime - R1) / (R2 - R1);
						else
							A = 1;

						// Interpolate between two scales.
						FVector Multiplier = Lerp(V1, V2, A);
						Particle.VelocityMultiplier = Multiplier;
						break;
					}	
				}
			}	
		}

		// Revolution scale.
		if ( UseRevolutionScale )
		{
			if	( Particle.MaxLifetime )
			{
				FLOAT RevolutionRelativeTime = appFractional( RelativeTime * (RevolutionScaleRepeats+1) );
				for (INT n=0; n<RevolutionScale.Num(); n++)
				{
					if ( RevolutionScale(n).RelativeTime >= RevolutionRelativeTime )
					{
						FVector V1,
								V2 = RevolutionScale(n).RelativeRevolution;
						FLOAT	R1,
								R2 = RevolutionScale(n).RelativeTime;
						if (n)
						{
							V1 = RevolutionScale(n-1).RelativeRevolution;
							R1 = RevolutionScale(n-1).RelativeTime;
						}
						else
						{
							V1 = FVector(1.f,1.f,1.f);
							R1 = 0.f;
						}
						FLOAT A;
						if ( R2 )
							A = (RevolutionRelativeTime - R1) / (R2 - R1);
						else
							A = 1;

						// Interpolate between two scales.
						FVector Multiplier = Lerp(V1, V2, A);
						Particle.RevolutionsMultiplier = Multiplier;
						break;
					}	
				}
			}	
		}

		// Color scaling.
		if ( UseColorScale && Particle.MaxLifetime )
		{
			FLOAT ColorRelativeTime	= appFractional( RelativeTime * (ColorScaleRepeats+1) );
			for (INT n=0; n<ColorScale.Num(); n++)
			{
				if ( ColorScale(n).RelativeTime >= ColorRelativeTime )
				{
					FLOAT R1;
					FLOAT R2  = ColorScale(n).RelativeTime;
					FColor C1;
					FColor C2 = ColorScale(n).Color;
					if (n)
					{
						C1 = ColorScale(n-1).Color;
						R1 = ColorScale(n-1).RelativeTime;
					}
					else
					{
						C1 = FColor(255,255,255,255);
						R1 = 0.f;
					}
					FLOAT A;
					if ( R2 )
						A = (ColorRelativeTime - R1) / (R2 - R1);
					else
						A = 1.f;
	
					// Interpolate between two colors.
					Color.R = Lerp(C1.R, C2.R, A);
					Color.G = Lerp(C1.G, C2.G, A);
					Color.B = Lerp(C1.B, C2.B, A);
					if ( DrawStyle == PTDS_AlphaBlend )
						Color.A = Lerp(C1.A, C2.A, A);
					else
						Color.A = 255;

					break;			
				}
			}	
		}
		
		Color.R *= Particle.ColorMultiplier.X;
		Color.G *= Particle.ColorMultiplier.Y;
		Color.B *= Particle.ColorMultiplier.Z;
		
		// Fade In/ Out.
		if ( (FadeOut && (Time > FadeOutStartTime) && (Particle.MaxLifetime != FadeOutStartTime)) 
		||   (FadeIn  && (Time < FadeInEndTime)    && FadeInEndTime)
			)
		{
			FLOAT FadeFactor;
			FPlane MaxFade;

			if ( FadeOut && (Time > FadeOutStartTime) )
			{
				FadeFactor = Time - FadeOutStartTime;
				FadeFactor /= (Particle.MaxLifetime - FadeOutStartTime);
				FadeFactor *= 255;
				MaxFade = FadeOutFactor;
			}
			else
			{
				
				FadeFactor = FadeInEndTime - Time;
				FadeFactor /= FadeInEndTime;
				FadeFactor *= 255;
				MaxFade = FadeInFactor;
				
			}
			
			if ( DrawStyle == PTDS_Modulated )
			{
				Color = FColor( 
					127,
					127,
					127,
					255 - (INT) (FadeFactor * MaxFade.W)
					);				
			}
			else if ( DrawStyle == PTDS_AlphaBlend )
			{
				Color.A = (BYTE)Clamp<FLOAT>(Color.A - FadeFactor * MaxFade.W, 0.f, 255.f);
			}
			else
			{
			/* // Original code causes an internal compiler error for XBox in release mode. - Erik
				Color = FColor( 
					Clamp<FLOAT>(Color.R - FadeFactor * MaxFade.X, 0.f, 255.f), 
					Clamp<FLOAT>(Color.G - FadeFactor * MaxFade.Y, 0.f, 255.f),
					Clamp<FLOAT>(Color.B - FadeFactor * MaxFade.Z, 0.f, 255.f),
					Clamp<FLOAT>(Color.A - FadeFactor * MaxFade.W, 0.f, 255.f)
					);
			*/
				INT ColR = (INT) Clamp<FLOAT>(Color.R - FadeFactor * MaxFade.X, 0.f, 255.f);
				INT ColG = (INT) Clamp<FLOAT>(Color.G - FadeFactor * MaxFade.Y, 0.f, 255.f);
				INT ColB = (INT) Clamp<FLOAT>(Color.B - FadeFactor * MaxFade.Z, 0.f, 255.f);
				INT ColA = (INT) Clamp<FLOAT>(Color.A - FadeFactor * MaxFade.W, 0.f, 255.f);
				Color = FColor( ColR,ColG,ColB,ColA );					
			}
			
		}
		Particle.Color = Color;

		// Bounding box creation.
		switch ( CoordinateSystem )
		{
			case PTCS_Independent:
			case PTCS_Absolute:
				BoundingBox += Particle.Location;
				break;
			case PTCS_Relative:
				BoundingBox += Particle.Location.TransformPointBy(GMath.UnitCoords * Owner->Rotation) + Owner->Location;
				break;
			default:
				break;
		}

		// Clamping velocity.
		if ( MaxAbsVelocity.X )
			Particle.Velocity.X = Clamp( Particle.Velocity.X, -MaxAbsVelocity.X, MaxAbsVelocity.X );
		if ( MaxAbsVelocity.Y )
			Particle.Velocity.Y = Clamp( Particle.Velocity.Y, -MaxAbsVelocity.Y, MaxAbsVelocity.Y );
		if ( MaxAbsVelocity.Z )
			Particle.Velocity.Z = Clamp( Particle.Velocity.Z, -MaxAbsVelocity.Z, MaxAbsVelocity.Z );

		// Friction.
		Particle.Velocity -= Particle.Velocity * VelocityLossRange.GetRand() * DeltaTime;

		// Don't tick if particle is no longer moving.
		if (Collided && MinSquaredVelocity && ( Particle.Velocity.SizeSquared() < MinSquaredVelocity ) )
			Particle.Flags |= PTF_NoTick;

		// Used by trail emitter e.g.
		UpdateParticle( DeltaTime, Index );
	}

	// Account for SizeScale when expanding bounding box.
	FLOAT MaxScale = 1.f;
	if ( UseSizeScale && !UseRegularSizeScale )
	{
		for (INT i=0; i<SizeScale.Num(); i++)
			MaxScale = Max( SizeScale(i).RelativeSize, MaxScale );
	}
	
	// Subclasses use this to expand bounding box accordingly.
	MaxSizeScale = MaxScale;

	// Ugh, this is getting ugly. Subtle assumptions because of indirect spawning.
	if(	(DeadParticles >= MaxActiveParticles || ActiveParticles == DeadParticles) && Rate == 0 && !RespawnDeadParticles )
		AllParticlesDead = true;
	else
		AllParticlesDead = false;

	return ActiveParticles - DeadParticles;
}


INT UParticleEmitter::RenderParticles( FDynamicActor* DynActor, FLevelSceneNode* SceneNode, TList<FDynamicLight*>* Lights, FRenderInterface* RI )
{
	Inactive=false;
	return 0;
}


// Actor forces
void UParticleEmitter::HandleActorForce( AActor* Other, FLOAT DeltaTime )
{
	switch ( Other->ForceType )
	{
		case FT_DragAlong:
			if ( Other->ForceRadius )
			{
				FParticle* Particle	 = &Particles(0);
				FLOAT RadiusSquared	 = Square(Other->ForceRadius);
				FLOAT RRadiusSquared = 1.f / RadiusSquared;
				for (INT n=0; n<ActiveParticles; n++)
				{
					// Don't change inactive particles.
					if (!(Particle->Flags & PTF_Active))
						continue;
		
					// Mass == 0 indicates no influence.
					if ( Particle->Mass )
					{
						FLOAT DistSquared = FDistSquared( Particle->Location, Other->Location );
						if ( DistSquared < RadiusSquared )
						{
							FVector Direction = (Particle->Location - Other->Location).SafeNormal();
							Direction        *= (Other->Velocity | Direction) > 0 ? 1 : -1;
							FLOAT Velocity	  = Other->Velocity.Size();
							FLOAT Difference  = Velocity - (Direction | Particle->Velocity);
				
							if ( Difference > 0 )
							{
								FLOAT Repulsion = Difference * DeltaTime * Other->ForceScale;
								if ( Repulsion > 0 )
								{
									Particle->Velocity += 
											Repulsion * 
											DistSquared * 
											RRadiusSquared * 
											Direction;
									// Particle has to be ticked. 
									Particle->Flags &= ~PTF_NoTick;
								}
							}
						}
					}
					Particle++;
				}
			}
			break;
		default:
			break;
	}

}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

