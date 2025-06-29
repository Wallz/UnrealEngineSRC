class ComboMessage extends LocalMessage;

static function string GetString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject 
	)
{
	return class<Combo>(OptionalObject).default.ExecMessage;
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

	if ( class<Combo>(OptionalObject).default.ComboAnnouncement != None )
		P.PlayAnnouncement(class<Combo>(OptionalObject).default.ComboAnnouncement,1,true);
}

defaultproperties
{
	bFadeMessage=True
	bIsUnique=True
	bBeep=False

	StackMode=SM_Down
    PosY=0.12

	DrawColor=(R=255,G=0,B=0,A=255)
	FontSize=0
}
