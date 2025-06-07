class UDamageTimer extends Info;

function Timer()
{
	if ( Pawn(Owner) != None )
		Pawn(Owner).DisableUDamage();
	Destroy();
}
