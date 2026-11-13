//
// Created by lovro on 13/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_DS_H
#define FXALGO_DS_H

#include <types.h>

typedef struct Effect_Algo_DistorsionInstance
{
    f32 gain;
    f32 dist; // dry/wet
    f32 volume;
} Effect_Algo_DistorsionInstance;

extern void Effect_Algo_DistorsionInstance_Init(Effect_Algo_DistorsionInstance *instance, f32 gain, f32 dist, f32 volume);
extern void Effect_Algo_Distorsion(Effect_Algo_DistorsionInstance *instance);

#endif //FXALGO_DS_H
