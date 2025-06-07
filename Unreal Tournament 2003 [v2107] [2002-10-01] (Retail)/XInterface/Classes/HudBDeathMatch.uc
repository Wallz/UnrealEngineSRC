#exec OBJ LOAD FILE=XGameShaders.utx
#exec OBJ LOAD FILE=Crosshairs.utx

class HudBDeathMatch extends HudBase
    config(User);

var() DigitSet DigitsBig;

var() SpriteWidget LHud1[4];
var() SpriteWidget LHud2[4];

var() SpriteWidget RHud1[4];
var() SpriteWidget RHud2[4];

const WEAPON_BAR_SIZE = 9; 

var() SpriteWidget WeaponBarAmmoFill[WEAPON_BAR_SIZE];
var() SpriteWidget WeaponBarTint[WEAPON_BAR_SIZE];
var() SpriteWidget WeaponBarTrim[WEAPON_BAR_SIZE];
var() SpriteWidget WeaponBarIcon[WEAPON_BAR_SIZE];
var() class<Weapon> BaseWeapons[WEAPON_BAR_SIZE];

var() SpriteWidget AmmoIcon;

var() SpriteWidget ScoreBg[4];
var() SpriteWidget Adrenaline[5];

var() array<SpriteWidget> Crosshairs;
var() SpriteWidget  HealthIcon;

var() NumericWidget AdrenalineCount;
var() NumericWidget ComboCount;
var() NumericWidget HealthCount;
var() NumericWidget AmmoCount;
var() NumericWidget ShieldCount;
var() NumericWidget mySpread;
var() NumericWidget myRank;
var() NumericWidget myScore;

var() SpriteWidget ShieldIconGlow;
var() SpriteWidget ShieldIcon;
var() SpriteWidget AdrenalineIcon;
var() SpriteWidget ReloadingTeamTint;
var() SpriteWidget ReloadingTrim;
var() SpriteWidget ReloadingFill;

var() SpriteWidget UDamageTeamTint;
var() SpriteWidget UDamageTrim;
var() SpriteWidget UDamageFill;

var() localized String text;
var() localized String LevelActionLoading, LevelActionPaused;

var() Font LevelActionFont;
var() Color LevelActionFontColor;

var() float LevelActionPositionX, LevelActionPositionY;
var() float CurrentWeaponPositionX, CurrentWeaponPositionY;

var() Texture LogoTexture;

var() float LogoScaleX;
var() float LogoScaleY;
var() float LogoPosX;
var() float LogoPosY;

var() float testLerp;

var() float comboTime;
var() float accumData[4];
var() float growScale[4];
var() float growTrace[4];
var() int   pulse[5];

var() bool ArmorGlow;
var() bool Displaying;
var() bool growing;
var() bool LowHealthPulse;
var() bool TeamLinked;
var() bool AdrenalineReady;

var() float TransRechargeAmount;

var transient float CurHealth, LastHealth, CurShield, LastShield, CurEnergy, CurAmmoPrimary, pulseHealthIcon,pulseArmorIcon;
var transient float MaxHealth, MaxShield, MaxEnergy, MaxAmmoPrimary;

var transient int CurScore, CurRank, ScoreDiff;

const DamageDirFront    = 0;
const DamageDirRight    = 1;
const DamageDirLeft     = 2;
const DamageDirBehind   = 3;

const DamageDirMax      = 4;

var int OldRemainingTime;
var sound CountDown[10];
var sound LongCount[6];

var PlayerReplicationInfo NamedPlayer;
var float NameTime;

var Material Portrait;
var float PortraitTime;
var float PortraitX;
var PlayerReplicationInfo PortraitPRI;

var localized string WonMatchPrefix, WonMatchPostFix, WaitingToSpawn;
var localized string YouveWonTheMatch, YouveLostTheMatch;
	
simulated function UpdatePrecacheMaterials()
{
    Level.AddPrecacheMaterial(Material'InterfaceContent.HUD.SkinA');
}

function CheckCountdown(GameReplicationInfo GRI)
{
	if ( (GRI == None) || (GRI.RemainingTime == 0) || (GRI.RemainingTime == OldRemainingTime) || (GRI.Winner != None) )
		return;
	
	OldRemainingTime = GRI.RemainingTime;
	if ( OldRemainingTime > 300 )
		return;
	if ( OldRemainingTime > 30 )
	{
		if ( OldRemainingTime == 300 )
			PlayerOwner.PlayAnnouncement(LongCount[0],1,true);
		else if ( OldRemainingTime == 180 )
			PlayerOwner.PlayAnnouncement(LongCount[1],1,true);
		else if ( OldRemainingTime == 120 )
			PlayerOwner.PlayAnnouncement(LongCount[2],1,true);
		else if ( OldRemainingTime == 60 )
			PlayerOwner.PlayAnnouncement(LongCount[3],1,true);
		return;
	}
	if ( OldRemainingTime == 30 )
		PlayerOwner.PlayAnnouncement(LongCount[4],1,true);
	else if ( OldRemainingTime == 20 )
		PlayerOwner.PlayAnnouncement(LongCount[5],1,true);
	else if ( (OldRemainingTime <= 10) && (OldRemainingTime > 0) )
		PlayerOwner.PlayAnnouncement(CountDown[OldRemainingTime - 1],1,true);
}

