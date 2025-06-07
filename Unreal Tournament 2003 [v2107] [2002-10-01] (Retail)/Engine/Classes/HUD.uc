class Hud extends Actor
    native
    config(user)
    transient
    exportstructs;

var() PlayerController PlayerOwner;
var() Pawn PawnOwner;
var() PlayerReplicationInfo PawnOwnerPRI;
var() Console PlayerConsole;

var() Scoreboard ScoreBoard;

// mini hud-menus
var() Actor VoteMenu;		// hook for mod authors
var() bool bShowVoteMenu;

var color WhiteColor, RedColor, GreenColor, CyanColor, BlueColor, GoldColor, PurpleColor, TurqColor, GrayColor;

var() config bool bHideHUD;
var() bool bShowScoreBoard;             // Display current score-board instead of Hud elements
var() bool bShowDebugInfo;              // if true, show properties of current ViewTarget
var() bool bShowBadConnectionAlert;     // Display indication of bad connection
var() globalconfig bool bMessageBeep;
var() globalconfig bool bShowWeaponInfo;
var() globalconfig bool bShowPersonalInfo;
var() globalconfig bool bShowPoints;
var() globalconfig bool bShowWeaponBar;
var() globalconfig bool bCrosshairShow;

var() Color ConsoleColor;

var() Font ProgressFont;
var() float ProgressFadeTime;
var() Color MOTDColor;

var() globalconfig float HudScale;          // Global Scale for all widgets
var() globalconfig float HudCanvasScale;    // Specifies amount of screen-space to use (for TV's).
var() globalconfig int CrosshairStyle;
var() globalconfig float CrosshairScale;
var() globalconfig float CrosshairOpacity;

var transient float ResScaleX, ResScaleY;

const ConsoleMessageCount = 4;

struct ConsoleMessage
{
	var string Text;
	var color TextColor;
	var float MessageLife;
	var PlayerReplicationInfo PRI;
};
var ConsoleMessage TextMessages[ConsoleMessageCount];

var() float ConsoleMessagePosX, ConsoleMessagePosY; // DP_LowerLeft

var string FontNames[9];
var Font FontArray[9];
var int FontScreenWidthMedium[9];
var int FontScreenWidthSmall[9];

/* Draw3DLine()
draw line in world space. Should be used when engine calls RenderWorldOverlays() event.
*/
native final function Draw3DLine(vector Start, vector End, color LineColor);

simulated event PostBeginPlay()
{
	local font f;
    Super.PostBeginPlay();
    LinkActors ();
    CreateKeyMenus();
	
	f = Font(DynamicLoadObject("UT2003Fonts.FontEurostile12",class'Font'));
	if (f!=None)
		ProgressFont = f;
	else
		log("#### BAH");
	
}

/* Reset() 
reset actor to initial state - used when restarting level without reloading.
*/
function Reset()
{
	bShowVoteMenu = false;
	bShowScoreboard = false;
	Super.Reset();
}

simulated function CreateKeyMenus(); 

simulated event Destroyed()
{
    if( ScoreBoard != None )
    {
        ScoreBoard.Destroy();
        ScoreBoard = None;
    }

    if( VoteMenu != None )
    {
        VoteMenu.Destroy();
        VoteMenu = None;
    }


    Super.Destroyed();
}


//=============================================================================
// Execs

/* toggles displaying scoreboard
*/
exec function ShowScores()
{
    bShowScoreboard = !bShowScoreboard;
}

/* toggles displaying properties of player's current viewtarget
*/
exec function ShowDebug()
{
    bShowDebugInfo = !bShowDebugInfo;
}

simulated event WorldSpaceOverlays()
{
    if ( bShowDebugInfo && Pawn(PlayerOwner.ViewTarget) != None )
        DrawRoute();
}

function CheckCountdown(GameReplicationInfo GRI);

event ConnectFailure(string FailCode, string URL)
{
	PlayerOwner.ReceiveLocalizedMessage(class'FailedConnect', class'FailedConnect'.Static.GetFailSwitch(FailCode));
}

