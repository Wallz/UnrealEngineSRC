/*=============================================================================
	UnDebuggerCore.cpp: Debugger Core Logic
	Copyright 1997-2001 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Lucas Alonso, Demiurge Studios
=============================================================================*/

#include "DebuggerLaunchPrivate.h"

/*-----------------------------------------------------------------------------
	UDebuggerCore.
-----------------------------------------------------------------------------*/

UDebuggerCore::UDebuggerCore()
{
	IsDebugging = FALSE;
	IsClosing = FALSE;
	CurrentState = NULL;
	PendingState = NULL;
	BreakpointManager = NULL;
	CallStack = NULL;
	Interface = NULL;
	AccessedNone = FALSE;
	BreakOnNone = FALSE;
	BreakASAP = FALSE;

	ChangeState(new DSIdleState(this));

	ResultBuffer = new BYTE[10000];
}



UDebuggerCore::~UDebuggerCore()
{

	if(CurrentState)
		delete CurrentState;

	if(PendingState)
		delete PendingState;

	if(BreakpointManager)
		delete BreakpointManager;

	if(CallStack)
		delete CallStack;

	if(Interface)
		delete Interface;

	// Clean up our buffer
	delete [] ResultBuffer;
}



void UDebuggerCore::AddWatch(const TCHAR* watchName)
{
	new(watchNames)FString(watchName);
}


void UDebuggerCore::RemoveWatch(const TCHAR* watchName)
{
	watchNames.RemoveItem(watchName);
}


void UDebuggerCore::ClearWatches()
{	
	watchNames.Empty();
}



void UDebuggerCore::Initialize()
{
	BreakpointManager = new FBreakpointManager( this );
	
	CallStack = new FCallStack( this );
	
	GLogHook = new FDebuggerLogHook;
	
	Interface = new DelphiInterface();
	if ( !Interface->Initialize( this ) )
		appErrorf( TEXT("Could not initialize the debugger interface!") );

	debugf( NAME_Init, TEXT("UnrealScript Debugger Core Initialized.") );


	// Load saved breakpoints from UDebugger.ini

	int breakpointLine;
	int i = 0;

	FString breakpointName(TEXT("Breakpoint"));
	breakpointName += appItoa(i);

	// while the ini file has more lines that read BreakpointXline=some_line_number
	while(GConfig->GetInt(TEXT("DEBUGGER.BREAKPOINTS"), *(breakpointName + TEXT("line")), breakpointLine, TEXT("UDebugger.ini")))
	{
		// grab the class
		FString breakpointClass;
		// fails if there is a breakpointXline=1234 and no breakpointXclass=somepackage.someclass
		check(GConfig->GetString(TEXT("DEBUGGER.BREAKPOINTS"), *(breakpointName + TEXT("class")), breakpointClass, TEXT("UDebugger.ini")));

		// set via the interface...ugly. 
		Interface->SetBreakpoint(*breakpointClass, breakpointLine);

		i++;
		breakpointName = TEXT("Breakpoint");
		breakpointName += appItoa(i);
	}

	// Load saved watches
	i = 0;
	FString watchINIVar(TEXT("watch"));
	watchINIVar += appItoa(i);

	FString watchName;

	// while the ini file has more lines that read BreakpointXline=some_line_number
	while(GConfig->GetString(TEXT("DEBUGGER.WATCHES"), *watchINIVar, watchName, TEXT("UDebugger.ini")))
	{
		AddWatch(*watchName);

		i++;
		watchINIVar = TEXT("watch");
		watchINIVar += appItoa(i);
	}

}



void UDebuggerCore::Close()
{
	debugf( NAME_Init, TEXT("UnrealScript Debugger Core Exit.") );
	
	for(int i=0; i<watchNames.Num(); i++)
	{
		FString watchVarName(TEXT("watch"));
		watchVarName += appItoa(i);
		GConfig->SetString(TEXT("DEBUGGER.WATCHES"), *watchVarName, *watchNames(i), TEXT("UDebugger.ini"));
	}

	delete BreakpointManager;
	BreakpointManager = NULL;
	
	delete CallStack;
	CallStack = NULL;
	
	Interface->Close();
	delete Interface;
	Interface = NULL;

	delete GLogHook;
	GLogHook = NULL;
}


