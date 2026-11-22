//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_noisegate.h"

#include "iohelper.h"

#include <gfx/effect_common_ui.h>

void Effect_NoiseGateInit(Effect_NoiseGate *fx)
{
    Effect_Algo_NoiseGateInstance_Init(&fx->algoInstance, DB_N60, 2, 2, 20);

    Effect_UI_InitDefaultEditScreen(fx->pots, ARRAY_CNT(fx->pots), "THR");
}

void Effect_NoiseGateDraw(Effect_NoiseGate *fx)
{
    Effect_UI_DrawDefaultEditScreen(fx->pots, ARRAY_CNT(fx->pots));
}

void Effect_NoiseGateHandleInput(Effect_NoiseGate *fx)
{
}


void Effect_NoiseGateProcessAudio(Effect_NoiseGate *fx)
{
    Effect_Algo_NoiseGate(&fx->algoInstance);
}

static constexpr int EFFECT_NOISEGATE_SERIALIZATION_SIZE = sizeof(f32);

u32 Effect_NoiseGateSerialize(Effect_NoiseGate *fxIn, void *bufOut)
{
    if (fxIn)
    {
        bufOut += IO_Write_f32(fxIn->algoInstance.thresholdSq, bufOut);
    }

    return EFFECT_NOISEGATE_SERIALIZATION_SIZE;
}

u32 Effect_NoiseGateDeserialize(Effect_NoiseGate *fxOut, void *bufIn)
{
    bufIn += IO_ReadToPtr_f32(&fxOut->algoInstance.thresholdSq, bufIn);

    return EFFECT_NOISEGATE_SERIALIZATION_SIZE;
}
