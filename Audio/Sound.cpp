//
// Created by shawn on 7/16/2025.
//

#include "Sound.h"

#include <audioclient.h>


HRESULT Sound::setFormat(WAVEFORMATEX *w_Format) {
    if (waveFormat != nullptr) {
        return E_FAIL;
    }
    waveFormat = w_Format;
    return S_OK;
}

WavFileHeader *Sound::getFileHeader() {
    wavFStream = std::ifstream(wavFilePath, std::ios::binary);
    if (!wavFStream.is_open())
        return nullptr;

    constexpr int headerDataBytes = WAV_HEADER_DATA_BYTES;
    char headerData[headerDataBytes];

    wavFStream.read(headerData, headerDataBytes);

    auto *headerInfo = new WavFileHeader();
    std::memcpy(headerInfo, headerData, headerDataBytes);
    return headerInfo;
}

void Sound::addTime(const long long dwMilliseconds) {
    msElapsed += dwMilliseconds;
}

TIME Sound::getTimeElapsed() const {
    const TIME time(msElapsed);
    return time;
}

TIME *Sound::getTotalDuration() {
    if (time == nullptr) {
        if (waveFileHeader == nullptr)
            return nullptr;

        const auto dSize = waveFileHeader->dataSize * TIME_MS_MULTIPLIER;
        const auto samples = waveFileHeader->sampleRate * waveFileHeader->numChannels * (
                                 waveFileHeader->bitsPerSample / 8);

        const auto msTotal = dSize / samples;

        time = new TimeSound(msTotal);
    }

    return time;
}


bool Sound::isOpen() {
    return wavFStream.is_open();
}

void Sound::close() {
    if (isOpen()) {
        wavFStream.close();
    }
}

bool Sound::setTime(const TIME &time) {
    if (!wavFStream.is_open()) {
        return false;
    }

    if (waveFileHeader == nullptr) {
        waveFileHeader = getFileHeader();
    }

    const auto *totalTime = getTotalDuration();
    if (!waveFileHeader || !totalTime || *totalTime < time) {
        return false;
    }


    //calculate where we want the index to go...
    const auto bytesPerSample = waveFileHeader->bitsPerSample / 8;
    const auto index =
            WAV_HEADER_DATA_BYTES + time.getSeconds() * waveFileHeader->sampleRate * waveFileHeader->numChannels *
            bytesPerSample;

    wavFStream.clear();
    wavFStream.seekg(index, std::ifstream::beg);
    msElapsed = time.getMilliseconds();
    return true;
}


HRESULT Sound::loadData(const UINT32 numFramesAvailable, BYTE *bufferData, DWORD *audioFlags) {
    if (isStopped) {
        *audioFlags |= AUDCLNT_BUFFERFLAGS_SILENT;
        return S_OK;
    }

    if (!waveFileHeader) {
        waveFileHeader = getFileHeader();
        if (!waveFileHeader)
            return AUD_READ_FILE_FAILED;
    }

    //Read the numFramesAvaiable from the wavFStream, write into the bufferData, and set the audio flags accordingly.
    const long long bytesPerFrame = waveFileHeader->numChannels * waveFileHeader->bitsPerSample / 8;
    const long long bytesToRead = numFramesAvailable * bytesPerFrame;
    wavFStream.read(reinterpret_cast<char *>(bufferData), bytesToRead);

    const auto actualBytesRead = wavFStream.gcount();

    if(actualBytesRead < bytesToRead && loop && restart()) {
        wavFStream.read(reinterpret_cast<char*>(bufferData + actualBytesRead), bytesToRead - actualBytesRead);
    }

    if ((actualBytesRead < bytesToRead || wavFStream.eof()) && !loop) {
        *audioFlags |= AUDCLNT_BUFFERFLAGS_SILENT;
    } else {
        *audioFlags = 0;
    }

    //std::cout << "Read " << wavFStream.gcount() << " bytes from WAV" << std::endl;

    return S_OK;
}

void Sound::mute() {
    volume = MIN_VOLUME;
}

bool Sound::muted() const {
    return getVolume() == 0;
}

float Sound::getVolume() const {
    return volume;
}

HRESULT Sound::setVolume(float vol) {
    if (vol < MIN_VOLUME)
        vol = MIN_VOLUME;

    vol = vol > MAX_VOLUME ? MAX_VOLUME : vol;

    this->volume = vol;

    //Everything went normal, if the audio volume is null, than master volume could not be set, which is okay
    //If it is not null, set it and return that result
    return a_Volume == nullptr ? S_OK : a_Volume->SetMasterVolume(volume, nullptr);
}

void Sound::pause() {
    isPaused = true;
}

bool Sound::paused() const {
    return isPaused;
}

void Sound::resume() {
    isPaused = false;
}

void Sound::stop() {
    close();
    isStopped = true;
}

bool Sound::stopped() const {
    return isStopped;
}

bool Sound::restart() {
    //not paused, not stopped, and set time is returned true
    return !paused() && !stopped() && setTime(TIME(0));
}

void Sound::setAudioController(ISimpleAudioVolume *a_Volume) {
    this->a_Volume = a_Volume;
}
