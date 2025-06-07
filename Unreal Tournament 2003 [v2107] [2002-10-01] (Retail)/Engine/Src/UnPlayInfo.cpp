/*=============================================================================
	UnPlayInfo.cpp: PlayInfo native functions.
	Copyright 2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Michel Comeau

	PlayInfo is a class that can handle modifying other classes properties
	defined as Config/GlobalConfig as a batch. It validates any new values
	sent its way with the property itself, except for strings. It can only
	handle simple types of values, like string, int, bool, byte, enum(?)
	and float. New Values are not modifying the handled classes defaults
	directly but in a batch when SaveSettings() is issued.

=============================================================================*/

#include "EnginePrivate.h"

IMPLEMENT_CLASS(UPlayInfo);

// native final function bool Clear()
void UPlayInfo::execClear( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execClear);

	P_FINISH;

	Settings.Empty();
	Groups.Empty();
	ClassStack.Empty();
	InfoClasses.Empty();

	unguardexecSlow;
}

// native final function AddClass(class Class)
void UPlayInfo::execAddClass( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execAddClass);

	P_GET_OBJECT(UClass,Class);
	P_FINISH;

	for (INT i = 0; i<InfoClasses.Num(); i++)
		if (InfoClasses(i) == Class)
		{
			debugf(NAME_Log, TEXT("Class '%s' already in PlayInfo."), Class->GetFullName());
			return;
		}

	INT idx = InfoClasses.AddItem(Class);
	ClassStack.AddItem(idx);

	unguardexecSlow;
}

// native final function AddClass(class Class)
// PopClass is used after a different class has been added and we need
// to come back to continue adding settings to the original class
void UPlayInfo::execPopClass( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execPopClass);
	P_FINISH;

	ClassStack.Pop();

	unguardexecSlow;
}

//native final function bool ReadSettings(class Class);
void UPlayInfo::execAddSetting( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execAddSetting);
	P_GET_STR(Group);
	P_GET_STR(PropertyName);
	P_GET_STR(DisplayName);
	P_GET_BYTE(SecLevel);
	P_GET_BYTE(Weight);
	P_GET_STR(RenderType);
	P_GET_STR_OPTX(Extras, TEXT(""));
	P_GET_STR_OPTX(ExtraPrivs, TEXT(""));
	P_FINISH;

	class UClass *Class;
	class UProperty *Prop = NULL;
	INT i;

