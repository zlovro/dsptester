//
// Created by lovro on 24/03/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef ST7920_H
#define ST7920_H

#include "types.h"

#define ST7920_MONOSPACE false
#define ST7920_WIDTH 128
#define ST7920_HEIGHT 64

#define ST7920_ORIGIN_X 2
#define ST7920_ORIGIN_Y 2

#define ST7920_LINE_HEIGHT    9
#define ST7920_FONT_HEIGHT    8
#define ST7920_FONT_SPACING_X 2

#define ST7920_MAX_GLYPH_WIDTH 8

typedef pstruct
{
    u8 drawingStartEnd; // first column that isnt empty
    // u8 drawingEnd   : 3; // last non empty column
    // u8 padding      : 2;
    u8 cols[ST7920_MAX_GLYPH_WIDTH];
} ST7920_Glyph;

extern u8 ST7920_FrontBuf[64 * (7 + 32)];
extern u8 ST7920_BackBuf[1024];

extern const u8 ST7920_Font[];

#define ST7920_FontGlyphs ((ST7920_Glyph*) ST7920_Font)
#define ST7920_ClearBack() ARRAY_ZERO(ST7920_BackBuf)

extern int ST7920_CursorX, ST7920_CursorY;
extern int ST7920_WrapX;

extern bool ST7920_InvertColors;

void ST7920_Init();
void ST7920_BackToFront();

// void ST7920_SetOrigin(u8 x, u8 y);
void ST7920_Seek(u8 x, u8 y);

inline void ST7290_SetPixel(int x, int y, bool p);

void ST7920_DrawCircleOutline(int cx, int cy, int r);

void ST7920_DrawLineHorizontalEx(u8 x, u8 y, u8 length, bool dotted);
void ST7920_DrawLineVerticalEx(u8 x, u8 y, u8 length, bool dotted);

#define ST7920_DrawLineHorizontal(x, y, len) ST7920_DrawLineHorizontalEx(x, y, len, false)
#define ST7920_DrawLineHorizontalDotted(x, y, len) ST7920_DrawLineHorizontalEx(x, y, len, true)

#define ST7920_DrawLineVertical(x, y, len) ST7920_DrawLineVerticalEx(x, y, len, false)
#define ST7920_DrawLineVerticalDotted(x, y, len) ST7920_DrawLineVerticalEx(x, y, len, true)

void ST7920_DrawLineBresenham(int x0, int y0, int x1, int y1);

void ST7920_DrawRectangleOutlineEx(u8 x, u8 y, u8 width, u8 height, u8 thickness, bool cutCorners, bool dotted);

#define ST7920_DrawRectangleOutline(x, y, w, h, thickness) ST7920_DrawRectangleOutlineEx(x, y, w, h, thickness, false, false)
#define ST7920_DrawRectangleOutline1(x, y, w, h) ST7920_DrawRectangleOutlineEx(x, y, w, h, 1, false, false)

#define ST7920_DrawRectangleOutlineNoCorners(x, y, w, h, thickness) ST7920_DrawRectangleOutlineEx(x, y, w, h, thickness, true, false)
#define ST7920_DrawRectangleOutlineNoCorners1(x, y, w, h) ST7920_DrawRectangleOutlineEx(x, y, w, h, 1, true, false)

#define ST7920_DrawRectangleOutlineNoCornersDottedEx1(x, y, w, h, dotted) ST7920_DrawRectangleOutlineEx(x, y, w, h, 1, true, dotted)

#define ST7920_DrawRectangleOutlineNoCornersDotted1(x, y, w, h) ST7920_DrawRectangleOutlineEx(x, y, w, h, 1, true, true)
#define ST7920_DrawRectangleOutlineDotted1(x, y, w, h) ST7920_DrawRectangleOutlineEx(x, y, w, h, 1, false, true)

void ST7920_DrawChar(char chr);

void ST7920_DrawStringLen(char *str, u8 length);
void ST7920_DrawString(char *str);
void ST7920_DrawStringAtPos(char *str, int x, int y);
void ST7920_DrawStringAtPosRightJustified(char *str, int x, int y);
void ST7920_DrawStringLenCenteredInRect(char *str, u8 length, u8 x, u8 y, u8 width, u8 height, bool vertically, bool horizontally);
void ST7920_DrawStringCenteredInRect(char *str, u8 x, u8 y, u8 width, u8 height, bool vertically, bool horizontally);

inline int ST7920_GetGlyphWidth(ST7920_Glyph *g);
int        ST7920_GetStringWidth(char *str);

void ST7920_InvertRegion(int x0, int y0, int x1, int y1);
void ST7920_DrawCheckerboard(int x0, int y0, int x1, int y1);

extern bool ST7920_DottedDefaultState;
extern int ST7920_DottedOffset;

void ST7920_Printf(char *format, ...);
void ST7920_PrintfCenteredInRect(char *format, u8 x, u8 y, u8 width, u8 height, bool vertically, bool horizontally, ...);

#endif //ST7920_H
