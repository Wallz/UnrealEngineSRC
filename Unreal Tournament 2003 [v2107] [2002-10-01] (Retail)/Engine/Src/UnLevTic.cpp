/*=============================================================================
	UnLevTic.cpp: Level timer tick function
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

#include "EnginePrivate.h"
#include "UnNet.h"
#include "UnPath.h"
/*-----------------------------------------------------------------------------
	Helper classes.
-----------------------------------------------------------------------------*/

//
// Priority sortable list.
//
struct FActorPriority
{
	INT			    Priority;	// Update priority, higher = more important.
	AActor*			Actor;		// Actor.
	UActorChannel*	Channel;	// Actor channel.
	FActorPriority()
	{}
	FActorPriority( FVector& ViewPos, FVector& ViewDir, UNetConnection* InConnection, AActor* InActor )
	{
		guard(FActorPriority::FActorPriority);
		Actor       = InActor;
		Channel     = InConnection->ActorChannels.FindRef(Actor);
		FLOAT Time  = Channel ? (InConnection->Driver->Time - Channel->LastUpdateTime) : InConnection->Driver->SpawnPrioritySeconds;
		
		FLOAT Dot = 0.f;
		if ( Actor->bAlwaysRelevant )
			Dot = 0.f;
		else if ( InConnection->Actor->Pawn && Actor->IsOwnedBy(InConnection->Actor->Pawn) )
			Dot = 1.f;
		else if ( (Actor->RemoteRole > ROLE_DumbProxy) || (Actor->Physics == PHYS_None) )
		{
			FVector Dir = Actor->Location - ViewPos;
			Dot   = ViewDir | Dir;
			if ( Dot < 0.f )
				Dot = -1.f;
			else
				Dot = 1.f;
		}
		
		Priority    = appRound(65536.0f * (3.0f+Dot) * Actor->GetNetPriority( (Channel && Channel->Recent.Num()) ? (AActor*)&Channel->Recent(0) : NULL, Time, InConnection->BestLag ));
		unguard;
	}
	friend INT Compare( const FActorPriority* A, const FActorPriority* B )
	{
		return B->Priority - A->Priority;
	}
};

/*-----------------------------------------------------------------------------
	Tick a single actor.
-----------------------------------------------------------------------------*/

UBOOL AActor::CheckOwnerUpdated()
{
	guardSlow(AActor::CheckOwnerUpdated);
	if( Owner && (INT)Owner->bTicked!=GetLevel()->Ticked )
	{
		GetLevel()->NewlySpawned = new(GEngineMem)FActorLink(this,GetLevel()->NewlySpawned);
		return 0;
	}
	return 1;
	unguardSlow;
}

UBOOL APawn::CheckOwnerUpdated()
{
	guardSlow(APawn::CheckOwnerUpdated);
	if( Owner && (INT)Owner->bTicked!=GetLevel()->Ticked )
	{
		GetLevel()->NewlySpawned = new(GEngineMem)FActorLink(this,GetLevel()->NewlySpawned);
		return 0;
	}
	// Handle controller-first updating.
	if( Controller && (INT)Controller->bTicked!=GetLevel()->Ticked )
	{
		GetLevel()->NewlySpawned = new(GEngineMem)FActorLink(this,GetLevel()->NewlySpawned);
		return 0;
	}
	return 1;
	unguardSlow;
}

void ALineOfSightTrigger::TickAuthoritative( FLOAT DeltaSeconds )
{
	guardSlow(ALineOfSightTrigger::TickAuthoritative);

	AActor::TickAuthoritative(DeltaSeconds);

	if ( bEnabled && !bTriggered && SeenActor
		&& (SeenActor->LastRenderTime >= OldTickTime) )
	{
		// find player pawn, and check if close enough
		for ( AController *C=Level->ControllerList; C; C=C->nextController )
		{
			if ( C->LocalPlayerController() )
			{
			    APlayerController *PC = C->GetAPlayerController();//Cast<APlayerController>(C);
			    if ( (PC->GetViewTarget()->Location - Location).SizeSquared() < MaxViewDist*MaxViewDist )
			    {
				    // check if looking directly enough at target
				    FVector Dir = (SeenActor->Location - PC->ViewTarget->Location).SafeNormal();
				    if ( (PC->Rotation.Vector() | Dir) > RequiredViewDir )
				    {
					    FCheckResult Hit(1.f);
					    GetLevel()->SingleLineCheck( Hit, this, SeenActor->Location, PC->ViewTarget->Location, TRACE_World|TRACE_StopAtFirstHit );
					    if ( !Hit.Actor )
						    eventPlayerSeesMe(PC);
				    }
				    break;
			    }
		    }
	    }
    }
	OldTickTime = Level->TimeSeconds;

	unguardSlow;
}

void AWeapon::TickAuthoritative( FLOAT DeltaSeconds )
{
	guardSlow(AWeapon::TickAuthoritative);

    AActor::TickAuthoritative( DeltaSeconds );

    if( bDeleteMe )
        return;

    if ( Instigator && Instigator->Weapon == this && !Instigator->bDeleteMe && Instigator->Controller != NULL && !Instigator->Controller->bDeleteMe 
		&& !Instigator->bNoWeaponFiring )
    {
        eventWeaponTick(DeltaSeconds);
        // client & server: update mode timing
        for (int mode = 0; mode < 2; mode++)
        {
            if (FireMode[mode] != NULL)
            {
                FireMode[mode]->ModeTick( DeltaSeconds );

                if (!Instigator || !Instigator->Controller) // pawn was killed during mode tick (ie: shieldgun against the wall)
                    return;

                if (Role == ROLE_Authority)
                {
                    if (FireMode[mode]->bServerDelayStartFire)
                    {
                        if (FireMode[mode]->NextFireTime <= Level->TimeSeconds + FireMode[mode]->PreFireTime)
                            eventServerStartFire(mode);
                    }
                    else if (FireMode[mode]->bServerDelayStopFire)
                    {
                        FireMode[mode]->bServerDelayStopFire = false;
                        //debugf(TEXT("ServerDelayStopFire %f"), Level->TimeSeconds);
                        eventStopFire(mode);
                    }
                }
            }
        }

        // client side only: determine when firing starts and stops
        if (ClientState == WS_ReadyToFire && Instigator->IsLocallyControlled())
        {
            UBOOL bAltFire = Instigator->Controller->bAltFire;
            UBOOL bFire = Instigator->Controller->bFire;

            if (ExchangeFireModes)
            {
                Exchange(bFire,bAltFire);
            }

            if (FireMode[0] != NULL)
            {
                if (FireMode[0]->bIsFiring && !bFire)
                {
                    eventClientStopFire(0);
                }
                else if (!FireMode[0]->bIsFiring && bFire)
                {
                    eventClientStartFire(0);
                }
            }

            if (FireMode[1] != NULL)
            {
                if (FireMode[1]->bIsFiring && !bAltFire)
                {
                    eventClientStopFire(1);
                }
                else if (!FireMode[1]->bIsFiring && bAltFire)
                {                                       
                    eventClientStartFire(1);
                }
            }
        }
    }

    unguardSlow;
}

