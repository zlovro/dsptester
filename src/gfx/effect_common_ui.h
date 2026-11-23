//
// Created by lovro on 22.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_COMMON_UI_H
#define EFFECT_COMMON_UI_H

#include <gfx/ui_control.h>

typedef struct Effect_UI_DefaultEditScreen
{
    Potentiometer *pots;
    Potentiometer *active;

    int potCount;

    bool selected;
} Effect_UI_DefaultEditScreen;

// varargs = labels
void Effect_UI_DefaultEditScreen_Init(Effect_UI_DefaultEditScreen *scr, Potentiometer *pots, int count, ...);
void Effect_UI_DefaultEditScreen_Draw(Effect_UI_DefaultEditScreen *scr);
void Effect_UI_DefaultEditScreen_HandleInput(Effect_UI_DefaultEditScreen *scr);

#endif //EFFECT_COMMON_UI_H
