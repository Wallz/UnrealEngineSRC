//=============================================================================
// AxWeatherEffect - implementation
// Copyright 2001 Digital Extremes - All Rights Reserved.
// Confidential.
//=============================================================================

#include "EnginePrivate.h"
#include "UnRender.h"
#include "xParticleMgr.h"

const BYTE PCL_NOT_VIS_MASK = 0x80;
const BYTE PCL_NO_DRAW      = 0x40;
const float InvWeatherParticleMass = 1.0f / 0.1f;

#define C_RGBA(r, g, b, a)	(((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

inline static float LerpInOut( float ratio, float x )
{
	if ( x < ratio )
	{
		float t = 1.0f / ratio;
		return x * t;
	}
	else
	{
		float t = 1.0f / (1.0f - ratio);
		return t * (1.0f - x);
	}
}

static float tmp_numCols = 4.0f;    
static float tmp_numRows = 4.0f;
static float tmp_texU = 0.0f;
static float tmp_texV = 0.0f;
static float tmp_ScaleGlow = 1.0f;
static FVector tmp_Color = FVector(255.0f,255.0f,255.0f);

inline static void CalcUV( float& tu, float& tv, float& tmp_texU, float& tmp_texV, int frame )
{
	int col = frame / (int)tmp_numCols;
    int row = frame - ((int)tmp_numRows * col);
    tu = (float)row * tmp_texU;
    tv = (float)col * tmp_texV;
}

inline static int RenderSpritePcl( DynamicVertex*& pVerts, FWeatherPcl& pcl, FMatrix& mat )
{
	float tu, tv;
	FVector center = mat.TransformFVector(pcl.pos);
	float t = pcl.Life;

    t = LerpInOut( 0.05f, t );
	t *= Max( pcl.Size * 0.5f, 1.0f );
    t *= pcl.DistAtten;
    if ( t < 0.0f )
        t = 0.0f;
    if ( t > 1.0f )
        t = 1.0f;

    FVector fclr = tmp_Color;
    fclr *= t * tmp_ScaleGlow;
    fclr *= 255.0f;

	DWORD clr = C_RGBA( (int)(fclr.X), (int)(fclr.Y), (int)(fclr.Z), (int)(255.0f * t) );
	FVector u(pcl.Size,0,0);
	FVector v(0,pcl.Size,0);
	CalcUV( tu, tv, tmp_texU, tmp_texV, pcl.frame );

	pVerts->Point = center + v;
	pVerts->Color = clr;
	pVerts->U = tu;
	pVerts->V = tv;
	pVerts++;

	pVerts->Point = center - u;
	pVerts->Color = clr;
	pVerts->U = tu;
	pVerts->V = tv + tmp_texV;
	pVerts++;

	pVerts->Point = center - v;
	pVerts->Color = clr;
	pVerts->U = tu + tmp_texU;
	pVerts->V = tv + tmp_texV;
	pVerts++;

	pVerts->Point = center + u;
	pVerts->Color = clr;
	pVerts->U = tu + tmp_texU;
	pVerts->V = tv;
	pVerts++;


	return 4;
}

inline static int RenderLinePcl( DynamicVertex*& pVerts, FWeatherPcl& pcl, FMatrix& mat )
{
    float tu, tv;
	FVector va = mat.TransformFVector(pcl.pos);
	float t = pcl.Life;

    t = LerpInOut( 0.05f, t );

	t *= Max( pcl.Size * 0.5f, 1.0f );
    t *= pcl.DistAtten;
    if ( t < 0.0f )
        t = 0.0f;
    if ( t > 1.0f )
        t = 1.0f;

    FVector fclr = tmp_Color;
    fclr *= t * tmp_ScaleGlow;
    fclr *= 255.0f;

	DWORD clr = C_RGBA( (int)(fclr.X), (int)(fclr.Y), (int)(fclr.Z), (int)(255.0f * t) );
	FVector u(pcl.Size,0,0);
	FVector v(0,pcl.Size,0);
	CalcUV( tu, tv, tmp_texU, tmp_texV, pcl.frame );

    FVector vb = mat.TransformFVector(pcl.pos + pcl.Vel*.03f);

    pVerts->Point = va + u;
	pVerts->Color = clr;
	pVerts->U = tu + tmp_texU;
	pVerts->V = tv;
	pVerts++;

	pVerts->Point = va - u;
	pVerts->Color = clr;
	pVerts->U = tu;
	pVerts->V = tv;
	pVerts++;

	pVerts->Point = vb - u;
	pVerts->Color = clr;
	pVerts->U = tu;
	pVerts->V = tv + tmp_texV;
	pVerts++;

	pVerts->Point = vb + u;
	pVerts->Color = clr;
	pVerts->U = tu + tmp_texU;
	pVerts->V = tv + tmp_texV;
	pVerts++;

    return 4;
}


