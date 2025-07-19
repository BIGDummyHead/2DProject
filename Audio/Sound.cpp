//
// Created by shawn on 7/16/2025.
//

#include "Sound.h"

#include <audioclient.h>

#include "AudioManager.h"


HRESULT Sound::setAudioSystemFormat(WAVEFORMATEX *w_Format) {
    if(w_Format == nullptr) {
        return E_FAIL;
    }
    audioSystemFormat = w_Format;
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

        if (fileFormat == nullptr)
            return nullptr;

        const auto dSize = fileFormat->dataSize * TIME_MS_MULTIPLIER;
        const auto samples = fileFormat->sampleRate * fileFormat->numChannels * (
                                 fileFormat->bitsPerSample / 8);

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

    if (fileFormat == nullptr) {
        fileFormat = getFileHeader();
    }

    const auto *totalTime = getTotalDuration();
    if (!fileFormat || !totalTime || *totalTime < time) {
        return false;
    }


    //calculate where we want the index to go...
    const auto bytesPerSample = fileFormat->bitsPerSample / 8;
    const auto index =
            WAV_HEADER_DATA_BYTES + time.getSeconds() * fileFormat->sampleRate * fileFormat->numChannels *
            bytesPerSample;


    //clear flags, seek the index required and update the elapsed time
    wavFStream.clear();
    wavFStream.seekg(index, std::ifstream::beg);
    msElapsed = time.getMilliseconds();
    return true;
}


HRESULT Sound::loadData(const UINT32 numFramesAvailable, BYTE *bufferData, DWORD *audioFlags) {
    if (isStopped) {
        //tell the audio manager to stop playing.
        *audioFlags |= AUDCLNT_BUFFERFLAGS_SILENT;
        return S_OK;
    }

    //get the file's format if there is none
    if (!fileFormat) {
        fileFormat = getFileHeader();
        if (!fileFormat)
            return AUD_READ_FILE_FAILED;
    }

    //Read the numFramesAvaiable from the wavFStream, write into the bufferData, and set the audio flags accordingly.
    const long long bytesPerFrame = fileFormat->numChannels * fileFormat->bitsPerSample / 8;
    const long long bytesToRead = numFramesAvailable * bytesPerFrame;
    wavFStream.read(reinterpret_cast<char *>(bufferData), bytesToRead);

    //fill in gaps if looping, so there is not silent audio.
    const auto actualBytesRead = wavFStream.gcount();
    if(actualBytesRead < bytesToRead && loop && restart()) {
        wavFStream.read(reinterpret_cast<char*>(bufferData + actualBytesRead), bytesToRead - actualBytesRead);
    }

    //If we have reached an endpoint to the file, and there is no looping, time to go.
    if ((actualBytesRead < bytesToRead || wavFStream.eof()) && !loop) {
        *audioFlags |= AUDCLNT_BUFFERFLAGS_SILENT;
    } else {
        *audioFlags = 0;
    }

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
    isPlaying = false;
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

HRESULT Sound::play(const bool& onCurrentThread) {
    const auto* device = AudioManager::getDefaultDevice();
    return play(device, onCurrentThread);
}

HRESULT Sound::play(const Device *device, const bool& onCurrentThread) {

    if(isPlaying) {
        return S_FALSE;
    }

    HRESULT result = S_OK;
    if(onCurrentThread) {
        result = AudioManager::startRendering(device, this);
    }else {
        auto thread = AudioManager::getRenderingOnThread(device, this);
        thread.detach();
    }

    isPlaying = SUCCEEDED(result);
    isStopped = !isPlaying;

    return result;
}



