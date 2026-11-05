//
// Created by lovro on 04/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_XPASS_H
#define FXALGO_XPASS_H

#include <types.h>
#include "fxalgo_fir.h"

typedef struct Effects_Algo_LowpassInstance
{
    f32 alpha;
    f32 prevState;
} Effects_Algo_LowpassInstance;

extern void Effects_Algo_LowpassInstance_Init(Effects_Algo_LowpassInstance *instance, f32 cutoff);
extern void Effects_Algo_Lowpass(Effects_Algo_LowpassInstance *instance, f32 *buf, int n);

typedef struct Effects_Algo_HighpassInstance
{
    f32 alpha;
    f32 prevStateX, prevStateY;
} Effects_Algo_HighpassInstance;

extern void Effects_Algo_HighpassInstance_Init(Effects_Algo_HighpassInstance *instance, f32 cutoff);
extern void Effects_Algo_Highpass(Effects_Algo_HighpassInstance *instance, f32 *buf, int n);

#define EFFECTS_ALGO_AA_TAPCOUNT 63

// FIR instance used. not a typo
extern void Effects_Algo_AAInstance_Init(Effects_Algo_FIRInstance* instance, f32 *state);
extern void Effects_Algo_AA(Effects_Algo_FIRInstance* instance, f32* buf, int n);

#endif //FXALGO_XPASS_H
