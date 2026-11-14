//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef UICONTROL_H
#define UICONTROL_H

#include <stdbool.h>

typedef struct UIControl
{
    bool isSelected;

    struct
    {
        int x0, y0, x1, y1;
    } rect;
} UIControl;

typedef struct Potentiometer
{
    UIControl baseControl;

    int   centerX;
    int   centerY;
    int   radius;
    int   degreeRange;
    int   value;
    char *label;
} Potentiometer;

#define POTENTIOMETER_SIDE_MARGIN 7
#define POTENTIOMETER_RADIUS_F 11.25
#define POTENTIOMETER_RADIUS_I ((int)POTENTIOMETER_RADIUS_F)
#define POTENTIOMETER_POS_Y 28
#define POTENTIOMETER_DEGREE_RANGE 300

#define POTS_ARRAY(n, ...) \
union \
{ \
struct \
{ \
Potentiometer __VA_ARGS__;\
};\
Potentiometer pots[n]; \
};

#define UICONTROL_ARRAY(n) UIControl* controlPtrs[n];

void Potentiometer_SetText(char *txt);
void Potentiometer_InitMany(Potentiometer *pots, int count);
void Potentiometer_Draw(Potentiometer *pot);

#endif //UICONTROL_H
