class GUIMultiColumnListBox extends GUIListBoxBase
	native;

cpptext
{
	void PreDraw(UCanvas* Canvas);		
}

var GUIMultiColumnList		 List;
var GUIMultiColumnListHeader Header;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	List	= GUIMultiColumnList(Controls[0]);
	Header	= GUIMultiColumnListHeader(Controls[2]);
	Header.MyList = List;
	Super.Initcomponent(MyController, MyOwner);
}

defaultproperties
{
	Begin Object Class=GUIMultiColumnList Name=MyMultiColumnList
	End Object
	Controls(0)=GUIMultiColumnList'MyMultiColumnList'
	Begin Object Class=GUIMultiColumnListHeader Name=MyHeader
	End Object
	Controls(2)=GUIMultiColumnListHeader'MyHeader'
}