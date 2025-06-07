/*=============================================================================
	UnSpriteEmitter.cpp: Unreal Sprite Emitter
	Copyright 2001 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Daniel Vogel
=============================================================================*/

#include "EnginePrivate.h"

#ifdef __PSX2_EE__  // I apologize profusely - JA
#include "../../PSX2Render/Src/VU1Support.h"
#endif

/*-----------------------------------------------------------------------------
	USpriteEmitter.
-----------------------------------------------------------------------------*/

#define VerticesPerParticle		4
#define IndicesPerParticle		6
#define PrimitivesPerParticle	2


IMPLEMENT_CLASS(USpriteEmitter);

void USpriteEmitter::PostEditChange()
{
	Super::PostEditChange();
	CleanUp();
	Initialize( MaxParticles );
}

// Initializes the Emitter.
void USpriteEmitter::Initialize( INT InMaxParticles )
{
	guard(USpriteEmitter::Initialize);
	Super::Initialize( InMaxParticles );
	RealProjectionNormal = ProjectionNormal.SafeNormal();
	UniformSize = UniformSize || (UseDirectionAs == PTDU_None);
#ifdef __PSX2_EE__
	PreCachePS2Emitter((void**)&PS2Data, MaxParticles, VerticesPerParticle); // we are doing a mini strip
#endif
	unguard;
}

// Update particles.
INT USpriteEmitter::UpdateParticles( FLOAT DeltaTime )
{
	guard(USpriteEmitter::UpdateParticles);
	INT Value	= Super::UpdateParticles( DeltaTime );
	FLOAT ExpandBy = MaxSizeScale;
	if( UniformSize )
		ExpandBy *= StartSizeRange.X.GetMax();
	else
		ExpandBy *= Max(StartSizeRange.X.GetMax(), StartSizeRange.Y.GetMax());
	BoundingBox = BoundingBox.ExpandBy( ExpandBy );
	return Value;
	unguard;
}

// CleanUp.
void USpriteEmitter::CleanUp()
{
	guard(USpriteEmitter::CleanUp);
	Super::CleanUp();
	unguard;
}

//
// FSpriteParticleVertex
//
class FSpriteParticleVertex
{
public:

	FVector	Position;
	FColor	Color;
	FLOAT	U,
			V,
			U2,
			V2;

	static INT GetComponents(FVertexComponent* OutComponents)
	{
		OutComponents[0].Type = CT_Float3;
		OutComponents[0].Function = FVF_Position;
		OutComponents[1].Type = CT_Color;
		OutComponents[1].Function = FVF_Diffuse;
		OutComponents[2].Type = CT_Float2;
		OutComponents[2].Function = FVF_TexCoord0;
		OutComponents[3].Type = CT_Float2;
		OutComponents[3].Function = FVF_TexCoord1;

		return 4;
	}
};

//
// FSpriteParticleVertexStream
//
class FSpriteParticleVertexStream : public FParticleVertexStream<FSpriteParticleVertex>
{
public:

	USpriteEmitter*		Emitter;
	INT					NumParticles;
	FLevelSceneNode*	SceneNode;

	// Constructor.
	FSpriteParticleVertexStream(USpriteEmitter* InEmitter,INT InNumParticles,FLevelSceneNode* InSceneNode)
	{
		Emitter			= InEmitter;
		NumParticles	= InNumParticles;
		SceneNode		= InSceneNode;
	}

	virtual INT GetSize()
	{
		return NumParticles * sizeof(FSpriteParticleVertex) * VerticesPerParticle;
	}

	// Generates the sprite vertices procedurally.
	virtual void GetStreamData(void* Dest)
	{
		INT RenderStartTime = appCycles();
		Emitter->FillVertexBuffer( (FSpriteParticleVertex*) Dest, SceneNode );
		GStats.DWORDStats( GEngineStats.STATS_Particle_SpriteSetupCycles ) += appCycles() - RenderStartTime;
	}
};

//
//
// FSpriteParticleIndexBuffer
class FSpriteParticleIndexBuffer : public FIndexBuffer
{
public:

	INT		NumParticles;
	QWORD	CacheId;

	// Constructor.
	FSpriteParticleIndexBuffer( INT InNumParticles )
	{
		NumParticles	= InNumParticles;
		CacheId			= MakeCacheID(CID_RenderIndices);
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
		return NumParticles * sizeof(_WORD) * IndicesPerParticle;
	}

