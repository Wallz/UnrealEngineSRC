//=============================================================================
// xDoubleDom
//=============================================================================
class xDoubleDom extends TeamGame
	config;

// todo: doesn't handle the case where controllingpawn leaves the game or is killed (when it awards score, could be dangling...)

var   xDomPoint xDomPoints[2];              // the two domination points in the level
var   int ScoreCountDown;                   // count down until a point is scored
var   transient int DisabledCountDown;      // count down until points are re-enabled after a point is scored
var() Sound ControlSounds[2];               // control sounds
var() Sound ScoreSounds[2];                 // score sounds
var() Sound AvertedSounds[4];               // score averted sounds
var() config int TimeToScore;               // duration both points must be controlled for to score a point
var() config int TimeDisabled;              // duration both points are disabled for after a point is scored
var sound NewRoundSound;

// mc - localized PlayInfo descriptions & extra info
var private localized string DDomPropsDisplayText[2];

static function PrecacheGameTextures(LevelInfo myLevel)
{
	class'xTeamGame'.static.PrecacheGameTextures(myLevel);
	
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMpointGREY');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMpointLINESg');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMPointABg');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMPointABr');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMpointLINESr');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMpointRED');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.RedScreenA');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMPointABb');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMpointLINESb');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.DOMpointBLUE');
	myLevel.AddPrecacheMaterial(Material'XEffects.RedMarker_T');
	myLevel.AddPrecacheMaterial(Material'XEffects.BlueMarker_T');
}

static function PrecacheGameStaticMeshes(LevelInfo myLevel)
{
	class'xDeathMatch'.static.PrecacheGameStaticMeshes(myLevel);
	
	myLevel.AddPrecacheStaticMesh(StaticMesh'XGame_rc.DomRing');
	myLevel.AddPrecacheStaticMesh(StaticMesh'XGame_rc.DomAMesh');
	myLevel.AddPrecacheStaticMesh(StaticMesh'XGame_rc.DomBMesh');
}

function PostBeginPlay()
{
	local NavigationPoint N;
	local int i;

	Super.PostBeginPlay();
    
    // find the first two domination points in the level
	i = 0;
	for(N = Level.NavigationPointList; N != None; N = N.NextNavigationPoint)
	{
		if (N.IsA('xDomPoint'))
		{
			xDomPoints[i] = xDomPoint(N);
			i++;
			if (i == 2) break;
		}
	}

    if (i != 2)
    {
        // there were not enough domination points in the level!
		log("xDoubleDom: Level has wrong number of Domination Points!",'Error');
	}
}

