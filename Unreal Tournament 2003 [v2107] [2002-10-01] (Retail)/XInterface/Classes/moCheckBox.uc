// ====================================================================
//  Class:  XInterface.moCheckBox
//  Parent: XInterface.GUIMenuOption
//
//  <Enter a description here>
// ====================================================================

class moCheckBox extends GUIMenuOption;

var		GUICheckBoxButton 	MyCheckBox;
var		bool				bChecked;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);

	MyCheckBox = GUICheckBoxButton(MyComponent);
}	

function bool IsChecked()
{
	return MyCheckBox.bChecked;
}

function bool Checked(bool C)
{
	MyCheckBox.bChecked = c;
	bChecked = c;
	OnChange(Self);
	return true;
}


defaultproperties
{
	ComponentClassName="xinterface.GUICheckBoxButton"
	bChecked=false;
}
