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

#include <audioclient.h>
#include "TimeSound.h"
#include "WavFileHeader.h"

#ifndef AUD_CLNT_FLAG_OK
#define AUD_CLNT_FLAG_OK 0
#endif

#define AUD_READ_FILE_FAILED 0x80004005
#define WAV_HEADER_DATA_BYTES 44
#define TIME_MS_MULTIPLIER 1000

#define MIN_VOLUME 0.0f
#define MAX_VOLUME 1.0f

typedef TimeSound TIME;

//Represents a WAV file that can be played via the AudioManager.
class Sound {

private:
    ISimpleAudioVolume* a_Volume = nullptr;
    std::string wavFilePath;
    std::ifstream wavFStream;
    WAVEFORMATEX* waveFormat = nullptr;
    WavFileHeader* waveFileHeader = nullptr;
    long long msElapsed;
    TIME* time = nullptr;
    float volume = 1.0f;
    bool isPaused = false;
    bool isStopped = false;

public:

    bool loop = false;

    explicit Sound(const std::string& wavFPath) {
        //Load the file path and get some kind of buffer for the AudioManager.
        wavFilePath = wavFPath;
    }

    HRESULT loadData(UINT32 numFramesAvailable, BYTE* bufferData, DWORD* audioFlags);
    HRESULT setFormat(WAVEFORMATEX* w_Format);
    WavFileHeader* getFileHeader();

    //This function is used to add time to the elapsed counter, not to change the time
    void addTime(long long dwMilliseconds);
    TIME getTimeElapsed() const;
    TIME* getTotalDuration();
    [[nodiscard]] bool setTime(const TIME& time);

    void close();
    [[nodiscard]] bool isOpen();

    HRESULT setVolume(float vol);
    float getVolume() const;

    void mute();
    bool muted() const;

    //Set flag to pause audio
    void pause();
    //Set flag to continue audio. Does not play it.
    void resume();

    //Set flag to stop audio
    void stop();

    bool stopped() const;
    bool paused() const;

    //Restart the audio from 0 ms
    bool restart();

    void setAudioController(ISimpleAudioVolume* a_Volume);
};



#endif //SOUND_H
