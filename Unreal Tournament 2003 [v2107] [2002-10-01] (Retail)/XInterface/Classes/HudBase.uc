class HudBase extends HUD
    exportstructs
    native;

enum EScaleMode
{
    SM_None,
    SM_Up,
    SM_Down,
    SM_Left,
    SM_Right
};

struct DigitSet
{
    var Material DigitTexture;
    var IntBox TextureCoords[11]; // 0-9, 11th element is negative sign
};

struct SpriteWidget
{
    var Material WidgetTexture;

    var ERenderStyle RenderStyle;

    var IntBox TextureCoords;
    var float TextureScale;

    var EDrawPivot DrawPivot;
    var float PosX, PosY;
    var int OffsetX, OffsetY;

    var EScaleMode ScaleMode;
    var float Scale;

    var Color Tints[2];
};

struct NumericWidget
{
    var ERenderStyle RenderStyle;

    var int MinDigitCount;

    var float TextureScale;

    var EDrawPivot DrawPivot;
    var float PosX, PosY;
    var int OffsetX, OffsetY;

    var Color Tints[2];

    var int bPadWithZeroes;

    var transient int Value;
};

var() transient int TeamIndex;

var() transient ERenderStyle PassStyle; // For debugging.

struct HudLocalizedMessage
{
    // The following block of variables are set when the message is entered;
    // (Message being set indicates that a message is in the list).

	var class<LocalMessage> Message;
	var String StringMessage;
	var int Switch;
	var PlayerReplicationInfo RelatedPRI, RelatedPRI2;
	var Object OptionalObject;
	var float EndOfLife;
	var float LifeTime;

    // The following block of variables are cached on first render;
    // (StringFont being set indicates that they've been rendered).

	var Font StringFont;
	var Color DrawColor;
    var EDrawPivot DrawPivot;
    var LocalMessage.EStackMode StackMode;
	var float PosX, PosY;
	var float DX, DY;
	
	// Stinky I know.
	var bool Drawn;
};

var() transient HudLocalizedMessage LocalMessages[8];
var() class<Actor> VoteMenuClass;						// hook for mod authors

// targeting
var Material TargetMaterial;
var transient bool bShowTargeting;
var transient Vector TargetingLocation;
var transient float TargetingSize;

// instruction
var() string InstructionText;
var() string InstructionKeyText;
var() float InstructTextBorderX;
var() float InstructTextBorderY;
var() float InstrDelta;
var() float InstrRate;
var() font InstructionFont;

var() bool DoCropping;
var() float CroppingAmount;
var() Material CroppingMaterial;

// Derived HUDs override UpdateHud to update variables before rendering;
// NO draw code should be in derived DrawHud's; they should instead override 
// DrawHudPass[A-D] and call their base class' DrawHudPass[A-D] (This cuts
// down on render state changes).

simulated function UpdateHud(); 

simulated function DrawHudPassA (Canvas C); // Alpha Pass
simulated function DrawHudPassB (Canvas C); // Additive Pass 
simulated function DrawHudPassC (Canvas C); // Alpha Pass
simulated function DrawHudPassD (Canvas C); // Alternate Texture Pass

simulated function DrawHud (Canvas C)
{
    Super.DrawHud (C);

    UpdateHud();
    
    if( bShowTargeting )
        DrawTargeting(C);

    PassStyle = STY_Alpha;
    DrawHudPassA (C);
    PassStyle = STY_Additive;
    DrawHudPassB (C);
    PassStyle = STY_Alpha;
    DrawHudPassC (C);
    PassStyle = STY_None;
    DrawHudPassD (C);

    DisplayLocalMessages (C);
}

native simulated function DrawSpriteWidget (Canvas C, out SpriteWidget W);
native simulated function DrawNumericWidget (Canvas C, out NumericWidget W, out DigitSet D);

simulated function ClearMessage( out HudLocalizedMessage M )
{
	M.Message = None;
    M.StringFont = None;
}

