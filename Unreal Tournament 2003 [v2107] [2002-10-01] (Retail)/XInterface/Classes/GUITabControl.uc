// ====================================================================
//	GUITabControl - This control has a number of tabs 
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUITabControl extends GUIMultiComponent
	Native;

cpptext
{
		void PreDraw(UCanvas* Canvas);
		void Draw(UCanvas* Canvas);									

		UGUIComponent* UnderCursor(FLOAT MouseX, FLOAT MouseY);
		UBOOL SpecialHit();
		
}	

var(Menu)	bool				bDockPanels;		// If true, associated panels will dock vertically with this control

var			array<GUITabButton> TabStack;	
var			GUITabButton		ActiveTab;

var			string				BackgroundStyleName;
var			GUIStyles			BackgroundStyle;
var			Material			BackgroundImage;
var			float 				TabHeight;
var			bool				bFillSpace;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	
	if (BackgroundStyleName != "")
		BackgroundStyle = Controller.GetStyle(BackgroundStyleName);

	OnKeyEvent = InternalOnKeyEvent;		
		
}

function bool InternalOnKeyEvent(out byte Key, out byte State, float delta)
{
	local int i,aTabIndex, StartIndex;
	
	if ( (FocusedControl!=None) || (State!=3) || (TabStack.Length<=0) )
		return false;

	if (ActiveTab==None)
	{
		for (i = 0; i < TabStack.Length; i++)
			if (ActivateTab(TabStack[i],false))
				break;

		return true;
	}
	else
	{		
		for(i=0;i<TabStack.Length;i++)
			if (TabStack[i]==ActiveTab)
			{
				aTabIndex = i;
				break;
			}
	}

	if ( (Key==0x25) || (Key==0x64) )
	{
		StartIndex = aTabIndex;
		while (true)
		{
			if (aTabIndex==0)
				aTabIndex=TabStack.Length-1;
			else
				aTabIndex--;

			if (aTabIndex == StartIndex || ActivateTab(TabStack[aTabIndex],false))
				break;
		}
		return true;
			
	}
	
	if ( (Key==0x27) || (Key==0x66) )
	{
		StartIndex = aTabIndex;
		while (true)
		{
			aTabIndex++;
			if (aTabIndex==TabStack.Length)
				aTabIndex=0;

			if (StartIndex == aTabIndex || ActivateTab(TabStack[aTabIndex],false))
				break;
		}
		return true;
	}
		
	return false;
}

function GUITabPanel AddTab(string Caption, string PanelClass, optional GUITabPanel ExistingPanel, optional string Hint, optional bool bForceActive)
{
	local class<GUITabPanel> NewPanelClass;
	
	local GUITabButton NewTabButton;
	local GUITabPanel  NewTabPanel;
	
	local int i;
	
	// Make sure this doesn't exist first
	
	for (i=0;i<TabStack.Length;i++)
	{
		if (TabStack[i].Caption ~= Caption)
		{
			log("A Tab with the caption"@Caption@"already exists.");
			return none;
		}
	} 

	if (ExistingPanel==None)	 
		NewPanelClass = class<GUITabPanel>(DynamicLoadObject(PanelClass,class'class'));
		
	if ( (ExistingPanel!=None) || (NewPanelClass != None) )
	{
		// instance the button
				
		NewTabButton = new class'GUITabButton';
		if (NewTabButton==None)
		{
			log("Could not create Tab for"@NEwPanelClass);
			return None;
		}
		else
			NewTabButton.InitComponent(Controller, Self);
		
		NewTabButton.Caption = Caption;
		NewTabButton.Hint = Hint;	
	
		// Attempt to spawn the menu

		if (ExistingPanel==None)
			NewTabPanel = new NewPanelClass;
		else
			NewTabPanel = ExistingPanel;
			
		if (NewTabPanel==None)
		{
			log("Could not create panel"@NewPanelClass);
			return None;
		}
		else
			NewTabPanel.InitComponent(Controller, Self);
		
		// Add the tab to controls
		
		Controls.Length = Controls.Length + 1;
		Controls[Controls.Length-1] = NewTabPanel;
		
		TabStack.Length = TabStack.Length + 1;
		TabStack[TabStack.Length-1] = NewTabButton;
		
		NewTabButton.OnClick = InternalTabClick;
		NewTabButton.MyPanel = NewTabPanel;
		NewTabButton.FocusInstead = self;
		NewTabButton.bNeverFocus = true;

		NewTabPanel.MyButton = NewTabButton;
		NewTabPanel.InitPanel();
		NewTabPanel.bVisible = false;
		
		if ( (TabStack.Length==1) || (bForceActive) )
		{
			ActivateTab(NewTabButton,true);
		}
		
		Return NewTabPanel;
		
	}
	else
		return none;
}

