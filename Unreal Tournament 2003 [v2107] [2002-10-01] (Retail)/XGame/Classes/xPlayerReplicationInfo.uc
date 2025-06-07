class xPlayerReplicationInfo extends TeamPlayerReplicationInfo;

var int flakcount,combocount,headcount;
var xUtil.PlayerRecord Rec;

var bool bForceNoPlayerLights;
var bool bNoTeamSkins;

replication
{
	reliable if( bNetInitial && (Role == ROLE_Authority) )
		bForceNoPlayerLights, bNoTeamSkins;
}

simulated function UpdatePrecacheMaterials()
{
	if ( CharacterName == "" )
		return;
    rec = class'xUtil'.static.FindPlayerRecord(CharacterName);
	if ( rec.Species != None )
	{
		if ( Team == None )
			rec.Species.static.LoadResources(rec, Level,self,255);
		else
			rec.Species.static.LoadResources(rec, Level,self,Team.TeamIndex);
	}
}

simulated function SetCharacterName(string S)
{
	Super.SetCharacterName(S);
	UpdateCharacter();
}

simulated event UpdateCharacter()
{
    Rec = class'xUtil'.static.FindPlayerRecord(CharacterName);
}

simulated function material GetPortrait()
{
	return Rec.Portrait;
}
