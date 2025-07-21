//
// Created by shawn on 7/16/2025.
//

#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <cstdint>
#include <cstring>

#include <audioclient.h>

#include "Device.h"
#include "TimeSound.h"
#include "WavFileHeader.h"
#include <vector>

#ifndef AUD_CLNT_FLAG_OK
#define AUD_CLNT_FLAG_OK 0
#endif

#define AUD_READ_FILE_FAILED 0x80004005
#define WAV_HEADER_DATA_BYTES 44
#define TIME_MS_MULTIPLIER 1000

#define MIN_VOLUME 0.0f
#define MAX_VOLUME 1.0f

class AudioManager;

typedef TimeSound TIME;

//Represents a WAV file that can be played via the AudioManager.
class Sound {

private:
    std::string wavFilePath;
    std::ifstream wavFStream;
    WavFileHeader* fileFormat = nullptr;
    WAVEFORMATEX* audioStreamFormat = nullptr;
    long long msElapsed = 0;
    TIME* time = nullptr;
    float volume = 1.0f;
    bool isPaused = false;
    bool isStopped = false;
    bool isPlaying = false;

    //Close the sound file.
    void close();

public:

    static std::vector<Sound*> sounds;

    //Loop audio after finishing
    bool loop = false;

    explicit Sound(const std::string& wavFPath) {
        //Load the file path and get some kind of buffer for the AudioManager.
        wavFilePath = wavFPath;
        sounds.push_back(this);
    }

    //Load data into the buffer from the file. Internal usage.
    HRESULT loadData(UINT32 numFramesAvailable, BYTE* bufferData, DWORD* audioFlags);

    //Read and get the WAV files header.
    WavFileHeader* getFileHeader();

    //This function is used to add time to the elapsed counter, not to change the time
    void addTime(long long dwMilliseconds);

    //Get the total time elapsed since the file started playing.
    TIME getTimeElapsed() const;

    //Get the total duration of the file.
    TIME* getTotalDuration();

    //Set the time of the playback, returns true if the time is within the bounds of the total time.
    [[nodiscard]] bool setTime(const TIME& time);


    //Determine if the sound file is open
    [[nodiscard]] bool isOpen();

    //Set the volume ( 0.0 -> 1.0 )
    void setVolume(float vol);

    //Get the current volume
    float getVolume() const;

    //Set volume to 0
    void mute();

    //Determines if volume is equal to 0
    bool muted() const;

    //Set flag to pause audio
    void pause();

    //Set flag to continue audio. Does not play it.
    void resume();

    //Stops the audio completly.
    void stop();

    //Determine if the audio has been stopped.
    bool stopped() const;

    //Determine if the audio has been paused.
    bool paused() const;

    //Set time to TIME(0)
    bool restart();

    WAVEFORMATEX* getAudioSystemFormat() const;
    void setAudioSystemFormat(WAVEFORMATEX* format);

    std::string getFilePath() const;
};



#endif //SOUND_H