State MatchInProgress
{
	function Timer()
	{
		local Controller TeamMate;
		local int i;
		
		Super.Timer();
		
		if (bGameEnded || xDomPoints[0] == None || xDomPoints[1] == None)
			return;
		
		// are both points currently disabled?
		if (DisabledCountDown > 0)
		{
			DisabledCountDown--;
            if ( DisabledCountDown == 7 )
            {
                for ( TeamMate = Level.ControllerList; TeamMate != None; TeamMate = TeamMate.NextController )
                    if ( PlayerController(TeamMate) != None )
						PlayerController(TeamMate).PlayAnnouncement(NewRoundSound,1,true);
			}		
	        else if ( (DisabledCountDown > 0) && (DisabledCountDown < 6) )
				BroadcastLocalizedMessage(class'TimerMessage', DisabledCountDown);
        
			if (DisabledCountDown == 0)
			{                    
				xDomPoints[0].ResetPoint(true);
				xDomPoints[1].ResetPoint(true);
			}

		}            
        
        // nothing more to do unless the same team controls both points
        if ( (xDomPoints[0].ControllingTeam != xDomPoints[1].ControllingTeam) || (xDomPoints[0].ControllingTeam == None) )
            return;
            
		if (ScoreCountDown == TimeToScore)
		{				
		    for (TeamMate = Level.ControllerList; TeamMate != None; TeamMate = TeamMate.NextController)
            {
			    if (TeamMate.IsA('PlayerController'))
				    PlayerController(TeamMate).PlayAnnouncement(ControlSounds[xDomPoints[0].ControllingTeam.TeamIndex],1,true);
        	}
		}        

		if (ScoreCountDown > 0 && ScoreCountDown < 9)
		{
		    for (TeamMate = Level.ControllerList; TeamMate != None; TeamMate = TeamMate.NextController)
            {
			    BroadcastLocalizedMessage(class'TimerMessage', ScoreCountDown );
        	}			
		}

		if (ScoreCountDown > 0)
        {
            // decrement the time remaining until a point is scored
	        ScoreCountDown--;
            return;
        }
        
		for (TeamMate = Level.ControllerList; TeamMate != None; TeamMate = TeamMate.NextController)
        {
			if (TeamMate.IsA('PlayerController'))
			{
				PlayerController(TeamMate).PlayAnnouncement(ScoreSounds[xDomPoints[0].ControllingTeam.TeamIndex],1,true);
				PlayerController(TeamMate).ClientPlaySound(class'CTFMessage'.Default.Riffs[Rand(3)]);
			}
        }			

		// controlling team has scored!					
        xDomPoints[0].ControllingTeam.Score += 1.0;
			
		if (GameStats!=None)
			GameStats.TeamScoreEvent(xDomPoints[0].ControllingTeam.TeamIndex,1,"dom_teamscore");
        // award scoring players some Adrenaline and increase their score
		if ( (xDomPoints[0].ControllingPawn != None) && (xDomPoints[0].ControllingPawn.Controller != None) )
        {
            xDomPoints[0].ControllingPawn.Controller.PlayerReplicationInfo.Score += 5.0;
		    xDomPoints[0].ControllingPawn.Controller.AwardAdrenaline(ADR_Goal);

			if (GameStats!=None)
				GameStats.ScoreEvent(xDomPoints[0].ControllingPawn.Controller.PlayerReplicationInfo,5,"dom_score");
        }

		if ( (xDomPoints[1].ControllingPawn != None) && (xDomPoints[1].ControllingPawn.Controller != None) )
        {
            xDomPoints[1].ControllingPawn.Controller.PlayerReplicationInfo.Score += 5.0;
		    xDomPoints[1].ControllingPawn.Controller.AwardAdrenaline(ADR_Goal);

			if (GameStats!=None)
				GameStats.ScoreEvent(xDomPoints[1].ControllingPawn.Controller.PlayerReplicationInfo,5,"dom_score");

        }
        
		// reset count
        ScoreCountDown = TimeToScore;
        
		// make all control points be untouchable for a short period
		DisabledCountDown = TimeDisabled;

		// reset both domination points and disable them
		xDomPoints[0].ResetPoint(false);
		xDomPoints[1].ResetPoint(false);
        
        // check if the game is over because either team has achieved the goal limit
        if ( GoalScore > 0 )
        {
		    for (i = 0; i < 2; i++ )
            {
			    if ( Teams[i].Score >= GoalScore )
                {
				    EndGame(None,"teamscorelimit");
                }
            }
        }
    }
}

function ClearControl(Controller Other)
{
    local Controller P;
	local PlayerController Player;    
    local Pawn Pick;

	if ((PlayerController(Other) == None) || (Other.PlayerReplicationInfo.Team.TeamIndex == 255))
		return;

	if ((xDomPoints[0].ControllingPawn != Other) || (xDomPoints[0].ControllingPawn != Other))
        return;
    
    // find a suitable teammate
	for(P = Level.ControllerList; P != None; P = P.nextController)
	{
		Player = PlayerController(P);

		if (Player != None && (Player != Other) && (Player.PlayerReplicationInfo.Team == Other.PlayerReplicationInfo.Team))
		{
			Pick = Player.Pawn;
            break;
		}
	}	    
    
    // update both control points with the new controlling pawn, since the other one is leaving (if no other suitable
    // teammate was found, then the point will return to neutral)
    if (xDomPoints[0].ControllingPawn == Other)
	{
		xDomPoints[0].ControllingPawn = Pick;
		xDomPoints[0].UpdateStatus();
	}

	if (xDomPoints[1].ControllingPawn == Other)
	{
		xDomPoints[1].ControllingPawn = Pick;
		xDomPoints[1].UpdateStatus();
	}
}

function Logout( Controller Exiting )
{
    ClearControl(Exiting);	
    Super.Logout(Exiting);
}

