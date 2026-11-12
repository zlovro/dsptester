//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_eq.h"

#include <math.h>

#include "dsp.h"

void Effect_Algo_EqInstance_Init(Effect_Algo_EqInstance *instance)
{
    for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
    {
        f32 freq = EFFECT_ALGO_EQ_F0 * powf(EFFECT_ALGO_EQ_F1 / EFFECT_ALGO_EQ_F0, (f32) i / (f32) (EFFECT_ALGO_EQ_BAND_COUNT - 1));

        Effect_Algo_EqBand *band = instance->bands + i;
        Effect_Algo_EqBandUpdate(band, freq, 1.0F, M_1SQ2_F);

        for (int j = 0; j < 3; ++j)
        {
            band->x[j] = band->y[j] = 0;
        }
    }
}

void Effect_Algo_Eq(Effect_Algo_EqInstance *instance)
{
    for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
    {
        Effect_Algo_EqBand *bandPtr = instance->bands + i;
        Effect_Algo_EqBand  band    = *bandPtr;

        f32 a0 = band.a[0];
        f32 a1 = band.a[1];
        f32 a2 = band.a[2];

        f32 b0 = band.b[0];
        f32 b1 = band.b[1];
        f32 b2 = band.b[2];

        f32 xn  = band.x[0];
        f32 xn1 = band.x[1];
        f32 xn2 = band.x[2];

        f32 yn  = band.y[0];
        f32 yn1 = band.y[1];
        f32 yn2 = band.y[2];

        for (int j = 0; j < DSP_CHUNK_SIZE; ++j)
        {
            xn2 = xn1;
            xn1 = xn;
            xn  = DSP_WorkBuf[j];

            yn2 = yn1;
            yn1 = yn;

            f32 suma = a0 * xn + a1 * xn1 + a2 * xn2;
            f32 sumb = b1 * yn1 + b2 * yn2;

            yn = b0 * (suma + sumb);

            DSP_WorkBuf[j] = yn;
        }

        bandPtr->x[0] = xn;
        bandPtr->x[1] = xn1;
        bandPtr->x[2] = xn2;

        bandPtr->y[0] = yn;
        bandPtr->y[1] = yn1;
        bandPtr->y[2] = yn2;
    }
}

void Effect_Algo_EqBandUpdate(Effect_Algo_EqBand *band, u16 freq, f32 gain, f32 q)
{
    band->freq = freq;
    band->gain = gain;
    band->q    = q;

    Effect_Algo_EqBandRecalculate(band);
}

void Effect_Algo_EqBandRecalculate(Effect_Algo_EqBand *band)
{
    f32 g = band->gain;
    f32 q = band->q;

    f32 wc = M_TWOPI_F * (f32) band->freq;

    // pre-warp
    wc = 2.0F / SAMPLE_TIME_F * tanf(wc * SAMPLE_TIME_F / 2.0F);

    f32 wct2 = wc * wc * SAMPLE_TIME_F * SAMPLE_TIME_F;

    f32 basea = 2 * (g / q) * wc * SAMPLE_TIME_F;
    f32 baseb = 2 * (1.0F / q) * wc * SAMPLE_TIME_F;

    band->a[0] = 4 + basea + wct2;
    band->a[1] = 2 * wct2 - 8;
    band->a[2] = 4 - basea + wct2;

    band->b[0] = 1.0F / (4 + baseb + wct2);
    band->b[1] = -(2 * wct2 - 8);
    band->b[2] = -(4 - baseb + wct2);
}