simulated function Tick(float deltaTime)
{
    Super.Tick(deltaTime);

	if ( PortraitTime - Level.TimeSeconds > 0 )
		PortraitX = FMax(0,PortraitX-3*deltaTime);
	else if ( Portrait != None )
	{
		PortraitX = FMin(1,PortraitX+3*deltaTime);
		if ( PortraitX == 1 )
			Portrait = None;
	}		

    pulseNumber(0, HealthCount, 0.26, 0.05, deltaTime, 0.25, 0,255,CurHealth, LastHealth);
    pulseNumber(1, ShieldCount, 0.26, 0.05, deltaTime, 0.25, 0,255,CurShield, LastShield);

    ArmorPulse(deltaTime,255,0,ShieldIconGlow,TeamIndex,250);
    if(CurHealth < 50)
        LowHealth(deltaTime,255,0,LHud2[3],TeamIndex,1000);
    else
        LHud2[3].Tints[TeamIndex].A = 50;
    
    if(CurHealth < 25)
        pulseWidget(deltaTime, 255, 0, HealthIcon, TeamIndex, 2, 2000);
    else
        HealthIcon.Tints[TeamIndex].A = 255;

    if(AdrenalineReady)
        pulseWidget(deltaTime, 255, 0, Adrenaline[4], TeamIndex, 2, 1000);
    else
        Adrenaline[4].Tints[TeamIndex].A = 0;
}

simulated function pulseWidget(float deltaTime, float max, float min, out SpriteWidget sprite, int tIndex, int pIndex, float pRate)
{
    local float accum;

    accum = deltaTime * pRate;
    
    if(sprite.Tints[tIndex].A < min)
        accumData[pIndex] += accum;
    else
        accumData[pIndex] -= accum;
    
    if(accumData[pIndex] < min)
        accumData[pIndex] = max;

    sprite.Tints[tIndex].A = accumData[pIndex];
}

simulated function pulseNumber(int gIndex, out NumericWidget number, float nScale, float growSpeed, float deltaTime, float oScale ,float test1,float test2, float first, float last)
{
    local float growAccum;

    growAccum = deltaTime * growSpeed;

    testLerp = test1 + (deltaTime * 2) *(test2 - test1);
    
    if(growing)
    {
        growScale[gIndex] -= growAccum;
        growTrace[gIndex] += testLerp;
        
        if(growTrace[gIndex] > 255)
            growTrace[gIndex] = 255;

        if (first < last)
        {
            growTrace[gIndex] = 0;
            growScale[gIndex] = nScale;
        }
        if(growScale[gIndex] < oScale)
            growScale[gIndex] = oScale;

        if(growScale[gIndex] < oScale && growTrace[gIndex] ==255)
            growing = false;

        number.Tints[TeamIndex].B = growTrace[gIndex];
        number.TextureScale = growScale[gIndex];
    }
    else if (first < last)
    {
        growTrace[gIndex] = 0;
        growScale[gIndex] = nScale;
        growing = true;
    }
    else
    {
        growScale[gIndex] = oScale;
    }
}


simulated function ArmorPulse(float deltaTime, float max, float min, out SpriteWidget sprite, int tIndex, float pRate)
{
    local int accum;

    accum = deltaTime * pRate;

    if(ArmorGlow)
        pulseArmorIcon += accum;
    else
        pulseArmorIcon -= accum;
    
    if(pulseArmorIcon<= min)
    {
        pulseArmorIcon= min;
        ArmorGlow = true;
    }
    else if(pulseArmorIcon >= max)
    {
        pulseArmorIcon= max;
        ArmorGlow = false;
    }
    
    sprite.Tints[tIndex].A = pulseArmorIcon;
    sprite.Tints[tIndex].B = pulseArmorIcon;

}
simulated function LowHealth(float deltaTime, float max, float min, out SpriteWidget sprite, int tIndex,float pRate)
{
    local int accum;

    accum = deltaTime * pRate;

    if(LowHealthPulse)
        pulseHealthIcon += accum;
    else
        pulseHealthIcon -= accum;
    
    if(pulseHealthIcon<= min)
    {
        pulseHealthIcon= min;
        LowHealthPulse = true;
    }
    else if(pulseHealthIcon >= max)
    {
        pulseHealthIcon= max;
        LowHealthPulse = false;
    }
    sprite.Tints[tIndex].A = pulseHealthIcon;
}

//////////////////////////////////////////
simulated function UpdateRankAndSpread(Canvas C)
{
    local int i;

	if ( (Scoreboard == None) || !Scoreboard.UpdateGRI() )
		return;
	
    for( i=0 ; i<PlayerOwner.GameReplicationInfo.PRIArray.Length ; i++ )
         if(PawnOwnerPRI == PlayerOwner.GameReplicationInfo.PRIArray[i])
         {
            myRank.Value = (i+1);
            break;
         }

	myScore.Value = Min (PawnOwnerPRI.Score, 999);  // max display space
	if ( PawnOwnerPRI == PlayerOwner.GameReplicationInfo.PRIArray[0] )
	{
		if ( PlayerOwner.GameReplicationInfo.PRIArray.Length > 1 )
			mySpread.Value = Min (PawnOwnerPRI.Score - PlayerOwner.GameReplicationInfo.PRIArray[1].Score, 999);
		else
			mySpread.Value = 0;
	}
	else
		mySpread.Value = Min (PawnOwnerPRI.Score - PlayerOwner.GameReplicationInfo.PRIArray[0].Score, 999);
        
    if( bShowPoints )
    {
        DrawNumericWidget (C, myScore, DigitsBig);
        if ( C.ClipX >= 640 )
			DrawNumericWidget (C, mySpread, DigitsBig);
        DrawNumericWidget (C, myRank, DigitsBig);
    }

    if(myRank.Value > 9)
    {
        myRank.TextureScale = 0.12;
        myRank.OffsetX = 240;
        myRank.OffsetY = 90;
    }
    else
    {
        myRank.TextureScale = 0.18;
        myRank.OffsetX = 150;
        myRank.OffsetY = 40;
    }
}

