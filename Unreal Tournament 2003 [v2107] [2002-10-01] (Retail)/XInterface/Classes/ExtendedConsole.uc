// ====================================================================
//  Class:  XInterface.ExtendedConsole
//  Parent: Engine.Console
//
//  <Enter a description here>
// ====================================================================

class ExtendedConsole extends Console;

#exec OBJ LOAD FILE=MenuSounds.uax
#exec OBJ LOAD FILE=InterfaceContent.utx

// Visible Console stuff

var globalconfig int MaxScrollbackSize;

var array<string> Scrollback;
var int SBHead, SBPos;	// Where in the scrollback buffer are we
var bool bCtrl;
var bool bConsoleHotKey;

var float   ConsoleSoundVol;

var localized string AddedCurrentHead;
var localized string AddedCurrentTail;

////// Speech Menu
var float SMLineSpace;

var enum ESpeechMenuState
{
	SMS_Main,
	SMS_Ack,
	SMS_FriendFire,
	SMS_Order,
	SMS_Other,
	SMS_Taunt,
	SMS_TauntAnim,
	SMS_PlayerSelect,
} SMState;

// These are put together using the menu and sent to the 'Speech' exec command.
var name SMType;
var int  SMIndex;
var string SMCallsign;

var int SMOffset;

var string	SMNameArray[48];
var int		SMIndexArray[48];
var int		SMArraySize;

var float SMOriginX, SMOriginY;
var float SMMargin, SMTab;

var localized string  SMStateName[8];
var localized string  SMAllString;
var localized string  SMMoreString;

var sound	SMOpenSound;
var sound   SMAcceptSound;
var sound   SMDenySound;

struct StoredPassword
{
	var config string	Server,
						Password;
};

var config array<StoredPassword>	SavedPasswords;
var config string					PasswordPromptMenu;
var string							LastConnectedServer,
									LastURL;


function OnStatsConfigured()
{
	ViewportOwner.GUIController.CloseAll(false);
	ViewportOwner.Actor.ClientTravel(LastURL,TRAVEL_Absolute,false);
}

event ConnectFailure(string FailCode,string URL)
{
	local string			Server;
	local int				Index;

	LastURL = URL;
	Server = Left(URL,InStr(URL,"/"));

	if(FailCode == "NEEDPW")
	{
		for(Index = 0;Index < SavedPasswords.Length;Index++)
		{
			if(SavedPasswords[Index].Server == Server)
			{
				ViewportOwner.Actor.ClearProgressMessages();
				ViewportOwner.Actor.ClientTravel(URL$"?password="$SavedPasswords[Index].Password,TRAVEL_Absolute,false);
				return;
			}
		}

		LastConnectedServer = Server;
		ViewportOwner.Actor.ClientOpenMenu(
			PasswordPromptMenu,
			false,
			URL,
			""
			);
		return;
	}
	else if(FailCode == "WRONGPW")
	{
		ViewportOwner.Actor.ClearProgressMessages();

		for(Index = 0;Index < SavedPasswords.Length;Index++)
		{
			if(SavedPasswords[Index].Server == Server)
			{
				SavedPasswords.Remove(Index,1);
				SaveConfig();
			}
		}

		LastConnectedServer = Server;
		ViewportOwner.Actor.ClientOpenMenu(
			PasswordPromptMenu,
			false,
			URL,
			""
			);
		return;
	}
	else if(FailCode == "NEEDSTATS")
	{
		ViewportOwner.Actor.ClearProgressMessages();
		ViewportOwner.Actor.ClientOpenMenu(
			"XInterface.UT2StatsPrompt",
			false,
			"",
			""
			);
		UT2StatsPrompt(GUIController(ViewportOwner.GUIController).MenuStack[GUIController(ViewportOwner.GUIController).MenuStack.Length - 1]).OnStatsConfigured = OnStatsConfigured;
		return;
	}

	ViewportOwner.Actor.ProgressCommand("menu:"$FailCode,"","");
}


