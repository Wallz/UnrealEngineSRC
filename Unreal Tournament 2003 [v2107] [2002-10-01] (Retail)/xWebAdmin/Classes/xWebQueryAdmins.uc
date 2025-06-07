// ====================================================================
//  Class:  XAdmin.xWebQueryAdmins
//  Parent: XWebAdmin.xWebQueryHandler
//
//  <Enter a description here>
// ====================================================================

class xWebQueryAdmins extends xWebQueryHandler;

struct RowGroup { var array<string>	rows; };

var config string AdminsIndexPage;
var config string UsersHomePage;
var config string UsersAccountPage;
var config string UsersAddPage;
var config string UsersBrowsePage;
var config string UsersEditPage;
var config string UsersGroupsPage;
var config string UsersMGroupsPage;
var config string GroupsAddPage;
var config string GroupsBrowsePage;
var config string GroupsEditPage;

var localized string NoteUserHomePage;
var localized string NoteAccountPage;
var localized string NoteUserAddPage;
var localized string NoteUserEditPage;
var localized string NoteUsersBrowsePage;
var localized string NoteGroupAddPage;
var localized string NoteGroupEditPage;
var localized string NoteGroupsBrowsePage;
var localized string NoteGroupAccessPage;
var localized string NoteMGroupAccessPage;

function bool Query(WebRequest Request, WebResponse Response)
{
	switch (Mid(Request.URI, 1))
	{
	case DefaultPage:		QueryAdminsFrame(Request, Response); return true;
	case AdminsIndexPage:	QueryAdminsMenu(Request, Response); return true;
		
	case UsersHomePage:		if (!MapIsChanging()) QueryUsersHomePage(Request, Response); return true;
	case UsersAccountPage:	if (!MapIsChanging()) QueryUserAccountPage(Request, Response); return true;
	case UsersBrowsePage:	if (!MapIsChanging()) QueryUsersBrowsePage(Request, Response); return true;
	case UsersAddPage:		if (!MapIsChanging()) QueryUsersAddPage(Request, Response); return true;
	case UsersEditPage:		if (!MapIsChanging()) QueryUsersEditPage(Request, Response); return true;
	case UsersGroupsPage:	if (!MapIsChanging()) QueryUsersGroupsPage(Request, Response); return true;
	case UsersMGroupsPage:	if (!MapIsChanging()) QueryUsersMGroupsPage(Request, Response); return true;
	case GroupsBrowsePage:	if (!MapIsChanging()) QueryGroupsBrowsePage(Request, Response); return true;
	case GroupsAddPage:		if (!MapIsChanging()) QueryGroupsAddPage(Request, Response); return true;
	case GroupsEditPage:	if (!MapIsChanging()) QueryGroupsEditPage(Request, Response); return true;
	}	
	return false;
}

function QueryAdminsFrame(WebRequest Request, WebResponse Response)
{
local String Page;
	
	// if no page specified, use the default
	Page = Request.GetVariable("Page", UsersHomePage);

	Response.Subst("IndexURI", 	AdminsIndexPage$"?Page="$Page);
	Response.Subst("MainURI", 	Page);

	ShowPage(Response, DefaultPage);
}

function QueryAdminsMenu(WebRequest Request, WebResponse Response)
{
	Response.Subst("Title", "Users & Groups Management");
	
	Response.Subst("UsersHomeURI", UsersHomePage);
	Response.Subst("UserAccountURI", UsersAccountPage);
	Response.Subst("UsersAddURI", UsersAddPage);
	Response.Subst("GroupsAddURI", GroupsAddPage);
	Response.Subst("UsersBrowseURI", UsersBrowsePage);
	Response.Subst("GroupsBrowseURI", GroupsBrowsePage);
	
	ShowPage(Response, AdminsIndexPage);
}

function QueryUsersHomePage(WebRequest Request, WebResponse Response)
{
	Response.Subst("Section", "Admin Home Page");
	Response.Subst("PageHelp", NoteUserHomePage);
	ShowPage(Response, UsersHomePage);
}