simulated function CalculateHealth()
{
    LastHealth = CurHealth;
    MaxHealth = PawnOwner.HealthMax;
    CurHealth = PawnOwner.Health;
}

simulated function CalculateShield()
{
    LastShield = CurShield;
    if( PawnOwner.IsA ('XPawn') )
    {
        MaxShield = XPawn(PawnOwner).ShieldStrengthMax;
        CurShield = Clamp (XPawn(PawnOwner).ShieldStrength, 0, MaxShield);
    }
    else
    {
        MaxShield = 100;
        CurShield = 0;
    }
}

simulated function CalculateEnergy()
{
	if ( PawnOwner.Controller == None )
	{
		MaxEnergy = PlayerOwner.AdrenalineMax;
		CurEnergy = Clamp (PlayerOwner.Adrenaline, 0, MaxEnergy);
	}
	else
	{
		MaxEnergy = PawnOwner.Controller.AdrenalineMax;
		CurEnergy = Clamp (PawnOwner.Controller.Adrenaline, 0, MaxEnergy);
	}
    AdrenalineCount.Value = CurEnergy;
}

simulated function CalculateAmmo()
{
    MaxAmmoPrimary = 1;
    CurAmmoPrimary = 1;

    if ( (PawnOwner != none) && (PawnOwner.Weapon != none) )
		PawnOwner.Weapon.GetAmmoCount(MaxAmmoPrimary,CurAmmoPrimary);
    RHud2[2].Scale = CurAmmoPrimary / MaxAmmoPrimary;       
}

simulated function CalculateScore()
{
    ScoreDiff = CurScore;
    CurScore = PawnOwnerPRI.Score;
}

simulated function DrawSpectatingHud (Canvas C)
{
 	local float RestartXL,YL;
	local string InfoString;
	
    DisplayLocalMessages (C);
    
    C.Font = GetMediumFontFor(C);
	if ( UnrealPlayer(Owner).bDisplayWinner )
		InfoString = YouveWonTheMatch;
	else if ( UnrealPlayer(Owner).bDisplayLoser )
	{
		if ( PlayerReplicationInfo(PlayerOwner.GameReplicationInfo.Winner) != None )
			InfoString = WonMatchPrefix$PlayerReplicationInfo(PlayerOwner.GameReplicationInfo.Winner).PlayerName$WonMatchPostFix;
		else
			InfoString = YouveLostTheMatch;
	}
    else if ( PlayerOwner.IsDead() )
		InfoString = class'ScoreboardDeathMatch'.default.Restart;
	else if ( (Level.NetMode != NM_Standalone) && (PlayerOwner.PlayerReplicationInfo != None) && PlayerOwner.PlayerReplicationInfo.bWaitingPlayer )
		InfoString = WaitingToSpawn;
	else
		return;
		
	C.StrLen(InfoString,RestartXL,YL);
	C.Style = ERenderStyle.STY_Alpha;
	C.DrawColor = GoldColor;
		C.SetPos(0.5*(C.ClipX-RestartXL), C.ClipY - 2*YL);
	C.DrawText(InfoString,true);
}

simulated function DrawCrosshair (Canvas C)
{
    local float NormalScale;
    local byte NormalOpacity[2];
    local int i;

    CrosshairStyle = Clamp(crosshairStyle, 0, Crosshairs.Length - 1);
    
    if (!bCrosshairShow)
        return;

    NormalScale = Crosshairs[CrosshairStyle].TextureScale;
    Crosshairs[CrosshairStyle].TextureScale *= CrosshairScale;
    
    for( i = 0; i < ArrayCount(Crosshairs[CrosshairStyle].Tints); i++ )
    {
        NormalOpacity[i] = Crosshairs[CrosshairStyle].Tints[i].A;
        Crosshairs[CrosshairStyle].Tints[i].A = float(Crosshairs[CrosshairStyle].Tints[i].A) * CrosshairOpacity;
    }

    DrawSpriteWidget (C, Crosshairs[CrosshairStyle]);

    Crosshairs[CrosshairStyle].TextureScale = NormalScale;

    for( i = 0; i < ArrayCount(Crosshairs[CrosshairStyle].Tints); i++ )
        Crosshairs[CrosshairStyle].Tints[i].A = NormalOpacity[i];
        
	DrawEnemyName(C);
}

function DrawEnemyName(Canvas C)
{
	local actor HitActor;
	local vector HitLocation,HitNormal,ViewPos;
	
	if ( PawnOwner.Controller == None )
		return;
	ViewPos = PawnOwner.Location + PawnOwner.BaseEyeHeight * vect(0,0,1);
	HitActor = trace(HitLocation,HitNormal,ViewPos+1200*vector(PawnOwner.Controller.Rotation),ViewPos,true);
	if ( (Pawn(HitActor) != None) && (Pawn(HitActor).PlayerReplicationInfo != None)
		&& ( (PawnOwner.PlayerReplicationInfo.Team == None) || (PawnOwner.PlayerReplicationInfo.Team != Pawn(HitActor).PlayerReplicationInfo.Team)) )
	{
		if ( (NamedPlayer != Pawn(HitActor).PlayerReplicationInfo) || (Level.TimeSeconds - NameTime > 0.5) )
		{
			PlayerOwner.ReceiveLocalizedMessage(class'PlayerNameMessage',0,Pawn(HitActor).PlayerReplicationInfo);
			NameTime = Level.TimeSeconds;
		}
		NamedPlayer = Pawn(HitActor).PlayerReplicationInfo;
	}
}    