event NotifyLevelChange()
{
	Super.NotifyLevelChange();
//	GUIController(ViewportOwner.GUIController).CloseAll(false);
}


////// End Speech Menu

exec function CLS()
{
	SBHead = 0;
	ScrollBack.Remove(0,ScrollBack.Length);
}

function PostRender( canvas Canvas );	// Subclassed in state

event Message( coerce string Msg, float MsgLife)
{
	if (ScrollBack.Length==MaxScrollBackSize)	// if full, Remove Entry 0
	{
		ScrollBack.Remove(0,1);
		SBHead = MaxScrollBackSize-1;
	}
	else
		SBHead++;		
	
	ScrollBack.Length = ScrollBack.Length + 1;
	
	Scrollback[SBHead] = Msg;
	Super.Message(Msg,MsgLife);
}

event bool KeyEvent( EInputKey Key, EInputAction Action, FLOAT Delta )
{
	if (Key==ConsoleHotKey)
	{
		if(Action==IST_Release)
			ConsoleOpen();
		return true;
	}

    return Super.KeyEvent(Key,Action,Delta);
}


function PlayConsoleSound(Sound S)
{
	if(ViewportOwner == None || ViewportOwner.Actor == None || ViewportOwner.Actor.Pawn == None)
		return;

	ViewportOwner.Actor.ClientPlaySound(S);//,true,ConsoleSoundVol);
}

//-----------------------------------------------------------------------------
// State used while typing a command on the console.

event NativeConsoleOpen()
{
	ConsoleOpen();
}

exec function ConsoleOpen()
{
	TypedStr = "";
	GotoState('ConsoleVisible');
	PlayConsoleSound(SMOpenSound);
}

exec function ConsoleClose()
{
	TypedStr="";
    if( GetStateName() == 'ConsoleVisible' )
	{
		PlayConsoleSound(SMDenySound);
        GotoState( '' ); 
	}
}

exec function ConsoleToggle()
{
    if( GetStateName() == 'ConsoleVisible' )
        ConsoleClose();
    else
        ConsoleOpen();
}