void AWeaponFire::ModeTick( FLOAT DeltaSeconds )
{
    eventModeTick(DeltaSeconds);

    if ( (bIsFiring && !bFireOnRelease)
        || (!bIsFiring && bFireOnRelease)
        || (HoldTime > MaxHoldTime && MaxHoldTime > 0.0f) )
    {
        if (Level->TimeSeconds > NextFireTime && !bNowWaiting)
        {
           eventModeDoFire();

            if (bWaitForRelease)
                bNowWaiting = true;
        }
    }
    else
    {
        if (bWaitForRelease && Level->TimeSeconds >= NextFireTime)
        {
            bNowWaiting = false;

            if (HoldTime == 0.0f)
                eventModeHoldFire();

            HoldTime += DeltaSeconds;
        }
    }
}

void AActor::TickAuthoritative( FLOAT DeltaSeconds )
{
	guardSlow(AActor::TickAuthoritative);

	// Tick the nonplayer.
	//clockSlow(GStats.DWORDStats(GEngineStats.STATS_Game_ScriptTickCycles));
	eventTick(DeltaSeconds);
	//unclockSlow(GStats.DWORDStats(GEngineStats.STATS_Game_ScriptTickCycles));

	// Update the actor's script state code.
	ProcessState( DeltaSeconds );

	UpdateTimers(DeltaSeconds );

	// Update LifeSpan.
	if( LifeSpan!=0.f )
	{
		LifeSpan -= DeltaSeconds;
        if( bAttenByLife ) // sjs - attenuate light by lifespan
        {
            LightBrightness = (BYTE)((float)LightBrightness*(LifeSpan / GetClass()->GetDefaultActor()->LifeSpan));
            if( LifeSpan < 1.0f )
                ScaleGlow = LifeSpan;
        }
		if( LifeSpan <= 0.0001f )
		{
			// Actor's LifeSpan expired.
			GetLevel()->DestroyActor( this );
			return;
		}
	}

	// Perform physics.
	if ( !bDeleteMe && (Physics!=PHYS_None) && (Role!=ROLE_AutonomousProxy) )
		performPhysics( DeltaSeconds );
	unguardSlow;
}

void AActor::TickSimulated( FLOAT DeltaSeconds )
{
	guardSlow(AActor::TickSimulated);

	TickAuthoritative(DeltaSeconds);
	unguardSlow;
}

void APawn::TickSimulated( FLOAT DeltaSeconds )
{
	guardSlow(APawn::TickSimulated);

	// Simulated Physics for pawns
	// simulate gravity
	Acceleration = Velocity.SafeNormal();
	if ( PhysicsVolume->bWaterVolume )
		Physics = PHYS_Swimming;
	else if ( bCanClimbLadders && PhysicsVolume->IsA(ALadderVolume::StaticClass()) )
		Physics = PHYS_Ladder;
	else if ( bSimulateGravity )
		Physics = PHYS_Walking;	// set physics mode guess for use by animation
	else 
		Physics = PHYS_Flying;

	//simulated pawns just predict location, no script execution
	moveSmooth(Velocity * DeltaSeconds);

	// if simulated gravity, check if falling
	if ( bSimulateGravity && !bSimGravityDisabled )
	{
		FCheckResult Hit(1.f);
		if ( Velocity.Z == 0.f )
			GetLevel()->SingleLineCheck(Hit, this, Location - FVector(0.f,0.f,1.5f * CollisionHeight), Location, TRACE_AllBlocking, FVector(CollisionRadius,CollisionRadius,4.f));
		else if ( Velocity.Z < 0.f )
			GetLevel()->SingleLineCheck(Hit, this, Location - FVector(0.f,0.f,8.f), Location, TRACE_AllBlocking, GetCylinderExtent());

		if ( (Hit.Time == 1.f) || (Hit.Normal.Z < UCONST_MINFLOORZ) )
		{
			if ( Velocity.Z == 0.f )
				Velocity.Z = 0.15f * PhysicsVolume->Gravity.Z;
			Velocity += 0.5f * PhysicsVolume->Gravity * DeltaSeconds;
			Physics = PHYS_Falling; 
		}
		else
			Velocity.Z = 0.f;
	}

	// Tick the nonplayer.
	//clockSlow(GStats.DWORDStats(GEngineStats.STATS_Game_ScriptTickCycles));
	eventTick(DeltaSeconds);
	//unclockSlow(GStats.DWORDStats(GEngineStats.STATS_Game_ScriptTickCycles));

	unguardSlow;
}

void AActor::TickSpecial( FLOAT DeltaSeconds )
{
}

void APawn::TickSpecial( FLOAT DeltaSeconds )
{
	guardSlow(APawn::TickSpecial);

	if ( !bInterpolating && bPhysicsAnimUpdate && Mesh )
		UpdateMovementAnimation(DeltaSeconds);

	// update weapon location (in case its playing sounds, etc.)
	// bAttachedMove = true (last switch in FarMoveActor) for weapon.
	if ( Weapon && !Weapon->bDeleteMe ) // gam
	{
		if ( !IsLocallyControlled() || !IsHumanControlled() 
			|| (Controller->GetViewTarget() != this) || ((APlayerController *)Controller)->bBehindView )
			GetLevel()->FarMoveActor( Weapon, Location, 0, 0, 1 ); 
	}
	// update eyeheight if someone is viewing through this pawn's eyes
	if ( bUpdateEyeheight || IsHumanControlled() )
		eventUpdateEyeHeight(DeltaSeconds);

	if( (Role==ROLE_Authority) && (BreathTime > 0.f) )
	{
		BreathTime -= DeltaSeconds;
		if (BreathTime < 0.001f)
		{
			BreathTime = 0.0f;
			eventBreathTimer();
		}
	}
	unguardSlow;
}

UBOOL AActor::PlayerControlled()
{
	return 0;
}

UBOOL APawn::PlayerControlled()
{
	return ( IsLocallyControlled() );
}

// sjs ---
void AActor::UpdateOverlay(FLOAT DeltaSeconds)
{
    if( OverlayMaterial )
    {
        if( Role < ROLE_Authority && ClientOverlayTimer != OverlayTimer )
        {
            ClientOverlayTimer = OverlayTimer;
            ClientOverlayCounter = ClientOverlayTimer;
        }
        float* value = NULL;
        if( Role < ROLE_Authority && ClientOverlayCounter != 0.0 )
        {
            value = &ClientOverlayCounter;
        }
        else if ( OverlayTimer != 0.0 )
        {
            value = &OverlayTimer;
        }
        if( value )
        {
            if( *value > 0.0f )
            {
                *value -= DeltaSeconds;
                if( *value <= 0.0f ) // count down
                    *value = 0.0f;
            }
            else
            {
                *value += DeltaSeconds;
                if( *value >= 0.0f ) // count up
                    *value = 0.0f;
            }
            if( *value == 0.0 )
                OverlayMaterial = NULL;
        }
    }
}
// --- sjs