class FWeatherQuadStream : public FVertexStream
{
public:
    AxWeatherEffect*  pFx;
	QWORD		CacheId;
    int         Revision;
    FMatrix     WorldToCamera;
    int         numQuads;
    int         renderable;
    void Init( AxWeatherEffect* pInFx, int inRenderable, FMatrix& inWorldToCamera )
    {
        pFx = pInFx;
        WorldToCamera = inWorldToCamera;
        Revision++;
        renderable = inRenderable;
    }

	FWeatherQuadStream()
	{
		CacheId = MakeCacheID(CID_RenderVertices);
        pFx = NULL;
        Revision = 1;    
        numQuads = 0;
        renderable = 0;
    }

	virtual QWORD GetCacheId()
	{
		return CacheId;
	}

	virtual INT GetRevision()
	{
		return Revision;
	}

	virtual INT GetSize()
	{
		return renderable * sizeof(DynamicVertex);
	}

	virtual INT GetStride()
	{
		return sizeof(DynamicVertex);
	}

	virtual INT GetComponents(FVertexComponent* OutComponents)
	{
		OutComponents[0].Type = CT_Float3;
		OutComponents[0].Function = FVF_Position;
        OutComponents[1].Type = CT_Color;
		OutComponents[1].Function = FVF_Diffuse;
        OutComponents[2].Type = CT_Float2;
		OutComponents[2].Function = FVF_TexCoord0;
        return 3;
	}

    virtual void GetStreamData(void* Dest)
    {
	    DynamicVertex* pOutVerts = (DynamicVertex*)Dest;
        numQuads = 0;
        FWeatherPcl* p = &pFx->pcl(0);
        int numPcl = pFx->pcl.Num();
        for ( int i=0; i<numPcl; i++ )
	    {
		    if ( p->frame & PCL_NOT_VIS_MASK )
            {
                p->frame &= ~PCL_NOT_VIS_MASK;
                p++;
			    continue;
            }

            if ( pFx->WeatherType == WT_Rain )
                RenderLinePcl( pOutVerts, *p, WorldToCamera );
            else
                RenderSpritePcl( pOutVerts, *p, WorldToCamera );

		    numQuads++;
            p++;
	    }


    }

	virtual void GetRawStreamData(void ** Dest, INT FirstVertex )
	{
		*Dest = NULL;
	}
};

FWeatherQuadStream weatherStream;

void AxWeatherEffect::CacheBlockers()
{
    // will this work in the postload?
    pclBlockers.Empty();
    checkSlow(XLevel);
    for( int i=0; i<XLevel->Actors.Num(); i++ )
    {
        if (XLevel->Actors(i) && XLevel->Actors(i)->IsA(AVolume::StaticClass()))
        {
            if( XLevel->Actors(i)->Tag == Tag ) // should be 'xWeatherEffect'
			{
                pclBlockers.AddItem( (AVolume*)XLevel->Actors(i) );
			}
        }
    }
}

