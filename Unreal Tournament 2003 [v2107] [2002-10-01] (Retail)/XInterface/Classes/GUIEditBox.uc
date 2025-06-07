// ====================================================================
//  Class:  xInterface.GUIEditBox
//
//	GUIEditBox - The basic text edit control.  I've merged Normal
//  edit, restricted edit, numeric edit and password edit in to 1 control.
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class GUIEditBox extends GUIButton
		Native;

#exec OBJ LOAD FILE=InterfaceContent.utx

cpptext
{
		void Draw(UCanvas* Canvas);
}
		
var(Menu)		string		TextStr;			// Holds the current string
var(Menu)		string		AllowedCharSet;		// Only Allow these characters
var(Menu)		bool		bMaskText;			// Displays the text as a *
var(Menu)		bool		bIntOnly;			// Only Allow Interger Numeric entry
var(Menu)		bool		bFloatOnly;			// Only Allow Float Numeric entry
var(Menu)		bool		bIncludeSign;		// Do we need to allow a -/+ sign
var(Menu)		bool		bConvertSpaces;		// Do we want to convert Spaces to "_"		
var(Menu)		int			MaxWidth;			// Holds the maximum width (in chars) of the string - 0 = No Max
var(Menu)		eTextCase	TextCase;			// Controls forcing case, etc
var(Menu)		int			BorderOffsets[4];	// How far in from the edit is the edit area
var(Menu)		bool		bReadOnly;			// Can't actually edit this box
var int 	CaretPos;		// Where is the cursor within the string
var	int		FirstVis;		// Position of the first visible character;
var int 	LastSizeX;		// Used to detect resolution changes
var int		LastCaret,LastLength;	// Used to make things quick

var bool	bAllSelected;	 
var byte	LastKey;
var float	DelayTime;

function InitComponent(GUIController MyController, GUIComponent MyOwner)
{
	Super.InitComponent(MyController, MyOwner);
	
	OnKeyType = InternalOnKeyType;
	OnKeyEvent = InternalOnKeyEvent;
	
	if ( (bIntOnly) || (bFloatOnly) )
	{
		AllowedCharSet = "0123456789";
		if (bFloatOnly)
			AllowedCharSet=AllowedCharSet$".";

	}

	bAllSelected=true;
}

event SetText(string NewText)
{
	TextStr = NewText;
	CaretPos=len(TextStr);
	OnChange(self);

	bAllSelected=true;
}

function DeleteChar()
{
	if (CaretPos==len(TextStr))
		return;
	else if (CaretPos==Len(TextStr)-1)
	{
		TextStr = left(TextStr,CaretPos);
		CaretPos=len(TextStr);
	}
	else
		TextStr = left(TextStr,CaretPos)$Mid(TextStr,CaretPos+1);
		
	OnChange(Self);		
		
}