function QueryUserAccountPage(WebRequest Request, WebResponse Response)
{
local string upass;

	Response.Subst("NameValue", HtmlEncode(CurAdmin.UserName));
	if (Request.GetVariable("edit", "") != "")
	{
		// Can only change his password
		upass = Request.GetVariable("Password", CurAdmin.Password);
		if (!CurAdmin.ValidPass(upass))
			StatusError(Response, "New password is too short!");
		else if (upass != CurAdmin.Password)
		{
			CurAdmin.Password = upass;
			Level.Game.AccessControl.SaveAdmins();
		}
	}

	Response.Subst("PassValue", CurAdmin.Password);
	Response.Subst("PrivTable", GetPrivsTable(CurAdmin.Privileges, true));
	Response.Subst("GroupLinks", "");
	Response.Subst("SubmitValue", "Accept");
	Response.Subst("PostAction", UsersAccountPage);
	Response.Subst("PageHelp", NoteAccountPage);
	ShowPage(Response, UsersAccountPage);
}

function QueryUsersBrowsePage(WebRequest Request, WebResponse Response)
{
local xAdminUser User;

	if (CanPerform("Al|Aa|Ae|Ag|Am"))
	{
		// Delete an Admin
		if (Request.GetVariable("delete") != "")
		{
			// Delete specified Admin Group	
			User = Level.Game.AccessControl.Users.FindByName(Request.GetVariable("delete"));
			if (User != None)
			{
				if (CurAdmin.CanManageUser(User))
				{
					StatusOk(Response, "User '"$HtmlEncode(User.UserName)$"' was removed!");
					// Remove User
					User.UnlinkGroups();
					Level.Game.AccessControl.Users.Remove(User);
					Level.Game.AccessControl.SaveAdmins();
				}
				else
					StatusError(Response, "Your privileges prevent you from delete this group");
			}
			else
				StatusError(Response, "Invalid group name specified");
		}
		// Show the list
		Response.Subst("BrowseList", GetUsersForBrowse(Response));

		Response.Subst("Section", "Browse Available Users");
		Response.Subst("PageHelp", NoteUsersBrowsePage);
		ShowPage(Response, UsersBrowsePage);
	}
	else
		AccessDenied(Response);
}

function QueryUsersAddPage(WebRequest Request, WebResponse Response)
{
local xAdminUser User;
local xAdminGroup Group;
local xAdminGroupList Groups;
local string uname, upass, uprivs, ugrp, ErrMsg;

	if (CanPerform("Aa"))
	{
		if (CurAdmin.bMasterAdmin)
			Groups = Level.Game.AccessControl.Groups;
		else
			Groups = CurAdmin.ManagedGroups;
			 
		if (Request.GetVariable("addnew") != "")
		{
			// Humm .. AddNew
			uname = Request.GetVariable("Username");
			upass = Request.GetVariable("Password");
			uprivs = FixPrivs(Request, "");
			ugrp = Request.GetVariable("Usergroup");
			Group = Groups.FindByName(ugrp);
			
			if (!CurAdmin.ValidName(uname))
				ErrMsg = "User name contains invalid characters!";
			else if (Level.Game.AccessControl.Users.FindByName(uname) != None)
				ErrMsg = "User name already used!";
			else if (!CurAdmin.ValidPass(upass))
				ErrMsg = "Password contains invalid characters!";
			else if (ugrp == "")
				ErrMsg = "You must select a group!";
			else if (Group == None)
				ErrMsg = "The Group you selected does not exist!";
		
			Response.Subst("NameValue", HtmlEncode(uname));
			Response.Subst("PassValue", upass);
			Response.Subst("PrivTable", GetPrivsTable(uprivs));

			if (ErrMsg == "")
			{
				// All settings are fine, create the new Group.
				User = Level.Game.AccessControl.Users.Create(uname, upass, uprivs);
				if (User != None)
				{
					User.AddGroup(Group);
					Level.Game.AccessControl.Users.Add(User);
					Level.Game.AccessControl.SaveAdmins();
				}
				else
				{
					// Only re-add the DDL if there was a problem.
					ErrMsg = "Exceptional error creating the new group";
				}
			}
			
			if (ErrMsg != "")
				StatusError(Response, ErrMsg);
		}
		else
			Response.Subst("PrivTable", GetPrivsTable(""));
		
		if (User != None)
		{
			Response.Subst("PostAction", UsersEditPage);
			Response.Subst("SubmitName", "addnew");
			Response.Subst("SubmitValue", "Modify Admin");
			Response.Subst("Section", "Modify an Administrator");
			Response.Subst("PageHelp", NoteUserEditPage);
			ShowPage(Response, UsersEditPage);
		}
		else
		{
			Response.Subst("Groups", GetGroupOptions(Groups, ugrp));
			Response.Subst("PostAction", UsersAddPage);
			Response.Subst("SubmitName", "addnew");
			Response.Subst("SubmitValue", "Add Admin");
			Response.Subst("Section", "Add a New Administrator");
			Response.Subst("PageHelp", NoteUserAddPage);
			ShowPage(Response, UsersAddPage);
		}
	}
	else
		AccessDenied(Response);
}

