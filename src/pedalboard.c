//
// Created by lovro on 12/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "pedalboard.h"

#include <assert.h>

#include "input.h"
#include "st7920.h"
#include "fx/effect_distorsion.h"
#include "fx/effect_noisegate.h"
#include "fx/effect_volume.h"
#include "ui/ui_common.h"

Effect_CommonData *Pedalboard_Pedals[PEDALBOARD_PEDAL_COUNT] = {};

int Pedalboard_SelectedPedalIndex = 0;

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

    Effect_CommonData *pedal = Pedalboard_Pedals[0];

    for (int row = 0, i = 0, y = startY; row < PEDALBOARD_ROWS; ++row, y += PEDALBOARD_PEDAL_HEIGHT + indivPedalSpacingY)
    {
        for (int col = 0, x = startX; col < PEDALBOARD_COLS; ++col, x += PEDALBOARD_PEDAL_WIDTH + indivPedalSpacingX)
        {
            char *abbrev;

            pedal = Pedalboard_Pedals[i];
            if (pedal)
            {
                Effect_Blueprint *pedalBlueprint = Effect_Registry[pedal->id].blueprint;

                abbrev = pedalBlueprint->abbrev;
            }
            else
            {
                abbrev = "--";
            }

            ST7920_DrawStringCenteredInRect(abbrev, x, y, PEDALBOARD_PEDAL_WIDTH, PEDALBOARD_PEDAL_HEIGHT, true, true);

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

            bool selected = i == selectedPedalIdx;
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
    u32 matrix = 0;

    Effect_RegistryEntry *entry = Effect_Registry + pedalId;

    Effect_Blueprint *blueprint   = entry->blueprint;
    void *            instances   = entry->instances;
    u32               fxSizeBytes = blueprint->fxSizeBytes;

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

    Effect_CommonData* pedal = instances + firstFreeIdx * fxSizeBytes;
    Pedalboard_InsertExisting(pos, pedal);
}

void Pedalboard_ClearSlot(int pos)
{
    Pedalboard_Pedals[pos] = NULL;
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
    if (InputData_Instance.rotaryDir == INPUT_DIR_L)
    {
        Pedalboard_SelectPrevPedal();
    }
    else if (InputData_Instance.rotaryDir == INPUT_DIR_R)
    {
        Pedalboard_SelectNextPedal();
    }
}