function GUITabPanel ReplaceTab(GUITabButton Which, string Caption, string PanelClass, optional GUITabPanel ExistingPanel, optional string Hint, optional bool bForceActive)
{
	local class<GUITabPanel> NewPanelClass;
	
	local GUITabPanel  NewTabPanel, OldTabPanel;
	local int i;
	
	if (ExistingPanel==None)	 
		NewPanelClass = class<GUITabPanel>(DynamicLoadObject(PanelClass,class'class'));
		
	if ( (ExistingPanel!=None) || (NewPanelClass != None) )
	{

		OldTabPanel = Which.MyPanel;
	
		if (ExistingPanel==None)
			NewTabPanel = new NewPanelClass;
		else
			NewTabPanel = ExistingPanel;

		if (NewTabPanel==None)
		{
			log("Could not create panel"@NewPanelClass);
			return none;
		}
		else
			NewTabPanel.InitComponent(Controller, Self);

		Which.Caption = Caption;
		Which.Hint = Hint;	
		Which.MyPanel = NewTabPanel;
		
		for (i=0;i<Controls.Length;i++)
			if (Controls[i]==OldTabPanel)
			{
				Controls[i]=NewTabPanel;
				NewTabPanel.MyButton = Which;
				NewTabPanel.InitPanel();
				NewTabPanel.bVisible = false;
				break;
			}
			
		if ( bForceActive ) 
			Which.ChangeActiveState(true, true);

		return NewTabPanel;			
			
	}
	else
		return none;
}

function RemoveTab(optional string Caption, optional GUITabButton who)
{
	local int i,index;
	local GUITabPanel P;
	
	if ( (caption=="") && (Who==None) )
		return;
		
	if (Who==None)
	{
		for (i=0;i<TabStack.Length;i++)
			if (TabStack[i].Caption ~= Caption)
			{
				Who=TabStack[i];
				Index=i;
				break;
			}
	}
		
	if (Who==None)
		return;

	TabStack.Remove(index,1);		
		
	for (i=0;i<Controls.Length;i++)
		if (GUITabPanel(Controls[i]) == P)
		{
			Controls.Remove(i,1);
			return;
		}
		
}		

function bool ActivateTab(GUITabButton Who, bool bFocusPanel)
{
	if (Who == none || !Who.CanShowPanel())		// null or not selectable
		return false;

	if (Who==ActiveTab)	// Same Tab, just accept
		return true;

	// Deactivate the Active tab		
		
	if (ActiveTab!=None)		
		ActiveTab.ChangeActiveState(false,bFocusPanel);

	// Set the new active Tab
		
	ActiveTab = Who;
	Who.ChangeActiveState(true,bFocusPanel);
	OnChange(Who);
	return true;
}

function bool ActivateTabByName(string tabname, bool bFocusPanel)
{
	local int i;
	local GUITabButton Who;

	Who = none;
	for ( i = 0; i < TabStack.length; i++ ) {
		if ( TabStack[i].Caption ~= tabname ) {
			Who = TabStack[i];
			break;
		}
	}

	return ActivateTab(Who, bFocusPanel);
}

function bool InternalTabClick(GUIComponent Sender)
{
	local GUITabButton But;
	
	But = GUITabButton(Sender);
	if (But==None)
		return false;
		
	ActivateTab(But,true);
	return true;
} 

event bool NextPage()
{
	local int i;

	// If 1 or no tabs in the stack, then query parents
	if (TabStack.Length < 2)
		return Super.NextPage();

	if (ActiveTab == None)
		i = 0;
	else
	{
		for (i = 0; i<TabStack.Length; i++)
			if (TabStack[i] == ActiveTab)
				break;
		i++;
		if ( i >= TabStack.Length )
			i = 0;
	}
	ActivateTab(TabStack[i], true);
	return true;
}

event bool PrevPage()
{
	local int i;

	if (TabStack.Length < 2)
		return Super.NextPage();

	if (ActiveTab == None)
		i = TabStack.Length - 1;
	else
	{
		for (i = 0; i<TabStack.Length; i++)
			if (TabStack[i] == ActiveTab)
				break;
		i--;
		if ( i < 0 )
			i = TabStack.Length - 1;
	}
	ActivateTab(TabStack[i], true);
	return true;
}


defaultproperties
{
	TabHeight=48
	bFillSpace=false
	bTabStop=true
	bDockPanels=false
}
