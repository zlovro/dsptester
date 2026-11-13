//
// Created by lovro on 18/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

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

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define clamp(a, mn, mx) ((a) > (mx) ? (mx) : ((a) < (mn) ? (mn) : (a)))
#define lerp(a, b, t) ((a) + ((b) - (a)) * (t))

#define sq(x) ((x) * (x))

#define pstruct struct __attribute__((packed))
#define DMA_BUFFER __attribute__((section(".dma_buffer")))
#define RAM_D2_BUFFER DMA_BUFFER
#define BDMA_BUFFER __attribute__((section(".bdma_buffer")))
#define RAM_D3_BUFFER BDMA_BUFFER
#define RAM_BUFFER __attribute__((section(".sram")))

#define M_PI_F 3.14159265358979323846F
#define M_TWOPI_F (2 * M_PI_F)
#define M_SQ2_F 1.41421356237309504880F
#define M_1SQ2_F (1.0F / M_SQ2_F)

#define DB_0  1.0F
#define DB_1  1.1220184543F
#define DB_2  1.41253754462F
#define DB_6  1.99526231497F
#define DB_20 10.0F

#define DB_N100 0.00001F
#define DB_N80  0.0001F
#define DB_N60  0.001F
#define DB_N40  0.01F
#define DB_N20  0.1F

#endif //TYPES_H