// Alpha Pass ==================================================================================
simulated function DrawHudPassA (Canvas C)
{
    ShowPointBarBottom(C);
    
    if( bShowPersonalInfo )
    {
        DrawSpriteWidget (C, LHud1[2]);
        DrawSpriteWidget (C, LHud1[1]);
        DrawSpriteWidget (C, LHud2[2]);
        DrawSpriteWidget (C, LHud2[1]);

        DrawSpriteWidget (C, Adrenaline[2] );
        DrawSpriteWidget (C, Adrenaline[1] );
    }

	if ( bShowWeaponBar && (PawnOwner.Weapon != None) )
        DrawWeaponBar(C);
	
    if( bShowWeaponInfo && (PawnOwner.Weapon != None) )
    {
        DrawSpriteWidget (C, RHud1[2]);
        DrawSpriteWidget (C, RHud1[1]);

        DrawSpriteWidget (C, RHud2[2]);
        DrawSpriteWidget (C, RHud2[1]);

        DrawSpriteWidget (C, RHud1[0]);
        DrawSpriteWidget (C, RHud2[0]);

        DrawSpriteWidget (C, RHud1[3]);
        DrawSpriteWidget (C, RHud2[3]);

        if ( PawnOwner.Weapon.bShowChargingBar )
        {
            ReloadingFill.Scale = PawnOwner.Weapon.ChargeBar();

            DrawSpriteWidget (C, ReloadingFill);
            DrawSpriteWidget (C, ReloadingTeamTint);
            DrawSpriteWidget (C, ReloadingTrim);
        }   

        if( (PawnOwner.Weapon.Ammo[0] != None) && (PawnOwner.Weapon.Ammo[0].IconMaterial != None) )
        {
            AmmoIcon.WidgetTexture = PawnOwner.Weapon.Ammo[0].IconMaterial;
            AmmoIcon.TextureCoords = PawnOwner.Weapon.Ammo[0].IconCoords;
            DrawSpriteWidget (C, AmmoIcon);
        }
    }

    if( bShowPersonalInfo && (ShieldCount.Value > 0) )
        DrawSpriteWidget (C, ShieldIconGlow);

    DisplayLocalMessages (C);
}

simulated function ShowPointBarTop(Canvas C)
{
    if( bShowPoints )
    {
        DrawSpriteWidget (C, ScoreBG[0]);
        DrawSpriteWidget (C, ScoreBG[3]);
    }
}
simulated function ShowPointBarBottom(Canvas C)
{
    if( bShowPoints )
    {
        DrawSpriteWidget (C, ScoreBG[2]);
        DrawSpriteWidget (C, ScoreBG[1]);
    }
}

// Alpha Pass ==================================================================================
simulated function DrawHudPassC (Canvas C)
{
	local float PortraitWidth,PortraitHeight, XL, YL, Abbrev;
	local string PortraitString;
	
	// portrait
	if ( bShowPersonalInfo && (Portrait != None) )
	{
		PortraitWidth = 0.125 * C.ClipY;
		PortraitHeight = 1.5 * PortraitWidth;
		C.DrawColor = WhiteColor;

		C.SetPos(-PortraitWidth*PortraitX + 0.025*PortraitWidth,0.5*(C.ClipY-PortraitHeight) + 0.025*PortraitHeight);
		C.DrawTile( Portrait, PortraitWidth, PortraitHeight, 0, 0, 256, 384);
		
		C.SetPos(-PortraitWidth*PortraitX,0.5*(C.ClipY-PortraitHeight));
		C.Font = GetFontSizeIndex(C,-2);
		PortraitString = PortraitPRI.PlayerName;
		C.StrLen(PortraitString,XL,YL);
		if ( XL > PortraitWidth )
		{
			C.Font = GetFontSizeIndex(C,-3);
			C.StrLen(PortraitString,XL,YL);
			if ( XL > PortraitWidth )
			{
				Abbrev = float(len(PortraitString)) * PortraitWidth/XL;
				PortraitString = left(PortraitString,Abbrev);
				C.StrLen(PortraitString,XL,YL);
			}
		}

		C.DrawColor = C.static.MakeColor(160,160,160);
		C.SetPos(-PortraitWidth*PortraitX + 0.025*PortraitWidth,0.5*(C.ClipY-PortraitHeight) + 0.025*PortraitHeight);
		C.DrawTile( Material'XGameShaders.ModuNoise', PortraitWidth, PortraitHeight, 0.0, 0.0, 512, 512 );

		C.DrawColor = WhiteColor;
		C.SetPos(-PortraitWidth*PortraitX,0.5*(C.ClipY-PortraitHeight));
		C.DrawTileStretched(texture 'InterfaceContent.Menu.BorderBoxA1', 1.05 * PortraitWidth, 1.05*PortraitHeight);

		C.DrawColor = WhiteColor;
		C.SetPos(C.ClipY/256-PortraitWidth*PortraitX + 0.5 * (PortraitWidth - XL),0.5*(C.ClipY+PortraitHeight) + 0.06*PortraitHeight);
		if ( PortraitPRI != None )
		{
			if ( PortraitPRI.Team != None )
			{
				if ( PortraitPRI.Team.TeamIndex == 0 )
					C.DrawColor = RedColor;
				else
					C.DrawColor = TurqColor;
			}
			C.DrawText(PortraitString,true);
		}
	}

    // Screen
    if( bShowPersonalInfo )
    {
        DrawSpriteWidget (C, LHud1[0]);
        DrawSpriteWidget (C, LHud2[0]);
        DrawSpriteWidget (C, LHud1[3]);
        DrawSpriteWidget (C, LHud2[3]);
    }   
    ShowPointBarTop(C);

    if( bShowPersonalInfo )
    {
        DrawSpriteWidget (C, Adrenaline[0]);
        DrawSpriteWidget (C, Adrenaline[3]);
        DrawSpriteWidget (C, Adrenaline[4]);
        DrawNumericWidget (C, AdrenalineCount, DigitsBig);
        DrawSpriteWidget (C, AdrenalineIcon);
 
		if( PawnOwner.IsA ('XPawn') && (XPawn(PawnOwner).UDamageTime > Level.TimeSeconds) )
		{
			UDamageFill.Scale = FMin((XPawn(PawnOwner).UDamageTime - Level.TimeSeconds) * 0.0333,1);

			DrawSpriteWidget (C, UDamageFill);
			DrawSpriteWidget (C, UDamageTeamTint);
			DrawSpriteWidget (C, UDamageTrim);
		}       
        DrawSpriteWidget (C, HealthIcon);
    
		if( ShieldCount.Value > 0 )
		{
			DrawSpriteWidget (C, ShieldIcon);
			DrawNumericWidget (C, ShieldCount, DigitsBig);
		}

        DrawNumericWidget (C, HealthCount, DigitsBig);
	}
	
    UpdateRankAndSpread(C);
    
    if( bShowWeaponInfo && (PawnOwner != None) && (PawnOwner.Weapon != None) )
    {
        DrawNumericWidget (C, AmmoCount, DigitsBig);
		PawnOwner.Weapon.DrawWeaponInfo(C);
    }
    
    DrawCrosshair(C);
}

