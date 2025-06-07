class GameMessage extends LocalMessage;

var(Message) localized string SwitchLevelMessage;
var(Message) localized string LeftMessage;
var(Message) localized string FailedTeamMessage;
var(Message) localized string FailedPlaceMessage;
var(Message) localized string FailedSpawnMessage;
var(Message) localized string EnteredMessage;
var(Message) localized string MaxedOutMessage;
var(Message) localized string OvertimeMessage;
var(Message) localized string GlobalNameChange;
var(Message) localized string NewTeamMessage;
var(Message) localized string NewTeamMessageTrailer;
var(Message) localized string NoNameChange;
var(Message) localized string VoteStarted;
var(Message) localized string VotePassed;
var(Message) localized string MustHaveStats;

var localized string NewPlayerMessage;

//
// Messages common to GameInfo derivatives.
//
static function string GetString(
    optional int Switch,
    optional PlayerReplicationInfo RelatedPRI_1, 
    optional PlayerReplicationInfo RelatedPRI_2,
    optional Object OptionalObject
    )
{
    switch (Switch)
    {
        case 0:
            return Default.OverTimeMessage;
            break;
        case 1:
            if (RelatedPRI_1 == None)
                return Default.NewPlayerMessage;

            return RelatedPRI_1.playername$Default.EnteredMessage;
            break;
        case 2:
            if (RelatedPRI_1 == None)
                return "";

            return RelatedPRI_1.OldName@Default.GlobalNameChange@RelatedPRI_1.PlayerName;
            break;
        case 3:
            if (RelatedPRI_1 == None)
                return "";
            if (OptionalObject == None)
                return "";

            return RelatedPRI_1.playername@Default.NewTeamMessage@TeamInfo(OptionalObject).GetHumanReadableName()$Default.NewTeamMessageTrailer;
            break;
        case 4:
            if (RelatedPRI_1 == None)
                return "";

            return RelatedPRI_1.playername$Default.LeftMessage;
            break;
        case 5:
            return Default.SwitchLevelMessage;
            break;
        case 6:
            return Default.FailedTeamMessage;
            break;
        case 7:
            return Default.MaxedOutMessage;
            break;
        case 8:
            return Default.NoNameChange;
            break;
        case 9:
            return RelatedPRI_1.playername@Default.VoteStarted;
            break;
        case 10:
            return Default.VotePassed;
            break;
        case 11:
			return Default.MustHaveStats;
			break;
    }
    return "";
}

defaultproperties
{
	NewPlayerMessage="A new player entered the game."
    OverTimeMessage="Score tied at the end of regulation. Sudden Death Overtime!!!"
    GlobalNameChange="changed name to"
    NewTeamMessage="is now on"
    NewTeamMessageTrailer=""
    SwitchLevelMessage="Switching Levels"
    MaxedOutMessage="Server is already at capacity."
    EnteredMessage=" entered the game."
    FailedTeamMessage="Could not find team for player"
    FailedPlaceMessage="Could not find a starting spot"
    FailedSpawnMessage="Could not spawn player"
    LeftMessage=" left the game."
    NoNameChange="Name is already in use."
    MustHaveStats="Must have stats enabled to join this server."
    VoteStarted="started a vote."
    VotePassed="Vote passed."
    bIsSpecial=false
	bIsConsoleMessage=true
}