function QueryUsersEditPage(WebRequest Request, WebResponse Response)
{
local xAdminUser User;
local string uname, upass, privs, ErrMsg;

	if (CanPerform("Aa|Ae"))
	{
		ErrMsg = "";
		
		Response.Subst("Section", "Modify an Administrator");

		User = Level.Game.AccessControl.GetUser(Request.GetVariable("edit"));
		if (User != None)
		{
			if (CurAdmin.CanManageUser(User))
			{
				// Operations
				if (Request.GetVariable("mod") != "")
				{
					// Validate the changes and modify the user information
					uname = Request.GetVariable("Username");
					upass = Request.GetVariable("Password");
					privs = FixPrivs(Request, User.Privileges);
					if (uname != User.UserName)
					{
						if (User.ValidName(uname))
						{
							if (Level.Game.AccessControl.GetUser(uname) == None)
								User.UserName = uname;
							else
								ErrMsg = "New name already exists";
						}
						else
							ErrMsg = "New name is invalid";
					}
					
					if (ErrMsg == "" && !(upass == User.Password))
					{
						if (User.ValidPass(upass))
							User.Password = upass;
						else
							ErrMsg = "New password is invalid";
					}
					
					if (ErrMsg == "" && privs != User.Privileges)
					{
						User.Privileges = privs;
						User.RedoMergedPrivs();
					}
					if (ErrMsg == "")
						Level.Game.AccessControl.SaveAdmins();
				}
				
				if (ErrMsg != "")
					StatusError(Response, ErrMsg);
			
				Response.Subst("NameValue", HtmlEncode(User.UserName));
				Response.Subst("PassValue", HtmlEncode(User.Password));
				Response.Subst("PrivTable", GetPrivsTable(User.Privileges));
				Response.Subst("PostAction", UsersEditPage);
				Response.Subst("SubmitName", "mod");
				Response.Subst("SubmitValue", "Modify Admin");
				Response.Subst("PageHelp", NoteUserEditPage);
				ShowPage(Response, UsersEditPage);
			}
			else
				ShowMessage(Response, "No Privileges", "You do not have the privileges to modify this admin");
		}
		else
			ShowMessage(Response, "Unknown Admin", "The admin you have selected does not exist!");
	}
	else
		AccessDenied(Response);
}

