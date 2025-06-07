//
// Switch is the note.
// RelatedPRI_1 is the player on the spree.
//
class KillingSpreeMessage extends CriticalEventPlus;

var(Messages)	localized string EndSpreeNote, EndSelfSpree, EndFemaleSpree, MultiKillString;
var(Messages)	localized string SpreeNote[10];
var(Messages)	localized string SelfSpreeNote[10];
var(Messages)	sound SpreeSound[10];
var(Messages)	localized string EndSpreeNoteTrailer;
 
static function int GetFontSize( int Switch, PlayerReplicationInfo RelatedPRI1, PlayerReplicationInfo RelatedPRI2, PlayerReplicationInfo LocalPlayer )
{
	if ( (LocalPlayer == RelatedPRI1) && (RelatedPRI2 == None) )
		return 2;
	return Default.FontSize;
}

static function string GetRelatedString(
    optional int Switch,
    optional PlayerReplicationInfo RelatedPRI_1, 
    optional PlayerReplicationInfo RelatedPRI_2,
    optional Object OptionalObject
    )
{
	if ( RelatedPRI_2 == None )
		return Default.SelfSpreeNote[Switch];
		
    return static.GetString(Switch,RelatedPRI_1,RelatedPRI_2,OptionalObject);
}

static function string GetString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	if (RelatedPRI_2 == None)
	{
		if (RelatedPRI_1 == None)
			return "";

		if (RelatedPRI_1.PlayerName != "")
			return RelatedPRI_1.PlayerName@Default.SpreeNote[Switch];
	} 
	else 
	{
		if (RelatedPRI_1 == None)
		{
			if (RelatedPRI_2.PlayerName != "")
			{
				if ( RelatedPRI_2.bIsFemale )
					return RelatedPRI_2.PlayerName@Default.EndFemaleSpree;
				else
					return RelatedPRI_2.PlayerName@Default.EndSelfSpree;
			}
		} 
		else 
		{
			return RelatedPRI_1.PlayerName$Default.EndSpreeNote@RelatedPRI_2.PlayerName@Default.EndSpreeNoteTrailer;
		}
	}
	return "";
}

static simulated function ClientReceive( 
	PlayerController P,
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	Super.ClientReceive(P, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject);

	if (RelatedPRI_2 != None)
		return;

	if (RelatedPRI_1 != P.PlayerReplicationInfo)
		P.PlayBeepSound();

	if ( RelatedPRI_1 == P.PlayerReplicationInfo )
		P.PlayAnnouncement(Default.SpreeSound[Switch],1,true);
}

defaultproperties
{
	 FontSize=0
	 bBeep=False
	 EndSpreeNote="'s killing spree ended by"
	 EndSpreeNoteTrailer=""
	 EndSelfSpree="was looking good till he killed himself!"
	 EndFemaleSpree="was looking good till she killed herself!"
	 SpreeNote(0)="is on a killing spree!"
	 SpreeNote(1)="is on a rampage!"
	 SpreeNote(2)="is dominating!"
	 SpreeNote(3)="is unstoppable!"
	 SpreeNote(4)="is Godlike!"
	 SpreeNote(5)="is Wicked SICK!"
	 SelfSpreeNote(0)="Killing Spree!"
	 SelfSpreeNote(1)="Rampage!"
	 SelfSpreeNote(2)="Dominating!"
	 SelfSpreeNote(3)="Unstoppable!"
	 SelfSpreeNote(4)="GODLIKE!"
	 SelfSpreeNote(5)="WICKED SICK!"
	 SpreeSound(0)=sound'AnnouncerMain.Killing_Spree'
	 SpreeSound(1)=sound'AnnouncerMain.Rampage'
	 SpreeSound(2)=sound'AnnouncerMain.Dominating'
	 SpreeSound(3)=sound'AnnouncerMain.Unstoppable'
	 SpreeSound(4)=sound'AnnouncerMain.GodLike'
	 SpreeSound(5)=sound'AnnouncerMain.WhickedSick'
}