//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effects.h"

#include <wchar.h>

#include "effect_distorsion.h"
#include "effect_eq.h"

f32 Effect_LastSample = 0;

EffectBlueprint EffectDistorsionBlueprint = EFFECT_BLUEPRINT(EffectDistorsion, EFFECT_ID_DISTORSION, "Distorsion", EffectDistorsionInit, EffectDistorsionDraw, EffectDistorsionHandleInput, EffectDistorsionProcessAudio);
EffectBlueprint EffectEqBlueprint         = EFFECT_BLUEPRINT(EffectEq, EFFECT_ID_EQ, "EQ", EffectEqInit, EffectEqDraw, EffectEqHandleInput, EffectEqProcessAudio);

EffectDistorsion EffectDistorsionInstances[EFFECTS_MAX_COUNT] = {};
EffectEq         EffectEqInstances[EFFECTS_MAX_COUNT]         = {};

EffectRegistryEntry EffectRegistry[] = {
    [EFFECT_ID_DISTORSION] = (EffectRegistryEntry){&EffectDistorsionBlueprint, EffectDistorsionInstances},
    [EFFECT_ID_EQ] = (EffectRegistryEntry){&EffectEqBlueprint, EffectEqInstances},
};

void EffectRegisterEffects()
{
    for (int i = 0; i < EFFECT_COUNT; ++i)
    {
        EffectRegistryEntry registryEntry = EffectRegistry[i];

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
