/*=============================================================================
	UnTrailEmitter.cpp: Unreal Trail Emitter
	Copyright 2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Daniel Vogel
=============================================================================*/

#include "EnginePrivate.h"

/*-----------------------------------------------------------------------------
	UTrailEmitter.
-----------------------------------------------------------------------------*/

IMPLEMENT_CLASS(UTrailEmitter);

void UTrailEmitter::PostEditChange()
{
	Super::PostEditChange();
	MaxPointsPerTrail = Max(3, MaxPointsPerTrail);
	CleanUp();
	Initialize( MaxParticles );
}

// Initializes the Emitter.
void UTrailEmitter::Initialize( INT InMaxParticles )
{
	guard(UTrailEmitter::Initialize);
	Super::Initialize( InMaxParticles );
	TrailData.Add( InMaxParticles * MaxPointsPerTrail );
	TrailInfo.Add( InMaxParticles );
	unguard;
}

// CleanUp.
void UTrailEmitter::CleanUp()
{
	guard(UTrailEmitter::CleanUp);
	Super::CleanUp();
	TrailData.Empty();
	TrailInfo.Empty();
	unguard;
}

// Spawn particle.
void UTrailEmitter::SpawnParticle( INT Index, FLOAT SpawnTime, INT Flags, INT SpawnFlags, const FVector& LocalLocationOffset )
{
	guard(UTrailEmitter::SpawnParticle);
	Super::SpawnParticle( Index, SpawnTime, Flags, SpawnFlags, LocalLocationOffset );
	TrailInfo(Index).TrailIndex		= 1;
	TrailInfo(Index).NumPoints		= 2;
	TrailInfo(Index).LastLocation	= Particles(Index).Location;
	
	TrailData(Index * MaxPointsPerTrail + 0).Location	= Particles(Index).Location;
	TrailData(Index * MaxPointsPerTrail + 0).Color		= Particles(Index).Color;
	TrailData(Index * MaxPointsPerTrail + 0).Size		= Particles(Index).Size.X;
	TrailData(Index * MaxPointsPerTrail + 0).Time		= Particles(Index).Time;

	TrailData(Index * MaxPointsPerTrail + 1).Location	= Particles(Index).Location;
	TrailData(Index * MaxPointsPerTrail + 1).Color		= Particles(Index).Color;
	TrailData(Index * MaxPointsPerTrail + 1).Size		= Particles(Index).Size.X;
	TrailData(Index * MaxPointsPerTrail + 1).Time		= Particles(Index).Time;
	unguard;
}

// Update particles.
INT UTrailEmitter::UpdateParticles( FLOAT DeltaTime )
{
	guard(UTrailEmitter::UpdateParticles);
	INT Value	= Super::UpdateParticles( DeltaTime );
	BoundingBox = BoundingBox.ExpandBy( MaxSizeScale );
	return Value;
	unguard;
}

void UTrailEmitter::UpdateParticle( FLOAT DeltaTime, INT Index )
{
	FParticle& Particle = Particles(Index);
	INT TrailIndex		= TrailInfo(Index).TrailIndex;
	INT NumPoints		= TrailInfo(Index).NumPoints;
	INT Offset			= MaxPointsPerTrail * Index;

	// Generate new point if particle moved further than distance threshold.
	if( FDistSquared(Particle.Location, TrailInfo(Index).LastLocation) >= Square(DistanceThreshold) )
	{
		// Wrap around if already at points limit.
		if( ++TrailIndex == MaxPointsPerTrail )
			TrailIndex = 0;
		
		NumPoints = Min( NumPoints+1, MaxPointsPerTrail );

		TrailInfo(Index).LastLocation = Particle.Location;
	}	
	
	TrailData(Offset + TrailIndex).Location	= Particle.Location;
	TrailData(Offset + TrailIndex).Color	= Particle.Color;
	TrailData(Offset + TrailIndex).Size		= Particle.Size.X;
	TrailData(Offset + TrailIndex).Time		= Owner->Level->TimeSeconds;

	TrailInfo(Index).NumPoints	= NumPoints;
	TrailInfo(Index).TrailIndex = TrailIndex;
}


struct FTrailVertex
{
	FVector	Position;
	FColor	Diffuse;
	FLOAT	U,
			V;

	static INT GetComponents(FVertexComponent* OutComponents)
	{
		OutComponents[0].Type		= CT_Float3;
		OutComponents[0].Function	= FVF_Position;
		OutComponents[1].Type		= CT_Color;
		OutComponents[1].Function	= FVF_Diffuse;
		OutComponents[2].Type		= CT_Float2;
		OutComponents[2].Function	= FVF_TexCoord0;

		return 3;
	}
};

class FTrailVertexStream : public FParticleVertexStream<FTrailVertex>
{
public:

	UTrailEmitter*				Emitter;
	INT							NumVerts;
	FSceneNode*					SceneNode;

	// Constructor.
	FTrailVertexStream( UTrailEmitter* InEmitter,INT InNumVerts, FSceneNode* InSceneNode )
	{
		Emitter			= InEmitter;
		NumVerts		= InNumVerts;
		SceneNode		= InSceneNode;
	}

