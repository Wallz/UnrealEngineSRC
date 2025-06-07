class Weapon extends Inventory
    abstract
    native
    nativereplication;

#exec Texture Import File=Textures\Weapon.tga Name=S_Weapon Mips=Off Alpha=1

replication
{
    // Things the server should send to the client.
    reliable if( Role==ROLE_Authority )
        Ammo;

    // Functions called by server on client
    reliable if( Role==ROLE_Authority )
        ClientWeaponSet, ClientWeaponThrown;

    // functions called by client on server
    reliable if( Role<ROLE_Authority )
        ServerStartFire, ServerStopFire;
}

const NUM_FIRE_MODES = 2;

var() class<WeaponFire> FireModeClass[NUM_FIRE_MODES];
var() editinline WeaponFire FireMode[NUM_FIRE_MODES];
var() Ammunition Ammo[NUM_FIRE_MODES];

// animation //
var() Name IdleAnim;
var() Name RestAnim;
var() Name AimAnim;
var() Name RunAnim;
var() Name SelectAnim;
var() Name PutDownAnim;

var() float IdleAnimRate;
var() float RestAnimRate;
var() float AimAnimRate;
var() float RunAnimRate;
var() float SelectAnimRate;
var() float PutDownAnimRate;

var() Sound SelectSound;
var() String SelectForce;  // jdf

// AI //
var()	int		BotMode; // the fire Mode currently being used for bots
var()	float	AIRating;
var		float	CurrentRating;	// rating result from most recent RateSelf()
var()	bool	bMeleeWeapon;
var()	bool	bSniping;
	
// other useful stuff //
var	  bool bShowChargingBar;
var	  bool bMatchWeapons;	// for team beacons (lightning gun potential links)
var() bool bCanThrow;
var() bool bForceSwitch; // if true, this weapon will prevent any other weapon from delaying the switch to it (bomb launcher)
var() bool bNotInPriorityList; // Should be displayed in a GUI weapon list
var	  bool bNotInDemo;

var class<Weapon> DemoReplacement;
var transient bool bPendingSwitch;
var() vector EffectOffset; // where muzzle flashes and smoke appear. replace by bone reference eventually
var() Localized string MessageNoAmmo;
var() float DisplayFOV;
var() enum EWeaponClientState
{
    WS_None,
    WS_Hidden,
    WS_BringUp,
    WS_PutDown,
    WS_ReadyToFire
} ClientState; // this will always be None on the server

var() config byte ExchangeFireModes;
var() config byte Priority;

var() byte DefaultPriority;

var float Hand;

simulated function float ChargeBar();

simulated function GetAmmoCount(out float MaxAmmoPrimary, out float CurAmmoPrimary)
{
	if ( Ammo[0] == None )
		return;
	MaxAmmoPrimary = Ammo[0].MaxAmmo;
	CurAmmoPrimary = Ammo[0].AmmoAmount;
}

simulated function DrawWeaponInfo(Canvas C);

//=================================================================
// AI functions

function float RangedAttackTime()
{
	return 0;
}

function bool RecommendRangedAttack()
{ 
	return false;
}

function bool FocusOnLeader(bool bLeaderFiring)
{
	return false;
}

function FireHack(byte Mode);

// return true if weapon effect has splash damage (if significant)
// use by bot to avoid hurting self
// should be based on current firing Mode if active
function bool SplashDamage()
{
    return FireMode[BotMode].bSplashDamage;
}

// return true if weapon should be fired to take advantage of splash damage
// For example, rockets should be fired at enemy feet
function bool RecommendSplashDamage()
{
    return FireMode[BotMode].bRecommendSplashDamage;
}

function float GetDamageRadius()
{
    if (FireMode[BotMode].ProjectileClass == None)
        return 0;
    else
        return FireMode[BotMode].ProjectileClass.default.DamageRadius;
}

// Repeater weapons like minigun should be 0.99, other weapons based on likelihood
// of firing again right away
function float RefireRate()
{
    return FireMode[BotMode].BotRefireRate;
}