//	debugf(TEXT("PlayInfo.AddSettings: PropName='%s', DispName='%s'"), (const TCHAR *)*PropertyName, (const TCHAR *)*DisplayName);
	// First, Context validity checks
	if (InfoClasses.Num() < 1)
	{
		debugf(NAME_Log, TEXT("Invalid Call to AddSetting() with no Class defined"));
		return;
	}		

	// 2nd, Set which class we will find the property in
	Class = InfoClasses(ClassStack(ClassStack.Num()-1));

	// First, Find the required property
	for ( TFieldIterator<UProperty> It(Class); It; ++It )
		if ( It->PropertyFlags & CPF_Config && It->GetFName() != NAME_None && appStricmp(It->GetName(), *PropertyName) == 0 )
		{
			Prop = *It;
			break;
		}
	
	// Reject when property name is invalid
	if (!Prop)
	{
		debugf(NAME_Log, TEXT("Property '%s' not found in Class '%s'"), *PropertyName, Class->GetFullName());
		return;
	}

	// Give a chance to the class to accept/reject adding the property in the list
	if (!Cast<AInfo>(Class->GetDefaultObject())->eventAcceptPlayInfoProperty(PropertyName))
		return;

	// Check for duplicates
	// TODO: Verify if values should be updated or do we reject ?
	for (i=0; i<Settings.Num(); i++)
		if (Settings(i).ThisProp == Prop)
		{
			debugf(NAME_Log, TEXT("Property '%s' already in list"), Prop->GetFullName());
			return;
		}

	// Property not in Settings List .. Good, Add it sorted to the list
	INT oldcmp = 0;
	BYTE oldw = 0;	// Current and Old weight, for ordering

	// Find index to insert based on Grouping and Weight
	for (i = 0; i<Settings.Num(); i++)
	{
	INT cmp = appStrcmp(*Group, *Settings(i).Grouping);
	INT IdxWeight = Settings(i).Weight;

		if (cmp < 0 || (cmp == 0 && (oldcmp > 0 || (oldcmp == 0 && Weight >= oldw)) && Weight < IdxWeight))
			break;

		oldcmp = cmp;
		oldw = IdxWeight;
	}

	if (i < Settings.Num())
		Settings.InsertZeroed(i);
	else					
		i = Settings.AddZeroed();

	FPlayInfoData &PID = Settings(i);
	TCHAR	TempStr[1024]=TEXT("");

	if (Prop->ExportText(0, TempStr, (BYTE *) Class->GetDefaultObject(), &Class->Defaults(0), PPF_Localized))
		PID.Value = TempStr;
				
	PID.ThisProp = Prop;
	PID.ClassFrom = Class;
	PID.SettingName = FString::Printf(TEXT("%s.%s"), *Class->GetFName(), Prop->GetName());
	PID.DisplayName = DisplayName;
	PID.Grouping = Group;
	PID.SecLevel = SecLevel;
	PID.Weight = Weight;
	PID.RenderType = RenderType;
	PID.Data = Extras;
	PID.ExtraPriv = ExtraPrivs;
	// TODO: Find a solution against ClassStack.Num() == 1, Allow script code to decide if all 2nd+ classes are global or not;
	PID.bGlobal = ((Prop->PropertyFlags & CPF_GlobalConfig) && Prop->GetOwnerClass() != Class && ClassStack.Num() == 1);
		
	// Update the Groups array
	if (Groups.FindItemIndex(PID.Grouping) == INDEX_NONE)
	{
		// Find Sorted Insertion Point
		for (i = 0; i<Groups.Num(); i++)
			if (appStrcmp(*PID.Grouping, *Groups(i)) < 0)
				break;

		if (i < Groups.Num())
		{
			Groups.InsertZeroed(i);
			Groups(i) = *PID.Grouping;
		}
		else
			new(Groups) FString(PID.Grouping);
	}
	unguardexecSlow;
}

void UPlayInfo::execSaveSettings( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execSaveSettings);
	P_FINISH;

	*(DWORD *)Result = 0;
	INT i;
	// For each item in the list
	for (i=0; i<Settings.Num(); i++)
	{
		// Copy the Value over
		UProperty *Prop = Settings(i).ThisProp;
		Prop->ImportText(*Settings(i).Value, (BYTE *) &Settings(i).ClassFrom->Defaults(Prop->Offset), PPF_Localized);
	}
	// Static Save our base classes
	for (i=0; i<InfoClasses.Num(); i++)
	{
		UClass *Cls = InfoClasses(i);
		UObject *Obj = (UObject *) &Cls->Defaults(0);
		Obj->SaveConfig();
	}
	unguardexecSlow;
}

