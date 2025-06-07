// ====================================================================
//  Class:  XInterface.GUIListBox
//
//  The GUIListBoxBase is a wrapper for a GUIList and it's ScrollBar   
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================
class GUIListBox extends GUIListBoxBase
	native;

var	GUIList List;	// For Quick Access;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	List = GUIList(Controls[0]);
	Super.Initcomponent(MyController, MyOwner);
	
	List.OnClick=InternalOnClick;
	List.OnClickSound=CS_Click;
	List.OnChange=InternalOnChange;
	
}

function bool InternalOnClick(GUIComponent Sender)
{
	List.InternalOnClick(Sender);
	OnClick(Self);
	return true;
}

function InternalOnChange(GUIComponent Sender)
{
	OnChange(Self);
}

function int ItemCount()
{
	return List.ItemCount;
}

defaultproperties
{
	Begin Object Class=GUIList Name=TheList
		Name="TheList"
	End Object
	Controls(0)=GUIList'TheList'
}