simulated event PostRender( canvas Canvas )
{
    local float XPos, YPos;

    LinkActors();

    ResScaleX = Canvas.SizeX / 640.0;
    ResScaleY = Canvas.SizeY / 480.0;

	CheckCountDown(PlayerOwner.GameReplicationInfo);
		
    if ( !PlayerOwner.bBehindView && (PawnOwner != None) && (PawnOwner.Weapon != None) )
		PawnOwner.Weapon.RenderOverlays(Canvas);

	if ( PawnOwner != None && PawnOwner.bSpecialHUD )
		PawnOwner.DrawHud(Canvas);
    if ( bShowDebugInfo )
    {
        Canvas.Font = GetConsoleFont(Canvas);
        Canvas.Style = ERenderStyle.STY_Alpha;
        Canvas.DrawColor = ConsoleColor;

        PlayerOwner.ViewTarget.DisplayDebug (Canvas, XPos, YPos);
    }
    else if( !bHideHud )
    {
        if (bShowScoreBoard)
        {
            if (ScoreBoard != None)
            {
                ScoreBoard.DrawScoreboard(Canvas);
				if ( Scoreboard.bDisplayMessages )
					DisplayMessages(Canvas);
			}
        }
        else
        {
            if ( (PlayerOwner == None) || (PawnOwner == None) || (PawnOwnerPRI == None) || PlayerOwner.IsSpectating() )
                DrawSpectatingHud(Canvas);
            else if( !PawnOwner.bHideRegularHUD )
                DrawHud(Canvas);

            if (!DrawLevelAction (Canvas))
            {
                if ((PlayerOwner != None) && (PlayerOwner.ProgressTimeOut > Level.TimeSeconds))
                    DisplayProgressMessages (Canvas);
            }

            if (bShowBadConnectionAlert)
                DisplayBadConnectionAlert (Canvas);
            DisplayMessages(Canvas);
        }

        if( bShowVoteMenu && VoteMenu!=None )
            VoteMenu.RenderOverlays(Canvas);
    }
    else if ( PawnOwner != None )
        DrawInstructionGfx(Canvas); 

    PlayerOwner.RenderOverlays (Canvas);

    if ((PlayerConsole != None) && PlayerConsole.bTyping)
        DrawTypingPrompt(Canvas, PlayerConsole.TypedStr);
}

simulated function DrawInstructionGfx( Canvas C );
simulated function SetInstructionText( string text );
simulated function SetInstructionKeyText( string text );

simulated function DrawRoute()
{
    local int i;
    local Controller C;
    local vector Start, End, RealStart;;
    local bool bPath;

    C = Pawn(PlayerOwner.ViewTarget).Controller;
    if ( C == None )
        return;
    if ( C.CurrentPath != None )
        Start = C.CurrentPath.Start.Location;
    else
        Start = PlayerOwner.ViewTarget.Location;
    RealStart = Start;

    if ( C.bAdjusting )
    {
        Draw3DLine(C.Pawn.Location, C.AdjustLoc, class'Canvas'.Static.MakeColor(255,0,255));
        Start = C.AdjustLoc;
    }

    // show where pawn is going
    if ( (C == PlayerOwner)
        || (C.MoveTarget == C.RouteCache[0]) && (C.MoveTarget != None) )
    {
        if ( (C == PlayerOwner) && (C.Destination != vect(0,0,0)) )
        {
            if ( C.PointReachable(C.Destination) )
            {
                Draw3DLine(C.Pawn.Location, C.Destination, class'Canvas'.Static.MakeColor(255,255,255));
                return;
            }
            C.FindPathTo(C.Destination);
        }
        for ( i=0; i<16; i++ )
        {
            if ( C.RouteCache[i] == None )
                break;
            bPath = true;
            Draw3DLine(Start,C.RouteCache[i].Location,class'Canvas'.Static.MakeColor(0,255,0));
            Start = C.RouteCache[i].Location;
        }
        if ( bPath )
            Draw3DLine(RealStart,C.Destination,class'Canvas'.Static.MakeColor(255,255,255));
    }
    else if ( PlayerOwner.ViewTarget.Velocity != vect(0,0,0) )
        Draw3DLine(RealStart,C.Destination,class'Canvas'.Static.MakeColor(255,255,255));

    if ( C == PlayerOwner )
        return;

    // show where pawn is looking
    if ( C.Focus != None )
        End = C.Focus.Location;
    else
        End = C.FocalPoint;
    Draw3DLine(PlayerOwner.ViewTarget.Location + Pawn(PlayerOwner.ViewTarget).BaseEyeHeight * vect(0,0,1),End,class'Canvas'.Static.MakeColor(255,0,0));
}

