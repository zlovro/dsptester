//
// Created by lovro on 23/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_lowpass.h"

#include <math.h>

#include "dsp.h"

static void Effect_Lowpass_Init(Effect_Lowpass *fx)
{
    Potentiometer_InitMany(fx->pots, EFFECT_POT_COUNT(fx));

    for (int i = 0; i < EFFECT_POT_COUNT(fx); ++i)
    {
        fx->controlPtrs[i] = &fx->pots[i].baseControl;
    }

    fx->header.controlCount = ARRAY_CNT(fx->controlPtrs);
    fx->header.controls     = fx->controlPtrs;
}

static void Effect_Lowpass_Draw(Effect_Lowpass *fx)
{
}

static void Effect_Lowpass_HandleInput(Effect_Lowpass *fx, u8 inputData)
{
}

static void Effect_Lowpass_ProcessAudio(Effect_Lowpass *fx)
{
}

Effect_Lowpass Effect_Lowpass_Instances[8] = {EFFECT_DEFINE(EFFECT_ID_LOWPASS, "Lowpass", Effect_Lowpass_Init, Effect_Lowpass_Draw, Effect_Lowpass_HandleInput, Effect_Lowpass_ProcessAudio)};
EffectArray    Effect_Lowpass_Array        = {ARRAY_CNT(Effect_Lowpass_Instances), sizeof(Effect_Lowpass), &Effect_Lowpass_Instances[0].header};

void Effect_Lowpass_ProcessAudio_Independent(u16 cutoff)
{
    f32 samplingPeriod = 1.0F / (float) SAMPLE_RATE;
    f32 b              = 2 * (float) M_PI * samplingPeriod * (f32) cutoff;
    f32 a              = b / (b + 1);

    DSP_WorkBuf[0] = a * DSP_WorkBuf[0] + (1 - a) * Effect_LastSample;

    for (int i = 1; i < ARRAY_CNT(DSP_WorkBuf); ++i)
    {
        DSP_WorkBuf[i] = a * DSP_WorkBuf[i] + (1 - a) * DSP_WorkBuf[i - 1];
    }

    Effect_LastSample = DSP_WorkBuf[ARRAY_CNT(DSP_WorkBuf) - 1];
}
