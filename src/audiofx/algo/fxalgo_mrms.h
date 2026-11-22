//
// Created by lovro on 22.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef FXALGO_MRMS_H
#define FXALGO_MRMS_H

#include <types.h>

/*
 * SOURCES:
 * - (22. 11. 2026.) https://github.com/pms67/IFX-Guitar-DSP/blob/main/firmware/InfiniFX-Reloaded-TikiDrive_Firmware/Core/Inc/IFX_MovingRMS.h
 */

#define EFFECT_ALGO_MRMS_MAX_BUF 2500

typedef struct Effect_Algo_MovingRMSInstance
{
    uint16_t sampleCount;
    uint16_t counter;

    f32 invSampleCount;

    f32 in[EFFECT_ALGO_MRMS_MAX_BUF];
    f32 outSq;
} Effect_Algo_MovingRMSInstance;

extern void Effect_Algo_MovingRMSInstance_Init(Effect_Algo_MovingRMSInstance *instance, u16 sampleCount);
extern f32 Effect_Algo_MovingRMSSingleshot(Effect_Algo_MovingRMSInstance *instance, f32 sample);

#endif //FXALGO_MRMS_H