UBOOL AActor::Tick( FLOAT DeltaSeconds, ELevelTick TickType )
{
	guard(AActor::Tick);

	// Ignore actors in stasis
	if
	(	bStasis 
	&&	((Physics==PHYS_None) || (Physics == PHYS_Rotating))
	&&	(GetLevel()->TimeSeconds - LastRenderTime > 5)
	&&	(Level->NetMode == NM_Standalone) )
	{
		bTicked = GetLevel()->Ticked;
		return 1;
	}

	// Handle owner-first updating.
	if ( !CheckOwnerUpdated() )
		return 0;
	bTicked = GetLevel()->Ticked;

	// Non-player update.
	if( (TickType==LEVELTICK_ViewportsOnly) && !PlayerControlled() )
		return 1;

    UpdateOverlay(DeltaSeconds); //sjs

	// Update all animation, including multiple passes if necessary.
	// Meshes are allowed autonomous updating.	
	UpdateAnimation(DeltaSeconds);


	// This actor is tickable.
	if( RemoteRole == ROLE_AutonomousProxy ) 
	{
        APlayerController *PC = GetTopPlayerController();//Cast<APlayerController>(GetTopOwner());
		if ( (PC && PC->LocalPlayerController()) || Physics == PHYS_Karma )
			TickAuthoritative(DeltaSeconds);
		else
		{
			eventTick(DeltaSeconds);

			// Update the actor's script state code.
			ProcessState( DeltaSeconds );
			// Server handles timers for autonomous proxy.
			UpdateTimers( DeltaSeconds );

			if ( !bDeleteMe && (Physics!=PHYS_None) && PC && (Level->TimeSeconds - PC->ServerTimeStamp > 0.5f * PC->MaxResponseTime) 
				&& (PC->ServerTimeStamp != 0.f) )
			{
				// force position update
				if ( !Velocity.IsZero() )
				{
					performPhysics( Level->TimeSeconds - PC->ServerTimeStamp );
				}
				PC->ServerTimeStamp = Level->TimeSeconds;
			}
		}
	}
	else if ( Role>ROLE_SimulatedProxy )
		TickAuthoritative(DeltaSeconds);
	else if ( Role == ROLE_SimulatedProxy )
		TickSimulated(DeltaSeconds);
	else if ( !bDeleteMe && ((Physics == PHYS_Falling) || (Physics == PHYS_Rotating)) ) // dumbproxies simulate falling if client side physics set
		performPhysics( DeltaSeconds );

	if ( bDeleteMe )
		return 1;

	TickSpecial(DeltaSeconds);	// perform any tick functions unique to an actor subclass
	return 1;
	unguard;
}


/* Controller Tick
Controllers are never animated, and do not look for an owner to be ticked before them
Non-player controllers don't support being an autonomous proxy
*/
UBOOL AController::Tick( FLOAT DeltaSeconds, ELevelTick TickType )
{
	guard(AController::Tick);

	// Ignore controllers with no pawn, or pawn is in stasis
	if
	(	bStasis
	|| (TickType==LEVELTICK_ViewportsOnly) 
	|| (Pawn && Pawn->bStasis 
	&&	((Pawn->Physics==PHYS_None) || (Pawn->Physics == PHYS_Rotating))
	&&	(GetLevel()->TimeSeconds - Pawn->LastRenderTime > 5)
	&&	(Level->NetMode == NM_Standalone)) )
	{
		bTicked = GetLevel()->Ticked;
		return 1;
	}

	bTicked = GetLevel()->Ticked;

	if( Role>=ROLE_SimulatedProxy )
		TickAuthoritative(DeltaSeconds);
	
	// Update eyeheight and send visibility updates
	// with PVS, monsters look for other monsters, rather than sending msgs

	if( Role==ROLE_Authority && TickType==LEVELTICK_All )
	{
		if( SightCounter < 0.0f )
		{
			if( IsProbing(NAME_EnemyNotVisible) )
			{
				CheckEnemyVisible();
				SightCounter = 0.05f + 0.1f * appFrand();
			}
			else
				SightCounter += 0.15f + 0.1f * appFrand();
		}

		SightCounter = SightCounter - DeltaSeconds; 
		// for best performance, players show themselves to players and non-players (e.g. monsters),
		// and monsters show themselves to players
		// but monsters don't show themselves to each other
		// also

		if( Pawn && !Pawn->bHidden && !Pawn->bAmbientCreature )
			ShowSelf();
	}

	if ( Pawn )
	{
		// rotate pawn toward focus
		if ( Pawn->bRotateToDesired )
			Pawn->rotateToward(Focus, FocalPoint);

		// face same direction as pawn
		Rotation = Pawn->Rotation;
	}
	if ( MonitoredPawn )
	{
		if ( !Pawn || MonitoredPawn->bDeleteMe || !MonitoredPawn->Controller )
			eventMonitoredPawnAlert();
		else
		{
			// quit if further than max dist, moving away fast, or has moved far enough
			if ( ((MonitoredPawn->Location - Pawn->Location).SizeSquared() > MonitorMaxDistSq)
				|| ((MonitoredPawn->Location - MonitorStartLoc).SizeSquared() > 0.25f * MonitorMaxDistSq) )
				eventMonitoredPawnAlert();
			else if ( (MonitoredPawn->Velocity.SizeSquared() > 0.6f * MonitoredPawn->GroundSpeed)
				&& ((MonitoredPawn->Velocity | (MonitorStartLoc - Pawn->Location)) > 0.f)
				&& ((MonitoredPawn->Location - Pawn->Location).SizeSquared() > 0.25f * MonitorMaxDistSq) )
				eventMonitoredPawnAlert();
		}
	}
	return 1;
	unguard;
}



