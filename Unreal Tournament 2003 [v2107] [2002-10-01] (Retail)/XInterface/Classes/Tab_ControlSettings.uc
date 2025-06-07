// ====================================================================
//  Class:  XInterface.Tab_ControlSettings
//  Parent: XInterface.GUITabPanel
//
//  <Enter a description here>
// ====================================================================

class Tab_ControlSettings extends GUITabPanel;

struct KeyBinding
{
	var 			bool 			bIsSectionLabel;
    var localized	string 			KeyLabel;
	var				string 			Alias;
	var 			array<int>		Binds;
	var				array<string>	BindKeyNames;
	var				array<string>	BindLocalizedKeyNames;
};

var GUIListBox	MyListBox;
var Color		BkColor;
var int NumBindings;
var localized KeyBinding Bindings[50];	// Holds the array of key bindings
var localized string Labels[50];	// fixed-length array for localization.  accesses clamped to [0,49]

var bool bSetNextKeyPress;
var	int	 NewIndex, NewSubIndex;
var GUIStyles SelStyle;

var int row, HackIndex;	// Hacky Hacky

var localized string Header,Footer;
var float SectionLabelMargin;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	local int i;
	Super.Initcomponent(MyController, MyOwner);

	MyListBox = GUIListBox(Controls[0]);
	MyListBox.List.OnDrawItem = DrawBinding;
	MyListBox.List.SelectedBkColor = BkColor;
	MyListBox.List.SelectedImage=None;
	MyListbox.List.OnClick=GetNewKey;
	MyListBox.List.OnChange=ListChange;
	MyListBox.List.OnKeyEvent = ListOnKeyEvent;
	MyListBox.List.bHotTrack=true;
	MyListBox.List.OnClickSound=CS_None;
	MyListBox.List.OnAdjustTop = MyOnAdjustTop;

	InitBindings();

	for (i=0;i<NumBindings;i++)
		MyListBox.List.Add(Bindings[i].KeyLabel);
	
	SelStyle = Controller.GetStyle("RoundButton");		
	MyListBox.List.Index=1;
	HackIndex=1;

			
}

function int Weight(int i)
{
	if ( (i==0x01) || (i==0x02) )
		return 100;
		
	if ( (i>=0x30) && (i<=0x5A) )
		return 50;

	if (i==0x20)
		return 45;
		
	if (i==0x04)
		return 40;

	if (i==0xEC || i==0xED)
		return 35;		
		
	if (i>=0x21 && i<=0x28)
		return 30;
		
	if (i>=0x60 && i<=0x6F)
		return 30;
			
	return 25;
}

function Swap(int index, int a, int b)
{
	local int TempInt;
	local string TempStrA, TempStrB;
	
	TempInt  = Bindings[Index].Binds[a];
	TempStrA = Bindings[Index].BindKeyNames[a];
	TempStrB = Bindings[Index].BindLocalizedKeyNames[a];
	
	Bindings[Index].Binds[a] = Bindings[Index].Binds[B];
	Bindings[Index].BindKeyNames[a] = Bindings[Index].BindKeyNames[b];
	Bindings[Index].BindLocalizedKeyNames[a] = Bindings[Index].BindLocalizedKeyNames[b];
	
	Bindings[Index].Binds[b] = TempInt;
	Bindings[Index].BindKeyNames[b] = TempStrA;
	Bindings[Index].BindLocalizedKeyNames[b] = TempStrB;
}
	

