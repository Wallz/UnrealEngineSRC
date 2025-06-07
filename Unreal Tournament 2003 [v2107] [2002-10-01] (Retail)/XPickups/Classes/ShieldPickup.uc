//=============================================================================
// ShieldPickup - cut and paste from TournamentHealth
//=============================================================================
class ShieldPickup extends TournamentPickUp
	abstract;

var() int ShieldAmount;

/* DetourWeight()
value of this path to take a quick detour (usually 0, used when on route to distant objective, but want to grab inventory for example)
*/
function float DetourWeight(Pawn Other,float PathWeight)
{
	local float Need;
	
	Need = Other.GetShieldStrengthMax() - Other.GetShieldStrength();
	if ( Need <= 0 )
		return 0;
	if ( AIController(Other.Controller).PriorityObjective() && (Need < 0.4 * Other.GetShieldStrengthMax()) )
		return 0; 
	return (0.013 * MaxDesireability * Min(ShieldAmount, Need));
}

event float BotDesireability(Pawn Bot)
{
	return (0.013 * MaxDesireability * Min(ShieldAmount, Bot.GetShieldStrengthMax() - Bot.GetShieldStrength()));
}

static function string GetLocalString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2
	)
{
	return Default.PickupMessage$Default.ShieldAmount;
}

auto state Pickup
{	
	function Touch( actor Other )
	{
        local Pawn P;
			
		if ( ValidTouch(Other) ) 
		{			
			P = Pawn(Other);
            if (P.AddShieldStrength(ShieldAmount))
            {
				AnnouncePickup(P);
                SetRespawn();
            }
		}
	}
}

defaultproperties
{
    ShieldAmount=20
    PickupMessage="You picked up a Shield Pack +"
    RespawnTime=30.000000
    MaxDesireability=1.500000
    RemoteRole=ROLE_DumbProxy
    AmbientGlow=64
    CollisionRadius=22.000000
    CollisionHeight=23.000000
    Mass=10.000000
}
