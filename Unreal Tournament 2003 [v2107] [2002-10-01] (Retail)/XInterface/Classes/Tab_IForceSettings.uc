// ====================================================================
//  Class:  XInterface.Tab_IForceSettings
//  Parent: XInterface.GUITabPanel
//
//  <Enter a description here>
// ====================================================================

class Tab_IForceSettings extends GUITabPanel;

var moCheckBox AutoAim;
var moCheckBox AutoSlope;
var moCheckBox InvertMouse;
var moCheckBox MouseSmooth;
var moCheckBox UseJoystick;
var GUISlider  MouseSens;
var GUISlider  MenuSens;
var moCheckBox ifWeapon;
var moCheckBox ifPickup;
var moCheckBox ifDamage;
var moCheckBox ifGUI;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{

	local int i;
	Super.Initcomponent(MyController, MyOwner);

	for (i=0;i<Controls.Length;i++)
		Controls[i].OnChange=InternalOnChange;

	AutoAim  	= moCheckBox(Controls[1]); AutoAim.Checked(PlayerOwner().Level.Game.AutoAim==1);
	AutoSlope	= moCheckBox(Controls[2]); AutoSlope.Checked(PlayerOwner().bSnapToLevel);
	InvertMouse = moCheckBox(Controls[3]); InvertMouse.Checked(class'PlayerInput'.default.bInvertMouse);
	MouseSmooth = moCheckBox(Controls[4]); MouseSmooth.Checked(class'PlayerInput'.default.MouseSmoothingMode>0);
	UseJoystick = moCheckBox(Controls[5]); UseJoystick.Checked(bool(PlayerOwner().ConsoleCommand("get ini:Engine.Engine.ViewportManager UseJoystick")));
	
	MouseSens	= GUISlider(Controls[7]); MouseSens.SetValue(class'PlayerInput'.default.MouseSensitivity);
	MenuSens	= GUISlider(Controls[9]); MenuSens.SetValue(Controller.MenuMouseSens);

	ifWeapon = moCheckBox(Controls[11]); ifWeapon.Checked(class'PlayerController'.default.bEnableWeaponForceFeedback );
	ifPickup = moCheckBox(Controls[12]); ifPickup.Checked(class'PlayerController'.default.bEnablePickupForceFeedback );
	ifDamage = moCheckBox(Controls[13]); ifDamage.Checked(class'PlayerController'.default.bEnableDamageForceFeedback );
	ifGUI	 = moCheckBox(Controls[14]); ifGUI.Checked(class'PlayerController'.default.bEnableGUIForceFeedback );		
		
}

function InternalOnChange(GUIComponent Sender)
{
	if (!Controller.bCurMenuInitialized)
		return;
		
	if (Sender==AutoAim)
	{
		if ( AutoAim.IsChecked() )
			PlayerOwner().Level.Game.AutoAim = 1.0;
		else
			PlayerOwner().Level.Game.AutoAim = 0;
			
		PlayerOwner().Level.Game.SaveConfig();
	}
	
	if (Sender==AutoSlope)
	{
		PlayerOwner().bSnapToLevel = AutoSlope.IsChecked();
		PlayerOwner().SaveConfig();
	}
	
	if (Sender==InvertMouse)
	{
		PlayerOwner().ConsoleCommand("set PlayerInput bInvertMouse "$InvertMouse.IsChecked());
		class'PlayerInput'.default.bInvertMouse = InvertMouse.IsChecked();
		class'PlayerInput'.static.StaticSaveConfig();
	}
	
	if (Sender==MouseSmooth)
	{
		if ( MouseSmooth.IsChecked() )
			class'PlayerInput'.default.MouseSmoothingMode = 1;
		else
			class'PlayerInput'.default.MouseSmoothingMode = 0;
	}
	
	if (Sender==UseJoystick)
	{
		PlayerOwner().ConsoleCommand("set ini:Engine.Engine.ViewportManager UseJoystick");
	}
	
	if (Sender==MouseSens)
	{
		class'PlayerInput'.default.MouseSensitivity = MouseSens.Value;
		PlayerOwner().ConsoleCommand("set PlayerInput MouseSensitivity "$MouseSens.Value);
		class'PlayerInput'.static.StaticSaveConfig();
	}
	
	if (Sender==MenuSens)
	{
		Controller.MenuMouseSens = MenuSens.Value;
		Controller.SaveConfig();
	}		
		
	if (Sender==ifWeapon)
	{
		PlayerOwner().bEnableWeaponForceFeedback = ifWeapon.IsChecked();
		class'PlayerController'.SaveConfig();
	}

	if (Sender==ifPickup)
	{
		PlayerOwner().bEnablePickupForceFeedback = ifPickup.IsChecked();
		class'PlayerController'.SaveConfig();
	}

	if (Sender==ifDamage)
	{
		PlayerOwner().bEnableDamageForceFeedback = ifDamage.IsChecked();
		class'PlayerController'.SaveConfig();
	}

	if (Sender==ifGUI)
	{
		PlayerOwner().bEnableGUIForceFeedback = ifGUI.IsChecked();
		class'PlayerController'.SaveConfig();
	}
}			