function bool InternalOnKeyType(out byte Key, optional string Unicode)
{
	local string temp,st;

	if (bReadOnly)
		return false;
	
	if(bAllSelected)
	{
		TextStr="";
		CaretPos=0;
		bAllSelected=false;
		OnChange(Self);
	}

	if (key==8) // Process Backspace	
	{
		if (CaretPos>0)
		{
			CaretPos--;
			DeleteChar();
		}
		
		return true;
	}
	
//	if (UniCode!="")
//		st = UniCode;
//	else
		st = chr(Key); 

	// Handle cut/paste/copy keys
	if (key<32)
		return true;
	/*  UNCOMMENT THIS BLOCK AND COMMENT OUT return true ABOVE TO ACTIVATE COPY/CUT/PASTE
	{
		if (!Controller.CtrlPressed)
			return true;  // old code captured any keys under 32

		switch (key) 
		{
		case 3:   // ctrl-c, copy to console
			PlayerOwner().CopyToClipboard(TextStr);
			bAllSelected=true;
			break;
		case 22: // ctrl-v, paste at position
			if ( (TextStr=="") || ( CaretPos==len(TextStr) ) )	// At the end of the string, just add
			{
				if (bAllSelected)
					TextStr="";
				TextStr = ConvertIllegal(TextStr$PlayerOwner().PasteFromClipboard());
				CaretPos=len(TextStr);
			}
			else
			{
				// We are somewhere inside the string, insert.
				temp    = ConvertIllegal(left(TextStr,CaretPos)$PlayerOwner().PasteFromClipboard()$Mid(TextStr,CaretPos));
				TextStr = temp;	
			}
			break;
		case 24:  // ctrl-x, clear and copy 
			PlayerOwner().CopyToClipboard(TextStr);
			TextStr="";
			CaretPos=0;
		}
		OnChange(Self);
		return true;
	}
	*/

	if ( (AllowedCharSet=="") || ( (bIncludeSign) && ( (st=="-") || (st=="+") ) && (TextStr=="") ) || (InStr(AllowedCharSet,St)>=0) )
	{

		if ( (MaxWidth==0) || (Len(TextStr)<MaxWidth) )
		{	
			if ( (bConvertSpaces) && ((st==" ") || (st=="?") || (st=="\\")) )
				st = "_";
		
			if ( (TextStr=="") || ( CaretPos==len(TextStr) ) )	// At the end of the string, just add
			{
				TextStr = TextStr$st;
				CaretPos=len(TextStr);
			}
			else
			{
				// We are somewhere inside the string, insert it.
				
				temp    = left(TextStr,CaretPos)$st$Mid(TextStr,CaretPos);
				TextStr = temp;	
				CaretPos++;
			}
			
			OnChange(Self);
		}
	}
		
	return false;
}

function bool InternalOnKeyEvent(out byte Key, out byte State, float delta)
{
	if (bReadOnly)
		return false;

	if ( (Key==0x2E) && (State==1) ) // Delete key
	{
		if(bAllSelected)
		{
			TextStr="";
			CaretPos = 0;
			bAllSelected=false;
			OnChange(Self);
		}
		else
			DeleteChar();

		return true;
	}
	
	if ( (Key==0x64 || Key==0x25) && (State==1) )	// Left Arrow
	{
		if(bAllSelected)
		{
			CaretPos = 0;
			bAllSelected=false;
		}
		else if (CaretPos>0)
			CaretPos--;
			
		return true;
	}
	
	if ( (Key==0x66 || Key==0x27) && (State==1) ) // Right Arrow
	{
		if(bAllSelected)
		{
			CaretPos = len(TextStr);
			bAllSelected=false;
		}
		else if ( CaretPos<Len(TextStr) )
			CaretPos++;
			
		return true;
	}
	
	if ( (Key==0x24 || Key==0x67) && (State==1) ) // Home
	{
		CaretPos=0;
		bAllSelected=false;
		return true;
	}
	
	if ( (Key==0x23 || Key==0x61) && (State==1) ) // End
	{
		CaretPos=len(TextStr);
		bAllSelected=false;
		return true;
	}
	
	return false;
}

// converts space-characters and chars not in the allowed char array
// ensure string stays within max bounds
function string ConvertIllegal(string inputstr) 
{
	local int i, max;
	local string retval;
	local string c;

	i = 0;
	max = Len(inputstr);
	while ( i < max )
	{
		c = Mid(inputstr,i,1);
		if ( AllowedCharSet != "" && InStr(AllowedCharSet,c) < 0 ) 
		{
			c = "";
		}
		if ( bConvertSpaces &&
			((c == " ") || (c =="?") || (c=="\\") ))
		{
			c = "_";
		} 
		retval = retval $ c;
		i++;
	}

	if (MaxWidth > 0)
		return Left(retval,MaxWidth);
	else 
		return retval;
}

function string LoadINI()
{
	local string s;

	s = Super.LoadINI();

	if (S!="")
		SetText(s);

	return s;
}

function SaveINI(string Value)
{
	Super.SaveINI(TextStr);
}

function string GetText()
{
	return TextStr;
}


defaultproperties
{
	StyleName="SquareButton"
	MaxWidth=0
	TextCase=TXTC_None
	LastCaret=-1
	LastLength=-1
	WinHeight=0.06
	bCaptureMouse=False
	OnClickSound=CS_Edit
}
