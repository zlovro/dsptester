//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_EQ_H
#define EFFECT_EQ_H

#include "types.h"
#include "effects.h"

#define EFFECT_EQ_F0 50
#define EFFECT_EQ_F1 6400
#define EFFECT_EQ_BAND_COUNT 8

typedef pstruct EffectEqBand
{
    f32 q;
    u16 freq;
} EffectEqBand;

typedef pstruct Effect_Eq
{
    EFFECT_COMMON_DATA;

    EffectEqBand bands[EFFECT_EQ_BAND_COUNT];
} EffectEq;

extern void EffectEqInit(EffectEq *eq);
extern void EffectEqDraw(EffectEq *eq);
extern void EffectEqHandleInput(EffectEq *eq, u8 inputData);
extern void EffectEqProcessAudio(EffectEq *eq);

#endif //EFFECT_EQ_H
