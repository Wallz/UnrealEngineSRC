//=============================================================================
// CTFGame.
//=============================================================================
class CTFGame extends TeamGame
	config;

function PostBeginPlay()
{
	Super.PostBeginPlay();
	SetTeamFlags();
}

function float SpawnWait(AIController B)
{
	if ( B.PlayerReplicationInfo.bOutOfLives )
		return 999;
	if ( Level.NetMode == NM_Standalone )
	{
		if ( !CTFSquadAI(Bot(B).Squad).FriendlyFlag.bHome && (Numbots <= 16) )
			return FRand();
		return ( 0.5 * FMax(2,NumBots-4) * FRand() );
	}
	return FRand();
}

function SetTeamFlags()
{
	local CTFFlag F;

	// associate flags with teams
	ForEach AllActors(Class'CTFFlag',F)
	{
		F.Team = Teams[F.TeamNum];
		F.Team.HomeBase = F.HomeBase;
		CTFTeamAI(F.Team.AI).FriendlyFlag = F;
		if ( F.TeamNum == 0 )
			CTFTeamAI(Teams[1].AI).EnemyFlag = F;
		else
			CTFTeamAI(Teams[0].AI).EnemyFlag = F;
	}
}

function Logout(Controller Exiting)
{
	if ( Exiting.PlayerReplicationInfo.HasFlag != None )
		CTFFlag(Exiting.PlayerReplicationInfo.HasFlag).Drop(vect(0,0,0));	
	Super.Logout(Exiting);
}

function bool CheckEndGame(PlayerReplicationInfo Winner, string Reason)
{
	local CTFFlag BestFlag;
	local Controller P;
	local PlayerController Player;
    local float EndTimeDelay;
    local bool bLastMan;
    
    bLastMan = ( Reason ~= "LastMan" );

	if ( (GameRulesModifiers != None) && !GameRulesModifiers.CheckEndGame(Winner, Reason) )
		return false;

	if ( bLastMan )
		GameReplicationInfo.Winner = Winner.Team;
	else
	{
		if ( Teams[1].Score == Teams[0].Score )
		{
			if ( !bOverTimeBroadcast )
			{
				StartupStage = 7;
				PlayStartupMessage();
				bOverTimeBroadcast = true;
			}
			return false;
		}		
		if ( Teams[1].Score > Teams[0].Score )
			GameReplicationInfo.Winner = Teams[1];
		else
			GameReplicationInfo.Winner = Teams[0];
	}
	
	BestFlag = CTFTeamAI(UnrealTeamInfo(GameReplicationInfo.Winner).AI).FriendlyFlag;
	EndGameFocus = BestFlag.HomeBase;
	EndGameFocus.bHidden = false;
	
    EndTimeDelay = 3.0;
	EndTime = Level.TimeSeconds + EndTimeDelay;
	for ( P=Level.ControllerList; P!=None; P=P.nextController )
	{
		P.GameHasEnded();
		Player = PlayerController(P);
		if ( Player != None )
		{
			Player.ClientSetBehindView(true);
			Player.ClientSetViewTarget(EndGameFocus);
			Player.SetViewTarget(EndGameFocus);
			PlayWinMessage(Player, (Player.PlayerReplicationInfo.Team == GameReplicationInfo.Winner));
			Player.ClientGameEnded();
			if ( CurrentGameProfile != None )
				CurrentGameProfile.bWonMatch = (Player.PlayerReplicationInfo.Team == GameReplicationInfo.Winner);
		}
	}
	BestFlag.HomeBase.bHidden = false;
	BestFlag.bHidden = true;
	return true;
}

