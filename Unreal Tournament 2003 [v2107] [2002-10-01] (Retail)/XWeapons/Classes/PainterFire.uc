class PainterFire extends WeaponFire;

var PainterBeamEffect Beam;
var float UpTime;
var bool bDoHit;
var bool bValidMark;
var bool bInitialMark;
var bool bAlreadyMarked;

var IonCannon IonCannon;
var float MarkTime;
var Vector MarkLocation;
var() float TraceRange;
var() float PaintDuration;
var Vector EndEffect;

var() Sound MarkSound;
var() Sound AquiredSound;

var() String TAGFireForce;
var() String TAGMarkForce;
var() String TAGAquiredForce;
var bool bMarkStarted;

function Destroyed()
{
    if (Beam != None)
    {
        Beam.Destroy();
    }

    Super.Destroyed();
}

state Paint
{
	function Rotator AdjustAim(Vector Start, float InAimError)
	{
		if ( Bot(Instigator.Controller) != None )
		{
			if ( bAlreadyMarked )
				return Rotator(MarkLocation - Start);
			else
				return rotator(Painter(Instigator.Weapon).MarkLocation - Instigator.Location + Instigator.CollisionHeight * vect(0,0,1));
		}
		else
			return Global.AdjustAim(Start, InAimError);
	}
	
    function BeginState()
    {
        IonCannon = None;

        if (Weapon.Role == ROLE_Authority)
        {
            if (Beam == None)
            {
                Beam = Spawn(class'PainterBeamEffect');
            }
            bInitialMark = true;
            bValidMark = false;
            MarkTime = Level.TimeSeconds;
            SetTimer(0.25, true);
        }
        
        ClientPlayForceFeedback(TAGFireForce);
    }

    function Timer()
    {
        bDoHit = true;
    }

    function ModeTick(float dt)
    {
        local Vector StartTrace, EndTrace, X,Y,Z;
        local Vector HitLocation, HitNormal;
        local Actor Other;
        local Rotator Aim;
        local bool bEngageCannon;
    			
        //if (Weapon.Role < ROLE_Authority) return;
        // ---- server only from here on ---- //

        if (!bIsFiring)
        {
            StopFiring();
        }

        Weapon.GetViewAxes(X,Y,Z);

        // the to-hit trace always starts right in front of the eye
        StartTrace = Instigator.Location + Instigator.EyePosition() + X*Instigator.CollisionRadius;

	    Aim = AdjustAim(StartTrace, AimError);
        X = Vector(Aim);
        EndTrace = StartTrace + TraceRange * X;

        Other = Trace(HitLocation, HitNormal, EndTrace, StartTrace, false);

        if (Other != None && Other != Instigator)
        {
            if ( bDoHit )
            {
                bValidMark = false;

                if (Other.bWorldGeometry)
                {
                    if (VSize(HitLocation - MarkLocation) < 50.0)
                    {
						Instigator.MakeNoise(3.0);
                        if (Level.TimeSeconds - MarkTime > 0.4)
                        {
                            bEngageCannon = (Level.TimeSeconds - MarkTime > PaintDuration);
                            if ( IonCannon == None )
								IonCannon = Painter(Weapon).CheckMark(HitLocation, bEngageCannon);
							else if ( !IonCannon.CheckMark(Instigator,HitLocation,bEngageCannon) )
								IonCannon = None;
								
                            if ( IonCannon != None )
                            {
                                if ( IonCannon.IsFiring() )
                                {
                                    Weapon.ConsumeAmmo(ThisModeNum, 1);

                                    Instigator.Controller.ClientSwitchToBestWeapon();

                                    if (Beam != None)
                                        Beam.SetTargetState(PTS_Aquired);
                                        
                                    StopForceFeedback(TAGMarkForce);
                                    ClientPlayForceFeedback(TAGAquiredForce);

                                    StopFiring();
                                }
                                else
                                {
                                    bValidMark = true;
                                    
                                    if (!bMarkStarted)
                                    {
										bMarkStarted = true;
										ClientPlayForceFeedback(TAGMarkForce);
									}
                                }
                            }
                            else
                            {
                                MarkTime = Level.TimeSeconds;
                                bValidMark = false;
                                bMarkStarted = false;
                            }
                        }
                    }
                    else
                    {
						bAlreadyMarked = true;
                        MarkTime = Level.TimeSeconds;
                        MarkLocation = HitLocation;
                        bValidMark = false;
                        bMarkStarted = false;
                    }
                }
                else
                {
                    MarkTime = Level.TimeSeconds;
                    bValidMark = false;
                    bMarkStarted = false;
                }
                bDoHit = false;
            }

            EndEffect = HitLocation;
        }
        else
        {
            EndEffect = EndTrace;
        }

        Painter(Weapon).EndEffect = EndEffect;

        if (Beam != None)
        {
            Beam.EndEffect = EndEffect;
            if (bValidMark)
                Beam.SetTargetState(PTS_Marked);
            else
                Beam.SetTargetState(PTS_Aiming);
        }
        
        if ( IonCannon != None )
        {
            if ( bValidMark )
            {
			    if ( IonCannon.Fear == None )
			    {
				    IonCannon.Fear = Spawn(class'AvoidMarker',,,MarkLocation);
				    IonCannon.Fear.SetCollisionSize(0.4 * IonCannon.DamageRadius,100); 
				    IonCannon.Fear.StartleBots();
			    }
		    }
		    else if ( IonCannon.Fear != None )
		    	IonCannon.RemoveFear();				
        }
    }

    function StopFiring()
    {
		bMarkStarted = false;
        if (Beam != None)
        {
            Beam.SetTargetState(PTS_Cancelled);
        }
        GotoState('');
    }

    function EndState()
    {
		bAlreadyMarked = false;
        SetTimer(0, false);
        StopForceFeedback(TAGFireForce);
    }
}

function DoFireEffect()
{
}

function ModeHoldFire()
{
    GotoState('Paint');
}

function StartBerserk()
{
}

function StopBerserk()
{
}

function vector GetFireStart(vector X, vector Y, vector Z)
{
    return Instigator.Location + Instigator.EyePosition() + X*Instigator.CollisionRadius;
}


defaultproperties
{
    AmmoClass=class'BallAmmo'
    AmmoPerFire=0

    FireAnim=Fire
    FireEndAnim=None

    //FlashEmitterClass=class'xEffects.LinkMuzFlash'

    TraceRange=10000
    FireRate=0.6
    bFireOnRelease=true

    BotRefireRate=1.0
	WarnTargetPct=+0.1
    bSplashDamage=true
    bRecommendSplashDamage=true

    PaintDuration=2.0

    MarkSound=Sound'WeaponSounds.TAGRifle.TagFireB'
    AquiredSound=Sound'WeaponSounds.TAGRifle.TagTargetAquired'
    TAGFireForce="TAGFireA"
    TAGMarkForce="TAGFireB"
    TAGAquiredForce="TAGAquire"

}
