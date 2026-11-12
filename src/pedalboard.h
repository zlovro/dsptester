//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef PEDALBOARD_H
#define PEDALBOARD_H

#include <fx/effect.h>
#include <assert.h>

#define PEDALBOARD_PEDAL_COUNT 6

#define PEDALBOARD_ROWS 2
#define PEDALBOARD_COLS 3

static_assert(PEDALBOARD_ROWS * PEDALBOARD_COLS == PEDALBOARD_PEDAL_COUNT);

#define PEDALBOARD_MARGIN 3

#define PEDALBOARD_PEDAL_PADDING 2
#define PEDALBOARD_PEDAL_WIDTH (3 * (ST7920_MAX_GLYPH_WIDTH + ST7920_FONT_SPACING_X) + 2 * PEDALBOARD_PEDAL_PADDING)
#define PEDALBOARD_PEDAL_HEIGHT (ST7920_FONT_HEIGHT + 2 * PEDALBOARD_PEDAL_PADDING)

extern struct Effect_CommonData *Pedalboard_Pedals[];
extern int Pedalboard_SelectedPedalIndex;

// must be ran after
extern void Pedalboard_Init();
extern void Pedalboard_Draw();

extern void Pedalboard_SelectNextPedal();
extern void Pedalboard_SelectPrevPedal();

#endif //PEDALBOARD_H
