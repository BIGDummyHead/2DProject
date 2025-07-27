//
// Created by shawn on 7/27/2025.
//

#include "SoundManager.h"

SoundManager* SoundManager::instance;

SoundManager *SoundManager::getInstance() {
    return instance;
}


SoundPack *SoundManager::getSoundPack() const {
    return soundPack;
}

std::vector<Sound *> *SoundManager::getSounds(const PACK_NAME &name) const {
    std::vector<Sound *> *pack;
    getSoundPack()->getPack(name, &pack);

    //does not matter if null
    return pack;
}

void SoundManager::setVolumeOfPack(const PACK_NAME &name, const float &volume) const {
    getSoundPack()->setVolume(name, volume);
}

void SoundManager::startThread() {
    audioThread = new std::thread([this]() {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

        if (FAILED(hr)) {
            this->audioRenderingError = hr;
            return;
        }

        //Start rendering our audio packs on the device with the settings and setup the monitoring.
        hr = AudioManager::startRendering(
            this->audioDevice,
            *this->soundPack,
            *this->settings,
            &this->resultMonitor
        );

        if (FAILED(hr)) {
            this->audioRenderingError = hr;
        }

        CoUninitialize();
    });

    audioThread->detach();
}

bool SoundManager::threadActive() const {
    return audioThread != nullptr && audioThread->joinable();
}


HRESULT SoundManager::getError() const {
    return this->audioRenderingError;
}

RenderResult *SoundManager::getRenderingResult() {
    if(!threadActive()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "The audio thread for the sound manager is not active.");
    }

    return &resultMonitor;
}