simulated function DisplayDebug(Canvas Canvas, out float YL, out float YPos)
{
    local int i;
    local string T;
    local name Anim;
    local float frame,rate;

    Canvas.SetDrawColor(0,255,0);
    Canvas.DrawText("WEAPON "$GetItemName(string(self)));
    YPos += YL;
    Canvas.SetPos(4,YPos);

    T = "     STATE: "$GetStateName()$" Timer: "$TimerCounter;

    Canvas.DrawText(T, false);
    YPos += YL;
    Canvas.SetPos(4,YPos);
    
    if ( DrawType == DT_StaticMesh )        
        Canvas.DrawText("     StaticMesh "$GetItemName(string(StaticMesh))$" AmbientSound "$AmbientSound, false);
    else 
        Canvas.DrawText("     Mesh "$GetItemName(string(Mesh))$" AmbientSound "$AmbientSound, false);
    YPos += YL;
    Canvas.SetPos(4,YPos);
    if ( Mesh != None )
    {
        // mesh animation
        GetAnimParams(0,Anim,frame,rate);
        T = "     AnimSequence "$Anim$" Frame "$frame$" Rate "$rate;
        if ( bAnimByOwner )
            T= T$" Anim by Owner";
        
        Canvas.DrawText(T, false);
        YPos += YL;
        Canvas.SetPos(4,YPos);
    }

    for ( i=0; i<NUM_FIRE_MODES; i++ )
    {
        if ( FireMode[i] == None )
        {
            Canvas.DrawText("NO FIREMODE "$i);
            YPos += YL;
            Canvas.SetPos(4,YPos);
        }
        else
            FireMode[i].DisplayDebug(Canvas,YL,YPos);
        
        if ( Ammo[i] == None )
        {
            Canvas.DrawText("NO AMMO "$i);
            YPos += YL;
            Canvas.SetPos(4,YPos);
        }
        else
            Ammo[i].DisplayDebug(Canvas,YL,YPos);
    }
}

simulated function Weapon RecommendWeapon( out float rating )
{
    local Weapon Recommended;
    local float oldRating;

    if ( (Instigator == None) || (Instigator.Controller == None) )
        rating = -2;
    else
        rating = RateSelf() + Instigator.Controller.WeaponPreference(self);

    if ( inventory != None )
    {
        Recommended = inventory.RecommendWeapon(oldRating);
        if ( (Recommended != None) && (oldRating > rating) )
        {
            rating = oldRating;
            return Recommended;
        }
    }
    return self;
}

function SetAITarget(Actor T);

/* BestMode()
choose between regular or alt-fire
*/
function byte BestMode()
{
	if ( Instigator.Controller.bFire != 0 )
		return 0;
	else if ( Instigator.Controller.bAltFire != 0 )
		return 1;
	if ( FRand() < 0.5 )
		return 1;
	return 0;
}

/* BotFire()
called by NPC firing weapon. Weapon chooses appropriate firing Mode to use (typically no change)
bFinished should only be true if called from the Finished() function
FiringMode can be passed in to specify a firing Mode (used by scripted sequences)
*/
function bool BotFire(bool bFinished, optional name FiringMode)
{
    local int newmode;
    local Controller C;

    C = Instigator.Controller;
	newMode = BestMode();

	if ( newMode == 0 )
	{
		C.bFire = 1;
		C.bAltFire = 0;
	}
	else
	{
		C.bFire = 0;
		C.bAltFire = 1;
	}

	if ( bFinished )
		return true;

    if ( FireMode[BotMode].bIsFiring )
		StopFire(BotMode);
	
    if ( !ReadyToFire(newMode) || ClientState != WS_ReadyToFire )
		return false; 

    BotMode = NewMode;
    StartFire(NewMode);
    return true;
}

// this returns the actual projectile spawn location or trace start
simulated function vector GetFireStart(vector X, vector Y, vector Z)
{
    return FireMode[BotMode].GetFireStart(X,Y,Z);
}

simulated function float AmmoStatus() // returns float value for ammo amount
{
    if (Ammo[0] == None)
        return 0.0;
    else
	    return float(Ammo[0].AmmoAmount) / float(Ammo[0].MaxAmmo);
}

// need to figure out modified rating based on enemy/tactical situation
simulated function float RateSelf()
{
    if ( !HasAmmo() )
        CurrentRating = -2;
	else if ( Instigator.Controller == None )
		return 0;
	else
		CurrentRating = Instigator.Controller.RateWeapon(self);
	return CurrentRating;
}

function float GetAIRating()
{
	return AIRating;
}

// tells bot whether to charge or back off while using this weapon
function float SuggestAttackStyle()
{
    return 0.0;
}

// tells bot whether to charge or back off while defending against this weapon
function float SuggestDefenseStyle()
{
    return 0.0;
}

// return true if recommend jumping while firing to improve splash damage (by shooting at feet)
// true for R.L., for example
function bool SplashJump()
{
    return FireMode[BotMode].bSplashJump;
}