state ConsoleVisible
{
	function bool KeyType( EInputKey Key, optional string Unicode )
	{
		// uncomment the //@ lines to activate copy/paste in console
		//@local PlayerController PC; 

		if (bIgnoreKeys || bConsoleHotKey)		
			return true;

		//@if (ViewportOwner != none)
		//@	PC = ViewportOwner.Actor;

		//@if (bCtrl && PC != none)
		//@{
		//@	if (Key == 3) //copy
		//@	{
		//@		PC.CopyToClipboard(TypedStr);
		//@		return true;
		//@	} 
		//@	else if (Key == 22) //paste
		//@	{
		//@		TypedStr = TypedStr$PC.PasteFromClipboard();
		//@		return true;
		//@	}
		//@	else if (Key == 24) // cut
		//@	{
		//@		PC.CopyToClipboard(TypedStr);
		//@		TypedStr="";
		//@		return true;
		//@	}
		//@}

		if( Key>=0x20 )
		{
			if( Unicode != "" )
				TypedStr = TypedStr $ Unicode;
			else
				TypedStr = TypedStr $ Chr(Key);
            return( true );
		}
		
		return( true );
	}

	function bool KeyEvent( EInputKey Key, EInputAction Action, FLOAT Delta )
	{
		local string Temp;
    
		if( Key==IK_Ctrl )
		{
			if (Action == IST_Press)
				bCtrl = true;
			else if (Action == IST_Release)
				bCtrl = false;
		}

		if (Action== IST_PRess)
		{
			bIgnoreKeys = false;
		}
	
		if(Key == ConsoleHotKey)
		{
			if(Action == IST_Press)
				bConsoleHotKey = true;
			else if(Action == IST_Release && bConsoleHotKey)
				ConsoleClose();
			return true;
		}
		else if (Key==IK_Escape)
		{
			if (Action==IST_Release)
			{
				if (TypedStr!="") 
				{
					TypedStr="";
					HistoryCur = HistoryTop;
				}
				else
				{
	                ConsoleClose();
					return true;
				}
			}
			return true;
		}
		else if( Action != IST_Press )
            return( true );

		else if( Key==IK_Enter )
		{
			if( TypedStr!="" )
			{
				// Print to console.

				History[HistoryTop] = TypedStr;
                HistoryTop = (HistoryTop+1) % ArrayCount(History);
				
				if ( ( HistoryBot == -1) || ( HistoryBot == HistoryTop ) )
                    HistoryBot = (HistoryBot+1) % ArrayCount(History);

				HistoryCur = HistoryTop;

				// Make a local copy of the string.
				Temp=TypedStr;
				TypedStr="";
				
				if( !ConsoleCommand( Temp ) )
					Message( Localize("Errors","Exec","Core"), 6.0 );
					
				Message( "", 6.0 );
			}

            return( true );
		}
		else if( Key==IK_Up )
		{
			if ( HistoryBot >= 0 )
			{
				if (HistoryCur == HistoryBot)
					HistoryCur = HistoryTop;
				else
				{
					HistoryCur--;
					if (HistoryCur<0)
                        HistoryCur = ArrayCount(History)-1;
				}
				
				TypedStr = History[HistoryCur];
			}
            return( true );
		}
		else if( Key==IK_Down )
		{
			if ( HistoryBot >= 0 )
			{
				if (HistoryCur == HistoryTop)
					HistoryCur = HistoryBot;
				else
                    HistoryCur = (HistoryCur+1) % ArrayCount(History);
					
				TypedStr = History[HistoryCur];
			}			

		}
		else if( Key==IK_Backspace || Key==IK_Left )
		{
			if( Len(TypedStr)>0 )
				TypedStr = Left(TypedStr,Len(TypedStr)-1);
            return( true );
		}
		
		else if ( Key==IK_PageUp || key==IK_MouseWheelUp )
		{
			if (SBPos<ScrollBack.Length-1)
			{
				if (bCtrl)
					SBPos+=5;
				else
					SBPos++;
				
				if (SBPos>=ScrollBack.Length)
				  SBPos = ScrollBack.Length-1;
			}
				
			return true;
		}			
		else if ( Key==IK_PageDown || key==IK_MouseWheelDown)
		{
			if (SBPos>0)
			{
				if (bCtrl)
					SBPos-=5;
				else
					SBPos--;
				
				if (SBPos<0)
					SBPos = 0;
			}
		}		
		
        return( true );
	}
	
    function BeginState()
	{
		SBPos = 0;
        bVisible= true;
		bIgnoreKeys = true;
		bConsoleHotKey = false;
        HistoryCur = HistoryTop;
		bCtrl = false;
    }
    function EndState()
    {
        bVisible = false;
		bCtrl = false;
		bConsoleHotKey = false;
    }
	
	function PostRender( canvas Canvas )
	{
	
		local float fw,fh;
		local float yclip,y;
		local int idx;
		
		Canvas.Font = class'HudBase'.static.GetConsoleFont(Canvas);
		yclip = canvas.ClipY*0.5;
		Canvas.StrLen("X",fw,fh); 
		
		Canvas.SetPos(0,0);
		Canvas.SetDrawColor(255,255,255,200);
		Canvas.Style=4;
		Canvas.DrawTileStretched(material'ConsoleBack',Canvas.ClipX,yClip);
		Canvas.Style=1;

		Canvas.SetPos(0,yclip-1);
		Canvas.SetDrawColor(255,255,255,255);
		Canvas.DrawTile(texture 'InterfaceContent.Menu.BorderBoxA',Canvas.ClipX,2,0,0,64,2);
		
		Canvas.SetDrawColor(255,255,255,255);

		Canvas.SetPos(0,yclip-5-fh);
		Canvas.DrawText("(>"@TypedStr$"_");
		
		idx = SBHead - SBPos;
		y = yClip-y-5-(fh*2);

		if (ScrollBack.Length==0)
			return;
		
		Canvas.SetDrawColor(255,255,255,255);
		while (y>fh && idx>=0)
		{
			Canvas.SetPos(0,y);
			Canvas.DrawText(Scrollback[idx],false);
			idx--;
			y-=fh;
		}			 			
	}
}

