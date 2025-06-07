
class LinkBeamEffect extends xEmitter;

#exec OBJ LOAD FILE=XEffectMat.utx

var Vector StartEffect, EndEffect;
var byte Links, OldLinks;
var byte LinkColor, OldLinkColor;
var Pawn LinkedPawn;
var bool bLockedOn, bHitSomething;
var Vector EffectOffset;
var Vector PrevLoc;
var Rotator PrevRot;
var float scorchtime;

var LinkSparks Sparks;
var LinkProtSphere ProtSphere;
var LinkMuzFlashBeam3rd MuzFlash;

const MAX_CHILDREN = 4;
var int NumChildren;
var LinkBeamChild Child[MAX_CHILDREN];

var Sound BeamSounds[4];

replication
{
    unreliable if (Role == ROLE_Authority)
        Links, LinkColor, LinkedPawn, bLockedOn, bHitSomething;
    unreliable if ( (Role == ROLE_Authority) && !bNetOwner )
        StartEffect, EndEffect;
}

simulated function Destroyed()
{
    local int c;

    if (Sparks != None)
    {
        Sparks.SetTimer(0, false);
        Sparks.mRegen = false;
        Sparks.LightType = LT_None;
    }

    if (MuzFlash != None)
    {
        MuzFlash.mRegen = false;
    }

    if (ProtSphere != None)
    {
        ProtSphere.Destroy();
    }

    for (c=0; c<MAX_CHILDREN; c++)
    {
        if (Child[c] != None)
            Child[c].Destroy();
    }

    Super.Destroyed();
}

