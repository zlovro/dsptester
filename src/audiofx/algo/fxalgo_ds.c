//
// Created by lovro on 13/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_ds.h"

#include <dsp.h>

void Effect_Algo_DistorsionInstance_Init(Effect_Algo_DistorsionInstance *instance, f32 gain, f32 dist, f32 volume)
{
    instance->gain = gain;
    instance->dist = dist;
    instance->volume = volume;
}

void Effect_Algo_Distorsion(Effect_Algo_DistorsionInstance *instance)
{
    f32 vol = instance->volume;

    for (int i = 0; i < DSP_CHUNK_SIZE; ++i)
    {
        DSP_WorkBuf[i] *= vol;
    }
}
