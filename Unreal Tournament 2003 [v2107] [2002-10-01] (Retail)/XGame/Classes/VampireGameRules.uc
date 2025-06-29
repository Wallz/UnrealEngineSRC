class VampireGameRules extends GameRules;

var() float ConversionRatio;
		
function int NetDamage( int OriginalDamage, int Damage, pawn injured, pawn instigatedBy, vector HitLocation, vector Momentum, class<DamageType> DamageType )
{
	if ( NextGameRules != None )
		return NextGameRules.NetDamage( OriginalDamage,Damage,injured,instigatedBy,HitLocation,Momentum,DamageType );
	if ( (InstigatedBy != Injured) && (InstigatedBy != None) && (InstigatedBy.Health > 0) 
		&& (InstigatedBy.PlayerReplicationInfo != None) && (Injured.PlayerReplicationInfo != None) 
		&& ((InstigatedBy.PlayerReplicationInfo.Team == None) || (InstigatedBy.PlayerReplicationInfo.Team != Injured.PlayerReplicationInfo.Team)) )
		InstigatedBy.Health = Min( InstigatedBy.Health+Damage*ConversionRatio, Max(InstigatedBy.Health,InstigatedBy.HealthMax) );
	return Damage;
}

defaultproperties
{
    ConversionRatio=0.5
}