/* 
PlayerControllers
Controllers are never animated, and do not look for an owner to be ticked before them
*/
UBOOL APlayerController::Tick( FLOAT DeltaSeconds, ELevelTick TickType )
{
	guard(APlayerController::Tick);

	bTicked = GetLevel()->Ticked;

	TimeSinceLastFogChange += DeltaSeconds;

	GetViewTarget();
	if( (RemoteRole == ROLE_AutonomousProxy) && !LocalPlayerController() ) 
	{
		// update viewtarget replicated info
		if( ViewTarget != Pawn )
		{
            APawn* TargetPawn = ViewTarget ? ViewTarget->GetAPawn() : NULL; //Cast<APawn>(ViewTarget);
			if ( TargetPawn )
			{
				TargetViewRotation = Rotation;
				TargetEyeHeight = TargetPawn->EyeHeight;
				if ( TargetPawn->Weapon )
					TargetWeaponViewOffset = TargetPawn->Weapon->PlayerViewOffset;
			}
		}

		// Update the actor's script state code.
		ProcessState( DeltaSeconds );
		// Server handles timers for autonomous proxy.
		UpdateTimers( DeltaSeconds );
	}
	else if( Role>=ROLE_SimulatedProxy )
	{
		// Player update.
		if( IsA(ACamera::StaticClass()) && !(ShowFlags & SHOW_PlayerCtrl) )
			return 1;

		// Process PlayerTick with input.
		if ( Player )
		{
			if ( !PlayerInput )
				eventInitInputSystem();
			if ( PlayerInput )
			{
				Player->ReadInput( DeltaSeconds );
				eventPlayerTick( DeltaSeconds );
				Player->ReadInput( -1.0f );
			}
		}

	/*		if( GetLevel()->DemoRecDriver && !GetLevel()->DemoRecDriver->ServerConnection )
		{
			PlayerController->DemoViewPitch = PlayerController->Rotation.Pitch;
			PlayerController->DemoViewYaw = PlayerController->Rotation.Yaw;
		} FIXME */

		// Update the actor's script state code.
		ProcessState( DeltaSeconds );

		UpdateTimers( DeltaSeconds );

		if ( bDeleteMe )
			return 1;

		// Perform physics.
		if( Physics!=PHYS_None && Role!=ROLE_AutonomousProxy )
			performPhysics( DeltaSeconds );
	}

	// During demo playback, setup view offsets for viewtarget
	if( GetLevel()->DemoRecDriver && GetLevel()->DemoRecDriver->ServerConnection )
	{
		if( Role == ROLE_Authority )
		{
			// update viewtarget replicated info
			if( !bBehindView )
			{
				APawn* TargetPawn = ViewTarget ? ViewTarget->GetAPawn() : NULL; //Cast<APawn>(ViewTarget);
				if ( TargetPawn )
				{
					TargetViewRotation = Rotation;
					TargetEyeHeight = TargetPawn->EyeHeight;
					if ( TargetPawn->Weapon )
						TargetWeaponViewOffset = TargetPawn->Weapon->PlayerViewOffset;
				}
			}
		}
	}

	// Update eyeheight and send visibility updates
	// with PVS, monsters look for other monsters, rather than sending msgs
	if( Role==ROLE_Authority && TickType==LEVELTICK_All )
	{
		if( SightCounter < 0.0f )
			SightCounter += 0.2f;

		SightCounter = SightCounter - DeltaSeconds; 
		if( Pawn && !Pawn->bHidden )
			ShowSelf();
	}

	return 1;
	unguard;
}

void AActor::UpdateTimers(FLOAT DeltaSeconds)
{
	guardSlow(AActor::UpdateTimers);

	// Update timers.
	if( TimerRate>0.0f && (TimerCounter+=DeltaSeconds)>=TimerRate )
	{
		// Normalize the timer count.
		INT TimerTicksPassed = 1;
		if( TimerRate > 0.0f )
		{
			TimerTicksPassed     = (int)(TimerCounter/TimerRate);
			TimerCounter -= TimerRate * TimerTicksPassed;
			if( TimerTicksPassed && !bTimerLoop )
			{
				// Only want a one-shot timer message.
				TimerTicksPassed = 1;
				TimerRate = 0.0f;
			}
		}

		// Call timer routine with count of timer events that have passed.
		eventTimer();
	}

	unguardSlow;
}

/*-----------------------------------------------------------------------------
	Network client tick.
-----------------------------------------------------------------------------*/

void ULevel::TickNetClient( FLOAT DeltaSeconds )
{
	guard(ULevel::TickNetClient);
	clock(GStats.DWORDStats(GEngineStats.STATS_Game_NetTickCycles));
	if( NetDriver->ServerConnection->State==USOCK_Open )
	{
		/* Don't replicate any properties from client to server
		for( TMap<AActor*,UActorChannel*>::TIterator ItC(NetDriver->ServerConnection->ActorChannels); ItC; ++ItC )
		{
			guard(UpdateLocalActors);
			UActorChannel* It = ItC.Value();
			APawn* PlayerPawn = It->GetActor() ? It->GetActor()->GetAPawn() : NULL; // Cast<APawn>(It->GetActor());
			if( PlayerPawn && PlayerPawn->Controller )
			{
				APlayerController* PC = PlayerPawn->Controller->GetAPlayerController();// Cast<APlayerController>(PlayerPawn->Controller);
				if ( PC && PC->Player )
					It->ReplicateActor();
			}
			unguard;
		}
		*/
	}
	else if( NetDriver->ServerConnection->State==USOCK_Closed )
	{
		// Server disconnected.

		// Determine if it's a map change or disconnect

		UGameEngine* GE = Cast<UGameEngine>(Engine);

		if (!(GE && GE->GPendingLevel))
			GE->SetProgress( *FString::Printf(TEXT("menu:%s"),*GE->DisconnectMenuClass), LocalizeError(TEXT("ConnectLost"),TEXT("Engine")), TEXT("") );			

		check(Engine->Client->Viewports.Num());
		Engine->SetClientTravel( Engine->Client->Viewports(0), TEXT("?closed"), 0, TRAVEL_Absolute );
	}
	unclock(GStats.DWORDStats(GEngineStats.STATS_Game_NetTickCycles));
	unguard;
}

/*-----------------------------------------------------------------------------
	Network server ticking individual client.
-----------------------------------------------------------------------------*/
UBOOL AActor::IsNetRelevantFor( APlayerController* RealViewer, AActor* Viewer, FVector SrcLocation )
{
	guard(AActor::IsNetRelevantFor);
	if( bAlwaysRelevant || IsOwnedBy(Viewer) || IsOwnedBy(RealViewer) || this==Viewer || Viewer==Instigator )
		return 1;
	else if( AmbientSound 
			&& ((Location-Viewer->Location).SizeSquared() < Square(GAudioMaxRadiusMultiplier*SoundRadius)) )
		return 1;
	else if ( (Owner && (Base == Owner) && !bOnlyOwnerSee)
			|| (Base && (AttachmentBone != NAME_None)) )
		return Base->IsNetRelevantFor( RealViewer, Viewer, SrcLocation );
	else if( (bHidden || bOnlyOwnerSee) && !bBlockPlayers && !AmbientSound )
		return 0;
	else
	{
		// check distance fog
		if ( Region.Zone->bFogZone && ((Location - SrcLocation).SizeSquared() > Region.Zone->DistanceFogEnd * Region.Zone->DistanceFogEnd) )
			return 0;
		if ( GetLevel()->Model->FastLineCheck(Location,SrcLocation) )
		{
			/*
			// check against terrain
			// FIXME - Jack, it would be nice to make this faster (a linecheck that just returns whether there was a hit)
			AZoneInfo* Z = Viewer->Region.Zone;
			if( Z && Z->bTerrainZone ) 
			{
				FVector End = Location;
				End.Z += 2.f * CollisionHeight;
				FCheckResult Hit(1.f);
				for( INT t=0;t<Z->Terrains.Num();t++ )
				{
					if( Z->Terrains(t)->LineCheck( Hit, End, SrcLocation, FVector(0.f,0.f,0.f) )==0 )
					{
						FPointRegion HitRegion = GetLevel()->Model->PointRegion( GetLevel()->GetLevelInfo(), Hit.Location );
						if( HitRegion.Zone == Z )
							return 0;
					}
				}
			} */
			/*
			// check against antiportal volumes
			FCheckResult Hit(1.f);
			for ( INT i=0; i<GetLevel()->AntiPortals.Num(); i++ )
				if ( GetLevel()->AntiPortals(i) && GetLevel()->AntiPortals(i)->GetPrimitive()->LineCheck( Hit, GetLevel()->AntiPortals(i), Location, SrcLocation, FVector(0.f,0.f,0.f), 0, TRACE_StopAtFirstHit )==0 )
					return 0;
			*/
			return 1;
		}
		return 0;
	}
	unguard;
}

