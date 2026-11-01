//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"
#include "uicontrol.h"

typedef enum : u8
{
    ROTARY_ENCODER_DIR_NULL,
    ROTARY_ENCODER_DIR_POS,
    ROTARY_ENCODER_DIR_NEG,
} RotaryEncoder_Direction;

#define ROTARY_ENCODER_BTN_PRESSED (1 << 2)

typedef enum : u16
{
    EFFECT_ID_DISTORSION,
    EFFECT_ID_EQ,
    EFFECT_ID_LOWPASS,
    EFFECT_COUNT,
} EffectHeader_Id;

typedef void (*FuncEffectInit)(void *);
typedef void (*FuncEffectDraw)(void *);
typedef void (*FuncEffectHandleInput)(void *, u8);
typedef void (*FuncEffectProcessAudio)(void *);

typedef struct EffectHeader
{
    const char *name;

    FuncEffectInit         funcInit;

    FuncEffectDraw         funcDraw;
    FuncEffectProcessAudio funcProcessAudio;

    // xxxx B DIR
    // B = button pressed, DIR = RotaryEncoder_Direction
    FuncEffectHandleInput funcHandleInput;

    UIControl **controls;

    EffectHeader_Id id;

    u8 selectedControlIdx;
    u8 controlCount;
} EffectHeader;

#define EFFECT_DEFINE(_id, _name, fnInit, fnDraw, fnHandleInput, fnProcessAudio) \
{ \
    .header = (EffectHeader){ \
        .id = _id, \
        .name = _name, \
        .funcDraw = (FuncEffectDraw) fnDraw, \
        .funcHandleInput =(FuncEffectHandleInput) fnHandleInput, \
        .funcInit = (FuncEffectInit) fnInit, \
        .funcProcessAudio = (FuncEffectProcessAudio) fnProcessAudio, \
    } \
}

#define EFFECT_HEADER EffectHeader header;
#define EFFECT_POT_COUNT(fx) (sizeof(fx->pots) / sizeof(Potentiometer))

typedef struct EffectArray
{
    int           count;
    int           fxSize;
    EffectHeader *array;
} EffectArray;

void Effect_RegisterEffects();

void Effect_SelectNextControl(EffectHeader *fx);
void Effect_SelectPreviousControl(EffectHeader *fx);

extern EffectArray *Effect_AllEffects[EFFECT_COUNT];
extern f32 Effect_LastSample;

#endif //EFFECTS_H
