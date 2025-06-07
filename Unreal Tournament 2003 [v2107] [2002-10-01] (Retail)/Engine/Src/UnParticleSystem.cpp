/*=============================================================================
	UnParticleSystem.cpp: Unreal Particle System
	Copyright 2001 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Daniel Vogel
=============================================================================*/

#include "EnginePrivate.h"

/*-----------------------------------------------------------------------------
	AEmitter.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(AEmitter);
IMPLEMENT_CLASS(UParticleMaterial);

// Initialize.
void AEmitter::Initialize()
{
	if( !Initialized )
	{
		ParticleMaterial	= ConstructObject<UParticleMaterial>(UParticleMaterial::StaticClass());
		GlobalOffset		= GlobalOffsetRange.GetRand();
		TimeTillReset		= TimeTillResetRange.GetRand();
		Initialized			= 1;
	}
	
	BoundingBox.Init();
	// Add Emitter icon to bounding box and expand by DrawScale.
	if ( GIsEditor )	
	{
		BoundingBox += Location;
		BoundingBox  = BoundingBox.ExpandBy( Abs(DrawScale) * DrawScale3D.GetAbsMax() );
	}
}

// Check for projectors hitting the emitter.
INT AEmitter::CheckForProjectors()
{
	guard(AEmitter::CheckForProjectors);

	INT ProjIndex = 0;

	// Check the collision hash for projectors touching this emitter.
	guard(FindProjectors)
	if(Level->XLevel->Hash)
	{
		FMemMark Mark(GMem);
		FCheckResult* Hit = Level->XLevel->Hash->ActorPointCheck( GMem, Location, BoundingBox.GetExtent(), TRACE_Projectors, 0 );
		for( ;Hit; Hit=Hit->GetNext() )
		{
			AProjector* Projector = Cast<AProjector>(Hit->Actor);
			// Add to Projectors list in ParticleMaterial.
			if( Projector 
			&& Projector->bProjectParticles
			&& ((Projector->ProjectTag==NAME_None) || (Tag==Projector->ProjectTag)) 
			)
				Projector->UpdateParticleMaterial( ParticleMaterial, ProjIndex++ );
		}
		Mark.Pop();
	}
	unguard;

	// Limit to 4 projectors at this time.
	ParticleMaterial->NumProjectors = Clamp<INT>( ProjIndex, 0, 4 );
	return ParticleMaterial->NumProjectors;

	unguard;
}

// Check whether effect is dynamic
void AEmitter::Spawned()
{
	Super::Spawned();
	if( !GIsEditor )
		DeleteParticleEmitters = 1;
}

// PostScriptDestroyed
void AEmitter::PostScriptDestroyed()
{
	guard(AEMitter::Destroy);
#if 1
	if( DeleteParticleEmitters )
	{
		for (INT i=0; i<Emitters.Num(); i++)
		{
			delete Emitters(i);
			Emitters(i) = NULL;
		}
		delete ParticleMaterial;
		ParticleMaterial = NULL;
	}
#endif
	Super::PostScriptDestroyed();
	unguard;
}

// Kill - shutdown a particle system and make it auto-destroy.
void AEmitter::Kill()
{
	guard(AEmitter::Kill);
	for( INT i=0;i<Emitters.Num();i++ )
	{
		if( Emitters(i) )
		{
			Emitters(i)->RespawnDeadParticles		= 0;
			Emitters(i)->InitialParticlesPerSecond	= 0;
			Emitters(i)->ParticlesPerSecond			= 0;
			Emitters(i)->AutoReset					= 0;
			Emitters(i)->AutoDestroy				= 0;
			Emitters(i)->KillPending				= 1;
		}
	}
	AutoDestroy = 1;
	AutoReset	= 0;
	unguard;
}

// Script execKill();
void AEmitter::execKill( FFrame& Stack, RESULT_DECL )
{
	guardSlow(AEmitter::execKill);
	P_FINISH;
	Kill();
	unguardexecSlow;
}

// Global tick
UBOOL AEmitter::Tick( FLOAT DeltaTime, enum ELevelTick TickType )
{
	guard(AEmitter::Tick);
	if ( !Super::Tick(DeltaTime, TickType) )
		return false;

	if( !Initialized )
		Initialize();

	// Don't tick if ViewportsOnly or if actor just got deleted.
	if (bDeleteMe || ((TickType == LEVELTICK_ViewportsOnly) && !GIsEditor) )
	{
		return true;
	}

	clock(GStats.DWORDStats(GEngineStats.STATS_Game_ParticleTickCycles));
	
	// Initialize bounding box.
	BoundingBox.Init();

	// Add Editor icon to bounding box.
	if( GIsEditor )
	{
		BoundingBox += Location;
		BoundingBox.ExpandBy( Abs(DrawScale) * DrawScale3D.GetAbsMax() );
	}

	UBOOL AllDead			= true;
	UBOOL AllDisabled		= true;
	UBOOL ActorForces		= false; 
	UseParticleProjectors	= false;

	// Route the ticking.
	for (INT i=0; i<Emitters.Num(); i++)
	{
		UParticleEmitter* Emitter = Emitters(i);

		if (!Emitter || Emitter->Disabled)
			continue;

		// Do we need to check for projectors?
		UseParticleProjectors |= Emitter->AcceptsProjectors;

		// This one is alive.
		AllDisabled = false;

		// Set the owner.
		Emitter->Owner = this;

		// In case it has been imported via T3D (cut & paste)
		if (!Emitter->Initialized)
			Emitter->Initialize( Emitter->MaxParticles );

		// Set fogging behaviour.
		Emitter->RealDisableFogging = Emitter->DisableFogging  || DisableFogging;

		// Handle initial delay.
		if ( Emitter->InitialDelay )
		{
			Emitter->InitialDelay -= DeltaTime;
			if ( Emitter->InitialDelay > 0.f )
				continue;
			else
				Emitter->InitialDelay = 0.f;
		}
		
		// Handle global offset.
		Emitter->GlobalOffset = GlobalOffset;

		// Prime the particle system.
		if ( !Emitter->WarmedUp && Emitter->WarmupTicksPerSecond && Emitter->RelativeWarmupTime )
		{
			FLOAT PrimeTime = Emitter->LifetimeRange.GetCenter() * Emitter->RelativeWarmupTime;
			FLOAT DeltaTime = 1.f / Emitter->WarmupTicksPerSecond;
			for ( INT i=0; i < (INT)(Emitter->WarmupTicksPerSecond * PrimeTime); i++)
				Emitter->UpdateParticles( DeltaTime );
			Emitter->WarmedUp = true;
		}

		// Local tick.
		if ( !Emitter->AllParticlesDead )
		{
			if (Emitter->Inactive)
				Emitter->InactiveTime += DeltaTime;
			else
				Emitter->InactiveTime = 0;

			// Don't tick if ParticleEmitter hasn't been visible for SecondsBeforeInactive seconds.
			if (! (Emitter->SecondsBeforeInactive && (Emitter->InactiveTime > Emitter->SecondsBeforeInactive)))
				Emitter->UpdateParticles( DeltaTime );
	
			Emitter->Inactive	= true;
			AllDead				= false;

			ActorForces |= Emitter->UseActorForces;

			// Handle actor forces.
			if ( Emitter->UseActorForces )
				for (INT i=0; i<Touching.Num(); i++)
					if ( Touching(i) && Touching(i)->ForceType != FT_None && !Touching(i)->bDeleteMe )
						Emitter->HandleActorForce( Touching(i), DeltaTime );
		}
		else if ( Emitters(i)->AutoReset && !AutoReset )
		{
			if ( (Emitters(i)->TimeTillReset-DeltaTime) > 0 )
			{
				Emitters(i)->TimeTillReset -= DeltaTime;
			}
			else
			{
				Emitters(i)->TimeTillReset = Emitters(i)->AutoResetTimeRange.GetRand();
				Emitters(i)->Reset();
			}
		}
		// AutoDestroy ParticleEmitter.
		else if ( Emitters(i)->AutoDestroy && !AutoDestroy )
		{
			Emitters(i)->Disabled = true;
		}

		// Expand bounding box.
		BoundingBox = BoundingBox + Emitter->BoundingBox;

		// TODO Volume Forces.
	}

	// Add to collision hash.
	if ( ActorForces )
	{
		// Don't block anything.
		if ( !ActorForcesEnabled )
			SetCollision( true, false, false );

		FLOAT X = Max( Abs(BoundingBox.Min.X - Location.X), Abs(BoundingBox.Max.X - Location.X) );
		FLOAT Y = Max( Abs(BoundingBox.Min.X - Location.X), Abs(BoundingBox.Max.X - Location.X) );
		FLOAT Z = Max( Abs(BoundingBox.Min.X - Location.X), Abs(BoundingBox.Max.X - Location.X) );
		
		FLOAT NewEmitterRadius = appSqrt( Square(X) + Square(Y) );
		FLOAT NewEmitterHeight = Z;

		// If new radius or height are bigger than old one increase new one by 20% to decrease
		// frequency of updates.
		if ( (NewEmitterRadius > EmitterRadius) || (NewEmitterHeight > EmitterHeight) )
		{
			if ( NewEmitterRadius > EmitterRadius )
				EmitterRadius = 1.2 * NewEmitterRadius;

			if ( NewEmitterHeight > EmitterHeight )
				EmitterHeight = 1.2 * NewEmitterHeight;

			SetCollisionSize( EmitterRadius, EmitterHeight );
		}

		ActorForcesEnabled = true;
	}
	// Remove from collision hash.
	else if ( ActorForcesEnabled )
	{
		SetCollision( false, false, false );
		ActorForcesEnabled = false;
	}

	// AutoDelete Emitter - Emitters.Num() > 0 as ParticleEmitters don't get auto deleted
	// if global AutoDelete is true.
	if ( !GIsEditor && AllDead && !AllDisabled && Emitters.Num() && AutoDestroy )
	{
		GetLevel()->DestroyActor( this );
		unclock(GStats.DWORDStats(GEngineStats.STATS_Game_ParticleTickCycles));
		return true;
	}

	// AutoReset Emitter.
	if ( AllDead && AutoReset && !AutoDestroy )
	{
		if ( (TimeTillReset-DeltaTime) > 0 )
		{
			TimeTillReset -= DeltaTime;
		}
		else
		{
			GlobalOffset	= GlobalOffsetRange.GetRand();
			TimeTillReset	= TimeTillResetRange.GetRand();
			for (INT i=0; i<Emitters.Num(); i++)
				Emitters(i)->Reset();
		}
	}

	// Pass updated bounding box to visibility code.
	ClearRenderData();

	unclock(GStats.DWORDStats(GEngineStats.STATS_Game_ParticleTickCycles));
	return true;
	unguard;
}

void AEmitter::Render(FDynamicActor* DynActor,FLevelSceneNode* SceneNode,TList<FDynamicLight*>* Lights,FRenderInterface* RI)
{
	guard(AEmitter::Render);

	if( !Initialized )
	{
		Initialize();
		return;
	}

	// Don't render if deleted.
	if( bDeleteMe )
		return;

	INT	RenderStartTime = appCycles(),
		RenderedParticles = 0;

	// Check for projectors if needed.
	if( UseParticleProjectors )
		CheckForProjectors();

	// Render emitters sequentially.
	for (INT i=0; i<Emitters.Num(); i++)
	{
		if ( !Emitters(i) ) 
			continue;
		Emitters(i)->Owner = this;
		RI->PushState();
		if ( !Emitters(i)->Disabled && (SceneNode->Viewport->Actor->ShowFlags & SHOW_Particles))
			RenderedParticles += Emitters(i)->RenderParticles( DynActor, SceneNode, Lights, RI );
		RI->PopState();
	}

	// Render bounding box.
	if( SceneNode->Viewport->bShowBounds )
	{
		FLineBatcher LineBatcher(RI);
		RI->SetTransform(TT_LocalToWorld, FMatrix::Identity);
		LineBatcher.DrawBox( BoundingBox, FColor(255,255,0) );
	}

	GStats.DWORDStats( GEngineStats.STATS_Particle_Particles ) += RenderedParticles;
	GStats.DWORDStats( GEngineStats.STATS_Particle_RenderCycles ) += (appCycles() - RenderStartTime);

	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

