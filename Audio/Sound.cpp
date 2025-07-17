//
// Created by shawn on 7/16/2025.
//

#include "Sound.h"

#include <audioclient.h>


HRESULT Sound::setFormat(WAVEFORMATEX *w_Format) {
    if(waveFormat != nullptr) {
        return E_FAIL;
    }
    waveFormat = w_Format;
    return S_OK;
}

WavFileHeader *Sound::getFileHeader() {
    wavFStream = std::ifstream(wavFilePath, std::ios::binary);
    if(!wavFStream.is_open())
        return nullptr;

    constexpr int headerDataBytes = WAV_HEADER_DATA_BYTES;
    char headerData[headerDataBytes];

    wavFStream.read(headerData, headerDataBytes);

    auto* headerInfo = new WavFileHeader();
    std::memcpy(headerInfo, headerData, headerDataBytes);
    return headerInfo;
}


HRESULT Sound::loadData(UINT32 numFramesAvailable, BYTE *bufferData, DWORD *audioFlags) {

    if(!waveFileHeader) {
        waveFileHeader = getFileHeader();
        if(!waveFileHeader)
            return AUD_READ_FILE_FAILED;
    }

    //Read the numFramesAvaiable from the wavFStream, write into the bufferData, and set the audio flags accordingly.

    //const long long bytesIntended = numFramesAvailable * waveFormat->nChannels * waveFormat->nAvgBytesPerSec;

    const long long bytesPerFrame = (waveFileHeader->numChannels * waveFileHeader->bitsPerSample) / 8;
    //const long long bytesPerFrame = (waveFormat->nChannels * waveFormat->wBitsPerSample) / 8;
    const long long bytesToRead = numFramesAvailable * bytesPerFrame;

    char fileDataRead[bytesToRead];
    // Read PCM data from wavFStream directly into bufferData
    wavFStream.read(fileDataRead, bytesToRead);

    std::memcpy(bufferData, fileDataRead, bytesToRead);

    if(wavFStream.eof() || bufferData == nullptr) {
        *audioFlags |= AUDCLNT_BUFFERFLAGS_SILENT;
    }
    else {
        *audioFlags = 0;
    }

    //std::cout << "Read " << wavFStream.gcount() << " bytes from WAV" << std::endl;

    return S_OK;
}




