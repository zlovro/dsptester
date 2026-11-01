//
// Created by lovro on 20/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "dsp.h"

#include <math.h>

#include "fx/effect_distorsion.h"

f32 DSP_WorkBuf[DSP_CHUNK_SIZE] = {};

static f32 sigmoid(f32 x, f32 l, f32 m, f32 a)
{
    return a / (1.0F + powf(M_E, -l * (x - m)));
}

void DSP_Process()
{
    Effect_Distorsion_ProcessAudio_Independent(25.0F, 0.3F, 1.0F);
}
