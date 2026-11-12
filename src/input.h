//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef INPUT_H
#define INPUT_H

#include <types.h>

typedef enum Input_Direction
{
    INPUT_DIR_L,
    INPUT_DIR_R,
    INPUT_DIR_NULL,
} Input_Direction;

typedef enum Input_ButtonState
{
    INPUT_BTN_DOWN,
    INPUT_BTN_HELD,
    INPUT_BTN_UP,
    INPUT_BTN_NULL,
} Input_ButtonState;

#define INPUT_BTN_COUNT 3

#define INPUT_ALG_PEDAL_DEADZONE 200

typedef struct InputData
{
    u16 algPedalValue;

    Input_Direction   rotaryDir: 2;
    Input_ButtonState rotaryBtn  : 2;

    Input_ButtonState btnNxt : 2;
    Input_ButtonState btnPrev: 2;
    Input_ButtonState btnAux : 2;
} InputData;

extern InputData InputData_Instance;

#endif //INPUT_H