simulated function DrawWeaponBar( Canvas C )
{
    local int i;
    
    local Weapon Weapons[WEAPON_BAR_SIZE];
    local Inventory Inv;
    local Weapon W, PendingWeapon;
	local int Count;
	
    if (PawnOwner.PendingWeapon != None)
        PendingWeapon = PawnOwner.PendingWeapon;
    else
        PendingWeapon = PawnOwner.Weapon;
 
	// fill:   
    for( Inv=PawnOwner.Inventory; Inv!=None; Inv=Inv.Inventory )
    {
        W = Weapon( Inv );
        
        if( W == None )
            continue;
            
        if( W.IconMaterial == None )
            continue;
		
		if ( W.InventoryGroup == 0 )
			Weapons[8] = W;
		else if ( W.InventoryGroup < 10 )
			Weapons[W.InventoryGroup-1] = W;
		Count++;
		if ( Count > 30 )
			break;
    }
	if ( PendingWeapon != None )
	{
		if ( PendingWeapon.InventoryGroup == 0 )
			Weapons[8] = PendingWeapon;
		else if ( PendingWeapon.InventoryGroup < 10 )
			Weapons[PendingWeapon.InventoryGroup-1] = PendingWeapon;
	}
		
    // Draw:
    for( i=0; i<WEAPON_BAR_SIZE; i++ )
    {
        W = Weapons[i];

        WeaponBarAmmoFill[i].Tints[0] = default.WeaponBarAmmoFill[i].Tints[0];
        WeaponBarAmmoFill[i].Tints[1] = default.WeaponBarAmmoFill[i].Tints[1];
        WeaponBarTint[i].Tints[0] = default.WeaponBarAmmoFill[i].Tints[0];
        WeaponBarTint[i].Tints[1] = default.WeaponBarAmmoFill[i].Tints[1];

        if( W == None )
        {
            WeaponBarAmmoFill[i].Scale  = 0;
        
            WeaponBarIcon[i].Tints[TeamIndex].A = 50;
            WeaponBarIcon[i].Tints[TeamIndex].R = 255;
            WeaponBarIcon[i].Tints[TeamIndex].B = 0;
            WeaponBarIcon[i].Tints[TeamIndex].G = 255;

            WeaponBarIcon[i].WidgetTexture = BaseWeapons[i].default.IconMaterial;
            WeaponBarIcon[i].TextureCoords = BaseWeapons[i].default.IconCoords;
        }
        else
        {
            WeaponBarAmmoFill[i].Scale = W.AmmoStatus();

            WeaponBarIcon[i].WidgetTexture = W.IconMaterial;
            WeaponBarIcon[i].TextureCoords = W.IconCoords;
        
            if (W == PendingWeapon)
            {
                WeaponBarIcon[i].Tints[TeamIndex].A = 255;
                WeaponBarIcon[i].Tints[TeamIndex].R = 255;
                WeaponBarIcon[i].Tints[TeamIndex].B = 0;
                WeaponBarIcon[i].Tints[TeamIndex].G = 255;

                WeaponBarTint[i].Tints[TeamIndex].A = 50;
                WeaponBarTint[i].Tints[TeamIndex].R = 255;
                WeaponBarTint[i].Tints[TeamIndex].B = 0;
                WeaponBarTint[i].Tints[TeamIndex].G = 255;
                
                WeaponBarAmmoFill[i].Tints[TeamIndex].B = 100;
                WeaponBarAmmoFill[i].Tints[TeamIndex].G = 50; 
                WeaponBarAmmoFill[i].Tints[TeamIndex].R = 220;
            }
            else
            {
                WeaponBarIcon[i].Tints[TeamIndex].A = 255;
                WeaponBarIcon[i].Tints[TeamIndex].R = 255;
                WeaponBarIcon[i].Tints[TeamIndex].B = 255;
                WeaponBarIcon[i].Tints[TeamIndex].G = 255;
            }
        }

        DrawSpriteWidget( C, WeaponBarAmmoFill[i] );
        DrawSpriteWidget( C, WeaponBarTint[i] );
        DrawSpriteWidget( C, WeaponBarTrim[i] );
        DrawSpriteWidget( C, WeaponBarIcon[i] );
    }
}

