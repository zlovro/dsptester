//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "input.h"

#include <stdio.h>

#include "pedalboard.h"
#include "gfx/ui_common.h"

#if IS_EMU
#include "emu.h"
#endif

InputData            InputData_Data        = {};
static InputData_Raw InputData_DataRawPrev = {};

// (curr, prev) --> Input_ButtonState
#define INPUT_MERGE_BTN_STATE(curr, prev) ((Input_ButtonState)((curr) << 1) | (prev))

static u32         Input_ScreenChangeTimestamp = 0;
static UI_ScreenId Input_LastScreenId          = UI_SCR_PEDALBOARD;

static struct
{
    u32 rotary, next, prev;
} Input_BtnDownTimestamps = {};

void Input_Handle(InputData_Raw inputRawIn)
{
    u32 tick;

    #if IS_EMU
    tick = EMU_GetTick();
    #endif

    InputData_Raw prev = InputData_DataRawPrev;

    InputData_Data.btnHome   = INPUT_MERGE_BTN_STATE(inputRawIn.btnHome, prev.btnHome);
    InputData_Data.btnNext   = INPUT_MERGE_BTN_STATE(inputRawIn.btnNext, prev.btnNext);
    InputData_Data.btnPrev   = INPUT_MERGE_BTN_STATE(inputRawIn.btnPrev, prev.btnPrev);
    InputData_Data.rotaryBtn = INPUT_MERGE_BTN_STATE(inputRawIn.rotaryBtn, prev.rotaryBtn);

    if (UI_CurrentScreenId != Input_LastScreenId)
    {
        InputData_Data = (InputData){
            .rotaryDir = INPUT_DIR_NULL
        };
        Input_ScreenChangeTimestamp = tick;
    }

    Input_LastScreenId = UI_CurrentScreenId;

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

    if (InputData_Data.rotaryBtn == INPUT_BTN_DOWN)
    {
        Input_BtnDownTimestamps.rotary = tick;
    }

    if (InputData_Data.btnNext == INPUT_BTN_DOWN)
    {
        Input_BtnDownTimestamps.next = tick;
    }

    if (InputData_Data.btnPrev == INPUT_BTN_DOWN)
    {
        Input_BtnDownTimestamps.prev = tick;
    }

    InputData_DataRawPrev = inputRawIn;

    u32 btnNextHeldDuration = tick - Input_BtnDownTimestamps.next;
    u32 btnPrevHeldDuration = tick - Input_BtnDownTimestamps.prev;

    u32 timeSinceScreenChange = tick - Input_ScreenChangeTimestamp;

    constexpr int PEDALBOARD_EDIT_ACTIVATION_TIME = 1000;
    constexpr int TUNER_ACTIVATION_TIME           = 300;

    bool heldNextLongEnough = btnNextHeldDuration >= PEDALBOARD_EDIT_ACTIVATION_TIME;
    bool heldPrevLongEnough = btnPrevHeldDuration >= PEDALBOARD_EDIT_ACTIVATION_TIME;
    bool heldBtnLongEnough  = heldNextLongEnough && heldPrevLongEnough;

    bool longEnoughInScreen = timeSinceScreenChange >= 500;

    bool canEditPedalboard = timeSinceScreenChange >= 2000 && INPUT_IS_BTN_PRESSED(InputData_Data.btnNext) && INPUT_IS_BTN_PRESSED(InputData_Data.btnPrev) && heldBtnLongEnough;
    bool canGoToTuner      = longEnoughInScreen && !heldBtnLongEnough &&
                             ((INPUT_IS_BTN_PRESSED(InputData_Data.btnNext) && InputData_Data.btnPrev == INPUT_BTN_UP) ||
                              (INPUT_IS_BTN_PRESSED(InputData_Data.btnPrev) && InputData_Data.btnNext == INPUT_BTN_UP));

    switch (UI_CurrentScreenId)
    {
        case UI_SCR_PEDALBOARD:
        {
            if (canEditPedalboard)
            {
                UI_CurrentScreenId = UI_SCR_PEDALBOARD_EDIT;
                break;
            }

            if (canGoToTuner)
            {
                UI_CurrentScreenId = UI_SCR_TUNER;
                break;
            }

            Pedalboard_HandleInput();
            break;
        }
        case UI_SCR_PEDALBOARD_EDIT:
        {
            if (canEditPedalboard)
            {
                UI_CurrentScreenId            = UI_SCR_PEDALBOARD;
                Pedalboard_IgnoreInputCounter = 2;
                break;
            }

            Pedalboard_HandleInput();
            break;
        }
        case UI_SCR_TUNER:
        {
            if (canGoToTuner)
            {
                UI_CurrentScreenId            = UI_SCR_PEDALBOARD;
                Pedalboard_IgnoreInputCounter = 1;
                break;
            }
            break;
        }
        case UI_SCR_PEDAL_EDIT:
        {
            bool heldRotaryLongEnough = INPUT_IS_BTN_PRESSED(InputData_Data.rotaryBtn) && tick - Input_BtnDownTimestamps.rotary >= 500;
            if (timeSinceScreenChange >= 500 && heldRotaryLongEnough)
            {
                UI_CurrentScreenId = UI_SCR_PEDALBOARD;
                break;
            }

            // this will never be null, the active pedal must be non-NULL in order to go from UI_SCR_PEDALBOARD --> UI_SCR_PEDAL_EDIT
            Effect_CommonData *pedal = Pedalboard_Pedals[Pedalboard_SelectedPedalIndex];
            Effect_Blueprint * bp    = Effect_Registry[pedal->id].blueprint;

            bp->funcHandleInput(pedal);
            break;
        }
        case UI_SCR_OPTIONS:
        {
            break;
        }

        default: break;
    }
}
