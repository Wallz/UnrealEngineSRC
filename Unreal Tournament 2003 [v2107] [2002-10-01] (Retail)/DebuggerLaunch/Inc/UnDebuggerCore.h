/*=============================================================================
	UnDebuggerCore.h: Debugger Core Logic
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Lucas Alonso, Demiurge Studios
=============================================================================*/

#define USE_INI TEXT("UT2003.ini")
#define USE_NAME TEXT("UT2003")


/*-----------------------------------------------------------------------------
	Forward Declarations.
-----------------------------------------------------------------------------*/

class	UDebuggerCore;
class	UDebuggerInterface;
class	FBreakpoint;
class		FInstanceBreakpoint;
class		FExecutionBreakpoint;
class	FBreakpointManager;
class	FStackNode;
class	FCallStack;
class	FDebuggerState;
class		DSWaitForInput;
class		DSWaitForCondition;
class			DSRunToCursor;
class			DSStepOut;
class			DSStepOverStack;
class			DSStepIntoStack;
class		DSIdleState;

enum    EUserAction;
enum    EBreakType;

/*-----------------------------------------------------------------------------
	UDebuggerCore.
-----------------------------------------------------------------------------*/

class UDebuggerCore : public UDebugger
{
public:
	UDebuggerCore();
	~UDebuggerCore();

	// Main Debugger Entry point
	void DebugInfo( UObject* Debugee, FFrame* Stack, FString InfoType, int LineNumber, int InputPos );
	virtual void NotifyAccessedNone();
	
	//
	void Initialize();
	void Close();
	void ChangeState( FDebuggerState* NewState );
	void ProcessPendingState();
	void UpdateInterface();
	void LoadBreakpoints();

	void LoadEditPackages();

	// Helpers
	UClass* GetDebugClass( FFrame* Stack );	// Derive the actual class associated with this stack.
	
	// Accessors
	FDebuggerState* GetCurrentState();
	FBreakpointManager* GetBreakpointManager();
	FCallStack* GetCallStack();
	UDebuggerInterface* GetInterface();

	void AddWatch(const TCHAR* watchName);
	void RemoveWatch(const TCHAR* watchName);
	void ClearWatches();

	// Tells the debugger if it should be breaking on None
	void SetBreakOnNone(UBOOL inBreakOnNone);

	// Break immediately
	void Break();

public:
	UBOOL IsDebugging;
	UBOOL IsClosing;	// True when the user has closed the debugger via the UI...
	UBOOL AccessedNone;
	UBOOL BreakOnNone;

protected:
	FDebuggerState* CurrentState;
	FDebuggerState* PendingState;
	FBreakpointManager* BreakpointManager;
	FCallStack* CallStack;
	UDebuggerInterface* Interface;

	void SetDebuggerLine( FStackNode* CNode );

private:

	UBOOL BreakASAP;

	TArray<FString> watchNames;
	
	// Insert a given property into the watch window.
	void PropertyToWatch( UProperty* Prop, UObject* Obj, FFrame* Stack, BOOL bIsLocal, int watch);
	void PropertyToWatch(UProperty* Prop, void* PropAddr, INT CurrentDepth, int watch, int watchParent  );
	// Update the Watch ListView with all the current variables the Stack/Object contain.
	void RefreshWatch(FStackNode* CNode );

	BYTE* ResultBuffer;
};


class FDebuggerLogHook : public FOutputDevice
{
	void Serialize(	const TCHAR* V, EName Event );
};

/*-----------------------------------------------------------------------------
	FBreakpointManager.
-----------------------------------------------------------------------------*/


class FBreakpoint
{
public:
	FBreakpoint( const TCHAR* InClassName, INT InLine );
	UBOOL IsEnabled;
	FString ClassName;
	INT Line;
	FBreakpointManager* BreakpointManager;
};