function InitBindings()
{
	local int i,j,k,index;
	local string KeyName, Alias, LocalizedKeyName;

	// Clear them all.
	
	for (i=0;i<NumBindings;i++)
	{
		if (Bindings[i].Binds.Length>0)
			Bindings[i].Binds.Remove(0,Bindings[i].Binds.Length);
			
		if (Bindings[i].BindKeyNames.Length>0)
			Bindings[i].BindKeyNames.Remove(0,Bindings[i].BindKeyNames.Length);
			
		if (Bindings[i].BindLocalizedKeyNames.Length>0)
			Bindings[i].BindLocalizedKeyNames.Remove(0,Bindings[i].BindLocalizedKeyNames.Length);
	}		

	for (i=0;i<255;i++)
	{
		KeyName = PlayerOwner().ConsoleCommand("KEYNAME"@i);
		LocalizedKeyName = PlayerOwner().ConsoleCommand("LOCALIZEDKEYNAME"@i);
		if (KeyName!="")
		{
			Alias = PlayerOwner().ConsoleCommand("KEYBINDING"@KeyName);
			if (Alias!="")
			{
				for (j=0;j<NumBindings;j++)
				{
					if (Bindings[j].Alias ~= Alias)
					{
						index = Bindings[j].Binds.Length;						
					
						Bindings[j].Binds[index] = i;
						Bindings[j].KeyLabel = Labels[Clamp(j,0,49)];
						Bindings[j].BindKeyNames[Index] = KeyName;
						Bindings[j].BindLocalizedKeyNames[Index] = LocalizedKeyName;

						for (k=0;k<index;k++)
						{
							if ( Weight(Bindings[j].Binds[k]) < Weight(Bindings[j].Binds[Index]) )
							{
								Swap(j,k,Index);
								break;
							}
						}	
					}
				}
			}
		}
	}
}

function string GetCurrentKeyBind(int index, int bind)
{

	if ( index>=NumBindings )
		return "";

	if (Bindings[Index].bIsSectionLabel)
		return "";

	if (Index==NewIndex && Bind==NewSubIndex)
		return "???";

	if (Bind>=Bindings[Index].Binds.Length)
		return "";
		
	return Bindings[Index].BindLocalizedKeyNames[Bind];
}

function DrawBinding(Canvas Canvas, int Item, float X, float Y, float W, float H, bool bSelected)
{
	local int x1,w1;
	local bool InBindArea;

	//local emenustate ems;

	if (Controller.MouseX >= controls[2].Bounds[0] && Controller.MouseX <=Controls[3].Bounds[2])
	{
		InBindArea=true;
	}

	if ( (Controller.HasMouseMoved()) && (!bSetNextKeyPress) )
	{
		if (InBindArea)
		{
	
			if ( ( Controller.MouseX >= Controls[2].Bounds[0] ) && ( Controller.MouseX <= Controls[2].Bounds[2] ) )
				Row = 0;

			else if ( (Controller.MouseX>=Controls[3].Bounds[0]) && (Controller.MouseX<=Controls[3].Bounds[2]) )
				Row = 1;

			InBindArea=true;
		}
			
		if ( InBindArea && (MyListBox.List.Index != HackIndex) && (Bindings[MyListBox.List.Index].bIsSectionLabel) )
			MyListBox.List.SetIndex(HackIndex);			
	}

	if(InBindArea)
	{
		if ( (bSetNextKeyPress) && (MyListBox.List.Index!=HackIndex) )
			MyListBox.List.SetIndex(HackIndex);


		if ( HackIndex !=MyListBox.List.Index)
			UpdateHint(MyListBox.List.Index);

		HackIndex = MyListBox.List.Index;
	}

	if ( Bindings[Item].bIsSectionLabel )
	{
		Canvas.CurX = Controls[1].ClientBounds[0]+3;
		Canvas.CurY = Y;
		Canvas.SetDrawColor(24,24,74,200);
		Canvas.DrawTile(Controller.DefaultPens[0],Controls[1].ClientBounds[2]-Controls[1].ClientBounds[0]-6,H,0,0,1,1);
		MyListBox.Style.DrawText(Canvas, MSAT_Pressed, Controls[1].ClientBounds[0]+SectionLabelMargin, Y, Controls[1].ClientBounds[2]-Controls[1].ClientBounds[0] ,H, TXTA_Left, Bindings[Item].KeyLabel);
	}
	else
		MyListBox.Style.DrawText(Canvas,MenuState, Controls[1].ActualLeft(), Y, Controls[1].ActualWidth(), H, TXTA_Center, Bindings[Item].KeyLabel);

		
	MyListBox.Style.DrawText(Canvas,MenuState, Controls[2].ActualLeft(), Y, Controls[2].ActualWidth(), H, TXTA_Center, GetCurrentKeyBind(Item,0));			
	MyListBox.Style.DrawText(Canvas,MenuState, Controls[3].ActualLeft(), Y, Controls[3].ActualWidth(), H, TXTA_Center, GetCurrentKeyBind(Item,1));
	
	
	if ( (bSelected) && (!Bindings[item].bIsSectionLabel) )
	{
		if (Row==0)
		{
			x1 = Controls[2].ActualLeft();
			w1 = Controls[2].ActualWidth();
		}
		else if (Row==1)
		{
			x1 = Controls[3].ActualLeft();
			w1 = Controls[3].ActualWidth();
		}
		
		if (!bSetNextKeyPress)
			SelStyle.Draw(Canvas,MSAT_Watched, x1, y, w1, h );
		else
			SelStyle.Draw(Canvas,MSAT_Pressed, x1, y, w1, h );
			
			
		UpdateHint(Item);		
	}

}