	virtual INT GetIndexSize()
	{
		return sizeof(_WORD);
	}

	virtual void GetContents(void* Data)
	{
		_WORD* WordData = (_WORD*)Data;
		for(INT VerticesIndex=0; VerticesIndex<NumParticles; VerticesIndex++)
		{
			*(WordData++) = 4*VerticesIndex+0;
			*(WordData++) = 4*VerticesIndex+1;
			*(WordData++) = 4*VerticesIndex+2;
			*(WordData++) = 4*VerticesIndex+2;
			*(WordData++) = 4*VerticesIndex+1;
			*(WordData++) = 4*VerticesIndex+3;
		}
	}
};



// Fills vertex buffer with data - called indirectly.
INT USpriteEmitter::FillVertexBuffer( FSpriteParticleVertex* Vertices, FLevelSceneNode* SceneNode )
{
	FVector ViewLocation = SceneNode->ViewOrigin;

	FVector ProjBase     = SceneNode->Deproject(FPlane(0,0,0,1));	
	FVector ProjUp		 = SceneNode->Deproject(FPlane(0,-1000,0,1)) - ProjBase;
	FVector ProjRight	 = SceneNode->Deproject(FPlane(1000,0,0,1)) - ProjBase;
	FVector ProjFront	 = ProjRight ^ ProjUp;

	// Transform coordinate system.
	if ( CoordinateSystem == PTCS_Relative )
	{
		ViewLocation	= ViewLocation	.TransformPointBy ( GMath.UnitCoords / Owner->Rotation / Owner->Location );
		ProjUp			= ProjUp		.TransformVectorBy( GMath.UnitCoords / Owner->Rotation );
		ProjRight		= ProjRight		.TransformVectorBy( GMath.UnitCoords / Owner->Rotation );
		ProjFront		= ProjFront		.TransformVectorBy( GMath.UnitCoords / Owner->Rotation );
	} 
	
	// Rotate normal if actor gets rotated.
	FVector RotatedProjectionNormal = RealProjectionNormal;
	if ( UseRotationFrom == PTRS_Actor && Owner)
		RotatedProjectionNormal = RotatedProjectionNormal.TransformVectorBy( GMath.UnitCoords * Owner->Rotation );

	// Better safe than sorry.
	ProjUp.Normalize();
	ProjRight.Normalize();
	ProjFront.Normalize();

#ifdef __PSX2_EE__
	FVU1ParticleData* PatchData = ((FVU1ParticleData*)PS2Data);
	FVU1ParticlePatch* Patch = PatchData->CurrentBuffer;
	INT PatchVertexIndex = 0;
#else
	FSpriteParticleVertex* Vertex = &Vertices[0];
#endif

	for(INT Index=0; Index<ActiveParticles; Index++)
	{
		FParticle* Particle = &Particles(Index);

		if(!(Particle->Flags & PTF_Active))
			continue;
		
		// Color.
		FColor Color = Particle->Color;

		// Time.
		FLOAT Time			= Particle->Time;
		FLOAT RelativeTime;
		if ( Particle->MaxLifetime )
			RelativeTime	= Clamp( Time / Particle->MaxLifetime, 0.f, 1.f );
		else
			RelativeTime	= 0.f;

		// Projection code.
		FVector Up;
		FVector Right;
		FVector ProjTemp;
		FVector Direction;

		if ( UseDirectionAs == PTDU_Normal )
		{
			Direction = RotatedProjectionNormal;
		}
		else if ( UseDirectionAs == PTDU_Forward )
		{
			Direction = Particle->Location - Particle->OldLocation;
			Direction.Normalize();
		}
		else
		{
			Direction = Particle->Location - Particle->OldLocation;
			Direction.Normalize();
			if ( (UseDirectionAs == PTDU_UpAndNormal) || (UseDirectionAs == PTDU_RightAndNormal) )
			{
				ProjTemp = Direction ^ RealProjectionNormal;
				//ProjTemp.Normalize();
			}
			else
			{
				ProjTemp = Direction ^ (Particle->Location - ViewLocation);
				ProjTemp.Normalize();
			}
		}

		FVector RotationAxis = ProjFront;
		switch ( UseDirectionAs )
		{
		case PTDU_None:
			Up	  = ProjUp    * Particle->Size.X; //!!(Particle->Size.Y ? Particle->Size.Y : Particle->Size.X);
			Right = ProjRight * Particle->Size.X;
			break;
		case PTDU_Scale:
			Up	  = ProjUp    * Particle->Size.Y;
			Right = ProjRight * Particle->Size.X;
			break;
		case PTDU_Up:
		case PTDU_UpAndNormal:
			Up	  = Direction * Particle->Size.Y;
			Right = ProjTemp  * Particle->Size.X;
			break;
		case PTDU_Right:
		case PTDU_RightAndNormal:
			Up    = ProjTemp  * Particle->Size.Y;
			Right = Direction * Particle->Size.X;
			break;
		case PTDU_Forward:
		case PTDU_Normal:
			Up    = (Direction ^ Direction.GetNonParallel()) * Particle->Size.Y;
			Right = (Direction ^ Up).SafeNormal() * Particle->Size.X;
			RotationAxis = Direction;
			break;
		default:
			Up	  = FVector(0,0,0);
			Right = FVector(0,0,0);
			break;
		}

		// Rotation/ Spinning.
		if ( SpinParticles )
		{
			INT Angle;
			if ( Particle->SpinsPerSecond.X < 0 )
				Angle = 0xFFFF - (appRound(Particle->StartSpin.X + Time * -Particle->SpinsPerSecond.X) & 0xFFFF);
			else
				Angle = appRound(Particle->StartSpin.X + Time * Particle->SpinsPerSecond.X) & 0xFFFF;

//			RotationAxis = Up ^ Right;
//			RotationAxis.Normalize();
			Up    =    Up.RotateAngleAxis(Angle, RotationAxis);
			Right = Right.RotateAngleAxis(Angle, RotationAxis);
		}

		// UV generation.
		FLOAT UMin  = 0; FLOAT UMax  = 1; FLOAT VMin  = 0; FLOAT U2Min = 0;
		FLOAT U2Max = 1; FLOAT VMax  = 1; FLOAT V2Min = 0; FLOAT V2Max = 1;

		FLOAT FU = 1.f;
		FLOAT FV = 1.f;
		INT Section = Particle->Subdivision;

		if (TextureUSubdivisions && TextureVSubdivisions)
		{
			FU	= 1.f / TextureUSubdivisions;
			FV	= 1.f / TextureVSubdivisions;

			if ( Section != -1 )
			{
				VMin  = (Section % TextureVSubdivisions) * FV;
				VMax  = Min( VMin + FV, 1.f );

				UMin  = (Section / TextureVSubdivisions) * FU;
				UMax  = Min( UMin + FU, 1.f );
			}
			else if ( Particle->MaxLifetime )
			{
				INT SubDivs	= TextureUSubdivisions * TextureVSubdivisions;
			
				if (UseSubdivisionScale)
				{
					INT t;
					for (t=0; t<SubdivisionScale.Num(); t++)
						if (RelativeTime <= SubdivisionScale(t))
							break;
					Section = t;
				}
				else if (SubdivisionEnd)
				{
					SubDivs = Max( 1, SubdivisionEnd - SubdivisionStart );
					Section	= appFloor(RelativeTime * SubDivs) + SubdivisionStart;
				}
				else
					Section	= appFloor(RelativeTime * SubDivs);
				
				Section = Clamp( Section, 0, TextureUSubdivisions * TextureVSubdivisions - 1 );	

				VMin  = (Section % TextureVSubdivisions) * FV;
				VMax  = Min( VMin + FV, 1.f );

				UMin  = (Section / TextureVSubdivisions) * FU;
				UMax  = Min( UMin + FU, 1.f );
			
				if ( BlendBetweenSubdivisions )
				{
					if ( ++Section != SubDivs )
					{
						V2Min  = (Section % TextureVSubdivisions) * FV;
						V2Max  = V2Min + FV;
	
						U2Min  = (Section / TextureVSubdivisions) * FU;
						U2Max  = U2Min + FU;
					}
					else
					{
						V2Min = VMin;
						V2Max = VMax;
						U2Min = UMin;
						U2Max = UMax;
					}
					FLOAT Alpha;
					//TODO: move range check to PostEditChange?
					if ( UseSubdivisionScale && (SubdivisionScale.Num() > SubDivs) )
					{
						FLOAT Offset;
						if ( --Section == 0 )
							Offset = 0.f;
						else
							Offset = SubdivisionScale( Section - 1 );
						FLOAT Length = Min(1.f, SubdivisionScale( Section )) - Offset;
						Alpha  = (RelativeTime - Offset ) / Length;
					}
					else
					{
						FLOAT Temp  = RelativeTime * SubDivs;
						Alpha = Temp - appFloor(Temp);
					}
					Color.A = (BYTE)(255 * Alpha);
				}
				Section--;
			}
		}	

		Color = Color.RenderColor();

		FVector ParticleLocation = Particle->Location;
		
		// TODO: add option to flip up

#ifdef __PSX2_EE__
		{
		FVU1ParticleVertex& PVertex = Patch->Vertices[PatchVertexIndex++];
		FVector Pos = ParticleLocation - Up - Right;

		PVertex.Pos.X = Pos.X;
		PVertex.Pos.Y = Pos.Y; 
		PVertex.Pos.Z = Pos.Z;
		PVertex.Color.ui32[2] = Color.R>>1;
		PVertex.Color.ui32[1] = Color.G>>1;
		PVertex.Color.ui32[0] = Color.B>>1;
		PVertex.Color.ui32[3] = Color.A>>1;
		PVertex.UV.X = UMin;
		PVertex.UV.Y = VMax;
		}

		{
		FVU1ParticleVertex& PVertex = Patch->Vertices[PatchVertexIndex++];
		FVector Pos = ParticleLocation - Up + Right;

		PVertex.Pos.X = Pos.X;
		PVertex.Pos.Y = Pos.Y; 
		PVertex.Pos.Z = Pos.Z;
		PVertex.Color.ui32[2] = Color.R>>1;
		PVertex.Color.ui32[1] = Color.G>>1;
		PVertex.Color.ui32[0] = Color.B>>1;
		PVertex.Color.ui32[3] = Color.A>>1;
		PVertex.UV.X = UMax;
		PVertex.UV.Y = VMax;
		}

		{
		FVU1ParticleVertex& PVertex = Patch->Vertices[PatchVertexIndex++];
		FVector Pos = ParticleLocation + Up - Right;

		PVertex.Pos.X = Pos.X;
		PVertex.Pos.Y = Pos.Y; 
		PVertex.Pos.Z = Pos.Z;
		PVertex.Color.ui32[2] = Color.R>>1;
		PVertex.Color.ui32[1] = Color.G>>1;
		PVertex.Color.ui32[0] = Color.B>>1;
		PVertex.Color.ui32[3] = Color.A>>1;
		PVertex.UV.X = UMin;
		PVertex.UV.Y = VMin;
		}

		{
		FVU1ParticleVertex& PVertex = Patch->Vertices[PatchVertexIndex++];
		FVector Pos = ParticleLocation + Up + Right;

		PVertex.Pos.X = Pos.X;
		PVertex.Pos.Y = Pos.Y; 
		PVertex.Pos.Z = Pos.Z;
		PVertex.Color.ui32[2] = Color.R>>1;
		PVertex.Color.ui32[1] = Color.G>>1;
		PVertex.Color.ui32[0] = Color.B>>1;
		PVertex.Color.ui32[3] = Color.A>>1;
		PVertex.UV.X = UMax;
		PVertex.UV.Y = VMin;
		}

		if (PatchVertexIndex + 4 > P_MAX_VERTICES_PER_PATCH)
		{
			// set the length of this in vertices
			Patch->Params.ui32[0] = PatchVertexIndex;

			// tell it that we are a "cnt" type of tag, not an "end"

			Patch->DMATag.ui32[0] &= ~0x70000000;
			Patch->DMATag.ui32[0] |= 0x10000000;

			Patch++;
			PatchVertexIndex = 0;
		}

#else
		Vertex->Position	= ParticleLocation - Up - Right;
		Vertex->Color		= Color;
		Vertex->U			= UMin;
		Vertex->U2			= U2Min;
		Vertex->V			= VMax;
		Vertex->V2			= V2Max;
		Vertex++;

		Vertex->Position	= ParticleLocation - Up + Right;
		Vertex->Color		= Color;
		Vertex->U			= UMax;
		Vertex->U2			= U2Max;
		Vertex->V			= VMax;
		Vertex->V2			= V2Max;
		Vertex++;
			
		Vertex->Position	= ParticleLocation + Up - Right;
		Vertex->Color		= Color;
		Vertex->U			= UMin;
		Vertex->U2			= U2Min;
		Vertex->V			= VMin;
		Vertex->V2			= V2Min;
		Vertex++;

		Vertex->Position	= ParticleLocation + Up + Right;
		Vertex->Color		= Color;
		Vertex->U			= UMax;
		Vertex->U2			= U2Max;
		Vertex->V			= VMin;
		Vertex->V2			= V2Min;
		Vertex++;
#endif
	}

#ifdef __PSX2_EE__
		if (PatchVertexIndex == 0)
			Patch--;

		// TODO: MAKE PatchVertexIndex INTO Patch->Params.ui32[0] EVERYWHERE!!!!!!!!!!!!!!
		// set the length of this in vertices
		Patch->Params.ui32[0] = PatchVertexIndex;

		// tell it that we are a "end" type of tag, not an "cnt"
#ifdef NO_DEFERRED_RENDERING
		Patch->DMATag.ui32[0] |= 0x70000000;
#else
		Patch->DMATag.ui32[0] &= ~0x70000000;
		Patch->DMATag.ui32[0] |= 0x60000000;
#endif
#endif
	return 0;
}