// return false if out of range, can't see target, etc.
function bool CanAttack(Actor Other)
{
    local float Dist, CheckDist;
    local vector HitLocation, HitNormal,X,Y,Z, projStart;
    local actor HitActor;
    local int m;
	local bool bInstantHit;

    if ( (Instigator == None) || (Instigator.Controller == None) )
        return false;

    // check that target is within range
    Dist = VSize(Instigator.Location - Other.Location);
    if ( (Dist > FireMode[0].MaxRange()) && (Dist > FireMode[1].MaxRange()) )
        return false;

    // check that can see target
    if ( !Instigator.Controller.LineOfSightTo(Other) )
        return false;

    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
		if ( FireMode[m].bInstantHit )
			bInstantHit = true;
		else
		{
			CheckDist = FMax(CheckDist, 0.5 * FireMode[m].ProjectileClass.Default.Speed);
	        CheckDist = FMax(CheckDist, 300);
	        CheckDist = FMin(CheckDist, VSize(Other.Location - Location));
		}
	}
    // check that would hit target, and not a friendly
    GetAxes(Instigator.Controller.Rotation, X,Y,Z);
    projStart = GetFireStart(X,Y,Z);
    if ( bInstantHit )
        HitActor = Trace(HitLocation, HitNormal, Other.Location + Other.CollisionHeight * vect(0,0,0.8), projStart, true);
    else
    {
        // for non-instant hit, only check partial path (since others may move out of the way)
        HitActor = Trace(HitLocation, HitNormal, 
                projStart + CheckDist * Normal(Other.Location + Other.CollisionHeight * vect(0,0,0.8) - Location), 
                projStart, true);
    }

    if ( (HitActor == None) || (HitActor == Other) || (Pawn(HitActor) == None) 
		|| (Pawn(HitActor).Controller == None) || !Instigator.Controller.SameTeamAs(Pawn(HitActor).Controller) )
        return true;

    return false;
}


//=================================================================

simulated function PostBeginPlay()
{
    local int m;
    Super.PostBeginPlay();
    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
        if (FireModeClass[m] != None)
        {
            FireMode[m] = Spawn(FireModeClass[m], self);
            FireMode[m].ThisModeNum = m;
            FireMode[m].Weapon = self;
            FireMode[m].Instigator = Instigator;
        }
    }
	if ( Level.bDropDetail || (Level.DetailMode == DM_Low) )
		MaxLights = Min(4,MaxLights);
}

simulated function Destroyed()
{
    local int m;

    AmbientSound = None;

    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
        if (FireMode[m] != None)
        {
            FireMode[m].Weapon = None;
            FireMode[m].Destroy();
            FireMode[m] = None;
        }
        if (Ammo[m] != None)
        {
            Ammo[m].Destroy();
            Ammo[m] = None;
        }
    }
    Super.Destroyed();
}

simulated function Reselect()
{
}

simulated event RenderOverlays( Canvas Canvas )
{
    local int m;

    if ((Hand < -1.0) || (Hand > 1.0))
        return;

    if (Instigator == None)
        return;

    // draw muzzleflashes/smoke for all fire modes so idle state won't
    // cause emitters to just disappear
    Canvas.DrawActor(None, false, true); // amb: Clear the z-buffer here

    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
        if (FireMode[m] != None)
        {
            FireMode[m].DrawMuzzleFlash(Canvas);
        }
    }

    SetLocation( Instigator.Location + Instigator.CalcDrawOffset(self) );
    SetRotation( Instigator.GetViewRotation() );

    bDrawingFirstPerson = true;
    Canvas.DrawActor(self, false, false, DisplayFOV);
    bDrawingFirstPerson = false;
}

simulated function SetHand(float InHand)
{
    Hand = InHand;
}

simulated function GetViewAxes( out vector xaxis, out vector yaxis, out vector zaxis )
{
    if ( Instigator.Controller == None )
        GetAxes( Instigator.Rotation, xaxis, yaxis, zaxis );
    else
        GetAxes( Instigator.Controller.Rotation, xaxis, yaxis, zaxis );
}

simulated function vector GetEffectStart()
{
    local Vector X,Y,Z;

    // jjs - this function should actually never be called in third person views
    // any effect that needs a 3rdp weapon offset should figure it out itself

    // 1st person
    if (Instigator.IsFirstPerson())
    {
        GetViewAxes(X, Y, Z);
        return (Instigator.Location + 
            Instigator.CalcDrawOffset(self) + 
            EffectOffset.X * X + 
            EffectOffset.Y * Y + 
            EffectOffset.Z * Z); 
    }
    // 3rd person
    else
    {
        GetViewAxes(X, Y, Z);
        return (Instigator.Location + 
            Instigator.EyeHeight*Vect(0,0,0.5) + 
            Vector(Instigator.Rotation) * 40.0); 
    }
}