function bool ListOnKeyEvent(out byte Key, out byte State, float delta)
{
	local bool result;
	local int oldIndex;

	if (Key==0x0D && State==3)
	{
		GetNewKey(None);
		return true;
	}	
	
	if ( (Key==0x08) && (State==3) )	// Backsapce
	{
		// Clear Over

		if ( ( Controller.MouseX >= Controls[2].Bounds[0] ) && ( Controller.MouseX <= Controls[2].Bounds[2] ) )
			RemoveExistingKey(MyListBox.List.Index,0);

		else if ( (Controller.MouseX>=Controls[3].Bounds[0]) && (Controller.MouseX<=Controls[3].Bounds[2]) )
			RemoveExistingKey(MyListBox.List.Index,1);

		
		UpdateHint(MyListBox.List.Index);
		
		return true;
	}


	// -- WARNING.. really big hack here

	if ( (State==1) && (Key==0x25 || Key==0x64 || Key==0x27 || Key ==0x66) )
	{
		if (row==0)
			row = 1;
		else
			row = 0;
		
		UpdateHint(MyListBox.List.Index);
		return true;
	}	
		
	
	OldIndex = MyListBox.List.Index;
	result = MyListBox.List.InternalOnKeyEvent(Key,State,delta);
	if (MyListBox.List.Index!=OldIndex)
	{
	
		if (Bindings[MyListBox.List.Index].bIsSectionLabel)
		{
			if (MyListBox.List.Index<OldIndex)
				SearchUp(OldIndex);
			else
				SearchDown(OldIndex);
		}
	}
	UpdateHint(MyListBox.List.Index);

}

function SearchUp(int OldIndex)
{
	local int cindex;
	
	cindex = MyListBox.List.Index;
	while (cindex>0)
	{
		if (!Bindings[cindex].bIsSectionLabel)
		{
			MyListBox.List.SetIndex(cIndex);
			return;
		}
		cindex--;
	}
	MyListBox.List.SetIndex(OldIndex);
}

function SearchDown(int OldIndex)
{
	local int cindex;
	
	cindex = MyListBox.List.Index;
	while (cindex<MyListBox.List.ItemCount-1)
	{
		if (!Bindings[cindex].bIsSectionLabel)
		{
			MyListBox.List.SetIndex(cIndex);
			return;
		}
		cindex++;
	}
	MyListBox.List.SetIndex(OldIndex);
}


	
function RemoveExistingKey(int Index, int SubIndex)
{

	if ( (Index>=NumBindings) || (SubIndex>=Bindings[Index].Binds.Length) || (Bindings[Index].Binds[SubIndex] <0) )
		return;

	// Clear the bind

	PlayerOwner().ConsoleCommand("SET Input"@Bindings[Index].BindKeyNames[SubIndex]);

	// Clear the entry

	Bindings[Index].Binds.Remove(SubIndex,1);
	Bindings[Index].BindKeyNames.Remove(SubIndex,1);
	Bindings[Index].BindLocalizedKeyNames.Remove(SubIndex,1);	
}

