//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_EQ_H
#define EFFECT_EQ_H

#include "types.h"
#include "effect.h"

#include <audiofx/algo/fxalgo_eq.h>

typedef pstruct Effect_Eq
{
    EFFECT_COMMON_DATA(Effect_Algo_EqInstance);
} Effect_Eq;

#define EFFECT_EQ_MAX_DB 60
#define EFFECT_EQ_MIN_DB (-60)
#define EFFECT_EQ_DELTA_DB 20
#define EFFECT_EQ_DB_LINE_COUNT (1 + (EFFECT_EQ_MAX_DB - EFFECT_EQ_MIN_DB))

extern void Effect_EqInit(Effect_Eq *fx);
extern void Effect_EqDraw(Effect_Eq *fx);
extern void Effect_EqHandleInput(Effect_Eq *fx);
extern void Effect_EqProcessAudio(Effect_Eq *fx);

extern u32 Effect_EqSerialize(Effect_Eq *fxIn, void *bufOut);
extern u32 Effect_EqDeserialize(Effect_Eq *fxOut, void *bufIn);

extern void Effect_EqBandUpdate(Effect_Eq *fx, int band, u16 freq, f32 gain, f32 q);
extern void Effect_EqBandUpdateByFreq(Effect_Eq *fx, u16 searchFreq, f32 gain, f32 q);

#endif //EFFECT_EQ_H
