/*=============================================================================
	UnDelphiInterface.cpp: Debugger Interface Interface
	Copyright 1997-2001 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Lucas Alonso, Demiurge Studios
=============================================================================*/

#include "DebuggerLaunchPrivate.h"




void DelphiCallback( const char* C )
{
	((DelphiInterface*)((UDebuggerCore*)GDebugger)->GetInterface())->Callback( C );
}



DelphiInterface::DelphiInterface()
{
	Debugger = NULL;
	ShowDllForm = NULL;
	AddClassToHierarchy = NULL;
	BuildHierarchy = NULL;
	ClearHierarchy = NULL;
	SetCallback = NULL;
}


int DelphiInterface::AddAWatch(int watch, int ParentIndex, const TCHAR* ObjectName, const TCHAR* Data)
{
	return DAddAWatch(watch, ParentIndex, TCHAR_TO_ANSI(ObjectName), TCHAR_TO_ANSI(Data));
}


void DelphiInterface::ClearAWatch(int watch)
{
	DClearAWatch(watch);
}


UBOOL DelphiInterface::Initialize( UDebuggerCore* DebuggerOwner )
{
	Debugger = DebuggerOwner;
	BindToDll();
	SetCallback( &DelphiCallback );
	ClearWatch( LOCAL_WATCH );
	ClearWatch( GLOBAL_WATCH );
	ClearWatch( WATCH_WATCH );
	Show();
	UpdateClassTree();
	return TRUE;
}


void DelphiInterface::Callback( const char* C )
{

	// uncomment to log all callback mesages from the UI
	debugf(TEXT("Callback:"));
	const TCHAR* command = ANSI_TO_TCHAR(C);
	debugf(command);

	if(ParseCommand(&command, TEXT("addbreakpoint")))
	{		
		TCHAR className[256];
		ParseToken(command, className, 256, 0);
		TCHAR lineNumString[10];
		ParseToken(command, lineNumString, 10, 0);
		SetBreakpoint(className, appAtoi(lineNumString));

	}
	else if(ParseCommand(&command, TEXT("removebreakpoint")))
	{
		TCHAR className[256];
		ParseToken(command, className, 256, 0);
		TCHAR lineNumString[10];
		ParseToken(command, lineNumString, 10, 0);
		RemoveBreakpoint(className, appAtoi(lineNumString));
	}
	else if(ParseCommand(&command, TEXT("addwatch")))
	{
		TCHAR watchName[256];
		ParseToken(command, watchName, 256, 0);
		Debugger->AddWatch(watchName);
	}
	else if(ParseCommand(&command, TEXT("removewatch")))
	{
		TCHAR watchName[256];
		ParseToken(command, watchName, 256, 0);
		Debugger->RemoveWatch(watchName);
	}
	else if(ParseCommand(&command, TEXT("clearwatch")))
	{
		Debugger->ClearWatches();
	}
	else if(ParseCommand(&command, TEXT("breakonnone")))
	{
		UBOOL breakValue;
		TCHAR breakValueString[5];
		ParseToken(command, breakValueString, 5, 0);
		breakValue = appAtoi(breakValueString);

		Debugger->SetBreakOnNone(breakValue);
	}
	else if(ParseCommand(&command, TEXT("break")))
	{
		Debugger->Break();
	}
	else if(Debugger->IsDebugging)
	{
		if(ParseCommand(&command, TEXT("go")))
		{
			// hide the debug window
			//DebugWindowState(0);
			Debugger->GetCurrentState()->HandleInput(UA_Go);
		}
		else if(ParseCommand(&command, TEXT("stepinto")))
		{
			Debugger->GetCurrentState()->HandleInput(UA_StepInto);
		}
		else if(ParseCommand(&command, TEXT("stepover")))
		{
			Debugger->GetCurrentState()->HandleInput(UA_StepOverStack);
		}
		else if(ParseCommand(&command, TEXT("stepoutof")))
		{
			Debugger->GetCurrentState()->HandleInput(UA_StepOut);
		}
		// Start the game running again on this message
		else if(ParseCommand(&command, TEXT("stopdebugging")))
		{
			Debugger->IsClosing = true;
		}
	}
}



void DelphiInterface::AddToLog( const TCHAR* Line )
{
	AddLineToLog(TCHAR_TO_ANSI(Line));
}



void DelphiInterface::Close()
{

}



void DelphiInterface::Show()
{
	ShowDllForm();
}



void DelphiInterface::Hide()
{

}



