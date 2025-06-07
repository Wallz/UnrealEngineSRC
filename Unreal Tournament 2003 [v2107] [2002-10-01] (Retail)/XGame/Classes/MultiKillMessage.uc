class MultiKillMessage extends LocalMessage;

#exec OBJ LOAD File=AnnouncerMain.uax

var	localized string 	KillString[7];
var sound		KillSound[7];

static function string GetString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject 
	)
{
	if ( class'PlayerController'.default.bNoMatureLanguage )
		return Default.KillString[Min(Switch,6)-1]; 

	else
		return Default.KillString[Min(Switch,7)-1]; 
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
	if ( class'PlayerController'.default.bNoMatureLanguage )
		P.PlayAnnouncement(Default.KillSound[Min(Switch-1,5)],1,true);
	else
		P.PlayAnnouncement(Default.KillSound[Min(Switch-1,6)],1,true);
}

static function int GetFontSize( int Switch, PlayerReplicationInfo RelatedPRI1, PlayerReplicationInfo RelatedPRI2, PlayerReplicationInfo LocalPlayer )
{
	if ( Switch < 4 )
		return 0;
	if ( Switch == 4 )
		return 1;
	if ( Switch == 7 )
		return 3;
	return 2;
}

defaultproperties
{
	KillString(0)="Double Kill!"
	KillString(1)="Multi Kill!"
	KillString(2)="Mega Kill!!"
	KillString(3)="ULTRA KILL!!"
	KillString(4)="M O N S T E R  K I L L !!!"
	KillString(5)="L U D I C R O U S !!!"
	KillString(6)="H O L Y  S H I T !"
	KillSound(0)=sound'AnnouncerMain.Double_Kill'
	KillSound(1)=sound'AnnouncerMain.MultiKill'
	KillSound(2)=sound'AnnouncerMain.MegaKill'
	KillSound(3)=sound'AnnouncerMain.UltraKill'
	KillSound(4)=sound'AnnouncerMain.MonsterKill_F'
	KillSound(5)=sound'AnnouncerMain.LudicrousKill_F'
	KillSound(6)=sound'AnnouncerMain.HolyShit_F'
	bFadeMessage=True
	bIsSpecial=True
	bIsUnique=True
	Lifetime=3
	bBeep=False

	DrawColor=(R=255,G=0,B=0)
	FontSize=1

	StackMode=SM_Down
    PosY=0.25

}
