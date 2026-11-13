//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include <math.h>

#include "effect_distorsion.h"
#include "dsp.h"
#include "iohelper.h"

void Effect_DistorsionInit(Effect_Distorsion *fx)
{
    Effect_Algo_DistorsionInstance_Init(&fx->algoInstance, DB_20, 0.5F, 1.0F);

    Potentiometer_InitMany(fx->pots, EFFECT_POT_COUNT(fx));

    for (int i = 0; i < EFFECT_POT_COUNT(fx); ++i)
    {
        fx->controlPtrs[i] = &fx->pots[i].baseControl;
    }

    fx->common.controlCount = ARRAY_CNT(fx->controlPtrs);
    fx->common.controls     = fx->controlPtrs;

    // for (int i = 0; i < EFFECT_DISTORSION_AA_COUNT; ++i)
    // {
    //     f32 *state = fx->aaStates[i];
    //     Effect_Algo_AAInstance_Init(fx->aaFirs + i, state);
    // }
    //
    // for (int i = 0; i < EFFECT_DISTORSION_HIPASS_COUNT; ++i)
    // {
    //     Effect_Algo_HighpassInstance_Init(fx->hipass + i, 100.0F);
    // }
}

void Effect_DistorsionDraw(Effect_Distorsion *fx)
{
}

void Effect_DistorsionHandleInput(Effect_Distorsion *fx)
{
}

void Effect_DistorsionProcessAudio(Effect_Distorsion *fx)
{
    // Effect_Algo_AA(fx->aaFirs + 0);
    //
    // f32         gain     = fx->potGain.value / (f32) fx->potGain.degreeRange;
    // const float MAX_GAIN = 1000;
    //
    // const float k = 10;
    //
    // for (int i = 0; i < DSP_CHUNK_SIZE; ++i)
    // {
    //     f32 y = DSP_WorkBuf[i] * gain * MAX_GAIN;
    //     y     = clamp(y, -1.0F, 1.0F);
    //
    //     f32 a = expf(k * y);
    //     f32 b = 1 / a;
    //
    //     f32 h = (a - b) / (a + b);
    //
    //     DSP_WorkBuf[i] = 0 ? y : h;
    // }
    //
    // Effect_Algo_AA(fx->aaFirs + 1);
}

static constexpr int EFFECT_DISTORSION_SERIALIZATION_SIZE = 3 * sizeof(f32);

u32 Effect_DistorsionSerialize(Effect_Distorsion *fxIn, void *bufOut)
{
    if (fxIn)
    {
        bufOut += IO_Write_f32(fxIn->algoInstance.gain, bufOut);
        bufOut += IO_Write_f32(fxIn->algoInstance.dist, bufOut);
        bufOut += IO_Write_f32(fxIn->algoInstance.volume, bufOut);
    }

    return EFFECT_DISTORSION_SERIALIZATION_SIZE;
}

u32 Effect_DistorsionDeserialize(Effect_Distorsion *fxOut, void *bufIn)
{
    bufIn += IO_ReadToPtr_f32(&fxOut->algoInstance.gain, bufIn);
    bufIn += IO_ReadToPtr_f32(&fxOut->algoInstance.dist, bufIn);
    bufIn += IO_ReadToPtr_f32(&fxOut->algoInstance.volume, bufIn);

    return EFFECT_DISTORSION_SERIALIZATION_SIZE;
}
