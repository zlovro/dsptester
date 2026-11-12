//
// Created by lovro on 20/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "dsp.h"

#include <math.h>

#include "fx/effect_distorsion.h"

f32 DSP_WorkBuf[DSP_CHUNK_SIZE] = {};

void DSP_Init()
{
}

void DSP_Process()
{
    Effect_Distorsion *ds = EFFECT_GET(Effect_Distorsion, EFFECT_ID_DISTORSION, 0);

    // Effect_Algo_AA(&fir, DSP_WorkBuf, DSP_CHUNK_SIZE);

    ds->potDistorsion.value = ds->potDistorsion.degreeRange;
    ds->potGain.value       = 100;

    Effect_DistorsionProcessAudio(ds);
}