simulated function Message( PlayerReplicationInfo PRI, coerce string Msg, name MsgType )
{
	local Class<LocalMessage> MessageClass;

	switch( MsgType )
	{
		case 'Say':
			Msg = PRI.PlayerName$": "$Msg;
			MessageClass = class'SayMessagePlus';
			break;
		case 'TeamSay':
			Msg = PRI.PlayerName$": "$Msg;
			MessageClass = class'TeamSayMessagePlus';
			break;
		case 'CriticalEvent':
			MessageClass = class'CriticalEventPlus';
			LocalizedMessage( MessageClass, 0, None, None, None, Msg );
			return;
		case 'DeathMessage':
			MessageClass = class'xDeathMessage';
			break;
		default:
			MessageClass = class'StringMessagePlus';
			break;
	}

	AddTextMessage(Msg,MessageClass,PRI);
}

simulated function LocalizedMessage( class<LocalMessage> Message, optional int Switch, optional PlayerReplicationInfo RelatedPRI_1, optional PlayerReplicationInfo RelatedPRI_2, optional Object OptionalObject, optional String CriticalString)
{
	local int i;

    if( Message == None )
        return;

    if( CriticalString == "" )
    {
		if ( PlayerOwner.PlayerReplicationInfo == RelatedPRI_1 )
			CriticalString = Message.static.GetRelatedString( Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject );
		else
			CriticalString = Message.static.GetString( Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject );
	}
	
	if( bMessageBeep && Message.default.bBeep )
		PlayerOwner.PlayBeepSound();

    if( !Message.default.bIsSpecial )
    {
	    AddTextMessage( CriticalString, Message,RelatedPRI_1 );
        return;
    }

    i = ArrayCount(LocalMessages);

	if( Message.default.bIsUnique )
	{
		for( i = 0; i < ArrayCount(LocalMessages); i++ )
		{
		    if( LocalMessages[i].Message == None )
                continue;

		    if( LocalMessages[i].Message == Message )
                break;
		}
	}
	else if ( Message.default.bIsPartiallyUnique )
	{
		for( i = 0; i < ArrayCount(LocalMessages); i++ )
		{
		    if( LocalMessages[i].Message == None )
                continue;

		    if( ( LocalMessages[i].Message == Message ) && ( LocalMessages[i].Switch == Switch ) )
                break;
        }
	}

    if( i == ArrayCount(LocalMessages) )
    {
	    for( i = 0; i < ArrayCount(LocalMessages); i++ )
	    {
		    if( LocalMessages[i].Message == None )
                break;
	    }
    }

    if( i == ArrayCount(LocalMessages) )
    {
	    for( i = 0; i < ArrayCount(LocalMessages) - 1; i++ )
		    LocalMessages[i] = LocalMessages[i+1];
    }

    ClearMessage( LocalMessages[i] );

	LocalMessages[i].Message = Message;
	LocalMessages[i].Switch = Switch;
	LocalMessages[i].RelatedPRI = RelatedPRI_1;
	LocalMessages[i].RelatedPRI2 = RelatedPRI_2;
	LocalMessages[i].OptionalObject = OptionalObject;
	LocalMessages[i].EndOfLife = Message.static.GetLifetime(Switch) + Level.TimeSeconds;
	LocalMessages[i].StringMessage = CriticalString;
	LocalMessages[i].LifeTime = Message.static.GetLifetime(Switch);
}

function Font GetFontSizeIndex(Canvas C, int FontSize)
{
    if ( C.ClipX >= 512 )
		FontSize++;
	if ( C.ClipX >= 640 )
		FontSize++;
	if ( C.ClipX >= 800 )
		FontSize++;
	if ( C.ClipX >= 1024 )
		FontSize++;
	if ( C.ClipX >= 1280 )
		FontSize++;
	if ( C.ClipX >= 1600 )
		FontSize++;
		
	return FontArray[Clamp( 8-FontSize, 0, 8)];
}

simulated function LayoutMessage( out HudLocalizedMessage Message, Canvas C )
{
    local int FontSize;

    FontSize = Message.Message.static.GetFontSize( Message.Switch, Message.RelatedPRI, Message.RelatedPRI2, PlayerOwner.PlayerReplicationInfo );
    Message.StringFont = GetFontSizeIndex(C,FontSize);
	Message.DrawColor = Message.Message.static.GetColor( Message.Switch );
    Message.Message.static.GetPos( Message.Switch, Message.DrawPivot, Message.StackMode, Message.PosX, Message.PosY );
    C.Font = Message.StringFont;
    C.TextSize( Message.StringMessage, Message.DX, Message.DY );
}