function QueryUsersGroupsPage(WebRequest Request, WebResponse Response)
{
local xAdminUser		User;
local xAdminGroupList	Groups;
local xAdminGroup		Group;
local StringArray	  GrpNames;
local string GroupRows, GrpName, Str;
local int i;
local bool bModify, bChecked;

	if (CanPerform("Ag"))
	{
		User = Level.Game.AccessControl.Users.FindByName(Request.GetVariable("edit"));
		if (User != None)
		{
			if (CurAdmin.CanManageUser(User))
			{
				if (CurAdmin.bMasterAdmin)
					Groups = Level.Game.AccessControl.Groups;
				else
					Groups = CurAdmin.ManagedGroups;
				
				// Work with a table of checkboxes now
				GroupRows = "";
				bModify = (Request.GetVariable("submit") != "");

				// Make a sorted list of Groups
				GrpNames = new(None)class'SortedStringArray';
				for (i=0; i<Groups.Count(); i++)
					GrpNames.Add(Groups.Get(i).GroupName, Groups.Get(i).GroupName);

				for (i=0; i<GrpNames.Count(); i++)
				{
					GrpName = GrpNames.GetItem(i);
					Group = Groups.FindByName(GrpName);
					bChecked = Request.GetVariable(GrpName) != "";

					if (bModify)
					{
						if (User.Groups.Contains(Group))
						{
							if (!bChecked)	// Remove the user from the group
								User.RemoveGroup(Group);
						}
						else
						{
							if (bChecked)
								User.AddGroup(Group);
						}
					}
					Response.Subst("GroupName", GrpName);

					Str = "";
					if (User.Groups.Contains(Group))
						Str = " checked";
					Response.Subst("Checked", Str);
					GroupRows = GroupRows$WebInclude("users_groups_row");
				}

				if (bModify)
					Level.Game.AccessControl.SaveAdmins();

				// Now just build up the page as a table with checkboxes
				Response.Subst("NameValue", HtmlEncode(User.UserName));
				Response.Subst("GroupRows", GroupRows);
				Response.Subst("PostAction", UsersGroupsPage);
				Response.Subst("Section", "Manage Groups For "$HtmlEncode(User.UserName));
				Response.Subst("PageHelp", NoteGroupAccessPage);
				ShowPage(Response, UsersGroupsPage);
			}
			else
				ShowMessage(Response, "No Privileges", "You do not have the privileges to modify this admin");
		}
		else
			ShowMessage(Response, "Unknown Admin", "The admin you have selected does not exist!");
	}
	else
		AccessDenied(Response);
}

function QueryUsersMGroupsPage(WebRequest Request, WebResponse Response)
{
local xAdminUser		User;
local xAdminGroupList	Groups;
local xAdminGroup		Group;
local StringArray	  GrpNames;
local string GroupRows, GrpName, Str;
local int i;
local bool bModify, bChecked;

	if (CanPerform("Am"))
	{

		User = Level.Game.AccessControl.Users.FindByName(Request.GetVariable("edit"));
		if (User != None)
		{
			if (CurAdmin.CanManageUser(User))
			{
				if (CurAdmin.bMasterAdmin)
					Groups = Level.Game.AccessControl.Groups;
				else
					Groups = CurAdmin.ManagedGroups;
				
				// Work with a table of checkboxes now
				GroupRows = "";
				bModify = (Request.GetVariable("submit") != "");

				// Make a sorted list of Groups
				GrpNames = new(None)class'SortedStringArray';
				for (i=0; i<Groups.Count(); i++)
					GrpNames.Add(Groups.Get(i).GroupName, Groups.Get(i).GroupName);

				for (i=0; i<GrpNames.Count(); i++)
				{
					GrpName = GrpNames.GetItem(i);
					Group = Groups.FindByName(GrpName);
					bChecked = Request.GetVariable(GrpName) != "";

					if (bModify)
					{
						if (User.Groups.Contains(Group))
						{
							if (!bChecked)	// Remove the user from the group
								User.RemoveGroup(Group);
						}
						else
						{
							if (bChecked)
								User.AddGroup(Group);
						}
					}
					Response.Subst("GroupName", GrpName);

					Str = "";
					if (User.Groups.Contains(Group))
						Str = " checked";
					Response.Subst("Checked", Str);
					GroupRows = GroupRows$WebInclude("users_groups_row");
				}

				if (bModify)
					Level.Game.AccessControl.SaveAdmins();

				// Now just build up the page as a table with checkboxes
				Response.Subst("Managed", "Managed ");
				Response.Subst("NameValue", HtmlEncode(User.UserName));
				Response.Subst("GroupRows", GroupRows);
				Response.Subst("PostAction", UsersMGroupsPage);
				Response.Subst("Section", "Manage Groups For "$HtmlEncode(User.UserName));
				Response.Subst("PageHelp", NoteMGroupAccessPage);
				ShowPage(Response, UsersGroupsPage);
			}
			else
				ShowMessage(Response, "No Privileges", "You do not have the privileges to modify this admin");
		}
		else
			ShowMessage(Response, "Unknown Admin", "The admin you have selected does not exist!");
	}
	else
		AccessDenied(Response);
}

