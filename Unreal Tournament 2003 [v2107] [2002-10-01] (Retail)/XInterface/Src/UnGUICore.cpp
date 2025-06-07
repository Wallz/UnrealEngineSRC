/*=============================================================================
	UnGUICore.cpp: See .UC for for info
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

	Contains the non composite controls

Revision history:
	* Created by Michel Comeau
=============================================================================*/

#include "XInterface.h"

IMPLEMENT_CLASS(UGUIProgressBar);

// =======================================================================================================================================================
// =======================================================================================================================================================
// UGUIProgressBar
// =======================================================================================================================================================
// =======================================================================================================================================================

void UGUIProgressBar::Draw(UCanvas* Canvas)
{
	guard(UGUIProgressBar::Draw);

	if (!bVisible)
		return;

	Super::Draw(Canvas);

	float Left = ActualLeft(), Width = ActualWidth();
	float Top = ActualTop(), Height = ActualHeight();
	
	if (CaptionWidth > 0.0 && Width > 0)
	{
	float W = CaptionWidth;

		if (W < 1.0)
			W *= Width;

		if (W > Width)
			W = Width;

		// Select the right font in the Canvas
		UGUIFont *Fnt = Controller->eventGetMenuFont(FontName);
		if (Fnt)
		{
			Canvas->Font = Fnt->eventGetFont(Canvas->SizeX);
		}

		// Draw the label
		Canvas->DrawTextJustified(CaptionAlign, Left, Top, Left + W, Top + Height, TEXT("%s"), *Caption);
		Left += W;
		Width -= W;
	}

	if ( (bShowHigh || bShowValue) && ValueRightWidth > 0.0 && Width > 0.0)
	{
	float W = ValueRightWidth;
	FString str;

		if (W < 1.0)
			W *= Width;

		if (W > Width)
			W = Width;

		if (bShowValue && bShowHigh)
			str = FString::Printf(TEXT("%0.0f/%0.0f"), Value, High);
		else if (bShowValue)
			str = FString::Printf(TEXT("%0.0f"), Value);
		else
			str = FString::Printf(TEXT("%0.0f"), High);

		Canvas->DrawTextJustified(ValueRightAlign, Left + Width - W, Top, Left + Width, Top + Height, TEXT("%s"), *str);

		Width -= W;
	}
	
	if (Width > GraphicMargin)
	{
		Width -= GraphicMargin;
		Left += GraphicMargin / 2;
	}

	// Actually Draw the content
	Canvas->Color = FColor(255,255,255);
	if (Width > 0.0 && BarBack)
		Canvas->DrawTileStretched(BarBack, Left, Top, Width, Height);

	if (Width > 0.0 && BarTop && Value > Low)
	{
		Canvas->Style = STY_Normal;
		Canvas->Color = BarColor;
		Canvas->DrawTileStretched(BarTop, Left, Top, Width * Value/High, Height);
	}
	unguard;
}