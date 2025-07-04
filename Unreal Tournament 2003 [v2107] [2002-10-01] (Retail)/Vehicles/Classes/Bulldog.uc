#exec OBJ LOAD FILE=..\staticmeshes\BulldogMeshes.usx
#exec OBJ LOAD FILE=..\sounds\WeaponSounds.uax
#exec OBJ LOAD FILE=..\textures\VehicleFX.utx

class Bulldog extends KCar
    placeable;

// triggers used to get into the Bulldog
var const vector	FrontTriggerOffset;
var BulldogTrigger	FLTrigger, FRTrigger, FlipTrigger;


// headlight projector
var const vector		HeadlightOffset;
var BulldogHeadlight	Headlight;
var bool				HeadlightOn;

// Light materials
var Material			ReverseMaterial;
var Material			BrakeMaterial;
var Material			TailOffMaterial;

var Material			HeadlightOnMaterial;
var Material			HeadlightOffMaterial;

var BulldogHeadlightCorona HeadlightCorona[8];
var (Bulldog) vector	HeadlightCoronaOffset[8];

// Used to prevent triggering sounds continuously
var float			UntilNextImpact;

// Wheel dirt emitter
var BulldogDust		Dust[4]; // FL, FR, RL, RR

// Distance below centre of tire to place dust.
var float			DustDrop;

// Ratio between dust kicked up and amount wheels are slipping
var (Bulldog) float	DustSlipRate;
var (Bulldog) float DustSlipThresh;

// Maximum speed at which you can get in the vehicle.
var (Bulldog) float	TriggerSpeedThresh;
var bool			TriggerState; // true for on, false for off.
var bool			FlipTriggerState;

// Destroyed Buggy
var (Bulldog) class<Actor>	DestroyedEffect;
var (Bulldog) sound			DestroyedSound;

// Weapon
var			  float	FireCountdown;
var			  float SinceLastTargetting;
var			  Pawn  CurrentTarget;

var (Bulldog) float		FireInterval;
var (Bulldog) float		TargettingInterval;
var (Bulldog) vector	RocketFireOffset; // Position (car ref frame) that rockets are launched from
var (Bulldog) float     TargettingRange;
var (Bulldog) Material	TargetMaterial;


// Bulldog sound things
var (Bulldog) float	EnginePitchScale;
var (Bulldog) sound	BulldogStartSound;
var (Bulldog) sound	BulldogIdleSound;
var (Bulldog) float	HitSoundThreshold;
var (Bulldog) sound	BulldogSquealSound;
var (Bulldog) float	SquealVelThresh;
var (Bulldog) sound	BulldogHitSound;

var (Bulldog) String BulldogStartForce;
var (Bulldog) String BulldogIdleForce;
var (Bulldog) String BulldogSquealForce;
var (Bulldog) String BulldogHitForce;

replication
{
	reliable if(Role == ROLE_Authority)
		HeadlightOn, CurrentTarget;
}

function PreBeginPlay()
{
	Super.PreBeginPlay();
	if ( Level.IsDemoBuild() )
		Destroy();
}