function QueryGroupsBrowsePage(WebRequest Request, WebResponse Response)
{
local xAdminGroup Group;

	if (CanPerform("Gl|Ge"))
	{
		Response.Subst("Section", "Browse Available Groups");
		if (Request.GetVariable("delete") != "")
		{
			// Delete specified Admin Group	
			Group = Level.Game.AccessControl.Groups.FindByName(Request.GetVariable("delete"));
			if (Group != None)
			{
				if (CurAdmin.CanManageGroup(Group))
				{
					StatusOk(Response, "Group '"$HtmlEncode(Group.GroupName)$"' was removed!");
					Group.UnlinkUsers();
					Level.Game.AccessControl.Groups.Remove(Group);
					Level.Game.AccessControl.SaveAdmins();
				}
				else
					StatusError(Response, "Your privileges prevent you from delete this group");
			}
			else
				StatusError(Response, "Invalid group name specified");
		}
		Response.Subst("BrowseList", GetGroupsForBrowse(Response));
		Response.Subst("PageHelp", NoteGroupsBrowsePage);
		ShowPage(Response, GroupsBrowsePage);
	}
	else
		AccessDenied(Response);
}

function QueryGroupsAddPage(WebRequest Request, WebResponse Response)
{
local xAdminGroup Group;
local string gname, gprivs, ErrMsg;
local int gsec;

	if (CanPerform("Ga"))
	{
		if (Request.GetVariable("addnew") != "")
		{
			// Humm .. AddNew
			gname = Request.GetVariable("GroupName");
			gprivs = FixPrivs(Request, "");
			gsec = int(Request.GetVariable("GameSec"));
			
			if (!class'xAdminGroup'.static.ValidName(gname))
				ErrMsg = "Group name contains invalid characters";
			else if (Level.Game.AccessControl.Groups.FindByName(gname) != None)
				ErrMsg = "Group name already used!";
			else if (gsec < 0)
				ErrMsg = "Negative security level is invalid";
			else if (gsec > CurAdmin.MaxSecLevel())
				ErrMsg = "You cannot assign a security level higher than yours";
		
			Response.Subst("NameValue", HtmlEncode(gname));
			Response.Subst("PrivTable", GetPrivsTable(gprivs));
			Response.Subst("GameSecValue", string(gsec));

			if (ErrMsg == "")
			{
				// All settings are fine, create the new Group.
				Group = Level.Game.AccessControl.Groups.CreateGroup(gname, gprivs, byte(gsec));
				if (Group != None)
				{
					CurAdmin.AddManagedGroup(Group);
					Level.Game.AccessControl.Groups.Add(Group);
					Level.Game.AccessControl.SaveAdmins();
				}
				else
					ErrMsg = "Exceptional error creating the new group";
			}
			
			if (ErrMsg != "")
				StatusError(Response, ErrMsg);
		}
		else
			Response.Subst("PrivTable", GetPrivsTable(""));
		
		if (Group != None)
		{
			Response.Subst("PostAction", GroupsEditPage);
			Response.Subst("SubmitName", "mod");
			Response.Subst("SubmitValue", "Modify Group");
			Response.Subst("PageHelp", NoteGroupEditPage);
			Response.Subst("Section", "Modify an Administration Group");
		}
		else
		{
			Response.Subst("PostAction", GroupsAddPage);
			Response.Subst("SubmitName", "addnew");
			Response.Subst("SubmitValue", "Add Group");
			Response.Subst("Section", "Add New Administration Group");
			Response.Subst("PageHelp", NoteGroupAddPage);
		}
		ShowPage(Response, GroupsEditPage);
	}
	else
		AccessDenied(Response);
}

