//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "input.h"

#include "pedalboard.h"
#include "gfx/ui_common.h"

InputData            InputData_Data        = {};
static InputData_Raw InputData_DataRawPrev = {};

// (curr, prev) --> Input_ButtonState
#define INPUT_MERGE_BTN_STATE(curr, prev) ((Input_ButtonState)((curr) << 1) | (prev))

void Input_Handle(InputData_Raw inputRawIn)
{
    InputData_Raw prev = InputData_DataRawPrev;

    InputData_Data.btnAux    = INPUT_MERGE_BTN_STATE(inputRawIn.btnAux, prev.btnAux);
    InputData_Data.btnNxt    = INPUT_MERGE_BTN_STATE(inputRawIn.btnNxt, prev.btnNxt);
    InputData_Data.btnPrev   = INPUT_MERGE_BTN_STATE(inputRawIn.btnPrev, prev.btnPrev);
    InputData_Data.rotaryBtn = INPUT_MERGE_BTN_STATE(inputRawIn.rotaryBtn, prev.rotaryBtn);

    #if IS_EMU == 0

    // not actually a button but has the same DOWN, UP etc. states
    Input_ButtonState rotaryAState = INPUT_MERGE_BTN_STATE(curr.rotaryA, prev.rotaryA);
    // Input_ButtonState rotaryBState = INPUT_MERGE_BTN_STATE(curr.rotaryB, prev.rotaryB);

    Input_Direction rotaryDir = INPUT_DIR_NULL;

    if (!curr.rotaryB)
    {
        // A is rising and B is 0
        if (rotaryAState == INPUT_BTN_DOWN)
        {
            rotaryDir = INPUT_DIR_L;
        }
        // A is falling and B is 0
        else if (rotaryAState == INPUT_BTN_UP)
            rotaryDir = INPUT_DIR_R; {
        }

        /*
         * XOR is equivalent to a conditional NOT.
         * if INPUT_ROTARY_FLIP is 1, the LSB of rotaryDir will be flipped.
         *
         * Since DIR_L = 0, DIR_R = 1, DIR_NULL = 2, flipping the LSB will change L to R
         * and vice versa without affecting the state of rotaryDir if rotaryDir == DIR_NULL
         */

        rotaryDir ^= INPUT_ROTARY_FLIP;
    }

    InputData_Data.rotaryDir = rotaryDir;
    #endif

    // TODO: change this to read from ADC on STM32

    #if IS_EMU
    InputData_Data.algPedalValue = 0;
    #else
    #endif

    InputData_DataRawPrev = inputRawIn;

    switch (UI_CurrentScreenId)
    {
        case UI_SCR_PEDALBOARD:
        {
            Pedalboard_HandleInput();
            break;
        }
        case UI_SCR_PEDAL_EDIT:
        {
            // this will never be null, the active pedal must be non-NULL in order to go from UI_SCR_PEDALBOARD --> UI_SCR_PEDAL_EDIT
            Effect_CommonData *pedal = Pedalboard_Pedals[Pedalboard_SelectedPedalIndex];
            Effect_Blueprint * bp    = Effect_Registry[pedal->id].blueprint;

            bp->funcDraw(pedal);
            break;
        }
        case UI_SCR_OPTIONS:
        {
            break;
        }

        default: break;
    }
}