simulated function IncrementFlashCount(int Mode)
{
    if ( WeaponAttachment(ThirdPersonActor) != None )
    {
        if (Mode == 0)
            WeaponAttachment(ThirdPersonActor).FiringMode = 0;
        else
            WeaponAttachment(ThirdPersonActor).FiringMode = 1;
        WeaponAttachment(ThirdPersonActor).FlashCount++;
        WeaponAttachment(ThirdPersonActor).ThirdPersonEffects();
    }
}

simulated function ZeroFlashCount(int Mode)
{
    if ( WeaponAttachment(ThirdPersonActor) != None )
    {
        if (Mode == 0)
            WeaponAttachment(ThirdPersonActor).FiringMode = 0;
        else
            WeaponAttachment(ThirdPersonActor).FiringMode = 1;
        WeaponAttachment(ThirdPersonActor).FlashCount = 0;
        WeaponAttachment(ThirdPersonActor).ThirdPersonEffects();
    }
}

simulated function Weapon WeaponChange( byte F, bool bSilent )
{   
    local Weapon newWeapon;

    if ( InventoryGroup == F )
    {
        if ( !HasAmmo() )
        {
            if ( Inventory == None )
                newWeapon = None;
            else
                newWeapon = Inventory.WeaponChange(F,bSilent);

            if ( !bSilent && (newWeapon == None) && Instigator.IsHumanControlled() )
                Instigator.ClientMessage( ItemName$MessageNoAmmo );

            return newWeapon;
        }       
        else 
            return self;
    }
    else if ( Inventory == None )
        return None;
    else
        return Inventory.WeaponChange(F,bSilent);
}

simulated function Weapon PrevWeapon(Weapon CurrentChoice, Weapon CurrentWeapon)
{
    if ( HasAmmo() )
    {
        if ( (CurrentChoice == None) )
        {
            if ( CurrentWeapon != self )
                CurrentChoice = self;
        }
        else if ( InventoryGroup == CurrentWeapon.InventoryGroup )
        {
            if ( (GroupOffset < CurrentWeapon.GroupOffset)
                && ((CurrentChoice.InventoryGroup != InventoryGroup) || (GroupOffset > CurrentChoice.GroupOffset)) )
                CurrentChoice = self;
		}
        else if ( InventoryGroup == CurrentChoice.InventoryGroup )
        {
            if ( GroupOffset > CurrentChoice.GroupOffset )
                CurrentChoice = self;
        }
        else if ( InventoryGroup > CurrentChoice.InventoryGroup )
        {
			if ( (InventoryGroup < CurrentWeapon.InventoryGroup)
                || (CurrentChoice.InventoryGroup > CurrentWeapon.InventoryGroup) )
                CurrentChoice = self;
        }
        else if ( (CurrentChoice.InventoryGroup > CurrentWeapon.InventoryGroup)
                && (InventoryGroup < CurrentWeapon.InventoryGroup) )
            CurrentChoice = self;
    }
    if ( Inventory == None )
        return CurrentChoice;
    else
        return Inventory.PrevWeapon(CurrentChoice,CurrentWeapon);
}

simulated function Weapon NextWeapon(Weapon CurrentChoice, Weapon CurrentWeapon)
{
    if ( HasAmmo() )
    {
        if ( (CurrentChoice == None) )
        {
            if ( CurrentWeapon != self )
                CurrentChoice = self;
        }
        else if ( InventoryGroup == CurrentWeapon.InventoryGroup )
        {
            if ( (GroupOffset > CurrentWeapon.GroupOffset)
                && ((CurrentChoice.InventoryGroup != InventoryGroup) || (GroupOffset < CurrentChoice.GroupOffset)) )
                CurrentChoice = self;
        }
        else if ( InventoryGroup == CurrentChoice.InventoryGroup )
        {
			if ( GroupOffset < CurrentChoice.GroupOffset )
                CurrentChoice = self;
        }

        else if ( InventoryGroup < CurrentChoice.InventoryGroup )
        {
            if ( (InventoryGroup > CurrentWeapon.InventoryGroup)
                || (CurrentChoice.InventoryGroup < CurrentWeapon.InventoryGroup) )
                CurrentChoice = self;
        }
        else if ( (CurrentChoice.InventoryGroup < CurrentWeapon.InventoryGroup)
                && (InventoryGroup > CurrentWeapon.InventoryGroup) )
            CurrentChoice = self;
    }
    if ( Inventory == None )
        return CurrentChoice;
    else
        return Inventory.NextWeapon(CurrentChoice,CurrentWeapon);
}


