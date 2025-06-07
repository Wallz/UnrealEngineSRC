//=============================================================================
// Translocator Launcher
//=============================================================================
class TransLauncher extends Weapon
    config(user);

#EXEC OBJ LOAD FILE=InterfaceContent.utx

var TransBeacon TransBeacon;
var bool bCameraMode;
var() float     MaxCamDist;
var() float AmmoCharge;
var int RepAmmo,OldAmmo;
var() float AmmoChargeMax;
var() float AmmoChargeRate;
var weapon OldWeapon;

replication
{
    reliable if ( bNetOwner && (ROLE==ROLE_Authority) )
        TransBeacon,RepAmmo;
}

simulated function bool HasAmmo()
{
    return true;
}

// AI Interface...
function GiveTo(Pawn Other, optional Pickup Pickup)
{
	Super.GiveTo(Other, Pickup);
	
	if ( Bot(Other.Controller) != None )
		Bot(Other.Controller).bHasTranslocator = true;
}

function bool ShouldTranslocatorHop(Bot B)
{
	local float dist;
	local Actor N;
		
	if ( B.bTranslocatorHop && (B.Focus == B.TranslocationTarget) && B.InLatentExecution(B.LATENT_MOVETOWARD) && B.Squad.AllowTranslocationBy(B) )
	{
		if ( (TransBeacon != None) && TransBeacon.IsMonitoring(B.Focus) )
			return false;
		dist = VSize(B.TranslocationTarget.Location - B.Pawn.Location);
		if ( dist < 300 )
		{
			// see if next path is possible
			N = B.AlternateTranslocDest();
			if ( (N == None) || ((vector(B.Rotation) Dot Normal(N.Location - B.Pawn.Location)) < 0.5)  )
			{
				if ( dist < 200 )
				{
					B.TranslocationTarget = None;
					B.RealTranslocationTarget = None;
					return false;
				}
			}
			else
			{
				B.TranslocationTarget = N;
				B.RealTranslocationTarget = B.TranslocationTarget;
				B.Focus = N;
				return true;
			}
		}
		if ( (vector(B.Rotation) Dot Normal(B.TranslocationTarget.Location - B.Pawn.Location)) < 0.5 ) 
		{
			SetTimer(0.1,false);
			return false;
		}
		return true;
	}
	return false;
}

simulated function Timer()
{
	local Bot B;

	if ( Instigator != None )
	{
		B = Bot(Instigator.Controller);
		if ( (B != None) && (B.TranslocationTarget != None) && (B.bPreparingMove || ShouldTranslocatorHop(B)) )
			FireHack(0);
	}   
	Super.Timer();
}

function FireHack(byte Mode)
{
	local Actor TTarget;
	local vector TTargetLoc;
	
	if ( Mode == 0 )
	{
		if ( TransBeacon != None )
		{
			// this shouldn't happen
			TransBeacon.bNoAI = true;
			TransBeacon.Destroy();
			TransBeacon = None;
		}
		TTarget = Bot(Instigator.Controller).TranslocationTarget;
		if ( TTarget == None )
			return;
		// hack in translocator firing here
        FireMode[0].PlayFiring();
        FireMode[0].FlashMuzzleFlash();
        FireMode[0].StartMuzzleSmoke();
        IncrementFlashCount(0);
		ProjectileFire(FireMode[0]).SpawnProjectile(Instigator.Location,Rot(0,0,0));
		// find correct initial velocity
		TTargetLoc = TTarget.Location;
		if ( JumpSpot(TTarget) != None )
		{
			TTargetLoc.Z += JumpSpot(TTarget).TranslocZOffset;
			if ( (Instigator.Anchor != None) && Instigator.ReachedDestination(Instigator.Anchor) )
			{
				// start from same point as in test
				Transbeacon.SetLocation(TransBeacon.Location + Instigator.Anchor.Location + (Instigator.CollisionHeight - Instigator.Anchor.CollisionHeight) * vect(0,0,1)- Instigator.Location);
			}
		}
		else if ( TTarget.Velocity != vect(0,0,0) )
		{
			TTargetLoc += 0.3 * TTarget.Velocity;
			TTargetLoc.Z = 0.5 * (TTargetLoc.Z + TTarget.Location.Z);
		}
		else if ( (Instigator.Physics == PHYS_Falling)
					&& (Instigator.Location.Z < TTarget.Location.Z)
					&& (Instigator.PhysicsVolume.Gravity.Z > -800) )
			TTargetLoc.Z += 128;
		
		TransBeacon.Velocity = Bot(Instigator.Controller).AdjustToss(TransBeacon.Speed, TransBeacon.Location, TTargetLoc,false);
		TransBeacon.SetTranslocationTarget(Bot(Instigator.Controller).RealTranslocationTarget);
	}
}

