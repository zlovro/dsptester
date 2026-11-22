//
// Created by lovro on 22.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef EFFECT_COMMON_UI_H
#define EFFECT_COMMON_UI_H

#include <gfx/ui_control.h>

// varargs = labels
void Effect_UI_InitDefaultEditScreen(Potentiometer* pots, int count, ...);
void Effect_UI_DrawDefaultEditScreen(Potentiometer* pots, int count);

#endif //EFFECT_COMMON_UI_H
