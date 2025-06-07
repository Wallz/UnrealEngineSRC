//=============================================================================
// xBombingRun.
//=============================================================================
class xBombingRun extends TeamGame
	config;

#exec OBJ LOAD FILE=E_Pickups.usx

var() int   ResetCountDown;
var() int   ResetTimeDelay;

var transient int TeamSpawnCount[2];
var xBombFlag Bomb;

var sound NewRoundSound;
var GameObjective HomeBase, EnemyBase, BombBase;
var float OldScore;

function PostBeginPlay()
{
	local xBombFlag B;

	Super.PostBeginPlay();

	// associate flags with teams
	ForEach AllActors(Class'xBombFlag',B)
	{
		BombingRunTeamAI(Teams[0].AI).Bomb = B;
		BombingRunTeamAI(Teams[1].AI).Bomb = B;
    }
	SetTeamBases();
}

static function PrecacheGameTextures(LevelInfo myLevel)
{
	class'xTeamGame'.static.PrecacheGameTextures(myLevel);
	
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.BallLauncherTex0');
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.BallLauncherEnergy');
	myLevel.AddPrecacheMaterial(Material'WeaponSkins.BallLauncherLine');
	
	myLevel.AddPrecacheMaterial(Material'XEffects.RedMarker_T');
	myLevel.AddPrecacheMaterial(Material'XEffects.BlueMarker_T');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.BombIconBlue');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.BombIconRed');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.BombIconYELLOW');
	myLevel.AddPrecacheMaterial(Material'XGameShaders.BRBall');
	myLevel.AddPrecacheMaterial(Material'XGameTextures.BombDeliveryTex');
}

static function PrecacheGameStaticMeshes(LevelInfo myLevel)
{
	class'xDeathMatch'.static.PrecacheGameStaticMeshes(myLevel);
	
	myLevel.AddPrecacheStaticMesh(StaticMesh'XGame_rc.BombSpawnMesh');
	myLevel.AddPrecacheStaticMesh(StaticMesh'XGame_rc.BombEffectMesh');
	myLevel.AddPrecacheStaticMesh(StaticMesh'E_Pickups.FullBomb');
}

function SetTeamBases()	// Important for tracking
{
	local xBombDelivery B;

	// associate flags with teams
	ForEach AllActors(Class'xBombDelivery',B)
	{
		Teams[B.Team].HomeBase = B;
	}
}

function Logout(Controller Exiting)
{
	if ( xBombFlag(Exiting.PlayerReplicationInfo.HasFlag) != None )
		xBombFlag(Exiting.PlayerReplicationInfo.HasFlag).Drop(vect(0,0,0));
	Super.Logout(Exiting);
}

function DiscardInventory( Pawn Other )
{
	if ( (Other.PlayerReplicationInfo != None) && (Other.PlayerReplicationInfo.HasFlag != None) )
        xBombFlag(Other.PlayerReplicationInfo.HasFlag).Drop(0.5 * Other.Velocity);
	Super.DiscardInventory(Other);
}

