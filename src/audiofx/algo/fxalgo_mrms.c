//
// Created by lovro on 22.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_mrms.h"

/*
 * SOURCES:
 * - (22. 11. 2026.) https://youtu.be/q_Anc8KyrXI
 * - (22. 11. 2026.) https://github.com/pms67/IFX-Guitar-DSP/blob/main/firmware/InfiniFX-Reloaded-TikiDrive_Firmware/Core/Src/IFX_NoiseGate.c
 */
void Effect_Algo_MovingRMSInstance_Init(Effect_Algo_MovingRMSInstance *instance, u16 sampleCount)
{
    instance->sampleCount    = sampleCount;
    instance->invSampleCount = 1.0f / (f32) sampleCount;

    instance->counter = 0;

    for (uint16_t n = 0; n < sampleCount; n++)
    {
        instance->in[n] = 0.0f;
    }

    instance->outSq = 0.0f;
}

f32 Effect_Algo_MovingRMSSingleshot(Effect_Algo_MovingRMSInstance *instance, f32 sample)
{
    f32 inSq = sample * sample;

    instance->in[instance->counter] = inSq;

    if (instance->counter == instance->sampleCount - 1)
    {
        instance->counter = 0;
    }
    else
    {
        instance->counter++;
    }

    f32 newOut      = instance->outSq + instance->invSampleCount * (inSq - instance->in[instance->counter]);
    instance->outSq = newOut;

    return newOut;
}
