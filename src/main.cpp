#include <chrono>
#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <thread>

#include "wavstreamer.hpp"
#include "main.hpp"

extern "C"
{
    #include "st7920.h"
    #include "ui/effects.h"
    #include "ui/ui_common.h"
    #include "dsp.h"
    #include "ui/screens/screen_pedalboard.h"
}

#define LCD_SPI_FRAME_SIZE (64 * (7 + 32))

constexpr int LCD_WIDTH  = 128;
constexpr int LCD_HEIGHT = 64;

constexpr u32 LCD_COLOR_BACK  = 0x0F2AD6FF;
constexpr u32 LCD_COLOR_FRONT = 0xE4EBA0FF;

const bool DSP_WavTest = true;

void UpdateLCD(uint8_t *spiFrame, SDL_Texture *texture)
{
    u32 *texBuf;
    int  pitch;
    SDL_LockTexture(texture, NULL, (void **) &texBuf, &pitch);

    int filled = 0;

    uint8_t *p = spiFrame;
    while (true)
    {
        if (filled >= LCD_HEIGHT * LCD_WIDTH)
        {
            break;
        }

        if (*p == 0xF8)
        {
            p++;
            u16 y = ((*p++) & 0x70);
            y |= (*p++ & 0x70) >> 4;

            p++;

            u16 x = ((*p++) & 0x70);
            x |= (*p++ & 0x70) >> 4;
        }

        if (*p == 0xFA)
        {
            p++;

            for (int i = 0; i < 16; i++)
            {
                auto x = *p++ | (*p++ >> 4);
                for (int j = 7; j >= 0; j--)
                {
                    *texBuf++ = x & 1 << j ? LCD_COLOR_FRONT : LCD_COLOR_BACK;
                }
            }

            filled += 128;
        }
    }

    SDL_UnlockTexture(texture);
}

static std::thread wavThread;

static void WAVPlay()
{
    if (wavThread.joinable())
    {
        wavThread.join();
    }
    wavThread = std::thread(WAVStreamer_Start, "audio/deformero_mute.wav");
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    int  scale  = 5;
    auto window = SDL_CreateWindow("STM32 DSP Tester", LCD_WIDTH * scale, LCD_HEIGHT * scale, 0);

    auto renderer = SDL_CreateRenderer(window, NULL);

    auto      run = true;
    SDL_Event ev;

    constexpr int TARGET_FRAMETIME_US = 1'000'000 / 60;

    auto screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, LCD_WIDTH, LCD_HEIGHT);
    SDL_SetTextureScaleMode(screenTexture, SDL_SCALEMODE_NEAREST);

    ST7920_Init();

    // rem. width, height = 124, 60

    // FX
    Effect_RegisterEffects();

    // WAV
    // WAVPlay();

    while (run)
    {
        auto timestamp_frameStart = std::chrono::system_clock::now();

        u8 inputData = 0;

        SDL_PollEvent(&ev);

        // LCD code

        ST7920_ClearBack();

        // ST7920_DrawCheckerboard(0, 0, 126, 2);
        // ST7920_InvertRegion(0, 0, 9, 3);

        // goto l_updateLcd;

        ST7920_DrawRectangleOutline(0, 0, LCD_WIDTH, LCD_HEIGHT, ST7920_ORIGIN_X);

        ST7920_Seek(ST7920_ORIGIN_X, ST7920_ORIGIN_Y);

        ST7920_DrawRectangleOutline(0, 0, LCD_WIDTH, UI_MARGIN_HEADER_Y, UI_GENERIC_MARGIN);

        ST7920_DrawStringAtPos("Efekt 1", ST7920_ORIGIN_X + UI_GENERIC_MARGIN, UI_PRESET_TEXT_Y_POS);

        // DrawString and DrawChar instead of DrawStringxxxx, we want to continue the "Preset" string to avoid a sprintf call

        char pageInfo[8];
        sprintf(pageInfo, "%02d", time(NULL) % 60);

        // ST7920_CheckerboardDefaultState = ;

        ST7920_DrawStringAtPosRightJustified(pageInfo, LCD_WIDTH - ST7920_ORIGIN_X - UI_GENERIC_MARGIN, UI_PRESET_TEXT_Y_POS);

        Screen_Pedalboard_Draw();

        l_updateLcd:

        ST7920_BackToFront();
        UpdateLCD(ST7920_FrontBuf, screenTexture);

        // input code

        switch (ev.type)
        {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            case SDL_EVENT_QUIT:
            {
                run = false;
                break;
            }

            case SDL_EVENT_KEY_DOWN:
            {
                switch (ev.key.key)
                {
                    case SDLK_ESCAPE:
                    {
                        run = false;
                        break;
                    }

                    case SDLK_LEFT:
                    {
                        // Effect_SelectPreviousControl(selectedEffect);
                        break;
                    }

                    case SDLK_RIGHT:
                    {
                        // Effect_SelectNextControl(selectedEffect);
                        break;
                    }

                    case SDLK_UP:
                    {
                        // int nextId = selectedEffect->id + 1;
                        // if (nextId >= EFFECT_COUNT)
                        // {
                        //     nextId = 0;
                        // }

                        break;
                    }

                    case SDLK_DOWN:
                    {
                        // int nextId = selectedEffect->id - 1;
                        // if (nextId < 0)
                        // {
                        //     nextId = EFFECT_COUNT - 1;
                        // }

                        break;
                    }

                    case SDLK_SPACE:
                    {
                        inputData |= ROTARY_ENCODER_BTN_PRESSED;
                        break;
                    }
                }

                break;
            }

            case SDL_EVENT_MOUSE_WHEEL:
            {
                if (ev.wheel.integer_y > 0)
                {
                    inputData |= ROTARY_ENCODER_DIR_POS;
                }
                else
                {
                    inputData |= ROTARY_ENCODER_DIR_NEG;
                }

                break;
            }

            default:
            {
                break;
            }
        }

        if (inputData)
        {
            // selectedEffect->funcHandleInput(selectedEffect, inputData);
        }

        // rendering code.

        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, screenTexture, NULL, NULL);

        SDL_RenderPresent(renderer);

        while (true)
        {
            long long frameDurationUs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timestamp_frameStart).count();

            if (frameDurationUs >= TARGET_FRAMETIME_US)
            {
                break;
            }

            SDL_DelayNS(1'000 * 100);
        }
    }

    wavThread.join();

    SDL_DestroyTexture(screenTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