simulated function DisplayProgressMessages (Canvas C)
{
    local int i, LineCount;
    local GameReplicationInfo GRI;
    local float FontDX, FontDY;
    local float X, Y;
    local int Alpha;
    local float TimeLeft;
    
    TimeLeft = PlayerOwner.ProgressTimeOut - Level.TimeSeconds;
    
    if( TimeLeft >= ProgressFadeTime )
        Alpha = 255;
    else
        Alpha = (255 * TimeLeft) / ProgressFadeTime;

    GRI = PlayerOwner.GameReplicationInfo;

    LineCount = 0;

    for (i = 0; i < ArrayCount (PlayerOwner.ProgressMessage); i++)
    {
        if (PlayerOwner.ProgressMessage[i] == "")
            continue;

        LineCount++;
    }

    if (GRI != None)
    {
        if( GRI.MOTDLine1 != "" ) LineCount++;
        if( GRI.MOTDLine2 != "" ) LineCount++;
        if( GRI.MOTDLine3 != "" ) LineCount++;
        if( GRI.MOTDLine4 != "" ) LineCount++;
    }

    C.Font = ProgressFont;

    C.Style = ERenderStyle.STY_Alpha;   

    C.TextSize ("A", FontDX, FontDY);

    X = (0.5 * HudCanvasScale * C.SizeX) + (((1.0 - HudCanvasScale) / 2.0) * C.SizeX);
    Y = (0.5 * HudCanvasScale * C.SizeY) + (((1.0 - HudCanvasScale) / 2.0) * C.SizeY);

    Y -= FontDY * (float (LineCount) / 2.0);

    for (i = 0; i < ArrayCount (PlayerOwner.ProgressMessage); i++)
    {
        if (PlayerOwner.ProgressMessage[i] == "")
            continue;

        C.DrawColor = PlayerOwner.ProgressColor[i];
        C.DrawColor.A = Alpha;

        C.TextSize (PlayerOwner.ProgressMessage[i], FontDX, FontDY);
        C.SetPos (X - (FontDX / 2.0), Y);
        C.DrawText (PlayerOwner.ProgressMessage[i]);

        Y += FontDY;
    }

    if( (GRI != None) && (Level.NetMode != NM_StandAlone) )
    {
        C.DrawColor = MOTDColor;
        C.DrawColor.A = Alpha;

        if( GRI.MOTDLine1 != "" )
        {
            C.TextSize (GRI.MOTDLine1, FontDX, FontDY);
            C.SetPos (X - (FontDX / 2.0), Y);
            C.DrawText (GRI.MOTDLine1);
            Y += FontDY;
        }

        if( GRI.MOTDLine2 != "" )
        {
            C.TextSize (GRI.MOTDLine2, FontDX, FontDY);
            C.SetPos (X - (FontDX / 2.0), Y);
            C.DrawText (GRI.MOTDLine2);
            Y += FontDY;
        }

        if( GRI.MOTDLine3 != "" )
        {
            C.TextSize (GRI.MOTDLine3, FontDX, FontDY);
            C.SetPos (X - (FontDX / 2.0), Y);
            C.DrawText (GRI.MOTDLine3);
            Y += FontDY;
        }
    
        if( GRI.MOTDLine4 != "" )
        {
            C.TextSize (GRI.MOTDLine4, FontDX, FontDY);
            C.SetPos (X - (FontDX / 2.0), Y);
            C.DrawText (GRI.MOTDLine4);
            Y += FontDY;
        }
    }
}

function DrawHud (Canvas C);
function DrawSpectatingHud (Canvas C);
function bool DrawLevelAction (Canvas C);

/* DisplayBadConnectionAlert()
Warn user that net connection is bad
*/
function DisplayBadConnectionAlert (Canvas C);

simulated function LocalizedMessage( class<LocalMessage> Message, optional int Switch, optional PlayerReplicationInfo RelatedPRI_1, optional PlayerReplicationInfo RelatedPRI_2, optional Object OptionalObject, optional string CriticalString );

simulated function DrawTypingPrompt (Canvas C, String Text)
{
    local float XPos, YPos;
    local float XL, YL;

    C.Font = GetConsoleFont(C);
    C.Style = ERenderStyle.STY_Alpha;
    C.DrawColor = ConsoleColor;

    C.TextSize ("A", XL, YL);

    XPos = (ConsoleMessagePosX * HudCanvasScale * C.SizeX) + (((1.0 - HudCanvasScale) * 0.5) * C.SizeX);
    YPos = (ConsoleMessagePosY * HudCanvasScale * C.SizeY) + (((1.0 - HudCanvasScale) * 0.5) * C.SizeY) - YL;

    C.SetPos (XPos, YPos);
    C.DrawTextClipped ("(>"@Text$"_", false);
}

