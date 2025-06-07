class DamTypeFlakChunk extends WeaponDamageType
	abstract;

var sound FlakMonkey;

static function IncrementKills(Controller Killer)
{
	local xPlayerReplicationInfo xPRI;
	
	xPRI = xPlayerReplicationInfo(Killer.PlayerReplicationInfo);
	if ( xPRI != None )
	{
		xPRI.flakcount++;
		if ( (xPRI.flakcount == 15) && (UnrealPlayer(Killer) != None) )
			UnrealPlayer(Killer).ClientDelayedAnnouncement(Default.FlakMonkey,15);
	}
}		

defaultproperties
{
	FlakMonkey=sound'AnnouncerMain.FlackMonkey'
    DeathString="%o was shredded by %k's flak cannon."
	MaleSuicide="%o was perforated by his own flak."
	FemaleSuicide="%o perforated by her own flak."

    WeaponClass=class'FlakCannon'
}

