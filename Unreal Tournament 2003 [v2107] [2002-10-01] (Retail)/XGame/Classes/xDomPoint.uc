//=============================================================================
// xDomPoint.
// For Double Domination (xDoubleDom) matches.
//=============================================================================
class xDomPoint extends DominationPoint;

#exec OBJ LOAD FILE=UCGeneric.utx

var() localized String PointName; // display name of this control point
var() Sound ControlSound;	      // sound played when this control point changes hands
var() Name ControlEvent;          // any actors with tags matching this will be triggered when activity occurs on the control point

var(Material) Material DomCombiner[2];
var(Material) Material CRedState[2];
var(Material) Material CBlueState[2];
var(Material) Material CNeutralState[2];
var(Material) Material CDisableState[2];

var(Material) Shader   DomShader;
var(Material) Material SRedState;
var(Material) Material SBlueState;
var(Material) Material SNeutralState;
var(Material) Material SDisableState;

var(Material) float    PulseSpeed;
var xDOMLetter         DomLetter;
var xDOMRing           DOMRing;
var transient byte     NoPulseAlpha;
var			  vector	EffectOffset;

simulated function PostBeginPlay()
{
	Super.PostBeginPlay();      
   
    if ( !Level.Game.IsA('xDoubleDom') && (Level.NetMode != NM_Client) )
		bHidden = true;
}

function string GetHumanName()
{
	return PointName;
}

function Touch(Actor Other)
{
    if ( (Pawn(Other) == None) || !Pawn(Other).IsPlayerPawn() )
		return;	
    
    // is this domination point controllable right now?
    if (bControllable)
    {
        // touching pawn is now the controlling pawn for this domination point
        ControllingPawn = Pawn(Other);
	    
        // update the domination point's status
        UpdateStatus();
    }
}

function UnTouch(Actor Other)
{
	local Pawn NewControl;
	
	if ( Other == ControllingPawn )
		ForEach TouchingActors(class'Pawn', NewControl)
		{
			if ( NewControl.IsPlayerPawn() )
			{
				ControllingPawn = NewControl;
				UpdateStatus();
				break;
			}
		}
}

simulated function float Pulse( float x )
{
	if ( x < 0.5 )
		return 2.0 * ( x * x * (3.0 - 2.0 * x) );
	else
		return 2.0 * (1.0 - ( x * x * (3.0 - 2.0 * x) ));
}

simulated function Tick( float t )
{
    local float f;
    local float alpha;

    Super.Tick(t);

    if ( DomShader != None && PulseSpeed != 0.0)
    {
        if (bControllable)
        {
            f = Level.TimeSeconds * PulseSpeed;
	        f = f - int(f);
            alpha = 255.0;
	        ConstantColor(DomShader.SpecularityMask).Color.A = Pulse(f) * alpha;
        }
        else
        {
            ConstantColor(DomShader.SpecularityMask).Color.A = NoPulseAlpha;
        }
    }
}

simulated function PostNetReceive()
{
	local xDomMonitor M;
	local byte NewTeam;
	
    if( !bControllable )
    {
        SetShaderStatus(CDisableState[0],SDisableState,CDisableState[1]);
        NewTeam = 254;
    }
    else if ( ControllingTeam == None )
    {
        SetShaderStatus(CNeutralState[0],SNeutralState,CNeutralState[1]);
        NewTeam = 255;
    }
    else 
    {
		if ( ControllingTeam.TeamIndex == 0 )
			SetShaderStatus(CRedState[0],SRedState,CRedState[1]);
		else
			SetShaderStatus(CBlueState[0],SBlueState,CBlueState[1]);
		NewTeam = ControllingTeam.TeamIndex;
	}
	       
    // send the event to trigger related actors
    ForEach DynamicActors(class'xDomMonitor', M, ControlEvent)
    {
		M.NewTeam = NewTeam;
		M.UpdateForTeam();
	}
}

