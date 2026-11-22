//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_NOISEGATE_H
#define FXALGO_NOISEGATE_H

#include <math.h>

#include "dsp.h"
#include "types.h"
#include "fxalgo_mrms.h"

typedef struct Effect_Algo_NoiseGateInstance
{
    Effect_Algo_MovingRMSInstance mrms;

    f32 thresholdSq;
    f32 holdTimeS;

    f32 attackCoeff;
    f32 releaseCoeff;

    f32 attackCounter;
    f32 releaseCounter;

    f32 smoothedGain;
} Effect_Algo_NoiseGateInstance;

extern void Effect_Algo_NoiseGateInstance_Init(Effect_Algo_NoiseGateInstance *instance, f32 threshold, f32 attackTimeMs, f32 releaseTimeMs, f32 holdTimeMs);
extern void Effect_Algo_NoiseGate(Effect_Algo_NoiseGateInstance *instance);

static inline void Effect_Algo_NoiseGateSetAttackRelease(Effect_Algo_NoiseGateInstance *instance, f32 attackMs, f32 releaseMs)
{
    instance->attackCoeff  = expf(-2197.22457734f / (SAMPLE_RATE * attackMs));
    instance->releaseCoeff = expf(-2197.22457734f / (SAMPLE_RATE * releaseMs));
}
#endif //FXALGO_NOISEGATE_H
