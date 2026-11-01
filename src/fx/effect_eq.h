//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_EQ_H
#define EFFECT_EQ_H

#include "types.h"
#include "ui/effects.h"

#define EFFECT_EQ_F0 50
#define EFFECT_EQ_F1 6400
#define EFFECT_EQ_BAND_COUNT 8

typedef pstruct Effect_Eq_Band
{
    float q;
    u16   freq;
} Effect_Eq_Band;

typedef pstruct Effect_Eq
{
    EFFECT_HEADER;

    Effect_Eq_Band bands[EFFECT_EQ_BAND_COUNT];
} Effect_Eq;

extern Effect_Eq   Effect_Eq_Instances[];
extern EffectArray Effect_Eq_Array;

typedef struct Effect_Eq_Band_Control
{
    UIControl base;
} Effect_Eq_Band_Control;

#endif //EFFECT_EQ_H
