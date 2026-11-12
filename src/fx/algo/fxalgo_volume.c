//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_volume.h"

#include "dsp.h"

void Effect_Algo_VolumeInstance_Init(Effect_Algo_VolumeInstance *instance, f32 volume)
{
    instance->volume = volume;
}

void Effect_Algo_Volume(Effect_Algo_VolumeInstance *instance)
{
    f32 vol = instance->volume;

    for (int i = 0; i < DSP_CHUNK_SIZE; ++i)
    {
        DSP_WorkBuf[i] *= vol;
    }
}
