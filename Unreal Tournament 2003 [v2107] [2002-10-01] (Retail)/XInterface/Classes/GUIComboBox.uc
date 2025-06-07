// ====================================================================
//	Class: XInterface.GUIComboBox 
//
//  A Combination of an EditBox, a Down Arrow Button and a ListBox
//
//  Written by Michel Comeau
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIComboBox extends GUIMultiComponent
	Native;

cpptext
{
		void PreDraw(UCanvas* Canvas);
		
}	
	
var(Menu)	int			MaxVisibleItems;
var(Menu)	bool		bListItemsOnly;
var(Menu)	bool		bShowListOnFocus;
var(Menu)	bool		bReadOnly;

var		int		Index;
var		string	TextStr;

// For Quick Access;

var		GUIList				List;	
var		GUIEditBox			Edit;	
var 	GUIListBox			ListBox;
var		GUIComboButton		DownButton;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{

	Edit		= GUIEditBox(Controls[0]);
	DownButton	= GUIComboButton(Controls[1]);
	ListBox		= GUIListBox(Controls[2]);
	
	Super.Initcomponent(MyController, MyOwner);

	List 			= ListBox.List;
	List.OnChange 	= ItemChanged;
	List.bHotTrack	= true;
	List.OnClickSound = CS_Click;
	List.OnCLick 	= InternalListClick;
	
	Edit.OnChange 			= TextChanged;
	Edit.OnMousePressed 	= InternalEditPressed;
	Edit.INIOption  		= INIOption;
	Edit.INIDefault 		= INIDefault;
	Edit.Hint				= Hint;
	Edit.bReadOnly  		= bReadOnly;
	

	List.OnDeActivate = InternalListDeActivate;
	
	DownButton.OnClick 		= ShowListBox;
	DownButton.FocusInstead = List;
	
	OnInvalidate = InternalOnInvalidate;
	
}

function InternalListDeActivate()
{
	if (!Edit.bPendingFocus)
		ListBox.bVisible = false;
}

function InternalOnInvalidate()
{
	if (Edit.MenuState!=MSAT_Focused)
		ListBox.bVisible=false;
		
	if (ListBox.MenuState==MSAT_Focused)
	{
		Edit.SetFocus(None);
	}
}

function InternalEditPressed(GUIComponent Sender, bool bRepeat)
{
	if ( (Edit.bReadOnly) && (!bRepeat) )
	{
		Controller.bIgnoreNextRelease = true;
	
		if ( !ListBox.bVisible )
		{
			ShowListBox(Self);
		}
		else
			HideListBox();
	}
		
	return;
}

function bool InternalListClick(GUIComponent Sender)
{
	List.InternalOnClick(Sender);
	Edit.SetFocus(none);
	return true;

}	

function HideListBox()
{
	ListBox.bVisible = false;
}

function bool ShowListBox(GUIComponent Sender)
{
	ListBox.bVisible = !ListBox.bVisible;

	if (ListBox.bVisible)
		List.SetFocus(none);
		
	return true;
}

function ItemChanged(GUIComponent Sender)
{
	Edit.SetText(List.SelectedText());
	Index = List.Index;
}

function TextChanged(GUIComponent Sender)
{
	TextStr = Edit.TextStr;
	OnChange(self);
}

function SetText(string NewText)
{
	Edit.SetText(NewText);
	List.Find(NewText);
}

function string Get()
{
	local string temp;

	temp = List.Get();

	if ( temp~=Edit.GetText() )
		return Edit.GetText();
	else
		return temp;
}

function string GetText() {
	return self.Get();
}

function object GetObject()
{
	local string temp;
	
	temp = List.Get();
	
	if ( temp~=Edit.GetText() )
		return List.GetObject();
	else
		return none;
}

function string GetExtra()
{
	local string temp;
	
	temp = List.Get();
	
	if ( temp~=Edit.GetText() )
		return List.GetExtra();
	else
		return "";
}

function SetIndex(int I)
{
	List.SetIndex(i);
}

function int GetIndex()
{
	return List.Index;
}

function AddItem(string Item, Optional object Extra, Optional string Str)
{
	List.Add(Item,Extra,Str);
}

function RemoveItem(int item, optional int Count)
{
	List.Remove(item, Count);
}

function string GetItem(int index)
{
	List.SetIndex(Index);
	return List.Get();
}

function object GetItemObject(int index)
{
	List.SetIndex(Index);
	return List.GetObject();
}

function string find(string Text, optional bool bExact)
{
	return List.Find(Text,bExact);
}

function int ItemCount()
{
	return List.ItemCount;
}

function ReadOnly(bool b)
{
	Edit.bReadOnly = b;
}

defaultproperties
{
	Begin Object Class=GUIEditBox Name=EditBox1
		TextStr=""
	End Object
	Controls(0)=GUIEditBox'EditBox1'
	Begin Object Class=GUIComboButton Name=ShowList
 		bRepeatClick=false
	End Object
	Controls(1)=GUIVertScrollButton'ShowList'
	Begin Object Class=GUIListBox Name=ListBox1
		StyleName="ComboListBox"
		bVisible=false
	End Object
	Controls(2)=GUIListBox'ListBox1'
	MaxVisibleItems=8
//	bAcceptsInput=true
	WinHeight=0.06
	
}
