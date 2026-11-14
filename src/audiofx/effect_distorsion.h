//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_DISTORSION_H
#define EFFECT_DISTORSION_H

#include "types.h"
#include "effect.h"
#include "gfx/ui_control.h"
#include <audiofx/algo/fxalgo_xpass.h>

#include "algo/fxalgo_ds.h"

#define EFFECT_DISTORSION_AA_COUNT 2
#define EFFECT_DISTORSION_HIPASS_COUNT 2

typedef pstruct Effect_Distorsion
{
    EFFECT_COMMON_DATA(Effect_Algo_DistorsionInstance);
    POTS_ARRAY(2, potGain, potDistorsion);

    UICONTROL_ARRAY(2);
} Effect_Distorsion;

extern void Effect_DistorsionInit(Effect_Distorsion *fx);
extern void Effect_DistorsionDraw(Effect_Distorsion *fx);
extern void Effect_DistorsionHandleInput(Effect_Distorsion *fx);
extern void Effect_DistorsionProcessAudio(Effect_Distorsion *fx);

extern u32 Effect_DistorsionSerialize(Effect_Distorsion *fxIn, void *bufOut);
extern u32 Effect_DistorsionDeserialize(Effect_Distorsion *fxOut, void *bufIn);

#endif //EFFECT_DISTORSION_H
