//
// Created by lovro on 18/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float  f32;
typedef double f64;

typedef char *      str;
typedef const char *cstr;

#define ARRAY_CNT(a) (sizeof(a) / sizeof(typeof(a[0])))
#define ARRAY_HALF(a) (a + (ARRAY_CNT(a) / 2))
#define ARRAY_END(a) (a + ARRAY_CNT(a))
#define ARRAY_ZERO(a) memset(a, 0, ARRAY_CNT(a) * sizeof(a[0]))

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define sq(x) ((x) * (x))

#define pstruct struct __attribute__((packed))
#define DMA_BUFFER __attribute__((section(".dma_buffer")))
#define RAM_D2_BUFFER DMA_BUFFER
#define BDMA_BUFFER __attribute__((section(".bdma_buffer")))
#define RAM_D3_BUFFER BDMA_BUFFER
#define RAM_BUFFER __attribute__((section(".sram")))

#endif //TYPES_H