simulated function UpdateHud()
{
    if ((PawnOwnerPRI != none) && (PawnOwnerPRI.Team != None))
        TeamIndex = Clamp (PawnOwnerPRI.Team.TeamIndex, 0, 1);
    else
        TeamIndex = 1; // Default to the blue HUD because it's sexier

    // Update values ===============================================================================

    CalculateScore();
    // Score.Value = CurScore;

    CalculateHealth();
    LHud2[2].Scale = FClamp(CurHealth / MaxHealth,0.0,1.0);
    HealthCount.Value = CurHealth;

    HealthIcon.Tints[TeamIndex].R = 255 - ( 255 * ( FClamp( CurHealth / MaxHealth, 0.0, 1.0) ));
    HealthIcon.Tints[TeamIndex].G = 0;
    HealthIcon.Tints[TeamIndex].B = 255;

    CalculateShield();
    ShieldCount.Value = CurShield;

    CalculateEnergy();
    Adrenaline[2].Scale = CurEnergy / MaxEnergy;
    if(CurEnergy == MaxEnergy)
        AdrenalineReady = true;
    else
        AdrenalineReady = false;

    CalculateAmmo();
    AmmoCount.Value = CurAmmoPrimary;
    if(!TeamLinked)
    {
        RHud2[3].Tints[TeamIndex].R = 255 - ( 255 * ( FClamp( CurAmmoPrimary / MaxAmmoPrimary, 0.0, 1.0) ));
        RHud2[3].Tints[TeamIndex].G = ( 255 * ( FClamp( CurAmmoPrimary / MaxAmmoPrimary, 0.0, 1.0) ));
        RHud2[3].Tints[TeamIndex].B = 0; 
    }
    Super.UpdateHud ();
}

function bool DrawLevelAction (Canvas C)
{
    local String LevelActionText;

    if ((Level.LevelAction == LEVACT_None) && (Level.Pauser != none))
    {
        LevelActionText = LevelActionPaused;
    }
    else if ((Level.LevelAction == LEVACT_Loading) || (Level.LevelAction == LEVACT_Precaching))
        LevelActionText = LevelActionLoading;
    else
        LevelActionText = "";

    if (LevelActionText == "")
        return (false);

    C.Font = LevelActionFont;
    C.DrawColor = LevelActionFontColor;
    C.Style = ERenderStyle.STY_Alpha;
    C.ColorModulate = C.default.ColorModulate;

    C.DrawScreenText (LevelActionText, LevelActionPositionX, LevelActionPositionY, DP_MiddleMiddle);
    return (true);
}

function DisplayPortrait(PlayerReplicationInfo PRI)
{
	local Material NewPortrait;
	
	NewPortrait = PRI.GetPortrait();
	if ( NewPortrait == None )
		return;
	if ( Portrait == None )
		PortraitX = 1;
	Portrait = NewPortrait;
	PortraitTime = Level.TimeSeconds + 3;
	PortraitPRI = PRI;
}

simulated function Message( PlayerReplicationInfo PRI, coerce string Msg, name MsgType )
{
	Super.Message(PRI,Msg,MsgType);
    if ( (MsgType == 'Say') || (MsgType == 'TeamSay') )
        DisplayPortrait(PRI);
}

