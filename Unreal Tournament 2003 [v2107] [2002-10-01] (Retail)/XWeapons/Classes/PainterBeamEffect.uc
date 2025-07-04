class PainterBeamEffect extends xEmitter;

#exec OBJ LOAD FILE=XEffectMat.utx

var PainterBeamSpot Spot;
var Vector StartEffect, EndEffect;
var Vector EffectOffset;
var LinkMuzFlashBeam3rd MuzFlash;
var float Brightness;
var enum EPainterTargetingState
{
    PTS_Aiming,
    PTS_Marked,
    PTS_Aquired,
    PTS_Cancelled
} TargetState;

var() Sound MarkSound;
var() Sound AquiredSound;
var() String MarkForce;  // jdf
var() String AmbientForce;  // jdf

replication
{
    unreliable if (Role == ROLE_Authority)
        StartEffect, EndEffect, TargetState;
}

simulated function PostBeginPlay()
{
    Spot = Spawn(class'PainterBeamSpot', self);
}

simulated function Destroyed()
{
    if (MuzFlash != None)
        MuzFlash.mRegen = false;

    if (Spot != None)
        Spot.Destroy();

    Super.Destroyed();
}

function SetTargetState(EPainterTargetingState NewState)
{
    TargetState = NewState;

    if (TargetState == PTS_Aquired)
    {
        GotoState('Aquired');
    }
    else if (TargetState == PTS_Cancelled)
    {
        GotoState('Cancelled');
    }
}

simulated function Tick(float dt)
{
    local Vector BeamDir;
    local Vector X,Y,Z;
    local xWeaponAttachment Attachment;

    if (Role == ROLE_Authority && (Instigator == None || Instigator.Controller == None))
    {
        Destroy();
        return;
    }

    if (Level.NetMode == NM_DedicatedServer)
    {
        StartEffect = Instigator.Location + Instigator.EyeHeight*Vect(0,0,1);
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
            EndEffect = Painter(Instigator.Weapon).EndEffect;
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
    SetRotation(Rotator(BeamDir));

    mSpawnVecA = EndEffect;

    if (Spot != None)
    {
        Spot.SetLocation(EndEffect - BeamDir*10.0);
    }

    if (TargetState == PTS_Marked)
    {
        if (Brightness == 40.0)
            PlaySound(MarkSound);
        SetBrightness( FMax(FMin(Brightness+dt*100.0, 250.0), 100.0) );
    }
    else
        SetBrightness( 40.0 );

    if (TargetState == PTS_Aquired)
        GotoState('Aquired');
    else if (TargetState == PTS_Cancelled)
        GotoState('Cancelled');
}

state Aquired
{
    simulated function BeginState()
    {
        PlaySound(AquiredSound);
        AmbientSound = None;
        SetTimer(0.4, false);
    }

    simulated function Timer()
    {
        GotoState('Cancelled');
    }

    simulated function Tick(float dt)
    {
        if (Level.NetMode != NM_DedicatedServer)
        {
            SetBrightness( 250.0 );
            mSizeRange[0] = FMin(mSizeRange[0]+dt*40.0, 16.0);
        }
    }

    simulated function SetTargetState(EPainterTargetingState NewState)
    {
    }
}

state Cancelled
{
    simulated function BeginState()
    {
        SetTimer(0.4, false);
    }

    simulated function Timer()
    {
        Destroy();
    }

    simulated function Tick(float dt)
    {
        if (Level.NetMode != NM_DedicatedServer)
        {
            SetBrightness( FMax(Brightness-dt*100.0, 40.0) );
            mSizeRange[0] = FMax(mSizeRange[0]-dt*40.0, 1.0);
        }
    }

    simulated function SetTargetState(EPainterTargetingState NewState)
    {
    }
}

simulated function SetBrightness(float b)
{
    Brightness = b;
    mColorRange[0].R = b;
    mColorRange[0].G = b;
    mColorRange[0].B = b;
    mColorRange[1] = mColorRange[0];
}

defaultproperties
{
    RemoteRole=ROLE_SimulatedProxy
    bNetTemporary=false
    bReplicateInstigator=true

    mParticleType=PT_Beam
    mStartParticles=1
    mAttenuate=false
    mSizeRange(0)=8.0 // width
    mRegenDist=100.0 // section length
    mMaxParticles=3 // planes
    mColorRange(0)=(R=100,B=100,G=100)
    mColorRange(1)=(R=100,B=100,G=100)
    mSpinRange(0)=0 // spin
    mAttenKa=1.0

    Skins(0)=Texture'XEffectMat.painter_beam'
    Style=STY_Additive
    bUnlit=true
    EffectOffset=(X=-5,Y=15,Z=-7)

    TargetState=PTS_Aiming

    AmbientSound=Sound'WeaponSounds.TAGRifle.TagFireA'
    MarkSound=Sound'WeaponSounds.TAGRifle.TagFireB'
    AquiredSound=Sound'WeaponSounds.TAGRifle.TagTargetAquired'

    SoundVolume=112
    SoundRadius=100
}
