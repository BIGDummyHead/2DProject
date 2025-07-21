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
#include <algorithm>
#include <cstdint>
#include <initguid.h>
#include <guiddef.h>
#include <span>
#include <limits>

DEFINE_GUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT,
0x00000003, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

DEFINE_GUID(KSDATAFORMAT_SUBTYPE_PCM,
0x00000001, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

class AudioManager {
private:

    static IMMDeviceEnumerator *deviceEnumerator;

    static bool logFail(const HRESULT &result, const char *logError);

    static IMMDeviceEnumerator *getDeviceEnumerator();

    static Device *createFromIMMDevice(IMMDevice *theDevice, EDataFlow flow, ERole role);

    //Sound mixer functions, jesus christ this was annoying
    static void mixSound(WAVEFORMATEX* as_Format, std::span<std::byte> mixerData, std::span<const std::byte> soundData, const float& volume);

    static void pcm16ConversionToFloat(std::span<float> mixer, std::span<const int16_t> sound, const float& volume);

    template <typename T, typename T_Sum>
    static void mixAudioPCM(std::span<T> mixer, std::span<const T> sound, const float& volume);

    static void mixAudioFloat(std::span<float> mixer, std::span<const float> sound, const float& volume);

public:

    static std::unique_ptr<std::vector<std::unique_ptr<Device> > > getDevices(
        EDataFlow dataFlowType = eRender, ERole role = eMultimedia);

    static Device *getDefaultDevice(EDataFlow dataFlowType = eRender, ERole role = eMultimedia);

    static std::unique_ptr<Device> getDevice(const std::string &id, EDataFlow dataFlowType = eRender,
                                             ERole role = eMultimedia);

    static bool sameFormat(const WAVEFORMATEX *formatA, const WAVEFORMATEX *formatB);

    static HRESULT startRendering(const Device *usingDevice,  std::vector<Sound *> &sounds, ISimpleAudioVolume* masterAudioController, bool stopOnSilence = false);


};



#endif //AUDIOMANAGER_H