//---------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------//

exec function AddCurrentToFavorites()
{
	local string address, ipString, portString;
	local int colonPos, portNum;

	if(ViewportOwner == None || ViewportOwner.Actor == None)
		return;

	// Get current network address
	address = ViewportOwner.Actor.GetServerNetworkAddress();

	if(address == "")
		return;

	// Parse text to find IP and possibly port number
	colonPos = InStr(address, ":");
	if(colonPos < 0)
	{
		// No colon - assume port 7777
		ipString = address;
		portNum = 7777;
	}
	else
	{	// Parse out port number
		ipString = Left(address, colonPos);
		portString = Mid(address, colonPos+1);
		portNum = int(portString);
	}

	// Add this server to the default list of favorites in the favorites page
	class'Browser_ServerListPageFavorites'.static.StaticAddFavorite(ipString, portNum, portNum+1);

	ViewportOwner.Actor.ClientMessage(AddedCurrentHead@address@AddedCurrentTail);
}

//---------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------//

exec function SpeechMenuToggle()
{
    if( GetStateName() == 'SpeechMenuVisible' )
	{
		GotoState('');
		return;
	}

	// Dont let spectators use the speech menu
	if(ViewportOwner.Actor.PlayerReplicationInfo.bOnlySpectator)
		return;

	// Don't show speech menu if no voice pack type
    if( ViewportOwner.Actor.PlayerReplicationInfo.VoiceType == None ) 
		return;

	GotoState('SpeechMenuVisible');
}

