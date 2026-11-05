//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include <math.h>

#include "effect_distorsion.h"
#include "dsp.h"

void Effect_Distorsion_Init(Effect_Distorsion *fx)
{
    Potentiometer_InitMany(fx->pots, EFFECT_POT_COUNT(fx));

    for (int i = 0; i < EFFECT_POT_COUNT(fx); ++i)
    {
        fx->controlPtrs[i] = &fx->pots[i].baseControl;
    }

    fx->common.controlCount = ARRAY_CNT(fx->controlPtrs);
    fx->common.controls     = fx->controlPtrs;

    Effects_Algo_AAInstance_Init(&fx->aaFir, fx->aaState);
}

void Effect_Distorsion_Draw(Effect_Distorsion *fx)
{
}

void Effect_Distorsion_HandleInput(Effect_Distorsion *fx, u8 inputData)
{
}

void Effect_Distorsion_ProcessAudio(Effect_Distorsion *fx)
{
    Effects_Algo_AA(&fx->aaFir, DSP_WorkBuf, DSP_CHUNK_SIZE);

    f32         gain     = fx->potGain.value / (f32) fx->potGain.degreeRange;
    const float MAX_GAIN = 40;

    for (int i = 0; i < DSP_CHUNK_SIZE; ++i)
    {
        f32 y = DSP_WorkBuf[i];

        float clipIn     = y * gain * MAX_GAIN;
        float absClipIn  = fabs(clipIn);
        float signClipIn = (clipIn >= 0.0f) ? 1.0f : -1.0f;

        const float THR     = 1 / 3.0F;
        float       clipOut = 0.0f;

        if (absClipIn < THR)
        {
            clipOut = 2.0f * clipIn;
        }
        else if (absClipIn >= THR && absClipIn < (2.0f * THR))
        {
            clipOut = signClipIn * (3.0f - (2.0f - 3.0f * absClipIn) * (2.0f - 3.0f * absClipIn)) / 3.0f;
        }
        else
        {
            clipOut = signClipIn;
        }

        DSP_WorkBuf[i] = clipOut / 10.0F;
    }

    // Effects_Algo_AA(&fx->aaFir, DSP_WorkBuf, DSP_CHUNK_SIZE);
}
