//
// Created by lovro on 04/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_xpass.h"

#include <math.h>

static f32 mLopassStates[128] = {};
static int mLopassCounter = 0;

void Effects_Algo_Lowpass1(f32 *buf, int n, f32 cutoff)
{
    const float DT = 1.0f / SAMPLE_RATE;

    float rc    = 1.0f / (2.0f * M_PI * cutoff);
    float alpha = DT / (rc + DT);

    mLopassCounter++;
    float prev_output = *state;

    for (int i = 0; i < num_samples; i++)
    {
        float current_sample = audio_data[i];

        // Difference equation: y[n] = y[n-1] + alpha * (x[n] - y[n-1])
        float output = prev_output + alpha * (current_sample - prev_output);

        audio_data[i] = output;
        prev_output   = output;
    }

    // 3. Save state back for continuous streaming buffers
    *state = prev_output;
}

static f32 mHipassStates[128] = {};
static int mHipassCounter = 0;

void Effects_Algo_Highpass1(f32 *buf, int n, f32 cutoff)
{
}
