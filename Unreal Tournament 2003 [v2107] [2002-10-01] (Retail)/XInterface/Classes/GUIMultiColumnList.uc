class GUIMultiColumnList extends GUIVertList
	native;

struct native MultiColumnSortData
{
	var string SortString;
	var const int SortItem;
};

var array<MultiColumnSortData> SortData;
var array<int> InvSortData;
var localized string ColumnHeadings[8];  // counting on number of ColumnWidths to be <= # ColumnHeadings
var array<float> ColumnWidths;
var array<float> InitColumnPerc; // Used to initialize column width to screen-relative size (if desired)
var float CellSpacing;
var int SortColumn; // If -1 - dont sort
var bool SortDescending;
var bool ExpandLastColumn;
var bool bInitialised;


// sorting stuff
var bool NeedsSorting;
native final function SortList();
native final function ChangeSortOrder();
native final function AddedItem();			// must be called when we add an item to the end of the list.
native final function UpdatedItem(int i);	// should be called when we update an item's data
event string GetSortString( int i );


// notification
event OnColumnSized(int column);

event OnSortChanged()
{
	local int OldItem;

	// get the selection item
	if( Index >= 0 )
		OldItem = SortData[Index].SortItem;

	// recalculate SortData
	ChangeSortOrder();
	// resort list
	SortList();
	NeedsSorting = False;

	// remap the selection item back again to keep the same item selected
	if( Index >= 0 && OldItem < InvSortData.Length )
		Index = InvSortData[OldItem];
}

function Clear()
{
	SortData.Remove(0,SortData.Length);
	InvSortData.Remove(0,SortData.Length);
	Super.Clear();
}

function RemovedCurrent()
{
	if( Index >= 0 )
	{	
		SortData.Remove(Index,1);
		InvSortData.Remove(Index,1);

		// Force updating of sort data
		OnSortChanged();
		Index = -1;
	}
}

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	OnPreDraw = InternalOnPreDraw;
}

function bool InternalOnPreDraw(Canvas C)
{
	local float x;
	local int i;
	local int OldItem;

	if(!bInitialised)
	{
		for(i=0; i<InitColumnPerc.Length; i++)
		{
			ColumnWidths[i] = C.ClipX * InitColumnPerc[i];
		}
		bInitialised=true;
	}

	if( NeedsSorting )
	{
		// get the selection item
		if( Index >= 0 )
			OldItem = SortData[Index].SortItem;

		SortList();	
		NeedsSorting = False;

		// remap the selection item back again to keep the same item selected
		if( Index >= 0 && OldItem < InvSortData.Length )
			Index = InvSortData[OldItem];
	}
	if( ExpandLastColumn )
	{
		for( i=0;i<ColumnWidths.Length-1;i++ )
			x += ColumnWidths[i];
        ColumnWidths[i] = ActualWidth() - x;
	}
	return true;
}

function GetCellLeftWidth( int Column, out float Left, out float Width )
{
	local int i;
	Left = 0;
	for( i=0;i<Column && i<ColumnWidths.Length;i++ )
		Left += ColumnWidths[i];
	if( i<ColumnWidths.Length )
		Width = ColumnWidths[i];
	else
		Width = 0;

	Left += CellSpacing;
	Width -= 2*CellSpacing;
}

defaultproperties
{
	CellSpacing=1.0
	SortColumn=-1
}