void AxWeatherEffect::UpdateViewer( FLevelSceneNode* SceneNode )
{
    float delta = GetLevel()->TimeSeconds - LastRenderTime;
	LastRenderTime = GetLevel()->TimeSeconds;

    // calc velocity
	eyeDir = FVector(SceneNode->CameraToWorld.M[2][0], SceneNode->CameraToWorld.M[2][1], SceneNode->CameraToWorld.M[2][2]);
    eyeMoveVec = SceneNode->ViewOrigin - eyePos;
    eyePos = SceneNode->ViewOrigin;
    eyeVel = eyeMoveVec.Size();
    if ( eyeVel > 0.001f )
    {
        eyeMoveVec *= 1.0f / eyeVel;
    }
    else
    {
        eyeVel = 0.0f;
        eyeMoveVec = FVector(0,0,0);
    }
    

    // move the spawn origin according to velocity and fudge
    spawnOrigin = SceneNode->ViewOrigin;    
    spawnVecU = FVector( 280, 0, 0 );
    spawnVecV = FVector( 0, 0, 280 );

    // extrapolate next eye position and bias spawn positions in that dir
    if ( eyeVel > 0.0f )
    {
		float eyeSpeed = eyeVel * (1.0f / delta);
		eyeSpeed = Clamp( eyeSpeed, 0.0f, 300.0f );
        spawnOrigin += eyeMoveVec * eyeSpeed;
    }

	if ( PhysicsVolume && PhysicsVolume->Brush )
	{
		if ( !PhysicsVolume->IsA(ADefaultPhysicsVolume::StaticClass()) )
		{
			FCheckResult Hit(0);
			// check for containment
			if ( PhysicsVolume->Brush->PointCheck(Hit,PhysicsVolume,spawnOrigin,FVector(0,0,0),0)!=0 )
			{
				// if not contained, find nearest point on the volume for spawn origin
				FCheckResult Hit(0);
				FVector start = spawnOrigin;
				FVector end = Location;
				if ( PhysicsVolume->Brush->LineCheck(Hit,PhysicsVolume,end,start,FVector(0,0,0),0,0)==0 )
				{
					spawnOrigin = Hit.Location;
				}
			}
		}
	}
}

void AxWeatherEffect::SetZone( UBOOL bTest, UBOOL bForceRefresh )
{
    Super::SetZone(bTest,bForceRefresh);

	if ( GIsEditor && XLevel )
	{
		APhysicsVolume* NewVolume = PhysicsVolume;
		for ( INT i=0; i<XLevel->Actors.Num(); i++ )
		{
			APhysicsVolume *Next = Cast<APhysicsVolume>(XLevel->Actors(i));
			if ( Next && Next->Encompasses(Location)
				&& (Next->Priority > NewVolume->Priority) )
			{
				NewVolume = Next;
			}
		}
		PhysicsVolume = NewVolume;
	}

    CacheBlockers();
}

void AxWeatherEffect::PreCalc()
{
    if ( pcl.Num() != numParticles )
    {
        pcl.Empty();
        pcl.AddZeroed( numParticles );
    }

    eyePos = Location;
    spawnOrigin = Location;
   	numFrames = (int)(numCols * numRows);
	texU = 1.0f / numCols;
	texV = 1.0f / numRows;

    if ( XLevel )
        CacheBlockers();
}

void AxWeatherEffect::PostEditLoad()
{
    SetZone(0,0);
    PreCalc();
    CacheBlockers();
}

void AxWeatherEffect::PostEditChange()
{
    Super::PostEditChange();
    PreCalc();
	SetZone(0,0);
}

void AxWeatherEffect::PostLoad()
{
    Super::PostLoad();
    PreCalc();
}

void AxWeatherEffect::Destroy()
{
    Super::Destroy();
}

void AxWeatherEffect::Spawned()
{
    Super::Spawned();
    PreCalc();
    for ( int i=0; i<pcl.Num(); i++ )
	{
		InitParticle( pcl(i) );
	}
}

#define IR(x)	((DWORD&)x)
#define EPS 0.0001f
inline static bool LineBox( FVector& lineDir, FVector& lineOrigin, float lineInvLength, FBox& box, float& t )
{
	bool Inside = true;
	FVector MinB = box.Min;
	FVector MaxB = box.Max;
	FVector MaxT(-1.0f,-1.0f,-1.0f);

	// Find candidate planes.
    DWORD i;
	for(i=0;i<3;i++)
	{
		if(lineOrigin[i] < MinB[i])
		{
			Inside = false;
			// Calculate T distances to candidate planes
			if(IR(lineDir[i]))
				MaxT[i] = (MinB[i] - lineOrigin[i]) / lineDir[i];
		}
		else if(lineOrigin[i] > MaxB[i])
		{
			Inside = false;
			// Calculate T distances to candidate planes
			if(IR(lineDir[i]))
				MaxT[i] = (MaxB[i] - lineOrigin[i]) / lineDir[i];
		}
	}

	// Ray line.origin inside bounding box
	if(Inside)
	{
		t = 0.0f;
		return true;
	}

	// Get largest of the maxT's for final choice of intersection
	DWORD WhichPlane = 0;
	if(MaxT[1] > MaxT[WhichPlane])
		WhichPlane = 1;
	if(MaxT[2] > MaxT[WhichPlane])
		WhichPlane = 2;

	// Check final candidate actually inside box
	if(IR(MaxT[WhichPlane])&0x80000000)
		return false;

	t = MaxT[WhichPlane] * lineInvLength;
	if ( t > 1.0f )
		return false;

	float f;
	for(i=0;i<3;i++)
	{
		f = lineOrigin[i] + MaxT[WhichPlane] * lineDir[i];
		if( f < MinB[i] - EPS || f > MaxB[i] + EPS)
			return false;
	}
	return true;	// ray hits box
}

