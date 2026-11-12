//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_NOISEGATE_H
#define EFFECT_NOISEGATE_H

#include "effect.h"
#include "types.h"
#include "algo/fxalgo_noisegate.h"

typedef pstruct Effect_NoiseGate
{
    EFFECT_COMMON_DATA;

    POTS_ARRAY(2, potGain, potDistorsion);

    UICONTROL_ARRAY(2);

    Effect_Algo_NoiseGateInstance algoInstance;
} Effect_NoiseGate;

extern void Effect_NoiseGateInit(Effect_NoiseGate *fx);
extern void Effect_NoiseGateDraw(Effect_NoiseGate *fx);
extern void Effect_NoiseGateHandleInput(Effect_NoiseGate *fx, u8 inputData);
extern void Effect_NoiseGateProcessAudio(Effect_NoiseGate *fx);

#endif //EFFECT_NOISEGATE_H