state SpeechMenuVisible
{
	function bool KeyType( EInputKey Key, optional string Unicode )
	{
		if (bIgnoreKeys)		
			return true;

		return false;
	}

	function class<TeamVoicePack> GetVoiceClass()
	{
		if(ViewportOwner == None || ViewportOwner.Actor == None || ViewportOwner.Actor.PlayerReplicationInfo == None)
			return None;

		return class<TeamVoicePack>(ViewportOwner.Actor.PlayerReplicationInfo.VoiceType);
	}

	// JTODO: Bubble sort. Sorry. But I already wrote the GUIList sort today and its late.
	function SortSMArray()
	{
		local int i,j, tmpInt;
		local string tmpString;

		for(i=0; i<SMArraySize-1; i++)
		{
			for(j=i+1; j<SMArraySize; j++)
			{
				if(SMNameArray[i] > SMNameArray[j])
				{
					tmpString = SMNameArray[i];
					SMNameArray[i] = SMNameArray[j];
					SMNameArray[j] = tmpString;

					tmpInt = SMIndexArray[i];
					SMIndexArray[i] = SMIndexArray[j];
					SMIndexArray[j] = tmpInt;
				}
			}
		}
	}

	// Rebuild the array of options based on the state we are now in.
	function RebuildSMArray()
	{
		local int i, index;
		local class<TeamVoicePack> tvp;
		local GameReplicationInfo GRI;
		local PlayerReplicationInfo MyPRI;
		local UnrealPlayer up;
		local name GameMesgGroup;

		SMArraySize = 0;
		SMOffset=0;

		tvp = GetVoiceClass();
		if(tvp == None)
			return;

		//Log("TVP:"$tvp$" NumTaunts:"$tvp.Default.numTaunts);
		if(SMState == SMS_Main)
		{
			for(i=1; i<7; i++)
			{
				SMNameArray[SMArraySize] = SMStateName[i];
				SMIndexArray[SMArraySize] = i;
				SMArraySize++;
			}
		}
		else if(SMState == SMS_Taunt)
		{
			for(i=0; i<tvp.Default.numTaunts; i++)
			{
				if(tvp.Default.MatureTaunt[i] == 1 && ViewportOwner.Actor.bNoMatureLanguage)
					continue;

				if(tvp.Default.TauntAbbrev[i] != "")
					SMNameArray[SMArraySize] = tvp.Default.TauntAbbrev[i];
				else
					SMNameArray[SMArraySize] = tvp.Default.TauntString[i];

				SMIndexArray[SMArraySize] = i;
				SMArraySize++;
			}

			SortSMArray();
		}
		else if(SMState == SMS_Ack)
		{
			for(i=0; i<tvp.Default.numAcks; i++)
			{
				SMNameArray[SMArraySize] = tvp.Default.AckString[i];
				SMIndexArray[SMArraySize] = i;
				SMArraySize++;
			}

			SortSMArray();
		}
		else if(SMState == SMS_FriendFire)
		{
			for(i=0; i<tvp.Default.numFFires; i++)
			{
				if(tvp.Default.FFireAbbrev[i] != "")
					SMNameArray[SMArraySize] = tvp.Default.FFireAbbrev[i];
				else
					SMNameArray[SMArraySize] = tvp.Default.FFireString[i];

				SMIndexArray[SMArraySize] = i;
				SMArraySize++;
			}

			SortSMArray();
		}
		else if(SMState == SMS_Order)
		{
			for(i=0; i<9; i++)
			{
				if(tvp.Default.OrderSound[i] == None)
					continue;

				index = tvp.static.OrderToIndex(i, ViewportOwner.Actor.Level.GetGameClass());

				if(tvp.Default.OrderAbbrev[index] != "")
					SMNameArray[SMArraySize] = tvp.Default.OrderAbbrev[index];
				else
					SMNameArray[SMArraySize] = tvp.Default.OrderString[index];

				SMIndexArray[SMArraySize] = index;
				SMArraySize++;
			}
		}
		else if(SMState == SMS_Other)
		{
			GameMesgGroup = ViewportOwner.Actor.Level.GetGameClass().Default.OtherMesgGroup;

			for(i=0; i<ArrayCount(tvp.Default.OtherString); i++)
			{
				if(tvp.Default.OtherSound[i] == None)
					continue;

				// If we have defined a group for this message, only put it in the menu if it matches the current game group.
				if(tvp.Default.OtherMesgGroup[i] != ''  && tvp.Default.OtherMesgGroup[i] != GameMesgGroup)
						continue;

				if(tvp.Default.OtherAbbrev[i] != "")
					SMNameArray[SMArraySize] = tvp.Default.OtherAbbrev[i];
				else
					SMNameArray[SMArraySize] = tvp.Default.OtherString[i];

				SMIndexArray[SMArraySize] = i;
				SMArraySize++;
			}
		}
		else if(SMState == SMS_PlayerSelect)
		{
			if(ViewportOwner == None || ViewportOwner.Actor == None || ViewportOwner.Actor.PlayerReplicationInfo == None)
				return;

			GRI = ViewportOwner.Actor.GameReplicationInfo;
			MyPRI = ViewportOwner.Actor.PlayerReplicationInfo;

			// First entry is to send to 'all'
			// HACK: Don't let you send 'Hold This Position' to all all bots
			if( SMIndex != 1)
			{
				SMNameArray[SMArraySize] = SMAllString;
				SMArraySize++;
			}

			for(i=0; i<GRI.PRIArray.Length; i++)
			{
				// Dont put player on list if myself, not on a team, on the same team, or a spectator.
				if( GRI.PRIArray[i].Team == None || MyPRI.Team == None )
					continue;

				if( GRI.PRIArray[i].Team.TeamIndex != MyPRI.Team.TeamIndex )
					continue;

				if( GRI.PRIArray[i].TeamId == MyPRI.TeamId )
					continue;

				if( GRI.PRIArray[i].bOnlySpectator )
					continue;

				SMNameArray[SMArraySize] = GRI.PRIArray[i].PlayerName;
				SMArraySize++;
				// Dont need a number- we use the name direct
			}

			SortSMArray();
		}
		else if(SMState == SMS_TauntAnim)
		{
			if(ViewportOwner == None || ViewportOwner.Actor == None)
				return;

			up = UnrealPlayer(ViewportOwner.Actor);
			if(up == None || up.Pawn == None)
				return;

			for(i=0; i<up.Pawn.TauntAnims.Length; i++)
			{
				SMNameArray[SMArraySize] = up.Pawn.TauntAnimNames[Clamp(i,0,7)];  // clamped because taunt array is max 8, see Pawn.uc
				SMIndexArray[SMArraySize] = i;
				SMArraySize++;
			}

			SortSMArray();
		}
	}

	//////////////////////////////////////////////

	function EnterState(ESpeechMenuState newState, optional bool bNoSound)
	{
		SMState = newState;
		RebuildSMArray();

		if(!bNoSound)
			PlayConsoleSound(SMAcceptSound);
	}

	function LeaveState() // Go up a level
	{
		PlayConsoleSound(SMDenySound);

		if(SMState == SMS_Main)
		{
			GotoState('');
		}
		else if(SMState == SMS_PlayerSelect)
			EnterState(SMS_Order, true);
		else
			EnterState(SMS_Main, true);
	}
	// // // // // //

	function HandleInput(int keyIn)
	{
		local int selectIndex;
		local UnrealPlayer up;


		// GO BACK - previous state (might back out of menu);
		if(keyIn == -1)
		{
			LeaveState();
			return;
		}

		// TOP LEVEL - we just enter a new state
		if(SMState == SMS_Main)
		{
			switch(keyIn)
			{
			case 1: SMType = 'ACK'; EnterState(SMS_Ack); break;
			case 2: SMType = 'FRIENDLYFIRE'; EnterState(SMS_FriendFire); break;
			case 3: SMType = 'ORDER'; EnterState(SMS_Order); break;
			case 4: SMType = 'OTHER'; EnterState(SMS_Other); break;
			case 5: SMType = 'TAUNT'; EnterState(SMS_Taunt); break;
			case 6: SMType = ''; EnterState(SMS_TauntAnim); break;
			}

			return;
		}

		// Next page on the same level
		if(keyIn == 0 )
		{
			// Check there is a next page!
			if(SMArraySize - SMOffset > 9)
				SMOffset += 9;
			return;
		}

		// Otherwise - we have selected something!
		selectIndex = SMOffset + keyIn - 1;
		if(selectIndex < 0 || selectIndex >= SMArraySize) // discard - out of range selections.
			return;

		if(SMState == SMS_Order)
		{
			SMIndex = SMIndexArray[selectIndex];
			EnterState(SMS_PlayerSelect);
		}
		else if(SMState == SMS_PlayerSelect)
		{
			if(SMNameArray[selectIndex] == SMAllString)
				ViewportOwner.Actor.Speech(SMType, SMIndex, "");
			else
				ViewportOwner.Actor.Speech(SMType, SMIndex, SMNameArray[selectIndex]);

			PlayConsoleSound(SMAcceptSound);

			GotoState(''); // Close menu after message
		}
		else if(SMState == SMS_TauntAnim)
		{
			up = UnrealPlayer(ViewportOwner.Actor);
			up.Taunt( up.Pawn.TauntAnims[ SMIndexArray[selectIndex] ] );
			PlayConsoleSound(SMAcceptSound);
			GotoState('');
		}
		else
		{
			ViewportOwner.Actor.Speech(SMType, SMIndexArray[selectIndex], "");
			PlayConsoleSound(SMAcceptSound);
			GotoState('');
		}
	}

	//////////////////////////////////////////////

	function DrawNumbers( canvas Canvas, int NumNums, bool IncZero, bool sizing, out float XMax, out float YMax )
	{
		local int i;
		local float XPos, YPos;
		local float XL, YL;

		XPos = Canvas.ClipX * (SMOriginX+SMMargin);
		YPos = Canvas.ClipY * (SMOriginY+SMMargin);
		Canvas.SetDrawColor(128,255,128,255);

		for(i=0; i<NumNums; i++)
		{
			Canvas.SetPos(XPos, YPos);
			if(!sizing)
				Canvas.DrawText((i+1)$"-", false);
			else
			{
				Canvas.TextSize((i+1)$"-", XL, YL);
				XMax = Max(XMax, XPos + XL);
				YMax = Max(YMax, YPos + YL);
			}
				
			YPos += SMLineSpace;
		}

		if(IncZero)
		{
			Canvas.SetPos(XPos, YPos);

			if(!sizing)
				Canvas.DrawText("0-", false);

			XPos += SMTab;
			Canvas.SetPos(XPos, YPos);

			if(!sizing)
				Canvas.DrawText(SMMoreString, false);
			else
			{
				Canvas.TextSize(SMMoreString, XL, YL);
				XMax = Max(XMax, XPos + XL);
				YMax = Max(YMax, YPos + YL);
			}
		}
	}

	function DrawCurrentArray( canvas Canvas, bool sizing, out float XMax, out float YMax )
	{
		local int i, stopAt;
		local float XPos, YPos;
		local float XL, YL;

		XPos = (Canvas.ClipX * (SMOriginX+SMMargin)) + SMTab;
		YPos = Canvas.ClipY * (SMOriginY+SMMargin);
		Canvas.SetDrawColor(255,255,255,255);

		stopAt = Min(SMOffset+9, SMArraySize);
		for(i=SMOffset; i<stopAt; i++)
		{
			Canvas.SetPos(XPos, YPos);
			if(!sizing)
				Canvas.DrawText(SMNameArray[i], false);
			else
			{
				Canvas.TextSize(SMNameArray[i], XL, YL);
				XMax = Max(XMax, XPos + XL);
				YMax = Max(YMax, YPos + YL);
			}

			YPos += SMLineSpace;
		}
	}

	//////////////////////////////////////////////

	function bool KeyEvent( EInputKey Key, EInputAction Action, FLOAT Delta )
	{

		// While speech menu is up, dont let user use console. Debateable.
		//if( KeyIsBoundTo( Key, "ConsoleToggle" ) )
		//	return true;
		//if( KeyIsBoundTo( Key, "Type" ) )
		//	return true;

		if (Action== IST_Press)
			bIgnoreKeys=false;

		if( Action != IST_Press )
			return false;

		if( Key==IK_Escape)
		{
			HandleInput(-1);
			return true ;
		}

		else if( Key == IK_1) { HandleInput(1); return true; }
		else if( Key == IK_2) { HandleInput(2); return true; }
		else if( Key == IK_3) { HandleInput(3); return true; }
		else if( Key == IK_4) { HandleInput(4); return true; }
		else if( Key == IK_5) { HandleInput(5); return true; }
		else if( Key == IK_6) { HandleInput(6); return true; }
		else if( Key == IK_7) { HandleInput(7); return true; }
		else if( Key == IK_8) { HandleInput(8); return true; }
		else if( Key == IK_9) { HandleInput(9); return true; }
		else if( Key == IK_0) { HandleInput(0); return true; }

		return false;
	}

	function Font MyGetSmallFontFor(canvas Canvas)
	{
		local int i;
		for(i=1; i<8; i++)
		{
			if ( class'HudBase'.default.FontScreenWidthSmall[i] <= Canvas.ClipX )
				return class'HudBase'.default.FontArray[i-1];
		}
		return class'HudBase'.default.FontArray[7];
	}

	function PostRender( canvas Canvas )
	{
		local float XL, YL;
		local int SelLeft;
		local float XMax, YMax;

		Canvas.Font = class'UT2MidGameFont'.static.GetMidGameFont(Canvas.ClipX); // Update which font to use.

		Canvas.TextSize("10-", XL, YL);
		SMLineSpace = YL * 1.1;
		SMTab = XL;

		SelLeft = SMArraySize - SMOffset;

		// First we figure out how big the bounding box needs to be
		XMax = 0;
		YMax = 0;
		DrawNumbers( canvas, Min(SelLeft, 9), SelLeft > 9, true, XMax, YMax);
		DrawCurrentArray( canvas, true, XMax, YMax);
		Canvas.TextSize(SMStateName[SMState], XL, YL);
		XMax = Max(XMax, Canvas.ClipX*(SMOriginX+SMMargin) + XL);
		YMax = Max(YMax, (Canvas.ClipY*SMOriginY) - (1.2*SMLineSpace) + YL);
		// XMax, YMax now contain to maximum bottom-right corner we drew to.

		// Then draw the box
		XMax -= Canvas.ClipX * SMOriginX;
		YMax -= Canvas.ClipY * SMOriginY;
		Canvas.SetDrawColor(255,255,255,255);
		Canvas.SetPos(Canvas.ClipX * SMOriginX, Canvas.ClipY * SMOriginY);
		Canvas.DrawTileStretched(texture 'InterfaceContent.Menu.BorderBoxD', XMax + (SMMargin*Canvas.ClipX), YMax + (SMMargin*Canvas.ClipY));

		// Then actually draw the stuff
		DrawNumbers( canvas, Min(SelLeft, 9), SelLeft > 9, false, XMax, YMax);
		DrawCurrentArray( canvas, false, XMax, YMax);

		// Finally, draw a nice title bar.
		Canvas.SetDrawColor(255,255,255,255);
		Canvas.SetPos(Canvas.ClipX*SMOriginX, (Canvas.ClipY*SMOriginY) - (1.5*SMLineSpace));
		Canvas.DrawTileStretched(texture 'InterfaceContent.Menu.BorderBoxD', XMax + (SMMargin*Canvas.ClipX), (1.5*SMLineSpace));
		
		Canvas.SetDrawColor(255,255,128,255);
		Canvas.SetPos(Canvas.ClipX*(SMOriginX+SMMargin), (Canvas.ClipY*SMOriginY) - (1.2*SMLineSpace));
		Canvas.DrawText(SMStateName[SMState]);
	}

    function BeginState()
	{
        bVisible = true;
		bIgnoreKeys = true;
		bCtrl = false;
	
		EnterState(SMS_Main, true);
		SMCallsign="";

		PlayConsoleSound(SMOpenSound);
	}

    function EndState()
    {
        bVisible = false;
		bCtrl = false;
    }

	// Close speech menu on level change
	event NotifyLevelChange()
	{
		Global.NotifyLevelChange();
		GotoState('');
	}
}

defaultproperties
{
	SMOriginX=0.01
	SMOriginY=0.3
	SMMargin=0.015

	SMStateName(0) = "Speech Menu"
	SMStateName(1) = "Acknowledge"
	SMStateName(2) = "Friendly Fire"
	SMStateName(3) = "Order"
	SMStateName(4) = "Other"
	SMStateName(5) = "Taunt"
	SMStateName(6) = "Taunt Anim"
	SMStateName(7) = "Player Select"

	SMAllString="[ALL]"
	SMMoreString="[MORE]"

	SMOpenSound=sound'MenuSounds.SelectDshort'
	SMAcceptSound=sound'MenuSounds.SelectJ'
	SMDenySound=sound'MenuSounds.SelectK'
	ConsoleSoundVol=0.3

	MaxScrollbackSize=128

	AddedCurrentHead="Added Server:"
	AddedCurrentTail="To Favorites!"
}
