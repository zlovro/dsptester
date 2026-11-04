//
// Created by lovro on 04/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_XPASS_H
#define FXALGO_XPASS_H

#include <types.h>
#include <dsp.h>

void Effects_Algo_Lowpass1(f32* buf, int n, f32 cutoff);
void Effects_Algo_Highpass1(f32* buf, int n, f32 cutoff);

#endif //FXALGO_XPASS_H