// Renders all active particles
INT USpriteEmitter::RenderParticles(FDynamicActor* DynActor,FLevelSceneNode* SceneNode,TList<FDynamicLight*>* Lights,FRenderInterface* RI)
{
	guard(USpriteEmitter::RenderParticles);

	// Handles clock'ing and other high level options.
	UParticleEmitter::RenderParticles( DynActor, SceneNode, Lights, RI );

	if ( Texture == NULL )
		return 0;
	
	INT RenderedParticles = 0;

	// Render all active particles.
	if( ActiveParticles > 0 )
	{
		// Calculate the number of active particles.
		for(INT ParticleIndex = 0;ParticleIndex < ActiveParticles;ParticleIndex++)
		{
			FParticle*	Particle = &Particles(ParticleIndex);

			if(Particle->Flags & PTF_Active)
				RenderedParticles++;
		}

		// Bail out if nothing to render.
		if (!RenderedParticles)
			return 0;

		// Create the dynamic vertex stream and index buffer.
		FSpriteParticleVertexStream SpriteParticleVertices(this,RenderedParticles,SceneNode);
		FSpriteParticleIndexBuffer  SpriteParticleIndices(RenderedParticles);

		// Rotate & translate if needed.
		if ( CoordinateSystem == PTCS_Relative )
		{
			FMatrix	LocalToWorld = FRotationMatrix(Owner->Rotation) * FTranslationMatrix(Owner->Location);
			RI->SetTransform(TT_LocalToWorld,LocalToWorld);
		}
		else
			RI->SetTransform(TT_LocalToWorld,FMatrix::Identity);

		// Set texture and blending.
		Owner->ParticleMaterial->ParticleBlending			= DrawStyle;
		if( SceneNode->Viewport->Actor->RendMap == REN_LightingOnly )
			Owner->ParticleMaterial->BitmapMaterial			= NULL;
		else
			Owner->ParticleMaterial->BitmapMaterial			= Texture;
		Owner->ParticleMaterial->BlendBetweenSubdivisions	= BlendBetweenSubdivisions && !UseRandomSubdivision;
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

#ifdef __PSX2_EE__

		// Sort of a weird hack to fill out the PS2Data
		SpriteParticleVertices.GetStreamData(NULL);
		FVU1ParticleData* PatchData = ((FVU1ParticleData*)PS2Data);

		extern void PSX2Render_RenderEmitter(FVU1ParticlePatch* Patches, UBOOL LineList, EParticleDrawStyle DrawStyle);
		PSX2Render_RenderEmitter(PatchData->CurrentBuffer, false, (EParticleDrawStyle)DrawStyle);
		PatchData->SwapBuffers();
		
#else //PSX2_EE
		// Set the particle vertex stream and index buffer. The particle vertices aren't actually 
		// generated until now.
		INT	BaseVertexIndex = RI->SetDynamicStream(VS_FixedFunction,&SpriteParticleVertices),
			BaseIndex		= RI->SetDynamicIndexBuffer(&SpriteParticleIndices,BaseVertexIndex);

		RI->DrawPrimitive(
			PT_TriangleList, 
			BaseIndex,
			RenderedParticles * PrimitivesPerParticle,
			0,
			RenderedParticles * VerticesPerParticle - 1
			);
#endif
	}

	return RenderedParticles;

	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

