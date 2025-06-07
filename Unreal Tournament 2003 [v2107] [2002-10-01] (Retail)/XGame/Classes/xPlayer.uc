class xPlayer extends UnrealPlayer
    DependsOn(xUtil);

#exec OBJ LOAD File="MenuSounds.uax"

// attract mode
var AttractCamera camlist[20];
var int numcams, curcam;
var Pawn attracttarget, attracttarget2;
var float camtime, targettime, gibwatchtime;
var bool autozoom;
var Vector focuspoint;

// combos
const MinComboKeyTime = 0.05;   // keys pressed faster than this will be considered a double-press
const MaxComboKeyTime = 0.35;   // max time player has between button presses

var transient int InputHistory[4];
var transient float LastKeyTime;
var transient int OldKey;
var float MinAdrenalineCost;
var string ComboNameList[16];
var class<Combo> ComboList[16];

var xUtil.PlayerRecord PawnSetupRecord;

replication
{
	// client to server
	reliable if ( Role < ROLE_Authority )
		ServerDoCombo; 
	unreliable if( Role < ROLE_Authority )
		L33tPhrase;
}
	
function StopFiring()
{
}

simulated function PlayBeepSound()
{
    ViewTarget.PlaySound(sound'MenuSounds.selectJ', SLOT_None,,,,,false);
}

simulated function float RateWeapon(Weapon w)
{
    return w.Default.Priority;
}

exec function L33TPhrase( int phraseNum )
{
	SendMessage(None, 'HIDDEN', phraseNum, 5, 'GLOBAL');
}

simulated event PostBeginPlay()
{
    local int c;
	
    Super.PostBeginPlay();

    for (c = 0; c < ArrayCount(ComboList); c++)
    {
        if ( ComboNameList[c] == "" )
            break;
		ComboList[c] = class<Combo>(DynamicLoadObject(ComboNameList[c],class'Class'));
		if ( ComboList[c] == None )
			break;
		MinAdrenalineCost = FMin(MinAdrenalineCost,ComboList[c].Default.AdrenalineCost);
    }
    FillCameraList();
    LastKillTime = -5.0;
}

event PlayerTick( float DeltaTime )
{
    local int CurrentKey, DiffKey;
    local int c, i;
	local bool bFullBuffer,bMatch;
	
	Super.PlayerTick(DeltaTime);

    if ( (Pawn == None) || !bAdrenalineEnabled || (Adrenaline < MinAdrenalineCost) )
    {
		InputHistory[0] = 0; 
		return;
	}
    CurrentKey = 0;

    if (aForward > 0)		CurrentKey = CurrentKey | 1; // CK_Up
    else if (aForward < 0)  CurrentKey = CurrentKey | 2; // CK_Down
    if (aStrafe > 0)		CurrentKey = CurrentKey | 4; // CK_Left
    else if (aStrafe < 0)   CurrentKey = CurrentKey | 8; // CK_Right

    if ( CurrentKey != OldKey )
    {
        DiffKey = CurrentKey & ~OldKey; // keys that are pressed now but were not last frame

        if (DiffKey != 0)
        {
            if (Level.TimeSeconds - LastKeyTime < MinComboKeyTime) // if the time was too short between the last keypress, consider it a double press
            {
                InputHistory[0] = InputHistory[0] | DiffKey;
            }
            else if (Level.TimeSeconds - LastKeyTime > MaxComboKeyTime) // wipe the buffer if too long since last keypress
            {
                InputHistory[0] = Diffkey;
                InputHistory[1] = 0;
                LastKeyTime = Level.TimeSeconds;
            }
            else
            {
				bFullBuffer = true;
                for ( i=3; i>0; i-- )
                {
                    InputHistory[i] = InputHistory[i - 1];
                    if ( InputHistory[i] == 0 )
						bFullBuffer = false;
				}
                InputHistory[0] = DiffKey;
                LastKeyTime = Level.TimeSeconds;
            }

            // check for combo matches
            if ( bFullBuffer )
            {
                for ( c=0; c<16; c++ )
                {
                    if ( ComboList[c] == None )
                        break;
                    else if ( (ComboList[c].Default.species == None) || (ComboList[c].Default.species == xPawn(Pawn).Species) )
                    {
						bMatch = true;
                        for ( i=0; i <4; i++)
                            if ( InputHistory[i] != ComboList[c].Default.keys[i] )
                            {
								bMatch = false;
                                break;
                            }

                        if ( bMatch )
                        {
                            DoCombo(ComboList[c]);
                            InputHistory[0] = 0;
                        }
                    }
                }
            }
        }

        OldKey = CurrentKey;
    }
}

