//
// Created by lovro on 04/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_xpass.h"

#include <math.h>

#include "dsp.h"

void Effect_Algo_LowpassInstance_Init(Effect_Algo_LowpassInstance *instance, f32 cutoff)
{
    float omegaC = 2.0f * (float) M_PI * cutoff / SAMPLE_RATE;
    float k      = 1.0f - cosf(omegaC);

    instance->alpha     = sqrtf(k * k + 2.0f * k) - k;
    instance->prevState = 0.0f;
}

void Effect_Algo_Lowpass(Effect_Algo_LowpassInstance *instance)
{
    f32 alpha = instance->alpha;
    f32 prev  = instance->prevState;

    for (size_t i = 0; i < DSP_CHUNK_SIZE; i++)
    {
        float x = DSP_WorkBuf[i];
        float y = prev + alpha * (x - prev);

        DSP_WorkBuf[i] = y;

        prev = y;
    }

    instance->prevState = prev;
}

void Effect_Algo_HighpassInstance_Init(Effect_Algo_HighpassInstance *instance, f32 cutoff)
{
    // used for precision.

    float omegaC = 2.0f * (float) M_PI * cutoff / SAMPLE_RATE;
    float cosW   = cosf(omegaC);

    float num   = cosW - sqrtf((cosW - 1.0f) * (cosW - 3.0f));
    float denom = 4.0f * cosW - 3.0f;

    instance->alpha = num / denom;

    instance->prevStateX = 0.0f;
    instance->prevStateY = 0.0f;
}

void Effect_Algo_Highpass(Effect_Algo_HighpassInstance *instance)
{
    f32 alpha = instance->alpha;

    f32 xPrev = instance->prevStateX;
    f32 yPrev = instance->prevStateY;

    for (size_t i = 0; i < DSP_CHUNK_SIZE; i++)
    {
        float x = DSP_WorkBuf[i];

        float y = alpha * (yPrev + x - xPrev);

        xPrev = x;
        yPrev = y;

        DSP_WorkBuf[i] = y;
    }

    instance->prevStateX = xPrev;
    instance->prevStateY = yPrev;
}

static const f32 Effect_Algo_AACoeffs[];

void Effect_Algo_AAInstance_Init(Effect_Algo_FIRInstance *instance, f32 *state)
{
    Effect_Algo_FIRInstance_Init(instance, Effect_Algo_AACoeffs, EFFECT_ALGO_AA_TAPCOUNT, state);
}

void Effect_Algo_AA(Effect_Algo_FIRInstance *instance)
{
    Effect_Algo_FIR(instance);
}

/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 46875 Hz

* 0 Hz - 10000 Hz
  gain = 1
  desired ripple = 0.25 dB
  actual ripple = 0.1683073899677272 dB

* 12000 Hz - 23437.5 Hz
  gain = 0
  desired attenuation = -60 dB
  actual attenuation = -60.817744020128465 dB

*/

static const f32 Effect_Algo_AACoeffs[EFFECT_ALGO_AA_TAPCOUNT] = {-0.000685536246014548, -0.0030037860835934625, -0.0027918015097060534, 0.0005509853590478026, 0.0029589165534247923, 0.000006180375051065511, -0.0038556190354007386, -0.001124841256227598, 0.004778252678325475, 0.002884430604117462, -0.005419158081127585, -0.005313569571910387, 0.005501074022412672, 0.008389364591568057, -0.004719056408671828, -0.012031596556134722, 0.0027282852443707363, 0.016094974596046127, 0.0008742974155066731, -0.020381146878126265, -0.006619736592658396, 0.02464790232052623, 0.015342217872753791, -0.028628786884620727, -0.028701458989072143, 0.032064952387554885, 0.051002413147167594, -0.03470931157671909, -0.09821991860193698, 0.036381231002510915, 0.31563230367863426, 0.46304683580339057, 0.31563230367863426, 0.036381231002510915, -0.09821991860193698, -0.03470931157671909, 0.051002413147167594, 0.032064952387554885, -0.028701458989072143, -0.028628786884620727, 0.015342217872753791, 0.02464790232052623, -0.006619736592658396, -0.020381146878126265, 0.0008742974155066731, 0.016094974596046127, 0.0027282852443707363, -0.012031596556134722, -0.004719056408671828, 0.008389364591568057, 0.005501074022412672, -0.005313569571910387, -0.005419158081127585, 0.002884430604117462, 0.004778252678325475, -0.001124841256227598, -0.0038556190354007386, 0.000006180375051065511, 0.0029589165534247923, 0.0005509853590478026, -0.0027918015097060534, -0.0030037860835934625, -0.000685536246014548};
