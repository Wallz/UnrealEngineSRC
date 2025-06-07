/* BossDM
designed for 1 vs 1 DM against boss bot that tries to stay just a little better than you
*/

class BossDM extends xDeathMatch;

var float BaseDifficulty;

event InitGame( string Options, out string Error )
{
    Super.InitGame(Options, Error);
    BaseDifficulty = GameDifficulty;
	bAdjustSkill = true;
}

function AdjustSkill(AIController B, PlayerController P, bool bWinner)
{
	if ( B.PlayerReplicationInfo.Score <= P.PlayerReplicationInfo.Score + 1 )
	{
		if ( bWinner )
			return;
		else 
			AdjustedDifficulty += 0.5;
	}
	else if ( bWinner )
	{
		Bot(B).Accuracy = 0;
		AdjustedDifficulty = FMax(BaseDifficulty-1, AdjustedDifficulty - 0.4);
		AdjustedDifficulty = FMax(AdjustedDifficulty,0);
	}
		
    B.Skill = AdjustedDifficulty;
}


defaultproperties
{
	GameName="Championship Match"
	bAdjustSkill=true
}