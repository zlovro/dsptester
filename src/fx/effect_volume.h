//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_VOLUME_H
#define EFFECT_VOLUME_H

#include <types.h>
#include <fx/effect.h>
#include <fx/algo/fxalgo_volume.h>

typedef pstruct Effect_Volume
{
    EFFECT_COMMON_DATA;

    POTS_ARRAY(2, potGain, potDistorsion);

    UICONTROL_ARRAY(2);

    Effect_Algo_VolumeInstance algoInstance;
} Effect_Volume;

extern void Effect_VolumeInit(Effect_Volume *fx);
extern void Effect_VolumeDraw(Effect_Volume *fx);
extern void Effect_VolumeHandleInput(Effect_Volume *fx, u8 inputData);
extern void Effect_VolumeProcessAudio(Effect_Volume *fx);

#endif //EFFECT_VOLUME_H
