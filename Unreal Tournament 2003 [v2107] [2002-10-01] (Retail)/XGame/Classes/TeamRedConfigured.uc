class TeamRedConfigured extends xTeamRoster;

/* this class used for configured instant action or multiplayer games with bots
*/
var config array<string> Characters;

function Initialize(int TeamBots)
{
	local int i;
	
	for ( i=0; i<Roster.Length; i++ )
		Roster[i].PrecacheRosterFor(self);
}

function int OverrideInitialBots(int N, UnrealTeamInfo T)
{
	return Roster.Length + T.Roster.Length;
}

function bool AllBotsSpawned()
{
	return (Size >= Roster.Length);
}

function PostBeginPlay()
{
	local int i;
	
	for ( i=0; i<Characters.Length; i++ )
		RosterNames[i] = Characters[i];
	Super.PostBeginPlay();
}

defaultproperties
{
}