function RemoveAllOccurance(byte NewKey)
{
	local int i,j;
	
	for (i=0;i<NumBindings;i++)
	{
		for (j=Bindings[i].Binds.Length-1;j>=0;j--)
		{
			if (Bindings[i].Binds[j]==NewKey)
			{
				RemoveExistingKey(i,j);
			}
		}
	}
	
}

function UpdateHint(int index)
{
	local int i;
	local string t;

	t = "";
	for (i=0;i<Bindings[Index].Binds.Length;i++)
	{
		if (t=="")
			t = ""$GetCurrentKeyBind(Index,i);
		else
			t = t$", "$GetCurrentKeyBind(Index,i);
	}
	MyListBox.List.Hint =Header$t$Footer;
	Controller.ActivePage.ChangeHint(MyListBox.List.Hint);		
}	

function AddNewKey(int Index, int SubIndex, byte NewKey)
{
	//local KeyBinding Where; 

	if (Index>=NumBindings) 
		return;

	if ( (SubIndex<Bindings[Index].Binds.Length) && (Bindings[Index].Binds[SubIndex] == NewKey) )
		return;
		
	RemoveAllOccurance(NewKey);

	if (SubIndex>=Bindings[Index].Binds.Length)
	{
		Bindings[Index].Binds.Length = Bindings[Index].Binds.Length + 1;
		Bindings[Index].BindKeyNames.Length = Bindings[Index].BindKeyNames.Length + 1;
		Bindings[Index].BindLocalizedKeyNames.Length = Bindings[Index].BindLocalizedKeyNames.Length + 1;
		SubIndex = Bindings[Index].Binds.Length-1;
	}
	Bindings[Index].Binds[SubIndex] = NewKey;
	Bindings[Index].BindKeyNames[SubIndex] = PlayerOwner().ConsoleCommand("KeyName"@NewKey);
	Bindings[Index].BindLocalizedKeyNames[SubIndex] = PlayerOwner().ConsoleCommand("LOCALIZEDKEYNAME"@NewKey);
	PlayerOwner().ConsoleCommand("SET Input"@Bindings[Index].BindKeyNames[SubIndex]@Bindings[Index].Alias);

	UpdateHint(Index);
	
}

function bool GetNewKey(GUIComponent Sender)
{
	local int SubIndex;

	if ( ( Controller.MouseX >= Controls[2].Bounds[0] ) && ( Controller.MouseX <= Controls[2].Bounds[2] ) )
		SubIndex = 0;
	else if ( (Controller.MouseX>=Controls[3].Bounds[0]) && (Controller.MouseX<=Controls[3].Bounds[2]) )
		SubIndex = 1;
	else
		return true;

	NewIndex = MyListBox.List.Index;
	NewSubIndex = SubIndex;
	bSetNextKeyPress=true;
	Controller.OnNeedRawKeyPress = RawKey;

	PlayerOwner().ClientPlaySound(Controller.EditSound);

	return true;
}

function bool RawKey(byte NewKey)
{

	if (NewKey!=0x1B)
		AddNewKey(NewIndex, NewSubIndex, NewKey);
		
	NewIndex = -1;
	NewSubIndex = -1;

	bSetNextKeyPress=false;
	Controller.OnNeedRawKeyPress = none;

	PlayerOwner().ClientPlaySound(Controller.ClickSound);

	return true;

}

function ListChange(GUIComponent Sender)
{
	UpdateHint(MyListBox.List.Index);
}