function ScoreFlag(Controller Scorer, CTFFlag theFlag)
{
	local float Dist,oppDist;
	local int i;
	local float ppp,numtouch;
	local vector FlagLoc;

	if ( Scorer.PlayerReplicationInfo.Team == theFlag.Team )
	{
		Scorer.AwardAdrenaline(ADR_Return);
		FlagLoc = TheFlag.Position().Location;
		Dist = vsize(FlagLoc - TheFlag.HomeBase.Location);
		
		if (TheFlag.TeamNum==0)
			oppDist = vsize(FlagLoc - Teams[1].HomeBase.Location);
		else
  			oppDist = vsize(FlagLoc - Teams[0].HomeBase.Location); 
	
		if (GameStats!=None)
			GameStats.GameEvent("flag_returned",""$theFlag.Team.TeamIndex,Scorer.PlayerReplicationInfo);

		BroadcastLocalizedMessage( class'CTFMessage', 1, Scorer.PlayerReplicationInfo, None, TheFlag.Team );
		
		if (Dist>1024)
		{
			// figure out who's closer
				
			if (Dist<=oppDist)	// In your team's zone
			{
				Scorer.PlayerReplicationInfo.Score += 3;
				
				if (GameStats!=None)
					GameStats.ScoreEvent(Scorer.PlayerReplicationInfo,3,"flag_ret_friendly");
			}
			else
			{
				Scorer.PlayerReplicationInfo.Score += 5;
				
				if (GameStats!=None)
					GameStats.ScoreEvent(Scorer.PlayerReplicationInfo,5,"flag_ret_enemy");
				
				if (oppDist<=1024)	// Denial
				{
  					Scorer.PlayerReplicationInfo.Score += 7;
					if (GameStats!=None)
						GameStats.ScoreEvent(Scorer.PlayerReplicationInfo,7,"flag_denial");
				}
					
			}					
		} 
		return;
	}
	
	// Figure out Team based scoring.
	if (TheFlag.FirstTouch!=None)	// Original Player to Touch it gets 5
	{
		if (GameStats!=None)
			GameStats.ScoreEvent(TheFlag.FirstTouch.PlayerReplicationInfo,5,"flag_cap_1st_touch");

		TheFlag.FirstTouch.PlayerReplicationInfo.Score += 5;
	}
		
	// Guy who caps gets 5
	Scorer.PlayerReplicationInfo.Score += 5;
	IncrementGoalsScored(Scorer.PlayerReplicationInfo);
    Scorer.AwardAdrenaline(ADR_Goal);
	
	// Each player gets 20/x but it's guarenteed to be at least 1 point but no more than 5 points 
	numtouch=0;	
	for (i=0;i<TheFlag.Assists.length;i++)
	{
		if (TheFlag.Assists[i]!=None)
			numtouch = numtouch + 1.0;
	}
	
	ppp = FClamp(20/numtouch,1,5);
		
	for (i=0;i<TheFlag.Assists.length;i++)
	{
		if (TheFlag.Assists[i]!=None)
		{
			if (GameStats!=None)
				GameStats.ScoreEvent(TheFlag.Assists[i].PlayerReplicationInfo,ppp,"flag_cap_assist");

			TheFlag.Assists[i].PlayerReplicationInfo.Score += int(ppp);
		}
	}

	// Apply the team score
	Scorer.PlayerReplicationInfo.Team.Score += 1.0;
	if (GameStats!=None)
	{
		GameStats.ScoreEvent(Scorer.PlayerReplicationInfo,5,"flag_cap_final");
		GameStats.TeamScoreEvent(Scorer.PlayerReplicationInfo.Team.TeamIndex,1,"flag_cap");	
		GameStats.GameEvent("flag_captured",""$theflag.Team.TeamIndex,Scorer.PlayerReplicationInfo);
	}	
	BroadcastLocalizedMessage( class'CTFMessage', 0, Scorer.PlayerReplicationInfo, None, TheFlag.Team );
	AnnounceScore(Scorer.PlayerReplicationInfo.Team.TeamIndex);
	CheckScore(Scorer.PlayerReplicationInfo);

    if ( bOverTime )
    {
		EndGame(Scorer.PlayerReplicationInfo,"timelimit");
    }
}

function DiscardInventory( Pawn Other )
{
	if ( (Other.PlayerReplicationInfo != None) && (Other.PlayerReplicationInfo.HasFlag != None) )
		CTFFlag(Other.PlayerReplicationInfo.HasFlag).Drop(0.5 * Other.Velocity);
	
	Super.DiscardInventory(Other);
	
}

State MatchOver
{
	function ScoreFlag(Controller Scorer, CTFFlag theFlag)
	{
	}
}

defaultproperties
{
	 bAllowTrans=true
	 bSpawnInTeamArea=true
     bScoreTeamKills=False
	 bWeaponStay=true
	 BeaconName="CTF"
	 GameName="Capture the Flag"
     MapPrefix="CTF"
	 GoalScore=3
	 TeamAIType(0)=class'UnrealGame.CTFTeamAI'
	 TeamAIType(1)=class'UnrealGame.CTFTeamAI'
	 bScoreVictimsTarget=true
    ADR_Kill=2.0
}
