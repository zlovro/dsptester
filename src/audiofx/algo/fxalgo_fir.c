//
// Created by lovro on 05/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_fir.h"

#include "dsp.h"

void Effect_Algo_FIRInstance_Init(Effect_Algo_FIRInstance *instance, f32 *coeffs, int nCoeff, f32 *state)
{
    instance->coeffs = coeffs;
    instance->nCoeff = nCoeff;
    instance->state  = state;

    instance->head = 0;
}

void Effect_Algo_FIR(Effect_Algo_FIRInstance *fir)
{
    for (size_t s = 0; s < DSP_CHUNK_SIZE; s++)
    {
        float input = DSP_WorkBuf[s];

        fir->state[fir->head] = input;

        float  output = 0.0f;
        size_t index  = fir->head;

        for (size_t i = 0; i < fir->nCoeff; i++)
        {
            output += fir->coeffs[i] * fir->state[index];
            if (index == 0)
            {
                index = fir->nCoeff - 1;
            }
            else
            {
                index--;
            }
        }

        fir->head = (fir->head + 1) % fir->nCoeff;

        DSP_WorkBuf[s] = output;
    }
}