function ScoreBomb(Controller Scorer, xBombFlag theFlag)
{
    local bool ThrowingScore;
	local int i;
	local float ppp,numtouch,maxpoints,maxper;
	local controller C;
	
    Bomb = theFlag;

    if( ResetCountDown > 0 )
    {
        //log("Ignoring score during reset countdown.",'BombingRun');
        theFlag.SendHome();
        return;
    }

	// blow up all redeemer guided warheads
	for ( C=Level.ControllerList; C!=None; C=C.NextController )
		if ( (C.Pawn != None) && C.Pawn.IsA('RedeemerWarhead') )
			C.Pawn.Fire(0);
  
    // are we dealing with a throwing score?
    if (Scorer.PlayerReplicationInfo.HasFlag == None)
        ThrowingScore = true;
    if ( (Scorer.Pawn != None) && Scorer.Pawn.Weapon.IsA('BallLauncher') )
		Scorer.ClientSwitchToBestWeapon();
	
	theFlag.Instigator = none;	// jmw - need this to stop the reentering of ScoreBomb due to touch with the base
    
    // awards for scoring
	IncrementGoalsScored(Scorer.PlayerReplicationInfo);
	OldScore = Scorer.PlayerReplicationInfo.Team.Score;
    if (ThrowingScore)
	{
        Scorer.PlayerReplicationInfo.Team.Score += 3.0;
		if (GameStats!=None)
			GameStats.TeamScoreEvent(Scorer.PlayerReplicationInfo.Team.TeamIndex,3,"ball_tossed");
			
		// Individual points				
			
		Scorer.PlayerReplicationInfo.Score += 2; // Just for scoring
		MaxPoints=10;
		MaxPer=2;

		if (GameStats!=None)
			GameStats.ScoreEvent(Scorer.PlayerReplicationInfo,5,"ball_thrown_final");
			
	}
    else
	{
  		Scorer.PlayerReplicationInfo.Team.Score += 7.0;
		if (GameStats!=None)
			GameStats.TeamScoreEvent(Scorer.PlayerReplicationInfo.Team.TeamIndex,7,"ball_carried");

		// Individual points				
			
		Scorer.PlayerReplicationInfo.Score += 5; // Just for scoring
		MaxPoints=20;
		MaxPer=5;

		if (GameStats!=None)
			GameStats.ScoreEvent(Scorer.PlayerReplicationInfo,5,"ball_cap_final");
	}

	// Each player gets MaxPoints/x but it's guarenteed to be at least 1 point but no more than MaxPer points 
	numtouch=0;	
	for (i=0;i<TheFlag.Assists.length;i++)
	{
		if (TheFlag.Assists[i]!=None)
			numtouch = numtouch + 1.0;
	}
	
	ppp = MaxPoints / numtouch;
	if (ppp<1.0)
		ppp = 1.0;

	if (ppp>MaxPer)
		ppp = MaxPer;
		
	for (i=0;i<TheFlag.Assists.length;i++)
	{
		if (TheFlag.Assists[i]!=None)
		{
			if (GameStats!=None)
				GameStats.ScoreEvent(TheFlag.Assists[i].PlayerReplicationInfo,ppp,"ball_score_assist");
			TheFlag.Assists[i].PlayerReplicationInfo.Score += int(ppp);
		}
	}
	
	if (TheFlag.FirstTouch!=None)	// Original Player to Touch it gets 5
	{
		if (GameStats!=None)
			GameStats.ScoreEvent(Scorer.PlayerReplicationInfo,2,"ball_score_1st_touch");

		TheFlag.FirstTouch.PlayerReplicationInfo.Score += 2;
	}
    
    Scorer.AwardAdrenaline(ADR_Goal);
	AnnounceScore(Scorer.PlayerReplicationInfo.Team.TeamIndex);

    if ( (bOverTime || (GoalScore != 0)) && (Teams[Scorer.PlayerReplicationInfo.Team.TeamIndex].Score >= GoalScore) )
		EndGame(Scorer.PlayerReplicationInfo,"teamscorelimit");
	else if ( bOverTime )
		EndGame(Scorer.PlayerReplicationInfo,"timelimit");
		
    if ( bGameEnded )
        theFlag.Score();
    else
    {
        ResetCountDown = ResetTimeDelay+1;
        theFlag.SendHomeDisabled(ResetTimeDelay);
    }
}