// super desireable for bot waiting to translocate
function float GetAIRating()
{
	local Bot B;

	B = Bot(Instigator.Controller);
	if ( B == None )
		return AIRating;
	if ( B.bPreparingMove && (B.TranslocationTarget != None) )
		return 10;
	if ( B.bTranslocatorHop && ((B.Focus == B.MoveTarget) || ((B.TranslocationTarget != None) && (B.Focus == B.TranslocationTarget))) && B.Squad.AllowTranslocationBy(B) ) 
	{
		if ( Instigator.Weapon == self )
			SetTimer(0.2,false);
		return 4;
	}
	if ( Instigator.Weapon == self )
	return AIRating;
}

function bool BotFire(bool bFinished, optional name FiringMode)
{
	return false;
}

// End AI interface

function ConsumeAmmo(int mode, float load)
{
}

function ReduceAmmo()
{
	enable('Tick');
    AmmoCharge -= 1;
    RepAmmo -= 1;
    if ( Bot(Instigator.Controller) != None )
    	Bot(Instigator.Controller).TranslocFreq = 3 + FMax(Bot(Instigator.Controller).TranslocFreq,Level.TimeSeconds); 
}

simulated function GetAmmoCount(out float MaxAmmoPrimary, out float CurAmmoPrimary)
{
	MaxAmmoPrimary = AmmoChargeMax;
	CurAmmoPrimary = AmmoCharge;
}

function GiveAmmo(int m, WeaponPickup WP, bool bJustSpawned)
{
    Super.GiveAmmo(m, WP,bJustSpawned);
    AmmoCharge = Default.AmmoCharge;
    RepAmmo = int(AmmoCharge);
}

simulated function BringUp(optional Weapon PrevWeapon)
{
    Super.BringUp();
    if ( (PrevWeapon != None) && PrevWeapon.HasAmmo() && !PrevWeapon.IsA('BallLauncher') )
		OldWeapon = PrevWeapon;
	else
		OldWeapon = None;
}

simulated function bool StartFire(int Mode)
{
	if ( (Mode == 1) || (Instigator.Controller.bAltFire == 0) || (PlayerController(Instigator.Controller) == None) )
		return Super.StartFire(Mode);
	if ( (OldWeapon != None) && OldWeapon.HasAmmo() )
	    Instigator.PendingWeapon = OldWeapon;
	ClientStopFire(0);
	Instigator.Controller.StopFiring();
	PutDown();
	OldWeapon = None;
    return false;
}

simulated function Tick(float dt)
{
    if (Role == ROLE_Authority)
    {
		if ( AmmoCharge >= AmmoChargeMax )
		{
			if ( RepAmmo != int(AmmoCharge) ) // condition to avoid unnecessary bNetDirty of ammo
				RepAmmo = int(AmmoCharge);	
			disable('Tick');
			return;
		}
		AmmoCharge += dt*AmmoChargeRate;
		AmmoCharge = FMin(AmmoCharge, AmmoChargeMax);
        if ( RepAmmo != int(AmmoCharge) ) // condition to avoid unnecessary bNetDirty of ammo
			RepAmmo = int(AmmoCharge);	
    }
    else
    {
        // client simulation of the charge bar
        AmmoCharge = FMin(AmmoCharge+dt*AmmoChargeRate, AmmoChargeMax);
        if (OldAmmo > RepAmmo)
            AmmoCharge -= 1.0;
        OldAmmo = RepAmmo;
    }
}

simulated function DoAutoSwitch()
{
}

