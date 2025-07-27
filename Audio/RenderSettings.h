//
// Created by shawn on 7/23/2025.
//

#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H

struct RenderSettings {
public:
    typedef DWORD STREAMING_FLAGS;

    bool stopOnSilence = false;
    AUDCLNT_SHAREMODE shareMode = AUDCLNT_SHAREMODE_SHARED;
    WAVEFORMATEX* format = nullptr;
    STREAMING_FLAGS streamFlags = AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM;

};

#endif //RENDERSETTINGS_H
