class xPawn extends UnrealPawn
    config(User)
    dependsOn(xUtil)
    dependsOn(xPawnSoundGroup)
    dependsOn(xPawnGibGroup);

#exec OBJ LOAD FILE=GameSounds.uax
#exec OBJ LOAD FILE=PlayerSounds.uax
#exec OBJ LOAD FILE=PlayerFootSteps.uax
#exec OBJ LOAD FILE=DanFX.utx
#exec OBJ LOAD FILE=GeneralAmbience.uax
#exec OBJ LOAD FILE=GeneralImpacts.uax
#exec OBJ LOAD FILE=DeRez.utx
#exec OBJ LOAD FILE=WeaponSounds.uax

var int RepeaterDeathCount;

var Combo CurrentCombo;
var bool bBerserk;
var bool bInvis;
var bool bOldInvis;
var bool bGibbed;

var(UDamage) Material UDamageWeaponMaterial;         // Weapon overlay material
var(UDamage) Sound UDamageSound;
var UDamageTimer UDamageTimer;
var float UDamageTime;
var float LastUDamageSoundTime;
var Material InvisMaterial;

var(Shield) float   ShieldChargeMax;                 // max strength
var(Shield) transient float ShieldCharge;            // current charge
var(Shield) float   ShieldChargeRate;                // amount to recharge per sec.

var(Shield) float   ShieldStrengthMax;               // max strength
var(Shield) float   ShieldConvertRate;               // speed at which charge is expended into strength
var(Shield) float   ShieldStrengthDecay;             // max strength

var(Shield) Material    ShieldHitMat;
var(Shield) float       ShieldHitMatTime;

var class<SpeciesType> Species;

var PlayerLight     LeftMarker;
var(Marker) Vector  LeftOffset;
var PlayerLight     RightMarker;
var(Marker) Vector  RightOffset;

var(Sounds) float GruntVolume; 
var(Sounds) float FootstepVolume; 

var transient int   SimHitFxTicker;

const DamageDirFront = 0;
const DamageDirRight = 1;
const DamageDirLeft  = 2;
const DamageDirBehind = 3;
var() float DamageDirReduction;
var() float DamageDirLimit;

var(Gib) class<xPawnGibGroup> GibGroupClass;
var(Gib) int GibCountCalf;
var(Gib) int GibCountForearm;
var(Gib) int GibCountHead;
var(Gib) int GibCountTorso;
var(Gib) int GibCountUpperArm;

var float MinTimeBetweenPainSounds;
var localized string HeadShotMessage;

// Common sounds

var(Sounds) Sound   SoundFootsteps[11]; // Indexed by ESurfaceTypes (sorry about the literal).
var(Sounds) class<xPawnSoundGroup> SoundGroupClass;

var class<Actor>    TeleportFXClass;
var class<Actor> TransEffects[2];

var xWeaponAttachment WeaponAttachment;

var ShadowProjector PlayerShadow;

var int  MultiJumpRemaining;
var int  MaxMultiJump;
var int  MultiJumpBoost; // depends on the tolerance (100)

var name WallDodgeAnims[4];
var name IdleHeavyAnim;
var name IdleRifleAnim;
var name FireHeavyRapidAnim;
var name FireHeavyBurstAnim;
var name FireRifleRapidAnim;
var name FireRifleBurstAnim;
var name FireRootBone;

var enum EFireAnimState
{
    FS_None,
    FS_PlayOnce,
    FS_Looping,
    FS_Ready
} FireState;

var Mesh SkeletonMesh;
var bool bSkeletized;
var bool bDeRes;
var float DeResTime;
var Emitter DeResFX;
var Material DeResMat0, DeResMat1;
var(DeRes) InterpCurve DeResLiftVel; // speed (over time) at which body rises
var(DeRes) InterpCurve DeResLiftSoftness; // vertical 'sprinyness' (over time) of bone lifters
var(DeRes) float  DeResGravScale; // reduce gravity on corpse during de-res
var(DeRes) float  DeResLateralFriction; // sideways friction while lifting

var(Karma) float RagdollLifeSpan; // MAXIMUM time the ragdoll will be around. De-res's early if it comes to rest.
var(Karma) float RagInvInertia; // Use to work out how much 'spin' ragdoll gets on death.
var(Karma) float RagDeathVel; // How fast ragdoll moves upon death
var(Karma) float RagShootStrength; // How much effect shooting ragdolls has. Be careful!
var(Karma) float RagSpinScale; // Increase propensity to spin around Z (up).
var(Karma) float RagDeathUpKick; // Amount of upwards kick ragdolls get when they die

var(Karma) material RagConvulseMaterial;

// Ragdoll impact sounds.
var(Karma) array<sound>		RagImpactSounds;
var(Karma) float			RagImpactSoundInterval;
var(Karma) float			RagImpactVolume;
var transient float			RagLastSoundTime;

// translocate effect
var Vector  TransEffectOrigin;
var int     TransEffectTicker;
var int     SimTransEffectTicker;
var class<Actor>    TransOutEffect[2];

var Controller OldController;
var Material RealSkins[2];
var class<TeamVoicePack> VoiceClass;

var(AI) string PlacedCharacterName;

replication
{
    unreliable if( Role==ROLE_Authority )
		TransEffectOrigin, TransEffectTicker, bInvis;
		
	reliable if( bNetOwner && (Role==ROLE_Authority) )
		MaxMultiJump, MultiJumpBoost;
		
	reliable if( Role==ROLE_Authority )
		ClientSetUDamageTime;
}

simulated function PlayWaiting() {}

function RosterEntry GetPlacedRoster()
{
	PlayerReplicationInfo.CharacterName = PlacedCharacterName;
	return class'xRosterEntry'.static.CreateRosterEntryCharacter(PlacedCharacterName);
}

function UnPossessed()
{
	OldController = Controller;
	Super.UnPossessed();
}

// return true if was controlled by a Player (AI or human)
simulated function bool WasPlayerPawn()
{
	return ( (OldController != None) && OldController.bIsPlayer );
}

function DoTranslocateOut(Vector PrevLocation)
{
    TransEffectTicker++;
    TransEffectOrigin = PrevLocation;
}

// Set up default blending parameters and pose. Ensures the mesh doesn't have only a T-pose whenever it first springs into view.
simulated function AssignInitialPose()
{
	TweenAnim(MovementAnims[0],0.0);
	AnimBlendParams(1, 1.0, 0.2, 0.2, 'Bip01 Spine1');
        BoneRefresh();	
}

simulated function Destroyed()
{
    if ( LeftMarker != None )
    {
        LeftMarker.Destroy();
        LeftMarker = None;
    }

    if ( RightMarker != None )
    {
        RightMarker.Destroy();
        RightMarker = None;
    }

    if( PlayerShadow != None )
        PlayerShadow.Destroy();

    if( DeResFX != None )
	{
		DeResFX.Emitters[0].SkeletalMeshActor = None;
		DeResFX.Kill();
	}

    Super.Destroyed();
}

simulated function RemoveFlamingEffects()
{
    local int i;

    if( Level.NetMode == NM_DedicatedServer )
        return;

    for( i=0; i<Attached.length; i++ )
    {
        if( Attached[i].IsA('xEmitter') && !Attached[i].IsA('BloodJet'))
        {
            xEmitter(Attached[i]).mRegen = false;
        }
    }
}

simulated event PhysicsVolumeChange( PhysicsVolume NewVolume )
{
    if ( NewVolume.bWaterVolume )
        RemoveFlamingEffects();
    Super.PhysicsVolumeChange(NewVolume);
}

/* return a value (typically 0 to 1) adjusting pawn's perceived strength if under some special influence (like berserk)
*/
function float AdjustedStrength()
{
	if ( bBerserk )
		return 1.0;
	return 0;
}

function PlayTeleportEffect( bool bOut, bool bSound)
{
	if ( (PlayerReplicationInfo == None) || (PlayerReplicationInfo.Team == None) || (PlayerReplicationInfo.Team.TeamIndex == 0) )
		Spawn(TransEffects[0],,,Location + CollisionHeight * vect(0,0,0.75));
	else
		Spawn(TransEffects[1],,,Location + CollisionHeight * vect(0,0,0.75));
    Super.PlayTeleportEffect( bOut, bSound );
}

function PlayMoverHitSound()
{
	PlaySound(SoundGroupClass.static.GetHitSound(), SLOT_Interact); 
}   

function PlayDyingSound()
{
	local bool playedDeathPhrase;

	// Dont play dying sound if a skeleton. Tricky without vocal chords.
	if ( bSkeletized )
		return;

	if ( bGibbed )
	{
        PlaySound(GibGroupClass.static.GibSound(), SLOT_Pain,2.5*TransientSoundVolume,true,500);
		return;
	}
	
    if ( HeadVolume.bWaterVolume )
    {
        PlaySound(GetSound(EST_Drown), SLOT_Pain,2.5*TransientSoundVolume,true,500);
        return;
    }

	if(FRand() < 0.05)
	{
		playedDeathPhrase = VoiceClass.static.PlayDeathPhrase(self);
		if(playedDeathPhrase)
			return;
	}

	PlaySound(SoundGroupClass.static.GetDeathSound(), SLOT_Pain,2.5*TransientSoundVolume, true,500);
}

function Gasp()
{
    if ( Role != ROLE_Authority )
        return;
    if ( BreathTime < 2 )
        PlaySound(GetSound(EST_Gasp), SLOT_Interact);
    else
        PlaySound(GetSound(EST_BreatheAgain), SLOT_Interact);
}

