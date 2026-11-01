//
// Created by lovro on 01/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "screen_pedalboard.h"

#include "st7920.h"
#include <assert.h>
#include "ui/ui_common.h"

void Screen_Pedalboard_Draw()
{
    const int availableSpaceX = ST7920_WIDTH - 2 * UI_MARGIN_XY - 2 * SCREEN_PEDALBOARD_MARGIN;
    const int avaiableSpaceY  = ST7920_HEIGHT - UI_MARGIN_HEADER_Y - UI_MARGIN_XY - SCREEN_PEDALBOARD_MARGIN * 2;

    const int startX = UI_MARGIN_XY + SCREEN_PEDALBOARD_MARGIN;
    const int startY = UI_MARGIN_HEADER_Y + SCREEN_PEDALBOARD_MARGIN;

    // ST7920_DrawRectangleOutline(startX, startY, availableSpaceX, avaiableSpaceY, 1);

    const int totalPedalEmptySpaceX = availableSpaceX - SCREEN_PEDALBOARD_COLS * SCREEN_PEDALBOARD_PEDAL_WIDTH;
    static_assert(totalPedalEmptySpaceX > 0);

    const int indivPedalSpacingX = totalPedalEmptySpaceX / (SCREEN_PEDALBOARD_COLS - 1);
    // static_assert(totalPedalEmptySpaceX % (SCREEN_PEDALBOARD_COLS - 1) == 0);
    static_assert(indivPedalSpacingX >= 2);

    const int totalPedalEmptySpaceY = avaiableSpaceY - SCREEN_PEDALBOARD_ROWS * SCREEN_PEDALBOARD_PEDAL_HEIGHT;
    static_assert(totalPedalEmptySpaceY > 0);

    const int indivPedalSpacingY = totalPedalEmptySpaceY / (SCREEN_PEDALBOARD_ROWS - 1);
    static_assert(totalPedalEmptySpaceY % (SCREEN_PEDALBOARD_ROWS - 1) == 0);
    static_assert(indivPedalSpacingY >= 3);

    const char *labels[SCREEN_PEDALBOARD_ROWS * SCREEN_PEDALBOARD_COLS] = {
        "NG",
        "--",
        "CE",
        "DS",
        "--",
        "--",
        "DE",
        "VO",
    };

    const int selectedIdx = 6;

    for (int row = 0, i = 0, y = startY; row < SCREEN_PEDALBOARD_ROWS; ++row, y += SCREEN_PEDALBOARD_PEDAL_HEIGHT + indivPedalSpacingY)
    {
        for (int col = 0, x = startX; col < SCREEN_PEDALBOARD_COLS; ++col, x += SCREEN_PEDALBOARD_PEDAL_WIDTH + indivPedalSpacingX)
        {
            ST7920_DrawStringCenteredInRect(labels[i], x, y, SCREEN_PEDALBOARD_PEDAL_WIDTH, SCREEN_PEDALBOARD_PEDAL_HEIGHT, true, true);

            if (col < SCREEN_PEDALBOARD_COLS - 1)
            {
                ST7920_DrawLineHorizontal(x + SCREEN_PEDALBOARD_PEDAL_WIDTH + 1, y + SCREEN_PEDALBOARD_PEDAL_HEIGHT / 2, indivPedalSpacingX - 2);
            }

            if (col == SCREEN_PEDALBOARD_COLS - 1 && row < SCREEN_PEDALBOARD_ROWS - 1)
            {
                const int conn_startX = startX + SCREEN_PEDALBOARD_PEDAL_WIDTH / 2;
                const int conn_startY = y + SCREEN_PEDALBOARD_PEDAL_HEIGHT + 1;

                const int conn_endX = x + SCREEN_PEDALBOARD_PEDAL_WIDTH / 2;

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

            bool selected = i == selectedIdx;
            if (selected)
            {
                ST7920_InvertRegion(x, y, x + SCREEN_PEDALBOARD_PEDAL_WIDTH, y + SCREEN_PEDALBOARD_PEDAL_HEIGHT);
            }

            bool modded = true;
            ST7920_DrawRectangleOutlineNoCornersDottedEx1(x, y, SCREEN_PEDALBOARD_PEDAL_WIDTH, SCREEN_PEDALBOARD_PEDAL_HEIGHT, modded & !selected);

            i++;
        }
    }
}