UBOOL APlayerController::IsNetRelevantFor( APlayerController* RealViewer, AActor* Viewer, FVector SrcLocation )
{
	guardSlow(APlayerController::IsNetRelevantFor);

	return ( this==RealViewer );
	unguardSlow;
}

UBOOL APawn::CacheNetRelevancy(UBOOL bIsRelevant, APlayerController* RealViewer, AActor* Viewer)
{
	guardSlow(APawn::CacheNetRelevancy);

	bCachedRelevant = bIsRelevant;
	NetRelevancyTime = Level->TimeSeconds;
	LastRealViewer = RealViewer;
	LastViewer = Viewer;
	return bIsRelevant;
	unguardSlow;
}

UBOOL APawn::IsNetRelevantFor( APlayerController* RealViewer, AActor* Viewer, FVector SrcLocation )
{
	guardSlow(APawn::IsNetRelevantFor);

	if ( (NetRelevancyTime == Level->TimeSeconds) && (RealViewer == LastRealViewer) && (Viewer == LastViewer) )
		return bCachedRelevant;
	if( bAlwaysRelevant || IsOwnedBy(Viewer) || IsOwnedBy(RealViewer) || this==Viewer || Viewer==Instigator )
		return CacheNetRelevancy(true,RealViewer,Viewer);
	if( AmbientSound 
			&& ((Location-Viewer->Location).SizeSquared() < Square(GAudioMaxRadiusMultiplier*SoundRadius)) )
		return CacheNetRelevancy(true,RealViewer,Viewer);
	else if( (bHidden || bOnlyOwnerSee) && !bBlockPlayers && !AmbientSound )
		return CacheNetRelevancy(false,RealViewer,Viewer);
	else
	{
		// check distance fog
		if ( Region.Zone->bFogZone && ((Location - SrcLocation).SizeSquared() > Region.Zone->DistanceFogEnd * Region.Zone->DistanceFogEnd) )
			return CacheNetRelevancy(false,RealViewer,Viewer);

		// check against BSP - check head and center
		//debugf(TEXT("Check relevance of %s"),*(PlayerReplicationInfo->PlayerName));
		if ( !GetLevel()->Model->FastLineCheck(Location + FVector(0.f,0.f,CollisionHeight),SrcLocation) 
			 && !GetLevel()->Model->FastLineCheck(Location,SrcLocation)  )
			return CacheNetRelevancy(false,RealViewer,Viewer);
		/*
		if ( CollisionRadius <= COMMONRADIUS )
			return 0;

		// if large collision volume, check edges
		if ( GetLevel()->Model->FastLineCheck(Location + FVector(0.f,CollisionRadius,0.f),SrcLocation) )
			return CacheNetRelevancy(true,RealViewer,Viewer);
		if ( GetLevel()->Model->FastLineCheck(Location - FVector(0.f,CollisionRadius,0.f),SrcLocation) )
			return CacheNetRelevancy(true,RealViewer,Viewer);
		if ( GetLevel()->Model->FastLineCheck(Location + FVector(CollisionRadius,0.f,0.f),SrcLocation) )
			return CacheNetRelevancy(true,RealViewer,Viewer);
		if ( GetLevel()->Model->FastLineCheck(Location - FVector(CollisionRadius,0.f,0.f),SrcLocation) )
			return CacheNetRelevancy(true,RealViewer,Viewer);
		return 0;
		*/
		/*
		// check against terrain
		// FIXME - Jack, it would be nice to make this faster (a linecheck that just returns whether there was a hit)
		AZoneInfo* Z = Viewer->Region.Zone;
		if( Z && Z->bTerrainZone ) 
		{
			FVector End = Location;
			End.Z += 2.f * CollisionHeight;
			FCheckResult Hit(1.f);
			for( INT t=0;t<Z->Terrains.Num();t++ )
			{
				if( Z->Terrains(t)->LineCheck( Hit, End, SrcLocation, FVector(0.f,0.f,0.f) )==0 )
				{
					FPointRegion HitRegion = GetLevel()->Model->PointRegion( GetLevel()->GetLevelInfo(), Hit.Location );
					if( HitRegion.Zone == Z )
					return CacheNetRelevancy(false,RealViewer,Viewer);
				}
			}
		}
		*/
		/*
		// check against antiportal volumes
		FCheckResult Hit(1.f);
		for ( INT i=0; i<GetLevel()->AntiPortals.Num(); i++ )
			if ( GetLevel()->AntiPortals(i) 
				&& GetLevel()->AntiPortals(i)->GetPrimitive()->LineCheck( Hit, GetLevel()->AntiPortals(i), Location + FVector(0.f,0.f,CollisionHeight), SrcLocation, FVector(0.f,0.f,0.f), 0, TRACE_StopAtFirstHit )==0 )
				return CacheNetRelevancy(false,RealViewer,Viewer);
		*/
		return CacheNetRelevancy(true,RealViewer,Viewer);
	}
	unguardSlow;
}

