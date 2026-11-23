//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_volume.h"

#include "iohelper.h"
#include <gfx/effect_common_ui.h>

static Effect_UI_DefaultEditScreen Screen;

void Effect_VolumeInit(Effect_Volume *fx)
{
    Effect_Algo_VolumeInstance_Init(&fx->algoInstance, 1.0F);

    Effect_UI_DefaultEditScreen_Init(&Screen, fx->pots, ARRAY_CNT(fx->pots), "VOL");
}

void Effect_VolumeDraw(Effect_Volume *fx)
{
    Effect_UI_DefaultEditScreen_Draw(&Screen);
}

void Effect_VolumeHandleInput(Effect_Volume *fx)
{
    Effect_UI_DefaultEditScreen_HandleInput(&Screen);
}

void Effect_VolumeProcessAudio(Effect_Volume *fx)
{
}

static constexpr int EFFECT_VOLUME_SERIALIZATION_SIZE = sizeof(f32);

u32 Effect_VolumeSerialize(Effect_Volume *fxIn, void *bufOut)
{
    if (fxIn)
    {
        bufOut += IO_Write_f32(fxIn->algoInstance.volume, bufOut);
    }

    return EFFECT_VOLUME_SERIALIZATION_SIZE;
}

u32 Effect_VolumeDeserialize(Effect_Volume *fxOut, void *bufIn)
{
    bufIn += IO_ReadToPtr_f32(&fxOut->algoInstance.volume, bufIn);

    return EFFECT_VOLUME_SERIALIZATION_SIZE;
}