function QueryGroupsEditPage(WebRequest Request, WebResponse Response)
{
local xAdminGroup Group;
local string ErrMsg, gname, gprivs;
local int gsec;

	if (CanPerform("Gm"))
	{
		Response.Subst("Section", "Modify an Administration Group");

		Group = Level.Game.AccessControl.Groups.FindByName(Request.GetVariable("edit"));
		if (Group != None)		// Do not let admins fake the system.
		{
			if (CurAdmin.CanManageGroup(Group))
			{
				if (Request.GetVariable("mod") != "")
				{
					// Save the changes
					gname = Request.GetVariable("GroupName");
					gprivs = FixPrivs(Request, Group.Privileges);
					gsec = Clamp(int(Request.GetVariable("GameSec")), 0, 255);
					if (gname != Group.GroupName)
					{
						if (Group.ValidName(gname))
						{
							if (Level.Game.AccessControl.Groups.FindByName(gname) == None)
								Group.GroupName = gname;
							else
								ErrMsg = "Group name already used!";
						}
						else
							ErrMsg = "Invalid characters in new group name";
					}

					if (ErrMsg == "")
					{
						if (gprivs != Group.Privileges)
							Group.SetPrivs(gprivs);

						Group.GameSecLevel = gsec;
						Level.Game.AccessControl.SaveAdmins();
					}
				}
			
				if (ErrMsg != "")
					StatusError(Response, ErrMsg);
			
				Response.Subst("NameValue", HtmlEncode(Group.GroupName));
				Response.Subst("PrivTable", GetPrivsTable(Group.Privileges));
				Response.Subst("GameSecValue", string(Group.GameSecLevel));
				Response.Subst("PostAction", GroupsEditPage);
				Response.Subst("SubmitName", "mod");
				Response.Subst("SubmitValue", "Modify Group");
				Response.Subst("PageHelp", NoteGroupEditPage);
				ShowPage(Response, GroupsEditPage);
			}
			else
				ShowMessage(Response, "No Privileges", "You do not have the privileges to modify this group");
		}
		else
			ShowMessage(Response, "Unknown Group", "The group you selected does not exist!");
	}
	else
		AccessDenied(Response);
}

// Must not forget to show only the Users from groups that the admin can manage
function string GetUsersForBrowse(WebResponse Response)
{
local ObjectArray	Users;
local xAdminUser	User;
local string OutStr;
local int i;
local bool CanDelete;

	CanDelete = CanPerform("Aa");
	Users = ManagedUsers();

	// Now, just make the users list a bunch of Rows
	if (Users.Count() == 0)
		return "<tr><td>** There are no admins to list **</td></tr>";


	OutStr = "<tr><td>Name</td><td>Privileges</td>";
	if (CanDelete)
		OutStr = OutStr$"<td>&nbsp;</td>";
	OutStr = OutStr$"</tr>";

	for (i = 0; i<Users.Count(); i++)
	{
		User = xAdminUser(Users.GetItem(i));
		
		Response.Subst("Username", Hyperlink(UsersEditPage$"?edit="$HtmlEncode(User.UserName), HtmlEncode(User.UserName), CanPerform("Ae|Aa")));
		Response.Subst("Privileges", User.Privileges);
		Response.Subst("Groups", "");
		Response.Subst("Managed", "");
		Response.Subst("Delete", "");
		// Build 1 Group Row
		if (CanPerform("Ag"))
			Response.Subst("Groups", Hyperlink(UsersGroupsPage$"?edit="$HtmlEncode(User.UserName),"Groups", true));
		if (CanPerform("Am"))
			Response.Subst("Managed", Hyperlink(UsersMGroupsPage$"?edit="$HtmlEncode(User.UserName),"Managed Groups", true));
		if (CanDelete)
			Response.Subst("Delete", Hyperlink(UsersBrowsePage$"?delete="$HtmlEncode(User.UserName), "Delete", true));

		OutStr = OutStr$Response.LoadParsedUHTM(Path$"/users_row.inc");
	}
	return OutStr;
}

// Must not forget to show only the Groups that the admin can add users to
function string GetGroupsForBrowse(WebResponse Response)
{
local xAdminGroup	Group;
local xAdminGroupList Groups;
local string OutStr;
local int i;
local bool CanDelete, CanEdit;

	CanDelete = CanPerform("Gd");
	CanEdit = CanPerform("Ge");

	if (CurAdmin.bMasterAdmin)
		Groups = Level.Game.AccessControl.Groups;
	else
		Groups = CurAdmin.ManagedGroups;

	if (Groups.Count() == 0)
		return "<tr><td>** There are no groups to list **</td></tr>";

	OutStr = "<tr><td>Name</td><td>Privileges</td><td>Game Sec Lvl</td>"$StringIf(CanDelete,"<td>&nbsp;</td>","")$"</tr>";
	for (i=0; i<Groups.Count(); i++)
	{
		Group = Groups.Get(i);
		// Build 1 Group Row
		Response.Subst("Groupname", Hyperlink(GroupsEditPage$"?edit="$HtmlEncode(Group.GroupName),HtmlEncode(Group.GroupName),true));
		Response.Subst("Privileges", Group.Privileges);
		Response.Subst("Gamesec", string(Group.GameSecLevel));
		Response.Subst("Delete", "");
		if (CanDelete)
		  Response.Subst("Delete", Hyperlink(GroupsBrowsePage$"?delete="$HtmlEncode(Group.GroupName), "Delete", true));

		OutStr = OutStr$Response.LoadParsedUHTM(Path$"/groups_row.inc");
	}
	return OutStr;
}

