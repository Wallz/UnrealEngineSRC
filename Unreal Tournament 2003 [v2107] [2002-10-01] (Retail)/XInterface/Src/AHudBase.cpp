//=============================================================================
// Copyright 2001 Digital Extremes - All Rights Reserved.
// Confidential.
//=============================================================================

#include "XInterface.h"

IMPLEMENT_CLASS(AHudBase);

//native simulated function DrawSpriteWidget (Canvas C, out SpriteWidget W);
void AHudBase::execDrawSpriteWidget( FFrame& Stack, RESULT_DECL )
{
	guard(AHudBase::execDrawSpriteWidget);

    P_GET_OBJECT(UCanvas,pCanvas);
    P_GET_STRUCT_REF(FSpriteWidget,W)

	P_FINISH;

    if( !W || !W->WidgetTexture )
		return;

    float TotalScaleX, TotalScaleY;
    float TextureX1, TextureY1;
    float TextureDX, TextureDY;
    float ScreenDX, ScreenDY;
    float ScreenOffsetX, ScreenOffsetY;
    float ScreenX, ScreenY;

    TotalScaleX = W->TextureScale * HudCanvasScale * ResScaleX * HudScale;
    TotalScaleY = W->TextureScale * HudCanvasScale * ResScaleY * HudScale;

    TextureX1 = W->TextureCoords.X1;
    TextureY1 = W->TextureCoords.Y1;

    TextureDX = W->TextureCoords.X2 - W->TextureCoords.X1 + 1.0f;
    TextureDY = W->TextureCoords.Y2 - W->TextureCoords.Y1 + 1.0f;

    if ((TextureDX == 0.0) || (TextureDY == 0.0))
        return;

    ScreenDX = Abs(TextureDX) * TotalScaleX;
    ScreenDY = Abs(TextureDY) * TotalScaleY;

    ScreenOffsetX = W->OffsetX * TotalScaleX;
    ScreenOffsetY = W->OffsetY * TotalScaleY;

    ScreenX = (W->PosX * HudCanvasScale * pCanvas->SizeX) + (((1.0f - HudCanvasScale) * 0.5f) * pCanvas->SizeX);
    ScreenY = (W->PosY * HudCanvasScale * pCanvas->SizeY) + (((1.0f - HudCanvasScale) * 0.5f) * pCanvas->SizeY);

    ScreenX += ScreenOffsetX;
    ScreenY += ScreenOffsetY;

    CalcPivotCoords( (EDrawPivot)(W->DrawPivot), ScreenX, ScreenY, ScreenDX, ScreenDY );

    //if ((W->Scale < 0.0f) || (W->Scale > 1.0f))
    //    debugf( NAME_Error, TEXT("DrawSpriteWidget() -- W->Scale out of range"));
	W->Scale = Clamp<FLOAT>(W->Scale,0.f,1.f);

    switch (W->ScaleMode)
    {
        case SM_None:
            break;

        case SM_Up:
            ScreenY += (1.0f - W->Scale) * ScreenDY;
            ScreenDY *= W->Scale;
            TextureY1 += (1.0f - W->Scale) * TextureDY;
            TextureDY *= W->Scale;
            break;

        case SM_Down:
            ScreenDY *= W->Scale;
            TextureDY *= W->Scale;
            break;

        case SM_Left:
            ScreenX += (1.0f - W->Scale) * ScreenDX;
            ScreenDX *= W->Scale;
            TextureX1 += (1.0f - W->Scale) * TextureDX;
            TextureDX *= W->Scale;
            break;

        case SM_Right:
            ScreenDX *= W->Scale;
            TextureDX *= W->Scale;
            break;
    }

    pCanvas->CurX = ScreenX;
	pCanvas->CurY = ScreenY;
	pCanvas->Style = W->RenderStyle;
    pCanvas->Color = W->Tints[TeamIndex];

    if ((PassStyle != STY_None) && (W->RenderStyle != PassStyle))
        debugf(NAME_Error,TEXT("DrawSpriteWidget() draw style mis-match (%d was supposed to be %d)"), W->RenderStyle, PassStyle );

	pCanvas->DrawTile
    (
        W->WidgetTexture,
        pCanvas->OrgX+pCanvas->CurX,
        pCanvas->OrgY+pCanvas->CurY,
        ScreenDX,
        ScreenDY,
        TextureX1,
        TextureY1,
        TextureDX,
        TextureDY,
        pCanvas->Z,
        pCanvas->Color.Plane(),
		FPlane(0.0f,0.0f,0.0f,0.0f)
    );

    unguard;
}

