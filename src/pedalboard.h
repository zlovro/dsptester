//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef PEDALBOARD_H
#define PEDALBOARD_H

#include <audiofx/effect.h>
#include <assert.h>

#define PEDALBOARD_PEDAL_COUNT 6

#define PEDALBOARD_ROWS 2
#define PEDALBOARD_COLS 3

static_assert(PEDALBOARD_ROWS * PEDALBOARD_COLS == PEDALBOARD_PEDAL_COUNT);

#define PEDALBOARD_MARGIN 3

#define PEDALBOARD_PEDAL_PADDING 2
#define PEDALBOARD_PEDAL_WIDTH (3 * (ST7920_MAX_GLYPH_WIDTH + ST7920_FONT_SPACING_X) + 2 * PEDALBOARD_PEDAL_PADDING)
#define PEDALBOARD_PEDAL_HEIGHT (ST7920_FONT_HEIGHT + 2 * PEDALBOARD_PEDAL_PADDING)

#define PEDALBOARD_PRESET_MAX_NAME_LENGTH 11
#define PEDALBOARD_PRESET_COUNT 99

typedef struct Pedalboard_Preset
{
    char name[PEDALBOARD_PRESET_MAX_NAME_LENGTH + 1];

    // if LSB set: 1st pedal is on, 1 left of LSB is set: 2nd pedal is on etc.
    u32 activePedals;
} Pedalboard_Preset;

extern Pedalboard_Preset Pedalboard_Presets[];
extern Pedalboard_Preset* Pedalboard_ActivePreset;

extern Effect_CommonData *Pedalboard_Pedals[];
#define PEDALBOARD_GET(t, idx) ((t*)(Pedalboard_Pedals[idx]))

extern int Pedalboard_SelectedPedalIndex;

// dynamically allocated
extern void *Pedalboard_DynamicIOBuf;

// must be ran after
extern void Pedalboard_Init();
extern void Pedalboard_Draw();

extern void Pedalboard_InsertExisting(int pos, Effect_CommonData *pedal);
extern void Pedalboard_InsertNew(int pos, EffectId pedalId);

#define Pedalboard_ClearSlot(pos) Pedalboard_Pedals[pos] = NULL
#define Pedalboard_ClearAll() for (int _i = 0; _i < PEDALBOARD_PEDAL_COUNT; ++_i) {Pedalboard_ClearSlot(_i);}

extern void Pedalboard_SelectNextPedal();
extern void Pedalboard_SelectPrevPedal();

extern void Pedalboard_HandleInput();

extern void Pedalboard_StateSerialize(void **outBufPtr);
extern void Pedalboard_StateDeserialize(void *inBuf);

extern void Pedalboard_SelectNextPreset();
extern void Pedalboard_SelectPrevPreset();
extern void Pedalboard_RefreshPreset();

#endif //PEDALBOARD_H
