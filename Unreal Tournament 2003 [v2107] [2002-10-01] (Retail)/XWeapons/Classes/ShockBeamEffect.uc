class ShockBeamEffect extends xEmitter;

var Vector HitNormal;

replication
{
    reliable if (bNetInitial && Role == ROLE_Authority)
        HitNormal;
}

function AimAt(Vector hl, Vector hn)
{
    HitNormal = hn;
    mSpawnVecA = hl;
    if (Level.NetMode != NM_DedicatedServer)
        SpawnEffects();
}

simulated function PostNetBeginPlay()
{
    if (Role < ROLE_Authority)
        SpawnEffects();
}

simulated function SpawnEffects()
{
    local Rotator HitRot;
    local Vector EffectLoc;
    local ShockBeamCoil Coil;
    local xWeaponAttachment Attachment;

    if (Instigator != None)
    {
        if ( Instigator.IsFirstPerson() && Instigator.Weapon != None )
        {
            SetLocation(Instigator.Weapon.GetEffectStart());
            Spawn(class'ShockMuzFlash',,, Location);
        }
        else
        {
            Attachment = xPawn(Instigator).WeaponAttachment;
            if (Attachment != None && (Level.TimeSeconds - Attachment.LastRenderTime) < 1)
                SetLocation(Attachment.GetTipLocation());
            else
                SetLocation(Instigator.Location + Instigator.EyeHeight*Vect(0,0,1) + Normal(mSpawnVecA - Instigator.Location) * 25.0); 
            Spawn(class'ShockMuzFlash3rd',,, Location);
        }
    }

    if ( EffectIsRelevant(EffectLoc,false) )
	{
		if (HitNormal != Vect(0,0,0))
		{
			HitRot = Rotator(HitNormal);
			EffectLoc = mSpawnVecA + HitNormal*2;
			Spawn(class'ShockImpactFlare',,, EffectLoc, HitRot);
			Spawn(class'ShockImpactRing',,, EffectLoc, HitRot);
			Spawn(class'ShockImpactScorch',,, EffectLoc, Rotator(-HitNormal));
			Spawn(class'ShockExplosionCore',,, EffectLoc+HitNormal*8, HitRot);
		}
	}

    if ( !Level.bDropDetail && Level.DetailMode != DM_Low )
    {
	    Coil = Spawn(class'ShockBeamCoil',,, Location, Rotation);
	    if (Coil != None)
	    {
		    Coil.mSpawnVecA = mSpawnVecA;
	    }
    }
}

defaultproperties
{
    RemoteRole=ROLE_SimulatedProxy
    bReplicateInstigator=true
    bReplicateMovement=false
    bNetTemporary=true
    LifeSpan=0.75
	NetPriority=3.0

    mParticleType=PT_Beam
    mStartParticles=1
    mAttenKa=0.1
    mSizeRange(0)=16.0
    mSizeRange(1)=48.0
    mRegenDist=150.0
    mLifeRange(0)=0.75
    mMaxParticles=3

    Texture=Texture'ShockBeamTex'
    Skins(0)=Texture'ShockBeamTex'
    Style=STY_Additive
    bUnlit=true
}
