/*=============================================================================
	UnScript.h: UnrealScript execution engine.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Native functions.
-----------------------------------------------------------------------------*/

//
// Native function table.
//
typedef void (UObject::*Native)( FFrame& TheStack, RESULT_DECL );
extern CORE_API Native GNatives[];
BYTE CORE_API GRegisterNative( INT iNative, const Native& Func );

extern CORE_API Native GCasts[];
BYTE CORE_API GRegisterCast( INT CastCode, const Native& Func );


//
// Registering a native function.
//
#if _MSC_VER
	#define IMPLEMENT_FUNCTION(cls,num,func) \
		extern "C" DLL_EXPORT Native int##cls##func = (Native)&cls::func; \
		static BYTE cls##func##Temp = GRegisterNative( num, int##cls##func );
#else
	#define IMPLEMENT_FUNCTION(cls,num,func) \
		extern "C" DLL_EXPORT { Native int##cls##func = (Native)&cls::func; } \
		static BYTE cls##func##Temp = GRegisterNative( num, int##cls##func );
#endif

/*-----------------------------------------------------------------------------
	Macros.
-----------------------------------------------------------------------------*/

//
// Macros for grabbing parameters for native functions.
//
#define P_GET_UBOOL(var)              DWORD var=0;                         Stack.Step( Stack.Object, &var    );
#define P_GET_UBOOL_OPTX(var,def)     DWORD var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_STRUCT(typ,var)         typ   var;                           Stack.Step( Stack.Object, &var    );
#define P_GET_STRUCT_OPTX(typ,var,def)typ   var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_STRUCT_REF(typ,var)     typ   var##T; GPropAddr=0;           Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); typ*     var = GPropAddr ? (typ    *)GPropAddr:&var##T;
#define P_GET_INT(var)                INT   var=0;                         Stack.Step( Stack.Object, &var    );
#define P_GET_INT_OPTX(var,def)       INT   var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_INT_REF(var)            INT   var##T=0; GPropAddr=0;         Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); INT*     var = GPropAddr ? (INT    *)GPropAddr:&var##T;
#define P_GET_FLOAT(var)              FLOAT var=0.f;                       Stack.Step( Stack.Object, &var    );
#define P_GET_FLOAT_OPTX(var,def)     FLOAT var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_FLOAT_REF(var)          FLOAT var##T=0.f; GPropAddr=0;       Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); FLOAT*   var = GPropAddr ? (FLOAT  *)GPropAddr:&var##T;
#define P_GET_BYTE(var)               BYTE  var=0;                         Stack.Step( Stack.Object, &var    );
#define P_GET_BYTE_OPTX(var,def)      BYTE  var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_BYTE_REF(var)           BYTE  var##T=0; GPropAddr=0;         Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); BYTE*    var = GPropAddr ? (BYTE   *)GPropAddr:&var##T;
#define P_GET_NAME(var)               FName var=NAME_None;                 Stack.Step( Stack.Object, &var    );
#define P_GET_NAME_OPTX(var,def)      FName var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_NAME_REF(var)           FName var##T=NAME_None; GPropAddr=0; Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); FName*   var = GPropAddr ? (FName  *)GPropAddr:&var##T;
#define P_GET_STR(var)                FString var;                         Stack.Step( Stack.Object, &var    );
#define P_GET_STR_OPTX(var,def)       FString var(def);                    Stack.Step( Stack.Object, &var    );
#define P_GET_STR_REF(var)            FString var##T; GPropAddr=0;         Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); FString* var = GPropAddr ? (FString*)GPropAddr:&var##T;
#define P_GET_OBJECT(cls,var)         cls*  var=NULL;                      Stack.Step( Stack.Object, &var    );
#define P_GET_OBJECT_OPTX(cls,var,def)cls*  var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_OBJECT_REF(cls,var)     cls*  var##T=NULL; GPropAddr=0;      Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); cls**    var = GPropAddr ? (cls   **)GPropAddr:&var##T;
#define P_GET_ARRAY_REF(typ,var)      typ   var##T[256]; GPropAddr=0;      Stack.Step( Stack.Object,  var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); typ*     var = GPropAddr ? (typ    *)GPropAddr: var##T;
#if __PSX2_EE__ || __GCN__
#define P_GET_SKIP_OFFSET(var)        _WORD var; {checkSlow(*Stack.Code==EX_Skip); Stack.Code++; appMemcpy(&var, Stack.Code, sizeof(_WORD)); Stack.Code+=2; }
#else
#define P_GET_SKIP_OFFSET(var)        _WORD var; {checkSlow(*Stack.Code==EX_Skip); Stack.Code++; var=*(_WORD*)Stack.Code; Stack.Code+=2; }
#endif
//DEBUGGER
#define P_FINISH                      Stack.Code++; if ( *Stack.Code == EX_DebugInfo ) Stack.Step( Stack.Object, NULL );

// gam ---
#define P_GET_TARRAY_REF(var,type)            TArray<type> var##T; GPropAddr=0;         Stack.Step( Stack.Object, &var##T ); if( GPropObject )GPropObject->NetDirty(GProperty); TArray<type>* var = GPropAddr ? (TArray<type>*)GPropAddr:&var##T;
// --- gam

//
// Convenience macros.
//
#define P_GET_VECTOR(var)           P_GET_STRUCT(FVector,var)
#define P_GET_VECTOR_OPTX(var,def)  P_GET_STRUCT_OPTX(FVector,var,def)
#define P_GET_VECTOR_REF(var)       P_GET_STRUCT_REF(FVector,var)
#define P_GET_ROTATOR(var)          P_GET_STRUCT(FRotator,var)
#define P_GET_ROTATOR_OPTX(var,def) P_GET_STRUCT_OPTX(FRotator,var,def)
#define P_GET_ROTATOR_REF(var)      P_GET_STRUCT_REF(FRotator,var)
#define P_GET_ACTOR(var)            P_GET_OBJECT(AActor,var)
#define P_GET_ACTOR_OPTX(var,def)   P_GET_OBJECT_OPTX(AActor,var,def)
#define P_GET_ACTOR_REF(var)        P_GET_OBJECT_REF(AActor,var)

//
// Iterator macros.
//
#define PRE_ITERATOR \
	INT wEndOffset = Stack.ReadWord(); \
	BYTE B=0, Buffer[MAX_CONST_SIZE]; \
	BYTE *StartCode = Stack.Code; \
	do {
#define POST_ITERATOR \
		while( (B=*Stack.Code)!=EX_IteratorPop && B!=EX_IteratorNext ) \
			Stack.Step( Stack.Object, Buffer ); \
		if( *Stack.Code++==EX_IteratorNext ) \
			Stack.Code = StartCode; \
	} while( B != EX_IteratorPop );

/*-----------------------------------------------------------------------------
	FFrame implementation.
-----------------------------------------------------------------------------*/

inline FFrame::FFrame( UObject* InObject )
:	Node		(InObject ? InObject->GetClass() : NULL)
,	Object		(InObject)
,	Code		(NULL)
,	Locals		(NULL)
{}
inline FFrame::FFrame( UObject* InObject, UStruct* InNode, INT CodeOffset, void* InLocals )
:	Node		(InNode)
,	Object		(InObject)
,	Code		(&InNode->Script(CodeOffset))
,	Locals		((BYTE*)InLocals)
{}
FORCEINLINE void FFrame::Step( UObject* Context, RESULT_DECL )
{
	guardSlow(FFrame::Step);
	INT B = *Code++;
	(Context->*GNatives[B])( *this, Result );
	unguardfSlow(( TEXT("(%s @ %s : %04X)"), Object->GetFullName(), Node->GetFullName(), Code - &Node->Script(0) ));
}
inline INT FFrame::ReadInt()
{
	INT Result;
#if __PSX2_EE__ || __GCN__
	appMemcpy(&Result, Code, sizeof(INT));
#else
	Result = *(INT*)Code;
#endif
	Code += sizeof(INT);
	return Result;
}
inline UObject* FFrame::ReadObject()
{
	UObject* Result;
#if __PSX2_EE__ || __GCN__
	appMemcpy(&Result, Code, sizeof(INT));
#else
	Result = *(UObject**)Code;
#endif
	Code += sizeof(INT);
	return Result;
}
inline FLOAT FFrame::ReadFloat()
{
	FLOAT Result;
#if __PSX2_EE__ || __GCN__
	appMemcpy(&Result, Code, sizeof(FLOAT));
#else
	Result = *(FLOAT*)Code;
#endif
	Code += sizeof(FLOAT);
	return Result;
}
inline INT FFrame::ReadWord()
{
	INT Result;
#if __PSX2_EE__ || __GCN__
	_WORD Temporary;
	appMemcpy(&Temporary, Code, sizeof(_WORD));
	Result = Temporary;
#else
	Result = *(_WORD*)Code;
#endif
	Code += sizeof(_WORD);
	return Result;
}
inline FName FFrame::ReadName()
{
	FName Result;
#if __PSX2_EE__ || __GCN__
	appMemcpy(&Result, Code, sizeof(FName));
#else
	Result = *(FName*)Code;
#endif
	Code += sizeof(FName);
	return Result;
}
CORE_API void GInitRunaway();

/*-----------------------------------------------------------------------------
	FStateFrame implementation.
-----------------------------------------------------------------------------*/

inline FStateFrame::FStateFrame( UObject* InObject )
:	FFrame		( InObject )
,	CurrentFrame( NULL )
,	StateNode	( InObject->GetClass() )
,	ProbeMask	( ~(QWORD)0 )
{}
inline const TCHAR* FStateFrame::Describe()
{
	return Node ? Node->GetFullName() : TEXT("None");
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

