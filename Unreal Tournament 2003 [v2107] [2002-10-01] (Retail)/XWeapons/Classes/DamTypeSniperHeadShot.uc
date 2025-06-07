class DamTypeSniperHeadShot extends WeaponDamageType
	abstract;

var class<LocalMessage> KillerMessage;
var sound HeadHunter;

static function IncrementKills(Controller Killer)
{
	local xPlayerReplicationInfo xPRI;
	
	if ( PlayerController(Killer) == None )
		return;
		
	PlayerController(Killer).ReceiveLocalizedMessage( Default.KillerMessage, 0, Killer.PlayerReplicationInfo, None, None );
	xPRI = xPlayerReplicationInfo(Killer.PlayerReplicationInfo);
	if ( xPRI != None )
	{
		xPRI.headcount++;
		if ( (xPRI.headcount == 15) && (UnrealPlayer(Killer) != None) )
			UnrealPlayer(Killer).ClientDelayedAnnouncement(Default.HeadHunter,15);
	}
}		

static function GetHitEffects(out class<xEmitter> HitEffects[4], int VictemHealth )
{
    HitEffects[0] = class'HitSmoke';
    HitEffects[1] = class'HitFlameBig';
}

defaultproperties
{
	HeadHunter=sound'AnnouncerMain.HeadHunter'
    DeathString="%o's cranium was made extra crispy by %k's lightning gun."
	MaleSuicide="%o violated the laws of space-time and sniped himself."
	FemaleSuicide="%o violated the laws of space-time and sniped herself."

    DamageOverlayMaterial=Material'XGameShaders.PlayerShaders.LightningHit'
    DamageOverlayTime=1.0

    bAlwaysSevers=true
    bSpecial=true

    WeaponClass=class'SniperRifle'
    KillerMessage=class'SpecialKillMessage'
    
    bCauseConvulsions=true
}

