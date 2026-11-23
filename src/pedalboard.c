//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "pedalboard.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>

#include "input.h"
#include "iohelper.h"
#include "gfx/st7920.h"
#include "audiofx/effect_distorsion.h"
#include "audiofx/effect_noisegate.h"
#include "gfx/ui_common.h"

Pedalboard_Preset  Pedalboard_Presets[PEDALBOARD_PRESET_COUNT] = {};
Effect_CommonData *Pedalboard_Pedals[PEDALBOARD_PEDAL_COUNT]   = {};

Pedalboard_Preset *Pedalboard_ActivePreset = Pedalboard_Presets + 4;

int Pedalboard_IgnoreInputCounter = 0;
int Pedalboard_SelectedPedalIndex = 2;

static bool     Pedalboard_ChoosingPedal = false;
static EffectId Pedalboard_ChoosingTmpId = EFFECT_ID_INVALID;

void Pedalboard_Init()
{
    if (!Effect_RegistryDone)
    {
        // Pedalboard must be initialized after all effects are registered
        assert(false);
    }

    Pedalboard_InsertNew(0, EFFECT_ID_NOISEGATE);
    Pedalboard_InsertNew(1, EFFECT_ID_DISTORSION);
    Pedalboard_InsertNew(2, EFFECT_ID_EQ);
    Pedalboard_InsertNew(5, EFFECT_ID_VOLUME);

    for (int i = 0; i < PEDALBOARD_PRESET_COUNT; ++i)
    {
        Pedalboard_Preset preset = {};

        sprintf(preset.name, "Preset %02d", i + 1);

        Pedalboard_Presets[i] = preset;
    }
}

