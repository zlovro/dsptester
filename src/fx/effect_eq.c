//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_eq.h"

#include <assert.h>
#include <math.h>

#include "dsp.h"
#include "algo/fxalgo_eq.h"

void Effect_EqInit(Effect_Eq *fx)
{
    Effect_Algo_EqInstance_Init(&fx->algoInstance);
}

void Effect_EqDraw(Effect_Eq *fx)
{
}

void Effect_EqHandleInput(Effect_Eq *fx, u8 inputData)
{
}

void Effect_EqProcessAudio(Effect_Eq *fx)
{
    Effect_Algo_Eq(&fx->algoInstance);
}

void Effect_EqBandUpdate(Effect_Eq *fx, int band, u16 freq, f32 gain, f32 q)
{
    Effect_Algo_EqBandUpdate(fx->algoInstance.bands + band, freq, gain, q);
}

void Effect_EqBandUpdateByFreq(Effect_Eq *fx, u16 searchFreq, f32 gain, f32 q)
{
    for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
    {
        Effect_Algo_EqBand* band = fx->algoInstance.bands + i;
        if (band->freq == searchFreq)
        {
            Effect_Algo_EqBandUpdate(band, searchFreq, gain, q);
            return;
        }
    }

    assert("Unknown eq freq");
}
