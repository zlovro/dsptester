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
    EFFECT_COMMON_DATA(Effect_Algo_VolumeInstance);

    POTS_ARRAY(2, potGain, potDistorsion);

    UICONTROL_ARRAY(2);
} Effect_Volume;

extern void Effect_VolumeInit(Effect_Volume *fx);
extern void Effect_VolumeDraw(Effect_Volume *fx);
extern void Effect_VolumeHandleInput(Effect_Volume *fx);
extern void Effect_VolumeProcessAudio(Effect_Volume *fx);

extern u32 Effect_VolumeSerialize(Effect_Volume *fxIn, void *bufOut);
extern u32 Effect_VolumeDeserialize(Effect_Volume *fxOut, void *bufIn);

#endif //EFFECT_VOLUME_H
