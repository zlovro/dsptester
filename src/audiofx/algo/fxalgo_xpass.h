//
// Created by lovro on 04/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_XPASS_H
#define FXALGO_XPASS_H

#include <types.h>
#include "fxalgo_fir.h"

typedef struct Effect_Algo_LowpassInstance
{
    f32 alpha;
    f32 prevState;
} Effect_Algo_LowpassInstance;

extern void Effect_Algo_LowpassInstance_Init(Effect_Algo_LowpassInstance *instance, f32 cutoff);
extern void Effect_Algo_Lowpass(Effect_Algo_LowpassInstance *instance);

typedef struct Effect_Algo_HighpassInstance
{
    f32 alpha;
    f32 prevStateX, prevStateY;
} Effect_Algo_HighpassInstance;

extern void Effect_Algo_HighpassInstance_Init(Effect_Algo_HighpassInstance *instance, f32 cutoff);
extern void Effect_Algo_Highpass(Effect_Algo_HighpassInstance *instance);

#define EFFECT_ALGO_AA_TAPCOUNT 63

// FIR instance used. not a typo
extern void Effect_Algo_AAInstance_Init(Effect_Algo_FIRInstance* instance, f32 *state);
extern void Effect_Algo_AA(Effect_Algo_FIRInstance* instance);

#endif //FXALGO_XPASS_H