simulated function SetShaderStatus( Material mat1, Material mat2, Material mat3 )
{
    if( DomCombiner[0] != None )
        Combiner(DomCombiner[0]).Material1 = mat1;
    if( DomCombiner[1] != None )
        Combiner(DomCombiner[1]).Material1 = mat3;
    if( DomShader != None )
    {
        if (PulseSpeed != 0.0)
            DomShader.Specular = mat2;
        else
            DomShader.Diffuse = mat2;
    }
}

function UpdateStatus()
{
	local Actor A;
	local TeamInfo NewTeam;
	local int OldIndex;
    
    if ( bControllable && ((ControllingPawn == None) || !ControllingPawn.IsPlayerPawn()) )
    {
		ControllingPawn = None;
		
        // check if any pawn currently touching
		ForEach TouchingActors(class'Pawn', ControllingPawn)
		{
			if ( ControllingPawn.IsPlayerPawn() )
				break;
			else
				ControllingPawn = None;
		}
	}
	
    // nothing to do if there is already a controlling team but no controlling pawn
    if (ControllingTeam != None && ControllingPawn == None)
        return;

    // who is the current controlling team of this domination point?
    if (ControllingPawn == None)
		NewTeam = None;
	else
        NewTeam = ControllingPawn.Controller.PlayerReplicationInfo.Team;

	// do nothing if there is no change in the controlling team (and there is a controlling team)
    if ((NewTeam == ControllingTeam) && (NewTeam != None))
		return;

    // for AI, update DefenderTeamIndex
    OldIndex = DefenderTeamIndex;
	if ( NewTeam == None )
	    DefenderTeamIndex = 255; // ie. "no team" since 0 is a valid team
	else
		DefenderTeamIndex = NewTeam.TeamIndex;
    
    if ( bControllable && (OldIndex != DefenderTeamIndex) )
		TeamGame(Level.Game).FindNewObjectives(self);

	// otherwise we have a new controlling team, or the domination point is being re-enabled
    ControllingTeam = NewTeam;
    
    if (ControllingTeam != None)
	{
		PlayAlarm();
	}

    xDoubleDom(Level.Game).ResetCount();
	
	if (ControllingTeam == None)
	{
        // goes dark while untouchable (disabled) after a score
        if (!bControllable) 
		{			
            LightType = LT_None;
            SetShaderStatus(CDisableState[0],SDisableState,CDisableState[1]);
            if (DomLetter != None)
                DomLetter.bHidden = true;
            if (DomRing != None)
                DomRing.bHidden = true;
        }
        // goes back to white when neutral again
        else if (bControllable) 
		{            			
            // change light emission properties
            LightHue = 255;
            LightBrightness = 128;
		    LightSaturation = 255;
            LightType = LT_SubtlePulse;
            SetShaderStatus(CNeutralState[0],SNeutralState,CNeutralState[1]);
            if (DomLetter != None)
            {
                DomLetter.bHidden = false;
                DomLetter.Skins[0] = class'xDomLetter'.Default.NeutralShader;
                DomLetter.RepSkin = class'xDomLetter'.Default.NeutralShader;
            }
            if (DomRing != None)
            {
                DomRing.bHidden = false;
                DomRing.Skins[0] = class'xDomRing'.Default.NeutralShader;
                DomRing.RepSkin = class'xDomRing'.Default.NeutralShader;
            }
		}
	}	
	else if (ControllingPawn.Controller.PlayerReplicationInfo.Team.TeamIndex == 0)
	{
        // red team controls it now
        LightType = LT_SubtlePulse;
        LightHue = 0;
        LightBrightness = 255;
		LightSaturation = 128;
        SetShaderStatus(CRedState[0],SRedState,CRedState[1]);
        if (DomLetter != None)
        {
            DomLetter.bHidden = false;
            DomLetter.Skins[0] = class'xDomLetter'.Default.RedTeamShader;
            DomLetter.RepSkin = class'xDomLetter'.Default.RedTeamShader;
        }
        if (DomRing != None)
        {
            DomRing.bHidden = false;
            DomRing.Skins[0] = class'xDomRing'.Default.RedTeamShader;
            DomRing.RepSkin = class'xDomRing'.Default.RedTeamShader;
        }
	}
	else
	{
        // blue team controls it now            
        LightType = LT_SubtlePulse;
        LightHue = 170;
        LightBrightness = 255;
		LightSaturation = 128;
        SetShaderStatus(CBlueState[0],SBlueState,CBlueState[1]);
        if (DomLetter != None)
        {
            DomLetter.bHidden = false;
            DomLetter.Skins[0] = class'xDomLetter'.Default.BlueTeamShader;
            DomLetter.RepSkin = class'xDomLetter'.Default.BlueTeamShader;
        }
        if (DomRing != None)
        {
            DomRing.bHidden = false;
            DomRing.Skins[0] = class'xDomRing'.Default.BlueTeamShader;
            DomRing.RepSkin = class'xDomRing'.Default.BlueTeamShader;
        }
	}
    
    // send the event to trigger related actors
    foreach DynamicActors(class'Actor', A, ControlEvent)
        A.Trigger(self, ControllingPawn);
}

