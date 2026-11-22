//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_noisegate.h"
#include <dsp.h>
#include <math.h>

void Effect_Algo_NoiseGateInstance_Init(Effect_Algo_NoiseGateInstance *instance, f32 threshold, f32 attackTimeMs, f32 releaseTimeMs, f32 holdTimeMs)
{
    instance->thresholdSq = threshold * threshold;

    instance->holdTimeS = 0.001f * holdTimeMs;

    instance->attackCounter  = 0.0f;
    instance->releaseCounter = 0.0f;
    instance->smoothedGain   = 0.0f;

    Effect_Algo_NoiseGateSetAttackRelease(instance, attackTimeMs, releaseTimeMs);

    // 50ms
    Effect_Algo_MovingRMSInstance_Init(&instance->mrms, (u16) (0.050F * SAMPLE_RATE));
}

void Effect_Algo_NoiseGate(Effect_Algo_NoiseGateInstance *instance)
{
    for (int i = 0; i < DSP_CHUNK_SIZE; ++i)
    {
        f32 inp     = DSP_WorkBuf[i];
        f32 inRmsSq = Effect_Algo_MovingRMSSingleshot(&instance->mrms, inp);

        f32 gain = 1.0f;
        if (inRmsSq < instance->thresholdSq)
        {
            gain = 0.0f;
        }

        // attack
        if (gain <= instance->smoothedGain)
        {
            if (instance->attackCounter > instance->holdTimeS)
            {
                // lowpass gain to minimize audio cuttiinstance
                instance->smoothedGain = instance->attackCoeff * instance->smoothedGain + (1.0f - instance->attackCoeff) * gain;
            }
            else
            {
                instance->attackCounter += SAMPLE_TIME_F;
            }

            instance->releaseCounter = 0.0f;
        }
        else if (gain > instance->smoothedGain)
        {
            if (instance->releaseCounter > instance->holdTimeS)
            {
                // lowpass gain to minimize audio cuttiinstance
                instance->smoothedGain = instance->releaseCoeff * instance->smoothedGain + (1.0f - instance->releaseCoeff) * gain;
            }
            else
            {
                instance->releaseCounter += SAMPLE_TIME_F;
            }

            instance->attackCounter = 0.0f;
        }

        DSP_WorkBuf[i] = inp * instance->smoothedGain;
    }
}
