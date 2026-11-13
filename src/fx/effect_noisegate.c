//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_noisegate.h"

#include "iohelper.h"

void Effect_NoiseGateInit(Effect_NoiseGate *fx)
{
    Effect_Algo_NoiseGateInstance_Init(&fx->algoInstance, DB_N80);

    Potentiometer_InitMany(fx->pots, EFFECT_POT_COUNT(fx));

    for (int i = 0; i < EFFECT_POT_COUNT(fx); ++i)
    {
        fx->controlPtrs[i] = &fx->pots[i].baseControl;
    }

    fx->common.controlCount = ARRAY_CNT(fx->controlPtrs);
    fx->common.controls     = fx->controlPtrs;
}

void Effect_NoiseGateDraw(Effect_NoiseGate *fx)
{
}

void Effect_NoiseGateHandleInput(Effect_NoiseGate *fx)
{
}

void Effect_NoiseGateProcessAudio(Effect_NoiseGate *fx)
{
}

static constexpr int EFFECT_NOISEGATE_SERIALIZATION_SIZE = sizeof(f32);

u32 Effect_NoiseGateSerialize(Effect_NoiseGate *fxIn, void *bufOut)
{
    if (fxIn)
    {
        bufOut += IO_Write_f32(fxIn->algoInstance.threshold, bufOut);
    }

    return EFFECT_NOISEGATE_SERIALIZATION_SIZE;
}

u32 Effect_NoiseGateDeserialize(Effect_NoiseGate *fxOut, void *bufIn)
{
    bufIn += IO_ReadToPtr_f32(&fxOut->algoInstance.threshold, bufIn);

    return EFFECT_NOISEGATE_SERIALIZATION_SIZE;
}
