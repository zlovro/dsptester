//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECTS_H
#define EFFECTS_H

#include <types.h>
#include <ui/uicontrol.h>

typedef enum : u16
{
    EFFECT_ID_DISTORSION,
    EFFECT_ID_EQ,
    EFFECT_COUNT,
} EffectId;

// (fxInstance*)
typedef void (*FuncEffectInit)(void *);

// (fxInstance*)
typedef void (*FuncEffectDraw)(void *);

// (fxInstance*, inputData)
typedef void (*FuncEffectHandleInput)(void *, u8);

// (fxInstance*)
typedef void (*FuncEffectProcessAudio)(void *);

typedef struct EffectCommonData
{
    EffectId id;

    UIControl **controls;
    u8          selectedControlIdx;
    u8          controlCount;
} EffectCommonData;

typedef struct EffectBlueprint
{
    const char *name;

    EffectId id;

    // corresponding instance size in bytes
    u32 fxSizeBytes;

    FuncEffectInit funcInit;

    FuncEffectDraw         funcDraw;
    FuncEffectProcessAudio funcProcessAudio;

    // xxxx B DIR
    // B = button pressed, DIR = RotaryEncoder_Direction
    FuncEffectHandleInput funcHandleInput;
} EffectBlueprint;

#define EFFECT_COMMON_DATA EffectCommonData common
#define EFFECT_BLUEPRINT(_type, _id, _name, fnInit, fnDraw, fnHandleInput, fnProcessAudio) \
    (EffectBlueprint){ \
        .fxSizeBytes = sizeof(_type), \
        .id = _id, \
        .name = _name, \
        .funcDraw = (FuncEffectDraw) fnDraw, \
        .funcHandleInput =(FuncEffectHandleInput) fnHandleInput, \
        .funcInit = (FuncEffectInit) fnInit, \
        .funcProcessAudio = (FuncEffectProcessAudio) fnProcessAudio, \
    }

#define EFFECT_POT_COUNT(fx) (sizeof(fx->pots) / sizeof(Potentiometer))

void Effect_RegisterEffects();
#define EFFECT_GET(t, id, idx) (((t*)((Effect_Registry + id)->instances)) + idx)

void EffectSelectNextControl(EffectCommonData *fx);
void EffectSelectPreviousControl(EffectCommonData *fx);

typedef struct EffectRegistryEntry
{
    EffectBlueprint *blueprint;
    void *          instances;
} EffectRegistryEntry;

extern EffectRegistryEntry Effect_Registry[];
extern f32                 Effect_LastSample;

#define EFFECTS_MAX_COUNT 6

#endif //EFFECTS_H