INT ULevel::ServerTickClient( UNetConnection* Connection, FLOAT DeltaSeconds )
{
	guard(ULevel::ServerTickClient);
	check(Connection);
	check(Connection->State==USOCK_Pending || Connection->State==USOCK_Open || Connection->State==USOCK_Closed);

	// Handle not ready channels.
	INT Updated=0;
	if( Connection->Actor && Connection->IsNetReady(0) && Connection->State==USOCK_Open 
		&& Connection->Driver->Time-Connection->LastReceiveTime<1.5f )
	{
		// Get list of visible/relevant actors.
		FLOAT PruneActors = 0.f;
		clock(PruneActors);
		FMemMark Mark(GMem);
		NetTag++;
		Connection->TickCount++;

		// Set up to skip all sent temporary actors.
		guard(SkipSentTemporaries);
		for( INT i=0; i<Connection->SentTemporaries.Num(); i++ )
			Connection->SentTemporaries(i)->NetTag = NetTag;
		unguard;

		// Get viewer coordinates.
		AActor*      Viewer    = Connection->Actor->GetViewTarget();
		APlayerController* InViewer  = Connection->Actor;
		FVector      Location  = Viewer->Location;
		FRotator     Rotation  = InViewer->Rotation;
		InViewer->eventPlayerCalcView( Viewer, Location, Rotation );
		check(Viewer);

		// Compute ahead-vectors for prediction.
		FVector Ahead = FVector(0,0,0);
		if( Connection->TickCount & 1 )
		{
			FLOAT PredictSeconds = (Connection->TickCount&2) ? 0.4f : 0.9f;
			Ahead = PredictSeconds * Viewer->Velocity;
			if( Viewer->Base )
				Ahead += PredictSeconds * Viewer->Base->Velocity;
			FCheckResult Hit(1.0f);
			Hit.Location = Location + Ahead;
			Viewer->GetLevel()->Model->LineCheck(Hit,NULL,Hit.Location,Location,FVector(0,0,0),NF_NotVisBlocking,0);
			Location = Hit.Location;
		}

		// Make list of all actors to consider.
		INT              ConsiderCount  = 0;
		FActorPriority*  PriorityList   = new(GMem,Actors.Num()-iFirstNetRelevantActor+2)FActorPriority;
		FActorPriority** PriorityActors = new(GMem,Actors.Num()-iFirstNetRelevantActor+2)FActorPriority*;
		FVector          ViewPos        = Viewer->Location;
		FVector          ViewDir        = InViewer->Rotation.Vector();
		DOUBLE			 LastTime		= Connection->LastRepTime;
		DOUBLE           ThisTime       = Connection->Driver->Time;
		guard(MakeConsiderList);

		// add LevelInfo to the list
		AActor* Actor = Actors(0);
		if( Actor && (Actor->RemoteRole!=ROLE_None) && (Actor->NetTag!=NetTag) )
		{
			if ( appRound(LastTime*Actor->NetUpdateFrequency)!=appRound(ThisTime*Actor->NetUpdateFrequency) )
			{
				UActorChannel* Channel = NULL;
				if ( Actor->bOnlyDirtyReplication ) 
					Channel = Connection->ActorChannels.FindRef(Actor);
				if ( Channel 
					&& Channel->Recent.Num() 
					&& Channel->Dirty.Num() == 0
					&& !Channel->ActorDirty )
				{
					Channel->RelevantTime = NetDriver->Time;
				}
				else
				{
					Actor->NetTag                 = NetTag;
					PriorityList  [ConsiderCount] = FActorPriority( ViewPos, ViewDir, Connection, Actor );
					PriorityActors[ConsiderCount] = PriorityList + ConsiderCount++;
				}
			}
			LastTime += 0.023f;
			ThisTime += 0.023f;
		}
		FLOAT ServerTickRate = Engine->GetMaxTickRate();
		for( INT i=iFirstNetRelevantActor; i<Actors.Num(); i++ )
		{
			AActor* Actor = Actors(i);
			if( Actor 
				&& (Actor->RemoteRole!=ROLE_None)
				&& (Actor->NetTag!=NetTag) )
			{
				if ( Actor->bAlwaysRelevant || !Actor->bOnlyRelevantToOwner || (Actor->Owner == Viewer) || (Actor->Owner == InViewer) )
				{
					if ( (Actor->NetUpdateFrequency >= ServerTickRate)
						|| appRound(LastTime*Actor->NetUpdateFrequency)!=appRound(ThisTime*Actor->NetUpdateFrequency) )
					{
						UActorChannel* Channel = NULL;
						if ( Actor->bOnlyDirtyReplication ) 
							Channel = Connection->ActorChannels.FindRef(Actor);
						if ( Channel 
							&& Channel->Recent.Num() 
							&& Channel->Dirty.Num() == 0
							&& !Channel->ActorDirty )
						{
							Channel->RelevantTime = NetDriver->Time;
						}
						else
						{
							Actor->NetTag                 = NetTag;
							PriorityList  [ConsiderCount] = FActorPriority( ViewPos, ViewDir, Connection, Actor );
							PriorityActors[ConsiderCount] = PriorityList + ConsiderCount++;
						}
					}
					LastTime += 0.023f;
					ThisTime += 0.023f;
				}
			}
		}
		Connection->LastRepTime = Connection->Driver->Time;
		unguard;
		unclock(PruneActors);

		//FLOAT RepTime = 0.f;
		FLOAT RelevantTime = 0.f;
		clock(RelevantTime);
		// Sort by priority.
		guard(SortConsiderList);
		Sort( PriorityActors, ConsiderCount );
		unguard;

		// Update all relevant actors in sorted order.
		guard(UpdateRelevant);
		for( INT j=0; j<ConsiderCount; j++ )
		{
			UActorChannel* Channel     = PriorityActors[j]->Channel;
			if ( !Channel || Channel->Actor ) //make sure didn't just close this channel
			{
				AActor*        Actor       = PriorityActors[j]->Actor;
				UBOOL          CanSee      = 0;
				// only check visibility on already visible actors every 1.0 + 0.5R seconds
				// bTearOff actors should never be checked
				if ( !Actor->bTearOff && (!Channel || NetDriver->Time-Channel->RelevantTime>1.f) )
					CanSee = Actor->IsNetRelevantFor( InViewer, Viewer, Location );
				if( CanSee || (Channel && NetDriver->Time-Channel->RelevantTime<NetDriver->RelevantTimeout) )
				{
					//unclock(RelevantTime);
					//clock(RepTime);
					// Find or create the channel for this actor.
					GStats.DWORDStats(GEngineStats.STATS_Net_NumPV)++;
					if( !Channel && Connection->PackageMap->ObjectToIndex(Actor->GetClass())!=INDEX_NONE )
					{
						// Create a new channel for this actor.
						Channel = (UActorChannel*)Connection->CreateChannel( CHTYPE_Actor, 1 );
						if( Channel )
							Channel->SetChannelActor( Actor );
					}
					if( Channel )
					{
						if ( !Connection->IsNetReady(0) ) // here also???
							break;
						if( CanSee )
							Channel->RelevantTime = NetDriver->Time + 0.5f * appFrand();
						if( Channel->IsNetReady(0) )
						{
							//debugf(TEXT("Replicate %s"),Actor->GetName());
							Channel->ReplicateActor();
							Updated++;
						}
						if ( !Connection->IsNetReady(0) )
							break;
					}
					//unclock(RepTime);
					//clock(RelevantTime);
				}
				else if( Channel )
				{
					Channel->Close();
					// skeletal attachments aren't relevant anymore either
					if ( Cast<USkeletalMesh>(Actor->Mesh) )
					{
						for ( INT k=0; k<Actor->Attached.Num(); k++ )
							if ( Actor->Attached(k) && !Actor->Attached(k)->bAlwaysRelevant 
								&& (Actor->Attached(k)->AttachmentBone != NAME_None) )
							{
								UChannel *AttachedChannel = Connection->ActorChannels.FindRef(Actor->Attached(k));
								if ( AttachedChannel )
									AttachedChannel->Close();
							}
					}
				}
			}
		}
		unguard;
		Mark.Pop();
		unclock(RelevantTime);
		//debugf(TEXT("Potential %04i ConsiderCount %03i Prune=%01.4f Relevance=%01.4f Rep=%01.4f"),Actors.Num()-iFirstNetRelevantActor, 
		//			ConsiderCount, PruneActors * GSecondsPerCycle * 1000.f,RelevantTime * GSecondsPerCycle * 1000.f,RepTime * GSecondsPerCycle * 1000.f);
	}
	return Updated;
	unguard;
}

/*-----------------------------------------------------------------------------
	Network server tick.
-----------------------------------------------------------------------------*/

void UNetConnection::SetActorDirty(AActor* DirtyActor )
{
	guardSlow(UNetConnection::SetActorDirty);
	if( Actor && State==USOCK_Open )
	{
		UActorChannel* Channel = ActorChannels.FindRef(DirtyActor);
		if ( Channel )
			Channel->ActorDirty = true;
	}
	unguardSlow;
}

