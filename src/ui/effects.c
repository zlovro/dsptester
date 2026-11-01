//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effects.h"

#include <wchar.h>

#include "../fx/effect_distorsion.h"
#include "../fx/effect_eq.h"
#include "../fx/effect_lowpass.h"

f32 Effect_LastSample = 0;

EffectArray *Effect_AllEffects[EFFECT_COUNT] = {
    [EFFECT_ID_DISTORSION] = &Effect_Distorsion_Array,
    [EFFECT_ID_EQ] = &Effect_Eq_Array,
    [EFFECT_ID_LOWPASS] = &Effect_Lowpass_Array,
};

void Effect_RegisterEffects()
{
    for (int i = 0; i < EFFECT_COUNT; ++i)
    {
        EffectArray* arr = Effect_AllEffects[i];
        arr->array->funcInit(arr->array);

        for (int j = 1; j < arr->count; ++j)
        {
            memcpy(arr->array + j, arr, arr->fxSize);
        }
    }
}

void Effect_SelectNextControl(EffectHeader *fx)
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

void Effect_SelectPreviousControl(EffectHeader *fx)
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
