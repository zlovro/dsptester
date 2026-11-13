//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect.h"

#include <wchar.h>

#include "effect_distorsion.h"
#include "effect_eq.h"
#include "effect_noisegate.h"
#include "effect_volume.h"
#include "pedalboard.h"

f32 Effect_LastSample = 0;

#define EFFECT_REGISTER_BLUEPRINT_EX(t, id, abbrev, n) Effect_Blueprint t ## Blueprint = EFFECT_BLUEPRINT(t, id, abbrev); t t ## Instances[n] = {}
#define EFFECT_REGISTER_BLUEPRINT(t, id, abbrev) EFFECT_REGISTER_BLUEPRINT_EX(t, id, abbrev, PEDALBOARD_PEDAL_COUNT)
#define EFFECT_REGISTER(t, id) [id] = (Effect_RegistryEntry){&t ## Blueprint, t ## Instances}

EFFECT_REGISTER_BLUEPRINT(Effect_Distorsion, EFFECT_ID_DISTORSION, "DS");
EFFECT_REGISTER_BLUEPRINT(Effect_Eq, EFFECT_ID_EQ, "EQ");
EFFECT_REGISTER_BLUEPRINT(Effect_NoiseGate, EFFECT_ID_NOISEGATE, "GAT");
EFFECT_REGISTER_BLUEPRINT(Effect_Volume, EFFECT_ID_VOLUME, "VOL");

Effect_RegistryEntry Effect_Registry[] = {
    EFFECT_REGISTER(Effect_Distorsion, EFFECT_ID_DISTORSION),
    EFFECT_REGISTER(Effect_Eq, EFFECT_ID_EQ),
    EFFECT_REGISTER(Effect_NoiseGate, EFFECT_ID_NOISEGATE),
    EFFECT_REGISTER(Effect_Volume, EFFECT_ID_VOLUME),
};

bool Effect_RegistryDone = false;

void Effect_RegisterEffects()
{
    for (int i = 0; i < EFFECT_COUNT; ++i)
    {
        Effect_RegistryEntry registryEntry = Effect_Registry[i];

        Effect_Blueprint *blueprint = registryEntry.blueprint;
        EffectId          id        = blueprint->id;

        void *instance = registryEntry.instances;
        for (int j = 0; j < PEDALBOARD_PEDAL_COUNT; ++j)
        {
            // effect common data must always be at offset 0x0
            Effect_CommonData *common = instance;

            common->id                 = id;
            common->controlCount       = 0;
            common->controls           = NULL;
            common->selectedControlIdx = 0;

            blueprint->funcInit(instance);

            instance += blueprint->instanceSize;
        }
    }

    Effect_RegistryDone = true;
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