simulated function SetScoreBoardClass (class<Scoreboard> ScoreBoardClass)
{
    if (ScoreBoard != None )
        ScoreBoard.Destroy();

    if (ScoreBoardClass == None)
        ScoreBoard = None;
    else
    {
        ScoreBoard = Spawn (ScoreBoardClass, Owner);

        if (ScoreBoard == None)
            log ("Hud::SetScoreBoard(): Could not spawn a scoreboard of class "$ScoreBoardClass, 'Error');
    }
}

exec function ShowHud()
{
    bHideHud = !bHideHud;
}

simulated function LinkActors()
{
    PlayerOwner = PlayerController (Owner);

    if (PlayerOwner == None)
    {
        PlayerConsole = None;
        PawnOwner = None;
        PawnOwnerPRI = None;
        return;
    }

    if (PlayerOwner.Player != None)
        PlayerConsole = PlayerOwner.Player.Console;
    else
        PlayerConsole = None;

    if ((PlayerOwner.ViewTarget != None) && 
        (Pawn(PlayerOwner.ViewTarget) != None) &&
        (Pawn(PlayerOwner.ViewTarget).Controller != None))
        PawnOwner = Pawn(PlayerOwner.ViewTarget);
    else if (PlayerOwner.Pawn != None )
        PawnOwner = PlayerOwner.Pawn;
    else
        PawnOwner = None;

    if ((PawnOwner != None) && (PawnOwner.Controller != None))
        PawnOwnerPRI = PawnOwner.PlayerReplicationInfo;
    else
        PawnOwnerPRI = PlayerOwner.PlayerReplicationInfo;
}

simulated function Message( PlayerReplicationInfo PRI, coerce string Msg, name MsgType )
{
    if ( bMessageBeep )
        PlayerOwner.PlayBeepSound();
    if ( (MsgType == 'Say') || (MsgType == 'TeamSay') )
        Msg = PRI.PlayerName$": "$Msg;
    AddTextMessage(Msg,class'LocalMessage',PRI);
}

function DisplayPortrait(PlayerReplicationInfo PRI);

function DisplayMessages(Canvas C)
{
    local int i, j, XPos, YPos,MessageCount;
    local float XL, YL;

    for( i = 0; i < ConsoleMessageCount; i++ )
    {
        if ( TextMessages[i].Text == "" )
            break;
        else if( TextMessages[i].MessageLife < Level.TimeSeconds )
        {
            TextMessages[i].Text = "";

            if( i < ConsoleMessageCount - 1 )
            {
                for( j=i; j<ConsoleMessageCount-1; j++ )
                    TextMessages[j] = TextMessages[j+1];
            }
            TextMessages[j].Text = "";
            break;
        }
        else 
			MessageCount++;
    }   

    XPos = (ConsoleMessagePosX * HudCanvasScale * C.SizeX) + (((1.0 - HudCanvasScale) / 2.0) * C.SizeX);
    YPos = (ConsoleMessagePosY * HudCanvasScale * C.SizeY) + (((1.0 - HudCanvasScale) / 2.0) * C.SizeY);

    C.Font = GetConsoleFont(C);
    C.DrawColor = ConsoleColor;

    C.TextSize ("A", XL, YL);

    YPos -= YL * MessageCount+1; // DP_LowerLeft
    YPos -= YL; // Room for typing prompt

    for( i=0; i<MessageCount; i++ )
    {
        if ( TextMessages[i].Text == "" )
            break;

        C.StrLen( TextMessages[i].Text, XL, YL );
        C.SetPos( XPos, YPos );
        C.DrawColor = TextMessages[i].TextColor;
        C.DrawText( TextMessages[i].Text, false );
        YPos += YL;
    }       
}

