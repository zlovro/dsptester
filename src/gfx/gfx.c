//
// Created by lovro on 14/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "gfx.h"

#include <stdio.h>

#include <gfx/ui_common.h>
#include <gfx/st7920.h>

#include <pedalboard.h>

u32          GFX_FrameCounter    = 0;

void GFX_Init()
{
}

void GFX_PreDraw()
{
    ST7920_Seek(ST7920_ORIGIN_X, ST7920_ORIGIN_Y);

    ST7920_ClearBack();
    ST7920_DrawRectangleOutline(0, 0, ST7920_WIDTH, ST7920_HEIGHT, ST7920_ORIGIN_X);
    ST7920_DrawRectangleOutline(0, 0, ST7920_WIDTH, UI_MARGIN_HEADER_Y, UI_GENERIC_MARGIN);

    ST7920_DottedOffset = GFX_FrameCounter / 4;
}

void GFX_Draw()
{
    GFX_PreDraw();

    switch (UI_CurrentScreenId)
    {
        case UI_SCR_PEDALBOARD:
        {
            ST7920_DrawStringAtPos(Pedalboard_ActivePreset->name, ST7920_ORIGIN_X + UI_GENERIC_MARGIN, UI_PRESET_TEXT_Y_POS);

            char presetIdxStr[4];
            sprintf(presetIdxStr, "%02d", (int) (1 + Pedalboard_ActivePreset - Pedalboard_Presets));

            ST7920_DrawStringAtPosRightJustified(presetIdxStr, ST7920_WIDTH - ST7920_ORIGIN_X - UI_GENERIC_MARGIN, UI_PRESET_TEXT_Y_POS);

            Pedalboard_Draw();
            break;
        }

        case UI_SCR_PEDAL_EDIT:
        {
            break;
        }
        case UI_SCR_OPTIONS:
        {
            break;
        }

        default: break;
    }

    GFX_FrameCounter++;

    ST7920_BackToFront();
}
