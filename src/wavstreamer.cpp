//
// Created by lovro on 19/06/2026.
// Copyright (c) 2026 lovro. All rights reserved.
//

#include <fstream>

#include <windows.h>
#include <mmsystem.h>

#include "wavstreamer.hpp"

extern "C"
{
    #include "dsp.h"
}

void WAVStreamer_Start(const std::filesystem::path &path)
{
    PlaySoundA(NULL, NULL, NULL);

    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::ofstream outFile("tmp.wav", std::ios::out | std::ios::binary);

    file.seekg(40);

    u32 dataSize;
    file.read((char *) &dataSize, 4);

    outFile.write("RIFF", 4);
    const uint32_t chunkSize = 36 + dataSize;
    outFile.write((const char*)&chunkSize, 4);
    outFile.write("WAVE", 4);

    // 2. Write "fmt " Subchunk
    outFile.write("fmt ", 4);
    const uint32_t subchunk1Size = 16;
    outFile.write((const char*)&subchunk1Size, 4); // Subchunk size (16 for PCM)

    const uint16_t audioFormat = 1;
    outFile.write((const char*)&audioFormat, 2);    // Audio format (1 = PCM)

    const uint16_t numChannels = 1;
    outFile.write((const char*)&numChannels, 2);    // Channels (1 = Mono)

    const uint32_t sampleRate = SAMPLE_RATE;
    outFile.write((const char*)&sampleRate, 4);     // Sample Rate (192kHz)

    const uint32_t byteRate = SAMPLE_RATE * 1 * (16 / 8);
    outFile.write((const char*)&byteRate, 4);       // Byte Rate (384000 bytes/sec)

    const uint16_t blockAlign = 1 * (16 / 8);
    outFile.write((const char*)&blockAlign, 2);     // Block Align (2 bytes)

    const uint16_t bitsPerSample = 16;
    outFile.write((const char*)&bitsPerSample, 2);  // Bits per sample (16-bit)

    // 3. Write "data" Subchunk Header
    outFile.write("data", 4);
    outFile.write((const char*)&dataSize, 4);       // Size of raw audio data

    int chunks = dataSize / DSP_CHUNK_SIZE;

    size_t tookTotalUs = 0;
    for (int i = 0; i < chunks; i++)
    {
        s16 tmpBuf[ARRAY_CNT(DSP_WorkBuf)];
        file.read((char*)tmpBuf, sizeof(tmpBuf));

        for (int j = 0; j < ARRAY_CNT(DSP_WorkBuf); ++j)
        {
            DSP_WorkBuf[j] = tmpBuf[j] / 32768.0F;
        }

        auto timestamp_frameStart = std::chrono::system_clock::now();
        DSP_Process();
        tookTotalUs += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timestamp_frameStart).count();

        for (int j = 0; j < ARRAY_CNT(DSP_WorkBuf); ++j)
        {
            tmpBuf[j] = (s16)(DSP_WorkBuf[j] * 32767.0F);
        }

        outFile.write((char *) tmpBuf, sizeof(tmpBuf));

        // while (true)
        // {
        //     long long frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timestamp_frameStart).count();
        //
        //     if (frameDuration >= frameDuration_us)
        //     {
        //         break;
        //     }
        // }
    }

    file.close();
    outFile.close();

    PlaySoundA("tmp.wav", NULL, SND_FILENAME | SND_ASYNC);

    f32 avg = (f32) tookTotalUs / chunks;
    printf("Average DSP_Process time - %.2fus (scaled to 480MHz - approx. %.2fus)\n", avg, avg * 200);
}
