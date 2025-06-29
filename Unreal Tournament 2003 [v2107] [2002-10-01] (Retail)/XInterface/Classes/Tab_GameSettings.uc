// ====================================================================
//  Class:  XInterface.Tab_DetailSettings
//  Parent: XInterface.GUITabPanel
//
//  <Enter a description here>
// ====================================================================

class Tab_GameSettings extends GUITabPanel;

var localized string	CrosshairNames[15];

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{

	local int i;
	Super.Initcomponent(MyController, MyOwner);

	for (i=0;i<Controls.Length;i++)
		Controls[i].OnChange=InternalOnChange;

	for(i = 0;i < ArrayCount(CrosshairNames);i++)
		moComboBox(Controls[8]).AddItem(CrosshairNames[i]);
	moComboBox(Controls[8]).ReadOnly(true);

	if (PlayerOwner().Level.Game.bAlternateMode)
		Controls[4].bVisible = false;
}

function InternalOnLoadINI(GUIComponent Sender, string s)
{

	local int i;

	if (Sender==Controls[2])
		moCheckBox(Sender).Checked(bool(s));
		
	else if (Sender==Controls[3])
		moCheckBox(Sender).Checked(class'Pawn'.default.bWeaponBob);
		
	else if (Sender==Controls[4])
		moCheckBox(Sender).Checked(class'GameInfo'.static.UseLowGore());

	else if (Sender==controls[5])
		moCheckBox(Sender).Checked(PlayerOwner().DodgingIsEnabled());
		
	else if (Sender==Controls[6])
		moCheckBox(Sender).Checked(class'GameInfo'.default.AutoAim > 0);
		
	else if (Sender==Controls[7])
		moCheckBox(Sender).Checked(class'HUD'.Default.bHideHUD);
		
	else if (Sender==Controls[8])
	{
	
		if (!class'HUD'.default.bCrosshairShow )
			i = 0;
		else
			i = class'HUD'.default.CrosshairStyle +1;
		
		moComboBox(Sender).SetText(CrosshairNames[i]);
		SetCrossHairGraphic(i);
	}		

	else if (Sender==Controls[11])
		moCheckBox(Sender).Checked(class'xGame.xDeathMessage'.default.bNoConsoleDeathMessages);
}		
		 
function string InternalOnSaveINI(GUIComponent Sender); 		// Do the actual work here

function InternalOnChange(GUIComponent Sender)
{
	local bool b;

	if (!Controller.bCurMenuInitialized)
		return;
		
	if (Sender==Controls[2])
		PlayerOwner().ConsoleCommand("set"@Sender.INIOption@moCheckBox(Sender).IsChecked());
		
	else if (Sender==Controls[3])
	{
		PlayerOwner().ConsoleCommand("set Pawn bWeaponBob "$moCheckBox(Sender).IsChecked());
		class'Pawn'.default.bWeaponBob = moCheckBox(Sender).IsChecked();
		class'Pawn'.static.StaticSaveConfig();
	}
		
	else if (Sender==Controls[4])
	{
		if (moCheckBox(Sender).IsChecked())
			class'GameInfo'.default.GoreLevel = 1;
		else
			class'GameInfo'.default.GoreLevel = 0;
			
		class'GameInfo'.static.StaticSaveConfig();
	}	

	else if (Sender==controls[5])
	{
		if( !moCheckBox(Sender).IsChecked() )
			PlayerOwner().SetDodging( false );
		else
			PlayerOwner().SetDodging( true );
	}
		
	else if (Sender==Controls[6])
	{
		if ( moCheckBox(Sender).IsChecked() )
			class'GameInfo'.Default.AutoAim = 0.5;
		else
			class'GameInfo'.Default.AutoAim = 0;

		class'GameInfo'.static.StaticSaveConfig();
	}			
		
	else if (Sender==Controls[7])
	{
		if ( moCheckBox(Sender).IsChecked() )
		{
			PlayerOwner().ConsoleCommand("set HUD bHideHud True");
			if (PlayerOwner().MyHud.bHideHud)
				PlayerOwner().consolecommand("showhud");
		}
		else
		{
			PlayerOwner().ConsoleCommand("set HUD bHideHud False");
			if (!PlayerOwner().MyHud.bHideHud)
				PlayerOwner().consolecommand("showhud");
		}
	}		
		
	else if (Sender==Controls[8])
	{
		SetCrossHairGraphic(moComboBox(Sender).GetIndex());
	
		if (moComboBox(Sender).GetText() == "Hidden")
		{	
			PlayerOwner().MyHud.bCrosshairShow = false;
			PlayerOwner().MyHud.SaveConfig();

			return;
		}
		
		PlayerOwner().MyHud.bCrosshairShow = true;
		PlayerOwner().MyHud.CrosshairStyle = moComboBox(Sender).GetIndex()-1;
		PlayerOwner().MyHud.SaveConfig();		
	}

	else if (Sender==Controls[11])
	{
		Log("xgame bNoConsoleDeathMessages was="$class'xgame.xdeathmessage'.default.bNoConsoleDeathMessages);
		b = moCheckBox(Sender).IsChecked();
		class'XGame.xDeathMessage'.default.bNoConsoleDeathMessages=b;
		Log("xgame bNoConsoleDeathMessages now="$class'xgame.xdeathmessage'.default.bNoConsoleDeathMessages);
		class'XGame.xDeathMessage'.static.StaticSaveConfig();
		Log("xgame bNoConsoleDeathMessages then="$class'xgame.xdeathmessage'.default.bNoConsoleDeathMessages);
	}		

}			

