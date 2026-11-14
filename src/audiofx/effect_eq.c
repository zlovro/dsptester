//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_eq.h"

#include <assert.h>
#include <math.h>

#include "dsp.h"
#include "iohelper.h"
#include "algo/fxalgo_eq.h"

void Effect_EqInit(Effect_Eq *fx)
{
    Effect_Algo_EqInstance_Init(&fx->algoInstance);
}

void Effect_EqDraw(Effect_Eq *fx)
{
}

void Effect_EqHandleInput(Effect_Eq *fx)
{
}

void Effect_EqProcessAudio(Effect_Eq *fx)
{
    Effect_Algo_Eq(&fx->algoInstance);
}

static constexpr int EFFECT_EQ_SERIALIZATION_SIZE = EFFECT_ALGO_EQ_BAND_COUNT * (2 * sizeof(f32) + sizeof(u16));

u32 Effect_EqSerialize(Effect_Eq *fxIn, void *bufOut)
{
    if (fxIn)
    {
        auto bands = fxIn->algoInstance.bands;

        for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
        {
            Effect_Algo_EqBand band = bands[i];

            bufOut += IO_Write_f32(band.gain, bufOut);
            bufOut += IO_Write_f32(band.q, bufOut);

            bufOut += IO_Write_u16(band.freq, bufOut);
        }
    }

    return EFFECT_EQ_SERIALIZATION_SIZE;
}

u32 Effect_EqDeserialize(Effect_Eq *fxOut, void *bufIn)
{
    for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
    {
        Effect_Algo_EqBand band = {};

        bufIn += IO_ReadToPtr_f32(&band.gain, bufIn);
        bufIn += IO_ReadToPtr_f32(&band.q, bufIn);

        bufIn += IO_ReadToPtr_u16(&band.freq, bufIn);

        fxOut->algoInstance.bands[i] = band;
    }

    return EFFECT_EQ_SERIALIZATION_SIZE;
}

void Effect_EqBandUpdate(Effect_Eq *fx, int band, u16 freq, f32 gain, f32 q)
{
    Effect_Algo_EqBandUpdate(fx->algoInstance.bands + band, freq, gain, q);
}

void Effect_EqBandUpdateByFreq(Effect_Eq *fx, u16 searchFreq, f32 gain, f32 q)
{
    for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
    {
        Effect_Algo_EqBand *band = fx->algoInstance.bands + i;
        if (band->freq == searchFreq)
        {
            Effect_Algo_EqBandUpdate(band, searchFreq, gain, q);
            return;
        }
    }

    assert(false);
}