simulated function PostNetBeginPlay()
{
	local vector RotX, RotY, RotZ;
	local int i;

    Super.PostNetBeginPlay();

    GetAxes(Rotation,RotX,RotY,RotZ);

	// Only have triggers on server
	if(Level.NetMode != NM_Client)
	{
		// Create triggers for gettting into the Bulldog
		FLTrigger = spawn(class'BulldogTrigger', self,, Location + FrontTriggerOffset.X * RotX + FrontTriggerOffset.Y * RotY + FrontTriggerOffset.Z * RotZ);
		FLTrigger.SetBase(self);
		FLTrigger.SetCollision(true, false, false);

		FRTrigger = spawn(class'BulldogTrigger', self,, Location + FrontTriggerOffset.X * RotX - FrontTriggerOffset.Y * RotY + FrontTriggerOffset.Z * RotZ);
		FRTrigger.SetBase(self);
		FRTrigger.SetCollision(true, false, false);

		TriggerState = true;

		// Create trigger for flipping the bulldog back over.
		FlipTrigger = spawn(class'BulldogTrigger', self,, Location);
		Fliptrigger.bCarFlipTrigger = true;
		FlipTrigger.SetBase(self);
		FlipTrigger.SetCollisionSize(350, 200);
		FlipTrigger.SetCollision(false, false, false);

		FlipTriggerState = false;
	}

	// Dont bother making emitters etc. on dedicated server
	if(Level.NetMode != NM_DedicatedServer)
	{
		// Create headlight projector. We make sure it doesn't project on the vehicle itself.
		Headlight = spawn(class'BulldogHeadlight', self,, Location + HeadlightOffset.X * RotX + HeadlightOffset.Y * RotY + HeadlightOffset.Z * RotZ);
		Headlight.SetBase(self);
		Headlight.SetRelativeRotation(rot(-2000, 32768, 0));

		// Create wheel dust emitters.
		Dust[0] = spawn(class'BulldogDust', self,, Location + WheelFrontAlong*RotX + WheelFrontAcross*RotY + (WheelVert-DustDrop)*RotZ);
		Dust[0].SetBase(self);

		Dust[1] = spawn(class'BulldogDust', self,, Location + WheelFrontAlong*RotX - WheelFrontAcross*RotY + (WheelVert-DustDrop)*RotZ);
		Dust[1].SetBase(self);	

		Dust[2] = spawn(class'BulldogDust', self,, Location + WheelRearAlong*RotX + WheelRearAcross*RotY + (WheelVert-DustDrop)*RotZ);
		Dust[2].SetBase(self);

		Dust[3] = spawn(class'BulldogDust', self,, Location + WheelRearAlong*RotX - WheelRearAcross*RotY + (WheelVert-DustDrop)*RotZ);
		Dust[3].SetBase(self);

		// Set light materials
		Skins[1]=BrakeMaterial;
		Skins[2]=HeadlightOffMaterial;

		// Spawn headlight coronas
		for(i=0; i<8; i++)
		{
			HeadlightCorona[i] = spawn(class'BulldogHeadlightCorona', self,, Location + (HeadlightCoronaOffset[i] >> Rotation) );
			HeadlightCorona[i].SetBase(self);
		}
	}

    // For KImpact event
    KSetImpactThreshold(HitSoundThreshold);

	// If this is not 'authority' version - don't destroy it if there is a problem. 
	// The network should sort things out.
	if(Role != ROLE_Authority)
	{
		KarmaParams(KParams).bDestroyOnSimError = False;
		KarmaParams(frontLeft.KParams).bDestroyOnSimError = False;
		KarmaParams(frontRight.KParams).bDestroyOnSimError = False;
		KarmaParams(rearLeft.KParams).bDestroyOnSimError = False;
		KarmaParams(rearRight.KParams).bDestroyOnSimError = False;
	}
}

simulated event Destroyed()
{
	local int i;

	//Log("Bulldog Destroyed");

	// Clean up random stuff attached to the car
	if(Level.NetMode != NM_Client)
	{

		FLTrigger.Destroy();
		FRTrigger.Destroy();
		FlipTrigger.Destroy();
	}

	if(Level.NetMode != NM_DedicatedServer)
	{
		Headlight.Destroy();

		for(i=0; i<4; i++)
			Dust[i].Destroy();

		for(i=0; i<8; i++)
			HeadlightCorona[i].Destroy();
	}

	// This will destroy wheels & joints
	Super.Destroyed();

	// Trigger destroyed sound and effect
	if(Level.NetMode != NM_DedicatedServer)
	{
		spawn(DestroyedEffect, self, , Location );
		PlaySound(DestroyedSound);
	}
}

function KImpact(actor other, vector pos, vector impactVel, vector impactNorm)
{
    /* Only make noises for Bulldog-world impacts */
    if(other != None)
        return;

    if(UntilNextImpact < 0)
    {
        //PlaySound(BulldogHitSound);

        // hack to stop the sound repeating rapidly on impacts
        //UntilNextImpact = GetSoundDuration(BulldogHitSound);
        
        
    }
}

simulated function ClientKDriverEnter(PlayerController pc)
{
	//Log("Bulldog ClientKDriverEnter");

	Super.ClientKDriverEnter(pc);
}

function KDriverEnter(Pawn p)
{
	//Log("Bulldog KDriverEnter");

	Super.KDriverEnter(p);

	//PlaySound(BulldogStartSound);

	//AmbientSound=BulldogIdleSound;

	p.bHidden = True;
	ReceiveLocalizedMessage(class'Vehicles.BulldogMessage', 1);
}

simulated function ClientKDriverLeave(PlayerController pc)
{
	//Log("Bulldog ClientKDriverLeave");

	Super.ClientKDriverLeave(pc);
}

function bool KDriverLeave(bool bForceLeave)
{
	local Pawn OldDriver;

	//Log("Bulldog KDriverLeave");

	OldDriver = Driver;

	// If we succesfully got out of the car, make driver visible again.
	if( Super.KDriverLeave(bForceLeave) )
	{
		OldDriver.bHidden = false;
		AmbientSound = None;	
		return true;
	}
	else
		return false;
}