function DoCombo( class<Combo> ComboClass )
{
    if (Adrenaline >= ComboClass.default.AdrenalineCost && !Pawn.InCurrentCombo() )
    {
        ServerDoCombo( ComboClass );
    }
}

function ServerDoCombo( class<Combo> ComboClass )
{
    if (Adrenaline >= ComboClass.default.AdrenalineCost && !Pawn.InCurrentCombo() )
    {
        if (ComboClass.default.ExecMessage != "")
            ReceiveLocalizedMessage( class'ComboMessage', , , , ComboClass );

        xPawn(Pawn).DoCombo( ComboClass );
    }
}

state AttractMode
{
	ignores SwitchWeapon, RestartLevel, ClientRestart, Suicide,
	 ThrowWeapon, NotifyPhysicsVolumeChange, NotifyHeadVolumeChange,
     Say, TeamSay;

	function bool IsSpectating()
	{
		return true;
	}

	function ProcessMove(float DeltaTime, vector NewAccel, eDoubleClickDir DoubleClickMove, rotator DeltaRot)	
	{
	}

	function PlayerMove(float DeltaTime)
	{
        local float deltayaw, destyaw;
        local Rotator newrot;

        if ( attracttarget == None )
			return;

        // updates camera yaw to smoothly rotate to the pawn facing
        if ( bBehindView )
        {
			if ( (attracttarget.Controller == None) || (AttractTarget.Controller.Enemy == None) )
				return;
            NewRot = Rotator(attracttarget.controller.Enemy.location - attracttarget.location);
			destyaw = NewRot.Yaw;
            deltayaw = (destyaw & 65535) - (rotation.yaw & 65535);
            if (deltayaw < -32768) deltayaw += 65536;
            else if (deltayaw > 32768) deltayaw -= 65536;

            newrot = rotation;
            newrot.yaw += deltayaw * DeltaTime;
            SetRotation(newrot);
        }
        else
        {
            newrot = CameraTrack(attracttarget, DeltaTime);
            SetRotation(newrot);
        }
	}

	exec function NextWeapon()
	{
	}

	exec function PrevWeapon()
	{
	}

	exec function Fire( optional float F )
	{
        // start playing
	}

	exec function AltFire( optional float F )
	{
        Fire(F);
	}

	function BeginState()
	{
		if ( Pawn != None )
		{
			SetLocation(Pawn.Location);
		}
		bCollideWorld = true;
        if ( curcam == -1 )   
        {
            FillCameraList();
            camtime = 0;
            targettime = 0;
            autozoom = true;
            curcam = -1;
        }
        
        Timer();
        SetTimer(0.5, true);
	}

	function EndState()
	{
		PlayerReplicationInfo.bIsSpectator = false;		
		bCollideWorld = false;
        curcam = -1;
	}

    function Timer()
    {
        local bool switchedbots;
        local Vector newloc;
        local int newcam;

        camtime += 0.5;
        targettime += 0.5;
        bFrozen = false;

        // keep watching a target for a few seconds after it dies
        if (gibwatchtime > 0)
        {
            gibwatchtime -= 0.5;
            if (gibwatchtime <= 0)
                attracttarget = None;
            else
                return;
        }
        else if ( attracttarget != None && attracttarget.Health <= 0 )
        {
            gibwatchtime = 4;
            //Log("attract: watching gib");
        }

        // switch targets //
        if (attracttarget == None
            || targettime > 30 )
        {
            attracttarget = PickNextBot(attracttarget);
            switchedbots = true;
            targettime = 0;
         }

        if (attracttarget == None)
            return;

        // switch views //
        if (
            switchedbots ||
            camtime > 10 ||
            bBehindView == false && (rotation.pitch < -10000 || !LineOfSight(curcam, attracttarget))
        )
        {
            camtime = 0;
            FovAngle = default.FovAngle;
            SetViewTarget(self);
	    	bBehindView = false;

            // look for a placed camera
            if (FindFixedCam(attracttarget, newcam))
            {
                focuspoint = attracttarget.Location;
                curcam = newcam;
                SetLocation(camlist[curcam].Location);
                FovAngle = camlist[curcam].ViewAngle;
        
                SetRotation(CameraTrack(attracttarget, 0));
                //Log("attract: camera "$camlist[curcam]);
            }
            // use a floating camera
            else if (FRand() < 0.5)
            {
                newloc = FindFloatingCam(attracttarget);
                focuspoint = attracttarget.Location;
                curcam = -1;
                SetLocation(newloc);
            
                SetRotation(CameraTrack(attracttarget, 0));
                //Log("attract: free camera");
            }
            // chase mode
            else
            {
                curcam = -1;
    		    SetViewTarget(attracttarget);		 
	    	    bBehindView = true;
                SetRotation(attracttarget.rotation);
                CameraDeltaRotation.Pitch = -3000;
                CameraDist = 6;
                //Log("attract: chase camera");
            }
        }
    }
}

