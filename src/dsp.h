//
// Created by lovro on 20/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef DSP_H
#define DSP_H

#include "types.h"

#define SAMPLE_RATE 192000
#define DSP_CHUNK_SIZE 1024

extern f32 DSP_WorkBuf[DSP_CHUNK_SIZE];

void DSP_Process();

#endif //DSP_H