//native simulated function DrawNumericWidget (Canvas C, out NumericWidget W, out DigitSet D);
void AHudBase::execDrawNumericWidget( FFrame& Stack, RESULT_DECL )
{
	guard(AHudBase::execDrawNumericWidget);

    P_GET_OBJECT(UCanvas,pCanvas);
    P_GET_STRUCT_REF(FNumericWidget,W)
    P_GET_STRUCT_REF(FDigitSet,D)
	P_FINISH;

    int Digits [32];

    if( !D->DigitTexture )
    {
        debugf( NAME_Warning, TEXT("DrawNumericWidget called with no texture!") );
        return;
    }

    int DigitCount;
    float ScreenX, ScreenY;

    float TotalScaleX, TotalScaleY;

    TotalScaleX = W->TextureScale * HudCanvasScale * ResScaleX * HudScale;
    TotalScaleY = W->TextureScale * HudCanvasScale * ResScaleY * HudScale;

    int AbsValue;
    int Digit;

    float TextureDX, TextureDY;
    float DigitDX, DigitDY;

    float ScreenDX, ScreenDY;

    float ScreenOffsetX, ScreenOffsetY;

    int MinDigitCount;

    AbsValue = Abs(W->Value);
    DigitCount = 0;

    TextureDX = 0;
    TextureDY = D->TextureCoords[0].Y2 - D->TextureCoords[0].Y1 + 1.0f;

    do
    {
        Digit = AbsValue % 10;
        Digits[DigitCount] = Digit;
        AbsValue /= 10;
        DigitCount++;

        DigitDX = D->TextureCoords[Digit].X2 - D->TextureCoords[Digit].X1 + 1.0f;
        DigitDY = D->TextureCoords[Digit].Y2 - D->TextureCoords[Digit].Y1 + 1.0f;

        if (DigitDY != TextureDY)
            debugf(NAME_Error,TEXT("DrawNumericWidget() -- DigitSet with uneven height detected [%d]"), Digit );

        TextureDX += DigitDX;

    } while (AbsValue != 0);

    if (W->bPadWithZeroes)
    {
        if (W->Value < 0)
            MinDigitCount = W->MinDigitCount - 1;
        else
            MinDigitCount = W->MinDigitCount;

        Digit = 0;

        DigitDX = D->TextureCoords[Digit].X2 - D->TextureCoords[Digit].X1 + 1.0f;
        DigitDY = D->TextureCoords[Digit].Y2 - D->TextureCoords[Digit].Y1 + 1.0f;

        while (DigitCount < MinDigitCount)
        {
            Digits[DigitCount] = Digit;
            DigitCount++;
            TextureDX += DigitDX;
        }

        if (W->Value < 0)
        {
            Digit = 10; // The 10th digit is the negative sign
            Digits[DigitCount] = Digit;
            DigitCount++;

            DigitDX = D->TextureCoords[Digit].X2 - D->TextureCoords[Digit].X1 + 1.0f;
            DigitDY = D->TextureCoords[Digit].Y2 - D->TextureCoords[Digit].Y1 + 1.0f;

            if (DigitDY != TextureDY)
                debugf(NAME_Error,TEXT("DrawNumericWidget() -- DigitSet with uneven height detected [-]"));

            TextureDX += DigitDX;
        }
    }
    else
    {
        MinDigitCount = W->MinDigitCount;

        if (W->Value < 0)
        {
            Digit = 10; // The 10th digit is the negative sign
            Digits[DigitCount] = Digit;
            DigitCount++;

            DigitDX = D->TextureCoords[Digit].X2 - D->TextureCoords[Digit].X1 + 1.0f;
            DigitDY = D->TextureCoords[Digit].Y2 - D->TextureCoords[Digit].Y1 + 1.0f;

            if (DigitDY != TextureDY)
                debugf(NAME_Error,TEXT("DrawNumericWidget() -- DigitSet with uneven height detected [-]"));

            TextureDX += DigitDX;
        }

        Digit = 0;

        DigitDX = D->TextureCoords[Digit].X2 - D->TextureCoords[Digit].X1 + 1;
        DigitDY = D->TextureCoords[Digit].Y2 - D->TextureCoords[Digit].Y1 + 1;

        while (DigitCount < MinDigitCount)
        {
            Digits[DigitCount] = -1;
            DigitCount++;
            TextureDX += DigitDX;
        }
    }

    ScreenDX = TextureDX * TotalScaleX;
    ScreenDY = TextureDY * TotalScaleY;

    ScreenOffsetX = W->OffsetX * TotalScaleX;
    ScreenOffsetY = W->OffsetY * TotalScaleY;

    ScreenX = (W->PosX * HudCanvasScale * pCanvas->SizeX) + (((1.0f - HudCanvasScale) * 0.5f) * pCanvas->SizeX);
    ScreenY = (W->PosY * HudCanvasScale * pCanvas->SizeY) + (((1.0f - HudCanvasScale) * 0.5f) * pCanvas->SizeY);

    ScreenX += ScreenOffsetX;
    ScreenY += ScreenOffsetY;

    CalcPivotCoords( (EDrawPivot)(W->DrawPivot), ScreenX, ScreenY, ScreenDX, ScreenDY );

	pCanvas->Style = W->RenderStyle;

    if ((PassStyle != STY_None) && (W->RenderStyle != PassStyle))
        debugf(NAME_Error,TEXT("DrawNumericWidget() draw style mis-match (%d was supposed to be %d)"), W->RenderStyle, PassStyle );

    pCanvas->Color = W->Tints[TeamIndex];

    do
    {
        DigitCount--;

        int Digit = Digits[DigitCount];

        if (Digit >= 0)
        {
            float DigitDX = D->TextureCoords[Digit].X2 - D->TextureCoords[Digit].X1 + 1.0f;
            float DigitDY = D->TextureCoords[Digit].Y2 - D->TextureCoords[Digit].Y1 + 1.0f;

            float ScreenDX = DigitDX * TotalScaleX;
            float ScreenDY = DigitDY * TotalScaleY;

            pCanvas->CurX = ScreenX;
            pCanvas->CurY = ScreenY;

            pCanvas->DrawTile
            (
                D->DigitTexture,
                pCanvas->OrgX+pCanvas->CurX,
                pCanvas->OrgY+pCanvas->CurY,
                ScreenDX,
                ScreenDY,
                D->TextureCoords[Digit].X1,
                D->TextureCoords[Digit].Y1,
                DigitDX,
                DigitDY,
                pCanvas->Z,
                pCanvas->Color.Plane(),
	            FPlane(0.0f,0.0f,0.0f,0.0f)
            );

            ScreenX += ScreenDX;
        }
        else
        {
            float DigitDX = D->TextureCoords[0].X2 - D->TextureCoords[0].X1 + 1.0f;
            float ScreenDX = DigitDX * TotalScaleX;
            ScreenX += ScreenDX;
        }

    } while (DigitCount != 0);

    unguard;
}