//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_eq.h"

#include <math.h>

static void Effect_Eq_Init(Effect_Eq *eq)
{
    int n = EFFECT_EQ_BAND_COUNT - 1;

    for (int i = 0; i <= n; ++i)
    {
        f32 freq = EFFECT_EQ_F0 * powf(EFFECT_EQ_F1 / EFFECT_EQ_F0, i / (f32) n);

        eq->bands[i] = ( Effect_Eq_Band){1.0F, freq};
    }
}

static void Effect_Eq_Draw(Effect_Eq *eq)
{
    int n = EFFECT_EQ_BAND_COUNT - 1;

    for (int i = 0; i < n; ++i)
    {
        f32 freq = EFFECT_EQ_F0 * powf(EFFECT_EQ_F1 / EFFECT_EQ_F0, i / (f32) n);

        eq->bands[i] = ( Effect_Eq_Band){1.0F, freq};
    }
}

static void Effect_Eq_HandleInput(Effect_Eq *eq, u8 inputData)
{
}

static void Effect_Eq_ProcessAudio(Effect_Eq *eq)
{
}

Effect_Eq   Effect_Eq_Instances[8] = {EFFECT_DEFINE(EFFECT_ID_EQ, "8-band EQ", Effect_Eq_Init, Effect_Eq_Draw, Effect_Eq_HandleInput, Effect_Eq_ProcessAudio)};
EffectArray Effect_Eq_Array        = {ARRAY_CNT(Effect_Eq_Instances), sizeof(Effect_Eq), &Effect_Eq_Instances[0].header};
