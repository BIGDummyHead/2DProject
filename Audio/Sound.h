//
// Created by shawn on 7/16/2025.
//

#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>
#include <cstdint>
#include <cstring>

#include "WavFileHeader.h"

#ifndef AUD_CLNT_FLAG_OK
#define AUD_CLNT_FLAG_OK 0
#endif
#include <cmath>

#define AUD_READ_FILE_FAILED 0x80004005
#define WAV_HEADER_DATA_BYTES 44;

//Represents a WAV file that can be played via the AudioManager.
class Sound {

private:
    std::string wavFilePath;
    std::ifstream wavFStream;
    WAVEFORMATEX* waveFormat = nullptr;
    WavFileHeader* waveFileHeader = nullptr;
public:
    explicit Sound(const std::string& wavFPath) {
        //Load the file path and get some kind of buffer for the AudioManager.
        wavFilePath = wavFPath;
    }

    HRESULT loadData(UINT32 numFramesAvailable, BYTE* bufferData, DWORD* audioFlags);
    HRESULT setFormat(WAVEFORMATEX* w_Format);
    WavFileHeader* getFileHeader();

};



#endif //SOUND_H