inline void AxWeatherEffect::InitParticle( FWeatherPcl& t )
{
    t.pos = spawnOrigin + Position.GetRand();
    t.Vel = spawnVel;
    t.Vel *= Speed.GetRand();
    t.Life = 1.0f;
    float lifeSecs = Life.GetRand();
	t.InvLifeSpan = 1.0f / lifeSecs;
	t.Size = Size.GetRand();
	t.frame = qRand() % (int)numFrames;
    t.HitTime = 0.0f;
    //if ( WeatherType == WT_Rain )
    //    t.Vel *= t.Size * 0.2f;

#if 1
	for ( int i=0; i<pclBlockers.Num(); i++ )
	{
		// this does not support rotated volumes for extra speed
		FCheckResult Hit(0);
		FVector start = t.pos - pclBlockers(i)->Location + pclBlockers(i)->PrePivot;
		FVector end = start + t.Vel * lifeSecs;
		if ( pclBlockers(i)->Brush->LineCheck(Hit,NULL,end,start,FVector(0,0,0),0,0)==0 )
		{
			t.HitTime = 1.0f - Hit.Time;
            break; // this is questionable, should check for closer impacts, but I don't want to slow this down anymore
		}
	}
#else
    // this isn't much better and far more constraining.
    FVector start = t.pos;
    FVector end = start + t.Vel * lifeSecs;
    FVector	dir = end - start;
    float invLen = dir.Size();
    invLen = 1.0f / invLen;
    dir *= invLen;    
	for ( int i=0; i<pclBlockers.Num(); i++ )
	{
        FVector localStart = start - pclBlockers(i)->Location + pclBlockers(i)->PrePivot;
        float time;
        if( LineBox(dir, localStart, invLen, pclBlockers(i)->Brush->BoundingBox, time ))
        {
            t.HitTime = 1.0f - time;
            break;
        }
	}
#endif
}