function LaunchRocket(bool bWasAltFire)
{
	local vector RotX, RotY, RotZ, FireLocation;
	local BulldogRocket R;
	local PlayerController PC;

	// Client can't do firing
	if(Role != ROLE_Authority)
		return;

	// Fire as many rockets as we have time to.
	GetAxes(Rotation, RotX, RotY, RotZ);
	FireLocation = Location + (RocketFireOffset >> Rotation);

	while(FireCountdown <= 0)
	{
		if(!bWasAltFire)
		{
			// Fire a rocket at the current target (starts flying forward and up a bit)

			R = spawn(class'Vehicles.BulldogRocket', self, , FireLocation, rotator(-1 * RotX + 0.2 * RotZ));
			R.Seeking = CurrentTarget;
	
			// Play firing noise
			PlaySound(Sound'WeaponSounds.RocketLauncher.RocketLauncherFire', SLOT_None,,,,, false);
			PC = PlayerController(Controller);
			if (PC != None && PC.bEnableWeaponForceFeedback)
				PC.ClientPlayForceFeedback("RocketLauncherFire");
		}
		else
		{
			//Log("Would AltFire At:"$CurrentTarget);
		}

		FireCountdown += FireInterval;
	}
}

// Fire a rocket (if we've had time to reload!)
function VehicleFire(bool bWasAltFire)
{
	Super.VehicleFire(bWasAltFire);

	if(FireCountdown < 0)
	{
		FireCountdown = 0;
		LaunchRocket(bWasAltFire);
	}
}

// For drawing targetting reticule
simulated function DrawHud(Canvas C)
{
	local int XPos, YPos;
	local vector ScreenPos;
	local float RatioX, RatioY;
	local float tileX, tileY;
	local float SizeX, SizeY, PosDotDir;
	local vector CameraLocation, CamDir;
	local rotator CameraRotation;

	if(CurrentTarget == None)
		return;

	SizeX = 64.0;
	SizeY = 64.0;

	ScreenPos = C.WorldToScreen( CurrentTarget.Location );

	// Dont draw reticule if target is behind camera
	C.GetCameraLocation( CameraLocation, CameraRotation );
	CamDir = vector(CameraRotation);
	PosDotDir = (CurrentTarget.Location - CameraLocation) Dot CamDir;
	if( PosDotDir < 0)
		return;

	RatioX = C.SizeX / 640.0;
	RatioY = C.SizeY / 480.0;

	tileX = sizeX * RatioX;
	tileY = sizeY * RatioX;

	XPos = ScreenPos.X;
	YPos = ScreenPos.Y;
	C.Style = ERenderStyle.STY_Additive;
	C.DrawColor.R = 255;
	C.DrawColor.G = 255;
	C.DrawColor.B = 255;
	C.DrawColor.A = 255;
	C.SetPos(XPos - tileX*0.5, YPos - tileY*0.5);
	C.DrawTile( TargetMaterial, tileX, tileY, 0.0, 0.0, 256, 256); //--- TODO : Fix HARDCODED USIZE
}

