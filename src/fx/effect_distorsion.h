//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_DISTORSION_H
#define EFFECT_DISTORSION_H

#include "types.h"
#include "ui/effects.h"
#include "ui/uicontrol.h"

typedef pstruct Effect_Distorsion
{
    EFFECT_HEADER;
    POTS_ARRAY(2, potGain, potDistorsion);

    UICONTROL_ARRAY(2);
} Effect_Distorsion;

extern Effect_Distorsion Effect_Distorsion_Instances[];
extern EffectArray Effect_Distorsion_Array;

void Effect_Distorsion_ProcessAudio_Independent(f32 gain, f32 tone, f32 dist);

#endif //EFFECT_DISTORSION_H