function MyOnAdjustTop(GUIComponent Sender)
{
	if( MyListBox.List.Index < MyListBox.List.Top )
		MyListBox.List.SetIndex( MyListBox.List.Top );
	else if( MyListBox.List.Index >= MyListBox.List.Top + MyListBox.List.ItemsPerPage )
		MyListBox.List.SetIndex( MyListBox.List.Top + MyListBox.List.ItemsPerPage );
}

function bool ResetClicked(GUIComponent Sender)
{
	Controller.ResetKeyboard();
	InitBindings();
	return true;
}	

defaultproperties
{

	Begin Object Class=GUIListBox Name=KeyConfigKeyList
		WinWidth=0.929493
		WinHeight=0.820040
		WinLeft=0.027188
		WinTop=0.094218
		bVisibleWhenEmpty=true
		StyleName="BindBox"
		Hint="Select Control to config"
	End Object
	Controls(0)=GUIListBox'KeyConfigKeyList'

	Begin Object class=GUIImage Name=KeyConfigBK1
		WinWidth=0.431248
		WinLeft=0.021641
		WinHeight=0.834063
		WinTop=0.083281
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
		bNeverFocus=true
		bAcceptsInput=true
	End Object
	Controls(1)=GUIImage'KeyConfigBK1'

	Begin Object class=GUIImage Name=KeyConfigBK2
		WinWidth=0.230937
		WinLeft=0.460078
		WinHeight=0.834063
		WinTop=0.083281
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(2)=GUIImage'KeyConfigBK2'

	Begin Object class=GUIImage Name=KeyConfigBK3
		WinWidth=0.232499
		WinLeft=0.688828
		WinHeight=0.834063
		WinTop=0.083281
		Image=Material'InterfaceContent.Menu.BorderBoxD'
		ImageColor=(R=255,G=255,B=255,A=160);
		ImageRenderStyle=MSTY_Alpha
		ImageStyle=ISTY_Stretched
	End Object
	Controls(3)=GUIImage'KeyConfigBK3'

	Begin Object class=GUILabel Name=KeyConfigAliasLabel
		Caption="Action"
		TextALign=TXTA_Center
		TextColor=(R=255,G=255,B=255,A=255)
		WinWidth=0.250000
		WinHeight=32.000000
		WinLeft=0.124414
		WinTop=0.004167
		TextFont="UT2HeaderFont"
	End Object
	Controls(4)=GUILabel'KeyConfigAliasLabel'

	Begin Object class=GUILabel Name=KeyConfigAliasLabel2
		Caption="Key 1"
		TextALign=TXTA_Center
		TextColor=(R=255,G=255,B=255,A=255)
		WinWidth=0.250000
		WinHeight=32.000000
		WinLeft=0.449414
		WinTop=0.004167
		TextFont="UT2HeaderFont"
	End Object
	Controls(5)=GUILabel'KeyConfigAliasLabel2'

	Begin Object class=GUILabel Name=KeyConfigAliasLabel3
		Caption="Key 2"
		TextALign=TXTA_Center
		TextColor=(R=255,G=255,B=255,A=255)
		WinWidth=0.250000
		WinHeight=32.000000
		WinLeft=0.686914
		WinTop=0.004167
		TextFont="UT2HeaderFont"
	End Object
	Controls(6)=GUILabel'KeyConfigAliasLabel3'

	Begin Object Class=GUIButton Name=KeyResetButton
		Caption="RESET"
		StyleName="SquareMenuButton"
		Hint="Reset keys to default values"
		WinWidth=0.231250
		WinHeight=0.068750
		WinLeft=0.687500
		WinTop=0.933333
		OnClick=ResetClicked
	End Object
	Controls(7)=GUIButton'KeyResetButton'


	WinTop=0.15
	WinLeft=0
	WinWidth=1
	WinHeight=0.77
	bAcceptsInput=false

	BKColor=(R=24,G=24,B=100,A=200)

	SectionLabelMargin=10

	Bindings(0)=(bIsSectionLabel=true,KeyLabel="Movement",Alias="")
	Bindings(1)=(bIsSectionLabel=false,KeyLabel="Forwards",Alias="MoveForward")
	Bindings(2)=(bIsSectionLabel=false,KeyLabel="Backwards",Alias="MoveBackward")
	Bindings(3)=(bIsSectionLabel=false,KeyLabel="Strafe Left",Alias="StrafeLeft")
	Bindings(4)=(bIsSectionLabel=false,KeyLabel="Strafe Right",Alias="StrafeRight")
	Bindings(5)=(bIsSectionLabel=false,KeyLabel="Jump",Alias="Jump")
	Bindings(6)=(bIsSectionLabel=false,KeyLabel="Walk",Alias="Walking")
	Bindings(7)=(bIsSectionLabel=false,KeyLabel="Crouch",Alias="Duck")
	Bindings(8)=(bIsSectionLabel=false,KeyLabel="Strafe Toggle",Alias="Strafe")
	Bindings(9)=(bIsSectionLabel=true,KeyLabel="Looking",Alias="")
	Bindings(10)=(bIsSectionLabel=false,KeyLabel="Turn Left",Alias="TurnLeft")
	Bindings(11)=(bIsSectionLabel=false,KeyLabel="Turn Right",Alias="TurnRight")
	Bindings(12)=(bIsSectionLabel=false,KeyLabel="Look Up",Alias="LookUp")
	Bindings(13)=(bIsSectionLabel=false,KeyLabel="Look Down",Alias="LookDown")
	Bindings(14)=(bIsSectionLabel=false,KeyLabel="Center View",Alias="CenterView")
	Bindings(15)=(bIsSectionLabel=true,KeyLabel="Weapons",Alias="")
	Bindings(16)=(bIsSectionLabel=false,KeyLabel="Fire",Alias="Fire")
	Bindings(17)=(bIsSectionLabel=false,KeyLabel="Alt-Fire",Alias="AltFire")
	Bindings(18)=(bIsSectionLabel=false,KeyLabel="Throw Weapon",Alias="ThrowWeapon")
	Bindings(19)=(bIsSectionLabel=false,KeyLabel="Best Weapon",Alias="SwitchToBestWeapon")
	Bindings(20)=(bIsSectionLabel=false,KeyLabel="Next Weapon",Alias="NextWeapon")
	Bindings(21)=(bIsSectionLabel=false,KeyLabel="Prev Weapon",Alias="PrevWeapon")
	Bindings(22)=(bIsSectionLabel=true,KeyLabel="Weapon Selection",Alias="")
	Bindings(23)=(bIsSectionLabel=false,KeyLabel="Super Weapon",Alias="SwitchWeapon 0")
	Bindings(24)=(bIsSectionLabel=false,KeyLabel="Shield Gun",Alias="SwitchWeapon 1")
	Bindings(25)=(bIsSectionLabel=false,KeyLabel="Assault Rifle",Alias="SwitchWeapon 2")
	Bindings(28)=(bIsSectionLabel=false,KeyLabel="Bio-Rifle",Alias="SwitchWeapon 3")
	Bindings(26)=(bIsSectionLabel=false,KeyLabel="Shock Rifle",Alias="SwitchWeapon 4")
	Bindings(27)=(bIsSectionLabel=false,KeyLabel="Link Gun",Alias="SwitchWeapon 5")
	Bindings(29)=(bIsSectionLabel=false,KeyLabel="Minigun",Alias="SwitchWeapon 6")
	Bindings(30)=(bIsSectionLabel=false,KeyLabel="Flak Cannon",Alias="SwitchWeapon 7")
	Bindings(31)=(bIsSectionLabel=false,KeyLabel="Rocket Launcher",Alias="SwitchWeapon 8")
	Bindings(32)=(bIsSectionLabel=false,KeyLabel="Lightning Rifle",Alias="SwitchWeapon 9")
	Bindings(33)=(bIsSectionLabel=false,KeyLabel="Translocator",Alias="SwitchWeapon 10")
	Bindings(34)=(bIsSectionLabel=true,KeyLabel="Communication",Alias="")
	Bindings(35)=(bIsSectionLabel=false,KeyLabel="Say",Alias="Talk")
	Bindings(36)=(bIsSectionLabel=false,KeyLabel="Team Say",Alias="TeamTalk")
	Bindings(37)=(bIsSectionLabel=false,KeyLabel="Speech Menu",Alias="SpeechMenuToggle")
	Bindings(38)=(bIsSectionLabel=true,KeyLabel="Taunts",Alias="")
	Bindings(39)=(bIsSectionLabel=false,KeyLabel="Pelvic Thrust",Alias="taunt pthrust")
	Bindings(40)=(bIsSectionLabel=false,KeyLabel="Ass Smack",Alias="taunt asssmack")
	Bindings(41)=(bIsSectionLabel=false,KeyLabel="Throat Cut",Alias="taunt throatcut")
	Bindings(42)=(bIsSectionLabel=true,KeyLabel="Hud",Alias="")
	Bindings(43)=(bIsSectionLabel=false,KeyLabel="Grow Hud",Alias="GrowHud")
	Bindings(44)=(bIsSectionLabel=false,KeyLabel="Shrink Hud",Alias="ShrinkHud")
	Bindings(45)=(bIsSectionLabel=true,KeyLabel="Miscellaneous",Alias="")
	Bindings(46)=(bIsSectionLabel=false,KeyLabel="Pause",Alias="Pause")
	Bindings(47)=(bIsSectionLabel=false,KeyLabel="Screenshot",Alias="shot")
 	Bindings(48)=(bIsSectionLabel=false,KeyLabel="Menu",Alias="ShowMenu")
	Bindings(49)=(bIsSectionLabel=false,KeyLabel="ScoreBoard",Alias="ShowScores")
	NumBindings=50
	Row=0
	Labels(0)="Movement"
	Labels(1)="Forwards"
	Labels(2)="Backwards"
	Labels(3)="Strafe Left"
	Labels(4)="Strafe Right"
	Labels(5)="Jump"
	Labels(6)="Walk"
	Labels(7)="Crouch"
	Labels(8)="Strafe Toggle"
	Labels(9)="Looking"
	Labels(10)="Turn Left"
	Labels(11)="Turn Right"
	Labels(12)="Look Up"
	Labels(13)="Look Down"
	Labels(14)="Center View"
	Labels(15)="Weapons"
	Labels(16)="Fire"
	Labels(17)="Alt-Fire"
	Labels(18)="Throw Weapon"
	Labels(19)="Best Weapon"
	Labels(20)="Next Weapon"
	Labels(21)="Prev Weapon"
	Labels(22)="Weapon Selection"
	Labels(23)="Super Weapon"
	Labels(24)="Shield Gun"
	Labels(25)="Assault Rifle"
	Labels(28)="Bio-Rifle"
	Labels(26)="Shock Rifle"
	Labels(27)="Link Gun"
	Labels(29)="Minigun"
	Labels(30)="Flak Cannon"
	Labels(31)="Rocket Launcher"
	Labels(32)="Lightning Rifle"
	Labels(33)="Translocator"
	Labels(34)="Communication"
	Labels(35)="Say"
	Labels(36)="Team Say"
	Labels(37)="Speech Menu"
	Labels(38)="Taunts"
	Labels(39)="Pelvic Thrust"
	Labels(40)="Ass Smack"
	Labels(41)="Throat Cut"
	Labels(42)="Hud"
	Labels(43)="Grow Hud"
	Labels(44)="Shrink Hud"
	Labels(45)="Miscellaneous"
	Labels(46)="Pause"
	Labels(47)="Screenshot"
 	Labels(48)="Menu"
	Labels(49)="ScoreBoard"
	Header="["
	Footer="] Perform this action ";
	
}