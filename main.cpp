#include <chrono>
#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

extern "C"
{
    #include "st7920.h"
}

#define LCD_SPI_FRAME_SIZE (64 * (7 + 32))

constexpr int LCD_WIDTH  = 128;
constexpr int LCD_HEIGHT = 64;

constexpr u32 LCD_COLOR_BACK  = 0x0F2AD6FF;
constexpr u32 LCD_COLOR_FRONT = 0xE4EBA0FF;

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

typedef struct Potentiometer
{
    int   centerX;
    int   centerY;
    int   radius;
    int   degreeRange;
    int   value;
    char *label;
    bool  selected;
} Potentiometer;

void DrawPot(Potentiometer *pot)
{
    int cx = pot->centerX;
    int cy = pot->centerY;
    int r  = pot->radius;

    ST7920_DrawCircleOutline(cx, cy, r);

    int degLeft  = 270 - (360 - pot->degreeRange) / 2;
    int degRight = pot->degreeRange + degLeft;

    int degWiper = degLeft - ((degRight - degLeft) * pot->value) / 100;

    f32 radWiper = degWiper * M_PI / 180;
    int rlim     = degWiper >= 90 ? r - 1 : r;

    int wiperX = cx + cosf(radWiper) * rlim;
    int wiperY = cy - sinf(radWiper) * rlim;

    ST7920_DrawLineBresenham(cx, cy, wiperX, wiperY);

    char buf[32];
    sprintf(buf, "%s\n%d", pot->label, pot->value);

    ST7920_DrawStringCenteredInRect(buf, cx - r, cy + r + ST7920_LineHeight / 2, 2 * r, 1000, false, true);

    int   linesInLabel = 1;
    char  c;
    char *p = buf;
    while (c = *p++)
    {
        if (c == '\n')
        {
            linesInLabel++;
        }
    }

    if (pot->selected)
    {
        int selectedMargin = 1;
        // ST7920_DrawRectangleOutline(cx - r - selectedMargin, cy - r - selectedMargin, r + selectedMargin, r + ST7920_LineHeight / 2 + linesInLabel * ST7920_LineHeight, 1);
        ST7920_InvertRegion(cx - r - selectedMargin, cy - r - selectedMargin, cx + r + selectedMargin, cy + r + ST7920_LineHeight / 2 + linesInLabel * ST7920_LineHeight);
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    int  scale  = 5;
    auto window = SDL_CreateWindow("STM32 DSP Tester", LCD_WIDTH * scale, LCD_HEIGHT * scale, 0);

    auto renderer = SDL_CreateRenderer(window, NULL);

    auto      run = true;
    SDL_Event ev;

    constexpr int TARGET_FRAMETIME = 1'000'000 / 60;

    auto screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, LCD_WIDTH, LCD_HEIGHT);
    SDL_SetTextureScaleMode(screenTexture, SDL_SCALEMODE_NEAREST);

    ST7920_Init();

    ST7920_OriginX = ST7920_OriginY = 2;

    Potentiometer potArray[] = {Potentiometer{.label = "Dist"}, Potentiometer{.label = "Gain"}, Potentiometer{.label = "Tone"}, Potentiometer{.label = "D/W"},};

    int pots      = sizeof(potArray) / sizeof(Potentiometer);
    int potmargin = 7;
    f32 rf        = (LCD_WIDTH - 2 * ST7920_OriginX - (pots + 1) * potmargin) / (float) (pots * 2);

    f32 xf = ST7920_OriginX;

    for (int i = 0; i < pots; i++)
    {
        xf += potmargin + rf;
        Potentiometer *pot = potArray + i;

        pot->centerX     = (int) xf;
        pot->centerY     = 28;
        pot->radius      = (int) rf;
        pot->degreeRange = 315;
        pot->value       = 0;
        pot->selected    = false;

        xf += rf;
    }

    Potentiometer *selectedPot = potArray;
    selectedPot->selected      = true;

    while (run)
    {
        auto timestamp_frameStart = std::chrono::system_clock::now();

        SDL_PollEvent(&ev);

        switch (ev.type)
        {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
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
                        selectedPot->selected = false;

                        selectedPot--;
                        if (selectedPot < potArray)
                        {
                            selectedPot = potArray + pots - 1;
                        }

                        selectedPot->selected = true;
                        break;
                    }

                    case SDLK_RIGHT:
                    {
                        selectedPot->selected = false;

                        selectedPot++;
                        if (selectedPot >= potArray + pots)
                        {
                            selectedPot = potArray;
                        }

                        selectedPot->selected = true;
                        break;
                    }
                }

                break;
            }

            case SDL_EVENT_MOUSE_WHEEL:
            {
                if (ev.wheel.integer_y > 0)
                {
                    selectedPot->value++;
                }
                else
                {
                    selectedPot->value--;
                }

                selectedPot->value = max(0, min(100, selectedPot->value));

                break;
            }

            default:
            {
                break;
            }
        }

        // LCD code

        ST7920_ClearBack();
        ST7920_DrawRectangleOutline(0, 0, LCD_WIDTH, LCD_HEIGHT, ST7920_OriginX);

        ST7920_Seek(ST7920_OriginX, ST7920_OriginY);

        for (int i = 0; i < pots; i++)
        {
            DrawPot(potArray + i);
        }

        int headerMargin = 2;
        int hHeader      = ST7920_LineHeight + headerMargin * 2;
        ST7920_DrawRectangleOutline(0, 0, LCD_WIDTH, ST7920_OriginY + hHeader, 1);
        ST7920_DrawStringAtPos("Overdrive", ST7920_OriginX + headerMargin, ST7920_OriginY + headerMargin);
        ST7920_DrawStringAtPosRightJustified("81/99", 128 - ST7920_OriginX - headerMargin, ST7920_OriginY + headerMargin);

        // ST7920_InvertRegion(1, 1, 25, 2);

        ST7920_BackToFront();
        UpdateLCD(ST7920_FrontBuf, screenTexture);

        // rendering code.

        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, screenTexture, NULL, NULL);

        SDL_RenderPresent(renderer);

        while (true)
        {
            long long frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timestamp_frameStart).count();

            if (frameDuration >= TARGET_FRAMETIME)
            {
                break;
            }

            SDL_DelayNS(1'000 * 100);
        }
    }

    SDL_DestroyTexture(screenTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
