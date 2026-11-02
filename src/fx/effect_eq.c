//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_eq.h"

#include <math.h>

void EffectEqInit(EffectEq *eq)
{
    int n = EFFECT_EQ_BAND_COUNT - 1;

    for (int i = 0; i <= n; ++i)
    {
        f32 freq = EFFECT_EQ_F0 * powf(EFFECT_EQ_F1 / EFFECT_EQ_F0, i / (f32) n);

        eq->bands[i] = ( EffectEqBand){1.0F, freq};
    }
}

void EffectEqDraw(EffectEq *eq)
{
    int n = EFFECT_EQ_BAND_COUNT - 1;

    for (int i = 0; i < n; ++i)
    {
        f32 freq = EFFECT_EQ_F0 * powf(EFFECT_EQ_F1 / EFFECT_EQ_F0, i / (f32) n);

        eq->bands[i] = ( EffectEqBand){1.0F, freq};
    }
}

void EffectEqHandleInput(EffectEq *eq, u8 inputData)
{
}

void EffectEqProcessAudio(EffectEq *eq)
{
}