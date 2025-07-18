//
// Created by shawn on 7/14/2025.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#include "Device.h"
#include <vector>



#include <Windows.h>
#include <functiondiscoverykeys_devpkey.h>
#include <memory>
#include <SDL_log.h>
#include <thread>

#include "Sound.h"


class AudioManager {
private:

    static IMMDeviceEnumerator *deviceEnumerator;

    static bool logFail(const HRESULT &result, const char *logError);

    static IMMDeviceEnumerator *getDeviceEnumerator();

    static Device *createFromIMMDevice(IMMDevice *theDevice, EDataFlow flow, ERole role);

public:

    static std::unique_ptr<std::vector<std::unique_ptr<Device> > > getDevices(
        EDataFlow dataFlowType = eRender, ERole role = eMultimedia);

    static Device *getDefaultDevice(EDataFlow dataFlowType = eRender, ERole role = eMultimedia);

    static std::unique_ptr<Device> getDevice(const std::string &id, EDataFlow dataFlowType = eRender,
                                             ERole role = eMultimedia);

    static bool sameFormat(const WAVEFORMATEX *formatA, const WAVEFORMATEX *formatB);

    static HRESULT startRendering(const Device* usingDevice, Sound* sound);
    static std::thread getRenderingOnThread(const Device* device, Sound* sound);

};


#endif //AUDIOMANAGER_H