function SetCrossHairGraphic(int Index)
{
	local GUIImage img;
	
	Img = GUIImage(Controls[9]);

	Img.Image = class'HudBDeathMatch'.default.Crosshairs[Index - 1].WidgetTexture;
	Img.bVisible = (Index!=0);
	
}		

defaultproperties
{

	Begin Object class=GUIImage Name=GameBK
		WinWidth=0.427148
		WinHeight=0.803125
		WinLeft=0.029297
		WinTop=0.130208
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(0)=GUIImage'GameBK'

	Begin Object class=GUIImage Name=GameBK1
		WinWidth=0.448633
		WinHeight=0.803125
		WinLeft=0.517578
		WinTop=0.130208
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(1)=GUIImage'GameBK1'

	Begin Object class=moCheckBox Name=GameScreenFlashes
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.050000
		WinTop=0.168385
		Caption="Screen Flashes"
		INIOption="ini:Engine.Engine.ViewportManager ScreenFlashes"
		OnLoadINI=InternalOnLoadINI
		Hint="Turn this option off to disable screen flashes when you take damage."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(2)=moCheckbox'GameScreenFlashes'		

	Begin Object class=moCheckBox Name=GameWeaponBob
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.050000
		WinTop=0.311614
		Caption="Weapon Bob"
		INIOption="@Internal"
		OnLoadINI=InternalOnLoadINI
		Hint="Prevent your weapon from bobbing up and down while moving."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(3)=moCheckbox'GameWeaponBob'		
	
	Begin Object class=moCheckBox Name=GameReduceGore
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.050000
		WinTop=0.457187
		Caption="Reduce Gore"
		INIOption="ini:Engine.Engine.AudioDevice ReverseStereo"
		OnLoadINI=InternalOnLoadINI
		Hint="Turn this option On to reduce the amount of blood and guts you see."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(4)=moCheckbox'GameReduceGore'		

	Begin Object class=moCheckBox Name=GameDodging
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.050000
		WinTop=0.604063
		Caption="Dodging"
		INIOption="ini:Engine.Engine.AudioDevice ReverseStereo"
		OnLoadINI=InternalOnLoadINI
		Hint="Toggling this option will toggle your ability to perform dodge moves in the game."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(5)=moCheckbox'GameDodging'		

	Begin Object class=moCheckBox Name=GameAutoAim
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.050000
		WinTop=0.754844
		Caption="Auto Aim"
		INIOption="ini:Engine.Engine.AudioDevice ReverseStereo"
		OnLoadINI=InternalOnLoadINI
		Hint="Turn this option on to have UT2003 help you aim (not available in Multiplayer)."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(6)=moCheckbox'GameAutoAim'		

	Begin Object class=moCheckBox Name=GameHudVisible
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.535547
		WinTop=0.168906
		Caption="Hide HUD"
		INIOption="@Internal"
		OnLoadINI=InternalOnLoadINI
		Hint="This option will toggle drawing of the HUD."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(7)=moCheckbox'GameHudVisible'		

	Begin Object class=moComboBox Name=GameCrossHair
		WinWidth=0.383398
		WinHeight=0.060000
		WinLeft=0.536523
		WinTop=0.249219
		Caption="Crosshair"
		INIOption="@Internal"
		OnLoadINI=InternalOnLoadINI
		Hint="Please select your crosshair!"
		CaptionWidth=0.3
		ComponentJustification=TXTA_Left
	End Object
	Controls(8)=moComboBox'GameCrossHair'		
			
	Begin Object class=GUIImage Name=GameCrossHairImage
		WinWidth=0.420000
		WinHeight=0.800000
		WinLeft=0.58
		WinTop=0.4
		X1=0
		Y1=0
		X2=64
		Y2=64
		ImageColor=(R=255,G=255,B=255,A=255);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Normal
		ImageAlign=IMGA_Center
	End Object
	Controls(9)=GUIImage'GameCrossHairImage'

	Begin Object class=GUIImage Name=CrosshairBK
		WinWidth=0.090000
		WinHeight=0.150000
		WinLeft=0.535000
		WinTop=0.325
		Image=Material'InterfaceContent.Menu.BorderBoxA'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(10)=GUIImage'CrosshairBK'

	Begin Object class=moCheckBox Name=GameDeathMsgs
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.536523
		WinTop=0.604063
		Caption="No Console Death Messages"
		INIOption="ini:Engine.XGame.xDeathMessage bNoConsoleDeathMessages"
		INIDefault="False"
		OnLoadINI=InternalOnLoadINI
		OnSaveINI=InternalOnSaveINI
		Hint="Turn off reporting of death messages in console"
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(11)=moCheckbox'GameDeathMsgs'		
			
	WinTop=0.15
	WinLeft=0
	WinWidth=1
	WinHeight=0.77
	bAcceptsInput=false		

	CrosshairNames(0)="Hidden"
	CrosshairNames(1)="Cross (1)"
	CrosshairNames(2)="Cross (2)"
	CrosshairNames(3)="Cross (3)"
	CrosshairNames(4)="Cross (4)"
	CrosshairNames(5)="Cross (5)"
	CrosshairNames(6)="Dot"
	CrosshairNames(7)="Pointer"
	CrosshairNames(8)="Triad (1)"
	CrosshairNames(9)="Triad (2)"
	CrosshairNames(10)="Triad (3)"
	CrosshairNames(11)="Bracket (1)"
	CrosshairNames(12)="Bracket (2)"
	CrosshairNames(13)="Circle (1)"
	CrosshairNames(14)="Circle (2)"
}