function string GetPrivsHeader(string privs, string text, bool cond, string tag)
{
local string headfile;

	headfile="/privs_header.inc";
	if (cond)
		headfile="/privs_header_chk.inc";

	Resp.Subst("Tag", tag);
	Resp.Subst("Checked", StringIf(Instr("|"$privs$"|", "|"$tag$"|") != -1, " checked", ""));
	Resp.Subst("Text", text);

	return Resp.LoadParsedUHTM(Path$headfile);
}

function string GetPrivsItem(string privs, string text, bool cond, string tag, optional bool bReadOnly)
{
	if (!cond)
		return "";
	
	Resp.Subst("Tag", tag);
	Resp.Subst("Text", text);
	Resp.Subst("Checked", StringIf(Instr("|"$privs$"|", "|"$tag$"|") != -1, " checked", ""));

	if (bReadOnly)
		return Resp.LoadParsedUHTM(Path$"/privs_element_ro.inc");

	return Resp.LoadParsedUHTM(Path$"/privs_element.inc");
}

function ObjectArray ManagedUsers()
{
local ObjectArray Users;
local int i, j;
local xAdminGroup Group;
local xAdminUser User;
local xAdminGroupList Groups;

	Users = New(None) class'SortedObjectArray';	
	
	if (CurAdmin.bMasterAdmin)
		Groups = Level.Game.AccessControl.Groups;
	else
		Groups = CurAdmin.ManagedGroups;
	
	for (i=0; i<Groups.Count(); i++)
	{
		Group = Groups.Get(i);
		for (j=0; j<Group.Users.Count(); j++)
		{
			User = Group.Users.Get(j);
			if (Users.FindTagId(User.UserName) < 0)
				Users.Add(User, User.UserName);
		}
	}
	return Users;
}

function string MakePrivsTable(xPrivilegeBase PM, string privs, bool bNoEdit)
{
local int pi, colcnt, maxcols;
local string mprivs, sprivs, mpriv, priv, OutStr;
local bool   bCan, bCanPriv, bCanEdit;

	mprivs=PM.MainPrivs;
	OutStr = "";
	pi = 0;
	maxcols=3;
	while (mprivs != "")
	{
		// Step 1: Check for a main priv type if the Manager can process anything
		mpriv = NextPriv(mprivs);
		bCan = CanPerform(mpriv);
		// If cant manage the whole group, check individually
		if (!bCan)
		{
			sprivs=PM.SubPrivs;
			while (!bCan && sprivs != "")
			{
				priv = NextPriv(sprivs);
				if (Left(priv,1) == mpriv)
					bCan = CanPerform(priv);
			}
		}

		// If we could manage anything, lets make checkboxes for them
		bCanEdit = false;
		if (!bNoEdit)
			bCanEdit = CanPerform(mpriv);

		if (bCan)
			OutStr = OutStr$GetPrivsHeader(privs, PM.Tags[pi], bCanEdit, mpriv)$"<table><tr>";
			
		pi++;
		
		sprivs = PM.SubPrivs;
		colcnt = 0;

		while (sprivs != "")
		{
			priv = NextPriv(sprivs);
			bCanPriv = CanPerform(priv);

			if (bNoEdit)
				bCanEdit = false;
			else
				bCanEdit = bCanPriv;

			if (Left(priv,1) == mpriv)
			{
				if (bCan && bCanPriv)
				{
					colcnt++;
					if (colcnt > maxcols)
					{
						colcnt -= maxcols;
						OutStr = OutStr$"</tr><tr>";
					}

					OutStr = OutStr$GetPrivsItem(privs, PM.Tags[pi], true, priv, !bCanEdit);
				}

				pi++;
			}
		}
		if (bCan)
			OutStr = OutStr$"</tr></table>";
	}
	return OutStr;
}

