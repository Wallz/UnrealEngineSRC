// ====================================================================
//  Class:  XWebAdmin.xWebQueryHandler
//  Parent: Engine.xAdminBase
//
//  <Enter a description here>
// ====================================================================

class xWebQueryHandler extends xAdminBase
		Within UTServerAdmin;

var string DefaultPage;
var string Title;
var string NeededPrivs;

function bool Init() {return true;}
function bool Query(WebRequest Request, WebResponse Response) { return false; }

defaultproperties
{
}
