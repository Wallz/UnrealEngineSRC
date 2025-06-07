// ====================================================================
//  Class:  XInterface.GUICharacterList
//  Parent: XInterface.GUIHorzList
//
//  <Enter a description here>
// ====================================================================

class GUICharacterList extends GUICircularList
		Native;

cpptext
{
	void PreDraw(UCanvas* Canvas);	
	void DrawItem(UCanvas* Canvas, INT Item, FLOAT X, FLOAT Y, FLOAT W, FLOAT H);
}

var array<xUtil.PlayerRecord> PlayerList;
var	bool					  bLocked;		
var Material				  DefaultPortrait;	// Image used for unused entries

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.Initcomponent(MyController, MyOwner);
	InitList();
}		

function InitList()
{
	local int i;
	local array<xUtil.PlayerRecord> AllPlayerList;

	class'xUtil'.static.GetPlayerList(AllPlayerList);

	// Filter out 'duplicate' characters - only used in single player
	for(i=0; i<AllPlayerList.Length; i++)
	{
		if(AllPlayerList[i].Menu != "DUP")
			PlayerList[PlayerList.Length] = AllPlayerList[i];
	}

	ItemCount = PlayerList.Length;
}

// Accessor function for the items.

function string SelectedText()
{
	if ( (Index >=0) && (Index <ItemCount) )
		return PlayerList[Index].DefaultName;
	else
		return "";
}

function Add(string NewItem, optional Object obj)
{
	return;	// GUICharacterLists can not be modifed at runtime
}

function Remove(int Index, optional int Count)
{
	return;	// GUICharacterLists can not be modifed at runtime
} 

function Clear()
{
	return;	// GUICharacterLists can not be modifed at runtime
}	

function find(string Text, optional bool bExact)
{
	local int i;
	for (i=0;i<ItemCount;i++)
	{
		if (bExact)
		{
			if (Text == PlayerList[i].DefaultName)
			{
				Index = i;
				Top = i;
				OnChange(self);
				return;
			}
		}
		else
		{
			if (Text ~= PlayerList[i].DefaultName)
			{
				Index = i;
				Top = i;
				OnChange(self);
				return;
			}
		}
	}
}

function material GetPortrait()
{
	return PlayerList[Index].Portrait;
}

function string GetName()
{
	return PlayerList[Index].DefaultName;
}

function xUtil.PlayerRecord GetRecord()
{
	return PlayerList[Index];
}

function string GetDecoText()
{	
	local string s;
	local int i;
	
	s = PlayerList[Index].TextName;
	i = InStr(S,".");
	s = Right(S,len(S)-i-1);

	return s;
}

function sound GetSound()
{
	local sound NameSound;
	local string SoundName, DefName;
	local int PeriodPos;

	// Use the player name, with periods replaced with underscores, as sound name
	DefName = PlayerList[Index].DefaultName;

	PeriodPos = InStr(DefName, ".");
	while(PeriodPos != -1)
	{
		DefName = Left(DefName, PeriodPos)$"_"$Mid(DefName, PeriodPos+1);
		PeriodPos = InStr(DefName, ".");
	}

	SoundName = "AnnouncerNames."$DefName;

	NameSound = sound(DynamicLoadObject(SoundName, class'Sound'));
	
	if(NameSound == None)
		Log("Could not find player name sound for: "$PlayerList[Index].DefaultName);

	return NameSound;
}


function ScrollLeft()
{
	if (bLocked)
	{
		if (Index>0)
		{
			Index--;
			OnChange(Self);
		}
	}
	else
		Super.ScrollLeft();
	

}

function ScrollRight()
{
	if (bLocked)
	{
		if (Index<ItemsPerPage-1)
		{
			Index++;
			OnChange(Self);
		}
	}
	else
		Super.ScrollRight();
}
	
function End()
{
	if (bLocked)
	{
		Index = ItemsPerPage-1;
		OnChange(Self);
	}
	else
		Super.End();
}	



defaultproperties
{
}
