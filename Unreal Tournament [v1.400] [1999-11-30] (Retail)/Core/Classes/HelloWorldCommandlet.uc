//=============================================================================
/// UnrealScript "hello world" sample Commandlet.
///
/// Usage:
///     ucc.exe HelloWorld
//=============================================================================
class HelloWorldCommandlet
	expands Commandlet;

var int intparm;
var string strparm;

function int Main( string Parms )
{
	log( "Hello, world!" );
	if( Parms!="" )
		log( "Command line parameters=" $ Parms );
	if( intparm!=0 )
		log( "You specified intparm=" $ intparm );
	if( strparm!="" )
		log( "You specified strparm=" $ strparm );
}

defaultproperties
{
	HelpCmd=HelloWorld
	HelpOneLiner=Sample "hello world" commandlet
	HelpUsage=HelloWorld (no parameters)
	HelpWebLink=
	LogToStdout=true
	HelpParm(0)="IntParm"
	HelpDesc(0)="An integer parameter"
	HelpParm(1)="StrParm"
	HelpDesc(1)="A string parameter"
}