simulated function GetScreenCoords(float PosX, float PosY, out float ScreenX, out float ScreenY, out HudLocalizedMessage Message, Canvas C )
{
    ScreenX = (PosX * HudCanvasScale * C.ClipX) + (((1.0f - HudCanvasScale) * 0.5f) * C.ClipX);
    ScreenY = (PosY * HudCanvasScale * C.ClipY) + (((1.0f - HudCanvasScale) * 0.5f) * C.ClipY);
   
    switch( Message.DrawPivot )
    {
        case DP_UpperLeft:
            break;

        case DP_UpperMiddle:
            ScreenX -= Message.DX * 0.5;
            break;

        case DP_UpperRight:
            ScreenX -= Message.DX;
            break;

        case DP_MiddleRight:
            ScreenX -= Message.DX;
            ScreenY -= Message.DY * 0.5;
            break;

        case DP_LowerRight:
            ScreenX -= Message.DX;
            ScreenY -= Message.DY;
            break;

        case DP_LowerMiddle:
            ScreenX -= Message.DX * 0.5;
            ScreenY -= Message.DY;
            break;

        case DP_LowerLeft:
            ScreenY -= Message.DY;
            break;

        case DP_MiddleLeft:
            ScreenY -= Message.DY * 0.5;
            break;

        case DP_MiddleMiddle:
            ScreenX -= Message.DX * 0.5;
            ScreenY -= Message.DY * 0.5;
            break;

    }
}

simulated function DrawMessage( Canvas C, int i, float PosX, float PosY, out float DX, out float DY )
{
    local float FadeValue;
    local float ScreenX, ScreenY;

    if( !LocalMessages[i].Message.default.bFadeMessage )
		C.DrawColor = LocalMessages[i].DrawColor;
    else
	{
		FadeValue = (LocalMessages[i].EndOfLife - Level.TimeSeconds);
		C.DrawColor = LocalMessages[i].DrawColor;
		C.DrawColor.A = LocalMessages[i].DrawColor.A * (FadeValue/LocalMessages[i].LifeTime);
    }

	C.Font = LocalMessages[i].StringFont;

    GetScreenCoords( PosX, PosY, ScreenX, ScreenY, LocalMessages[i], C );
	C.SetPos( ScreenX, ScreenY );
	C.DrawTextClipped( LocalMessages[i].StringMessage, false );

    DX = LocalMessages[i].DX / C.ClipX;
    DY = LocalMessages[i].DY / C.ClipY;
    
    LocalMessages[i].Drawn = true;
}

simulated function DisplayLocalMessages( Canvas C )
{
	local float PosX, PosY, DY, DX;
    local int i, j;
    local float FadeValue;

	C.Reset();

    // Pass 1: Layout anything that needs it and cull dead stuff.
    
    for( i = 0; i < ArrayCount(LocalMessages); i++ )
    {
		if( LocalMessages[i].Message == None )
            break;

        LocalMessages[i].Drawn = false;

        if( LocalMessages[i].StringFont == None )
            LayoutMessage( LocalMessages[i], C );

        if( LocalMessages[i].StringFont == None )
        {
            log( "LayoutMessage("$LocalMessages[i].Message$") failed!", 'Error' );

	        for( j = i; j < ArrayCount(LocalMessages) - 1; j++ )
		        LocalMessages[j] = LocalMessages[j+1];
            ClearMessage( LocalMessages[j] );
            i--;
            continue;
        }

		if( LocalMessages[i].Message.default.bFadeMessage )
		{
			FadeValue = (LocalMessages[i].EndOfLife - Level.TimeSeconds);

			if( FadeValue <= 0.0 ) 
            {
	            for( j = i; j < ArrayCount(LocalMessages) - 1; j++ )
		            LocalMessages[j] = LocalMessages[j+1];
                ClearMessage( LocalMessages[j] );
                i--;
                continue;
            }
        }
    }

    // Pass 2: Go through the list and draw each stack:

    for( i = 0; i < ArrayCount(LocalMessages); i++ )
	{
		if( LocalMessages[i].Message == None )
            break;

        if( LocalMessages[i].Drawn )
            continue;

	    PosX = LocalMessages[i].PosX;
	    PosY = LocalMessages[i].PosY;
	    
        if( LocalMessages[i].StackMode == SM_None )
        {
            DrawMessage( C, i, PosX, PosY, DX, DY );
            continue;
        }
        
        for( j = i; j < ArrayCount(LocalMessages); j++ )
        {
            if( LocalMessages[j].Drawn )
                continue;

            if( LocalMessages[i].PosX != LocalMessages[j].PosX ) 
                continue;

            if( LocalMessages[i].PosY != LocalMessages[j].PosY ) 
                continue;

            if( LocalMessages[i].DrawPivot != LocalMessages[j].DrawPivot ) 
                continue;

            if( LocalMessages[i].StackMode != LocalMessages[j].StackMode ) 
                continue;
        
            DrawMessage( C, j, PosX, PosY, DX, DY );
            
            switch( LocalMessages[j].StackMode )
            {
                case SM_Up:
                    PosY -= DY;
                    break;
                    
                case SM_Down:
                    PosY += DY;
                    break;
            }
        }
    }
}

