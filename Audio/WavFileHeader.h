//
// Created by shawn on 7/17/2025.
//

#ifndef WAVFILEHEADER_H
#define WAVFILEHEADER_H
#include <Windows.h>

struct WavFileHeader {
public:
    // RIFF Chunk
    char riff[4]; // "RIFF"
    DWORD chunkSize;
    char wave[4]; // "WAVE"
    // Format Chunk
    char fmt[4]; // "fmt "
    DWORD fmtSize;
    WORD audioFormat; // 1 for PCM
    WORD numChannels;
    DWORD sampleRate;
    DWORD byteRate;
    WORD blockAlign;
    WORD bitsPerSample;
    // Data Chunk
    char data[4]; // "data"
    DWORD dataSize;

    [[nodiscard]] WAVEFORMATEX *getAsWavFormatEx() const {
        auto *format = new WAVEFORMATEX;

        format->wFormatTag = this->audioFormat;
        format->nChannels = this->numChannels;
        format->nSamplesPerSec = this->sampleRate;
        format->nAvgBytesPerSec = this->byteRate;
        format->nBlockAlign = this->blockAlign;
        format->wBitsPerSample = this->bitsPerSample;

        format->cbSize = 0;

        return format;
    }
};

#endif //WAVFILEHEADER_H
