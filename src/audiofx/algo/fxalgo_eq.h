//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_EQ_H
#define FXALGO_EQ_H

#define EFFECT_ALGO_EQ_F0 50
#define EFFECT_ALGO_EQ_F1 6250
#define EFFECT_ALGO_EQ_BAND_COUNT 4

#include <types.h>

typedef pstruct Effect_Algo_EqBand
{
    f32 q;
    f32 gain;

    f32 a[3];
    f32 b[3];

    // inputs, outputs
    f32 x[3], y[3];

    u16 freq;

    char freqLabel[4];
} Effect_Algo_EqBand;

typedef pstruct Effect_Algo_EqInstance
{
    Effect_Algo_EqBand bands[EFFECT_ALGO_EQ_BAND_COUNT];
} Effect_Algo_EqInstance;

extern void Effect_Algo_EqInstance_Init(Effect_Algo_EqInstance *instance);
extern void Effect_Algo_Eq(Effect_Algo_EqInstance *instance);

extern void Effect_Algo_EqBandUpdate(Effect_Algo_EqBand *band, u16 freq, f32 gain, f32 q);
extern void Effect_Algo_EqBandRecalculate(Effect_Algo_EqBand *band);

#endif //FXALGO_EQ_H