void DelphiInterface::Update( const TCHAR* ClassName, const TCHAR* PackageName, INT LineNumber, const TCHAR* OpcodeName, const TCHAR* objectName )
{
	FString openName(PackageName);
	openName += TEXT(".");
	openName += ClassName;

	debugf(TEXT("DelphiInterface::Update going to line %d in class %s"), LineNumber, *openName);
	EditorLoadClass(TCHAR_TO_ANSI(*openName));
	EditorGotoLine(LineNumber, 1);

	SetCurrentObjectName(TCHAR_TO_ANSI(objectName));

	//DebugWindowState(1);
}


void DelphiInterface::UpdateCallStack( TArray<FString>& StackNames )
{
	
	CallStackClear();
	for(int i = 0; i<StackNames.Num(); i++)
	{	
		CallStackAdd(TCHAR_TO_ANSI(*StackNames(i)));
	}
}



void DelphiInterface::SetBreakpoint( const TCHAR* ClassName, INT Line )
{
	FString upper(ClassName);
	upper = upper.Caps();
	Debugger->GetBreakpointManager()->SetBreakpoint( ClassName, Line );
	AddBreakpoint(TCHAR_TO_ANSI(*upper), Line);
}



void DelphiInterface::RemoveBreakpoint( const TCHAR* ClassName, INT Line )
{
	FString upper(ClassName);
	upper = upper.Caps();
	Debugger->GetBreakpointManager()->RemoveBreakpoint( ClassName, Line );
	DRemoveBreakpoint(TCHAR_TO_ANSI(*upper), Line);
}	



void DelphiInterface::UpdateClassTree()
{
	ClearHierarchy();
	AddClassToHierarchy( "Core..Object" );
	RecurseClassTree( UObject::StaticClass() );
	BuildHierarchy();
}



void DelphiInterface::RecurseClassTree( UClass* RClass )
{
	for( TObjectIterator<UClass> It; It; ++It )
		if ( Cast<UClass>(It->SuperField) == RClass )
		{
			// Get package name
			FString FullName = It->GetFullName();
			int CutPos = FullName.InStr( TEXT(".") );
			
			// Extract the package name and chop off the 'Class' thing.
			FString PackageName = FullName.Left( CutPos );
			PackageName = PackageName.Right( PackageName.Len() - 6 );
			AddClassToHierarchy( TCHAR_TO_ANSI(*FString::Printf( TEXT("%s.%s.%s"), *PackageName, RClass->GetName(), It->GetName() )) );

			RecurseClassTree( *It );
		}
}



void DelphiInterface::LockWatch(int watch)
{
	DLockList(watch);
}



void DelphiInterface::UnlockWatch(int watch)
{
	DUnlockList(watch);
}


void DelphiInterface::BindToDll()
{
	// Get pointers to the delphi functions
	ShowDllForm = (DelphiVoidVoid)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "ShowDllForm" );
	EditorCommand = (DelphiVoidChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "EditorCommand" );
	EditorLoadTextBuffer = (DelphiVoidCharChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "EditorLoadTextBuffer" );
	AddClassToHierarchy = (DelphiVoidChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "AddClassToHierarchy" );
	BuildHierarchy = (DelphiVoidVoid)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "BuildHierarchy" );
	ClearHierarchy = (DelphiVoidVoid)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "ClearHierarchy" );
	BuildHierarchy = (DelphiVoidVoid)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "BuildHierarchy" );
	ClearWatch = (DelphiVoidInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "ClearWatch" );
	AddWatch = (DelphiVoidIntChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "AddWatch" );
	SetCallback = (DelphiVoidVoidPtr)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll")), "SetCallback" );
	AddBreakpoint = (DelphiVoidCharInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "AddBreakpoint" );
	DRemoveBreakpoint = (DelphiVoidCharInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "RemoveBreakpoint" );
	EditorGotoLine = (DelphiVoidIntInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "EditorGotoLine" );
	AddLineToLog = (DelphiVoidChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll")), "AddLineToLog" );
	EditorLoadClass = (DelphiVoidChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "EditorLoadClass" );
	CallStackClear = (DelphiVoidVoid)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "CallStackClear" );
	CallStackAdd = (DelphiVoidChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "CallStackAdd" );
	DebugWindowState = (DelphiVoidInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "DebugWindowState" );
	DClearAWatch = (DelphiVoidInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "ClearAWatch" );
	DAddAWatch = (DelphiIntIntIntCharChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "AddAWatch" );
	DLockList = (DelphiVoidInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "LockList" );
	DUnlockList = (DelphiVoidInt)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "UnlockList" );
	SetCurrentObjectName = (DelphiVoidChar)GetProcAddress( (HMODULE)LoadLibrary( TEXT("dinterface.dll") ), "SetCurrentObjectName" );
}