void ULevel::TickNetServer( FLOAT DeltaSeconds )
{
	guard(ULevel::TickNetServer);

	// Update all clients.
	INT Updated=0;

	// first, set which channels have dirty actors (need replication)
	AActor* Actor = Actors(0);
	if( Actor && Actor->bNetDirty )
	{
		for( INT j=NetDriver->ClientConnections.Num()-1; j>=0; j-- )
			NetDriver->ClientConnections(j)->SetActorDirty(Actor);
		Actor->bNetDirty = 0;
	}
	for( INT i=iFirstNetRelevantActor; i<Actors.Num(); i++ )
	{
		AActor* Actor = Actors(i);
		if( Actor && Actor->bNetDirty )
		{
			for( INT j=NetDriver->ClientConnections.Num()-1; j>=0; j-- )
				NetDriver->ClientConnections(j)->SetActorDirty(Actor);
			Actor->bNetDirty = 0;
		}
	}
	clock(GStats.DWORDStats(GEngineStats.STATS_Game_NetTickCycles));
	for( INT i=NetDriver->ClientConnections.Num()-1; i>=0; i-- )
		Updated += ServerTickClient( NetDriver->ClientConnections(i), DeltaSeconds );
	unclock(GStats.DWORDStats(GEngineStats.STATS_Game_NetTickCycles));

	// Log message.
	if( (INT)(TimeSeconds-DeltaSeconds)!=(INT)(TimeSeconds) )
		debugf( NAME_Title, LocalizeProgress(TEXT("RunningNet"),TEXT("Engine")), *GetLevelInfo()->Title, *URL.Map, NetDriver->ClientConnections.Num() );

	// Stats.
	if( Updated )
	{
		for( INT i=0; i<NetDriver->ClientConnections.Num(); i++ )
		{
			UNetConnection* Connection = NetDriver->ClientConnections(i);
			if( Connection->Actor && Connection->State==USOCK_Open )
			{
				if( Connection->UserFlags&1 )
				{
					// Send stats.
					INT NumActors=0;
					for( INT i=0; i<Actors.Num(); i++ )
						NumActors += Actors(i)!=NULL;

					FString Stats = FString::Printf
					(
						TEXT("r=%i cli=%i act=%03.1f (%i) net=%03.1f pv/c=%i rep/c=%i rpc/c=%i"),
						appRound(Engine->GetMaxTickRate()),
						NetDriver->ClientConnections.Num(),
						GSecondsPerCycle*1000*GStats.DWORDStats(GEngineStats.STATS_Game_ActorTickCycles),
						NumActors,
						GSecondsPerCycle*1000*GStats.DWORDStats(GEngineStats.STATS_Game_NetTickCycles),
						GStats.DWORDStats(GEngineStats.STATS_Net_NumPV)   / NetDriver->ClientConnections.Num(),
						GStats.DWORDStats(GEngineStats.STATS_Net_NumReps) / NetDriver->ClientConnections.Num(),
						GStats.DWORDStats(GEngineStats.STATS_Net_NumRPC)  / NetDriver->ClientConnections.Num()
					);
					Connection->Actor->eventClientMessage( *Stats, NAME_None );
				}
				if( Connection->UserFlags&2 )
				{
					FString Stats = FString::Printf
					(
						TEXT("snd=%02.1f recv=%02.1f"),
						GSecondsPerCycle*1000*Connection->Driver->SendCycles,
						GSecondsPerCycle*1000*Connection->Driver->RecvCycles
					);
					Connection->Actor->eventClientMessage( *Stats, NAME_None );
				}
			}
		}
	}
	unguard;
}

/*-----------------------------------------------------------------------------
	Demo Recording tick.
-----------------------------------------------------------------------------*/

INT ULevel::TickDemoRecord( FLOAT DeltaSeconds )
{
	guard(ULevel::TickDemo);

	// All replicatable actors are assumed to be relevant for demo recording.
	UNetConnection* Connection = DemoRecDriver->ClientConnections(0);
	for( INT i=0; i<Actors.Num(); i++ )
	{
		AActor* Actor = Actors(i);
		UBOOL IsNetClient = (GetLevelInfo()->NetMode == NM_Client);
		if
		(	Actor
		&&	(Actor->RemoteRole!=ROLE_None || (IsNetClient && Actor->Role!=ROLE_None && Actor->Role != ROLE_Authority))
		&&  (i>=iFirstDynamicActor || Actor->IsA(AZoneInfo::StaticClass()))
		&&  (!Actor->bNetTemporary || Connection->SentTemporaries.FindItemIndex(Actor)==INDEX_NONE)
		&&  (Actor->bStatic || !Actor->GetClass()->GetDefaultActor()->bStatic))
		{
			// Create a new channel for this actor.
			UActorChannel* Channel = Connection->ActorChannels.FindRef( Actor );
			if( !Channel && Connection->PackageMap->ObjectToIndex(Actor->GetClass())!=INDEX_NONE )
			{
				// Check we haven't run out of actor channels.
				Channel = (UActorChannel*)Connection->CreateChannel( CHTYPE_Actor, 1 );
				check(Channel);
				Channel->SetChannelActor( Actor );
			}
			if( Channel )
			{
				// Send it out!
				check(!Channel->Closing);
				if( Channel->IsNetReady(0) )
				{
					Actor->bDemoRecording = 1;
					Actor->bClientDemoRecording = IsNetClient;
					if(IsNetClient)
						Exchange(Actor->RemoteRole, Actor->Role);
					Channel->ReplicateActor();
					if(IsNetClient)
						Exchange(Actor->RemoteRole, Actor->Role);
					Actor->bDemoRecording = 0;
					Actor->bClientDemoRecording = 0;
				}
			}
		}
	}
	return 1;
	unguard;
}
INT ULevel::TickDemoPlayback( FLOAT DeltaSeconds )
{
	guard(ULevel::TickDemoPlayback);
	if
	(	GetLevelInfo()->LevelAction==LEVACT_Connecting 
	&&	DemoRecDriver->ServerConnection->State!=USOCK_Pending )
	{
		GetLevelInfo()->LevelAction = LEVACT_None;
		Engine->SetProgress( TEXT(""), TEXT(""), TEXT(""), 0.0f );
	} 
	if( DemoRecDriver->ServerConnection->State==USOCK_Closed )
	{
		// Demo stopped playing
		check(Engine->Client->Viewports.Num());
		Engine->SetClientTravel( Engine->Client->Viewports(0), TEXT("?entry"), 0, TRAVEL_Absolute );
	}
	return 1;
	unguard;
}