state ViewPlayer extends PlayerWalking
{
	function PlayerMove(float DeltaTime)
	{
        Super.PlayerMove(DeltaTime);
        
        CameraSwivel = CameraTrack(pawn, DeltaTime);
    }

    function PlayerCalcView(out actor ViewActor, out vector CameraLocation, out rotator CameraRotation )
    {
        // not calling super
        CameraRotation = CameraSwivel;
        CameraLocation = location; //camlist[curcam].location;
        ViewActor = self;
    }

    function BeginState()
    {
        FillCameraList();
        bBehindView = true;
        SetViewTarget(self);
        curcam = -2;
        autozoom = true;
        Timer();
        SetTimer(0.5, true);
    }

    function EndState()
    {
        CameraSwivel = rot(0,0,0);
        bBehindView = false;
        FovAngle = default.FovAngle;
        DesiredFOV = FovAngle;
        SetViewTarget(pawn);
    }

    function Timer()
    {
        local Vector newloc;
        local int newcam;

        if (curcam == -2 || !LineOfSight(curcam, pawn))
        {
            //Log("attract: switch camera");

            camtime = 0;

            if (FindFixedCam(pawn, newcam))
            {       
                if (curcam != newcam)
                {
                    focuspoint = pawn.Location;
                    curcam = newcam;
                    SetLocation(camlist[curcam].location);
                    FovAngle = camlist[curcam].ViewAngle;
                    //Log("attract: viewing from "$camlist[curcam]);
                }
                else
                {
                    //Log("attract: zoinks! this shouldn't happen");
                }
            }
            else
            {
                newloc = FindFloatingCam(pawn);
                SetLocation(newloc);
                curcam = -1;
                FovAngle = default.FovAngle;
                focuspoint = pawn.Location;
                //Log("attract: floating");
            }

            CameraSwivel = CameraTrack(pawn, 0);
        }
    }

    exec function TogglePlayerAttract()
    {
        GotoState('PlayerWalking');
    }
}

function FillCameraList()
{
    local AttractCamera cam;
    numcams = 0;
    foreach AllActors(class'AttractCamera', cam)
    {
        camlist[numcams++] = cam;
        if (numcams == 20) break;
    }
}

function bool FindFixedCam(Pawn target, out int newcam)
{
    local int c, bestc;
    local float dist, bestdist;

    bestc = -1;

    for (c = 0; c < numcams; c++)
    {
        dist = VSize(target.location - camlist[c].location);

        if ((bestc == -1 || dist < bestdist) && LineOfSight(c, target))
        {
            bestc = c;
            bestdist = dist;
        }
    }

    if (bestc == -1) return false;

    newcam = bestc;
    return true;
}

function Vector FindFloatingCam(Pawn target)
{
    local Vector v1, v2, d;
    local Rotator r;
    local Vector hitloc, hitnormal;
    local Actor hitactor;
    local int tries;

    while (tries++ < 10)
    {
        v1 = target.Location;
        r = target.Rotation;
        r.Pitch = FRand()*12000 - 2000;
        if (VSize(target.Velocity) < 100)
            r.Yaw += FRand()*24000;
        else
            r.Yaw += FRand()*12000;
        d = Vector(r);
        v2 = v1 + d*2000;
        v1 += d*50;

        hitactor = Trace(hitloc, hitnormal, v2, v1, false);

        if (hitactor != None && VSize(hitloc - v1) > 250)
        {
            return (hitloc - d*50);
        }
    }
    // no good spots found, return something reasonable
    if (hitactor != None)
        return (hitloc - d*50);
    else
        return v2;
}

function Pawn PickNextBot(Pawn current)
{
    local Controller con;
    local int b;

    if (current != None) con = current.Controller;
    for (b=0; b<Level.Game.NumBots; b++)
    {
        if (con != None) con = con.NextController;
        if (con == None) con = Level.ControllerList;
        if (con.IsA('Bot') && con.Pawn != None && !con.IsInState('PlayerWaiting'))
        {
            return con.Pawn;
        }
    }
    return None;
}

