class UT2MidGameFont extends GUIFont;

var int FontScreenWidth[7];

static function Font GetMidGameFont(int xRes)
{
	local int i;
	for(i=0; i<7; i++)
	{
		if ( default.FontScreenWidth[i] <= XRes )
			return default.FontArray[i];
	}
	return default.FontArray[6];
}

// Same as 
event Font GetFont(int XRes)
{
	return GetMidGameFont(xRes);
}


defaultproperties
{
	KeyName="UT2MidGameFont"

    FontArray(0)=Font'FontEurostile17'
    FontArray(1)=Font'FontEurostile14'
    FontArray(2)=Font'FontEurostile11'
    FontArray(3)=Font'FontEurostile11'
    FontArray(4)=Font'FontEurostile9'
    FontArray(5)=Font'FontNeuzeit9'
    FontArray(6)=Font'FontSmallText'

    FontScreenWidth(0)=2048
    FontScreenWidth(1)=1600
    FontScreenWidth(2)=1280
    FontScreenWidth(3)=1024
    FontScreenWidth(4)=800
    FontScreenWidth(5)=640
    FontScreenWidth(6)=600
}