	virtual INT GetSize()
	{
		return NumVerts * sizeof(FTrailVertex);
	}

	// Generates the spark vertices procedurally.
	virtual void GetStreamData(void* Dest)
	{
		FVector ViewLocation = SceneNode->ViewOrigin;

		FVector ProjBase     = SceneNode->Deproject(FPlane(0,0,0,1));	
		FVector ProjUp		 = SceneNode->Deproject(FPlane(0,-1000,0,1)) - ProjBase;
		FVector ProjRight	 = SceneNode->Deproject(FPlane(1000,0,0,1)) - ProjBase;
		FVector ProjFront	 = ProjRight ^ ProjUp;

		// Better safe than sorry.
		ProjUp.Normalize();
		ProjRight.Normalize();
		ProjFront.Normalize();

		FTrailVertex*	Vertex = (FTrailVertex*) Dest;
		for(INT ParticleIndex = 0;ParticleIndex < Emitter->ActiveParticles;ParticleIndex++)
		{
			FParticle*	Particle = &Emitter->Particles(ParticleIndex);

			if( !(Particle->Flags & PTF_Active) )
				continue;

			INT NumPoints = Emitter->TrailInfo(ParticleIndex).NumPoints;
			if( NumPoints <= 1 )
				continue;

			// Do a second pass for crossed sheets.
			for( INT PassCount=0; PassCount<(Emitter->UseCrossedSheets ? 2 : 1); PassCount++ )
			{	
				INT TrailIndex = Emitter->TrailInfo(ParticleIndex).TrailIndex;

				// Setup last, current and next location and wrap around index if necessary.
				FVector LastLocation	= FVector(0,0,0);
				FVector Location		= Emitter->TrailData(ParticleIndex * Emitter->MaxPointsPerTrail + TrailIndex).Location;
				
				// Setup size and color.
				FLOAT	Size			= Emitter->TrailData(ParticleIndex * Emitter->MaxPointsPerTrail + TrailIndex).Size;
				FColor	Color			= Emitter->TrailData(ParticleIndex * Emitter->MaxPointsPerTrail + TrailIndex).Color;

				// Setup next location and wrap around if necessary.
				if( --TrailIndex < 0 )
					TrailIndex += Emitter->MaxPointsPerTrail;			
				FVector NextLocation	= Emitter->TrailData(ParticleIndex * Emitter->MaxPointsPerTrail + TrailIndex).Location;

				Color = Color.RenderColor();

				// Generate vertices.
				for( INT i=0; i<NumPoints; i++ )
				{
					FVector	Tangent;

					// No LastLocation.
					if( i == 0 )
					{
						Tangent			= (NextLocation - Location).SafeNormal();
					}
					// No NextLocation.
					else
					if( i == NumPoints-1 )
					{
						Tangent			= (Location - LastLocation).SafeNormal();
					}
					// Both Last and NextLocation are available.
					else
					{
						FVector LastDir	= Location - LastLocation;
						FVector NextDir	= NextLocation - Location;		
						Tangent			= ((LastDir + NextDir) / 2.f).SafeNormal();
					}

					// Twisted system.
					FVector ProjFront	= (Location - ViewLocation).SafeNormal();
					FVector Right		= (FVector(0,0,1) ^ Tangent).SafeNormal();
					FLOAT Angle			= Emitter->UseCrossedSheets ? 16384 : Emitter->MaxTrailTwistAngle * (Right | ProjFront);
					FVector Up			= !Emitter->UseCrossedSheets || PassCount ? Right.RotateAngleAxis( Angle, Tangent ).SafeNormal() : Right;
					Up					*= Size;
					
					Vertex->Position	= Location + Up;
					Vertex->Diffuse		= Color;
					Vertex->U			= i / (FLOAT) NumPoints;
					Vertex->V			= 1.f;
					Vertex++;

					Vertex->Position	= Location - Up;
					Vertex->Diffuse		= Color;
					Vertex->U			= i / (FLOAT) NumPoints;
					Vertex->V			= 0.f;
					Vertex++;

					// Update last and current location.
					LastLocation		= Location;
					Location			= NextLocation;
					
					// Update size and color.
					Size				= Emitter->TrailData(ParticleIndex * Emitter->MaxPointsPerTrail + TrailIndex).Size;
					Color				= Emitter->TrailData(ParticleIndex * Emitter->MaxPointsPerTrail + TrailIndex).Color;

					// Update next location and wrap around if necessary.
					if( --TrailIndex < 0 )
						TrailIndex += Emitter->MaxPointsPerTrail;
					NextLocation		=  Emitter->TrailData(ParticleIndex * Emitter->MaxPointsPerTrail + TrailIndex).Location;

				}
			}
		}
	}
};

// FTrailIndexBuffer
class FTrailIndexBuffer : public FIndexBuffer
{
public:

	UTrailEmitter*	Emitter;
	INT				NumIndices;
	QWORD			CacheId;

	// Constructor.
	FTrailIndexBuffer( UTrailEmitter* InEmitter, INT InNumIndices )
	{
		Emitter		= InEmitter;
		NumIndices	= InNumIndices;
		CacheId		= MakeCacheID(CID_RenderIndices);
	}

