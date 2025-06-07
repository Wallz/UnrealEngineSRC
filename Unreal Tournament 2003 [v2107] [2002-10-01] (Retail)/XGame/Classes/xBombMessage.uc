//=============================================================================
// BombMessage.
//=============================================================================
class xBombMessage extends CriticalEventPlus;

var(Message) localized string ReturnBlue, ReturnRed;
var(Message) localized string ReturnedBlue, ReturnedRed;
var(Message) localized string CaptureBlue, CaptureRed;
var(Message) localized string DroppedBlue, DroppedRed;
var(Message) localized string HasBlue,HasRed;

var sound	ReturnSounds[2];
var sound	DroppedSounds[2];
var Sound	TakenSounds[2];
var sound	Riffs[3];

static simulated function ClientReceive( 
	PlayerController P,
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	if ( Switch == 3 )
		P.PlayAnnouncement(default.ReturnSounds[0],1, true);

	Super.ClientReceive(P, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject);
	if ( TeamInfo(OptionalObject) == None )
		return;

	switch (Switch)
	{
		case 0:
			P.ClientPlaySound(Default.Riffs[Rand(3)]);
			break;
		// Returned the flag.
		case 1:
		case 3:
		case 5:
			P.PlayAnnouncement(default.ReturnSounds[TeamInfo(OptionalObject).TeamIndex],1, true);
			break;

		// Dropped the flag.
		case 2:
			P.PlayAnnouncement(default.DroppedSounds[TeamInfo(OptionalObject).TeamIndex],2, true);
			break;
		case 4:
		case 6:
			P.PlayAnnouncement(default.TakenSounds[TeamInfo(OptionalObject).TeamIndex],2, true);
			break;
	}
}

static function string GetString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	if ( TeamInfo(OptionalObject) == None )
		return "";
	switch (Switch)
	{
		// Captured the flag.
		case 0:
			if (RelatedPRI_1 == None)
				return "";

			if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
				return RelatedPRI_1.PlayerName@Default.CaptureRed;
			else
				return RelatedPRI_1.PlayerName@Default.CaptureBlue;
			break;

		// Returned the flag.
		case 1:
			if (RelatedPRI_1 == None)
			{
				if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
					return Default.ReturnedRed;
				else
					return Default.ReturnedBlue;
			}
			if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
				return RelatedPRI_1.PlayerName@Default.ReturnRed;
			else
				return RelatedPRI_1.playername@Default.ReturnBlue;
			break;

		// Dropped the flag.
		case 2:
			if (RelatedPRI_1 == None)
				return "";

			if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
				return RelatedPRI_1.playername@Default.DroppedRed;
			else
				return RelatedPRI_1.playername@Default.DroppedBlue;
			break;

		// Was returned.
		case 3:
			if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
				return Default.ReturnedRed;
			else
				return Default.ReturnedBlue;
			break;

		// Has the flag.
		case 4:
			if (RelatedPRI_1 == None)
				return "";
			if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
				return RelatedPRI_1.playername@Default.HasRed;
			else
				return RelatedPRI_1.playername@Default.HasBlue;
			break;

		// Auto send home.
		case 5:
			if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
				return Default.ReturnedRed;
			else
				return Default.ReturnedBlue;
			break;

		// Pickup
		case 6:
			if (RelatedPRI_1 == None)
				return "";
			if ( TeamInfo(OptionalObject).TeamIndex == 0 ) 
				return RelatedPRI_1.playername@Default.HasRed;
			else
				return RelatedPRI_1.playername@Default.HasBlue;
			break;
	}
	return "";
}

defaultproperties
{
	ReturnBlue="returns the ball!" 
	ReturnRed="returns the ball!"
	ReturnedBlue="The ball was returned!"
	ReturnedRed="The ball was returned!"
	CaptureBlue="delivered the ball!"
	CaptureRed="delivered the ball!"
	DroppedBlue="dropped the ball!"
	DroppedRed="dropped the ball!"
	HasRed="has the ball!"
	HasBlue="has the ball!"

    ReturnSounds(0)=Sound'AnnouncerMain.BallReset'
    ReturnSounds(1)=Sound'AnnouncerMain.BallReset'
    DroppedSounds(0)=Sound'AnnouncerMain.Red_Pass_Fumbled'
    DroppedSounds(1)=Sound'AnnouncerMain.Blue_Pass_Fumbled'
	TakenSounds(0)=Sound'AnnouncerMain.Red_Team_on_Offence'
	TakenSounds(1)=Sound'AnnouncerMain.Blue_Team_on_Offence'

    Riffs(0)=sound'GameSounds.UT2K3Fanfare03'
    Riffs(1)=sound'GameSounds.UT2K3Fanfare07'
    Riffs(2)=sound'GameSounds.UT2K3Fanfare08'
    
	bIsUnique=True
    FontSize=1
	StackMode=SM_Down
    PosY=0.12
}