defaultproperties
{
	Begin Object class=GUIImage Name=InputBK1
		WinWidth=0.381328
		WinHeight=0.636485
		WinLeft=0.021641
		WinTop=0.106510
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(0)=GUIImage'InputBK1'
	
	Begin Object class=moCheckBox NAme=InputAutoAim
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.060938
		WinTop=0.057396
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Auto Aim"
		Hint="Enabling this option will activate computer-assisted aiming in single player games."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
		bVisible=false;
	End Object
	Controls(1)=moCheckBox'InputAutoAim'		

	Begin Object class=moCheckBox NAme=InputAutoSlope
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.060937
		WinTop=0.175989
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Auto Slope"
		Hint="When enabled, your view will automatically pitch up/down when on a slope."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(2)=moCheckBox'InputAutoSlope'		
	
	Begin Object class=moCheckBox NAme=InputInvertMouse
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.060938
		WinTop=0.321927
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Invert Mouse"
		Hint="When enabled, the Y axis of your mouse will be inverted."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(3)=moCheckBox'InputInvertMouse'		
	
	Begin Object class=moCheckBox NAme=InputMouseSmoothing
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.060938
		WinTop=0.478541
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Mouse Smoothing"
		Hint="Enable this option to automatically smooth out movements in your mouse."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(4)=moCheckBox'InputMouseSmoothing'
		
	Begin Object class=moCheckBox NAme=InputUseJoystick
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.060938
		WinTop=0.651302
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Enable Joystick"
		Hint="Enable this option to enable joystick support."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(5)=moCheckBox'InputUseJoystick'		


	Begin Object class=GUILabel Name=InputSliderLabel1
		Caption="Mouse Sensitivity (In Game)"
		TextALign=TXTA_Center
		TextColor=(R=255,G=255,B=255,A=255)
		WinWidth=0.332422
		WinHeight=32.000000
		WinLeft=0.543945
		WinTop=0.126042
	End Object
	Controls(6)=GUILabel'InputSliderLabel1'

	Begin Object class=GUISlider Name=InputMouseSensitivity
		WinWidth=0.250000
		WinLeft=0.580078
		WinTop=0.188698
		MinValue=1
		MaxValue=25
		Hint="Adjust mouse sensitivity"
	End Object
	Controls(7)=GUISlider'InputMouseSensitivity'
	
	Begin Object class=GUILabel Name=InputSliderLabel2
		Caption="Mouse Sensitivity (Menus)"
		TextALign=TXTA_Center
		TextColor=(R=255,G=255,B=255,A=255)
		WinWidth=0.308594
		WinHeight=32.000000
		WinLeft=0.548828
		WinTop=0.327865
	End Object
	Controls(8)=GUILabel'InputSliderLabel2'
	
	Begin Object class=GUISlider Name=InputMenuSensitivity
		WinWidth=0.250000
		WinLeft=0.580078
		WinTop=0.403542
		MinValue=1
		MaxValue=6
		Hint="Adjust mouse speed within the menus"
	End Object
	Controls(9)=GUISlider'InputMenuSensitivity'
	
	Begin Object class=GUIImage Name=InputBK2
		WinWidth=0.957500
		WinHeight=0.240977
		WinLeft=0.021641
		WinTop=0.76
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(10)=GUIImage'InputBK2'
	
	Begin Object class=moCheckBox NAme=InputIFWeaponEffects
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.100000
		WinTop=0.852
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Weapon Effects"
		Hint="Turn this option On/Off to feel the weapons you fire."
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(11)=moCheckBox'InputIFWeaponEffects'		
	
	Begin Object class=moCheckBox NAme=InputIFPickupEffects
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.100000
		WinTop=0.933
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Pickup Effects"
		Hint="Turn this option On/Off to feel the items you pick up."
		CaptionWidth=0.9		
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(12)=moCheckBox'InputIFPickupEffects'
	
	Begin Object class=moCheckBox NAme=InputIFDamageEffects
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.563867
		WinTop=0.852
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="Damage Effects"
		Hint="Turn this option On/Off to feel the damage you take."
		CaptionWidth=0.9		
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(13)=moCheckBox'InputIFDamageEffects'
	
	Begin Object class=moCheckBox NAme=InputIFGUIEffects
		WinWidth=0.300000
		WinHeight=0.040000
		WinLeft=0.563867
		WinTop=0.933
		ComponentClassName="xinterface.GUICheckBoxButton"
		Caption="GUI Effects"
		Hint="Turn this option On/Off to feel the GUI."
		CaptionWidth=0.9		
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(14)=moCheckBox'InputIFGUIEffects'

	Begin Object class=GUILabel Name=InputIForceLabel
		Caption="I-Force Force Feedback"
		TextALign=TXTA_Center
		TextColor=(R=255,G=255,B=255,A=255)
		WinWidth=0.276563
		WinHeight=32.000000
		WinLeft=0.367187
		WinTop=0.777
	End Object
	Controls(15)=GUILabel'InputIForceLabel'
	
			
	WinTop=0.15
	WinLeft=0
	WinWidth=1
	WinHeight=0.77
	bAcceptsInput=false		

}
