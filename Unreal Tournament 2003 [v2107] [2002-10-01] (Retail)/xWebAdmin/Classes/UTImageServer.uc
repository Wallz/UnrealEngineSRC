class UTImageServer extends ImageServer;

// Changes:
// - Check Login with new xAdminManager

event Query(WebRequest Request, WebResponse Response)
{
local string AdminRealm;
//local xAdminUser User;

	AdminRealm    = class'UTServerAdmin'.default.AdminRealm;

	// Check authentication:
/*	User = Level.Game.AccessControl.AdminManager.WebLogin(Request.UserName, Request.Password);
	if ( User == None)
	{
		Response.FailAuthentication(AdminRealm);
		return;
	}
	*/
	Super.Query(Request, Response);
//	Level.Game.AccessControl.AdminManager.WebLogout(User);
}

defaultproperties
{
}
