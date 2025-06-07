class TournamentHealth extends TournamentPickUp
	abstract;

var() int HealingAmount;
var() bool bSuperHeal;

/* DetourWeight()
value of this path to take a quick detour (usually 0, used when on route to distant objective, but want to grab inventory for example)
*/
function float DetourWeight(Pawn Other,float PathWeight)
{
	local int Heal;
	
	if ( AIController(Other.Controller).PriorityObjective() && (Other.Health > 65) )
		return 0;
	Heal = Min(GetHealMax(Other),Other.Health + HealingAmount) - Other.Health;
	return (0.02 * Heal * MaxDesireability)/PathWeight;
}

event float BotDesireability(Pawn Bot)
{
	local float desire;
	local int HealMax;

	HealMax = GetHealMax(Bot);
	desire = Min(HealingAmount, HealMax - Bot.Health);

	if ( (Bot.Weapon != None) && (Bot.Weapon.AIRating > 0.5) )
		desire *= 1.7;
	if ( bSuperHeal || (Bot.Health < 45) )
		return ( FMin(0.03 * desire, 2.2) );
	else
	{
		if ( desire > 6 )
			desire = FMax(desire,25);
		else if ( Bot.Controller.bHuntPlayer )
			return 0;
		return ( FMin(0.017 * desire, 2.0) ); 
	}
}

static function string GetLocalString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2
	)
{
	return Default.PickupMessage$Default.HealingAmount;
}

function int GetHealMax(Pawn P)
{
	local int HealMax;
	
	HealMax = P.HealthMax;
	if (bSuperHeal) 
		HealMax = Min(199, HealMax * 2.0);
	return HealMax;
}

auto state Pickup
{	
	function Touch( actor Other )
	{
		local Pawn P;
			
		if ( ValidTouch(Other) ) 
		{
			P = Pawn(Other);
            if (P.GiveHealth(HealingAmount, GetHealMax(P)))
            {
				AnnouncePickup(P);
                SetRespawn();
            }
		}
	}
}

defaultproperties
{
     HealingAmount=20
     PickupMessage="You picked up a Health Pack +"
     RespawnTime=30.000000
     MaxDesireability=0.700000
     RemoteRole=ROLE_DumbProxy
     AmbientGlow=255
     CollisionRadius=32.000000
     CollisionHeight=23.000000
     Mass=10.000000
}
