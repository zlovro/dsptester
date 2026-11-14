//
// Created by lovro on 05/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_FIR_H
#define FXALGO_FIR_H

#include <types.h>

typedef struct
{
    f32 *coeffs;
    int  nCoeff;
    f32 *state;
    int  head;
} Effect_Algo_FIRInstance;

extern void Effect_Algo_FIRInstance_Init(Effect_Algo_FIRInstance *instance, f32 *coeffs, int nCoeff, f32 *state);
extern void Effect_Algo_FIR(Effect_Algo_FIRInstance *instance);

#endif //FXALGO_FIR_H