State MatchInProgress
{
	function Timer()
	{
        local Controller C;
        local TranslocatorBeacon Trans;

        Super.Timer();
		
        if (ResetCountDown > 0)
        {
            ResetCountDown--;
            if ( ResetCountDown < 3 )
            {
				// blow up all redeemer guided warheads
				for ( C=Level.ControllerList; C!=None; C=C.NextController )
					if ( (C.Pawn != None) && C.Pawn.IsA('RedeemerWarhead') )
						C.Pawn.Fire(1);
			}         
            if ( ResetCountDown == 8 )
            {
                for ( C = Level.ControllerList; C != None; C = C.NextController )
                    if ( PlayerController(C) != None )
						PlayerController(C).PlayAnnouncement(NewRoundSound,1,true);
			}		
	        else if ( (ResetCountDown > 1) && (ResetCountDown < 7) )
				BroadcastLocalizedMessage(class'TimerMessage', ResetCountDown-1);
            else if (ResetCountDown == 1)
            {
                // reset all players position and rotation on the field for the next round
                for ( C = Level.ControllerList; C != None; C = C.NextController )
    	        {	
   					C.StartSpot = FindPlayerStart(C, C.PlayerReplicationInfo.Team.TeamIndex);
    				if ( C.StartSpot != None )
    				{
						C.SetLocation(C.StartSpot.Location);
						C.SetRotation(C.StartSpot.Rotation);
					}
                    if ( C.Pawn != None )
                    {
						if ( xPawn(C.Pawn) != None )
						{
							if (xPawn(C.Pawn).CurrentCombo != None)
							{
								C.Adrenaline = 0;
								xPawn(C.Pawn).CurrentCombo.Destroy();
							}
							if ( xPawn(C.Pawn).UDamageTimer != None )
							{
								xPawn(C.Pawn).UDamageTimer.Destroy();
								xPawn(C.Pawn).DisableUDamage();
							}
						}
						C.Pawn.Health = Max(C.Pawn.Health,C.Pawn.Default.Health);
						Level.Game.SetPlayerDefaults(C.Pawn);
						C.Pawn.SetLocation(C.StartSpot.Location);
						C.Pawn.SetRotation(C.StartSpot.Rotation);
						C.Pawn.Velocity = vect(0,0,0);
						C.Pawn.PlayTeleportEffect(false, true);
					}
    				if ( C.StartSpot != None )
						C.ClientSetLocation(C.StartSpot.Location,C.StartSpot.Rotation);
     	        }
				
                // destroy translocator beacons
                foreach DynamicActors( class'TranslocatorBeacon', Trans )
                    Trans.Destroy();
				
                Bomb.SendHome();
                ResetCountDown = 0;
           }
        }
        else
        {
			if ( Bomb == None )
				ForEach DynamicActors(class'xBombFlag',Bomb)
					break;
			if ( Bomb != None )
				GameReplicationInfo.FlagPos = Bomb.Position().Location;
		}
    }
}

function AnnounceScore(int ScoringTeam)
{
	local Controller C;
	local sound ScoreSound;
	local int OtherTeam;
	
	if ( ScoringTeam == 1 )
		OtherTeam = 0;
	else
		OtherTeam = 1;
		
	if ( OldScore <= Teams[OtherTeam].Score ) 
	{
		if ( Teams[ScoringTeam].Score > Teams[OtherTeam].Score )
			ScoreSound = TakeLead[ScoringTeam];
		else
			ScoreSound = CaptureSound[ScoringTeam];
	}
	else
	{
		if ( OldScore <= Teams[OtherTeam].Score + 3 )
			ScoreSound = IncreaseLead[ScoringTeam];
		else
			ScoreSound = CaptureSound[ScoringTeam];
	}
			
	for ( C=Level.ControllerList; C!=None; C=C.NextController )
	{
		if ( C.IsA('PlayerController') )
			PlayerController(C).PlayAnnouncement(ScoreSound,1,true);
	}
}

defaultproperties
{
	bAllowTrans=true
    bScoreTeamKills=False
    bWeaponStay=true
    bSpawnInTeamArea=true
    BeaconName="BR"
    GameName="Bombing Run"
    HUDType="XInterface.HudBBombingRun"
    GoalScore=15
    MapPrefix="BR"
    MapListType="XInterface.MapListBombingRun"
    ADR_Kill=2.0
	DefaultEnemyRosterClass="xGame.xTeamRoster"
	
    DeathMessageClass=class'XGame.xDeathMessage'
    ResetTimeDelay=11

    TeamAIType(0)=class'UnrealGame.BombingRunTeamAI'
    TeamAIType(1)=class'UnrealGame.BombingRunTeamAI'
    bTeamScoreRounds=false

    ScreenShotName="MapThumbnails.ShotBombingRun"
    DecoTextName="XGame.BombingRun"

	NewRoundSound=sound'AnnouncerMain.NewRoundIn'
    Acronym="BR"

	OtherMesgGroup="BombingRun"
}