void Pedalboard_Draw()
{
    constexpr int availableSpaceX = ST7920_WIDTH - 2 * UI_MARGIN_XY - 2 * PEDALBOARD_MARGIN;
    constexpr int avaiableSpaceY  = ST7920_HEIGHT - UI_MARGIN_HEADER_Y - UI_MARGIN_XY - PEDALBOARD_MARGIN * 2;

    constexpr int startX = UI_MARGIN_XY + PEDALBOARD_MARGIN;
    constexpr int startY = UI_MARGIN_HEADER_Y + PEDALBOARD_MARGIN;

    // ST7920_DrawRectangleOutline(startX, startY, availableSpaceX, avaiableSpaceY, 1);

    constexpr int totalPedalEmptySpaceX = availableSpaceX - PEDALBOARD_COLS * PEDALBOARD_PEDAL_WIDTH;
    static_assert(totalPedalEmptySpaceX > 0);

    constexpr int indivPedalSpacingX = totalPedalEmptySpaceX / (PEDALBOARD_COLS - 1);
    // static_assert(totalPedalEmptySpaceX % (PEDALBOARD_COLS - 1) == 0);
    static_assert(indivPedalSpacingX >= 2);

    constexpr int totalPedalEmptySpaceY = avaiableSpaceY - PEDALBOARD_ROWS * PEDALBOARD_PEDAL_HEIGHT;
    static_assert(totalPedalEmptySpaceY > 0);

    constexpr int indivPedalSpacingY = totalPedalEmptySpaceY / (PEDALBOARD_ROWS - 1);
    static_assert(totalPedalEmptySpaceY % (PEDALBOARD_ROWS - 1) == 0);
    static_assert(indivPedalSpacingY >= 3);

    int selectedPedalIdx = Pedalboard_SelectedPedalIndex;

    ST7920_DrawRectangleOutline(0, 0, ST7920_WIDTH, UI_MARGIN_HEADER_Y, UI_GENERIC_MARGIN);

    char tmp[16];

    bool     isEditing     = UI_CurrentScreenId == UI_SCR_PEDALBOARD_EDIT;
    bool     choosingPedal = Pedalboard_ChoosingPedal;
    EffectId choosingTmpId = Pedalboard_ChoosingTmpId;

    for (int row = 0, i = 0, y = startY; row < PEDALBOARD_ROWS; ++row, y += PEDALBOARD_PEDAL_HEIGHT + indivPedalSpacingY)
    {
        for (int col = 0, x = startX; col < PEDALBOARD_COLS; ++col, x += PEDALBOARD_PEDAL_WIDTH + indivPedalSpacingX)
        {
            char *abbrev;

            Effect_CommonData *pedal = Pedalboard_Pedals[i];
            if (pedal)
            {
                Effect_Blueprint *pedalBlueprint = Effect_Registry[pedal->id].blueprint;

                abbrev = pedalBlueprint->abbrev;
            }
            else
            {
                abbrev = "--";
            }

            bool selected = i == selectedPedalIdx;

            char *txtToDraw = abbrev;
            if (choosingPedal && selected && isEditing)
            {
                if (choosingTmpId >= EFFECT_COUNT)
                {
                    abbrev = "--";
                }
                else
                {
                    abbrev = Effect_Registry[choosingTmpId].blueprint->abbrev;
                }

                sprintf(tmp, "%c%s%c", ST7920_FONT_ICON_ARROW_LEFT, abbrev, ST7920_FONT_ICON_ARROW_RIGHT);
                txtToDraw = tmp;
            }

            ST7920_DrawStringCenteredInRect(txtToDraw, x, y, PEDALBOARD_PEDAL_WIDTH, PEDALBOARD_PEDAL_HEIGHT, true, true);

            if (col < PEDALBOARD_COLS - 1)
            {
                ST7920_DrawLineHorizontal(x + PEDALBOARD_PEDAL_WIDTH + 1, y + PEDALBOARD_PEDAL_HEIGHT / 2, indivPedalSpacingX - 2);
            }

            if (col == PEDALBOARD_COLS - 1 && row < PEDALBOARD_ROWS - 1)
            {
                const int conn_startX = startX + PEDALBOARD_PEDAL_WIDTH / 2;
                const int conn_startY = y + PEDALBOARD_PEDAL_HEIGHT + 1;

                const int conn_endX = x + PEDALBOARD_PEDAL_WIDTH / 2;

                // ST7290_SetPixel(conn_endX, conn_startY - 1, 0);

                int k = conn_startY;
                for (; k < conn_startY + indivPedalSpacingY / 2; k++)
                {
                    ST7290_SetPixel(conn_endX, k, 1);
                }

                ST7920_DrawLineHorizontal(conn_startX + 1, k, conn_endX - conn_startX - 1);

                for (k = k + 1; k != conn_startY + indivPedalSpacingY - 2; k++)
                {
                    ST7290_SetPixel(conn_startX, k, 1);
                }

                ST7920_DrawLineBresenham(conn_startX, k - 1, conn_startX - 2, k - 3);
                ST7920_DrawLineBresenham(conn_startX, k - 1, conn_startX + 2, k - 3);
            }

            if (selected)
            {
                ST7920_InvertRegion(x, y, x + PEDALBOARD_PEDAL_WIDTH, y + PEDALBOARD_PEDAL_HEIGHT);
            }

            bool modded = false;
            ST7920_DrawRectangleOutlineNoCornersDottedEx1(x, y, PEDALBOARD_PEDAL_WIDTH, PEDALBOARD_PEDAL_HEIGHT, modded & !selected);

            i++;
        }
    }
}

void Pedalboard_InsertExisting(int pos, Effect_CommonData *pedal)
{
    Pedalboard_Pedals[pos] = pedal;
}