simulated function ViewPlayer()
{
    if ( Instigator.Controller.IsA('PlayerController') && PlayerController(Instigator.Controller).ViewTarget == TransBeacon )
    {
        //log("Going ViewPlayer mode");
        PlayerController(Instigator.Controller).ClientSetViewTarget( Instigator );
        PlayerController(Instigator.Controller).SetViewTarget( Instigator );
        bCameraMode = true;
        Transbeacon.SetRotation(PlayerController(Instigator.Controller).Rotation);
    }
    bCameraMode = false;
}

simulated function ViewCamera()
{
    if ( TransBeacon!=None )
    {
        //log("Going Camera mode");
        if ( Instigator.Controller.IsA('PlayerController') )
        {
            PlayerController(Instigator.Controller).SetViewTarget(TransBeacon);
            PlayerController(Instigator.Controller).ClientSetViewTarget(TransBeacon);
            PlayerController(Instigator.Controller).SetRotation( TransBeacon.Rotation );
        }
    }
    bCameraMode = true;
}


simulated function Reselect()
{
    if ( bCameraMode )
        ViewPlayer();
    else
        ViewCamera();
}

simulated event RenderOverlays( Canvas Canvas )
{
	local float tileScaleX, tileScaleY, dist, clr;

	if ( PlayerController(Instigator.Controller).ViewTarget == TransBeacon )
    {
		tileScaleX = Canvas.SizeX / 640.0f;
		tileScaleY = Canvas.SizeY / 480.0f;

        Canvas.DrawColor.R = 255;
		Canvas.DrawColor.G = 255;
		Canvas.DrawColor.B = 255;
		Canvas.DrawColor.A = 255;

        Canvas.Style = 255;
		Canvas.SetPos(0,0);
        Canvas.DrawTile( Material'TransCamFB', Canvas.SizeX, Canvas.SizeY, 0.0, 0.0, 512, 512 ); // !! hardcoded size
        Canvas.SetPos(0,0);

        dist = VSize(TransBeacon.Location - Instigator.Location);
        if ( dist > MaxCamDist )
        {
            clr = 255.0;
        }   
        else
        {
            clr = (dist / MaxCamDist);
            clr *= 255.0;
        }
        clr = Clamp( clr, 20.0, 255.0 );
        Canvas.DrawColor.R = clr;
		Canvas.DrawColor.G = clr;
		Canvas.DrawColor.B = clr;
        Canvas.DrawColor.A = 255;
        Canvas.DrawTile( Material'ScreenNoiseFB', Canvas.SizeX, Canvas.SizeY, 0.0, 0.0, 512, 512 ); // !! hardcoded size	
	}
    else
    {
        Super.RenderOverlays(Canvas);
    }
}

simulated function bool PutDown()
{
    ViewPlayer();
    return Super.PutDown();
}

simulated function Destroyed()
{
    if (TransBeacon != None)
        TransBeacon.Destroy();
    Super.Destroyed();
}

simulated function float ChargeBar()
{
	return AmmoCharge - int(AmmoCharge);
} 

defaultproperties
{
    ItemName="Translocator"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=322,Y1=7,X2=444,Y2=98)

	bShowChargingBar=true
    bCanThrow=false
    AmmoCharge=5.0f
    RepAmmo=5
    AmmoChargeMax=5.0f
    AmmoChargeRate=0.30f
    FireModeClass(0)=TransFire
    FireModeClass(1)=TransRecall
    InventoryGroup=10
    Mesh=mesh'Weapons.TransLauncher_1st'
    BobDamping=1.8
    PickupClass=None
    EffectOffset=(X=100.0,Y=30.0,Z=-19.0)
    DrawScale=1.0
    AttachmentClass=class'TransAttachment'
    DisplayFOV=60.0

    IdleAnimRate=0.25
    PutDownAnim=PutDown
	SelectAnim=Pickup
	    
    PlayerViewOffset=(X=7,Y=7,Z=-4.5)
    PlayerViewPivot=(Pitch=1000,Roll=0,Yaw=400)
    MaxCamDist=4000.0
    UV2Texture=Material'XGameShaders.WeaponEnvShader'
    SelectSound=Sound'WeaponSounds.Translocator_change'
	SelectForce="Translocator_change"


	AIRating=-1.0
	CurrentRating=-1.0

	DefaultPriority=1
}
