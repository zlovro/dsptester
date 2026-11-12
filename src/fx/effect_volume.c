//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_volume.h"

void Effect_VolumeInit(Effect_Volume *fx)
{
    Potentiometer_InitMany(fx->pots, EFFECT_POT_COUNT(fx));

    for (int i = 0; i < EFFECT_POT_COUNT(fx); ++i)
    {
        fx->controlPtrs[i] = &fx->pots[i].baseControl;
    }

    fx->common.controlCount = ARRAY_CNT(fx->controlPtrs);
    fx->common.controls     = fx->controlPtrs;
}

void Effect_VolumeDraw(Effect_Volume *fx)
{
}

void Effect_VolumeHandleInput(Effect_Volume *fx, u8 inputData)
{
}

void Effect_VolumeProcessAudio(Effect_Volume *fx)
{
}