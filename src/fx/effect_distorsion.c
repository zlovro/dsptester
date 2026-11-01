//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_distorsion.h"

#include <wchar.h>
#include <math.h>

#include "dsp.h"
#include "effect_lowpass.h"

static void Effect_Distorsion_Init(Effect_Distorsion *fx)
{
    Potentiometer_InitMany(fx->pots, EFFECT_POT_COUNT(fx));

    for (int i = 0; i < EFFECT_POT_COUNT(fx); ++i)
    {
        fx->controlPtrs[i] = &fx->pots[i].baseControl;
    }

    fx->header.controlCount = ARRAY_CNT(fx->controlPtrs);
    fx->header.controls     = fx->controlPtrs;
}

static void Effect_Distorsion_Draw(Effect_Distorsion *fx)
{
}

static void Effect_Distorsion_HandleInput(Effect_Distorsion *fx, u8 inputData)
{
}

static void Effect_Distorsion_ProcessAudio(Effect_Distorsion *fx)
{
}

Effect_Distorsion Effect_Distorsion_Instances[8] = {EFFECT_DEFINE(EFFECT_ID_DISTORSION, "Distorsion", Effect_Distorsion_Init, Effect_Distorsion_Draw, Effect_Distorsion_HandleInput, Effect_Distorsion_ProcessAudio)};
EffectArray       Effect_Distorsion_Array        = {ARRAY_CNT(Effect_Distorsion_Instances), sizeof(Effect_Distorsion), &Effect_Distorsion_Instances[0].header};

static f32 DSP_WorkBufBackup[ARRAY_CNT(DSP_WorkBuf)];

void Effect_Distorsion_ProcessAudio_Independent(f32 gain, f32 tone, f32 dist)
{
    Effect_Lowpass_ProcessAudio_Independent(12000);

    memcpy(DSP_WorkBufBackup, DSP_WorkBuf, sizeof(DSP_WorkBufBackup));

    const f32 p = 0.5F;
    const f32 b = 0.8F;
    const f32 k = 5.8F;

    for (int i = 0; i < ARRAY_CNT(DSP_WorkBuf); ++i)
    {
        f32 in = DSP_WorkBuf[i] * gain;
        f32 inAbs = fabs(in);
        f32 out = min(1.0F, inAbs < 0.5F ? inAbs : b * (logf(inAbs) - logf(p)) + p);

        DSP_WorkBuf[i] = copysignf(out, in);
    }

    // Effect_Lowpass_ProcessAudio_Independent(12000);

    // Effect_LastSample = DSP_WorkBuf[ARRAY_CNT(DSP_WorkBuf) - 1];
}
