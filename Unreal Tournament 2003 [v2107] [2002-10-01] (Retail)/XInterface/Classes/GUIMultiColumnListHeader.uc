class GUIMultiColumnListHeader extends GUIComponent
	native;

cpptext
{
	UBOOL MousePressed(UBOOL IsRepeat);
	UBOOL MouseReleased();
	UBOOL MouseMove(INT XDelta, INT YDelta);
	UBOOL MouseHover();
	void Draw(UCanvas* Canvas);
	void PreDraw(UCanvas* Canvas);
}

var GUIMultiColumnList MyList;
var int SizingCol;
var int ClickingCol;

defaultproperties
{
	StyleName="ServerBrowserGridHeader"
	bCaptureTabs=False
	bNeverFocus=False
	bTabStop=False
	bAcceptsInput=True
	SizingCol=-1
	ClickingCol=-1
}