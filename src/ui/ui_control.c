//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "ui_control.h"

#include <math.h>
#include <stdio.h>

#include "st7920.h"

void Potentiometer_SetText(char *txt)
{
}

void Potentiometer_InitMany(Potentiometer *pots, int count)
{
    f32 betweenMargin = (128 - 2 * ST7920_ORIGIN_X - 2 * POTENTIOMETER_SIDE_MARGIN - 2 * count * POTENTIOMETER_RADIUS_F) / (count - 1.0F);

    f32 xf = ST7920_ORIGIN_X + POTENTIOMETER_SIDE_MARGIN;
    for (int i = 0; i < count; i++)
    {
        xf += POTENTIOMETER_RADIUS_F;

        Potentiometer *pot = pots + i;

        pot->centerX     = (int) xf;
        pot->centerY     = POTENTIOMETER_POS_Y;
        pot->radius      = POTENTIOMETER_RADIUS_I;
        pot->degreeRange = POTENTIOMETER_DEGREE_RANGE;
        pot->value       = 0;

        xf += POTENTIOMETER_RADIUS_F + betweenMargin;
    }
}

void Potentiometer_Draw(Potentiometer *pot)
{
    int cx = pot->centerX;
    int cy = pot->centerY;
    int r  = pot->radius;

    ST7920_DrawCircleOutline(cx, cy, r);

    int degLeft  = 270 - (360 - pot->degreeRange) / 2;
    int degRight = pot->degreeRange + degLeft;

    int degWiper = degLeft - ((degRight - degLeft) * pot->value) / 100;

    f32 radWiper = degWiper * M_PI / 180;
    int rlim     = degWiper >= 90 ? r - 1 : r;

    int wiperX = cx + cosf(radWiper) * rlim;
    int wiperY = cy - sinf(radWiper) * rlim;

    ST7920_DrawLineBresenham(cx, cy, wiperX, wiperY);

    char buf[32];
    sprintf(buf, "%s\n%d", pot->label, pot->value);

    ST7920_DrawStringCenteredInRect(buf, cx - r, cy + r + ST7920_LINE_HEIGHT / 2, 2 * r, 1000, false, true);

    int   linesInLabel = 1;
    char  c;
    char *p = buf;
    while (c = *p++)
    {
        if (c == '\n')
        {
            linesInLabel++;
        }
    }

    if (pot->baseControl.isSelected)
    {
        int selectedMargin = 1;
        ST7920_InvertRegion(cx - r - selectedMargin, cy - r - selectedMargin, cx + r + selectedMargin, cy + r + ST7920_LINE_HEIGHT / 2 + linesInLabel * ST7920_LINE_HEIGHT);
    }
}
