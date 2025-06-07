class DamTypeShockCombo extends WeaponDamageType
	abstract;

var sound ComboWhore;

static function GetHitEffects(out class<xEmitter> HitEffects[4], int VictemHealth )
{
    HitEffects[0] = class'HitSmoke';
}

static function IncrementKills(Controller Killer)
{
	local xPlayerReplicationInfo xPRI;
	
	xPRI = xPlayerReplicationInfo(Killer.PlayerReplicationInfo);
	if ( xPRI != None )
	{
		xPRI.combocount++;
		if ( (xPRI.combocount == 15) && (UnrealPlayer(Killer) != None) )
			UnrealPlayer(Killer).ClientDelayedAnnouncement(Default.ComboWhore,15);
	}
}		

defaultproperties
{
	ComboWhore=sound'AnnouncerMain.ComboWhore'
    DeathString="%o couldn't avoid the blast from %k's shock combo."
	MaleSuicide="%o made a tactical error with his shock combo."
	FemaleSuicide="%o made a tactical error with her shock combo."

    bAlwaysSevers=true

    WeaponClass=class'ShockRifle'
    bDetonatesGoop=true
}

