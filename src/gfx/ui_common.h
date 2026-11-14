//
// Created by lovro on 01/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef UI_COMMON_H
#define UI_COMMON_H

#include <gfx/st7920.h>

#define UI_GENERIC_MARGIN 2
#define UI_MARGIN_XY ST7920_ORIGIN_X
#define UI_MARGIN_HEADER_Y (UI_MARGIN_XY + ST7920_LINE_HEIGHT + 1 * 2 + 2)

#define UI_PRESET_TEXT_Y_POS (ST7920_ORIGIN_Y + 1)

typedef enum UI_ScreenId
{
    UI_SCR_PEDALBOARD,
    UI_SCR_PEDAL_EDIT,
    UI_SCR_OPTIONS,
    UI_SCR_COUNT
} UI_ScreenId;

extern UI_ScreenId UI_CurrentScreenId;

#endif //UI_COMMON_H
