//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_DISTORSION_H
#define EFFECT_DISTORSION_H

#include "types.h"
#include "effects.h"
#include "ui/uicontrol.h"

typedef pstruct Effect_Distorsion
{
    EFFECT_COMMON_DATA;
    POTS_ARRAY(2, potGain, potDistorsion);

    UICONTROL_ARRAY(2);
} EffectDistorsion;

extern void EffectDistorsionInit(EffectDistorsion *fx);
extern void EffectDistorsionDraw(EffectDistorsion *fx);
extern void EffectDistorsionHandleInput(EffectDistorsion *fx, u8 inputData);
extern void EffectDistorsionProcessAudio(EffectDistorsion *fx);

#endif //EFFECT_DISTORSION_H
