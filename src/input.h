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

/**
 * bits: CP (current | previous)
 * (curr << 1) | prev
 *
 * C | P | Y
 * 0 | 0 | NULL
 * 0 | 1 | UP
 * 1 | 0 | DOWN
 * 1 | 1 | HELD
 */
typedef enum Input_ButtonState
{
    INPUT_BTN_NULL = 0b00,
    INPUT_BTN_UP   = 0b01,
    INPUT_BTN_DOWN = 0b10,
    INPUT_BTN_HELD = 0b11,
} Input_ButtonState;

#define INPUT_BTN_COUNT 3
#define INPUT_ALG_PEDAL_DEADZONE 200
#define INPUT_ROTARY_FLIP 1

typedef struct InputData
{
    u16 algPedalValue;

    Input_Direction   rotaryDir: 2;
    Input_ButtonState rotaryBtn: 2;

    Input_ButtonState btnNxt : 2;
    Input_ButtonState btnPrev: 2;
    Input_ButtonState btnAux : 2;
} InputData;

typedef struct InputData_Raw
{
    bool btnNxt : 1;
    bool btnPrev: 1;
    bool btnAux : 1;

    bool rotaryA  : 1;
    bool rotaryB  : 1;
    bool rotaryBtn: 1;
} InputData_Raw;

extern InputData InputData_Data;

// converts values from InputData_DataRaw to InputData_Data
extern void Input_Handle(InputData_Raw* inputRawIn);

#endif //INPUT_H
