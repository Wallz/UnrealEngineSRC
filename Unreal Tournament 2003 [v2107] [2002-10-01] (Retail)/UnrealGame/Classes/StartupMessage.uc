class StartupMessage extends CriticalEventPlus;

var localized string Stage[8], NotReady, SinglePlayer;
var sound	Riff;

static simulated function ClientReceive( 
	PlayerController P,
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	Super.ClientReceive(P, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject);
	if ( Switch < 7 )
	{
		P.Level.FillPrecacheMaterialsArray();
		P.Level.FillPrecacheStaticMeshesArray();
	}		
	// don't play sound if quickstart=true, so no 'play' voiceover at start of tutorials
	if ( Switch == 5 && P != none && P.Level != none && P.Level.Game != none && (!P.Level.Game.IsA('Deathmatch') || !DeathMatch(P.Level.Game).bQuickstart) )
		P.PlayAnnouncement(sound'AnnouncerMain.Play',1,true);
	else if ( (Switch > 1) && (Switch < 5) )
		P.PlayBeepSound();
	else if ( Switch == 7 )
		P.ClientPlaySound(Default.Riff);
}

static function string GetString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	if ( (RelatedPRI_1 != None) && (RelatedPRI_1.Level.NetMode == NM_Standalone) )
	{
		if ( Switch < 2 )
			return Default.SinglePlayer;
	}	
	else if ( switch == 1 )
	{
		if ( (RelatedPRI_1 == None) || !RelatedPRI_1.bWaitingPlayer )
			return Default.Stage[0];
		else if ( RelatedPRI_1.bReadyToPlay )
			return Default.Stage[1];
		else
			return Default.NotReady;
	}
	return Default.Stage[Switch];
}

defaultproperties
{
	bIsUnique=true
	bBeep=False
	DrawColor=(R=32,G=64,B=255)
	Stage(0)="Waiting for other players."
	Stage(1)="Waiting for ready signals. You are READY."
	Stage(2)="The match is about to begin...3"
	Stage(3)="The match is about to begin...2"
	Stage(4)="The match is about to begin...1"
	Stage(5)="The match has begun!"
	Stage(6)="The match has begun!"
	Stage(7)="OVER TIME!"
	NotReady="You are NOT READY. Press Fire!"
	SinglePlayer="Press FIRE to start!"
	
    Riff=sound'GameSounds.UT2K3Fanfare11'
}