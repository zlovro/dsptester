//
// Created by lovro on 22.11.2026..
// Copyright (c) 2026 lovro. All rights reserved.
//

#include "emu.h"
#include <sys/timeb.h>

struct timeb EMU_StartTime;

void EMU_Init()
{
    ftime(&EMU_StartTime);
}

u32 EMU_GetTick()
{
    struct timeb t;
    ftime(&t);

    return 1000 * (t.time - EMU_StartTime.time) + (t.millitm - EMU_StartTime.millitm);
}