defaultproperties
{
    ReloadingTrim=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=450,Y1=415,X1=836,Y2=453),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-137,OffsetY=15,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    ReloadingTeamTint=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=450,Y1=454,X1=836,Y2=490),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-137,OffsetY=15,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    ReloadingFill=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=450,Y1=454,X1=836,Y2=490),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-137,OffsetY=15,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=0,A=255),Tints[1]=(R=255,G=255,B=0,A=255))

    UDamageTrim=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=450,Y1=453,X1=836,Y2=415),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=137,OffsetY=15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    UDamageTeamTint=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=450,Y1=490,X1=836,Y2=454),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=137,OffsetY=15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    UDamageFill=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=450,Y1=490,X1=836,Y2=454),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=137,OffsetY=15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=0,B=255,A=255),Tints[1]=(R=255,G=0,B=255,A=255))

    // Digits
    DigitsBig=(DigitTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords[0]=(X1=100,Y1=400,X2=199,Y2=499),TextureCoords[1]=(X1=0,Y1=0,X2=99,Y2=99),TextureCoords[2]=(X1=100,Y1=0,X2=199,Y2=99),TextureCoords[3]=(X1=0,Y1=100,X2=99,Y2=199),TextureCoords[4]=(X1=100,Y1=100,X2=199,Y2=199),TextureCoords[5]=(X1=0,Y1=200,X2=99,Y2=299),TextureCoords[6]=(X1=100,Y1=200,X2=199,Y2=299),TextureCoords[7]=(X1=0,Y1=300,X2=99,Y2=399),TextureCoords[8]=(X1=100,Y1=300,X2=199,Y2=399),TextureCoords[9]=(X1=0,Y1=400,X2=99,Y2=499),TextureCoords[10]=(X1=200,Y1=0,X2=299,Y2=99)))//(X1=100,Y1=400,X2=199,Y2=499)))

    // HUD ELEMENTS -left
    LHud1[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=763,X2=298,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255)) // -- Hud Border
    LHud1[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=299,Y1=763,X2=454,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150)) // -- Hud Color
    LHud1[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=455,Y1=763,X2=610,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200)) // -- Hud Fill
    LHud1[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=0,Y1=880,X2=142,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=85,OffsetY=50,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255)) // -- Hud Cirlce
    
    LHud2[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=611,Y1=900,X2=979,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=180,OffsetY=60,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    LHud2[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=611,Y1=777,X2=979,Y2=899),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=180,OffsetY=60,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    LHud2[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=611,Y1=654,X2=979,Y2=776),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=180,OffsetY=60,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    LHud2[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=810,Y1=200,X2=1023,Y2=413),TextureScale=0.25,DrawPivot=DP_UpperLeft,PosX=0.0,PosY=0.835,OffsetX=90,OffsetY=35,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=0,A=100),Tints[1]=(R=255,G=255,B=0,A=100))
    
    // - right
    RHud1[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=143,Y1=763,X1=298,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    RHud1[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=299,Y1=763,X1=454,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    RHud1[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=455,Y1=763,X1=610,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    RHud1[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=0,Y1=880,X1=142,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-85,OffsetY=50,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    
    RHud2[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=611,Y1=900,X1=979,Y2=1023),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-180,OffsetY=60,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    RHud2[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=611,Y1=777,X1=979,Y2=899),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-180,OffsetY=60,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    RHud2[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=611,Y1=654,X1=979,Y2=776),TextureScale=0.3,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-180,OffsetY=60,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    RHud2[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=810,Y1=200,X2=1023,Y2=413),TextureScale=0.25,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0.835,OffsetX=-68,OffsetY=37,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    
    // Adrenaline
    Adrenaline[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=611,Y1=900,X1=979,Y2=1023),TextureScale=0.23,DrawPivot=DP_UpperRight,PosX=1,PosY=0,OffsetX=-95,OffsetY=10,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Adrenaline[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=611,Y1=777,X1=979,Y2=899),TextureScale=0.23,DrawPivot=DP_UpperRight,PosX=1,PosY=0,OffsetX=-95,OffsetY=10,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    Adrenaline[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X2=611,Y1=654,X1=979,Y2=776),TextureScale=0.23,DrawPivot=DP_UpperRight,PosX=1,PosY=0,OffsetX=-95,OffsetY=10,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    Adrenaline[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=0,Y1=880,X2=142,Y2=1023),TextureScale=0.23,DrawPivot=DP_UpperRight,PosX=1,PosY=0,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Adrenaline[4]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=810,Y1=200,X2=1023,Y2=413),TextureScale=0.2,DrawPivot=DP_UpperRight,PosX=1,PosY=0,OffsetX=35,OffsetY=-28,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=0,A=0),Tints[1]=(R=255,G=255,B=0,A=0))
    
    ScoreBg[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=611,Y1=900,X2=979,Y2=1023),TextureScale=0.23,DrawPivot=DP_UpperLeft,PosX=0,PosY=0,OffsetX=95,OffsetY=10,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    ScoreBg[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=611,Y1=777,X2=979,Y2=899),TextureScale=0.23,DrawPivot=DP_UpperLeft,PosX=0,PosY=0,OffsetX=95,OffsetY=10,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    ScoreBg[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=611,Y1=654,X2=979,Y2=776),TextureScale=0.23,DrawPivot=DP_UpperLeft,PosX=0,PosY=0,OffsetX=95,OffsetY=10,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    ScoreBg[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=0,Y1=880,X2=142,Y2=1023),TextureScale=0.23,DrawPivot=DP_UpperLeft,PosX=0,PosY=0,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))

    // Icons
    HealthIcon=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=0,Y1=750,X2=142,Y2=879),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0,PosY=0.835,OffsetX=82,OffsetY=55,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=0,G=255,B=0,A=255),Tints[1]=(R=0,G=255,B=0,A=255))
    ShieldIcon=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=458,Y1=584,X2=583,Y2=749),TextureScale=0.25,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.835,OffsetX=0,OffsetY=130,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    ShieldIconGlow=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=333,Y1=584,X2=457,Y2=749),TextureScale=0.26,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.835,OffsetX=-1,OffsetY=125,ScaleMode=SM_Left,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    AdrenalineIcon=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=0,Y1=620,X2=142,Y2=749),TextureScale=0.23,DrawPivot=DP_UpperRight,PosX=1,PosY=0,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    
    Crosshairs[0]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Cross1',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.75,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[1]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Cross2',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.75,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[2]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Cross3',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.75,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[3]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Cross4',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.75,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[4]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Cross5',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.75,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[5]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Dot',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.75,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[6]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Pointer',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.6,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[7]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Triad1',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.7,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[8]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Triad2',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.7,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[9]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Triad3',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.7,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[10]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Bracket1',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.6,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[11]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Bracket2',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.6,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[12]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Circle1',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.4,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    Crosshairs[13]=(WidgetTexture=Material'Crosshairs.Hud.Crosshair_Circle2',TextureCoords=(X1=0,Y1=0,X2=64,Y2=64),TextureScale=0.4,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.5,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    
    AdrenalineCount=(TextureScale=0.18,DrawPivot=DP_UpperRight,PosX=1.0,PosY=0,OffsetX=-260,OffsetY=40,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    myScore=(TextureScale=0.18,MinDigitCount=2,DrawPivot=DP_UpperRight,PosX=0,PosY=0,OffsetX=560,OffsetY=40,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    mySpread=(TextureScale=0.09,MinDigitCount=2,DrawPivot=DP_UpperRight,PosX=0,PosY=0,OffsetX=655,OffsetY=135,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    myRank=(TextureScale=0.15,MinDigitCount=2,DrawPivot=DP_UpperRight,PosX=0,PosY=0,OffsetX=150,OffsetY=40,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))

    HealthCount=(TextureScale=0.25,DrawPivot=DP_MiddleRight,PosX=0,PosY=0.835,OffsetX=620,OffsetY=145,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    ShieldCount=(TextureScale=0.25,DrawPivot=DP_MiddleMiddle,PosX=0.5,PosY=0.835,OffsetX=0,OffsetY=145,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    AmmoCount=(TextureScale=0.25,DrawPivot=DP_UpperRight,PosX=1,PosY=0.835,OffsetX=-340,OffsetY=95,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    
    // Tint, TeamColor, Fill display for Weapons
    WeaponBarAmmoFill[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.10,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.19,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.28,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.37,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[4]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.46,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[5]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.55,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[6]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.64,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[7]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.73,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))
    WeaponBarAmmoFill[8]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.82,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=200),Tints[1]=(R=48,G=75,B=120,A=200))

    WeaponBarTint[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.10,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.19,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.28,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.37,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[4]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.46,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[5]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.55,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[6]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.64,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[7]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.73,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))
    WeaponBarTint[8]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=678,X2=332,Y2=749),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.82,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=100,G=0,B=0,A=100),Tints[1]=(R=37,G=66,B=102,A=150))

    WeaponBarTrim[0]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.10,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[1]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.19,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[2]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.28,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[3]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.37,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[4]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.46,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[5]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.55,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[6]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.64,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[7]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.73,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))
    WeaponBarTrim[8]=(WidgetTexture=Material'InterfaceContent.Hud.SkinA',TextureCoords=(X1=143,Y1=533,X2=332,Y2=605),TextureScale=0.3,DrawPivot=DP_UpperLeft,PosX=0.82,PosY=0.95,OffsetX=0,OffsetY=0,ScaleMode=SM_None,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))

    WeaponBarIcon[0]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.10,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[1]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.19,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[2]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.28,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[3]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.37,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[4]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.46,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[5]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.55,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[6]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.64,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[7]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.73,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))
    WeaponBarIcon[8]=(TextureScale=0.35,DrawPivot=DP_UpperLeft,PosX=0.82,PosY=0.95,OffsetX=18,OffsetY=-15,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=50),Tints[1]=(R=255,G=255,B=255,A=50))

    AmmoIcon=(TextureScale=0.45,DrawPivot=DP_UpperLeft,PosX=1.0,PosY=0.835,OffsetX=-151,OffsetY=44,ScaleMode=SM_Right,Scale=1.0,RenderStyle=STY_Alpha,Tints[0]=(R=255,G=255,B=255,A=255),Tints[1]=(R=255,G=255,B=255,A=255))

    BaseWeapons[0]=class'ShieldGun'
    BaseWeapons[1]=class'AssaultRifle'
    BaseWeapons[2]=class'BioRifle'
    BaseWeapons[3]=class'ShockRifle'
    BaseWeapons[4]=class'LinkGun'
    BaseWeapons[5]=class'MiniGun'
    BaseWeapons[6]=class'FlakCannon'
    BaseWeapons[7]=class'RocketLauncher'
    BaseWeapons[8]=class'SniperRifle'

    LevelActionLoading="LOADING..."
    LevelActionPaused="PAUSED"
    LevelActionFont=Font'FontMedium'
    LevelActionFontColor=(R=255,G=255,B=255,A=255)

    CurrentWeaponPositionX=0.845
    CurrentWeaponPositionY=0.9

    LevelActionPositionX=0.5
    LevelActionPositionY=0.25

    ConsoleMessagePosX=0.005
    ConsoleMessagePosY=0.825

    LogoTexture=Material'InterfaceContent.Logos.Logo'
    LogoScaleX=0.25
    LogoScaleY=0.25
    LogoPosX=0.49
    LogoPosY=0.15

	CountDown(0)=sound'AnnouncerMain.One'
	CountDown(1)=sound'AnnouncerMain.Two'
	CountDown(2)=sound'AnnouncerMain.03_2'
	CountDown(3)=sound'AnnouncerMain.Four'
	CountDown(4)=sound'AnnouncerMain.Five'
	CountDown(5)=sound'AnnouncerMain.Six'
	CountDown(6)=sound'AnnouncerMain.Seven'
	CountDown(7)=sound'AnnouncerMain.Eight'
	CountDown(8)=sound'AnnouncerMain.Nine'
	CountDown(9)=sound'AnnouncerMain.Ten'
	
	LongCount(0)=sound'AnnouncerMain.5_minute_warning'
	LongCount(1)=sound'AnnouncerMain.3_minutes_remain'
	LongCount(2)=sound'AnnouncerMain.2_minutes_remain'
	LongCount(3)=sound'AnnouncerMain.1_minute_remains'
	LongCount(4)=sound'AnnouncerMain.30_seconds_remain'
	LongCount(5)=sound'AnnouncerMain.20_seconds'
	
	WonMatchPrefix=""
	WonMatchPostFix=" won the match!"
	
	YouveWonTheMatch="You've won the match!"
	YouveLostTheMatch="You've lost the match."
	WaitingToSpawn="Press [Fire] to join the match!"
}
