class xJuggGibGroup extends xPawnGibGroup;

static function class<xEmitter> GetBloodEmitClass()
{
	if ( FRand() < 0.4 )
        return class'BotSparks';
    
	if ( class'GameInfo'.static.UseLowGore() )
 		return Default.LowGoreBloodEmitClass;
    return Default.BloodEmitClass;
}

defaultproperties
{
}
