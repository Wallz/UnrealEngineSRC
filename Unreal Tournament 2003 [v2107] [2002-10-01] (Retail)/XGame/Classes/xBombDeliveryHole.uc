class xBombDeliveryHole extends Decoration
    notplaceable;

#exec OBJ LOAD File=XGame_StaticMeshes.usx

function Touch(Actor Other)
{
    local xBombFlag bomb;

    bomb = xBombFlag(Other);

    if (bomb != None)
        TryThrowScore(bomb);
    else
        TryJumpScore(Pawn(Other));
}

function bool CheckScorer(Pawn p, bool bKill)
{
	local vector NewVel;
	
    // valid player
    if (p == None || !p.IsPlayerPawn())
        return false;
		
    // opposing team delivery
    if ( (P.Controller == None)
		|| (P.Controller.PlayerReplicationInfo == None)
		|| (P.Controller.PlayerReplicationInfo.Team.TeamIndex == xBombDelivery(Owner).Team) )
    {
		if ( (Bot(P.Controller) != None) && P.Controller.IsInState('Testing') )
			return false;
			
		if ( bKill )
		{
			// propel him on through
			NewVel = 300 * Normal(P.Velocity);
			NewVel.Z = 100;
			P.AddVelocity(NewVel);
			P.Died( P.Controller, class'Suicided', P.Location );
		}
        return false;
	}
    return true;
}

function TryThrowScore(xBombFlag bomb)
{
    if ( !CheckScorer(bomb.Instigator, false) )
        return;

    // throwing score!            
    xBombingRun(Level.Game).ScoreBomb(bomb.Instigator.Controller, bomb);
    xBombDelivery(Owner).ScoreEffect(false);
}

function TryJumpScore(Pawn holder)
{
    if ( !CheckScorer(holder, true) )
        return;

    if (holder.Controller.PlayerReplicationInfo.HasFlag == None)
        return;

    // contact score!
    xBombingRun(Level.Game).ScoreBomb(holder.Controller, xBombFlag(holder.Controller.PlayerReplicationInfo.HasFlag));			
	TriggerEvent(Event,self, Holder);
    xBombDelivery(Owner).ScoreEffect(true);
}

// !! this uses a simple staticmesh for collision, a cylinder wouldn't suffice.

defaultproperties
{
	RemoteRole=ROLE_None
    bHidden=true
    bCollideActors=true   
    bStatic=false
    bNoDelete=false
    CollisionRadius=60.000000
    CollisionHeight=60.000000
    DrawScale=1.00
    DrawType=DT_StaticMesh
    StaticMesh=XGame_StaticMeshes.BombGateCol
    Style=STY_Normal
    bUseCylinderCollision=false
    LightType=LT_None
}