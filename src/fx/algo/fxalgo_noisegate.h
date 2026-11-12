//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_NOISEGATE_H
#define FXALGO_NOISEGATE_H

#include "types.h"

typedef struct Effect_Algo_NoiseGateInstance
{
    f32 threshold;
} Effect_Algo_NoiseGateInstance;

extern void Effect_Algo_NoiseGateInstance_Init(Effect_Algo_NoiseGateInstance *instance, f32 threshold);
extern void Effect_Algo_NoiseGate(Effect_Algo_NoiseGateInstance *instance);

#endif //FXALGO_NOISEGATE_H
