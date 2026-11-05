//
// Created by lovro on 05/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "fxalgo_fir.h"

void Effects_Algo_FIRInstance_Init(Effects_Algo_FIRInstance *instance, f32 *coeffs, int nCoeff, f32 *state)
{
    instance->coeffs = coeffs;
    instance->nCoeff = nCoeff;
    instance->state  = state;

    instance->head = 0;
}

void Effects_Algo_FIR(Effects_Algo_FIRInstance *fir, f32 *buf, int n)
{
    for (size_t s = 0; s < n; s++)
    {
        float input = buf[s];

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

        buf[s] = output;
    }
}
