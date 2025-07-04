class xRosterEntry extends RosterEntry
	    dependsOn(xUtil);

var() xUtil.PlayerRecord PlrProfile;

static function xRosterEntry CreateRosterEntry(int prIdx)
{
    local xRosterEntry xre;
    local xUtil.PlayerRecord pr;

    pr = class'xUtil'.static.GetPlayerRecord(prIdx);

    xre = new(None) class'xRosterEntry';
    xre.PlayerName = pr.DefaultName;
    xre.PawnClassName = "xGame.xPawn"; 
    xre.PlrProfile = pr;
    xre.Init();

    return xre;
}

static function xRosterEntry CreateRosterEntryCharacter(string CharName)
{
    local xRosterEntry xre;
    local xUtil.PlayerRecord pr;

    pr = class'xUtil'.static.FindPlayerRecord(CharName);

    xre = new(None) class'xRosterEntry';
    xre.PlayerName = pr.DefaultName;
    xre.PawnClassName = "xGame.xPawn"; 
    xre.PlrProfile = pr;
    xre.Init();

    return xre;
}

function PrecacheRosterFor(UnrealTeamInfo T)
{
     if ( PlrProfile.Species == None )
    {
		warn("Could not load species "$PlrProfile.Species$" for "$PlrProfile.DefaultName);
		return;
	}

	PlrProfile.Species.static.LoadResources( PlrProfile, T.Level, None, T.TeamIndex );
}

function InitBot(Bot B)
{
    B.SetPawnClass(PawnClassName, PlayerName);
		
    // Set bot attributes based on the PlayerRecord
    CombatStyle    = FClamp(class'Bot'.Default.CombatStyle + float(PlrProfile.CombatStyle),-1,1);    
    Aggressiveness = FClamp(class'Bot'.Default.BaseAggressiveness +float(PlrProfile.Aggressiveness),0,1);
    Accuracy        = FClamp(float(PlrProfile.Accuracy),-2,2);
    StrafingAbility = FClamp(float(PlrProfile.StrafingAbility),-2,2);
    Tactics         = FClamp(float(PlrProfile.Tactics),-2,2);
    if ( PlrProfile.FavoriteWeapon == "" )
		FavoriteWeapon = None;
	else
	    FavoriteWeapon = class<Weapon>(DynamicLoadObject(PlrProfile.FavoriteWeapon,class'Class'));
    bJumpy = bool(PlrProfile.bJumpy);
    
   	Super.InitBot(B);
}

defaultproperties
{
}