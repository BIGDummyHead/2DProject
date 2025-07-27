//
// Created by shawn on 7/23/2025.
//

#ifndef RENDERRESULT_H
#define RENDERRESULT_H

struct RenderResult {

private:
    bool stopping = false;

public:
    //The audio client to get services from
    IAudioClient* audioClient = nullptr;
    //The master audio volume controller
    ISimpleAudioVolume* masterVolumeController = nullptr;
    //The audio rendering client, reading and writing buffer.
    IAudioRenderClient* renderClient = nullptr;
    //The audio system's format.
    WAVEFORMATEX* audioSystemFormat = nullptr;

    //Request to stop the audio stream
    void requestStop() {

        if(audioClient == nullptr) {
            SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Cannot request to stop when AudioClient is null.");
            return;
        }

        stopping = true;
        audioClient = nullptr;
        masterVolumeController = nullptr;
        renderClient = nullptr;
        audioSystemFormat = nullptr;
    }

    [[nodiscard]] bool requestedStop() const {
        return stopping;
    }



};

#endif //RENDERRESULT_H