void Pedalboard_InsertNew(int pos, EffectId pedalId)
{
    if (pedalId >= EFFECT_COUNT)
    {
        Pedalboard_InsertExisting(pos, NULL);
        return;
    }

    u32 matrix = 0;

    Effect_RegistryEntry *entry = Effect_Registry + pedalId;

    Effect_Blueprint *bp          = entry->blueprint;
    void *            instances   = entry->instances;
    u32               fxSizeBytes = bp->instanceSize;

    for (int i = 0; i < PEDALBOARD_PEDAL_COUNT; ++i)
    {
        Effect_CommonData *pedal = Pedalboard_Pedals[i];
        if (!pedal)
        {
            continue;
        }

        EffectId id = pedal->id;

        if (pedalId == id)
        {
            int distance         = (void *) pedal - instances;
            int idxIntoInstances = distance / fxSizeBytes;

            matrix |= 1 << idxIntoInstances;
        }
    }

    // gets the index of the rightmost set bit + 1 (LSB = 1)
    u32 firstFreeIdx = __builtin_ffs(~matrix) - 1;

    if (firstFreeIdx >= PEDALBOARD_PEDAL_COUNT)
    {
        assert(false);
    }

    Effect_CommonData *pedal = instances + firstFreeIdx * fxSizeBytes;
    bp->funcInit(pedal);

    Pedalboard_InsertExisting(pos, pedal);
}

void Pedalboard_SelectNextPedal()
{
    Pedalboard_SelectedPedalIndex++;
    if (Pedalboard_SelectedPedalIndex >= PEDALBOARD_PEDAL_COUNT)
    {
        Pedalboard_SelectedPedalIndex = 0;
    }
}

void Pedalboard_SelectPrevPedal()
{
    Pedalboard_SelectedPedalIndex--;
    if (Pedalboard_SelectedPedalIndex < 0)
    {
        Pedalboard_SelectedPedalIndex = PEDALBOARD_PEDAL_COUNT - 1;
    }
}

void Pedalboard_HandleInput()
{
    bool editingBoard = UI_CurrentScreenId == UI_SCR_PEDALBOARD_EDIT;

    InputData in = InputData_Data;

    if (in.rotaryDir == INPUT_DIR_L)
    {
        if (Pedalboard_ChoosingPedal && editingBoard)
        {
            int id = Pedalboard_ChoosingTmpId;
            id--;

            if (id == -1)
            {
                id = EFFECT_ID_INVALID;
            }
            else if (id == EFFECT_ID_INVALID - 1)
            {
                id = EFFECT_COUNT - 1;
            }

            Pedalboard_ChoosingTmpId = id;
        }
        else
        {
            Pedalboard_SelectPrevPedal();
        }
    }
    else if (in.rotaryDir == INPUT_DIR_R)
    {
        if (Pedalboard_ChoosingPedal && editingBoard)
        {
            int id = Pedalboard_ChoosingTmpId;
            id++;

            if (id == EFFECT_COUNT)
            {
                id = EFFECT_ID_INVALID;
            }
            else if (id == EFFECT_ID_INVALID)
            {
                id = 0;
            }

            Pedalboard_ChoosingTmpId = id;
        }
        else
        {
            Pedalboard_SelectNextPedal();
        }
    }

    if (Pedalboard_IgnoreInputCounter == 0)
    {
        if (!INPUT_IS_BTN_PRESSED(in.btnPrev) && in.btnNext == INPUT_BTN_UP)
        {
            Pedalboard_SelectNextPreset();
        }
        else if (!INPUT_IS_BTN_PRESSED(in.btnNext) && in.btnPrev == INPUT_BTN_UP)
        {
            Pedalboard_SelectPrevPreset();
        }
    }

    if (Pedalboard_IgnoreInputCounter)
    {
        if (in.btnPrev == INPUT_BTN_UP || in.btnNext == INPUT_BTN_UP)
        {
            Pedalboard_IgnoreInputCounter--;
        }
    }

    if (in.rotaryBtn == INPUT_BTN_DOWN)
    {
        Effect_CommonData *pedal = Pedalboard_Pedals[Pedalboard_SelectedPedalIndex];


        if (editingBoard)
        {
            if (Pedalboard_ChoosingPedal)
            {
                // end of choosing
                Pedalboard_InsertNew(Pedalboard_SelectedPedalIndex, Pedalboard_ChoosingTmpId);
            }
            else
            {
                // start of choosing
                Pedalboard_ChoosingTmpId = pedal ? pedal->id : EFFECT_ID_INVALID;
            }

            Pedalboard_ChoosingPedal = !Pedalboard_ChoosingPedal;
        }
        else
        {
            if (pedal)
            {
                UI_CurrentScreenId = UI_SCR_PEDAL_EDIT;
            }
        }
    }
}

