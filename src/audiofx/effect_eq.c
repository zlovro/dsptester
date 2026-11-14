//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "effect_eq.h"

#include <assert.h>
#include <math.h>

#include "dsp.h"
#include "iohelper.h"
#include "algo/fxalgo_eq.h"

#include <gfx/ui_common.h>
#include <gfx/st7920.h>
#include <pedalboard.h>

void Effect_EqInit(Effect_Eq *fx)
{
    Effect_Algo_EqInstance_Init(&fx->algoInstance);
}

void Effect_EqDraw(Effect_Eq *fx)
{
    constexpr int startX = UI_MARGIN_XY + 1;
    constexpr int startY = UI_MARGIN_XY + 1;

    constexpr int availableSpaceX = ST7920_WIDTH - 2 * startX;
    constexpr int avaiableSpaceY  = ST7920_HEIGHT - 2 * startY;

    constexpr int eqBandContainerHeight = avaiableSpaceY;

    constexpr int eqBandContainerWidth           = 28;
    constexpr int eqBandContainerTotalFreeSpaceX = availableSpaceX - EFFECT_ALGO_EQ_BAND_COUNT * eqBandContainerWidth;
    constexpr int eqBandContainerSpacingX        = (eqBandContainerTotalFreeSpaceX) / (EFFECT_ALGO_EQ_BAND_COUNT - 1);

    static_assert(eqBandContainerSpacingX > 0);

    constexpr int eqBandFaderHeight      = 40;
    constexpr int eqBandTextStartOffsetY = eqBandFaderHeight;
    constexpr int eqBandTextRectHeight   = eqBandContainerHeight - eqBandTextStartOffsetY;
    constexpr int textRectYTop           = startY + eqBandTextStartOffsetY;

    static_assert(eqBandTextRectHeight >= ST7920_LINE_HEIGHT);

    for (int i = 0, x = startX; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i, x += eqBandContainerSpacingX)
    {
        Effect_Algo_EqBand band = fx->algoInstance.bands[i];

        ST7920_DrawRectangleOutlineNoCorners1(x, startY, eqBandContainerWidth, eqBandContainerHeight);

        ST7920_DrawStringCenteredInRect(band.freqLabel, x, textRectYTop, eqBandContainerWidth, eqBandTextRectHeight, true, true);

        // fader
        ST7920_DrawRectangleOutlineDotted1(x, startY, eqBandContainerWidth, eqBandFaderHeight);

        int centerX = x + eqBandContainerWidth / 2;

        constexpr int faderLineStartY = startY + 2;
        constexpr int faderLineHeight = eqBandFaderHeight - 4;
        constexpr int faderLineFinalY = faderLineStartY + faderLineHeight - 1;

        // ST7920_DrawLineVertical(centerX, faderLineStartY, faderLineHeight);

        ST7920_DrawLineHorizontal(centerX - 1, faderLineStartY, 2);
        ST7920_DrawLineHorizontal(centerX - 1, faderLineFinalY, 2);

        for (int i = 0; i < EFFECT_EQ_DB_LINE_COUNT; ++i)
        {
        }

        x += eqBandContainerWidth;
    }
}

void Effect_EqHandleInput(Effect_Eq *fx)
{
}

void Effect_EqProcessAudio(Effect_Eq *fx)
{
    Effect_Algo_Eq(&fx->algoInstance);
}

static constexpr int EFFECT_EQ_SERIALIZATION_SIZE = EFFECT_ALGO_EQ_BAND_COUNT * (2 * sizeof(f32) + sizeof(u16));

u32 Effect_EqSerialize(Effect_Eq *fxIn, void *bufOut)
{
    if (fxIn)
    {
        auto bands = fxIn->algoInstance.bands;

        for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
        {
            Effect_Algo_EqBand band = bands[i];

            bufOut += IO_Write_f32(band.gain, bufOut);
            bufOut += IO_Write_f32(band.q, bufOut);

            bufOut += IO_Write_u16(band.freq, bufOut);
        }
    }

    return EFFECT_EQ_SERIALIZATION_SIZE;
}

u32 Effect_EqDeserialize(Effect_Eq *fxOut, void *bufIn)
{
    for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
    {
        Effect_Algo_EqBand band = {};

        bufIn += IO_ReadToPtr_f32(&band.gain, bufIn);
        bufIn += IO_ReadToPtr_f32(&band.q, bufIn);

        bufIn += IO_ReadToPtr_u16(&band.freq, bufIn);

        fxOut->algoInstance.bands[i] = band;
    }

    return EFFECT_EQ_SERIALIZATION_SIZE;
}

void Effect_EqBandUpdate(Effect_Eq *fx, int band, u16 freq, f32 gain, f32 q)
{
    Effect_Algo_EqBandUpdate(fx->algoInstance.bands + band, freq, gain, q);
}

void Effect_EqBandUpdateByFreq(Effect_Eq *fx, u16 searchFreq, f32 gain, f32 q)
{
    for (int i = 0; i < EFFECT_ALGO_EQ_BAND_COUNT; ++i)
    {
        Effect_Algo_EqBand *band = fx->algoInstance.bands + i;
        if (band->freq == searchFreq)
        {
            Effect_Algo_EqBandUpdate(band, searchFreq, gain, q);
            return;
        }
    }

    assert(false);
}