simulated function Tick(float dt)
{
    local float LocDiff, RotDiff, WiggleMe,ls;
    local int c, n;
    local Vector BeamDir,X,Y,Z, HitLocation, HitNormal;
    local xWeaponAttachment Attachment;
    local actor HitActor;

    if (Role == ROLE_Authority && (Instigator == None || Instigator.Controller == None))
    {
        Destroy();
        return;
    }

    if (Level.NetMode == NM_DedicatedServer)
    {
        StartEffect = Instigator.Location + Instigator.EyeHeight*Vect(0,0,1);
        SetLocation(StartEffect);
        return;
    }

    // set beam start location
    if ( Instigator == None )
    {
        SetLocation(StartEffect);
    }
    else
    {
        if ( Instigator.IsFirstPerson() && Instigator.Weapon != None )
        {
            Instigator.Weapon.GetViewAxes(X, Y, Z);
	        SetLocation( (Instigator.Location + Instigator.CalcDrawOffset(Instigator.Weapon) + EffectOffset.X * X + EffectOffset.Y * Y + EffectOffset.Z * Z) );
        }
        else
        {
            Attachment = xPawn(Instigator).WeaponAttachment;
            if (Attachment != None && (Level.TimeSeconds - Attachment.LastRenderTime) < 1)
                SetLocation( Attachment.GetTipLocation() );
            else
                SetLocation( Instigator.Location + Instigator.EyeHeight*Vect(0,0,1) + Normal(EndEffect - Instigator.Location) * 25.0 );
        }
        if (Role == ROLE_Authority) // what clients will use if their instigator is not relevant yet
            StartEffect = Location;
    }

    BeamDir = Normal(EndEffect - Location);

    if (Instigator != None && Instigator.IsHumanControlled())
        SetRotation(xPlayer(Instigator.Controller).GetAim());
    else
        SetRotation(Rotator(BeamDir));


    if (MuzFlash == None)
        MuzFlash = Spawn(class'LinkMuzFlashBeam3rd', self);

    MuzFlash.bHidden = (Instigator == None || Instigator.IsFirstPerson());

    if (Sparks == None && EffectIsRelevant(EndEffect, false))
    {
        Sparks = Spawn(class'LinkSparks', self);
    }

    ls = class'LinkFire'.default.LinkScale[Min(Links,5)];

    if (Links != OldLinks || LinkColor != OldLinkColor)
    {
        // beam size
        mSizeRange[0] = default.mSizeRange[0] * (ls*0.6 + 1);
    
        mWaveShift = default.mWaveShift * (ls*0.6 + 1);

        // create/destroy children
        NumChildren = Min(Links+1, MAX_CHILDREN);
        for (c=0; c<MAX_CHILDREN; c++)
        {
            if (c < NumChildren && !Level.bDropDetail && Level.DetailMode != DM_Low)
            {
                if (Child[c] == None)
                    Child[c] = Spawn(class'LinkBeamChild', self);

                Child[c].mSizeRange[0] = 2.0 + 4.0 * (NumChildren - c);
            }
            else
            {
                if (Child[c] != None)
                    Child[c].Destroy();
            }
        }

        if (LinkColor == 0)
        {
            if (Links > 0)
            {
                Skins[0] = FinalBlend'XEffectMat.LinkBeamYellowFB';
                MuzFlash.Skins[0] = Texture'XEffectMat.link_muz_yellow';
                LightHue = 40;
            }
            else
            {
                Skins[0] = FinalBlend'XEffectMat.LinkBeamGreenFB';
                MuzFlash.Skins[0] = Texture'XEffectMat.link_muz_green';
                LightHue = 100;
            }
        }
        else if (LinkColor == 1)
        {
            Skins[0] = FinalBlend'XEffectMat.LinkBeamRedFB';
            MuzFlash.Skins[0] = Texture'XEffectMat.link_muz_red';
            LightHue = 0;
        }
        else
        {
            Skins[0] = FinalBlend'XEffectMat.LinkBeamBlueFB';
            MuzFlash.Skins[0] = Texture'XEffectMat.link_muz_blue';
            LightHue = 160;
        }

        AmbientSound = BeamSounds[Min(Links,3)];

        MuzFlash.mSizeRange[0] = MuzFlash.default.mSizeRange[0] * (ls*0.5 + 1);
        MuzFlash.mSizeRange[1] = MuzFlash.mSizeRange[0];

        LightBrightness = 180 + 40*ls;
        LightRadius = 6 + 3*ls;

        if (Sparks != None)
        {
            Sparks.SetLinkStatus(Links, (LinkColor > 0), ls);
            Sparks.bHidden = (LinkColor > 0);
            Sparks.LightHue = LightHue;
            Sparks.LightBrightness = LightBrightness;
            Sparks.LightRadius = LightRadius - 3;
        }

        if (LinkColor > 0 && LinkedPawn != None)
        {
            if (ProtSphere == None)
            {
                ProtSphere = Spawn(class'LinkProtSphere');
                if (LinkColor == 2)
                    ProtSphere.Skins[0] = Texture'XEffectMat.link_muz_blue';
            }
        }

        OldLinks = Links;
        OldLinkColor = LinkColor;
    }

    if ( Level.bDropDetail || Level.DetailMode == DM_Low )
    {
		bDynamicLight = false;
        LightType = LT_None;
    }
    else if ( bDynamicLight )
        LightType = LT_Steady;

    if (LinkedPawn != None)
    {
        EndEffect = LinkedPawn.Location + LinkedPawn.EyeHeight*Vect(0,0,0.5) - BeamDir*30.0;
    }

    mSpawnVecA = EndEffect;

    mWaveLockEnd = bLockedOn || (LinkColor > 0);

    // magic wiggle code
    if (bLockedOn || (LinkColor > 0))
    {
        mWaveAmplitude = FMax(0.0, mWaveAmplitude - (mWaveAmplitude+5)*4.0*dt);
    }
    else
    {
        LocDiff = VSize((Location - PrevLoc) * Vect(1,1,5));
        RotDiff = VSize(Vector(Rotation) - Vector(PrevRot));
        WiggleMe = FMax(LocDiff*0.02, RotDiff*4.0);
        mWaveAmplitude = FMax(1.0, mWaveAmplitude - mWaveAmplitude*1.0*dt);
        mWaveAmplitude = FMin(16.0, mWaveAmplitude + WiggleMe);
    }

    PrevLoc = Location;
    PrevRot = Rotation;

    for (c=0; c<NumChildren; c++)
    {
        if (Child[c] != None)
        {
            n = c+1;
            Child[c].SetLocation(Location);
            Child[c].SetRotation(Rotation);
            Child[c].mSpawnVecA = mSpawnVecA;
            //Child[c].mWaveFrequency = mWaveFrequency*0.5; // * 2.0*n;
            Child[c].mWaveShift = mWaveShift*0.6;
            Child[c].mWaveAmplitude = n*4.0 + mWaveAmplitude*((16.0-n*4.0)/16.0);
            Child[c].mWaveLockEnd = (LinkColor > 0); //mWaveLockEnd;
            Child[c].Skins[0] = Skins[0];
        }
    }

    if (Sparks != None)
    {
        Sparks.SetLocation(EndEffect-BeamDir*10.0);
        if (bHitSomething)
            Sparks.SetRotation(Rotation);
        else
            Sparks.SetRotation(Rotator(-BeamDir));
        Sparks.mRegenRange[0] = Sparks.DesiredRegen;
        Sparks.mRegenRange[1] = Sparks.DesiredRegen;
        Sparks.bDynamicLight = true;
    }

    if (LinkColor > 0 && LinkedPawn != None)
    {
        if (ProtSphere != None)
        {
            ProtSphere.SetLocation(EndEffect);
            ProtSphere.SetRotation(Rotation);
            ProtSphere.bHidden = false;
            if (LinkedPawn.IsFirstPerson())
                ProtSphere.mSizeRange[0] = 20.0;
            else
                ProtSphere.mSizeRange[0] = 35.0;
            ProtSphere.mSizeRange[1] = ProtSphere.mSizeRange[0];
        }
    }
    else
    {
        if (ProtSphere != None)
            ProtSphere.bHidden = true;
    }
    if ( bHitSomething && (Level.NetMode != NM_DedicatedServer) && (Level.TimeSeconds - ScorchTime > 0.07) )
    {
		ScorchTime = Level.TimeSeconds;
		HitActor = trace(HitLocation,HitNormal,EndEffect + 100*BeamDir,EndEffect - 100*BeamDir,true);	
		if ( (HitActor != None) && HitActor.bWorldGeometry )
			spawn(class'LinkScorch',,,HitLocation,rotator(-HitNormal));
	}
}


