//=============================================================================
// Info, the root of all information holding classes.
//=============================================================================
class Info extends Actor
	abstract
	hidecategories(Movement,Collision,Lighting,LightColor,Karma,Force)
	native;

// mc: Fill a PlayInfoData structure to allow easy access to 
static function FillPlayInfo(PlayInfo PlayInfo)
{
	PlayInfo.AddClass(default.Class);
}

static event bool AcceptPlayInfoProperty(string PropertyName)
{
	return true;
}

defaultproperties
{
	RemoteRole=ROLE_None
	NetUpdateFrequency=10
     bHidden=True
	 bOnlyDirtyReplication=true
	 bSkipActorPropertyReplication=true
}
