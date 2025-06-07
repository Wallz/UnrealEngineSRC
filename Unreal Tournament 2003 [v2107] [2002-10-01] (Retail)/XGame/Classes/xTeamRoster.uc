class xTeamRoster extends UnrealTeamInfo;

function PostBeginPlay()
{
	local array<xUtil.PlayerRecord> PlayerRecords;
	local int i,j;
	
	Super.PostBeginPlay();

	// add RosterNames to roster
	class'xUtil'.static.GetPlayerList(PlayerRecords);	
	for ( i=0; i<RosterNames.Length; i++ )
	{
		j = Roster.Length;
		Roster.Length = Roster.Length + 1;
		Roster[j] = class'xRosterEntry'.Static.CreateRosterEntryCharacter(RosterNames[i]);
	}
}	

function Initialize(int TeamBots)
{
	local int i;

	for ( i=Roster.Length; i<TeamBots; i++ )
		AddRandomPlayer();
	
	for ( i=0; i<TeamBots; i++ )
		Roster[i].PrecacheRosterFor(self);
}

function FillPlayerTeam(GameProfile G)
{
	local int i,j, limit;
	
	limit = Min (G.LINEUP_SIZE, G.GetNumTeammatesForMatch());
	for ( i=0; i<limit; i++ )
	{
		j = Roster.Length;
		Roster.Length = Roster.Length + 1;
		Roster[j] = class'xRosterEntry'.Static.CreateRosterEntryCharacter(G.PlayerTeam[G.PlayerLineup[i]]);
		Roster[j].SetOrders(G.PlayerPositions[G.PlayerLineup[i]]);
	}
	TeamSymbolName = G.TeamSymbolName;
}	

function RosterEntry GetRandomPlayer()
{
	local array<xUtil.PlayerRecord> PlayerRecords;
	local int r,i;
	
	class'xUtil'.static.GetPlayerList(PlayerRecords);	
	r = Rand(PlayerRecords.Length);
	
	if ( !AlreadyExistsEntry(PlayerRecords[r].DefaultName,false) )
		return class'xRosterEntry'.Static.CreateRosterEntry(r);
		
	for ( i=r; i<PlayerRecords.Length; i++ )
		if ( !AlreadyExistsEntry(PlayerRecords[i].DefaultName,false) )
			return class'xRosterEntry'.Static.CreateRosterEntry(i);
	
	for ( i=0; i<r; i++ )
		if ( !AlreadyExistsEntry(PlayerRecords[i].DefaultName,false) )
			return class'xRosterEntry'.Static.CreateRosterEntry(i);
	
	return class'xRosterEntry'.Static.CreateRosterEntry(r);
}

function bool AlreadyExistsEntry(string CharacterName, bool bNoRecursion)
{
	local int i;
	
	for ( i=0; i<Roster.Length; i++ )
		if ( (xRosterEntry(Roster[i]) != None) && (xRosterEntry(Roster[i]).PlayerName == CharacterName) )
			return true;
	
	if ( !bNoRecursion && UnrealTeamInfo(Level.Game.OtherTeam(self)) != None )
		return UnrealTeamInfo(Level.Game.OtherTeam(self)).AlreadyExistsEntry(CharacterName,true);		
	return false;
}

function bool BelongsOnTeam(class<Pawn> PawnClass)
{
	return true;
}

defaultproperties
{
}