simulated function CreateKeyMenus() // create vote/speech menus here
{
	if ( (PlayerController(Owner).PlayerReplicationInfo != None)
		&& PlayerController(Owner).PlayerReplicationInfo.bOnlySpectator )
		return;
    if( VoteMenuClass != None )
        VoteMenu = Spawn(VoteMenuClass,self);
}

function Draw2DLocationDot(Canvas C, vector Loc,float OffsetX, float OffsetY, float ScaleX, float ScaleY)
{
	local rotator Dir;
	local float Angle;
	local Actor Start;
	
	if ( PawnOwner == None )
		Start = PlayerOwner;
	else
		Start = PawnOwner;
		
	Dir = rotator(Loc - Start.Location);
	Angle = ((Dir.Yaw - PlayerOwner.Rotation.Yaw) & 65535) * 6.2832/65536;
	C.Style = ERenderStyle.STY_Alpha;
	C.SetPos(OffsetX * C.ClipX + ScaleX * C.ClipX * sin(Angle),
			OffsetY * C.ClipY - ScaleY * C.ClipY * cos(Angle));
	C.DrawTile(Material'InterfaceContent.Hud.SkinA',24*C.ClipX/1600,24*C.ClipX/1600,838,238,144,144);
}

simulated function SetTargeting( bool bShow, optional Vector TargetLocation, optional float Size )
{
    bShowTargeting = bShow;
    if( bShow )
    {   
        TargetingLocation = TargetLocation;
        if( Size != 0.0 )
            TargetingSize = Size;
    }
}

simulated function DrawTargeting( Canvas C )
{
    local int XPos, YPos;
    local vector ScreenPos;
    local vector X,Y,Z,Dir;
    local float RatioX, RatioY;
    local float tileX, tileY;
    local float Dist;

    local float SizeX;
    local float SizeY;

    SizeX = TargetingSize * 96.0;
    SizeY = TargetingSize * 96.0;

    if( !bShowTargeting )
        return;

    ScreenPos = C.WorldToScreen( TargetingLocation );

    RatioX = C.SizeX / 640.0;
    RatioY = C.SizeY / 480.0;

    tileX = sizeX * RatioX;
    tileY = sizeY * RatioX;

    GetAxes(PlayerOwner.Rotation, X,Y,Z);
	Dir = TargetingLocation - PawnOwner.Location;
	Dist = VSize(Dir);
	Dir = Dir/Dist;

    if ( (Dir Dot X) > 0.0 ) // don't draw if it's behind the eye
	{
		XPos = ScreenPos.X;
		YPos = ScreenPos.Y;
        C.Style = ERenderStyle.STY_Additive;
        C.DrawColor.R = 255;
        C.DrawColor.G = 255;
        C.DrawColor.B = 255;
        C.DrawColor.A = 255;
		C.SetPos(XPos - tileX*0.5, YPos - tileY*0.5);
        C.DrawTile( TargetMaterial, tileX, tileY, 0.0, 0.0, 256, 256); //--- TODO : Fix HARDCODED USIZE
        //log("Drawing passtarget focus1");
	}
}

