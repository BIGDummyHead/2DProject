//
// Created by shawn on 7/23/2025.
//

#ifndef RENDERRESULT_H
#define RENDERRESULT_H

struct RenderResult {
public:
    //The audio client to get services from
    IAudioClient* audioClient = nullptr;
    //The master audio volume controller
    ISimpleAudioVolume* masterVolumeController = nullptr;
    //The audio rendering client, reading and writing buffer.
    IAudioRenderClient* renderClient = nullptr;
    //The audio system's format.
    WAVEFORMATEX* audioSystemFormat = nullptr;



};

#endif //RENDERRESULT_H
