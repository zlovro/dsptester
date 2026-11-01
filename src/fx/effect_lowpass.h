//
// Created by lovro on 23/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_LOWPASS_H
#define EFFECT_LOWPASS_H

#include <ui/effects.h>
#include <ui/uicontrol.h>

typedef pstruct Effect_Lowpass
{
    EFFECT_HEADER;
    POTS_ARRAY(2, potCutoff);

    UICONTROL_ARRAY(2);
} Effect_Lowpass;

extern Effect_Lowpass Effect_Lowpass_Instances[];
extern EffectArray    Effect_Lowpass_Array;

void Effect_Lowpass_ProcessAudio_Independent(u16 cutoff);

#endif //EFFECT_LOWPASS_H