defaultproperties
{
    RemoteRole=ROLE_SimulatedProxy
    bNetTemporary=false
    bReplicateInstigator=true

    mParticleType=PT_Beam
    mStartParticles=1
    mAttenuate=false
    mSizeRange(0)=11.0 // width
    mRegenDist=65.0 // section length
    mMaxParticles=3 // planes
    mColorRange(0)=(R=240,B=240,G=240)
    mColorRange(1)=(R=240,B=240,G=240)
    mSpinRange(0)=45000 // spin
    mAttenKa=0.0
    mWaveFrequency=0.06
    mWaveAmplitude=8.0
    mWaveShift=100000.0
    mBendStrength=3.0
    mWaveLockEnd=true

    Skins(0)=FinalBlend'XEffectMat.LinkBeamGreenFB'
    Style=STY_Additive
    bUnlit=true
    EffectOffset=(X=22,Y=11,Z=1.4)
    SoundRadius=100
    SoundVolume=255

    bDynamicLight=true
    LightType=LT_Steady
    LightRadius=4
    LightHue=100 // 0=red 160=blue 40=yellow
    LightSaturation=100
    LightBrightness=180
    LightEffect=LE_QuadraticNonIncidence

    AmbientSound=Sound'WeaponSounds.LinkGun.BLinkGunBeam1'
    BeamSounds(0)=Sound'WeaponSounds.LinkGun.BLinkGunBeam1'
    BeamSounds(1)=Sound'WeaponSounds.LinkGun.BLinkGunBeam2'
    BeamSounds(2)=Sound'WeaponSounds.LinkGun.BLinkGunBeam3'
    BeamSounds(3)=Sound'WeaponSounds.LinkGun.BLinkGunBeam4'
}
