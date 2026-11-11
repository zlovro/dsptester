//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_DISTORSION_H
#define EFFECT_DISTORSION_H

#include "types.h"
#include "effects.h"
#include "ui/uicontrol.h"
#include <fx/algo/fxalgo_xpass.h>

#define EFFECT_DISTORSION_AA_COUNT 2
#define EFFECT_DISTORSION_HIPASS_COUNT 2

typedef pstruct Effect_Distorsion
{
    EFFECT_COMMON_DATA;
    POTS_ARRAY(2, potGain, potDistorsion);

    UICONTROL_ARRAY(2);

    f32 aaStates[EFFECT_DISTORSION_AA_COUNT][EFFECTS_ALGO_AA_TAPCOUNT];
    Effects_Algo_FIRInstance aaFirs[EFFECT_DISTORSION_AA_COUNT];

    Effects_Algo_HighpassInstance hipass[EFFECT_DISTORSION_HIPASS_COUNT];
} Effect_Distorsion;

extern void Effect_Distorsion_Init(Effect_Distorsion *fx);
extern void Effect_Distorsion_Draw(Effect_Distorsion *fx);
extern void Effect_Distorsion_HandleInput(Effect_Distorsion *fx, u8 inputData);
extern void Effect_Distorsion_ProcessAudio(Effect_Distorsion *fx);

#endif //EFFECT_DISTORSION_H