// Extract the value of a given property at a given address
void UDebuggerCore::PropertyToWatch( UProperty* Prop, void* PropAddr, INT CurrentDepth, int watch, int watchParent )
{
	UObject* ObjResult = NULL;
	// This SHOULD be sufficient.
	appMemzero( ResultBuffer, 10000 );

	FString VarName;
	FString VarValue;

	if ( Prop->IsA( UStructProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
			VarName = FString::Printf(TEXT("%s %s"), Cast<UStructProperty>(Prop)->Struct->GetName(), Prop->GetName());
			VarValue = FString::Printf(TEXT("\"%s\""), Cast<UStructProperty>(Prop)->GetName());
	}
	else if ( Prop->IsA( UIntProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		INT IntResult = *(INT*)ResultBuffer;
		VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
		VarValue = FString::Printf(TEXT("\"%i\""), IntResult );
	}
	else if ( Prop->IsA( UByteProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		// Give it to printf as an integer.
		INT ByteResult = *(BYTE*)ResultBuffer;
		VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
		VarValue = FString::Printf(TEXT("\"%i\""), ByteResult );
	}
	else if ( Prop->IsA( UFloatProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		FLOAT FloatResult = *(FLOAT*)ResultBuffer;
		VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
		VarValue = FString::Printf(TEXT("\"%d\""), FloatResult );
	}
	else if ( Prop->IsA( UClassProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		UClass* ClassResult = *(UClass**)ResultBuffer;
		if ( ClassResult != NULL )
		{
			VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
			VarValue = FString::Printf(TEXT("\"%s\""), ClassResult );
		}
		else
		{
			VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
			VarValue = TEXT("\"None\"");
		}
	}
	else if ( Prop->IsA( UObjectProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		ObjResult = *(UObject**)ResultBuffer;
		if ( ObjResult != NULL )
		{
			/*if(CurrentDepth < 1)
			{				
				Result = TEXT("(");
				// Recurse every property in this struct, and copy it's value into Result;
				for( TFieldIterator<UProperty> It( ObjResult->GetClass() ); It; ++It )
				{
					static FString oldResult;
					oldResult = Result;
					Result += FString::Printf(TEXT("%s %s=%s, "), It->GetClass()->GetName(), It->GetName(), *(GetPropText( *It, (BYTE*)ObjResult + It->Offset, CurrentDepth + 1 )) );
				}
				Result = Result.Left(Result.Len()-2);
				Result += TEXT(")");
			}
			else*/
			{
				VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
				VarValue = FString::Printf(TEXT("\"%s\""), ObjResult->GetName());
			}
		}
		else
		{
			VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
			VarValue = TEXT("\"None\"");
		}
	}
	else if ( Prop->IsA( UNameProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		FName NameResult = *(FName*)ResultBuffer;
		VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
		VarValue = *NameResult;
	}
	else if ( Prop->IsA( UStrProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		FString StringResult = *(FString*)ResultBuffer;
		VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
		VarValue = StringResult;
	}
	else if ( Prop->IsA( UBoolProperty::StaticClass() ) )
	{
		Prop->CopyCompleteValue( ResultBuffer, PropAddr );
		INT BoolResult = *(BITFIELD*)ResultBuffer;
		VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
		VarValue = BoolResult ? TEXT("\"True\"") : TEXT("\"False\"");
	}
	else
	{
		VarName = FString::Printf(TEXT("%s %s"), Prop->GetClass()->GetName(), Prop->GetName());
		VarValue = TEXT("No Handler");
	}


	int ID = Interface->AddAWatch(watch, watchParent, *VarName, *VarValue);

	if(Prop->IsA( UStructProperty::StaticClass() ) && CurrentDepth < 2)
	{
		// Recurse every property in this struct, and copy it's value into Result;
		for( TFieldIterator<UProperty> It(Cast<UStructProperty>(Prop)->Struct); It; ++It )
		{
			// Special case for nested stuff, don't leave it up to VarName/VarValue since we need to recurse
			PropertyToWatch(*It, (BYTE*)PropAddr + It->Offset, CurrentDepth + 1, watch, ID);
		}
	}

	if(Prop->IsA(UObjectProperty::StaticClass()) && CurrentDepth < 1 && ObjResult)
	{				
		for( TFieldIterator<UProperty> It( ObjResult->GetClass() ); It; ++It )
		{
			//GetPropText( *It, (BYTE*)ObjResult + It->Offset, CurrentDepth + 1 );
			PropertyToWatch(*It, (BYTE*)ObjResult + It->Offset, CurrentDepth + 1, watch, ID);
		}
	}
}


// Insert a given property into the watch window. 
void UDebuggerCore::PropertyToWatch( UProperty* Prop, UObject* Obj, FFrame* Stack, BOOL bIsLocal, int watch)
{
	// format:
	// <TYPE> <NAME>=<DATA>
	// object A=(String Name="\"Blah\"", Vector Location=(float X="1",float Y="2",float Z="4"))
	// Each branch is encased by () then each var comes as "TYPE NAME = data"
	
	void* PropAddr = NULL;

	if ( bIsLocal )
		PropAddr = Stack->Locals + Prop->Offset;
	else
		PropAddr = (BYTE*)Obj + Prop->Offset;

	PropertyToWatch(Prop, PropAddr, 0, watch, -1);
}


// Update the Watch ListView with all the current variables the Stack/Object contain.
void UDebuggerCore::RefreshWatch( FStackNode* CNode )
{
	TArray<INT> foundWatchNamesIndicies;


	if ( CNode == NULL )
		return;
	UFunction* Function = Cast<UFunction>(CNode->GetFrame()->Node);
	UProperty*	ReturnValue = FindField<UProperty>(Function,TEXT("ReturnValue"));
	UProperty* Parm;
	int Index = 0;

	Interface->LockWatch(Interface->LOCAL_WATCH);
	Interface->LockWatch(Interface->GLOBAL_WATCH);
	Interface->LockWatch(Interface->WATCH_WATCH);
	Interface->ClearAWatch(Interface->LOCAL_WATCH);
	Interface->ClearAWatch(Interface->GLOBAL_WATCH);
	Interface->ClearAWatch(Interface->WATCH_WATCH);

	// Setup the local variable watch
	if ( Function != NULL )
	{
		for(Parm = Function->PropertyLink,Index = 0;Parm && Index < Function->NumParms;Parm = Parm->PropertyLinkNext,Index++)
		{
			if(Parm == ReturnValue)
			{
				Parm = Parm->PropertyLinkNext;
				break;
			}
			// new(StackNames)FString( GetCallStack()->GetNode(i)->GetFrame()->Node->GetFullName() );
			PropertyToWatch( Parm, CNode->GetObject(), CNode->GetFrame(), TRUE, Interface->LOCAL_WATCH );

			
			// check to see if this is one we want to watch
			int index;
			if(watchNames.FindItem(Parm->GetName(), index))
			{
				PropertyToWatch( Parm, CNode->GetObject(), CNode->GetFrame(), TRUE, Interface->WATCH_WATCH );
				foundWatchNamesIndicies.AddItem(index);
			}
		}

		if(Function->Script.Num())
		{
			for(;Parm;Parm = Parm->PropertyLinkNext)
			{
				PropertyToWatch( Parm, CNode->GetObject(), CNode->GetFrame(), TRUE, Interface->LOCAL_WATCH );

				// check to see if this is one we want to watch
				int index;
				if(watchNames.FindItem(Parm->GetName(), index))
				{
					PropertyToWatch( Parm, CNode->GetObject(), CNode->GetFrame(), TRUE, Interface->WATCH_WATCH );
					foundWatchNamesIndicies.AddItem(index);
				}
			}
		}
	}


	// Setup the global vars watch
	for(TFieldIterator<UProperty> PropertyIt(CNode->GetObject()->GetClass());PropertyIt;++PropertyIt)
	{
		PropertyToWatch( *PropertyIt, CNode->GetObject(), CNode->GetFrame(), FALSE, Interface->GLOBAL_WATCH );

		
		// check to see if this is one we want to watch
		int index;
		if(watchNames.FindItem((*PropertyIt)->GetName(), index))
		{
			PropertyToWatch((*PropertyIt), CNode->GetObject(), CNode->GetFrame(), FALSE, Interface->WATCH_WATCH );
			foundWatchNamesIndicies.AddItem(index);
		}
	}

	// for those watchNames we didn't find, stick them into the watchVars with a message for the user
	// For each name we're watching
	for(int i = 0; i < watchNames.Num(); i++)
	{
		int index;
		// if this index is not in the found indicies
		if(!foundWatchNamesIndicies.FindItem(i, index))
		{
			FString message(TEXT("WatchVar "));
			message += watchNames(i);
			// insert a dummy message
			// <TYPE> <NAME>=<DATA>
			Interface->AddAWatch(Interface->WATCH_WATCH, -1, *message, TEXT("Variable Not Found"));
		}
	}
	Interface->UnlockWatch(Interface->LOCAL_WATCH);
	Interface->UnlockWatch(Interface->GLOBAL_WATCH);
	Interface->UnlockWatch(Interface->WATCH_WATCH);
}

void UDebuggerCore::NotifyAccessedNone()
{
	AccessedNone=TRUE;
}

void UDebuggerCore::Break()
{
	BreakASAP = true;
}

void UDebuggerCore::SetBreakOnNone(UBOOL inBreakOnNone)
{
	BreakOnNone = inBreakOnNone;
	AccessedNone = FALSE;
}

// Main debugger entry point
void UDebuggerCore::DebugInfo( UObject* Debugee, FFrame* Stack, FString InfoType, int LineNumber, int InputPos )
{
	// Wierd Devastation fix
	if ( Stack->Node->IsA( UClass::StaticClass() ) )
		return;

	// Process any waiting states
	ProcessPendingState();
	
	if ( CallStack && BreakpointManager && CurrentState )
	{
		if ( !GIsRequestingExit && !IsClosing )
		{
			CallStack->UpdateStack( Debugee, Stack, LineNumber, InputPos, *InfoType );
			
			// Ignore these special breakpoints
			if (   InfoType == TEXT("OPEREFP") 
				|| InfoType == TEXT("PREVSTACK") 
				|| InfoType == TEXT("NEWSTACK")
				|| InfoType == TEXT("LATENTPREVSTACK") 
				|| InfoType == TEXT("LATENTNEWSTACK") )
				return;
			
			// If there's something on the stack (always should be true)
			if ( CallStack->GetStackDepth() > 0 )
			{
				// Break?
				FString className = GetDebugClass( Stack )->GetFullName();
				// Get rid of "CLASS "
				className = className.Right(className.Len() - 6);
				if ( BreakpointManager->QueryBreakpoint(className , LineNumber) || (AccessedNone && BreakOnNone) || BreakASAP)
				{
					IsDebugging = 1;
					debugf(TEXT("Breaking at line %d class %s"), LineNumber, className);
					// UpdateInterface();
					// Halt execution, and wait for user input
					ChangeState( new DSWaitForInput(this) );
					ProcessPendingState();
					AccessedNone = FALSE;
					BreakASAP = FALSE;
				}
				
				// Let the state work its magic, chances are we won't return here for a while...
				CurrentState->UpdateStackInfo( CallStack->GetTopNode() );
				CurrentState->Process();
			}
		}
	}
}



// Process any pending debugger states
void UDebuggerCore::ProcessPendingState()
{
	if ( PendingState != NULL )
	{
		if ( CurrentState != NULL )
		{
			delete CurrentState;
			CurrentState = NULL;
		}
		CurrentState = PendingState;
		PendingState = NULL;
	}
}



// Set a pending state change
void UDebuggerCore::ChangeState( FDebuggerState* NewState )
{
	if(PendingState)
	{
		delete PendingState;
	}
	else
	{
		PendingState = NewState;
	}
}



// Update the interface
void UDebuggerCore::UpdateInterface()
{
	if ( IsDebugging )
	{
		FStackNode* TopNode = GetCallStack()->GetTopNode();
		
		// Get package name
		FString FullName = TopNode->GetClass()->GetFullName();
		int CutPos = FullName.InStr( TEXT(".") );
		
		// Extract the package name and chop off the 'Class' thing.
		FString PackageName = FullName.Left( CutPos );
		PackageName = PackageName.Right( PackageName.Len() - 6 );

		Interface->Update(	TopNode->GetClass()->GetName(), 
							*PackageName,
							TopNode->GetLine(),
							TopNode->GetInfo(),
							TopNode->GetObject()->GetName());
		RefreshWatch( TopNode );

		TArray<FString> StackNames;
		for(int i=0;i<GetCallStack()->GetStackDepth();i++) 
		{
			new(StackNames)FString( GetCallStack()->GetNode(i)->GetFrame()->Node->GetFullName() );
			//StackNames.AddItem(  );
		}
		Interface->UpdateCallStack( StackNames );

	}
	//debugf(TEXT("UpdateInterface commented out"));
}


void UDebuggerCore::LoadEditPackages()
{
	TArray<FString> EditPackages;
	TMultiMap<FString,FString>* Sec = GConfig->GetSectionPrivate( TEXT("Editor.EditorEngine"), 0, 1, USE_INI );
	Sec->MultiFind( FString(TEXT("EditPackages")), EditPackages );
	TObjectIterator<UEngine> EngineIt;
	if ( EngineIt )
		for( INT i=0; i<EditPackages.Num(); i++ )
		{
			if(appStrcmp(*EditPackages(i), TEXT("UnrealEd"))) // don't load the UnrealEd 
			{
				if( !EngineIt->LoadPackage( NULL, *EditPackages(i), LOAD_NoWarn ) )
					appErrorf( TEXT("Can't find edit package '%s'"), *EditPackages(i) );
			}
		}
	Interface->UpdateClassTree();
}


UClass* UDebuggerCore::GetDebugClass( FFrame* Stack )
{
	UClass* RClass;
	
	// Function?
	RClass = Cast<UClass>( Stack->Node->GetOuter() ); 
	
	// Nope, a state, we need to go one level higher to get the class
	if ( RClass == NULL )
		RClass = Cast<UClass>( Stack->Node->GetOuter()->GetOuter() );

	if ( RClass == NULL )
		RClass = Cast<UClass>( Stack->Node );
	
	// Make sure it's a real class

	check(RClass!=NULL);

	return RClass;
}



FDebuggerState* UDebuggerCore::GetCurrentState()
{
	return CurrentState;
}



FBreakpointManager* UDebuggerCore::GetBreakpointManager()
{
	return BreakpointManager;
}



FCallStack* UDebuggerCore::GetCallStack()
{
	return CallStack;
}



UDebuggerInterface* UDebuggerCore::GetInterface()
{
	return Interface;
}



void FDebuggerLogHook::Serialize( const TCHAR* V, EName Event )
{
	if(((UDebuggerCore*)GDebugger)->GetInterface())
	{
		((UDebuggerCore*)GDebugger)->GetInterface()->AddToLog( *FString::Printf(TEXT("%s: %s"), FName::SafeString(Event), V) );
	}
}



/*-----------------------------------------------------------------------------
	FCallStack.
-----------------------------------------------------------------------------*/

FCallStack::FCallStack( UDebuggerCore* InParent )
{
	Parent = InParent;
	StackDepth = 0;
}



FCallStack::~FCallStack()
{
	Parent = NULL;
	StackDepth = 0;
}



// Update the call stack, adding new FStackNodes if necessary
// Take into account latent state stack anomalies...
void FCallStack::UpdateStack( UObject* Debugee, FFrame* FStack, int LineNumber, int InputPos, const TCHAR* AdditionalInfo )
{
	// Check if stack change is due to a latent function in a state (meaning thread of execution 
/*	if ( !appStricmp(AdditionalInfo, TEXT("LATENTPREVSTACK")) )
	{
		delete Stack.Pop();
		StackDepth--;
		check(StackDepth > 0);
//		Parent->UpdateInterface();		
	}
	// Normal change... pop the top stack off the call stack
	else*/ if ( !appStricmp(AdditionalInfo, TEXT("PREVSTACK")) )
	{
		delete Stack.Pop();
		StackDepth--;
		if ( StackDepth == 0 )
		{
			Parent->ChangeState( new DSIdleState(Parent) );
			Parent->IsDebugging = FALSE;
		}
//		Parent->UpdateInterface();
	}
	// Add new stack
	else if ( !appStricmp(AdditionalInfo, TEXT("NEWSTACK")) /*|| !appStricmp(AdditionalInfo, TEXT("LATENTNEWSTACK")) */)
	{
		if(!appStricmp(AdditionalInfo, TEXT("LATENTNEWSTACK")))
		{
			debugf(TEXT("BLAH"));
		}	
		INT VerifyDupe = 1;
		for(int i=0;i<StackDepth;i++)
			if ( GetNode(i)->GetFrame() == FStack )
				VerifyDupe = 0;

		if ( VerifyDupe )
		{
			FStackNode* NewNode = new FStackNode( Debugee, FStack, Parent->GetDebugClass( FStack ) );
			NewNode->Update( LineNumber, InputPos, AdditionalInfo );
			Stack.AddItem( NewNode );
			StackDepth++;
		}
		
//		Parent->UpdateInterface();
	}
	// Stack has not changed. Update the current node with line number and current opcode type
	else if ( GetTopNode() && GetTopNode()->GetFrame() == FStack )
	{
		GetTopNode()->Update( LineNumber, InputPos, AdditionalInfo );		
	}

//	CurrentLineNumber = LineNumber;
}



FStackNode* FCallStack::GetTopNode() const
{
	return GetNode( StackDepth-1 );
}



FStackNode* FCallStack::GetPreviousNode() const
{
	return GetNode( StackDepth-2 );
}



FStackNode* FCallStack::GetNode( INT i ) const
{
	return Stack(i);
}



INT FCallStack::GetStackDepth() const
{
	return StackDepth;
}

/*-----------------------------------------------------------------------------
	FStackNode Implementation
-----------------------------------------------------------------------------*/

FStackNode::FStackNode( UObject* Debugee, FFrame* Stack, UClass* InClass )
{
	Object = Debugee;
	StackNode = Stack;
	Class = InClass;
}



void FStackNode::Update( INT LineNumber, INT InputPos, const TCHAR* InInfo )
{
	Line = LineNumber;
	Pos = InputPos;
	Info = InInfo;
}



UObject* FStackNode::GetObject()
{
	return Object;
}



UClass* FStackNode::GetClass()
{
	return Class;
}



FFrame* FStackNode::GetFrame()
{
	return StackNode;
}



INT FStackNode::GetLine()
{
	return Line;
}



INT FStackNode::GetPos()
{
	return Pos;
}



const TCHAR* FStackNode::GetInfo()
{
	return *Info;
}



/*-----------------------------------------------------------------------------
	Breakpoints
-----------------------------------------------------------------------------*/

FBreakpoint::FBreakpoint( const TCHAR* InClassName, INT InLine )
{
	ClassName = InClassName;
	Line = InLine;
	IsEnabled = true;
}


FBreakpointManager::FBreakpointManager( UDebuggerCore* InParent )
{
}



FBreakpointManager::~FBreakpointManager()
{

	// clear all the existing breakpoints
	GConfig->EmptySection( TEXT("DEBUGGER.BREAKPOINTS"), TEXT("UDebugger.ini") );

	// while the ini file has more lines that read BreakpointXline=some_line_number
	for(int i=0;i<Breakpoints.Num();i++)
	{
		FString breakpointName(TEXT("Breakpoint"));
		breakpointName += appItoa(i);
		GConfig->SetInt(TEXT("DEBUGGER.BREAKPOINTS"), *(breakpointName + TEXT("line")), Breakpoints(i).Line, TEXT("UDebugger.ini"));
		GConfig->SetString(TEXT("DEBUGGER.BREAKPOINTS"), *(breakpointName + TEXT("class")), *Breakpoints(i).ClassName, TEXT("UDebugger.ini"));
	}

	GConfig->Flush(false, TEXT("UDebugger.ini"));

}


UBOOL FBreakpointManager::QueryBreakpoint( const FString& sClassName, INT sLine )
{
	FString upper = sClassName.Caps();

	for(int i=0;i<Breakpoints.Num();i++)
	{
	/*	if( Breakpoints(i).ClassName == (upper) )
		{
			debugf(TEXT("Matched class"));

			if(Breakpoints(i).Line == sLine)
			{
				debugf(TEXT("matched line"));
				if(Breakpoints(i).Line == sLine)
				{
					debugf(TEXT("TOTAL MATCH"));
				}
			}
			else
			{
				debugf(TEXT("lines don't match %d != %d"),Breakpoints(i).Line, sLine);
			}
		}*/


		if ( Breakpoints(i).IsEnabled && Breakpoints(i).ClassName == (upper) && Breakpoints(i).Line == sLine )
			return TRUE;
	}
	
	return FALSE;
}


void FBreakpointManager::SetBreakpoint( const FString& sClassName, INT sLine )
{
	FString upper = sClassName.Caps();

	for(int i=0;i<Breakpoints.Num();i++)
	{
		if ( Breakpoints(i).ClassName == (upper) && Breakpoints(i).Line == sLine )
			return;
	}

	Breakpoints( Breakpoints.AddZeroed() ) = FBreakpoint( *upper, sLine );
}


void FBreakpointManager::RemoveBreakpoint( const FString& sClassName, INT sLine )
{
	FString upper = sClassName.Caps();

	for(int i=0;i<Breakpoints.Num();i++)
	{
		if ( Breakpoints(i).ClassName == (upper) && Breakpoints(i).Line == sLine )
			Breakpoints.Remove(i);
	}
}



void FBreakpointManager::Serialize( FArchive& Ar )
{
	// Make sure we're loading the right type of file
	FString Ident(TEXT("UDEBUGV1"));
	Ar << Ident;

	if ( Ident != TEXT("UDEBUGV1") )
	{
		GWarn->Logf(TEXT("Incorrect breakpoint file format!"));
		return;
	}

	if ( Ar.IsLoading() )
	{
		INT Num = 0;
		Ar << Num;
		Breakpoints.Empty();
		GLog->Logf(TEXT("Loading %i breakpoints."), Num );
		Breakpoints.Add( Num );
		for(int i=0;i<Num;i++)
		{
			INT VerifyNum = 0;
			Ar << VerifyNum;

			// File is bad... reset the breakpoints and return
			if ( i != VerifyNum )
			{
				GWarn->Logf(TEXT("Error, Expected %i, got %i."), i, VerifyNum );
				Breakpoints.Empty();
				return;
			}
			Ar << Breakpoints(i).Line;
			Ar << Breakpoints(i).ClassName;
			Ar << Breakpoints(i).IsEnabled;
		}
	}
	else
	{
		INT Num = Breakpoints.Num();
		Ar << Num;
		GLog->Logf(TEXT("Saving %i breakpoints."), Num );
		for(int i=0;i<Num;i++)
		{
			Ar << i;
			Ar << Breakpoints(i).Line;
			Ar << Breakpoints(i).ClassName;
			Ar << Breakpoints(i).IsEnabled;
		}
	}

	FString Term(TEXT("ENDV1"));
	Ar << Term;

	if ( Term != TEXT("ENDV1") )
	{
		GWarn->Logf(TEXT("Unexptected terminator."));
		Breakpoints.Empty();
		return;
	}

}
/*-----------------------------------------------------------------------------
	Debugger states
-----------------------------------------------------------------------------*/

void FDebuggerState::UpdateStackInfo( FStackNode* CNode ) 
{
	CurrentNode = CNode;
}


void FDebuggerState::HandleInput( EUserAction UserInput ) 
{

}


void DSWaitForInput::Process() 
{
//	FDebuggerState::Process();
	Debugger->IsDebugging = 1;
	Debugger->UpdateInterface();
	bContinue = FALSE;
	Debugger->GetInterface()->Show();
	PumpMessages();
}


void DSWaitForInput::HandleInput( EUserAction UserInput ) 
{
	FString CurrentInfo;

	switch ( UserInput )
	{
	case UA_RunToCursor:
		/*CHARRANGE sel;

		RichEdit_ExGetSel (Parent->Edit.hWnd, &sel);

		if ( sel.cpMax != sel.cpMin )
		{

		//appMsgf(0,TEXT("Invalid cursor position"));			
			return;
		}
		Parent->ChangeState( new DSRunToCursor( sel.cpMax, Parent->GetCallStack()->GetStackDepth() ) );*/
		Debugger->IsDebugging = 0;
		ContinueExecution();
		break;
	case UA_Exit:
		Debugger->IsDebugging = 0;
		GIsRequestingExit = 1;
		ContinueExecution();
		break;
	case UA_StepInto:
		Debugger->ChangeState( new DSStepInto( Debugger->GetCallStack()->GetStackDepth(), Debugger  ) );
		ContinueExecution();
		Debugger->IsDebugging = 1; // 
		break;
	case UA_StepOver:
	/*	if ( CurrentInfo != TEXT("RETURN") && CurrentInfo != TEXT("RETURNNOTHING") )
		{
			
			Debugger->ChangeState( new DSStepOver( CurrentObject,
												 CurrentClass,
												 CurrentStack, 
												 CurrentLine, 
												 CurrentPos, 
												 CurrentInfo,
												 Debugger->GetCallStack()->GetStackDepth(), Debugger ) );
			

		}*/
		debugf(TEXT("Warning: UA_StepOver currently unimplemented"));
		Debugger->IsDebugging = 1;
		ContinueExecution();
		break;
	case UA_StepOverStack:
		CurrentInfo = CurrentNode->GetInfo();
	    if ( CurrentInfo != TEXT("RETURN") && CurrentInfo != TEXT("RETURNNOTHING") )
		{
			Debugger->ChangeState( new DSStepOverStack( Debugger->GetCallStack()->GetTopNode()->GetLine(), Debugger->GetCallStack()->GetTopNode()->GetObject(), Debugger  ) );
		}
		ContinueExecution();
		Debugger->IsDebugging = 0; // set right before displaying the UI
		break;
	case UA_StepOut:
		Debugger->ChangeState( new DSStepOut( Debugger->GetCallStack()->GetStackDepth(), Debugger  ) );
		ContinueExecution();
		Debugger->IsDebugging = 1; // 
		break;
	case UA_Go:
		Debugger->ChangeState( new DSIdleState(Debugger) );
		ContinueExecution();
		Debugger->IsDebugging = 0;
		break;
	}
}


void DSWaitForInput::ContinueExecution() 
{
	bContinue = TRUE;
}


void DSWaitForInput::PumpMessages() 
{

	GIsRunning = false;
	//HACCEL hAccel = LoadAccelerators (hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while( bContinue == FALSE && Debugger->IsClosing == FALSE )
	{
		guard(MessagePump);
		MSG Msg;
		
		while( PeekMessageX(&Msg,NULL,0,0,PM_REMOVE) )
		{
			if( Msg.message == WM_QUIT )
			{
				GIsRequestingExit = 1;
				ContinueExecution();
			}
			guard(TranslateMessage);
			TranslateMessage( &Msg );
			unguardf(( TEXT("%08X %i"), (INT)Msg.hwnd, Msg.message ));

			guard(DispatchMessage);
			DispatchMessageX( &Msg );
			unguardf(( TEXT("%08X %i"), (INT)Msg.hwnd, Msg.message ));
		}
		unguard;
	}
	GIsRunning = true;

}



void DSWaitForCondition::Process() 
{
	if ( EvaluateCondition() )
	{
		// Condition was MET. We now delegate control to a
		// user-controlled state.
		Debugger->ChangeState( new DSWaitForInput(Debugger) );
		Debugger->DebugInfo( CurrentNode->GetObject(), 
			CurrentNode->GetFrame(), 
			CurrentNode->GetInfo(), 
			CurrentNode->GetLine(), 
			CurrentNode->GetPos() );
	}
	// Otherwise continue execution.
}


UBOOL DSWaitForCondition::EvaluateCondition()
{
	return FALSE;
}

DSWaitForCondition::DSWaitForCondition(UDebuggerCore* inDebugger) : FDebuggerState(inDebugger)
{

}


DSRunToCursor::DSRunToCursor( int InPos, int SDepth, UDebuggerCore* inDebugger ) : DSWaitForCondition(inDebugger) 
{

}



UBOOL DSRunToCursor::EvaluateCondition()
{
	return TRUE;
}



DSStepOut::DSStepOut( INT SDepth, UDebuggerCore* inDebugger ) 
	: DSWaitForCondition(inDebugger), EvalDepth(SDepth)
{

}


DSStepInto::DSStepInto( INT SDepth, UDebuggerCore* inDebugger ) 
	: DSWaitForCondition(inDebugger), EvalDepth(SDepth)
{

}



UBOOL DSStepOut::EvaluateCondition()
{
	if ( Debugger->GetCallStack()->GetStackDepth() < EvalDepth )
		return TRUE;

	return FALSE;
}



UBOOL DSStepInto::EvaluateCondition()
{
	if ( Debugger->GetCallStack()->GetStackDepth() >= EvalDepth )
		return TRUE;

	return FALSE;
}


DSStepOverStack::DSStepOverStack( INT inLineNumber, UObject* inObject, UDebuggerCore* inDebugger ) 
	: DSWaitForCondition(inDebugger), LineNumber(inLineNumber), EvalObject(inObject)
{
	EvalDepth = Debugger->GetCallStack()->GetStackDepth();
}



DSIdleState::DSIdleState(UDebuggerCore* inDebugger) : FDebuggerState(inDebugger)
{
	
}



DSWaitForInput::DSWaitForInput(UDebuggerCore* inDebugger) : FDebuggerState(inDebugger)
{

}



FDebuggerState::FDebuggerState(UDebuggerCore* inDebugger) : Debugger(inDebugger)
{

}


UBOOL DSStepOverStack::EvaluateCondition()
{
	if ( Debugger->GetCallStack()->GetTopNode()->GetLine() != LineNumber && 
		Debugger->GetCallStack()->GetTopNode()->GetObject() == EvalObject &&
		Debugger->GetCallStack()->GetStackDepth() == EvalDepth)
	{
		return TRUE;
	}
	return FALSE;
}


