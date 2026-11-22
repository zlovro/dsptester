//
// Created by lovro on 22.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_common_ui.h"

#include <stdarg.h>

#include "ui_common.h"

void Effect_UI_InitDefaultEditScreen(Potentiometer *pots, int count, ...)
{
    va_list args;
    va_start(args, count);

    constexpr int potSpacingX = 5;
    constexpr int totalSpaceX = ST7920_WIDTH - 2 * UI_GENERIC_MARGIN;

    int totalPotWidth = 0;
    for (int i = 0; i < count; ++i)
    {
        Potentiometer *pot = pots + i;
        Potentiometer_Init(pot, va_arg(args, const char*), 64, 32, false);

        totalPotWidth += pot->width;
    }

    va_end(args);

    int availableSpaceX = totalSpaceX - (count - 1) * potSpacingX - totalPotWidth;
    int startX          = UI_GENERIC_MARGIN + availableSpaceX / 2;

    int x = startX;
    for (int i = 0; i < count; ++i)
    {
        Potentiometer *pot = pots + i;
        Potentiometer_SetPos(pot, x + POTENTIOMETER_RADIUS, ST7920_HEIGHT / 2 - (POTENTIOMETER_LABEL_Y_PADDING + ST7920_LINE_HEIGHT) / 2, true);

        x += pot->width + potSpacingX;
    }
}

void Effect_UI_DrawDefaultEditScreen(Potentiometer *pots, int count)
{
    for (int i = 0; i < count; ++i)
    {
        Potentiometer_Draw(pots + i);
    }
}