	virtual QWORD GetCacheId()
	{
		return CacheId;
	}

	virtual INT GetRevision()
	{
		return 1;
	}

	virtual INT GetSize()
	{
		return NumIndices * sizeof(_WORD);
	}

	virtual INT GetIndexSize()
	{
		return sizeof(_WORD);
	}

	virtual void GetContents(void* Data)
	{
		_WORD* WordData = (_WORD*)Data;
		INT VertexIndex = 0;

		for(INT ParticleIndex = 0;ParticleIndex < Emitter->ActiveParticles;ParticleIndex++)
		{
			FParticle*	Particle = &Emitter->Particles(ParticleIndex);

			if( !(Particle->Flags & PTF_Active) )
				continue;

			if( Emitter->TrailInfo(ParticleIndex).NumPoints <= 1 )
				continue;

			// Do a second pass for crossed sheets.
			for( INT PassCount=0; PassCount<(Emitter->UseCrossedSheets ? 2 : 1); PassCount++ )
			{
				for( INT i=0; i<Emitter->TrailInfo(ParticleIndex).NumPoints-1; i++ )
				{
					*(WordData++) = 2*VertexIndex+0;
					*(WordData++) = 2*VertexIndex+1;
					*(WordData++) = 2*VertexIndex+2;
					*(WordData++) = 2*VertexIndex+2;
					*(WordData++) = 2*VertexIndex+1;
					*(WordData++) = 2*VertexIndex+3;
					VertexIndex++;
				}
				VertexIndex++;
			}
		}
	}
};


// Renders all active particles
INT UTrailEmitter::RenderParticles(FDynamicActor* DynActor,FLevelSceneNode* SceneNode,TList<FDynamicLight*>* Lights,FRenderInterface* RI)
{
	guard(UTrailEmitter::RenderParticles);

	UParticleEmitter::RenderParticles( DynActor, SceneNode, Lights, RI );

	if( Texture == NULL )
		return 0;

	INT RenderedParticles = 0;

	// Render the particles.
	if( ActiveParticles > 0 )
	{
		// Calculate the number of active particles.
		INT NumVerts	= 0,
			NumTris		= 0,
			NumIndices	= 0;
		for( INT ParticleIndex = 0;ParticleIndex < ActiveParticles;ParticleIndex++ )
		{
			FParticle* Particle = &Particles(ParticleIndex);

			if(Particle->Flags & PTF_Active)
			{
				INT NumPoints = TrailInfo(ParticleIndex).NumPoints;
				if( NumPoints > 1 )
				{
					RenderedParticles++;
					NumTris		+= NumPoints * 2 - 2;
					NumVerts	+= NumPoints * 2;
					NumIndices	+= NumPoints * 6 - 6;
				}
			}
		}

		// Bail out if nothing to render.
		if( !RenderedParticles || !NumVerts || !NumIndices )
			return 0;

		//!!vogel: TODO 
		if( UseCrossedSheets )
		{
			NumTris		*= 2;
			NumVerts	*= 2;
			NumIndices	*= 2;
		}

		// Create the dynamic vertex stream and index buffer.
		FTrailVertexStream TrailVertices(this,NumVerts,SceneNode);
		FTrailIndexBuffer  TrailIndices(this,NumIndices);

		// Rotate & translate if needed.
		RI->SetTransform(TT_LocalToWorld,FMatrix::Identity);

		// Set texture and blending.
		Owner->ParticleMaterial->ParticleBlending			= DrawStyle;
		if( SceneNode->Viewport->Actor->RendMap == REN_LightingOnly )
			Owner->ParticleMaterial->BitmapMaterial			= NULL;
		else
			Owner->ParticleMaterial->BitmapMaterial			= Texture;
		Owner->ParticleMaterial->BlendBetweenSubdivisions	= 0;
		Owner->ParticleMaterial->RenderTwoSided				= 1;
		Owner->ParticleMaterial->UseTFactor					= 0;
		Owner->ParticleMaterial->AlphaTest					= AlphaTest;
		Owner->ParticleMaterial->AlphaRef					= AlphaRef;
		Owner->ParticleMaterial->AcceptsProjectors			= AcceptsProjectors;
		Owner->ParticleMaterial->ZTest						= ZTest;
		Owner->ParticleMaterial->ZWrite						= ZWrite;
		Owner->ParticleMaterial->Wireframe					= SceneNode->Viewport->IsWire();
		
		RI->EnableLighting(0,1);
		RI->SetMaterial(Owner->ParticleMaterial);

		// Set the particle vertex stream and index buffer.
		// The particle vertices aren't actually generated until now.
		INT	BaseVertexIndex = RI->SetDynamicStream(VS_FixedFunction,&TrailVertices),
			BaseIndex		= RI->SetDynamicIndexBuffer(&TrailIndices,BaseVertexIndex);

		RI->DrawPrimitive(
			PT_TriangleList, 
			BaseIndex,
			NumTris,
			0,
			NumVerts - 1
		);
	}

	return RenderedParticles;

	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