function ResetCount()
{
    local Sound Aversion;
	local Controller TeamMate;
    
    if ( ScoreCountDown < TimeToScore )
    {
        Aversion = AvertedSounds[1];
        if (TimeToScore - ScoreCountDown > (TimeToScore/2))
        {
            if (TimeToScore - ScoreCountDown >= (TimeToScore-1))
            {
                // within one second of a score
                Aversion = AvertedSounds[3];
            }
            else
            {
                // halfway to a score
                Aversion = AvertedSounds[2];
            }
        }
        else if ( TimeToScore - ScoreCountDown < 2 )
			Aversion = AvertedSounds[0];
        
        for (TeamMate = Level.ControllerList; TeamMate != None; TeamMate = TeamMate.NextController)
        {
			if (TeamMate.IsA('PlayerController'))
                PlayerController(TeamMate).PlayAnnouncement(Aversion,2,true);
        }			
    }
    
    ScoreCountDown = TimeToScore;
}

function bool CriticalPlayer(Controller Other)
{
	// In DOM, critical players are anyone within 1024 units of the dom point
	// who have los to it.
	
	if ( (vsize(Other.Pawn.Location - xDomPoints[0].Location) <=1024) || 
	     (vsize(Other.Pawn.Location - xDomPoints[1].Location) <=1024) )
		return true; 

	return Super.CriticalPlayer(Other);
}

function Killed( Controller Killer, Controller Killed, Pawn KilledPawn, class<DamageType> damageType )
{   
	if ( (Killer != None) && Killer.bIsPlayer && Killed.bIsPlayer && 
         (Killer.PlayerReplicationInfo.Team != Killed.PlayerReplicationInfo.Team) 
         && (xPawn(Killer.Pawn) != None) && (DisabledCountDown > 0))
    {
        Killer.AwardAdrenaline(ADR_MinorBonus);
    }

	Super.Killed(Killer, Killed, KilledPawn, damageType);
}

static function FillPlayInfo(PlayInfo PlayInfo)
{
	local int i;

	Super.FillPlayInfo(PlayInfo);  // Always begin with calling parent

	PlayInfo.AddSetting("Rules", "TimeToScore",  default.DDomPropsDisplayText[i++], 1, 60, "Text", "3;0:60");
	PlayInfo.AddSetting("Rules", "TimeDisabled", default.DDomPropsDisplayText[i++], 1, 70, "Text", "3;0:60");
}

defaultproperties
{
	HUDType="XInterface.HudBDoubleDomination"
    MapListType="XInterface.MapListDoubleDomination"
    GameName="Double Domination"
	GoalScore=3
    ScoreCountDown=10
    TimeToScore=10
    TimeDisabled=10
    MapPrefix="DOM"
    BeaconName="DOM"	     	
    bScoreTeamKills=false
	bSpawnInTeamArea=true
    bTeamScoreRounds=false
    ADR_Kill=2.0
	DefaultEnemyRosterClass="xGame.xTeamRoster"
   
    // sounds for this game type
	ControlSounds(0)=Sound'AnnouncerMain.Red_Team_Dominating'
    ControlSounds(1)=Sound'AnnouncerMain.Blue_Team_Dominating'
    ScoreSounds(0)=Sound'AnnouncerMain.Red_Team_Scores'
    ScoreSounds(1)=Sound'AnnouncerMain.Blue_Team_Scores'
    AvertedSounds(0)=Sound'GameSounds.DDAverted'
    AvertedSounds(1)=Sound'AnnouncerMain.Averted'
    AvertedSounds(2)=Sound'AnnouncerMain.Narrowly_Averted'
    AvertedSounds(3)=Sound'AnnouncerMain.Last_Second_Save'
    DeathMessageClass=class'XGame.xDeathMessage'
    
    TeamAIType(0)=class'UnrealGame.DOMTeamAI'
    TeamAIType(1)=class'UnrealGame.DOMTeamAI'

    ScreenShotName="MapThumbnails.ShotDoubleDom"
    DecoTextName="XGame.DoubleDom"
	NewRoundSound=sound'AnnouncerMain.NewRoundIn'

    Acronym="DOM2"
	OtherMesgGroup="DoubleDom"

    DDomPropsDisplayText[0]="Time To Score"
    DDomPropsDisplayText[1]="Time Disabled"
}
