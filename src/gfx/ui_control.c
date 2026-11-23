//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "ui_control.h"

#include <math.h>
#include <stdio.h>

#include "st7920.h"

void UIControl_Rect_FromCentre(UIControl_Rect *rect, int cx, int cy, int width, int height)
{
    int x0 = cx - width / 2;
    int y0 = cy - height / 2;

    UIControl_Rect_FromTopLeft(rect, x0, y0, width, height);
}

void UIControl_Rect_FromTopLeft(UIControl_Rect *rect, int x0, int y0, int width, int height)
{
    rect->x0 = x0;
    rect->y0 = y0;
    rect->x1 = x0 + width - 1;
    rect->y1 = y0 + height - 1;
}

void Potentiometer_Init(Potentiometer *pot, const char *label, int x, int y, bool posIsCentre)
{
    pot->label = label;

    Potentiometer_SetPos(pot, x, y, posIsCentre);
}

void Potentiometer_SetPos(Potentiometer *pot, int x, int y, bool posIsCentre)
{
    int wh = max(ST7920_GetStringWidth(pot->label), POTENTIOMETER_WH) + 2;

    pot->width = wh;

    if (posIsCentre)
    {
        pot->centerX = x;
        pot->centerY = y;

        UIControl_Rect_FromCentre(&pot->common.rect, x, y, wh, wh);

        pot->common.rect.bottom += POTENTIOMETER_LABEL_Y_PADDING + ST7920_LINE_HEIGHT;
    }
    else
    {
        int height = wh + POTENTIOMETER_LABEL_Y_PADDING + ST7920_LINE_HEIGHT;

        pot->centerX = x + wh / 2;
        pot->centerY = y + height / 2;

        UIControl_Rect_FromTopLeft(&pot->common.rect, x, y, wh, height);
    }

    // refresh wiper
    Potentiometer_SetValue(pot, pot->value);
}

void Potentiometer_SetValue(Potentiometer *pot, f32 value)
{
    constexpr int degRem   = (360 - POTENTIOMETER_DEGREE_RANGE) / 2;
    constexpr int degLeft  = 270 - degRem;
    constexpr int degRight = 270 + degRem;

    int degWiper = degLeft - (int) (POTENTIOMETER_DEGREE_RANGE * value);

    f32 radWiper = (f32) degWiper * M_PI_F / 180.0F;
    int rlim     = degWiper >= 90 ? POTENTIOMETER_RADIUS - 1 : POTENTIOMETER_RADIUS;

    pot->wiperX = pot->centerX + cosf(radWiper) * rlim;
    pot->wiperY = pot->centerY - sinf(radWiper) * rlim;

    pot->value = value;
}

void Potentiometer_Draw(Potentiometer *pot)
{
    int cx = pot->centerX;
    int cy = pot->centerY;

    ST7920_DrawCircleOutline(cx, cy, POTENTIOMETER_RADIUS);

    ST7920_DrawLineBresenham(cx, cy, pot->wiperX, pot->wiperY);

    UIControl_Rect rect = pot->common.rect;
    ST7920_DrawStringCenteredInRect(pot->label, rect.x0, cy + POTENTIOMETER_RADIUS + POTENTIOMETER_LABEL_Y_PADDING, UIControl_Rect_Width(rect), ST7920_LINE_HEIGHT, true, true);
}