function Controller GetKillerController()
{
	if ( Controller != None )
		return Controller;
	if ( OldController != None )
		return OldController;
	return None;
}

function TeamInfo GetTeam()
{
	if ( PlayerReplicationInfo != None )
		return PlayerReplicationInfo.Team;
	if ( (OldController != None) && (OldController.PlayerReplicationInfo != None) )
		return OldController.PlayerReplicationInfo.Team;
	return None;
}

function Died(Controller Killer, class<DamageType> damageType, vector HitLocation)
{
    if (CurrentCombo != None)
    {
		CurrentCombo.Destroy();
		if ( Controller != None )
			Controller.Adrenaline = 0;
	}
    if ( UDamageTimer != None )
    {
		UDamageTimer.Destroy();
		DisableUDamage();
	}

    Super.Died( Killer, damageType, HitLocation );
}

simulated function TickFX(float DeltaTime)
{
    local int i;
    local float reduction;
	local xPlayerReplicationInfo xPRI;

    reduction = FClamp(deltatime*DamageDirReduction, 2.f, 255.f);
    
    for(i=0; i<DamageDirMax; i++)
    {
		if ( DamageDirIntensity[i] != 0 ) // to avoid unnecessary bNetDirty
			DamageDirIntensity[i] = Clamp(DamageDirIntensity[i] - int(reduction), 0, 255);
    }

    if ( SimHitFxTicker != HitFxTicker )
    {
        ProcessHitFX();
    }

    // do translocate-out effect
    if( TransEffectTicker != SimTransEffectTicker && Level.NetMode != NM_DedicatedServer )
    {
        SimTransEffectTicker = TransEffectTicker;
        if ( (PlayerReplicationInfo == None) || (PlayerReplicationInfo.Team == None) || (PlayerReplicationInfo.Team.TeamIndex == 0) )
			Spawn(TransOutEffect[0],self,,TransEffectOrigin,rot(16384,0,0));
		else
			Spawn(TransOutEffect[1],self,,TransEffectOrigin,rot(16384,0,0));
    }

	if(bInvis && !bOldInvis) // Going invisible
	{
		// Save the 'real' non-invis skin
		RealSkins[0] = Skins[0];
		RealSkins[1] = Skins[1];

		Skins[0] = InvisMaterial;
		Skins[1] = InvisMaterial;

		// Remove/disallow projectors on invisible people
		Projectors.Remove(0, Projectors.Length);
		bAcceptsProjectors = false;

		// Invisible - no shadow
		if(PlayerShadow != None)
			PlayerShadow.bShadowActive = false;

		// No giveaway flames either
		RemoveFlamingEffects();
	}
	else if(!bInvis && bOldInvis) // Going visible
	{
		Skins[0] = RealSkins[0];
		Skins[1] = RealSkins[1];

		bAcceptsProjectors = Default.bAcceptsProjectors;

		if(PlayerShadow != None)
			PlayerShadow.bShadowActive = true;
	}

	bOldInvis = bInvis;


	xPRI = xPlayerReplicationInfo(PlayerReplicationInfo);

    if( (PlayerReplicationInfo == None) || (PlayerReplicationInfo.Team == None) || (Level.NetMode == NM_DedicatedServer) 
		|| class'DeathMatch'.Default.bNoCoronas || bPlayedDeath || (xPRI != None && xPRI.bForceNoPlayerLights) )
        return;

	if ( LeftMarker == None && !bInvis )
	{
		LeftMarker = Spawn(class'PlayerLight',self,,Location);
		if( !AttachToBone(LeftMarker,'lshoulder') )
		{
			log( "Couldn't attach LeftMarker to lshoulder", 'Error' );
			LeftMarker.Destroy();
			return;
		}

		RightMarker = Spawn(class'PlayerLight',self,,Location);
		if( !AttachToBone(RightMarker,'rshoulder') )
		{
			log( "Couldn't attach RightMarker to rshoulder", 'Error' );
			RightMarker.Destroy();
			return;
		}
		LeftMarker.SetRelativeLocation(LeftOffset);
		RightMarker.SetRelativeLocation(RightOffset);
		if ( PlayerReplicationInfo.Team.TeamIndex == 0 )
		{
			RightMarker.Texture = Texture'RedMarker_t';
			LeftMarker.Texture = Texture'RedMarker_t';
		}
		else
		{
			RightMarker.Texture = Texture'BlueMarker_t';
			LeftMarker.Texture = Texture'BlueMarker_t';
		}
	}
	else if( LeftMarker != None && bInvis )
	{
		LeftMarker.Destroy();
		RightMarker.Destroy();
	}
}

simulated function AttachEffect( class<xEmitter> EmitterClass, Name BoneName, Vector Location, Rotator Rotation )
{
    local Actor a;
    local int i;

    if( BoneName == 'None' )
        return;

    for( i = 0; i < Attached.Length; i++ )
    {
        if( Attached[i] == None )
            continue;

        if( Attached[i].AttachmentBone != BoneName )
            continue;

        if( ClassIsChildOf( EmitterClass, Attached[i].Class ) )
            return;
    }

    a = Spawn( EmitterClass,,, Location, Rotation );

    if( !AttachToBone( a, BoneName ) )
    {
        log( "Couldn't attach "$EmitterClass$" to "$BoneName, 'Error' );
        a.Destroy();
        return;
    }

    for( i = 0; i < Attached.length; i++ )
    {
        if( Attached[i] == a )
            break;
    }

    a.SetRelativeRotation( Rotation );
}

simulated event SetHeadScale(float NewScale)
{
	HeadScale = NewScale;
	SetBoneScale(4,HeadScale,'head');
}

simulated function SpawnGiblet( class<Gib> GibClass, Vector Location, Rotator Rotation, float GibPerterbation )
{
    local Gib Giblet;
    local Vector Direction, Dummy;

    if( (GibClass == None) || class'GameInfo'.static.UseLowGore() )
        return;
	
	Instigator = self;
    Giblet = Spawn( GibClass,,, Location, Rotation );

    if( Giblet == None )
        return;

    GibPerterbation *= 32768.0;
    Rotation.Pitch += ( FRand() * 2.0 * GibPerterbation ) - GibPerterbation;
    Rotation.Yaw += ( FRand() * 2.0 * GibPerterbation ) - GibPerterbation;
    Rotation.Roll += ( FRand() * 2.0 * GibPerterbation ) - GibPerterbation;

    GetAxes( Rotation, Dummy, Dummy, Direction );

    Giblet.Velocity = Velocity + Normal(Direction) * 512.0;
}