/*-----------------------------------------------------------------------------
	Main level timer tick handler.
-----------------------------------------------------------------------------*/
UBOOL ULevel::IsPaused()
{
	guardSlow(ULevel::IsPaused);
	
	return ( GetLevelInfo()->Pauser && (TimeSeconds >= GetLevelInfo()->PauseDelay) );
	unguardSlow;
}
//
// Update the level after a variable amount of time, DeltaSeconds, has passed.
// All child actors are ticked after their owners have been ticked.
//
void ULevel::Tick( ELevelTick TickType, FLOAT DeltaSeconds )
{
	guard(ULevel::Tick);
	ALevelInfo* Info = GetLevelInfo();

	FMemMark Mark(GMem);
	FMemMark EngineMark(GEngineMem);
	GInitRunaway();
	InTick=1;

	// Update the net code and fetch all incoming packets.
	guard(UpdatePreNet);
	if( NetDriver )
	{
		NetDriver->TickDispatch( DeltaSeconds );
		if( NetDriver->ServerConnection )
			TickNetClient( DeltaSeconds );
	}
	unguard;

	// Fetch demo playback packets from demo file.
	guard(UpdatePreDemoRec);
	if( DemoRecDriver )
	{
		DemoRecDriver->TickDispatch( DeltaSeconds );
		if( DemoRecDriver->ServerConnection )
			TickDemoPlayback( DeltaSeconds );
	}
	unguard;

	// Update collision.
	guard(UpdateCollision);
	if( Hash )
		Hash->Tick();
	unguard;

	// Update time.
	guard(UpdateTime);
	if( !GIsBenchmarking )
		DeltaSeconds *= Info->TimeDilation;
	if ( !IsPaused() )
		TimeSeconds += DeltaSeconds;
	Info->TimeSeconds = TimeSeconds;
	UpdateTime(Info);
	if( Info->bPlayersOnly )
		TickType = LEVELTICK_ViewportsOnly;
	unguard;

	// Clamp time between 2000 fps and 2.5 fps.
	DeltaSeconds = Clamp(DeltaSeconds,0.0005f,0.40f);

	// If caller wants time update only, or we are paused, skip the rest.
	clock(GStats.DWORDStats(GEngineStats.STATS_Game_ActorTickCycles));
	if
	(	(TickType!=LEVELTICK_TimeOnly)
	&&	!IsPaused()
	&&	(!NetDriver || !NetDriver->ServerConnection || NetDriver->ServerConnection->State==USOCK_Open) )
	{
		// Tick all actors, owners before owned.
		guard(TickAllActors);
		NewlySpawned = NULL;
		INT Updated  = 0;

#ifdef WITH_KARMA
        /* KTODO: Should we do this _after_ actor physics? */
        KTickLevelKarma(this, DeltaSeconds);
#endif

		for( INT iActor=iFirstDynamicActor; iActor<Actors.Num(); iActor++ )
			if( Actors( iActor ) && !Actors(iActor)->bDeleteMe )
				Updated += Actors( iActor )->Tick(DeltaSeconds,TickType);
		while( NewlySpawned && Updated )
		{
			FActorLink* Link = NewlySpawned;
			NewlySpawned     = NULL;
			Updated          = 0;
			for( Link; Link; Link=Link->Next )
				if( Link->Actor->bTicked!=(DWORD)Ticked && !Link->Actor->bDeleteMe )
					Updated += Link->Actor->Tick( DeltaSeconds, TickType );
		}
#ifdef WITH_KARMA
		if(KGData)
			KGData->bDoTick = 0;
#endif
		unguard;
	}
	else if( IsPaused() )
	{
		// Absorb input if paused.
		guard(AbsorbedPaused);
		NewlySpawned = NULL;
		INT Updated  = 1;
		for( INT iActor=iFirstDynamicActor; iActor<Actors.Num(); iActor++ )
		{
			APlayerController* PC=Cast<APlayerController>(Actors(iActor));
			if( PC && PC->Player )
			{
				PC->Player->ReadInput( DeltaSeconds );
				PC->PlayerInput->eventPlayerInput( DeltaSeconds );
				for( TFieldIterator<UFloatProperty> It(PC->GetClass()); It; ++It )
					if( It->PropertyFlags & CPF_Input )
						*(FLOAT*)((BYTE*)PC + It->Offset) = 0.f;
				PC->bTicked = (DWORD)Ticked;
			}
			else if( Actors(iActor) )
			{
				if ( Actors(iActor)->bAlwaysTick && !Actors(iActor)->bDeleteMe )
					Actors(iActor)->Tick(DeltaSeconds,TickType);
				else
					Actors(iActor)->bTicked = (DWORD)Ticked;
			}
		}
		while( NewlySpawned && Updated )
		{
			FActorLink* Link = NewlySpawned;
			NewlySpawned     = NULL;
			Updated          = 0;
			for( Link; Link; Link=Link->Next )
				if( Link->Actor->bTicked!=(DWORD)Ticked && !Link->Actor->bDeleteMe )
				{
					if ( Link->Actor->bAlwaysTick )
						Updated += Link->Actor->Tick( DeltaSeconds, TickType );
					else
						Link->Actor->bTicked = (DWORD)Ticked;
				}
		}
		unguard;
	}
	unclock(GStats.DWORDStats(GEngineStats.STATS_Game_ActorTickCycles));
/*
	if ( (GStats.DWORDStats(GEngineStats.STATS_Game_ActorTickCycles) * GSecondsPerCycle * 1000.0f >= 30.f)
		|| (GScriptCycles * GSecondsPerCycle * 1000.0f > 10.f) )
 		debugf(TEXT("********** ActorTick Cycles %f at %f pathtime %f script time %f"), 
					GStats.DWORDStats(GEngineStats.STATS_Game_ActorTickCycles) * GSecondsPerCycle * 1000.0f, 
					TimeSeconds,
					GStats.DWORDStats(GEngineStats.STATS_Game_FindPathCycles) * GSecondsPerCycle * 1000.0f,
					GScriptCycles * GSecondsPerCycle * 1000.0f
					);
*/
 	if ( Engine->Client && Engine->Client->InteractionMaster )
		Engine->Client->InteractionMaster->MasterProcessTick(DeltaSeconds);

	// Update net server and flush networking.
	guard(UpdateNetServer);
	if( NetDriver )
	{
		if( !NetDriver->ServerConnection )
			TickNetServer( DeltaSeconds );
		NetDriver->TickFlush();
	}
	unguard;

	// Demo Recording.
	guard(UpdatePostDemoRec);
	if( DemoRecDriver )
	{
		if( !DemoRecDriver->ServerConnection )
			TickDemoRecord( DeltaSeconds );
		DemoRecDriver->TickFlush();
	}
	unguard;
	// Finish up.
	Ticked = !Ticked;
	InTick = 0;
	Mark.Pop();
	EngineMark.Pop();

	clock(GStats.DWORDStats(GEngineStats.STATS_Game_CleanupDestroyedCycles));
	// gam ---
#ifndef _XBOX
	UGameEngine* GameEngine = Cast<UGameEngine>(Engine);
	if( GameEngine && ( !GameEngine->GLevel || ( GameEngine->GLevel == GameEngine->GEntry ) ) )
    	CleanupDestroyed( 1 );
	else
    	CleanupDestroyed( 0 );
#endif
    // --- gam
	unclock(GStats.DWORDStats(GEngineStats.STATS_Game_CleanupDestroyedCycles));
	
	unguardf(( TEXT("(NetMode=%i)"), GetLevelInfo()->NetMode ));
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

