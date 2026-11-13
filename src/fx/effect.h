//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECTS_H
#define EFFECTS_H

#include <types.h>
#include <ui/ui_control.h>

enum EffectId : u16
{
    EFFECT_ID_DISTORSION,
    EFFECT_ID_EQ,
    EFFECT_ID_NOISEGATE,
    EFFECT_ID_VOLUME,
    EFFECT_COUNT,

    EFFECT_ID_INVALID = 0xFFFF
};

typedef enum EffectId EffectId;

// all 4 functions: func(void* fxInstance)
typedef void (*FuncEffectInit)(void *);
typedef void (*FuncEffectDraw)(void *);
typedef void (*FuncEffectHandleInput)(void *);
typedef void (*FuncEffectProcessAudio)(void *);

// 2 functions: u32 func(void* fxInstance, void* buf) - returns size, if fxInstance is NULL return the size of bytes to write
typedef u32 (* FuncEffectSerialize)(void *, void *);
typedef u32 (*FuncEffectDeserialize)(void *, void *);

typedef struct Effect_CommonData
{
    EffectId id;

    UIControl **controls;
    u8          selectedControlIdx;
    u8          controlCount;
} Effect_CommonData;

typedef struct Effect_Blueprint
{
    // corresponding instance size in bytes
    u32 instanceSize;

    FuncEffectInit         funcInit;
    FuncEffectDraw         funcDraw;
    FuncEffectProcessAudio funcProcessAudio;
    FuncEffectHandleInput  funcHandleInput;

    FuncEffectSerialize   funcSerialize;
    FuncEffectDeserialize funcDeserialize;

    char abbrev[4];

    EffectId id;
} Effect_Blueprint;

#define EFFECT_COMMON_DATA(algoType) Effect_CommonData common; algoType algoInstance
#define EFFECT_BLUEPRINT(_type, _id, _abbrev) \
    (Effect_Blueprint){ \
        .instanceSize = sizeof(_type), \
        .id = _id, \
        .abbrev = _abbrev, \
        .funcDraw = (FuncEffectDraw) (_type ## Draw), \
        .funcHandleInput =(FuncEffectHandleInput) (_type ## HandleInput), \
        .funcInit = (FuncEffectInit) (_type ## Init), \
        .funcProcessAudio = (FuncEffectProcessAudio) (_type ## ProcessAudio), \
        .funcSerialize = (FuncEffectSerialize) (_type ## Serialize), \
        .funcDeserialize = (FuncEffectDeserialize) (_type ## Deserialize), \
    }

#define EFFECT_POT_COUNT(fx) (sizeof(fx->pots) / sizeof(Potentiometer))

#define EFFECT_GET(t, id, idx) (((struct t*)((Effect_Registry + id)->instances)) + idx)
#define EFFECT_GET0(t, id) EFFECT_GET(t, id, 0)

void Effect_RegisterEffects();

void EffectSelectNextControl(Effect_CommonData *fx);
void EffectSelectPreviousControl(Effect_CommonData *fx);

typedef struct EffectRegistryEntry
{
    Effect_Blueprint *blueprint;
    void *            instances;
} Effect_RegistryEntry;

extern bool Effect_RegistryDone;

extern Effect_RegistryEntry Effect_Registry[];
extern f32                  Effect_LastSample;

#endif //EFFECTS_H
