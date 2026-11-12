//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect.h"

#include <wchar.h>

#include "effect_distorsion.h"
#include "effect_eq.h"

f32 Effect_LastSample = 0;

EffectBlueprint EffectDistorsionBlueprint = EFFECT_BLUEPRINT(Effect_Distorsion, EFFECT_ID_DISTORSION, "Distorsion", Effect_Distorsion_Init, Effect_Distorsion_Draw, Effect_Distorsion_HandleInput, Effect_Distorsion_ProcessAudio);
EffectBlueprint EffectEqBlueprint         = EFFECT_BLUEPRINT(Effect_Eq, EFFECT_ID_EQ, "EQ", Effect_EqInit, Effect_EqDraw, Effect_EqHandleInput, Effect_EqProcessAudio);

Effect_Distorsion Effect_Distorsion_Instances[EFFECTS_MAX_COUNT] = {};
Effect_Eq         EffectEqInstances[EFFECTS_MAX_COUNT]           = {};

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
        EffectId         id        = blueprint->id;

        void *instance = registryEntry.instances;
        for (int j = 0; j < EFFECTS_MAX_COUNT; ++j)
        {
            // effect common data must always be at offset 0x0
            Effect_CommonData *common = instance;

            common->id                 = id;
            common->controlCount       = 0;
            common->controls           = NULL;
            common->selectedControlIdx = 0;

            blueprint->funcInit(instance);

            instance += blueprint->fxSizeBytes;
        }
    }
}

void EffectSelectNextControl(Effect_CommonData *fx)
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

void EffectSelectPreviousControl(Effect_CommonData *fx)
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