simulated function Tick(float Delta)
{
    Local float BestAim, BestDist, TotalSlip, EnginePitch, VMag;
	Local vector RotX, RotY, RotZ, FireLocation;
	local pawn NewTarget;
	local int i;

    Super.Tick(Delta);

	// Weapons (run on server and replicated to client)
	if(Role == ROLE_Authority)
	{
		// Only put headlights on when driver is in. Save batteries!
		if(Driver != None)
			HeadlightOn=true;
		else
			HeadlightOn=false;

		// Do targetting (do nothing if no controller)
		SinceLastTargetting += Delta;
		if(Controller != None && SinceLastTargetting > TargettingInterval)
		{
			GetAxes(Rotation, RotX, RotY, RotZ);
	
			FireLocation = Location + (RocketFireOffset >> Rotation);

			// BestAim is zero - so we will only target things in front of the buggy
			BestAim = 0;
			NewTarget = Controller.PickTarget(BestAim, BestDist, -1.0 * RotX, FireLocation, TargettingRange);
			
			// If this is a new lock - play lock-on noise
			if(NewTarget != CurrentTarget)
			{
				if(NewTarget == None)
					PlayerController(Controller).ClientPlaySound(Sound'WeaponSounds.SeekLost');
				else
					PlayerController(Controller).ClientPlaySound(Sound'WeaponSounds.LockOn');
			}

			CurrentTarget = NewTarget;
			SinceLastTargetting = 0;
		}

		// Countdown to next shot
		FireCountdown -= Delta;

		// This is for sustained barrages.
		// Primary fire takes priority
		if(bVehicleIsFiring)
			LaunchRocket(false);
		else if(bVehicleIsAltFiring)
			LaunchRocket(true);
	}

	// Dont bother doing effects on dedicated server.
	if(Level.NetMode != NM_DedicatedServer)
	{
		// Update brake light colors.
		if(Gear == -1 && OutputBrake == false)
		{
			// REVERSE
			Skins[1] = ReverseMaterial;
		}
		else if(Gear == 0 && OutputBrake == true)
		{
			// BRAKE
			Skins[1] = BrakeMaterial;
		}
		else
		{
			// OFF
			Skins[1] = TailOffMaterial;
		}

		// Set headlight projector state.
		Headlight.DetachProjector();
		if(HeadlightOn)
		{
			Headlight.AttachProjector();
			Skins[2] = HeadlightOnMaterial;
			for(i=0; i<8; i++)
				HeadlightCorona[i].bHidden = false;
		}
		else
		{
			Skins[2] = HeadlightOffMaterial;
			for(i=0; i<8; i++)
				HeadlightCorona[i].bHidden = true;
		}

		// Update dust kicked up by wheels.
		Dust[0].UpdateDust(frontLeft, DustSlipRate, DustSlipThresh);
		Dust[1].UpdateDust(frontRight, DustSlipRate, DustSlipThresh);
		Dust[2].UpdateDust(rearLeft, DustSlipRate, DustSlipThresh);
		Dust[3].UpdateDust(rearRight, DustSlipRate, DustSlipThresh);
	}

    UntilNextImpact -= Delta;

	// This assume the sound is an idle-ing sound, and increases with pitch
	// as wheels speed up.
	EnginePitch = 64 + ((WheelSpinSpeed/EnginePitchScale) * (255-64));
    SoundPitch = FClamp(EnginePitch, 0, 254);
	//Log("Engine Pitch:"$SoundPitch);
    //SoundVolume = 255;

    // Currently, just use rear wheels for slipping noise
    TotalSlip = rearLeft.GroundSlipVel + rearRight.GroundSlipVel;
    //log("TotalSlip:"$TotalSlip);
	if(TotalSlip > SquealVelThresh)
	{
		rearLeft.AmbientSound = BulldogSquealSound;
	}
	else
	{
		rearLeft.AmbientSound = None;
	}

	// Dont have triggers on network clients.
	if(Level.NetMode != NM_Client)
	{
		// If vehicle is moving, disable collision for trigger.
		VMag = VSize(Velocity);

		if(!bIsInverted && VMag < TriggerSpeedThresh && TriggerState == false)
		{
			FLTrigger.SetCollision(true, false, false);
			FRTrigger.SetCollision(true, false, false);
			TriggerState = true;
		}
		else if((bIsInverted || VMag > TriggerSpeedThresh) && TriggerState == true)
		{
			FLTrigger.SetCollision(false, false, false);
			FRTrigger.SetCollision(false, false, false);
			TriggerState = false;
		}

		// If vehicle is inverted, and slow, turn on big 'flip vehicle' trigger.
		if(bIsInverted && VMag < TriggerSpeedThresh && FlipTriggerState == false)
		{
			FlipTrigger.SetCollision(true, false, false);
			FlipTriggerState = true;	
		}
		else if((!bIsInverted || VMag > TriggerSpeedThresh) && FlipTriggerState == true)
		{
			FlipTrigger.SetCollision(false, false, false);
			FlipTriggerState = false;
		}
	}
}

// Really simple at the moment!
function TakeDamage(int Damage, Pawn instigatedBy, Vector hitlocation, 
						Vector momentum, class<DamageType> damageType)
{
	// Avoid damage healing the car!
	if(Damage < 0)
		return;

	if(damageType == class'DamTypeSuperShockBeam')
		Health -= 100; // Instagib doesn't work on vehicles
	else
		Health -= 0.5 * Damage; // Weapons do less damage

	// The vehicle is dead!
	if(Health <= 0)
	{
		if ( Controller != None )
		{
			if( Controller.bIsPlayer )
			{
				ClientKDriverLeave(PlayerController(Controller)); // Just to reset HUD etc.
				Controller.PawnDied(self); // This should unpossess the controller and let the player respawn
			}
			else
				Controller.Destroy();
		}

		Destroy(); // Destroy the vehicle itself (see Destroyed below)
	}

    //KAddImpulse(momentum, hitlocation);
}