function bool LineOfSight(int c, Pawn target)
{
    local vector v1, v2;
    local AttractCamera cam;
    local Vector hitloc, hitnormal;

    if (c >= 0) {
        cam = camlist[c];
        v1 = cam.location;
    } else {
        v1 = self.location;
    }
    v2 = target.location;
    v2.z += target.eyeheight;
    v2 += Normal(v1 - v2) * 100;
    return (Trace(hitloc, hitnormal, v1, v2, false) == None);
}

function Rotator CameraTrack(Pawn target, float DeltaTime)
{
    local float dist;
    local Vector lead;
    local float minzoomdist, maxzoomdist, viewangle, viewwidth;
    
    // update focuspoint
    lead = target.location + Vect(0,0,2) * Target.CollisionHeight; // + target.Velocity*0.5;
    dist = VSize(lead - focuspoint);
    if (dist > 20)
    {
        focuspoint += Normal(lead - focuspoint) * dist * DeltaTime * 2.0;
    }

    // adjust zoom within bounds (FovAngle 30-100)
    if (autozoom)
    {
        dist = VSize(Location - target.Location);

        if (curcam >= 0)
        {
            minzoomdist = camlist[curcam].minzoomdist;
            maxzoomdist = camlist[curcam].maxzoomdist;
            viewangle = camlist[curcam].ViewAngle; 
        }
        else
        {
            minzoomdist = 600;
            maxzoomdist = 1200;
            viewangle = default.FovAngle;
        }

        if (dist < minzoomdist)
        {
            FovAngle = viewangle;
        }
        else if (dist < maxzoomdist)
        {
            viewwidth = minzoomdist*Tan(viewangle*PI/180 / 2);
            FovAngle = Atan(viewwidth, dist) * 180/PI * 2;
        }

        DesiredFOV = FovAngle;
    }

    return Rotator(focuspoint - location);
}

function PawnDied(Pawn P)
{
	if ( Pawn != P )
		return;
    bBehindview = true;
	LastKillTime = -5.0;
	
    if (Pawn != None)
    {
        curcam = -1;
        SetLocation(FindFloatingCam(Pawn));
        SetRotation(CameraTrack(Pawn, 0));
    }

    Super.PawnDied(P);
}

function SetPawnClass(string inClass, string inCharacter)
{
    local class<xPawn> pClass;
    
    pClass = class<xPawn>(DynamicLoadObject(inClass, class'Class'));
    if (pClass != None)
        PawnClass = pClass;

    PawnSetupRecord = class'xUtil'.static.FindPlayerRecord(inCharacter);
    PlayerReplicationInfo.SetCharacterName(PawnSetupRecord.DefaultName);
}

function Possess( Pawn aPawn )
{
    local xPawn xp;

    Super.Possess( aPawn );

    xp = xPawn(aPawn);
	if(xp != None)
		xp.Setup(PawnSetupRecord, true);
}

// for changing character on the fly (for next respawn)
exec function ChangeCharacter(string newCharacter)
{
    SetPawnClass(string(PawnClass), newCharacter);
	UpdateURL("Character", newCharacter, true);
    SaveConfig();
}

simulated event PostNetReceive()
{
    local xUtil.PlayerRecord rec;

	if (PlayerReplicationInfo != None)
    {
        rec = class'xUtil'.static.FindPlayerRecord(PlayerReplicationInfo.CharacterName);
		if ( rec.Species != None )
		{
			if ( PlayerReplicationInfo.Team == None )
				rec.Species.static.LoadResources(rec, Level, PlayerReplicationInfo, 255);
			else
				rec.Species.static.LoadResources(rec, Level, PlayerReplicationInfo, PlayerReplicationInfo.Team.TeamIndex);
		}
        bNetNotify = false;
    }
}

function rotator GetAim()
{
    if (bBehindView)
        return Pawn.Rotation;
    else
        return Rotation;
}

defaultproperties
{
    bNetNotify=true
    TeamBeaconTexture=Texture'TeamSymbols.TeamBeaconT'
    LinkBeaconTexture=Texture'TeamSymbols.LinkBeaconT'
    curcam=-1
    ComboNameList(0)="XGame.ComboSpeed"
    ComboNameList(1)="XGame.ComboBerserk"
    ComboNameList(2)="XGame.ComboDefensive"
    ComboNameList(3)="XGame.ComboInvis"
    PathWhisps[0]=class'RedWhisp'
    PathWhisps[1]=class'BlueWhisp'
    PlayerReplicationInfoClass=Class'xGame.xPlayerReplicationInfo'
    PawnClass=class'xGame.xPawn'
    MinAdrenalineCost=100.0
}
