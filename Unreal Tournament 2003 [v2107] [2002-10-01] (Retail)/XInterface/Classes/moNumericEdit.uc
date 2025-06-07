// ====================================================================
//  Class:  XInterface.moNumericEdit
//  Parent: XInterface.GUIMenuOption
//
//  <Enter a description here>
// ====================================================================

class moNumericEdit extends GUIMenuOption;

var		GUINumericEdit	MyNumericEdit;
var		int				MinValue, MaxValue;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);
	MyNumericEdit = GUINumericEdit(MyComponent);
	MyNumericEdit.MinValue = MinValue;
	MyNumericEdit.MaxValue = MaxValue;
	MyNumericEdit.CalcMaxLen();

	FriendlyLabel=none;	
	MyNumericEdit.Controls[0].FriendlyLabel = MyLabel;
	
	
}

function SetValue(int V)
{
	MyNumericEdit.SetValue(v);
}

function int GetValue()
{
	return int(MyNumericEdit.Value);
}

function InternalOnChange(GUIComponent Sender)
{
	MyNumericEdit.EditOnChange(Sender);
	OnChange(self);
}


defaultproperties
{
	ComponentClassName="xinterface.GUINumericEdit"
	OnClickSound=CS_Click
}