function HolderDied()
{
    local int m;

    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
        if (FireMode[m].bIsFiring)
        {
            StopFire(m);
            if (FireMode[m].bFireOnRelease)
                FireMode[m].ModeDoFire();
        }
    }
}

simulated function bool CanThrow()
{
    return (bCanThrow && (ClientState == WS_ReadyToFire || Level.NetMode == NM_DedicatedServer));
}

function DropFrom(vector StartLocation)
{
    local int m;
	local Pickup Pickup;

    if (!bCanThrow || !HasAmmo())
        return;

    ClientWeaponThrown();

    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
        if (FireMode[m].bIsFiring)
            StopFire(m);
    }

	if ( Instigator != None )
	{
		DetachFromPawn(Instigator);	
	}	

	Pickup = Spawn(PickupClass,,, StartLocation);
	if ( Pickup != None )
	{
    	Pickup.InitDroppedPickupFor(self);
	    Pickup.Velocity = Velocity;
        if (Instigator.Health > 0)
            WeaponPickup(Pickup).bThrown = true;
    }

    Destroy();
}

simulated function DetachFromPawn(Pawn P)
{
	Super.DetachFromPawn(P);
	P.AmbientSound = None;
}

simulated function ClientWeaponThrown()
{
    local int m;

    AmbientSound = None;
    Instigator.AmbientSound = None;

    if( Level.NetMode != NM_Client )
        return;

    Instigator.DeleteInventory(self);
    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
        if (Ammo[m] != None)
            Instigator.DeleteInventory(Ammo[m]);
    }
}

function GiveTo(Pawn Other, optional Pickup Pickup)
{
    local int m;
    local weapon w;
    local bool bPossiblySwitch, bJustSpawned;

    Instigator = Other;

    w = Weapon(Instigator.FindInventoryType(class));
    if (w == None)
    {
		bJustSpawned = true;
        Super.GiveTo(Other);
        bPossiblySwitch = true;
    }
    else
    {
        if (!W.HasAmmo())
            bPossiblySwitch = true;
    }

    if ( Pickup == None )
        bPossiblySwitch = true;

    for (m = 0; m < NUM_FIRE_MODES; m++)
    {
        if ( FireMode[m] != None )
        {
            FireMode[m].Instigator = Instigator;
            GiveAmmo(m,WeaponPickup(Pickup),bJustSpawned);
        }
    }

	if ( (Instigator.Weapon != None) && Instigator.Weapon.IsFiring() )
		bPossiblySwitch = false;
		
    ClientWeaponSet(bPossiblySwitch);
}

function GiveAmmo(int m, WeaponPickup WP, bool bJustSpawned)
{
    local bool bJustSpawnedAmmo;
    local int addAmount;

    if ( FireMode[m] != None && FireMode[m].AmmoClass != None )
    {
        Ammo[m] = Ammunition(Instigator.FindInventoryType(FireMode[m].AmmoClass));
		bJustSpawnedAmmo = false;
		
        if ( (Ammo[m] == None) && (FireMode[m].AmmoClass != None) )
        {
            Ammo[m] = Spawn(FireMode[m].AmmoClass, FireMode[m]);
            Instigator.AddInventory(Ammo[m]);
            bJustSpawnedAmmo = true;
        }
        else if ( (m == 0) || (FireMode[m].AmmoClass != FireMode[0].AmmoClass) ) 
			bJustSpawnedAmmo = ( bJustSpawned || ((WP != None) && !WP.bWeaponStay) );

        if ( (WP != None) && (WP.AmmoAmount[m] > 0) )
        {
            addAmount = WP.AmmoAmount[m];
        }
        else if ( bJustSpawnedAmmo )
        {
            addAmount = Ammo[m].InitialAmount;
        }
        
        Ammo[m].AddAmmo(addAmount);
        Ammo[m].GotoState('');
    }
}   