class FBreakpointManager
{
public:
	FBreakpointManager( UDebuggerCore* InParent );
	~FBreakpointManager();
	UBOOL QueryBreakpoint( const FString& ClassName, INT Line  );
	void SetBreakpoint( const FString& Class, INT Line );
	void RemoveBreakpoint( const FString& Class, INT Line );
	void Serialize( FArchive& Ar );
private:
	TArray<FBreakpoint> Breakpoints;
	UDebuggerCore* Debugger;
};



/*-----------------------------------------------------------------------------
	FStackNode.
-----------------------------------------------------------------------------*/

class FStackNode
{
public:
	FStackNode( UObject* Debugee, FFrame* Stack, UClass* InClass );
	void Update( INT LineNumber, INT InputPos, const TCHAR* Info );

	UObject* GetObject();
	UClass* GetClass();
	FFrame* GetFrame();
	INT GetLine();
	INT GetPos();
	const TCHAR* GetInfo();

private:
	UObject* Object;
	UClass* Class;
	FFrame* StackNode;
	INT Line;
	INT Pos;
	FString Info;
};



/*-----------------------------------------------------------------------------
	FCallStack.
-----------------------------------------------------------------------------*/

class FCallStack
{
public:
	FCallStack( UDebuggerCore* InParent );
	~FCallStack();
	void UpdateStack( UObject* Debugee, FFrame* FStack, int LineNumber, int InputPos, const TCHAR* AdditionalInfo );
	FStackNode* GetTopNode() const;
	FStackNode* GetPreviousNode() const;
	FStackNode* GetNode( INT i ) const;
	INT GetStackDepth() const;

private:
	TArray<FStackNode*> Stack;
	INT StackDepth;
	UDebuggerCore* Parent;
};



/*-----------------------------------------------------------------------------
	FDebuggerState.
-----------------------------------------------------------------------------*/

class FDebuggerState
{
public:

	FDebuggerState(UDebuggerCore* inDebugger);

	virtual void Process() = 0;
	virtual void UpdateStackInfo( FStackNode* CNode );
	virtual void HandleInput( EUserAction UserInput );

protected:
	UDebuggerCore* Debugger;
	FStackNode* CurrentNode;
};



class DSWaitForInput : public FDebuggerState
{
public:

	DSWaitForInput(UDebuggerCore* inDebugger);

	virtual void Process();
	virtual void HandleInput( EUserAction UserInput );
	virtual void ContinueExecution();
	virtual void PumpMessages();

protected:
	UBOOL bContinue;
};



class DSWaitForCondition : public FDebuggerState
{
public:
	DSWaitForCondition(UDebuggerCore* inDebugger);

	virtual void Process();
	virtual UBOOL EvaluateCondition();
};



class DSRunToCursor : public DSWaitForCondition
{
public:

	DSRunToCursor( int InPos, int SDepth, UDebuggerCore* inDebugger );
	UBOOL EvaluateCondition();
	INT ExpectedPos;
	INT EvalDepth;
};



class DSIdleState : public FDebuggerState
{
public:
	DSIdleState(UDebuggerCore* inDebugger);

	virtual void Process() {}
};



class DSStepOut : public DSWaitForCondition
{
public:
	DSStepOut( INT SDepth, UDebuggerCore* inDebugger);

protected:
	UBOOL EvaluateCondition();
	FFrame*  OldStack;
	INT EvalDepth;
};


class DSStepInto : public DSWaitForCondition
{
public:
	DSStepInto( INT SDepth , UDebuggerCore* inDebugger);
	
protected:
	UBOOL EvaluateCondition();
	FFrame*  OldStack;
	INT EvalDepth;
};


class DSStepOverStack : public DSWaitForCondition
{
public:
	DSStepOverStack( INT inLineNumber, UObject* inObject, UDebuggerCore* inDebugger );

protected:
	UBOOL EvaluateCondition();
	INT LineNumber;
	INT EvalDepth;
	UObject* EvalObject;
};