UBOOL AxWeatherEffect::Tick( FLOAT deltaTime, ELevelTick TickType )
{
    guard(AxWeatherEffect::Tick);
    if ( bHidden )
		return Super::Tick( deltaTime, TickType );

    INT SetupStartTime = appCycles();

    Box.IsValid = 1;
	Box.Min = Location;
    Box.Max = Location;
	numActive = 0;
    FWeatherPcl* p = &pcl(0);
    float invEyeDistSqr = 1.0f / Square(maxPclEyeDist);
    int numPcl = pcl.Num();
	for ( int i=0; i<numPcl; i++ )
	{
        p->Life -= deltaTime * p->InvLifeSpan;
        
        // recycle expired
        if ( p->Life <= 0.0f )
		{
            p->pos = spawnOrigin + Position.GetRand();
            p->Vel = spawnVel;
            p->Vel *= Speed.GetRand();
            p->Life = 1.0f;
            float lifeSecs = Life.GetRand();
	        p->InvLifeSpan = 1.0f / lifeSecs;
	        p->Size = Size.GetRand();
	        p->frame = qRand() % (int)numFrames;
            p->HitTime = 0.0f;
	        for ( int j=0; j<pclBlockers.Num(); j++ )
	        {
		        // this does not support rotated volumes for extra speed
		        FCheckResult Hit(0);
		        FVector start = p->pos - pclBlockers(j)->Location + pclBlockers(j)->PrePivot;
		        FVector end = start + p->Vel * lifeSecs;
		        if ( pclBlockers(j)->Brush->LineCheck(Hit,NULL,end,start,FVector(0,0,0),0,0)==0 )
		        {
			        p->HitTime = 1.0f - Hit.Time;
                    break; // this is questionable, should check for closer impacts, but I don't want to slow this down anymore
		        }
	        }
       		numActive++;
		    Box += p->pos;
		    p++;
            continue;
		}

        // skip updates for particles in collision stasis
        if ( p->Life < p->HitTime )
        {
            numActive++;
		    p++;
            continue;
        }

        // do update
		p->pos += p->Vel * deltaTime;
        p->DistAtten = 1.0f - ((p->pos - eyePos).SizeSquared()*invEyeDistSqr);
        if ( p->DistAtten <= 0.0f )
            p->Life = 0.0f;

		// noise
        if ( WeatherType == WT_Snow && qFRand() <= deviation)
        {
			float burst = 20.0f;
			p->Vel.X += -burst + qFRand() * ( burst + burst );
			p->Vel.Y += -burst + qFRand() * ( burst + burst );
			if ( Abs(p->Vel.X) > 40.0f )
				p->Vel.X *= 0.75f;
			if ( Abs(p->Vel.Y) > 40.0f )
				p->Vel.Y *= 0.75f;
        }

		numActive++;
		Box += p->pos;
		p++;
	}

    GStats.DWORDStats( GEngineStats.STATS_Particle_SpriteSetupCycles ) += appCycles() - SetupStartTime;

    unguard;

    return Super::Tick( deltaTime, TickType );
    
}

void AxWeatherEffect::Render( FLevelSceneNode* SceneNode, FRenderInterface* RI )
{
    if ( SceneNode->Viewport->IsOrtho() )
        return;

    INT	RenderStartTime = appCycles();

    UpdateViewer( SceneNode );
    LastRenderTime = GetLevel()->TimeSeconds;

    if( !GIsEditor )
        GetLevel()->FarMoveActor( this, SceneNode->ViewOrigin);

    if ( Skins.Num() == 0 || !Skins(0) )
        return;

	tmp_texU = texU;
    tmp_texV = texV;
    tmp_numCols = numCols;
    tmp_numRows = numRows;
    tmp_ScaleGlow = ScaleGlow;
    tmp_Color = FVector(1,1,1);//FVector( SkinsDiffuse[0].R, SkinsDiffuse[0].G, SkinsDiffuse[0].B );

	RI->SetMaterial( Skins(0) );
	RI->EnableLighting(0);

    RI->SetTransform(TT_WorldToCamera,FMatrix::Identity);
    RI->SetTransform(TT_LocalToWorld, FMatrix::Identity);

    numCols = numCols;
    numRows = numRows;
    texU = texU;
    texV = texV;

    // cull behind particles, need to first determine 'renderable' amount for dynamic buffer lock
    int renderable = Min( pcl.Num() * 4, 5400 * 4 );
    FPlane eyePlane( SceneNode->ViewOrigin, eyeDir);
    int i=0;
    FWeatherPcl* p = &pcl(0);
    int pclNum = pcl.Num();
    for ( i=0; i<pclNum; i++ )
	{
        if ( p->Life <= 0.0f || eyePlane.PlaneDot( p->pos ) < 0.0f || (p->Life < p->HitTime) )
        {
            p->frame |= PCL_NOT_VIS_MASK;
            p++;
            continue;
        }
        renderable += 4;
        p++;
    }

    weatherStream.Init(this,renderable,SceneNode->WorldToCamera);
    int	firstVertex = SceneNode->Viewport->RI->SetDynamicStream(VS_FixedFunction,&weatherStream);
	SceneNode->Viewport->RI->DrawQuads( firstVertex, weatherStream.numQuads );

    // Reset the camera transform.
    RI->SetTransform(TT_WorldToCamera, SceneNode->WorldToCamera);

    GStats.DWORDStats( GEngineStats.STATS_Particle_Particles ) += weatherStream.numQuads;
    GStats.DWORDStats( GEngineStats.STATS_Particle_RenderCycles ) += (appCycles() - RenderStartTime);
}

IMPLEMENT_CLASS(AxWeatherEffect);