void Pedalboard_StateSerialize(void **outBufPtr)
{
    u32 idSize    = sizeof(EffectId) * PEDALBOARD_PEDAL_COUNT;
    u32 allocSize = idSize;

    for (int i = 0; i < PEDALBOARD_PEDAL_COUNT; ++i)
    {
        Effect_CommonData *pedal = Pedalboard_Pedals[i];
        if (!pedal)
        {
            continue;
        }

        EffectId          id = pedal->id;
        Effect_Blueprint *bp = Effect_Registry[id].blueprint;

        allocSize += bp->funcSerialize(NULL, NULL);
    }

    void *buf       = malloc(allocSize);
    void *bufIdPtr  = buf;
    void *bufDatPtr = buf + idSize;
    *outBufPtr      = buf;


    for (int i = 0; i < PEDALBOARD_PEDAL_COUNT; ++i)
    {
        Effect_CommonData *pedal = Pedalboard_Pedals[i];
        if (pedal)
        {
            bufIdPtr += IO_Write_EffectId(pedal->id, bufIdPtr);

            Effect_Blueprint *bp = Effect_Registry[pedal->id].blueprint;

            bufDatPtr += bp->funcSerialize(pedal, bufDatPtr);
        }
        else
        {
            bufIdPtr += IO_Write_EffectId(EFFECT_ID_INVALID, bufIdPtr);
        }
    }
}

void Pedalboard_StateDeserialize(void *inBuf)
{
    Pedalboard_ClearAll();

    void *tmp = inBuf;
    for (int i = 0; i < PEDALBOARD_PEDAL_COUNT; ++i, tmp += sizeof(EffectId))
    {
        EffectId id = IO_ReadToVal_EffectId(tmp);
        if (id == EFFECT_ID_INVALID)
        {
            continue;
        }

        Pedalboard_InsertNew(i, id);
    }

    for (int i = 0; i < PEDALBOARD_PEDAL_COUNT; ++i)
    {
        Effect_CommonData *pedal = Pedalboard_Pedals[i];
        if (!pedal)
        {
            continue;
        }

        Effect_Blueprint *bp = Effect_Registry[pedal->id].blueprint;
        tmp += bp->funcDeserialize(pedal, tmp);
    }
}

void Pedalboard_SelectNextPreset()
{
    Pedalboard_ActivePreset++;
    if (Pedalboard_ActivePreset >= Pedalboard_Presets + PEDALBOARD_PRESET_COUNT)
    {
        Pedalboard_ActivePreset = Pedalboard_Presets;
    }

    Pedalboard_RefreshPreset();
}

void Pedalboard_SelectPrevPreset()
{
    Pedalboard_ActivePreset--;
    if (Pedalboard_ActivePreset < Pedalboard_Presets)
    {
        Pedalboard_ActivePreset = Pedalboard_Presets + PEDALBOARD_PRESET_COUNT - 1;
    }

    Pedalboard_RefreshPreset();
}

void Pedalboard_RefreshPreset()
{
    u32 mtx = Pedalboard_ActivePreset->activePedals;

    for (int i = 0; i < PEDALBOARD_PEDAL_COUNT; ++i)
    {
        // TODO: pedal ON/OFF
        bool on = mtx & 1 << i;
    }
}