function string GetPrivsTable(string privs, optional bool bNoEdit)
{
local string str;
local int i;

	// Start by getting all rows for known privilege groups
	str = "";
	for (i=0; i<Level.Game.AccessControl.PrivManagers.Length; i++)
		str = str$MakePrivsTable(Level.Game.AccessControl.PrivManagers[i], privs, bNoEdit);

	if (str == "")
		str = "You cannot assign privileges";
	return str;
}

function string FixPrivs(WebRequest Request, string oldprivs)
{
local string privs, myprivs, priv;

	if (CurAdmin.bMasterAdmin)
		myprivs = Level.Game.AccessControl.AllPrivs;
	else
		myprivs = CurAdmin.MergedPrivs;

	// Before starting, remove privs i can manage
	privs = "";
	while (oldprivs != "")
	{
		priv = NextPriv(oldprivs);
		if (Instr("|"$myprivs$"|", "|"$priv$"|") == -1)
		{
			if (privs == "")
				privs = priv;
			else
				privs = privs$"|"$priv;
		}
	}		
	
	while (myprivs != "")
	{
		priv = NextPriv(myprivs);
		if (Request.GetVariable(priv) != "")
		{
			if (privs == "")
				privs = priv;
			else
				privs = privs$"|"$priv;
		}
	}
	return privs;
}

function string GetGroupOptions(xAdminGroupList Groups, string grpsel)
{
local int i;
local string OutStr, GrpName;
local StringArray	  GrpNames;

	if (Groups.Count() == 0)
		return "<option value=\"\">*** None ***</option>";

	// Step 1: Sort the groups
	GrpNames = new(None) class'SortedStringArray';
	for (i=0; i<Groups.Count(); i++)
		GrpNames.Add(Groups.Get(i).GroupName, Groups.Get(i).GroupName);

	if (GrpNames.Count() == 0)
		return "<option value=\"\">*** None ***</option>";
		
	// Step 2: Build the group list
	OutStr = "";
	for (i=0; i<GrpNames.Count(); i++)
	{
		GrpName = GrpNames.GetItem(i);
		OutStr = OutStr$"<option value='"$GrpName$"'";
		if (GrpName == grpsel)
			OutStr = OutStr$" selected";
		OutStr = OutStr$">"$HtmlEncode(GrpName)$"</option>";
	}
	return OutStr;
}

defaultproperties
{
	Title="Admins & Groups"
	DefaultPage="adminsframe"
	AdminsIndexPage="admins_menu"
	UsersHomePage="admins_home"
	UsersAccountPage="admins_account"
	UsersAddPage="users_add"
	UsersBrowsePage="users_browse"
	UsersEditPage="users_edit"
	UsersGroupsPage="users_groups"
	UsersMGroupsPage="users_mgroups"
	GroupsAddPage="groups_add"
	GroupsBrowsePage="groups_browse"
	GroupsEditPage="groups_edit"

	NoteUserHomePage="Welcome to Admins &amp; Groups Management"
	NoteAccountPage="Here you can change your password if required. You can also see which privileges were assigned to you by your manager."
	NoteUserAddPage="As an Admin of this server you can add new Admins and give them privileges. Make sure that the password assigned to the new Admin is not easy to hack."
	NoteUserEditPage="As an Admin of this server you can modify informations and privileges for another Admin that you can Manage."
	NoteUsersBrowsePage="Here you can see other Admins that you can manage and modify their privilege and groups assignment."
	NoteGroupAddPage="You can create new groups which will have a common set of privileges. Groups are used to give the same privileges to multiple Admins."
	NoteGroupEditPage="You can modify which privileges were assigned to this group. Note that you can only change privileges that you have yourself."
	NoteGroupsBrowsePage="Here you can see all the groups that you can manage, click on a group name to modify it."
	NoteGroupAccessPage="Here you can decide in which groups the selected admin will be part of. This will decide which base privileges this admin will have."
	NoteMGroupAccessPage="Here you can decide which groups this admin will be able to manage. He will be able to assign other admins to this group."
}