function ResetPoint(bool enabled)
{
	if ( !bControllable && enabled )
		TeamGame(Level.Game).FindNewObjectives(self);

    bControllable = enabled;
    ControllingPawn = None;
    ControllingTeam = None;
    UpdateStatus();
}

function PlayAlarm()
{
	SetTimer(5.0, false);
	AmbientSound = ControlSound;
}

function Timer()
{
	AmbientSound = None;

    // don't call super here since we don't want it incrementing score!
}

function bool BetterObjectiveThan(GameObjective Best, byte DesiredTeamNum, byte RequesterTeamNum)
{
	if ( (Best == None) || (DefenderTeamIndex == DesiredTeamNum) )
		return true;
	return false;
}

defaultproperties
{
	PrePivot=(X=0.0,Y=0.0,Z=70.0)
	bUseCylinderCollision=true
	RemoteRole=ROLE_SimulatedProxy
	bAlwaysRelevant=true
    bHidden=false
    bStasis=false
    bStatic=false
	bNoDelete=true
	bCollideActors=true
    bNetNotify=true
    bControllable=true    
    bTeamControlled=true
    bDynamicLight=true
    CollisionRadius=60.000000
    CollisionHeight=40.000000
    DrawScale=0.60000
    DrawType=DT_StaticMesh
    StaticMesh=XGame_rc.DominationPointMesh
    Skins(0)=Texture'XGameTextures.DominationPointTex'            
	Skins(1)=XGameShaders.DomShaders.DomPointACombiner
    Style=STY_Normal	
    LightType=LT_SubtlePulse
	LightEffect=LE_QuadraticNonIncidence
	LightRadius=6
    LightBrightness=128	
    LightHue=255
    LightSaturation=255
    SoundRadius=255
    SoundVolume=255
    ControlSound=sound'GameSounds.DDAlarm'	
    DomCombiner(0)=XGameShaders.DomShaders.DomACombiner
    CRedState(0)=Texture'UCGeneric.SolidColors.Red'
    CBlueState(0)=Texture'UCGeneric.SolidColors.Blue'
    CNeutralState(0)=Texture'UCGeneric.SolidColors.White'
    CDisableState(0)=Texture'UCGeneric.SolidColors.Black'
    DomShader=XGameShaders.DomShaders.PulseAShader
    SRedState=Texture'XGameShaders.DomShaders.RedGrid'
    SBlueState=XGameShaders.DomShaders.BlueGrid
    SNeutralState=XGameShaders.DomShaders.GreyGrid
    SDisableState=XGameShaders.DomShaders.GreyGrid    
    PulseSpeed=1.0
    NoPulseAlpha=128
    DestructionMessage="" 
    DefenderTeamIndex=255
    EffectOffset=(X=0.0,Y=0.0,Z=60.0)
}