simulated function ProcessHitFX()
{
    local Coords boneCoords;
    local class<xEmitter> HitEffects[4];
    local int i;
    local float GibPerterbation;

    if( Level.NetMode == NM_DedicatedServer )
        return;

    for ( SimHitFxTicker = SimHitFxTicker; SimHitFxTicker != HitFxTicker; SimHitFxTicker = (SimHitFxTicker + 1) % ArrayCount(HitFX) )
    {
        if( HitFX[SimHitFxTicker].damtype == None )
            continue;
        
        boneCoords = GetBoneCoords( HitFX[SimHitFxTicker].bone );
        
        if ( !Level.bDropDetail )
        {
			AttachEffect( GibGroupClass.static.GetBloodEmitClass(), HitFX[SimHitFxTicker].bone, boneCoords.Origin, HitFX[SimHitFxTicker].rotDir );
	        
			HitFX[SimHitFxTicker].damtype.static.GetHitEffects( HitEffects, Health );

			if( !PhysicsVolume.bWaterVolume ) // don't attach effects under water
			{
				for( i = 0; i < ArrayCount(HitEffects); i++ )
				{
					if( HitEffects[i] == None )
						continue;

					AttachEffect( HitEffects[i], HitFX[SimHitFxTicker].bone, boneCoords.Origin, HitFX[SimHitFxTicker].rotDir );
				}
			}
		}
        if ( class'GameInfo'.static.UseLowGore() )
			HitFX[SimHitFxTicker].bSever = false;
			
        if( HitFX[SimHitFxTicker].bSever )
        {
            GibPerterbation = HitFX[SimHitFxTicker].damtype.default.GibPerterbation;

            switch( HitFX[SimHitFxTicker].bone )
            {
                case 'lthigh':
                case 'rthigh':
                    SpawnGiblet( GetGibClass(EGT_Calf), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    SpawnGiblet( GetGibClass(EGT_Calf), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    GibCountCalf -= 2;
                    break;
                
                case 'rfarm':
                case 'lfarm':
                    SpawnGiblet( GetGibClass(EGT_UpperArm), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    SpawnGiblet( GetGibClass(EGT_Forearm), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    GibCountForearm--;
                    GibCountUpperArm--;
                    break;
                
                case 'head':
                    SpawnGiblet( GetGibClass(EGT_Head), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    GibCountTorso--;
                    break;
                
                case 'spine':
                case 'none':
                    if( LeftMarker != None )
						LeftMarker.Destroy();
                
                    if( RightMarker != None )
						RightMarker.Destroy();
 
                    SpawnGiblet( GetGibClass(EGT_Torso), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    GibCountTorso--;
					bGibbed = true;
                    while( GibCountHead-- > 0 )
                        SpawnGiblet( GetGibClass(EGT_Head), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    while( GibCountForearm-- > 0 )
                        SpawnGiblet( GetGibClass(EGT_UpperArm), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );
                    while( GibCountUpperArm-- > 0 )
                        SpawnGiblet( GetGibClass(EGT_Forearm), boneCoords.Origin, HitFX[SimHitFxTicker].rotDir, GibPerterbation );

                    break;
            }

            HideBone(HitFX[SimHitFxTicker].bone);
        }
    }
}

simulated function HideBone(name boneName)
{
	local int BoneScaleSlot;
	
    if( boneName == 'lthigh' )
		boneScaleSlot = 0;
	else if ( boneName == 'rthigh' )
		boneScaleSlot = 1;
	else if( boneName == 'rfarm' )
		boneScaleSlot = 2;
	else if ( boneName == 'lfarm' )
		boneScaleSlot = 3;
	else if ( boneName == 'head' )
		boneScaleSlot = 4;
	else if ( boneName == 'spine' )
		boneScaleSlot = 5;
		
    SetBoneScale(BoneScaleSlot, 0.0, BoneName);
}	
	
function CalcHitLoc( Vector hitLoc, Vector hitRay, out Name boneName, out float dist )
{
    boneName = GetClosestBone( hitLoc, hitRay, dist );
}

function DoDamageFX( Name boneName, int Damage, class<DamageType> DamageType, Rotator r )
{
	local float DismemberProbability;
    local bool bExtraGib;
    
    if ( FRand() > 0.3f || Damage > 30 || Health <= 0 )
    {
        HitFX[HitFxTicker].damtype = DamageType;

        if( Health <= 0 )
        {
            switch( boneName )
            {
                case 'lfoot':
                    boneName = 'lthigh';
                    break;

                case 'rfoot':
                    boneName = 'rthigh';
                    break;

                case 'rhand':
                    boneName = 'rfarm';
                    break;

                case 'lhand':
                    boneName = 'lfarm';
                    break;

                case 'rshoulder':
                case 'lshoulder':
                    boneName = 'spine';
                    break;
            }

			if( DamageType.default.bAlwaysSevers || (Damage == 1000) )
			{
                HitFX[HitFxTicker].bSever = true;
                if ( boneName == 'None' )
                {
					boneName = 'spine';
					bExtraGib = true;
				}
			}
            else if( (Damage*DamageType.Default.GibModifier > 50+120*FRand()) && (Damage + Health > 0) ) // total gib prob
			{
				HitFX[HitFxTicker].bSever = true;
				boneName = 'spine';
				bExtraGib = true;
			}
            else
            {
	            DismemberProbability = Abs( (Health - Damage*DamageType.Default.GibModifier) / 130.0f );
				switch( boneName )
                {
                    case 'lthigh':
                    case 'rthigh':
                    case 'rfarm':
                    case 'lfarm':
                    case 'head':
                        if( FRand() < DismemberProbability )
                            HitFX[HitFxTicker].bSever = true;
                        break;

                    case 'None':
 						boneName = 'spine';
                     case 'spine':
                        if( FRand() < DismemberProbability * 0.3 )
                        {
                            HitFX[HitFxTicker].bSever = true;
                            if ( FRand() < 0.65 )
								bExtraGib = true;
						}
                        break;
                }
            }
        }
        
        if ( class'GameInfo'.static.UseLowGore() )
        {
			HitFX[HitFxTicker].bSever = false;
			bExtraGib = false;
		}

        HitFX[HitFxTicker].bone = boneName;
        HitFX[HitFxTicker].rotDir = r;
        HitFxTicker++;
        if( HitFxTicker > ArrayCount(HitFX)-1 )
            HitFxTicker = 0;
        if ( bExtraGib )
        {
			if ( FRand() < 0.3 )
			{
				DoDamageFX('lthigh',1000,DamageType,r);
				DoDamageFX('rthigh',1000,DamageType,r);
			}
			else if ( FRand() < 0.5 )
				DoDamageFX('lthigh',1000,DamageType,r);
			else
				DoDamageFX('rthigh',1000,DamageType,r);
		}				
    }
}

simulated function StartDeRes()
{
	local KarmaParamsSkel skelParams;

    if( Level.NetMode == NM_DedicatedServer )
        return;

	AmbientGlow=254;
	MaxLights=0;

	DeResFX = Spawn(class'DeResPart', self, , Location);
	if ( DeResFX != None )
	{
		DeResFX.Emitters[0].SkeletalMeshActor = self;
		DeResFX.SetBase(self);
	}

	Skins[0]=DeResMat0;
	Skins[1]=DeResMat1;

    if( Physics == PHYS_KarmaRagdoll )
    {
		// Attach bone lifter to raise body
        KAddBoneLifter('bip01 Spine', DeResLiftVel, DeResLateralFriction, DeResLiftSoftness);
        KAddBoneLifter('bip01 Spine2', DeResLiftVel, DeResLateralFriction, DeResLiftSoftness);
		
		// Turn off gravity while de-res-ing
		KSetActorGravScale(DeResGravScale);
		
        // Turn off collision with the world for the ragdoll.
        KSetBlockKarma(false);
        
        // Turn off convulsions during de-res
        skelParams = KarmaParamsSkel(KParams);
		skelParams.bKDoConvulsions = false;        
    }

    AmbientSound = Sound'GeneralAmbience.Texture19';
    SoundRadius = 200.0;

	// Turn off collision when we de-res (avoids rockets etc. hitting corpse!)
	SetCollision(false, false, false);

	// Remove/disallow projectors
	Projectors.Remove(0, Projectors.Length);
	bAcceptsProjectors = false;

	// Remove shadow
	if(PlayerShadow != None)
		PlayerShadow.bShadowActive = false;

	// Remove flames
	RemoveFlamingEffects();

	// Turn off any overlays
	SetOverlayMaterial(None, 0.0f, true);

    bDeRes = true;
}

simulated function SetOverlayMaterial( Material mat, float time, bool bOverride )
{
	if ( Level.bDropDetail || Level.DetailMode == DM_Low )
		time *= 0.75;
	Super.SetOverlayMaterial(mat,time,bOverride);
}

simulated function TickDeRes(float DeltaTime)
{   
	if(LifeSpan < 3.0)
	{
		AmbientGlow = BYTE(254.0 * (LifeSpan / 3.0)); // Scale down over time.
		//ScaleGlow = 1.0 * (LifeSpan / 3.0); // Scale down over time.
		//Log("SG:"$ScaleGlow$" AG:"$AmbientGlow);
	}
}

simulated function Tick(float DeltaTime)
{
	if ( Level.NetMode == NM_DedicatedServer )
		return;
	if ( Controller != None )
		OldController = Controller;
		
    TickFX(DeltaTime);

    if ( bDeRes )
    {
        TickDeRes(DeltaTime);
    }

    // assume dead if bTearOff - for remote clients unfff unfff
    if ( bTearOff )
    {
        if ( !bPlayedDeath )
            PlayDying(HitDamageType, TakeHitLocation);
        return;
    }
}

simulated function PostBeginPlay()
{
    Super.PostBeginPlay();
    AssignInitialPose();      
    
    if(bActorShadows && bPlayerShadows && (Level.NetMode != NM_DedicatedServer))
    {
        PlayerShadow = Spawn(class'ShadowProjector',Self,'',Location);
        PlayerShadow.ShadowActor = self;
        PlayerShadow.bBlobShadow = bBlobShadow;
        PlayerShadow.LightDirection = Normal(vect(1,1,3));
        PlayerShadow.LightDistance = 320;
        PlayerShadow.MaxTraceDistance = 350;
        PlayerShadow.InitShadow();
    }
}

function int ShieldAbsorb( int damage )
{
    if (ShieldStrength == 0)
    {
        return damage;
    }
    if (ShieldStrength > damage)
    {
        ShieldStrength -= damage;
		SetOverlayMaterial( ShieldHitMat, ShieldHitMatTime, false );
	    PlaySound(sound'WeaponSounds.ArmorHit', SLOT_Pain,2*TransientSoundVolume,,400); 
        return 0;
    }
    else
    {
        damage -= ShieldStrength;
		SetOverlayMaterial( ShieldHitMat, ShieldHitMatTime, false );
        ShieldStrength = 0;
        return damage;
    }
}

function CalcDamageDir(vector hitRay, int damage)
{
    local vector x, y, z;
    local float dotx, doty;

    if (hitRay == vect(0,0,0))
    {
        UpdateDamageDirIntensity(DamageDirFront, damage);
        return;
    }

    GetAxes(GetViewRotation(), x, y, z);
    
    dotx = hitRay dot x;
    doty = hitRay dot y;

    if (doty < -DamageDirLimit)
    {
//        log("right");
        UpdateDamageDirIntensity(DamageDirRight, damage);
    }
    else if (doty > DamageDirLimit)
    {
  //      log("left");
        UpdateDamageDirIntensity(DamageDirLeft, damage);
    }
    else if (dotx < 0.f)
    {
    //    log("front");
        UpdateDamageDirIntensity(DamageDirFront, damage);
    }
    else
    {
        UpdateDamageDirIntensity(DamageDirBehind, damage);
      //  log("behind");
    }
}

function UpdateDamageDirIntensity(int i, int damage)
{
    DamageDirIntensity[i] = Clamp(DamageDirIntensity[i] + (120), 0, 255);
}

function PlayHit(float Damage, Pawn InstigatedBy, vector HitLocation, class<DamageType> damageType, vector Momentum)
{
    local Vector HitNormal;
    local Vector HitRay;
    local Name HitBone;
    local float HitBoneDist;
    local PlayerController PC;
    local XPawn XInstigatedBy;
	local bool bShowEffects;
	
	Super.PlayHit(Damage,InstigatedBy,HitLocation,DamageType,Momentum);
    if ( Damage <= 0 )
		return;

    PC = PlayerController(Controller);
	bShowEffects = ( (Level.NetMode != NM_Standalone) || (Level.TimeSeconds - LastRenderTime < 3) 
					|| ((InstigatedBy != None) && (PlayerController(InstigatedBy.Controller) != None)) 
					|| (PC != None) );
	if ( !bShowEffects )
		return;
    XInstigatedBy = xPawn(InstigatedBy);

    HitRay = vect(0,0,0);
    if( InstigatedBy != None )
        HitRay = Normal(HitLocation-(InstigatedBy.Location+(vect(0,0,1)*InstigatedBy.EyeHeight)));

	CalcDamageDir(HitRay, Damage);

    if( DamageType.default.bLocationalHit )
        CalcHitLoc( HitLocation, HitRay, HitBone, HitBoneDist );
    else
    {
        HitLocation = Location;
        HitBone = 'None';
        HitBoneDist = 0.0f;
    }

    if( DamageType.default.bAlwaysSevers && DamageType.default.bSpecial )
        HitBone = 'head';

	if( InstigatedBy != None )
		HitNormal = Normal( Normal(InstigatedBy.Location-HitLocation) + VRand() * 0.2 + vect(0,0,2.8) );
	else
		HitNormal = Normal( Vect(0,0,1) + VRand() * 0.2 + vect(0,0,2.8) );
	
	if ( DamageType.Default.bCausesBlood )
	{
		if ( class'GameInfo'.static.UseLowGore() )
			Spawn( GibGroupClass.default.LowGoreBloodHitClass,,, HitLocation, Rotator(HitNormal) );
		else	
			Spawn( GibGroupClass.default.BloodHitClass,,, HitLocation, Rotator(HitNormal) );
	}
	
	// hack for flak cannon gibbing
	if ( (DamageType.name == 'DamTypeFlakChunk') && (Health < 0) && (VSize(InstigatedBy.Location - Location) < 350) )
		DoDamageFX( HitBone, 8*Damage, DamageType, Rotator(HitNormal) );
	else
		DoDamageFX( HitBone, Damage, DamageType, Rotator(HitNormal) );

	if (DamageType.default.DamageOverlayMaterial != None && Damage > 0 ) // additional check in case shield absorbed
				SetOverlayMaterial( DamageType.default.DamageOverlayMaterial, DamageType.default.DamageOverlayTime, false );
}

function bool CheckReflect( Vector HitLocation, out Vector RefNormal, int Damage )
{
    if (Weapon != None)
        return Weapon.CheckReflect( HitLocation, RefNormal, Damage );
    else
        return false;
}

function name GetWeaponBoneFor(Inventory I)
{
     return 'righthand';
}

event Landed(vector HitNormal)
{
    Super.Landed(HitNormal);
    MultiJumpRemaining = MaxMultiJump;
    
    if (Health > 0)
        PlaySound(GetSound(EST_Land), SLOT_Interact, FMin(1,-0.3 * Velocity.Z/JumpZ));
}

// ----- animation ----- //

simulated function name GetAnimSequence()
{
    local name anim;
    local float frame, rate;
    
    GetAnimParams(0, anim, frame, rate);
    return anim;
}

simulated function PlayDoubleJump()
{
    local name Anim;

    Anim = DoubleJumpAnims[Get4WayDirection()];
    if ( PlayAnim(Anim, 1.0, 0.1) )
        bWaitForAnim = true;
    AnimAction = Anim;
}

simulated event SetAnimAction(name NewAction)
{
    AnimAction = NewAction;
    if (!bWaitForAnim)
    {
		if ( AnimAction == 'Weapon_Switch' )
        {
            AnimBlendParams(1, 1.0, 0.0, 0.2, FireRootBone);
            PlayAnim(NewAction,, 0.0, 1);
        }
        else if ( (Physics == PHYS_None) 
			|| ((Level.Game != None) && Level.Game.IsInState('MatchOver')) )
        {
            PlayAnim(AnimAction,,0.1);
			AnimBlendToAlpha(1,0.0,0.05);
        }
        else if ( (Physics == PHYS_Falling) || ((Physics == PHYS_Walking) && (Velocity.Z != 0)) ) 
		{
			if ( CheckTauntValid(AnimAction) )
			{
				if (FireState == FS_None || FireState == FS_Ready)
				{
					AnimBlendParams(1, 1.0, 0.0, 0.2, FireRootBone);
					PlayAnim(NewAction,, 0.1, 1);
					FireState = FS_Ready;
				}
			}			
			else if ( PlayAnim(AnimAction) )
			{
				if ( Physics != PHYS_None )
					bWaitForAnim = true;
			}
		}
        else if (bIsIdle && !bIsCrouched && (Bot(Controller) == None) ) // standing taunt
        {
            PlayAnim(AnimAction,,0.1);
			AnimBlendToAlpha(1,0.0,0.05);
        }
        else // running taunt
        {
            if (FireState == FS_None || FireState == FS_Ready)
            {
                AnimBlendParams(1, 1.0, 0.0, 0.2, FireRootBone);
                PlayAnim(NewAction,, 0.1, 1);
                FireState = FS_Ready;
            }
        }
    }
}

simulated function StartFiring(bool bHeavy, bool bRapid)
{
    local name FireAnim;

    if ( HasUDamage() && (Level.TimeSeconds - LastUDamageSoundTime > 0.25) )
    {
        LastUDamageSoundTime = Level.TimeSeconds;
        PlaySound(UDamageSound, SLOT_None, 1.5*TransientSoundVolume,,700);
    }

    if (Physics == PHYS_Swimming)
        return;

    if (bHeavy)
    {
        if (bRapid)
            FireAnim = FireHeavyRapidAnim;
        else
            FireAnim = FireHeavyBurstAnim;
    }
    else
    {
        if (bRapid)
            FireAnim = FireRifleRapidAnim;
        else
            FireAnim = FireRifleBurstAnim;
    }

    AnimBlendParams(1, 1.0, 0.0, 0.2, FireRootBone);

    if (bRapid)
    {
        if (FireState != FS_Looping)
        {
            LoopAnim(FireAnim,, 0.0, 1);
            FireState = FS_Looping;
        }
    }
    else
    {
        PlayAnim(FireAnim,, 0.0, 1);
        FireState = FS_PlayOnce;
    }

    IdleTime = Level.TimeSeconds;
}

simulated function StopFiring()
{
    if (FireState == FS_Looping)
    {
        FireState = FS_PlayOnce;
    }
    IdleTime = Level.TimeSeconds;
}

simulated function AnimEnd(int Channel)
{
    if (Channel == 1)
    {
        if (FireState == FS_Ready)
        {
            AnimBlendToAlpha(1, 0.0, 0.12);
            FireState = FS_None;
        }
        else if (FireState == FS_PlayOnce)
        {
            PlayAnim(IdleWeaponAnim,, 0.2, 1);
            FireState = FS_Ready;
            IdleTime = Level.TimeSeconds;
        }
        else
            AnimBlendToAlpha(1, 0.0, 0.12);
    }
    else if ( bKeepTaunting && (Channel == 0) )
		PlayVictoryAnimation();
}

function PlayWeaponSwitch(Weapon NewWeapon)
{
    SetAnimAction('Weapon_Switch');
}

function PlayVictoryAnimation()
{
	local int tauntNum;

	// First 4 taunts are 'order' anims. Don't pick them.
	tauntNum = Rand(TauntAnims.Length - 3);
	SetAnimAction(TauntAnims[3 + tauntNum]);
}

simulated function SetWeaponAttachment(xWeaponAttachment NewAtt)
{
    WeaponAttachment = NewAtt;
    if (WeaponAttachment.bHeavy)
        IdleWeaponAnim = IdleHeavyAnim;
    else
        IdleWeaponAnim = IdleRifleAnim;
}

// Event called whenever ragdoll convulses
event KSkelConvulse()
{
	if(RagConvulseMaterial != None)
		SetOverlayMaterial(RagConvulseMaterial, 0.4, true);
}

simulated final function RandSpin(float spinRate)
{
    DesiredRotation = RotRand(true);
    RotationRate.Yaw = spinRate * 2 *FRand() - spinRate;
    RotationRate.Pitch = spinRate * 2 *FRand() - spinRate;
    RotationRate.Roll = spinRate * 2 *FRand() - spinRate;   

    bFixedRotationDir = true;
    bRotateToDesired = false;
}

simulated function PlayDying(class<DamageType> DamageType, vector HitLoc)
{
	local vector shotDir, hitLocRel, deathAngVel, shotStrength;
	local float maxDim, frame, rate;
	local string RagSkelName;
	local KarmaParamsSkel skelParams;
    local name seq;
	local bool PlayersRagdoll;
	local PlayerController pc;
	local LavaDeath LD;
	
	AmbientSound = None;
    bCanTeleport = false; // sjs - fix karma going crazy when corpses land on teleporters
    bReplicateMovement = false;
    bTearOff = true;
    bPlayedDeath = true;

    if (CurrentCombo != None)
        CurrentCombo.Destroy();
		
	HitDamageType = DamageType; // these are replicated to other clients
    TakeHitLocation = HitLoc;

    if ( (DamageType != None) && DamageType.default.bSkeletize && (SkeletonMesh != None) )
    {
        if (!bSkeletized)
        {
            GetAnimParams( 0, seq, frame, rate );
            LinkMesh(SkeletonMesh, true);
            Skins.Length = 0;
            PlayAnim(seq, 0, 0);
            SetAnimFrame(frame);
            if (Physics == PHYS_Walking)
                Velocity = Vect(0,0,0);
            TearOffMomentum *= 0.25;
            bSkeletized = true;

			if(DamageType == class'FellLava')
			{
				LD = spawn(class'LavaDeath');
				if ( LD != None )
				{
					LD.SetLocation(Location);
					LD.SetRotation(Rotation);
					LD.SetBase(self);
				}
				// This should destroy itself once its finished.

				PlaySound( sound'WeaponSounds.BExplosion5', SLOT_None, 1.5*TransientSoundVolume );
			}
        }
    }

    // stop shooting
    AnimBlendParams(1, 0.0);
    FireState = FS_None;
	LifeSpan = RagdollLifeSpan;

    GotoState('Dying');

	if ( Level.NetMode != NM_DedicatedServer )
	{
		// Is this the local player's ragdoll?
		if(OldController != None)
			pc = PlayerController(OldController);
		if( pc != None && pc.ViewTarget == self )
			PlayersRagdoll = true;

		// In low physics detail, if we were not just controlling this pawn, 
		// and it has not been rendered in 3 seconds, just destroy it.
		if(Level.PhysicsDetailLevel == PDL_Low && !PlayersRagdoll && (Level.TimeSeconds - LastRenderTime > 3) )
		{
			Destroy();
			return;
		}

		// Try and obtain a rag-doll setup
		if(Species != None)
			RagSkelName = Species.static.GetRagSkelName(GetMeshName());
		else
			Log("xPawn.PlayDying: No Species");

		// If we managed to find a name, try and make a rag-doll slot availbale.
		if( RagSkelName != "" )
		{
			KMakeRagdollAvailable();
		}
		
		if( KIsRagdollAvailable() && RagSkelName != "" )
		{  
			skelParams = KarmaParamsSkel(KParams);
			skelParams.KSkeleton = RagSkelName;
			KParams = skelParams;
			
			//Log("RAGDOLL");
			
			// Stop animation playing.
			StopAnimating(true);
			
			// DEBUG
			//if(VSize(TearOffMomentum) < 0.01)
			//	Log("TearOffMomentum magnitude of Zero");
			// END DEBUG
			
			if(DamageType != None && DamageType.default.bKUseOwnDeathVel)
			{
				RagDeathVel = DamageType.default.KDeathVel;
				RagDeathUpKick = DamageType.default.KDeathUpKick;
			}

			// Set the dude moving in direction he was shot in general
			shotDir = Normal(TearOffMomentum);
			shotStrength = RagDeathVel * shotDir;
    		
		    // Calculate angular velocity to impart, based on shot location.
		    hitLocRel = TakeHitLocation - Location;
    		
		    // We scale the hit location out sideways a bit, to get more spin around Z.
		    hitLocRel.X *= RagSpinScale;
		    hitLocRel.Y *= RagSpinScale;
		    deathAngVel = RagInvInertia * (hitLocRel Cross shotStrength);
    		
    		// Set initial angular and linear velocity for ragdoll.
			// Scale horizontal velocity for characters - they run really fast!
			skelParams.KStartLinVel.X = 0.6 * Velocity.X;
			skelParams.KStartLinVel.Y = 0.6 * Velocity.Y;
			skelParams.KStartLinVel.Z = 1.0 * Velocity.Z;
    		skelParams.KStartLinVel += shotStrength;

			// If not moving downwards - give extra upward kick
			if(Velocity.Z > -10)
				skelParams.KStartLinVel.Z += RagDeathUpKick;
    		
    		skelParams.KStartAngVel = deathAngVel;

    		// Set up deferred shot-bone impulse
			maxDim = Max(CollisionRadius, CollisionHeight);
    		
    		skelParams.KShotStart = TakeHitLocation - (1 * shotDir);
    		skelParams.KShotEnd = TakeHitLocation + (2*maxDim*shotDir);
    		skelParams.KShotStrength = RagShootStrength;
    		
    		// If this damage type causes convulsions, turn them on here.
    		if(DamageType != None && DamageType.default.bCauseConvulsions)
    		{
    			RagConvulseMaterial=DamageType.default.DamageOverlayMaterial;
    			skelParams.bKDoConvulsions = true;
		    }
    		    		
    		// Turn on Karma collision for ragdoll.
			KSetBlockKarma(true);
			
			// Set physics mode to ragdoll. 
			// This doesn't actaully start it straight away, it's deferred to the first tick.
			SetPhysics(PHYS_KarmaRagdoll);

			// If viewing this ragdoll, set the flag to indicate that it is 'important'
			if( PlayersRagdoll )
				skelParams.bKImportantRagdoll = true;

			return;
		}
		// jag
	}
		
	// non-ragdoll death fallback
	Velocity += TearOffMomentum;
    BaseEyeHeight = Default.BaseEyeHeight;
    SetTwistLook(0, 0);
    SetInvisibility(0.0);
    PlayDirectionalDeath(HitLoc);
    SetPhysics(PHYS_Falling);
}

simulated function SpawnGibs(Rotator HitRotation, float ChunkPerterbation)
{
	bGibbed = true;
	PlayDyingSound();
    if( GibCountTorso+GibCountHead+GibCountForearm+GibCountUpperArm > 3 )
    {
        if ( class'GameInfo'.static.UseLowGore() )
             Spawn( GibGroupClass.default.LowGoreBloodGibClass,,,Location );
        else
            Spawn( GibGroupClass.default.BloodGibClass,,,Location );
    }
    if ( class'GameInfo'.static.UseLowGore() )
		return;

    SpawnGiblet( GetGibClass(EGT_Torso), Location, HitRotation, ChunkPerterbation );
    GibCountTorso--;

    while( GibCountTorso-- > 0 )
        SpawnGiblet( GetGibClass(EGT_Torso), Location, HitRotation, ChunkPerterbation );
    while( GibCountHead-- > 0 )
        SpawnGiblet( GetGibClass(EGT_Head), Location, HitRotation, ChunkPerterbation );
    while( GibCountForearm-- > 0 )
        SpawnGiblet( GetGibClass(EGT_UpperArm), Location, HitRotation, ChunkPerterbation );
    while( GibCountUpperArm-- > 0 )
        SpawnGiblet( GetGibClass(EGT_Forearm), Location, HitRotation, ChunkPerterbation );
}

function ClientDying(class<DamageType> DamageType, vector HitLocation)
{
}

function PlayTakeHit(vector HitLocation, int Damage, class<DamageType> DamageType)
{
    PlayDirectionalHit(HitLocation);

    if( Level.TimeSeconds - LastPainSound < MinTimeBetweenPainSounds )
        return;

    LastPainSound = Level.TimeSeconds;

    if( HeadVolume.bWaterVolume )
    {
        if( DamageType.IsA('Drowned') )
            PlaySound( GetSound(EST_Drown), SLOT_Pain,1.5*TransientSoundVolume ); 
        else
            PlaySound( GetSound(EST_HitUnderwater), SLOT_Pain,1.5*TransientSoundVolume ); 
        return;
    }

    PlaySound(SoundGroupClass.static.GetHitSound(), SLOT_Pain,2*TransientSoundVolume,,400); 
}

// jag
// Called when in Ragdoll when we hit something over a certain threshold velocity
// Used to play impact sounds.
event KImpact(actor other, vector pos, vector impactVel, vector impactNorm)
{
	local int numSounds, soundNum;
	numSounds = RagImpactSounds.Length;

	//log("ouch! iv:"$VSize(impactVel));

	if(numSounds > 0 && Level.TimeSeconds > RagLastSoundTime + RagImpactSoundInterval)
	{
		soundNum = Rand(numSounds);
		//Log("Play Sound:"$soundNum);
		PlaySound(RagImpactSounds[soundNum], SLOT_Pain, RagImpactVolume);
		RagLastSoundTime = Level.TimeSeconds;
	}
}
//jag

simulated function PlayDirectionalDeath(Vector HitLoc)
{
    local Vector X,Y,Z, Dir;

    GetAxes(Rotation, X,Y,Z);
    HitLoc.Z = Location.Z;
    
    // random
    if ( VSize(Velocity) < 10.0 && VSize(Location - HitLoc) < 1.0 )
    {
        Dir = VRand();
    }
    // velocity based
    else if ( VSize(Velocity) > 0.0 )
    {
        Dir = Normal(Velocity*Vect(1,1,0));
    }
    // hit location based
    else
    {
        Dir = -Normal(Location - HitLoc);
    }

    if ( Dir Dot X > 0.7 || Dir == vect(0,0,0))
        PlayAnim('DeathB',, 0.2);
    else if ( Dir Dot X < -0.7 )
         PlayAnim('DeathF',, 0.2);
    else if ( Dir Dot Y > 0 )
        PlayAnim('DeathL',, 0.2);
    else
        PlayAnim('DeathR',, 0.2);
}

simulated function PlayDirectionalHit(Vector HitLoc)
{
    local Vector X,Y,Z, Dir;

    GetAxes(Rotation, X,Y,Z);
    HitLoc.Z = Location.Z;
    
    // random
    if ( VSize(Location - HitLoc) < 1.0 )
    {
        Dir = VRand();
    }
    // hit location based
    else
    {
        Dir = -Normal(Location - HitLoc);
    }

    if ( Dir Dot X > 0.7 || Dir == vect(0,0,0))
    {
        PlayAnim('HitF',, 0.1);
    }
    else if ( Dir Dot X < -0.7 )
    {
        PlayAnim('HitB',, 0.1);
    }
    else if ( Dir Dot Y > 0 )
    {
        PlayAnim('HitR',, 0.1);
    }
    else
    {
        PlayAnim('HitL',, 0.1);
    }
}

simulated function FootStepping(int Side)
{
    local int SurfaceType, i;
	local actor A;
	local material FloorMat;
	local vector HL,HN,Start,End;
	
    SurfaceType = 0;
		
    for ( i=0; i<Touching.Length; i++ )
		if ( (PhysicsVolume(Touching[i]) != None) && PhysicsVolume(Touching[i]).bWaterVolume )
		{
			if ( FRand() < 0.5 )
				PlaySound(sound'PlayerSounds.FootStepWater2', SLOT_Interact, FootstepVolume );
			else
				PlaySound(sound'PlayerSounds.FootStepWater1', SLOT_Interact, FootstepVolume );
			return;
		}

	if ( bIsCrouched || bIsWalking )
		return;
		
	if ( (Base!=None) && (!Base.IsA('LevelInfo')) && (Base.SurfaceType!=0) )
	{
		SurfaceType = Base.SurfaceType;
	}
	else
	{
		Start = Location - Vect(0,0,1)*CollisionHeight;
		End = Start - Vect(0,0,16);
		A = Trace(hl,hn,End,Start,false,,FloorMat);
		if (FloorMat !=None)
			SurfaceType = FloorMat.SurfaceType;
	}		
	PlaySound(SoundFootsteps[SurfaceType], SLOT_Interact, FootstepVolume,,400 );
}

simulated function PlayFootStepLeft()
{
    PlayFootStep(-1);
}

simulated function PlayFootStepRight()
{
    PlayFootStep(1);
}

// ----- shield control ----- //
function float GetShieldStrengthMax()
{
    return ShieldStrengthMax;
}

function float GetShieldStrength()
{
    // could return max if it's active right now, which make it unable to be recharged while it's on...
    return ShieldStrength;
}

function bool AddShieldStrength(int ShieldAmount)
{
	if (ShieldStrength < ShieldStrengthMax)
	{
		ShieldStrength = Min(ShieldStrengthMax, ShieldStrength + ShieldAmount);
        return true;
	}
    return false;
}

function bool InCurrentCombo()
{
	return (CurrentCombo != None);
}

// used by bots doing combos
function DoComboName( string ComboClassName )
{
    local class<Combo> ComboClass;

    ComboClass = class<Combo>( DynamicLoadObject( ComboClassName, class'Class' ) );
    if ( ComboClass != None )
			DoCombo( ComboClass );
	else 
		log("WARNING - Couldn't create combo "$ComboClassName);
}

function DoCombo( class<Combo> ComboClass )
{
    if ( ComboClass != None )
    {
        if (CurrentCombo == None)
        {
	        CurrentCombo = Spawn( ComboClass, self );

			// Record stats for using the combo

			if (Level.Game.GameStats!=None)
				Level.Game.GameStats.SpecialEvent(PlayerReplicationInfo,""$CurrentCombo.Class); 
        }
    }
}

simulated function bool HasUDamage()
{
    return (UDamageTime > Level.TimeSeconds);
}

function ClientSetUDamageTime(float NewUDam)
{
	UDamageTime = Level.TimeSeconds + NewUDam;
}

function EnableUDamage(float amount)
{
    UDamageTime = FMax(UDamageTime, Level.TimeSeconds+amount);
    ClientSetUDamageTime(UDamageTime - Level.TimeSeconds);
    if ( UDamageTimer == None )
		UDamageTimer = Spawn(class'UDamageTimer',self);
	UDamageTimer.SetTimer(UDamageTime - Level.TimeSeconds,false);
	LightType = LT_Steady;
	bDynamicLight = true;
    SetWeaponOverlay(UDamageWeaponMaterial, UDamageTime - Level.TimeSeconds, false);
}

function DisableUDamage()
{
	LightType = LT_None;
	bDynamicLight = false;
	UDamageTime = Level.TimeSeconds - 1;
	ClientSetUDamageTime(-1);
    SetWeaponOverlay(UDamageWeaponMaterial, UDamageTime - Level.TimeSeconds, false);
}

function SetWeaponOverlay(Material mat, float time, bool override)
{
    if (Weapon != None)
    {
        Weapon.SetOverlayMaterial(mat, time, override);
        if (WeaponAttachment(Weapon.ThirdPersonActor) != None)
            WeaponAttachment(Weapon.ThirdPersonActor).SetOverlayMaterial(mat, time, override);
    }
}

function ChangedWeapon()
{
    if (Weapon != None && Role < ROLE_Authority)
    {
        if (bBerserk)
            Weapon.StartBerserk();
    }

    Super.ChangedWeapon();
}

function ServerChangedWeapon(Weapon OldWeapon, Weapon NewWeapon)
{
    if (HasUDamage() || bInvis)
        SetWeaponOverlay(None, 0.f, true);

    Super.ServerChangedWeapon(OldWeapon, NewWeapon);

    if (bInvis)
        SetWeaponOverlay(InvisMaterial, Weapon.OverlayTimer, true);
    else if (HasUDamage())
        SetWeaponOverlay(UDamageWeaponMaterial, UDamageTime - Level.TimeSeconds, false);

    if (bBerserk)
        Weapon.StartBerserk();
}

function SetInvisibility(float time)
{
    bInvis = (time > 0.0);
    if (Role == ROLE_Authority)
    {
        if (bInvis)
		{
			Visibility = 1;
            SetWeaponOverlay(InvisMaterial, time, true);
        }
        else
        {
			Visibility = Default.Visibility;
            if (HasUDamage())
                SetWeaponOverlay(UDamageWeaponMaterial, UDamageTime - Level.TimeSeconds, true);
            else
                SetWeaponOverlay(None, 0.0, true);
        }
    }
}

/* BotDodge()
returns appropriate vector for dodge in direction Dir (which should be normalized)
*/
function vector BotDodge(Vector Dir)
{
	local vector Vel;
	
	Vel = DodgeSpeedFactor*GroundSpeed*Dir;
	Vel.Z = DodgeSpeedZ;
	return Vel;
}

function bool Dodge(eDoubleClickDir DoubleClickMove)
{
    local vector X,Y,Z, TraceStart, TraceEnd;
    local float VelocityZ;
    local name Anim;

    if ( bIsCrouched || bWantsToCrouch || (Physics != PHYS_Walking && Physics != PHYS_Falling) )
        return false;

    GetAxes(Rotation,X,Y,Z);

    if ( Physics == PHYS_Falling )
    {
        if (DoubleClickMove == DCLICK_Forward)
            TraceEnd = -X;
        else if (DoubleClickMove == DCLICK_Back)
            TraceEnd = X;
        else if (DoubleClickMove == DCLICK_Left)
            TraceEnd = Y;
        else if (DoubleClickMove == DCLICK_Right)
            TraceEnd = -Y;
        TraceStart = Location - CollisionHeight*Vect(0,0,1) + TraceEnd*CollisionRadius;
        TraceEnd = TraceStart + TraceEnd*32.0;
        if (FastTrace(TraceEnd, TraceStart))
        {
            return false;
        }
        else
        {
            if (DoubleClickMove == DCLICK_Forward)
                Anim = WallDodgeAnims[0];
            else if (DoubleClickMove == DCLICK_Back)
                Anim = WallDodgeAnims[1];
            else if (DoubleClickMove == DCLICK_Left)
                Anim = WallDodgeAnims[2];
            else if (DoubleClickMove == DCLICK_Right)
                Anim = WallDodgeAnims[3];

            if ( PlayAnim(Anim, 1.0, 0.1) )
                bWaitForAnim = true;
                AnimAction = Anim;

            if (Velocity.Z < -DodgeSpeedZ*0.5) Velocity.Z += DodgeSpeedZ*0.5;
        }
    }

    VelocityZ = Velocity.Z;

    if (DoubleClickMove == DCLICK_Forward)
        Velocity = DodgeSpeedFactor*GroundSpeed*X + (Velocity Dot Y)*Y;
    else if (DoubleClickMove == DCLICK_Back)
        Velocity = -DodgeSpeedFactor*GroundSpeed*X + (Velocity Dot Y)*Y; 
    else if (DoubleClickMove == DCLICK_Left)
        Velocity = -DodgeSpeedFactor*GroundSpeed*Y + (Velocity Dot X)*X; 
    else if (DoubleClickMove == DCLICK_Right)
        Velocity = DodgeSpeedFactor*GroundSpeed*Y + (Velocity Dot X)*X; 
 
    Velocity.Z = VelocityZ + DodgeSpeedZ;
    CurrentDir = DoubleClickMove;
    SetPhysics(PHYS_Falling);
    PlayOwnedSound(GetSound(EST_Dodge), SLOT_Pain, GruntVolume,,80);
    return true;
}

function DoDoubleJump( bool bUpdating )
{
    PlayDoubleJump();

    if ( !bIsCrouched && !bWantsToCrouch )
    {
		if ( !IsLocallyControlled() )
			MultiJumpRemaining -= 1;
        Velocity.Z = JumpZ + MultiJumpBoost;
        SetPhysics(PHYS_Falling);
        if ( !bUpdating )
			PlayOwnedSound(GetSound(EST_DoubleJump), SLOT_Pain, GruntVolume,,80);
    }
}

function bool CanDoubleJump()
{
	return ( (MultiJumpRemaining > 0) && (Physics == PHYS_Falling) );
}

function bool DoJump( bool bUpdating )
{
    // This extra jump allows a jumping or dodging pawn to jump again mid-air
    // (via thrusters). The pawn must be within +/- 100 velocity units of the 
    // apex of the jump to do this special move. 
    if ( !bUpdating && CanDoubleJump()&& (Abs(Velocity.Z) < 100) && IsLocallyControlled() )
    {
		if ( PlayerController(Controller) != None )
			PlayerController(Controller).bDoubleJump = true;
        DoDoubleJump(bUpdating);
        MultiJumpRemaining -= 1;
        return true;
    }

    if ( Super.DoJump(bUpdating) )
    {
		if ( !bUpdating )
			PlayOwnedSound(GetSound(EST_Jump), SLOT_Pain, GruntVolume,,80);
        return true;
    }
    return false;
}

simulated event PostNetReceive()
{
	if ( PlayerReplicationInfo != None )
    {
		Setup(class'xUtil'.static.FindPlayerRecord(PlayerReplicationInfo.CharacterName));
        bNetNotify = false;
    }
}

simulated function ClientRestart()
{
	Super.ClientRestart();
	if ( Controller != None )
		OldController = Controller;
}

simulated function Setup(xUtil.PlayerRecord rec, optional bool bLoadNow)
{
	if ( class'DeathMatch'.default.bForceDefaultCharacter )
		rec = class'xUtil'.static.FindPlayerRecord("Gorge");	
 
    Species = rec.Species;
    if ( Species == None )
    {
		warn("Could not load species "$rec.Species$" for "$rec.DefaultName);
		return;
	}
	if ( Species.static.Setup(self,rec) )
		ResetPhysicsBasedAnim();
}

simulated function ResetPhysicsBasedAnim()
{
    bIsIdle = false;
    bWaitForAnim = false;
}

function Sound GetSound(xPawnSoundGroup.ESoundType soundType)
{
    return SoundGroupClass.static.GetSound(soundType);
}

function class<Gib> GetGibClass(xPawnGibGroup.EGibType gibType)
{
    return GibGroupClass.static.GetGibClass(gibType);
}

simulated function DoDerezEffect()
{
    Spawn(TeleportFXClass);
}

State Dying
{
    simulated function AnimEnd( int Channel )
    {
        ReduceCylinder();
    }

	event FellOutOfWorld(eKillZType KillType)
	{
		local LavaDeath LD;

		// If we fall past a lava killz while dead- burn off skin.
		if( KillType == KILLZ_Lava )
		{
			if (!bSkeletized)
			{
				//LinkMesh(SkeletonMesh, true);
				//Skins.Length = 0;
				bSkeletized = true;

				LD = spawn(class'LavaDeath', , , Location + vect(0, 0, 10), Rotation );
				if ( LD != None )
					LD.SetBase(self);
				// This should destroy itself once its finished.

				PlaySound( sound'WeaponSounds.BExplosion5', SLOT_None, 1.5*TransientSoundVolume );
			}

			return;
		}

		Super.FellOutOfWorld(KillType);
	}

    function LandThump()
    {
        // animation notify - play sound if actually landed, and animation also shows it
        if ( Physics == PHYS_None)
        {
            bThumped = true;
            PlaySound(GetSound(EST_CorpseLanded));
        }
    }

    simulated function TakeDamage( int Damage, Pawn InstigatedBy, Vector Hitlocation, Vector Momentum, class<DamageType> damageType)
    {
        local Vector SelfToHit, SelfToInstigator, CrossPlaneNormal;
        local float W;
        local float YawDir;

        local Vector HitNormal, shotDir;
        local Vector PushLinVel, PushAngVel;
        local Name HitBone;
        local float HitBoneDist;
        local int MaxCorpseYawRate;

		if(bPlayedDeath && Physics == PHYS_KarmaRagdoll)
		{
			// Can't shoot corpses during de-res
			if(bDeRes)
				return;
			
			//log("HIT RAGDOLL. M:"$Momentum);
			// Throw the body if its a rocket explosion or shock combo
			if(damageType.Name == 'DamTypeRocket' || damageType.Name == 'DamTypeShockCombo' || damageType.Name == 'DamTypeFlakShell')
			{
				shotDir = Normal(Momentum);
                PushLinVel = (RagDeathVel * shotDir) +  vect(0, 0, 250);
				PushAngVel = Normal(shotDir Cross vect(0, 0, 1)) * -18000;
				KSetSkelVel( PushLinVel, PushAngVel );
			}
			else
			{
                PushLinVel = RagShootStrength*Normal(Momentum);
				KAddImpulse(PushLinVel, HitLocation);
			}
			
			return;
		}

        if ( DamageType.default.bFastInstantHit && GetAnimSequence() == 'Death_Spasm' && RepeaterDeathCount < 6)
        {
            PlayAnim('Death_Spasm',, 0.2);
            RepeaterDeathCount++;
        }
        else if (Damage > 0)
        {
			if ( InstigatedBy != None )
			{
				if ( InstigatedBy.IsA('xPawn') && xPawn(InstigatedBy).bBerserk )
					Damage *= 2;

				// Figure out which direction to spin:

				if( InstigatedBy.Location != Location )
				{
					SelfToInstigator = InstigatedBy.Location - Location;
					SelfToHit = HitLocation - Location;

					CrossPlaneNormal = Normal( SelfToInstigator cross Vect(0,0,1) );
					W = CrossPlaneNormal dot Location;

					if( HitLocation dot CrossPlaneNormal < W )
						YawDir = -1.0;
					else
						YawDir = 1.0;
				}
			}
            if( VSize(Momentum) < 10 )
            {
                Momentum = - Normal(SelfToInstigator) * Damage * 1000.0;
                Momentum.Z = Abs( Momentum.Z );
            }

            SetPhysics(PHYS_Falling);
            Momentum = Momentum / Mass;
            AddVelocity( Momentum ); 
            bBounce = true;

            RotationRate.Pitch = 0;
            RotationRate.Yaw += VSize(Momentum) * YawDir;

            MaxCorpseYawRate = 150000;
            RotationRate.Yaw = Clamp( RotationRate.Yaw, -MaxCorpseYawRate, MaxCorpseYawRate );
            RotationRate.Roll = 0;

            bFixedRotationDir = true;
            bRotateToDesired = false;

            Health -= Damage;
            CalcHitLoc( HitLocation, vect(0,0,0), HitBone, HitBoneDist );

            if( InstigatedBy != None )
                HitNormal = Normal( Normal(InstigatedBy.Location-HitLocation) + VRand() * 0.2 + vect(0,0,2.8) );
            else
                HitNormal = Normal( Vect(0,0,1) + VRand() * 0.2 + vect(0,0,2.8) );

            DoDamageFX( HitBone, Damage, DamageType, Rotator(HitNormal) );
        }
    }

    simulated function BeginState()
	{
		Super.BeginState();
		AmbientSound = None;
        if ( LeftMarker != None )
            LeftMarker.Destroy();

        if ( RightMarker != None )
            RightMarker.Destroy();
 	}

    simulated function Timer()
	{
		local KarmaParamsSkel skelParams;

		if ( !PlayerCanSeeMe() )
        {
			Destroy();
        }
        // If we are running out of life, bute we still haven't come to rest, force the de-res.
        // unless pawn is the viewtarget of a player who used to own it
        else if ( LifeSpan <= DeResTime && bDeRes == false )
        {
			skelParams = KarmaParamsSkel(KParams);

			// check not viewtarget
			if ( (PlayerController(OldController) != None) && (PlayerController(OldController).ViewTarget == self) )
			{
				skelParams.bKImportantRagdoll = true;
				LifeSpan = FMax(LifeSpan,DeResTime + 2.0);
				SetTimer(1.0, false);
				return;
			}
			else
			{
				skelParams.bKImportantRagdoll = false;
			}
            // spawn derez
            StartDeRes();
        }
		else
        {
			SetTimer(1.0, false);
        }
	}
	
	// We shorten the lifetime when the guys comes to rest.
	event KVelDropBelow()
	{
		local float NewLifeSpan;
	
		if(bDeRes == false)
		{
			//Log("Low Vel - Reducing LifeSpan!");
			NewLifeSpan = DeResTime + 3.5;
			if(NewLifeSpan < LifeSpan)
				LifeSpan = NewLifeSpan;
		}
	}
}

defaultproperties
{
    bNetNotify=true

    DamageDirReduction=30.0
    DamageDirLimit=0.5

    // default everything to juggmaleaa
    Mesh=Mesh'Jugg.JuggMaleA'
    Skins(0)=Texture'PlayerSkins.JuggMaleABodyA'
    Skins(1)=Texture'PlayerSkins.JuggMaleAHeadA'
    Species=class'SPECIES_Jugg'
    SoundGroupClass=class'xJuggMaleSoundGroup'
    GibGroupClass=class'xJuggGibGroup'

    bStatic=False
    DrawScale=1.0

    Buoyancy=+00099.000000
    UnderWaterTime=+00020.000000
    bCanStrafe=True
    DrawType=DT_Mesh
    Style=STY_Normal
    LightBrightness=255
    LightHue=204
    LightSaturation=0
    LightRadius=3
    RotationRate=(Pitch=3072,Yaw=20000,Roll=2048)

    BloodEffect=class'BloodJet'
    LowGoreBlood=class'AlienSmallHit'
    AirControl=+0.35
    bStasis=false
    bCanCrouch=true
    bCanClimbLadders=True
    bCanPickupInventory=True
    WalkingPct=+0.4

    BaseEyeHeight=30.0 
    EyeHeight=30.0     
    CollisionRadius=25.0
    CollisionHeight=44.0  
    GroundSpeed=440.0
    AirSpeed=440.0
    WaterSpeed=220.0
    DodgeSpeedFactor=1.5
    DodgeSpeedZ=210.0
    AccelRate=2048.0
    JumpZ=340.0
    CrouchHeight=29.0
    CrouchRadius=25.0

    LODBias=1.8

    ShieldChargeMax=1000.0
    ShieldChargeRate=0.0
    ShieldStrengthMax=150.0
    ShieldConvertRate=200.0
    ShieldStrengthDecay=35.0

	PrePivot=(X=0.0,Y=0.0,Z=-5.0);
    LeftOffset=(X=20.0,Y=25.0,Z=20.0)
    RightOffset=(X=20.0,Y=25.0,Z=-20.0)
     ControllerClass=class'XGame.xBot'

    SoundFootsteps(0)=Sound'PlayerSounds.Final.FootstepDefault'
    SoundFootsteps(1)=Sound'PlayerSounds.Final.FootstepRock'
    SoundFootsteps(2)=Sound'PlayerSounds.Final.FootstepDirt'
    SoundFootsteps(3)=Sound'PlayerSounds.Final.FootStepMetal'
    SoundFootsteps(4)=Sound'PlayerSounds.Final.FootstepWood'
    SoundFootsteps(5)=Sound'PlayerSounds.Final.FootstepPlant'
    SoundFootsteps(6)=Sound'PlayerSounds.Final.FootstepFlesh'
    SoundFootsteps(7)=Sound'PlayerSounds.Final.FootstepIce'
    SoundFootsteps(8)=Sound'PlayerSounds.Final.FootstepSnow'
    SoundFootsteps(9)=Sound'PlayerSounds.Final.FootstepWater'
    SoundFootsteps(10)=Sound'PlayerSounds.Final.FootstepGlass'

   RequiredEquipment(0)="XWeapons.AssaultRifle"
   RequiredEquipment(1)="XWeapons.ShieldGun"

    MinTimeBetweenPainSounds=0.75
    ScaleGlow=1.0
    AmbientGlow=20

    TeleportFXClass=class'TransEffect'

    GibCountCalf=4
    GibCountForearm=2
    GibCountHead=2
    GibCountTorso=2
    GibCountUpperArm=2

    bDoTorsoTwist=true

    bActorShadows=true
    bPlayerShadows=true
    ShieldHitMat=Material'XGameShaders.PlayerShaders.PlayerShieldSh'
    ShieldHitMatTime=1.0
    UDamageWeaponMaterial=Material'XGameShaders.PlayerShaders.WeaponUDamage'
    InvisMaterial=FinalBlend'XEffectMat.InvisOverlayFB'

    bPhysicsAnimUpdate=true
    MovementAnims(0)=RunF
    MovementAnims(1)=RunB
    MovementAnims(2)=RunL
    MovementAnims(3)=RunR
    SwimAnims(0)=SwimF
    SwimAnims(1)=SwimB
    SwimAnims(2)=SwimL
    SwimAnims(3)=SwimR
    CrouchAnims(0)=CrouchF
    CrouchAnims(1)=CrouchB
    CrouchAnims(2)=CrouchL
    CrouchAnims(3)=CrouchR
    WalkAnims(0)=WalkF
    WalkAnims(1)=WalkB
    WalkAnims(2)=WalkL
    WalkAnims(3)=WalkR
    AirStillAnim=Jump_Mid
    AirAnims(0)=JumpF_Mid
    AirAnims(1)=JumpB_Mid
    AirAnims(2)=JumpL_Mid
    AirAnims(3)=JumpR_Mid
    TakeoffStillAnim=Jump_Takeoff
    TakeoffAnims(0)=JumpF_Takeoff
    TakeoffAnims(1)=JumpB_Takeoff
    TakeoffAnims(2)=JumpL_Takeoff
    TakeoffAnims(3)=JumpR_Takeoff
    LandAnims(0)=JumpF_Land
    LandAnims(1)=JumpB_Land
    LandAnims(2)=JumpL_Land
    LandAnims(3)=JumpR_Land
    DoubleJumpAnims(0)=DoubleJumpF
    DoubleJumpAnims(1)=DoubleJumpB
    DoubleJumpAnims(2)=DoubleJumpL
    DoubleJumpAnims(3)=DoubleJumpR
    DodgeAnims(0)=DodgeF
    DodgeAnims(1)=DodgeB
    DodgeAnims(2)=DodgeL
    DodgeAnims(3)=DodgeR
    WallDodgeAnims(0)=WallDodgeF
    WallDodgeAnims(1)=WallDodgeB
    WallDodgeAnims(2)=WallDodgeL
    WallDodgeAnims(3)=WallDodgeR

    TurnRightAnim=TurnR
    TurnLeftAnim=TurnL
    CrouchTurnRightAnim=Crouch_TurnR
    CrouchTurnLeftAnim=Crouch_TurnL
    IdleRestAnim=Idle_Rest
    IdleCrouchAnim=Crouch
    IdleSwimAnim=Swim_Tread
    IdleWeaponAnim=Idle_Rifle
    IdleHeavyAnim=Idle_Biggun
    IdleRifleAnim=Idle_Rifle
    FireHeavyRapidAnim=Biggun_Burst
    FireHeavyBurstAnim=Biggun_Aimed
    FireRifleRapidAnim=Rifle_Burst
    FireRifleBurstAnim=Rifle_Aimed

	TauntAnims(0)=gesture_point
	TauntAnimNames(0)="Point"

	TauntAnims(1)=gesture_beckon
	TauntAnimNames(1)="Beckon"

	TauntAnims(2)=gesture_halt
	TauntAnimNames(2)="Halt"

	TauntAnims(3)=gesture_cheer
	TauntAnimNames(3)="Cheer"

	TauntAnims(4)=PThrust
	TauntAnimNames(4)="Pelvic Thrust"

	TauntAnims(5)=AssSmack
	TauntAnimNames(5)="Ass Smack"
	
	TauntAnims(6)=ThroatCut
	TauntAnimNames(6)="Throat Cut"
	
	TauntAnims(7)=Specific_1
	TauntAnimNames(7)="Unique"

    MaxMultiJump=1
    MultiJumpRemaining=1
    MultiJumpBoost=25

    RagdollLifeSpan=13
	RagDeathVel=200
	RagInvInertia=4
	RagShootStrength=8000
	RagSpinScale=2.5
	RagDeathUpKick=150

  	Begin Object Class=KarmaParamsSkel Name=PawnKParams
		KFriction=0.6
		KRestitution=0.3
		KAngularDamping=0.05
		KLinearDamping=0.15
		KBuoyancy=1
		KStartEnabled=True
		KImpactThreshold=500
		KVelDropBelowThreshold=50
		KConvulseSpacing=(Min=0.5,Max=2.2)
		bHighDetailOnly=False
        Name="PawnKParams"
    End Object
    KParams=KarmaParams'PawnKParams'

	RagImpactSounds(0)=sound'GeneralImpacts.Breakbone_01'
	RagImpactSounds(1)=sound'GeneralImpacts.Breakbone_02'
	RagImpactSounds(2)=sound'GeneralImpacts.Breakbone_03'
	RagImpactSounds(3)=sound'GeneralImpacts.Breakbone_04'
	
	RagImpactVolume=2.5
	RagImpactSoundInterval=0.5
	
    VoiceType="xGame.MercMaleVoice"

    RootBone="Bip01"
    HeadBone="Bip01 Head"
    SpineBone1="Bip01 Spine1"
    SpineBone2="Bip01 Spine2"
    FireRootBone="Bip01 Spine"

    UDamageSound=Sound'GameSounds.UDamageFire'
    
    GruntVolume=0.18
    FootstepVolume=0.15
    
    DeResTime=6.0
	DeResMat0=FinalBlend'DeRez.DeRezFinalBody'
	DeResMat1=FinalBlend'DeRez.DeRezFinalHead'
	DeResLiftVel=(Points=((InVal=0,OutVal=0),(InVal=2.5,OutVal=32),(InVal=100,OutVal=32)))
	DeResLiftSoftness=(Points=((InVal=0,OutVal=0.3),(InVal=2.5,OutVal=0.05),(InVal=100,OutVal=0.05)))
    DeResGravScale=0.0
    DeResLateralFriction=0.3
    TransOutEffect(0)=class'TransDeRes'
    TransOutEffect(1)=class'TranseDeResBlue'
	TransEffects(0)=class'TransEffectRed'
	TransEffects(1)=class'TransEffectBlue'
	PlacedCharacterName="Gorge"
	
	MaxLights=8
}
