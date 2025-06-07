class Browser_Prefs extends Browser_Page;

var GUITitleBar		StatusBar;

var localized string	ViewStatsStrings[3];
var bool				bIsInitialised;

var localized string	NoMutString;
var localized string	AnyMutString;
var array<xUtil.MutatorRecord> MutatorRecords;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	local int i;

	Super.InitComponent(MyController, MyOwner);

	if(bIsInitialised)
		return;

	GUIButton(GUIPanel(Controls[0]).Controls[0]).OnClick=BackClick;
	
	// Set options for stats server viewing
	moComboBox(Controls[5]).AddItem(ViewStatsStrings[0]);
	moComboBox(Controls[5]).AddItem(ViewStatsStrings[1]);
	moComboBox(Controls[5]).AddItem(ViewStatsStrings[2]);
	moComboBox(Controls[5]).ReadOnly(true);

	// Load mutators into combobox
	class'xUtil'.static.GetMutatorList(MutatorRecords);

	moComboBox(Controls[6]).AddItem(AnyMutString, None, "Any");
	moComboBox(Controls[6]).AddItem(NoMutString, None, "None");

	for(i=0; i<MutatorRecords.Length; i++)
	{
		moComboBox(Controls[6]).AddItem(MutatorRecords[i].FriendlyName, None, MutatorRecords[i].ClassName);
	}
	moComboBox(Controls[6]).ReadOnly(true);

	for(i=1; i<8; i++)
	{
		Controls[i].OnLoadINI=MyOnLoadINI;
		Controls[i].OnChange=MyOnChange;
	}

	StatusBar = GUITitleBar(GUIPanel(Controls[0]).Controls[1]);

	bIsInitialised=true;
}

// delegates
function bool BackClick(GUIComponent Sender)
{
	Controller.CloseMenu(true);
	return true;		
} 

function MyOnLoadINI(GUIComponent Sender, string s)
{
	local int i;

	if(Sender == Controls[1])
		moCheckBox(Controls[1]).Checked(Browser.bOnlyShowStandard);
	else if(Sender == Controls[2])
		moCheckBox(Controls[2]).Checked(Browser.bOnlyShowNonPassword);
	else if(Sender == Controls[3])
		moCheckBox(Controls[3]).Checked(Browser.bDontShowFull);
	else if(Sender == Controls[4])
		moCheckBox(Controls[4]).Checked(Browser.bDontShowEmpty);
	else if(Sender == Controls[5])
		moComboBox(Controls[5]).SetText(ViewStatsStrings[Browser.StatsServerView]);
	else if(Sender == Controls[6])
	{
		if(Browser.DesiredMutator == "Any")
			moComboBox(Controls[6]).SetText(AnyMutString);
		else if(Browser.DesiredMutator == "None")
			moComboBox(Controls[6]).SetText(NoMutString);
		else
		{
			// Find the Mutator with this class name, and put its friendly name in the box
			for(i=0; i<MutatorRecords.Length; i++)
			{
				if( Browser.DesiredMutator == MutatorRecords[i].ClassName )
				{
					moComboBox(Controls[6]).SetText( MutatorRecords[i].FriendlyName );
					return;
				}
			}
		}
	}
	else if(Sender == Controls[7])
	{
		moEditBox(Controls[7]).SetText(Browser.CustomQuery);
	}
}

function MyOnChange(GUIComponent Sender)
{
	local string t;

	if(Sender == Controls[1])
		Browser.bOnlyShowStandard = moCheckBox(Controls[1]).IsChecked();
	else if(Sender == Controls[2])
		Browser.bOnlyShowNonPassword = moCheckBox(Controls[2]).IsChecked();
	else if(Sender == Controls[3])
		Browser.bDontShowFull = moCheckBox(Controls[3]).IsChecked();
	else if(Sender == Controls[4])
		Browser.bDontShowEmpty = moCheckBox(Controls[4]).IsChecked();
	else if(Sender == Controls[5])
	{
		t = moComboBox(Controls[5]).GetText();

		if(t == ViewStatsStrings[0])
			Browser.StatsServerView = SSV_Any;
		else if(t == ViewStatsStrings[1])
			Browser.StatsServerView = SSV_OnlyStatsEnabled;
		else if(t == ViewStatsStrings[2])
			Browser.StatsServerView = SSV_NoStatsEnabled;
	}
	else if(Sender == Controls[6])
	{
		Browser.DesiredMutator = moComboBox(Controls[6]).GetExtra();
	}
	else if(Sender == Controls[7])
	{
		Browser.CustomQuery = moEditBox(Controls[7]).GetText();
	}

	Browser.SaveConfig();
}

