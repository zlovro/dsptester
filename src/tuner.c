//
// Created by lovro on 23.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "tuner.h"

#include "gfx/st7920.h"

void Tuner_Init()
{

}

void Tuner_Draw()
{
    // TODO: TUNER!
    ST7920_DrawStringCenteredInRect("Tuner\nWIP", 0, 0, ST7920_WIDTH, ST7920_HEIGHT, true, true);
}

void Tuner_Update()
{
}
