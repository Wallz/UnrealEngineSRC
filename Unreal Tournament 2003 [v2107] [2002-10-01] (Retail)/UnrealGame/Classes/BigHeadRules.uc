class BigHeadRules extends GameRules;

var MutBigHead BigHeadMutator;

function ScoreKill(Controller Killer, Controller Killed)
{
	if ( (Killer != None) && (Killer.Pawn != None) )
		Killer.Pawn.SetHeadScale(BigHeadMutator.GetHeadScaleFor(Killer.Pawn));

	if ( NextGameRules != None )
		NextGameRules.ScoreKill(Killer,Killed);
}