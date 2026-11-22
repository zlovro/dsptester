//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef UICONTROL_H
#define UICONTROL_H

#include <stdbool.h>

#include "types.h"

typedef struct UIControl_Rect
{
    union
    {
        int x0;
        int left;
    };

    union
    {
        int y0;
        int top;
    };

    union
    {
        int x1;
        int right;
    };

    union
    {
        int y1;
        int bottom;
    };
} UIControl_Rect;

typedef struct UIControl_Common
{
    UIControl_Rect rect;
    bool           isSelected;
} UIControl_Common;

typedef struct Potentiometer
{
    UIControl_Common common;

    const char *label;

    int centerX;
    int centerY;

    // 0 to 1
    f32 value;

    int wiperX;
    int wiperY;

    int width;
} Potentiometer;

#define POTENTIOMETER_DEGREE_RANGE 300
#define POTENTIOMETER_RADIUS 12
#define POTENTIOMETER_WH (POTENTIOMETER_RADIUS * 2)
#define POTENTIOMETER_LABEL_Y_PADDING 2

#define POTS_ARRAY(n, ...) \
    union \
    { \
        struct \
        { \
            Potentiometer __VA_ARGS__;\
        };\
        Potentiometer pots[n]; \
    };

#define UIControl_Rect_Width(r) (1 + (r).x1 - (r).x0)
#define UIControl_Rect_Height(r) (1 + (r).y1 - (r).y0)

void UIControl_Rect_FromCentre(UIControl_Rect *rect, int cx, int cy, int width, int height);
void UIControl_Rect_FromTopLeft(UIControl_Rect *rect, int x0, int y0, int width, int height);

void Potentiometer_Init(Potentiometer *pot, const char *label, int x, int y, bool posIsCentre);
void Potentiometer_SetPos(Potentiometer *pot, int x, int y, bool posIsCentre);
void Potentiometer_SetValue(Potentiometer *pot, f32 value);
void Potentiometer_Draw(Potentiometer *pot);

#endif //UICONTROL_H