simulated function ClientWeaponSet(bool bPossiblySwitch)
{
    local int Mode;

    Instigator = Pawn(Owner);

    bPendingSwitch = bPossiblySwitch;

    if( Instigator == None )
    {
        GotoState('PendingClientWeaponSet');
        return;
    }

    for( Mode = 0; Mode < NUM_FIRE_MODES; Mode++ )
    {
        if( FireModeClass[Mode] != None )
        {
            if( ( FireMode[Mode] == None ) || ( FireMode[Mode].AmmoClass != None ) && ( Ammo[Mode] == None ) )
            {
                GotoState('PendingClientWeaponSet');
                return;
            }
        }
            
        FireMode[Mode].Instigator = Instigator;
    }
 
    ClientState = WS_Hidden;
    GotoState('Hidden');

    if( Level.NetMode == NM_DedicatedServer || !Instigator.IsHumanControlled() )
        return;

    if( Instigator.Weapon == self || Instigator.PendingWeapon == self ) // this weapon was switched to while waiting for replication, switch to it now
    {
        if (Instigator.PendingWeapon != None)
            Instigator.ChangedWeapon();
        else
            BringUp();
        return;
    }

    if( Instigator.PendingWeapon != None && Instigator.PendingWeapon.bForceSwitch )
        return;

    if( Instigator.Weapon == None )
    {
        Instigator.PendingWeapon = self;
        Instigator.ChangedWeapon();
    }
    else if ( bPossiblySwitch )
    {
		if ( PlayerController(Instigator.Controller) != None && PlayerController(Instigator.Controller).bNeverSwitchOnPickup )
			return;
        if ( Instigator.PendingWeapon != None )
        {
            if ( RateSelf() > Instigator.PendingWeapon.RateSelf() )
            {
                Instigator.PendingWeapon = self;
                Instigator.Weapon.PutDown();
            }
        }
        else if ( RateSelf() > Instigator.Weapon.RateSelf() )
        {
            Instigator.PendingWeapon = self;
            Instigator.Weapon.PutDown();
        }
    }
}

// jdf ---
simulated function ClientPlayForceFeedback( String EffectName )
{
    local PlayerController PC;

    PC = PlayerController(Instigator.Controller);
    if ( PC != None && PC.bEnableWeaponForceFeedback && Instigator.IsLocallyControlled() )
    {
        PC.ClientPlayForceFeedback( EffectName );
    }
}

simulated function StopForceFeedback( String EffectName )
{
    local PlayerController PC;

    PC = PlayerController(Instigator.Controller);
    if ( PC != None && PC.bEnableWeaponForceFeedback && Instigator.IsLocallyControlled() )
    {
        PC.StopForceFeedback( EffectName );
    }
}
// --- jdf

simulated function BringUp(optional Weapon PrevWeapon)
{
   local int Mode;

    if (ClientState == WS_Hidden)
    {
        PlayOwnedSound(SelectSound, SLOT_Interact,,,,, false);
		ClientPlayForceFeedback(SelectForce);  // jdf

        if (Instigator.IsLocallyControlled())
        {
            if (HasAnim(SelectAnim))
                PlayAnim(SelectAnim, SelectAnimRate, 0.0);
        }

        ClientState = WS_BringUp;
        SetTimer(0.3, false);
    }
    for (Mode = 0; Mode < NUM_FIRE_MODES; Mode++)
	{
		FireMode[Mode].bIsFiring = false;
		FireMode[Mode].HoldTime = 0.0;
		FireMode[Mode].bServerDelayStartFire = false;
		FireMode[Mode].bServerDelayStopFire = false;
	}	
}

simulated function bool PutDown()
{
    local int Mode;

    if (ClientState == WS_BringUp || ClientState == WS_ReadyToFire)
    {
        if (!Instigator.PendingWeapon.bForceSwitch)
        {
            for (Mode = 0; Mode < NUM_FIRE_MODES; Mode++)
            {
                if (FireMode[Mode].bFireOnRelease && FireMode[Mode].bIsFiring)
                    return false;
            }
        }

        if (Instigator.IsLocallyControlled())
        {
            for (Mode = 0; Mode < NUM_FIRE_MODES; Mode++)
            {
                if (FireMode[Mode].bIsFiring)
                    ClientStopFire(Mode);
            }

            if (ClientState != WS_BringUp && HasAnim(PutDownAnim))
                PlayAnim(PutDownAnim, PutDownAnimRate, 0.0);
        }
        ClientState = WS_PutDown;

        SetTimer(0.3, false);
    }
    for (Mode = 0; Mode < NUM_FIRE_MODES; Mode++)
    {
		FireMode[Mode].bServerDelayStartFire = false;
		FireMode[Mode].bServerDelayStopFire = false;
	}	
    Instigator.AmbientSound = None;
    return true; // return false if preventing weapon switch
}

simulated function Fire(float F)
{
}

simulated function AltFire(float F)
{
}

simulated event WeaponTick(float dt); // only called on currently held weapon

