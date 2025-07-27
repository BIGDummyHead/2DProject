//
// Created by shawn on 7/27/2025.
//

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <SDL_log.h>
#include <thread>

#include "AudioManager.h"
#include "SoundPack.h"
#include "RenderResult.h"
#include "RenderSettings.h"

//A simple class that utilizes all Sound classes to simplify the API
class SoundManager {
private:
    SoundPack *soundPack = nullptr;
    static SoundManager *instance;

    RenderSettings *settings = nullptr;
    RenderResult resultMonitor;

    Device *audioDevice = nullptr;
    HRESULT audioRenderingError = S_OK;

    std::thread* audioThread = nullptr;

    void startThread();


public:
    typedef std::string PACK_NAME;

    explicit SoundManager(RenderSettings *settings, Device *audioDevice = nullptr) {

        if (instance) {
            SDL_LogError(SDL_LOG_CATEGORY_ASSERT,
                         "You may not create a new Sound Manager, one already exist. Please use SoundManager::getInstance()");
            delete this;
            return;
        }

        if (!settings) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Settings MUST NOT be NULL");
            return;
        }

        this->settings = settings;

        instance = this;
        soundPack = new SoundPack();

        soundPack->addPack(NOISES_BACKGROUND);
        soundPack->addPack(NOISES_MISC);
        soundPack->addPack(NOISES_VFX);

        if (!audioDevice) {
            audioDevice = AudioManager::getDefaultDevice();
        }

        this->audioDevice = audioDevice;

        startThread();
    }

    ~SoundManager() {
        if (soundPack) {
            delete soundPack;
        }

        //release and delete
        audioDevice->release();
        delete audioDevice;

        instance = nullptr;
        delete this;
    }

    const PACK_NAME NOISES_BACKGROUND = "noises_background";
    const PACK_NAME NOISES_MISC = "noises_misc";
    const PACK_NAME NOISES_VFX = "noises_vfx";

    [[nodiscard]] SoundPack *getSoundPack() const;

    //Grab a pack, set the volume of all items.
    void setVolumeOfPack(const PACK_NAME &name, const float &volume) const;

    //Get the sounds in a pack
    std::vector<Sound *>* getSounds(const PACK_NAME &name) const;

    //Is thread active? Non null and joinable
    bool threadActive() const;

    //Get an error that was thrown by the renderer or initialization process. Usually S_OK
    HRESULT getError() const;

    //Gives real time feedback about the ongoing sound playing. Has tooling for WASAPI
    RenderResult* getRenderingResult();


    static SoundManager* getInstance();
};


#endif //SOUNDMANAGER_H