function AddTextMessage(string M, class<LocalMessage> MessageClass, PlayerReplicationInfo PRI)
{
	local int i;


	if( bMessageBeep && MessageClass.Default.bBeep )
		PlayerOwner.PlayBeepSound();
	
    for( i=0; i<ConsoleMessageCount; i++ )
    {
        if ( TextMessages[i].Text == "" )
            break;
    }

    if( i == ConsoleMessageCount )
    {        
        for( i=0; i<ConsoleMessageCount-1; i++ )
            TextMessages[i] = TextMessages[i+1];
    }
    
    TextMessages[i].Text = M;
    TextMessages[i].MessageLife = Level.TimeSeconds + MessageClass.Default.LifeTime;
    TextMessages[i].TextColor = MessageClass.static.GetConsoleColor(PRI);
    TextMessages[i].PRI = PRI;
}

exec function GrowHUD()
{
    if( !bShowWeaponInfo )
        bShowWeaponInfo = true;
    else if( !bShowPersonalInfo )
        bShowPersonalInfo = true;
    else if( !bShowPoints )
        bShowPoints = true;
    else if ( !bShowWeaponBar )
		bShowWeaponBar = true;
	SaveConfig();
}

exec function ShrinkHUD()
{
	if ( bShowWeaponBar )
		bShowWeaponBar = false;
    else if( bShowPoints )
        bShowPoints = false;
    else if( bShowPersonalInfo )
        bShowPersonalInfo = false;
    else if( bShowWeaponInfo )
        bShowWeaponInfo = false;
	SaveConfig();
}

simulated function SetTargeting( bool bShow, optional Vector TargetLocation, optional float Size );
simulated function DrawCrosshair(Canvas C);
simulated function SetCropping( bool Active );

static function font GetConsoleFont(Canvas C)
{
	local int FontSize;
	
	FontSize = 5;
	if ( C.ClipX < 640 )
		FontSize++;
	if ( C.ClipX < 800 )
		FontSize++;
	if ( C.ClipX < 1024 )
		FontSize++;
	if ( C.ClipX < 1280 )
		FontSize++;
	if ( C.ClipX < 1600 )
		FontSize++;
	return Default.FontArray[Min(8,FontSize)];
}

static function Font GetMediumFontFor(Canvas Canvas)
{
	local int i;
	
	for ( i=0; i<8; i++ )
	{
		if ( Default.FontScreenWidthMedium[i] <= Canvas.ClipX )
			return Default.FontArray[i];
	}
	return Default.FontArray[8];
}

static function Font LargerFontThan(Font aFont)
{
	local int i;
	
	for ( i=0; i<7; i++ )
		if ( Default.FontArray[i] == aFont )
			return Default.FontArray[Max(0,i-4)];	
	return Default.FontArray[5];
}

simulated function DrawTargeting( Canvas C );

defaultproperties
{
    bMessageBeep=true
    bHidden=True
    RemoteRole=ROLE_None

    bHideHUD=false

    ConsoleColor=(R=153,G=216,B=253,A=255)

    ProgressFont=Font'DefaultFont'
    MOTDColor=(R=255,G=255,B=255,A=255)
    ProgressFadeTime=1.0

    HudCanvasScale=0.95
    HudScale=1.0

    bShowWeaponInfo=true
    bShowPersonalInfo=true
    bShowPoints=true
    bShowWeaponBar=true

    bCrosshairShow=true
    CrosshairScale=1.0
    CrosshairOpacity=1.0
    CrosshairStyle=0

    ConsoleMessagePosX=0.00
    ConsoleMessagePosY=1.00

    WhiteColor=(R=255,G=255,B=255,A=255)
    RedColor=(R=255,G=0,B=0,A=255)
    BlueColor=(R=0,G=0,B=255,A=255)
    GreenColor=(R=0,G=255,B=0,A=255)
    GoldColor=(R=255,G=255,B=0,A=255)
    TurqColor=(R=0,G=128,B=255,A=255)
    GrayColor=(R=200,G=200,B=200,A=255)
    CyanColor=(R=0,G=255,B=255,A=255)
    PurpleColor=(R=255,G=0,B=255,A=255)
    
    FontArray(0)=Font'DefaultFont'		// these are overridden in a subclass
    FontArray(1)=Font'DefaultFont'
    FontArray(2)=Font'DefaultFont'
    FontArray(3)=Font'DefaultFont'
    FontArray(4)=Font'DefaultFont'
    FontArray(5)=Font'DefaultFont'
    FontArray(6)=Font'DefaultFont'
    FontArray(7)=Font'DefaultFont'
    FontArray(8)=Font'DefaultFont'
}
