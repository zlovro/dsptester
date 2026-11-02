//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_distorsion.h"

#include <wchar.h>
#include <math.h>

#include "dsp.h"

void EffectDistorsionInit(EffectDistorsion *fx)
{
    Potentiometer_InitMany(fx->pots, EFFECT_POT_COUNT(fx));

    for (int i = 0; i < EFFECT_POT_COUNT(fx); ++i)
    {
        fx->controlPtrs[i] = &fx->pots[i].baseControl;
    }

    fx->common.controlCount = ARRAY_CNT(fx->controlPtrs);
    fx->common.controls     = fx->controlPtrs;
}

void EffectDistorsionDraw(EffectDistorsion *fx)
{
}

void EffectDistorsionHandleInput(EffectDistorsion *fx, u8 inputData)
{
}

void EffectDistorsionProcessAudio(EffectDistorsion *fx)
{
}