defaultproperties
{
	Begin Object Class=GUIButton Name=MyBackButton
		Caption="BACK"
		StyleName="SquareMenuButton"
		WinWidth=0.2
		WinLeft=0
		WinTop=0
		WinHeight=0.5
	End Object

	Begin Object class=GUITitleBar name=MyStatus
		WinWidth=1
		WinHeight=0.5
		WinLeft=0
		WinTop=0.5
		StyleName="SquareBar"
		Caption=""
		bUseTextHeight=false
		Justification=TXTA_Left
	End Object

	Begin Object Class=GUIPanel Name=FooterPanel
		Controls(0)=MyBackButton
		Controls(1)=MyStatus
		WinWidth=1
		WinHeight=0.1
		WinLeft=0
		WinTop=0.9
	End Object
	Controls(0)=GUIPanel'FooterPanel'

	Begin Object class=moCheckBox Name=OnlyStandardCheckBox
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.2
		WinTop=0.15
		Caption="Only Standard Servers"
		INIOption="@Internal"
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(1)=moCheckbox'OnlyStandardCheckBox'

	Begin Object class=moCheckBox Name=NoPasswdCheckBox
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.2
		WinTop=0.25
		Caption="Only Servers Without Passwords"
		INIOption="@Internal"
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(2)=moCheckbox'NoPasswdCheckBox'

	Begin Object class=moCheckBox Name=NoFullCheckBox
		WinWidth=0.400000
		WinHeight=0.040000
		WinLeft=0.2
		WinTop=0.35
		Caption="No Full Servers"
		INIOption="@Internal"
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(3)=moCheckbox'NoFullCheckBox'

	Begin Object class=moCheckBox Name=NoEmptyCheckBox
		WinWidth=0.4
		WinHeight=0.04
		WinLeft=0.200000
		WinTop=0.45
		Caption="No Empty Servers"
		INIOption="@Internal"
		CaptionWidth=0.9
		bSquare=true
		ComponentJustification=TXTA_Left
	End Object
	Controls(4)=moCheckbox'NoEmptyCheckBox'

	Begin Object class=moComboBox Name=StatsViewCombo
		WinWidth=0.720003
		WinHeight=0.04
		WinLeft=0.200000
		WinTop=0.55
		Caption="Stats Servers"
		INIOption="@INTERNAL"
		CaptionWidth=0.5
		ComponentJustification=TXTA_Left
	End Object
	Controls(5)=moComboBox'StatsViewCombo'	

	Begin Object class=moComboBox Name=MutatorCombo
		WinWidth=0.720003
		WinHeight=0.04
		WinLeft=0.200000
		WinTop=0.65
		Caption="Mutator"
		INIOption="@INTERNAL"
		CaptionWidth=0.5
		ComponentJustification=TXTA_Left
	End Object
	Controls(6)=moComboBox'MutatorCombo'	

	Begin Object class=moEditBox Name=CustomQuery
		WinWidth=0.720003
		WinHeight=0.04
		WinLeft=0.200000
		WinTop=0.7500000
		Caption="Custom Query"
		INIOption="@INTERNAL"
		CaptionWidth=0.5
	End Object
	Controls(7)=moEditBox'CustomQuery'

	Begin Object Class=GUILabel Name=FilterTitle
		WinWidth=0.720003
		WinHeight=0.056250
		WinLeft=0.1500000
		WinTop=0.05
		Caption="Server Filtering Options:"
		TextAlign=TXTA_Left
		TextColor=(R=230,G=200,B=0,A=255)
		TextFont="UT2HeaderFont"
	End Object
	Controls(8)=GUILabel'FilterTitle'


	ViewStatsStrings(0)="Any Servers"
	ViewStatsStrings(1)="Only Stats Servers"
	ViewStatsStrings(2)="No Stats Servers"

	NoMutString="No Mutators"
	AnyMutString="Any Mutator"
}