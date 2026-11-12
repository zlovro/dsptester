#include <chrono>
#include <cmath>
#include <SDL3/SDL.h>
#include <filesystem>
#include <thread>

#include <matplotlibcpp.h>

#include "wavstreamer.hpp"
#include "main.hpp"


namespace plt = matplotlibcpp;

extern "C"
{
    #include <dsp.h>
    #include <ui/ui_common.h>
    #include <st7920.h>
    #include <pedalboard.h>
    #include <input.h>

    #include <fftw3.h>
}

#define LCD_SPI_FRAME_SIZE (64 * (7 + 32))

constexpr int LCD_WIDTH  = 128;
constexpr int LCD_HEIGHT = 64;

constexpr u32 LCD_COLOR_BACK  = 0x0F2AD6FF;
constexpr u32 LCD_COLOR_FRONT = 0xE4EBA0FF;

const bool DSP_WavTest = true;

static std::thread wavThread;

void        UpdateLCD(uint8_t *spiFrame, SDL_Texture *texture);
static void WAVPlay();
static void PlotFFT(f32 *buf, int n);

int main()
{
    #if TEST_TRANSFER_FUNC

    DSP_WorkBuf[0] = 1;

    Effect_RegisterEffects();

    Effect_Eq *eq = (Effect_Eq *) Effect_Registry[EFFECT_ID_EQ].instances;
    Effect_EqBandUpdateByFreq(eq, 50, 0.1, 10);
    Effect_EqBandUpdateByFreq(eq, 400, 2, 10);
    Effect_EqBandUpdateByFreq(eq, 1600, 10, 10);

    Effect_EqProcessAudio(eq);

    PlotFFT(DSP_WorkBuf, DSP_CHUNK_SIZE);

    plt::show();

    return 0;
    #else

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
    DSP_Init();
    WAVPlay();

    Effect_RegisterEffects();
    Pedalboard_Init();

    long frameCounter = 0;

    while (run)
    {
        auto timestamp_frameStart = std::chrono::system_clock::now();

        SDL_PollEvent(&ev);

        // LCD code

        ST7920_Seek(ST7920_ORIGIN_X, ST7920_ORIGIN_Y);

        ST7920_ClearBack();
        ST7920_DrawRectangleOutline(0, 0, LCD_WIDTH, LCD_HEIGHT, ST7920_ORIGIN_X);

        ST7920_DrawRectangleOutline(0, 0, LCD_WIDTH, UI_MARGIN_HEADER_Y, UI_GENERIC_MARGIN);

        ST7920_DrawStringAtPos("Efekt 1", ST7920_ORIGIN_X + UI_GENERIC_MARGIN, UI_PRESET_TEXT_Y_POS);

        // DrawString and DrawChar instead of DrawStringxxxx, we want to continue the "Preset" string to avoid a sprintf call

        char pageInfo[8];
        sprintf(pageInfo, "%02d", time(NULL) % 60);

        ST7920_DrawStringAtPosRightJustified(pageInfo, LCD_WIDTH - ST7920_ORIGIN_X - UI_GENERIC_MARGIN, UI_PRESET_TEXT_Y_POS);

        ST7920_DottedOffset = frameCounter / 4;
        Pedalboard_Draw();

    l_updateLcd:

        ST7920_BackToFront();
        UpdateLCD(ST7920_FrontBuf, screenTexture);

        // input code
        InputData_Instance.rotaryDir = INPUT_DIR_NULL;

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
                SDL_Keycode key = ev.key.key;

                if (key == SDLK_LEFT)
                {
                    InputData_Instance.rotaryDir = INPUT_DIR_L;
                }
                else if (key == SDLK_RIGHT)
                {
                    InputData_Instance.rotaryDir = INPUT_DIR_R;
                }
            }

            default:
            {
                break;
            }
        }

        Pedalboard_HandleInput();

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

        frameCounter++;
    }

    wavThread.join();

    SDL_DestroyTexture(screenTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    #endif
}


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

static void WAVPlay()
{
    if (wavThread.joinable())
    {
        wavThread.join();
    }
    wavThread = std::thread(WAVStreamer_Start, "audio/pm-ph.wav");
}


static void PlotFFT(f32 *buf, int n)
{
    fftwf_complex *signal = fftwf_alloc_complex(n);
    fftwf_complex *result = fftwf_alloc_complex(n);

    // init before fill.
    fftwf_plan plan = fftwf_plan_dft_1d(n, signal, result,FFTW_FORWARD,FFTW_ESTIMATE);

    for (int i = 0; i < n; ++i)
    {
        signal[i][0] = buf[i];
        signal[i][1] = 0;
    }

    fftwf_execute(plan);

    std::vector<f32> x;
    std::vector<f32> y;

    int i0 = (int) (20.0F / (SAMPLE_RATE / (float) n));

    for (int i = i0; i < n / 2; ++i)
    {
        f32 a = result[i][0];
        f32 b = result[i][1];

        f32 mag   = sqrtf(a * a + b * b);
        f32 magDb = 20 * log10f(mag);

        f32 freq = i * (SAMPLE_RATE / (float) n);
        // f32 freq = i;

        x.push_back(freq);
        y.push_back(magDb);

        // x.push_back(i);
        // y.push_back(mag);
    }

    plt::grid(true);

    plt::xlabel("Hz");
    plt::ylabel("dB");

    plt::tight_layout();

    plt::semilogx(x, y, "b-");

    fftwf_destroy_plan(plan);
    fftwf_free(signal);
    fftwf_free(result);
}