simulated function OutOfAmmo()
{
    if ( !Instigator.IsLocallyControlled() || HasAmmo() )
        return;

    DoAutoSwitch();
}

simulated function DoAutoSwitch()
{
    Instigator.Controller.SwitchToBestWeapon();
}

//// client only ////
simulated event ClientStartFire(int Mode)
{
    if (Pawn(Owner).Controller.IsInState('GameEnded'))
        return;
        
    if (Role < ROLE_Authority)
    {
        if (StartFire(Mode))
        {
            //Log("ClientStartFire"@Level.TimeSeconds);
            ServerStartFire(Mode);
        }
    }
    else
    {
        StartFire(Mode);
    }
}

simulated event ClientStopFire(int Mode)
{
    if (Role < ROLE_Authority)
    {
        //Log("ClientStopFire"@Level.TimeSeconds);
        StopFire(Mode);
    }
    ServerStopFire(Mode);    
}

//// server only ////
event ServerStartFire(byte Mode)
{
    if ( (FireMode[Mode].NextFireTime <= Level.TimeSeconds + FireMode[Mode].PreFireTime)
		&& StartFire(Mode) )
    {
        //Log("ServerStartFire"@Level.TimeSeconds);
        FireMode[Mode].ServerStartFireTime = Level.TimeSeconds;
        FireMode[Mode].bServerDelayStartFire = false;
    }
    else
        FireMode[Mode].bServerDelayStartFire = true;
}

function ServerStopFire(byte Mode)
{
    // if a stop was received on the same frame as a start then we need to delay the stop for one frame
    if (FireMode[Mode].bServerDelayStartFire || FireMode[Mode].ServerStartFireTime == Level.TimeSeconds)
    {
        //log("Stop Delayed");
        FireMode[Mode].bServerDelayStopFire = true;
    }
    else
    {
        //Log("ServerStopFire"@Level.TimeSeconds);
        StopFire(Mode);
    }
}

simulated function bool ReadyToFire(int Mode)
{
    local int alt;

    if (Mode == 0) 
        alt = 1; 
    else 
        alt = 0;

    if ( (FireMode[alt].bModeExclusive && FireMode[alt].bIsFiring)	
		|| !FireMode[Mode].AllowFire()
		|| (FireMode[Mode].NextFireTime > Level.TimeSeconds + FireMode[Mode].PreFireTime) )
    {
        return false;
    }

	return true;
}

//// client & server ////
simulated function bool StartFire(int Mode)
{
    local int alt;

    if (!ReadyToFire(Mode))
        return false;

    if (Mode == 0) 
        alt = 1; 
    else 
        alt = 0;

    FireMode[Mode].bIsFiring = true;
    FireMode[Mode].NextFireTime = Level.TimeSeconds + FireMode[Mode].PreFireTime;

    if (FireMode[alt].bModeExclusive)
    {
        // prevents rapidly alternating fire modes
        FireMode[Mode].NextFireTime = FMax(FireMode[Mode].NextFireTime, FireMode[alt].NextFireTime);
    }

    if (Instigator.IsLocallyControlled())
    {
        if (FireMode[Mode].PreFireTime > 0.0 || FireMode[Mode].bFireOnRelease)
        {
            FireMode[Mode].PlayPreFire();
        }
        FireMode[Mode].FireCount = 0;
    }

    return true;
}

simulated event StopFire(int Mode)
{
    if (Instigator.IsLocallyControlled() && !FireMode[Mode].bFireOnRelease)
        FireMode[Mode].PlayFireEnd();

    FireMode[Mode].bIsFiring = false;
    FireMode[Mode].StopFiring();
    if (!FireMode[Mode].bFireOnRelease)
        ZeroFlashCount(Mode);
}

simulated function Timer()
{
	local int Mode;
	
    if (ClientState == WS_BringUp)
    {
		for( Mode = 0; Mode < NUM_FIRE_MODES; Mode++ )
	       FireMode[Mode].InitEffects();
        PlayIdle();
        ClientState = WS_ReadyToFire;
    }
    else if (ClientState == WS_PutDown)
    {
		if ( Instigator.PendingWeapon == None )
		{
			PlayIdle();
			ClientState = WS_ReadyToFire;
		}
		else
		{
			ClientState = WS_Hidden;
			Instigator.ChangedWeapon();
			for( Mode = 0; Mode < NUM_FIRE_MODES; Mode++ )
				FireMode[Mode].DestroyEffects();
		}
    }
}


simulated function bool IsFiring() // called by pawn animation, mostly
{
    return  ( ClientState == WS_ReadyToFire && (FireMode[0].IsFiring() || FireMode[1].IsFiring()) );
}