void UPlayInfo::execStoreSetting( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execStoreSetting);
	P_GET_INT(Index);
	P_GET_STR(NewVal);
	P_GET_STR_OPTX(DataRange, TEXT(""));
	P_FINISH;

	*(DWORD *)Result = 0;
	if (Index >= 0 && Index < Settings.Num())
	{
	INT i = Index;

		UProperty *Prop = Settings(i).ThisProp;
		// Checkboxes return "" when unchecked
		if (appStricmp(TEXT("check"), *Settings(i).RenderType) == 0 && appStricmp(*NewVal, TEXT("True")) != 0)
			NewVal = TEXT("False");

		if (Prop->IsA(UStrProperty::StaticClass()))	// Special Case for Strings
		{
			Settings(i).Value = NewVal;
			*(DWORD *)Result = 1;
		}
		else
		{
		TCHAR NewData[1024];
		FString Left, Temp, Right;

			if (appStricmp(TEXT("Text"), *Settings(i).RenderType) == 0)
			{
				// Check for Range Validation for Integers
				if (Prop->IsA(UIntProperty::StaticClass()))
				{
					// Do a numerical range validation
					if (DataRange.Split(TEXT(";"), &Left, &Temp))
					{
						if (Temp.Split(TEXT(":"), &Left, &Right))
						{
						int RngMin, RngMax, IntVal;

							IntVal = appAtoi(*NewVal);
							RngMin = appAtoi(*Left);
							RngMax = appAtoi(*Right);

							if (IntVal < RngMin)	IntVal = RngMin;
							if (IntVal > RngMax)	IntVal = RngMax;

							NewVal = NewVal.Printf(TEXT("%i"), IntVal);
						}
					}
				}

				// Check for Range Validation for Floats
				if (Prop->IsA(UFloatProperty::StaticClass()))
				{
					// Do a numerical range validation
					if (DataRange.Split(TEXT(";"), &Left, &Temp))
					{
						if (Temp.Split(TEXT(":"), &Left, &Right))
						{
						FLOAT RngMin, RngMax, FltVal;

							FltVal = appAtof(*NewVal);
							RngMin = appAtof(*Left);
							RngMax = appAtof(*Right);

							if (FltVal < RngMin)	FltVal = RngMin;
							if (FltVal > RngMax)	FltVal = RngMax;

							NewVal = NewVal.Printf(TEXT("%f"), FltVal);
						}
					}
				}
			}

			if (Prop->ImportText(*NewVal, (BYTE *) NewData, PPF_Localized) != NULL)
			{
				// Re-export value to Settings(i).Value
				TCHAR	TempStr[1024]=TEXT("");

				// HACK: I dont want to change defaults unless the PlayInfo is fully accepted with SaveSettings()
				BYTE *HackOffset = (BYTE *)NewData - Prop->Offset;
				if (Prop->ExportText(0, TempStr, HackOffset, HackOffset, PPF_Localized))
				{
					Settings(i).Value = TempStr;
					*(DWORD *)Result = 1;
				}
			}
			else
				debugf(NAME_Log, TEXT("Invalid value for '%s': %s"), Prop->GetName(), *Settings(i).Value);
		}
	}
	else
		debugf(NAME_Log, TEXT("Invalid Setting Index : %d"), Index);

	unguardexecSlow;
}

// TODO : Allow for Setting name to contain or not contain class name.
void UPlayInfo::execFindIndex( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execFindIndex);

	P_GET_STR(SettingName);
	P_FINISH;

	INT i;
	for (i = 0; i<Settings.Num(); i++)
		if (appStrcmp(*Settings(i).SettingName, *SettingName) == 0)
			break;

	*(INT *)Result = (i < Settings.Num()) ? i : INDEX_NONE;
	unguardexecSlow;
}

void UPlayInfo::execSplitStringToArray( FFrame& Stack, RESULT_DECL )
{
	guardSlow(UPlayInfo::execSplitStringToArray);

	P_GET_TARRAY_REF(AStr, FString);
	P_GET_STR(Src);
	P_GET_STR(Div);
	P_FINISH;

	AStr->Empty();
	Split(*AStr, Src, *Div);

	unguardexecSlow;
}

void UPlayInfo::Split(TArray<FString>& OutA, const FString& Src, const TCHAR *SplitText)
{
FString Txt = Src;
INT l = appStrlen(SplitText);

	OutA.Empty();
	while (Txt.Len() > 0 && l>0)
	{
	INT pos;

		pos = Txt.InStr(SplitText);
		if (pos >= 0)
		{
			new(OutA) FString(Txt.Left(pos));
			Txt = Txt.Mid(pos+l);
		}
		else
		{
			new(OutA) FString(Txt);
			Txt.Empty();
		}
	}
}

