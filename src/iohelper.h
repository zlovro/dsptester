//
// Created by lovro on 13/11/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef IOHELPER_H
#define IOHELPER_H

#include <types.h>
#include <fx/effect.h>

#define IO_DefFunc(t) \
    static inline u32 IO_Write_ ## t (t x, void* buf) \
    { *(t*)buf = x; return sizeof(t); } \
    static inline t IO_ReadToVal_ ## t (void* buf) \
    { return *(t*)buf; } \
    static inline u32 IO_ReadToPtr_ ## t (t* out, void* buf) \
    { *out = *(t*)buf; return sizeof(t); }

IO_DefFunc(f32);
IO_DefFunc(u16);
IO_DefFunc(EffectId);

#endif //IOHELPER_H