function bool IsRapidFire() // called by pawn animation
{
    if (FireMode[1] != None && FireMode[1].bIsFiring) 
        return FireMode[1].bPawnRapidFireAnim;
    else if (FireMode[0] != None)
        return FireMode[0].bPawnRapidFireAnim;
    else
        return false;
}

function ConsumeAmmo(int Mode, float load)
{
    if (Ammo[Mode] != None)
        Ammo[Mode].UseAmmo(int(load));
}

simulated function bool HasAmmo()
{
    return ( (Ammo[0] != None && FireMode[0] != None && Ammo[0].AmmoAmount >= FireMode[0].AmmoPerFire)
          || (Ammo[1] != None && FireMode[1] != None && Ammo[1].AmmoAmount >= FireMode[1].AmmoPerFire) );
}

// called every time owner takes damage while holding this weapon - used by shield gun
function AdjustPlayerDamage( out int Damage, Pawn InstigatedBy, Vector HitLocation, 
                             out Vector Momentum, class<DamageType> DamageType)
{
}

simulated function StartBerserk()
{
    if (FireMode[0] != None)
        FireMode[0].StartBerserk();
    if (FireMode[1] != None)
        FireMode[1].StartBerserk();
}

simulated function StopBerserk()
{
    if (FireMode[0] != None)
        FireMode[0].StopBerserk();
    if (FireMode[1] != None)
        FireMode[1].StopBerserk();
}

function AnimEnd(int channel)
{
    local name anim;
    local float frame, rate;

    GetAnimParams(0, anim, frame, rate);

    if (ClientState == WS_ReadyToFire)
    {
        if (anim == FireMode[0].FireAnim && HasAnim(FireMode[0].FireEndAnim)) // rocket hack
        {
            PlayAnim(FireMode[0].FireEndAnim, FireMode[0].FireEndAnimRate, 0.0);
        }
        else if (anim== FireMode[1].FireAnim && HasAnim(FireMode[1].FireEndAnim))
        {
            PlayAnim(FireMode[1].FireEndAnim, FireMode[1].FireEndAnimRate, 0.0);
        }
        else if ((FireMode[0] == None || !FireMode[0].bIsFiring) && (FireMode[1] == None || !FireMode[1].bIsFiring))
        {
            PlayIdle();
        }
    }
}

simulated function PlayIdle()
{
    LoopAnim(IdleAnim, IdleAnimRate, 0.2);
}

state PendingClientWeaponSet
{
    simulated function Timer()
    {
        if ( Pawn(Owner) != None )
            ClientWeaponSet(bPendingSwitch);
    }

    simulated function BeginState()
    {
        SetTimer(0.05, true);
    }

    simulated function EndState()
    {
        SetTimer(0.0, false);
    }
}

state Hidden
{
}

function bool CheckReflect( Vector HitLocation, out Vector RefNormal, int AmmoDrain )
{
    return false;
}

function DoReflectEffect(int Drain)
{

}

function bool HandlePickupQuery( pickup Item )
{
    local WeaponPickup wpu;

	if (class == Item.InventoryType)
    {
        wpu = WeaponPickup(Item);
        if (wpu != None)
            return !wpu.AllowRepeatPickup();
        else
            return false;
    }

    if ( Inventory == None )
		return false;

	return Inventory.HandlePickupQuery(Item);
}

// ugly hack for tutorial
function bool ShootHoop(Controller B, Vector ShootLoc)
{
	return false;
}

defaultproperties
{
    DrawType=DT_Mesh
    Style=STY_Normal
    PlayerViewOffset=(X=0,Y=0,Z=0)
    InventoryGroup=1

    FireModeClass(0)=None
    FireModeClass(1)=None

    // animation //
    IdleAnim=Idle
    RestAnim=Rest
    AimAnim=Aim
    RunAnim=Run
    SelectAnim=Select
    PutDownAnim=Down

    IdleAnimRate=1.0
    RestAnimRate=1.0
    AimAnimRate=1.0
    RunAnimRate=1.0
    SelectAnimRate=1.5
    PutDownAnimRate=1.5

    // other useful stuff //
    MessageNoAmmo=" has no ammo"
    DisplayFOV=90.0
    bCanThrow=true

    AIRating=0.5
    CurrentRating=0.5

    AttachmentClass=class'WeaponAttachment'

    NetPriority=3.0
    ScaleGlow=1.5
    AmbientGlow=20
    MaxLights=6
    SoundVolume=255
}
