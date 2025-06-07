class Sound extends Object
    native
	hidecategories(Object)
    noexport;

var native const byte Data[28]; // sizeof (FSoundData) :(
var native const Name FileType;
var native const String FileName;
var native const int OriginalSize;
var native const float Duration;
var native const int Handle;
var native const int Flags;

var(Sound) native float Likelihood;
var(Sound) float BaseRadius;
var(Sound) float VelocityScale;

defaultproperties
{
    Duration=-1.0
    Likelihood=1.0
    BaseRadius=2000.0
    VelocityScale=0.0
}