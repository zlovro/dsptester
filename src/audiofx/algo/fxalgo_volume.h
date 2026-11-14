//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_VOLUME_H
#define FXALGO_VOLUME_H

#include <types.h>

typedef struct Effect_Algo_VolumeInstance
{
    f32 volume;
} Effect_Algo_VolumeInstance;

extern void Effect_Algo_VolumeInstance_Init(Effect_Algo_VolumeInstance *instance, f32 volume);
extern void Effect_Algo_Volume(Effect_Algo_VolumeInstance *instance);

#endif //FXALGO_VOLUME_H
