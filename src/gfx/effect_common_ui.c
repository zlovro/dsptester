//
// Created by lovro on 22.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_common_ui.h"

#include <stdarg.h>

#include "input.h"
#include "ui_common.h"

void Effect_UI_DefaultEditScreen_Init(Effect_UI_DefaultEditScreen *scr, Potentiometer *pots, int count, ...)
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

    scr->pots     = pots;
    scr->potCount = count;
    scr->active   = pots;
    scr->selected = false;
}

void Effect_UI_DefaultEditScreen_Draw(Effect_UI_DefaultEditScreen *scr)
{
    for (int i = 0; i < scr->potCount; ++i)
    {
        Potentiometer *pot = scr->pots + i;
        Potentiometer_Draw(pot);

        if (scr->active == pot)
        {
            UIControl_Rect rect = pot->common.rect;
            ST7920_DrawLineHorizontal(rect.left, rect.bottom, UIControl_Rect_Width(rect));
            // ST7920_InvertRegion(rect.x0, rect.y0, rect.x1, rect.y1);
            // ST7920_DrawRectangleOutline1(rect.x0, rect.y0, UIControl_Rect_Width(rect), UIControl_Rect_Height(rect));

            if (scr->selected)
            {
                ST7920_InvertRegion(rect.x0, rect.y0, rect.x1, rect.y1);
                // ST7920_DrawLineHorizontal(rect.left, rect.bottom, UIControl_Rect_Width(rect));
                // ST7920_DrawRectangleOutline1(rect.x0, rect.y0, UIControl_Rect_Width(rect), UIControl_Rect_Height(rect));
            }
        }
    }
}

void Effect_UI_DefaultEditScreen_HandleInput(Effect_UI_DefaultEditScreen *scr)
{
    InputData in = InputData_Data;

    if (in.rotaryBtn == INPUT_BTN_DOWN)
    {
        scr->selected = !scr->selected;
    }

    bool selected = scr->selected;
    if (in.rotaryDir == INPUT_DIR_L)
    {
        if (selected)
        {
            f32 value = scr->active->value;

            value -= UI_POTENTIOMETER_STEP;
            if (value < 0)
            {
                value = 0;
            }

            Potentiometer_SetValue(scr->active, value);
        }
        else
        {
            scr->active--;
            if (scr->active < scr->pots)
            {
                scr->active = scr->pots + scr->potCount - 1;
            }
        }
    }
    else if (in.rotaryDir == INPUT_DIR_R)
    {
        if (selected)
        {
            f32 value = scr->active->value;

            value += UI_POTENTIOMETER_STEP;
            if (value >= 1.0F)
            {
                value = 1.0F;
            }

            Potentiometer_SetValue(scr->active, value);
        }
        else
        {
            scr->active++;
            if (scr->active >= scr->pots + scr->potCount)
            {
                scr->active = scr->pots;
            }
        }
    }
}
