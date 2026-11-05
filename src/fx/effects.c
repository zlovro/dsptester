//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effects.h"

#include <wchar.h>

#include "effect_distorsion.h"
#include "effect_eq.h"

f32 Effect_LastSample = 0;

EffectBlueprint EffectDistorsionBlueprint = EFFECT_BLUEPRINT(Effect_Distorsion, EFFECT_ID_DISTORSION, "Distorsion", Effect_Distorsion_Init, Effect_Distorsion_Draw, Effect_Distorsion_HandleInput, Effect_Distorsion_ProcessAudio);
EffectBlueprint EffectEqBlueprint         = EFFECT_BLUEPRINT(EffectEq, EFFECT_ID_EQ, "EQ", EffectEqInit, EffectEqDraw, EffectEqHandleInput, EffectEqProcessAudio);

Effect_Distorsion Effect_Distorsion_Instances[EFFECTS_MAX_COUNT] = {};
EffectEq         EffectEqInstances[EFFECTS_MAX_COUNT]         = {};

EffectRegistryEntry Effect_Registry[] = {
    [EFFECT_ID_DISTORSION] = (EffectRegistryEntry){&EffectDistorsionBlueprint, Effect_Distorsion_Instances},
    [EFFECT_ID_EQ] = (EffectRegistryEntry){&EffectEqBlueprint, EffectEqInstances},
};

void Effect_RegisterEffects()
{
    for (int i = 0; i < EFFECT_COUNT; ++i)
    {
        EffectRegistryEntry registryEntry = Effect_Registry[i];

        EffectBlueprint *blueprint = registryEntry.blueprint;

        void *instance = registryEntry.instances;
        for (int j = 0; j < EFFECTS_MAX_COUNT; ++j)
        {
            blueprint->funcInit(instance);

            instance += blueprint->fxSizeBytes;
        }
    }
}

void EffectSelectNextControl(EffectCommonData *fx)
{
    u8 idx = fx->selectedControlIdx;

    fx->controls[idx]->isSelected = false;

    idx++;
    if (idx >= fx->controlCount)
    {
        idx = 0;
    }

    fx->selectedControlIdx        = idx;
    fx->controls[idx]->isSelected = true;
}

void EffectSelectPreviousControl(EffectCommonData *fx)
{
    int idx = fx->selectedControlIdx;

    fx->controls[idx]->isSelected = false;

    idx--;
    if (idx < 0)
    {
        idx = fx->controlCount - 1;
    }

    fx->selectedControlIdx        = idx;
    fx->controls[idx]->isSelected = true;
}
