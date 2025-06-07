//=============================================================================
// MapList.
//
// contains a list of maps to cycle through
//
//=============================================================================
class MapList extends Info
	abstract;

var(Maps) config array<string> Maps;
var config int MapNum;

// When Spawned, removed any list entry that are empty
event Spawned()
{
local int i;
local bool bChanged;

	bChanged = false;

	for (i=0; i<Maps.Length; i++)
	{
		if (Maps[i] == "")
		{
			Maps.Remove(i, 1);
			bChanged = true;
			i--;
		}
	}

	if (bChanged)
	{
		MapNum=0;
		SaveConfig();
		Log("MapList had invalid entries!");
	}
}

function GetAllMaps(out string s) // sjs
{
    local int i;

    s = "";
    for( i=0; i<Maps.Length; i++ )
    {
        if( Maps[i] ~= "Entry" )
            continue;
        if( Maps[i] == "" )
            continue;
        s = s $ "," $ Maps[i];
    }
}

function string GetNextMap()
{
	local string CurrentMap;
	local int i;

	CurrentMap = GetURLMap();
	if ( CurrentMap != "" )
	{
		if ( !(Right(CurrentMap,4) ~= ".ut2") )
			CurrentMap = CurrentMap$".ut2";

		for ( i=0; i<Maps.Length; i++ )
		{
			if ( CurrentMap ~= Maps[i] )
			{
				MapNum = i;
				break;
			}
		}
	}

	// search vs. w/ or w/out .unr extension

	MapNum++;
	if ( MapNum >= Maps.Length )
		MapNum = 0;

	SaveConfig();
	return Maps[MapNum];
}