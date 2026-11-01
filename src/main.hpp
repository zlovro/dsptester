//
// Created by lovro on 20/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#ifndef MAIN_HPP
#define MAIN_HPP

typedef enum
{
    ERR_OK,

    ERR_ASIO_INIT = 1000,
    ERR_ASIO_CREATE_BUFFERS,
    ERR_ASIO_START,
    ERR_ASIO_STOP,
    ERR_ASIO_CH_INFO,

    ERR_ASIO_DRV_INIT_COULD_NOT_LOAD_DRIVER = 2000,

    ERR_ASIO_BYPASS = 99000,
} DSP_Error;

extern const bool DSP_WavTest;

#endif //MAIN_HPP
