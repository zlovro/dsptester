//
// Created by lovro on 20/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef DSP_H
#define DSP_H

#include "types.h"

#define SAMPLE_RATE 48000
#define SAMPLE_TIME_F (1.0F / SAMPLE_RATE)

#define TEST_TRANSFER_FUNC 1

#if TEST_TRANSFER_FUNC
#define DSP_CHUNK_SIZE 8192
#else
#define DSP_CHUNK_SIZE 1024
#endif

extern f32 DSP_WorkBuf[DSP_CHUNK_SIZE];

extern void DSP_Init();
extern void DSP_Process();

#endif //DSP_H