simulated function SetCropping( bool Active )
{
    DoCropping = active;
}

simulated function DrawInstructionGfx(Canvas C)
{
    local float CropHeight;

    //log("DrawInstructionGfx");

    DrawCrosshair(C);
    DrawTargeting(C);
    if( DoCropping )
    {
        // todo: lerp the crop height
        CropHeight = (C.SizeY * CroppingAmount) * 0.5;
        C.SetPos(0, 0);
        C.DrawTile( Texture'Engine.BlackTexture', C.SizeX, CropHeight, 0.0, 0.0, 64, 64 );
        C.SetPos( 0, C.SizeY-CropHeight );
        C.DrawTile( Texture'Engine.BlackTexture', C.SizeX, CropHeight, 0.0, 0.0, 64, 64 );
    }
    DrawInstructionText(C);
    DrawInstructionKeyText(C);
}

simulated function DrawInstructionText(Canvas C)
{
    if( InstructionText == "" )
        return;

    C.Font = InstructionFont;

    C.SetOrigin( InstructTextBorderX, InstructTextBorderY );
    C.SetClip( C.SizeX-InstructTextBorderX, C.SizeY );
    C.SetPos(0,0);

	C.DrawText( InstructionText );    

    C.SetOrigin(0.0, 0.0);
    C.SetClip( C.SizeX, C.SizeY );
}

simulated function DrawInstructionKeyText(Canvas C)
{
    local float strX;
    local float strY;

    if( InstructionKeyText == "" )
        return;

    C.Font = InstructionFont;
    C.SetOrigin( InstructTextBorderX, InstructTextBorderY );
    C.SetClip( C.SizeX-InstructTextBorderX, C.SizeY );

    C.StrLen( InstructionKeyText, strX, strY );

    C.SetOrigin( InstructTextBorderX, C.SizeY-strY-InstructTextBorderY );
    C.SetClip( C.SizeX-InstructTextBorderX, C.SizeY );
    C.SetPos(0,0);

	C.DrawText( InstructionKeyText );

    C.SetOrigin(0.0, 0.0);
    C.SetClip( C.SizeX, C.SizeY );
}

simulated function SetInstructionText( string text )
{
    InstructionText = text;
}

simulated function SetInstructionKeyText( string text )
{
    InstructionKeyText = text;
}

defaultproperties
{
    TargetMaterial=Material'InterfaceContent.Hud.fbBombFocus'

    ProgressFont=Font'FontMedium'

	FontArray(0)=Font'jFontLarge1024x768'	//37
    FontArray(1)=Font'jFontLarge800x600'		//29
    FontArray(2)=Font'jFontLarge'			//24
    FontArray(3)=Font'jFontMedium1024x768'	//21
    FontArray(4)=Font'jFontMedium800x600'	//17
    FontArray(5)=Font'jFontMedium'			//14
    FontArray(6)=Font'jFontSmall'			//12
    FontArray(7)=Font'jFontSmallText800x600' //9
    FontArray(8)=Font'FontSmallText'		//6

    FontScreenWidthMedium(0)=2048
    FontScreenWidthMedium(1)=1600
    FontScreenWidthMedium(2)=1280
    FontScreenWidthMedium(3)=1024
    FontScreenWidthMedium(4)=800
    FontScreenWidthMedium(5)=640
    FontScreenWidthMedium(6)=512
    FontScreenWidthMedium(7)=400
    FontScreenWidthMedium(8)=320

    FontScreenWidthSmall(0)=4096
    FontScreenWidthSmall(1)=3200
    FontScreenWidthSmall(2)=2560
    FontScreenWidthSmall(3)=2048
    FontScreenWidthSmall(4)=1600
    FontScreenWidthSmall(5)=1280
    FontScreenWidthSmall(6)=1024
    FontScreenWidthSmall(7)=800
    FontScreenWidthSmall(8)=640

    CroppingAmount=0.25
    InstructTextBorderX=10.0
    InstructTextBorderY=10.0
	InstructionFont=Font'jFontMono800x600'
}