defaultproperties
{
	DrawType=DT_StaticMesh
	StaticMesh=StaticMesh'BulldogMeshes.Simple.S_Chassis'

    Begin Object Class=KarmaParamsRBFull Name=KParams0
        KInertiaTensor(0)=20
        KInertiaTensor(1)=0
        KInertiaTensor(2)=0
        KInertiaTensor(3)=30
        KInertiaTensor(4)=0
        KInertiaTensor(5)=48
		KCOMOffset=(X=0.8,Y=0.0,Z=-0.7)
		KStartEnabled=True
		KFriction=1.6
		KLinearDamping=0
		KAngularDamping=0
		bKNonSphericalInertia=False
        bHighDetailOnly=False
        bClientOnly=False
		bKDoubleTickRate=True
        Name="KParams0"
    End Object
    KParams=KarmaParams'KParams0'

	DrawScale=0.4

	FrontTireClass=class'BulldogTire'
	RearTireClass=class'BulldogTire'

	WheelFrontAlong=-100
	WheelFrontAcross=110
	WheelRearAlong=115
	WheelRearAcross=110
	WheelVert=-15

	DestroyedEffect=class'XEffects.RocketExplosion'
	DestroyedSound=sound'WeaponSounds.P1RocketLauncherAltFire'


	//FrontTriggerOffset=(X=174,Y=0,Z=20)
	FrontTriggerOffset=(X=0,Y=165,Z=10)

	TriggerSpeedThresh=40

	HeadlightOffset=(X=-200,Y=0,Z=50)
	HeadlightOn=false

	// Light materials
	ReverseMaterial=Material'VehicleFX.BDReverseLight_ON'
	BrakeMaterial=Material'VehicleFX.BDTaiLight_ON'
	TailOffMaterial=Material'VehicleFX.BDTailOff'

	HeadlightOnMaterial=Material'VehicleFX.BDHeadlights_ON'
	HeadlightOffMaterial=Material'VehicleFX.BDHeadlights'

	HeadlightCoronaOffset(0)=(X=-199,Y=51,Z=57)
	HeadlightCoronaOffset(1)=(X=-199,Y=-51,Z=57)
	HeadlightCoronaOffset(2)=(X=-128,Y=38,Z=125)
	HeadlightCoronaOffset(3)=(X=-189,Y=93,Z=28)
	HeadlightCoronaOffset(4)=(X=-183,Y=-93,Z=26)
	HeadlightCoronaOffset(5)=(X=-190,Y=-51,Z=77)
	HeadlightCoronaOffset(6)=(X=-128,Y=63,Z=123)
	HeadlightCoronaOffset(7)=(X=-185,Y=85,Z=10)

	// Sounds
	EnginePitchScale=655350
	HitSoundThreshold=30
	SquealVelThresh=15

	DustSlipRate=2.8
	DustSlipThresh=0.1
	DustDrop=30

	// Weaponary
	bSpecialHUD=true
	FireInterval=0.9
	TargettingInterval=0.5
	RocketFireOffset=(X=0,Y=0,Z=180)
	TargettingRange=5000
    TargetMaterial=Material'InterfaceContent.Hud.fbBombFocus'
	

	// Driver positions
	
	ExitPositions(0)=(X=0,Y=200,Z=100)
	ExitPositions(1)=(X=0,Y=-200,Z=100)
	ExitPositions(2)=(X=350,Y=0,Z=100)
	ExitPositions(3)=(X=-350,Y=0,Z=100)
	
	DrivePos=(X=-165,Y=0,Z=-100)

    OutputBrake=1
    Steering=0
    Throttle=0
    Gear=1
	StopThreshold=40

    MaxSteerAngle=3400
    MaxBrakeTorque=55

	TorqueCurve=(Points=((InVal=0,OutVal=270),(InVal=200000,OutVal=270),(InVal=300000,OutVal=0)))
    TorqueSplit=1.0
    SteerPropGap=500
    SteerTorque=15000
    SteerSpeed=20000

    TireMass=0.5    
    ChassisMass=8

    TireRollFriction=1.5
    TireLateralFriction=1.5
    TireRollSlip=0.06
    TireLateralSlip=0.04
    TireMinSlip=0.001
    TireSlipRate=0.007
    TireSoftness=0.0
    TireAdhesion=0
    TireRestitution=0

	HandbrakeThresh=1000
	TireHandbrakeSlip=0.2
	TireHandbrakeFriction=-1.2

    SuspStiffness=150
    SuspDamping=14.5
    SuspHighLimit=0.5
    SuspLowLimit=-0.5
    SuspRef=0

	AirSpeed = 5000

	Health=800

    // sound
    SoundRadius=255
    
    UntilNextImpact=500
}