class HudBCaptureTheFlag extends HudBTeamDeathMatch;

struct FFlagWidget
{
    var EFlagState FlagState;
    var SpriteWidget Widgets[4];
};

var() SpriteWidget SymbolGB[2];
var() FFlagWidget FlagWidgets[2];
var Actor RedBase, BlueBase;

simulated function PostBeginPlay()
{
	Super.PostBeginPlay();
	SetTimer(1.0, True);
}
simulated function ShowPointBarBottom(Canvas C)
{
}
simulated function ShowPointBarTop(Canvas C)
{
}
simulated function ShowPersonalScore(Canvas C)
{
}


simulated function TeamScoreOffset()
{
	ScoreTeam[1].OffsetX = 180;
	if ( ScoreTeam[1].Value < 0 )
		ScoreTeam[1].OffsetX += 90;
	if( abs(ScoreTeam[1].Value) > 9 )
		ScoreTeam[1].OffsetX += 90;
	//if( abs(ScoreTeam[1].Value) > 99 )
	//	ScoreTeam[1].OffsetX -= 90;
}

// Alpha Pass ==================================================================================
simulated function ShowTeamScorePassA(Canvas C)
{
	local CTFBase B;
	
	Super.ShowTeamScorePassA(C);
    DrawSpriteWidget (C, SymbolGB[0]);
	DrawSpriteWidget (C, SymbolGB[1]);

	LTeamHud[0].OffsetX=-95;
	LTeamHud[1].OffsetX=-95;
	LTeamHud[2].OffsetX=-95;

	RTeamHud[0].OffsetX=95;
	RTeamHud[1].OffsetX=95;
	RTeamHud[2].OffsetX=95;

	
	TeamSymbols[0].TextureScale = 0.075;
	TeamSymbols[1].TextureScale = 0.075;
	
	TeamSymbols[0].OffsetX = -600;
	TeamSymbols[1].OffsetX = 600;

	TeamSymbols[0].OffsetY = 90;
	TeamSymbols[1].OffsetY = 90;

	ScoreTeam[0].OffsetX = -270;
	ScoreTeam[1].OffsetX = 180;

	ScoreTeam[0].OffsetY = 75;
	ScoreTeam[1].OffsetY = 75;

    DrawSpriteWidget (C, FlagWidgets[0].Widgets[FlagWidgets[0].FlagState]);
    DrawSpriteWidget (C, FlagWidgets[1].Widgets[FlagWidgets[1].FlagState]);
    
	if ( RedBase == None )
	{
		ForEach DynamicActors(Class'CTFBase', B)
		{
			if ( B.IsA('xRedFlagBase') )
				RedBase = B;
			else
				BlueBase = B;
		}
	}
	if ( RedBase != None )
	{
		C.DrawColor = RedColor;
		Draw2DLocationDot(C, RedBase.Location,0.46, 0.0365, 0.0305, 0.0405);
	}
	if ( BlueBase != None )
	{
		C.DrawColor = BlueColor;
		Draw2DLocationDot(C, BlueBase.Location,0.526, 0.0365, 0.0305, 0.0405);
	}
}

function Timer()
{
	Super.Timer();

    if ( (PawnOwnerPRI == None) || PlayerOwner.IsSpectating() )
        return;

	if ( PawnOwnerPRI.HasFlag != None )
		PlayerOwner.ReceiveLocalizedMessage( class'CTFHUDMessage', 0 );

	if ( (PlayerOwner.GameReplicationInfo != None) 
		&& (PlayerOwner.GameReplicationInfo.FlagState[PlayerOwner.PlayerReplicationInfo.Team.TeamIndex] == EFlagState.FLAG_HeldEnemy) )
		PlayerOwner.ReceiveLocalizedMessage( class'CTFHUDMessage', 1 );
}

simulated function UpdateTeamHud()
{
	local int i;
    
    for (i = 0; i < 2; i++)
    {
		FlagWidgets[i].FlagState = PlayerOwner.GameReplicationInfo.FlagState[i];
    }
    Super.UpdateTeamHUD();
}

defaultproperties
{
	SymbolGB[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=0,Y1=880,X1=142,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=0.5,PosY=0.0,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
 	SymbolGB[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=0,Y1=880,X1=142,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.5,PosY=0.0,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))

    ScoreBg[0]=(TextureCoords=(X2=0,Y1=0,X1=0,Y2=0))
    ScoreBg[1]=(TextureCoords=(X2=0,Y1=0,X1=0,Y2=0))
    ScoreBg[2]=(TextureCoords=(X2=0,Y1=0,X1=0,Y2=0))
    ScoreBg[3]=(TextureCoords=(X2=0,Y1=0,X1=0,Y2=0))

    FlagWidgets(0)=(FlagState=FLAG_Home,Widgets[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=743,Y1=97,X2=834,Y2=148),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=-67,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=60,B=60,A=255),Tints[1]=(R=255,G=60,B=60,A=255)),Widgets[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=926,Y1=97,X2=1017,Y2=148),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=-67,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=60,B=60,A=255),Tints[1]=(R=255,G=60,B=60,A=255)),Widgets[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=926,Y1=97,X2=1017,Y2=148),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=-67,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=60,B=60,A=255),Tints[1]=(R=255,G=60,B=60,A=255)),Widgets[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=835,Y1=97,X2=925,Y2=148),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=-67,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=60,B=60,A=255),Tints[1]=(R=255,G=60,B=60,A=255)))
    FlagWidgets(1)=(FlagState=FLAG_Home,Widgets[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=743,Y1=97,X1=834,Y2=147),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=68,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=205,B=255,A=255),Tints[1]=(R=100,G=205,B=255,A=255)),Widgets[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=926,Y1=97,X1=1017,Y2=148),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=68,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=205,B=255,A=255),Tints[1]=(R=100,G=205,B=255,A=255)),Widgets[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=926,Y1=97,X1=1017,Y2=148),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=68,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=205,B=255,A=255),Tints[1]=(R=100,G=205,B=255,A=255)),Widgets[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=835,Y1=97,X1=925,Y2=148),TextureScale=0.31,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.0,OffsetX=68,OffsetY=70,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=205,B=255,A=255),Tints[1]=(R=100,G=205,B=255,A=255)))
}

