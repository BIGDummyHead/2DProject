//
// Created by shawn on 7/14/2025.
//

#include "AudioManager.h"


IMMDeviceEnumerator *AudioManager::deviceEnumerator;

bool AudioManager::logFail(const HRESULT &result, const char *logError) {
    const bool failed = FAILED(result);
    if (failed) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, logError);
    }
    return failed;
}

IMMDeviceEnumerator *AudioManager::getDeviceEnumerator() {
    if (deviceEnumerator != nullptr) {
        SAFE_RELEASE(deviceEnumerator);
    }

    IMMDeviceEnumerator *pEnumerator;

    //creating a device enumerator single object...
    constexpr CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    constexpr IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

    //must be called before using cocreate instance
    HRESULT result = CoInitialize(nullptr);
    if (logFail(result, "Failed to initialize the com")) {
        return nullptr;
    }

    result = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator,
                              reinterpret_cast<void **>(&pEnumerator));

    deviceEnumerator =
            logFail(result, "Could not create an instance of the IMMDeviceEnumerator") ? nullptr : pEnumerator;

    return deviceEnumerator;
}


std::unique_ptr<Device> AudioManager::getDevice(const std::string &id, const EDataFlow dataFlowType, const ERole role) {
    const auto devices = getDevices(dataFlowType, role);
    std::unique_ptr<Device> theDevice = nullptr;
    for (auto &device: *devices) {
        if (device->getID() == id) {
            theDevice = std::move(device);
            continue;
        }
        //release any unused device
        device->release();
    }

    return theDevice;
}

Device *AudioManager::getDefaultDevice(const EDataFlow dataFlowType, const ERole role) {
    IMMDeviceEnumerator *dEnum = getDeviceEnumerator();
    if (!dEnum) {
        return nullptr;
    }

    IMMDevice *m_Device;
    const HRESULT result = dEnum->GetDefaultAudioEndpoint(dataFlowType, role, &m_Device);

    if (logFail(result, "Failed to obtain the default audio endpoint.")) {
        return nullptr;
    }

    auto *device = createFromIMMDevice(m_Device, dataFlowType, role);
    return device;
}


std::unique_ptr<std::vector<std::unique_ptr<Device> > > AudioManager::getDevices(
    const EDataFlow dataFlowType, const ERole role) {
    IMMDeviceEnumerator *m_Enumerator = getDeviceEnumerator();

    //Get ready to go over the audio endpoints
    IMMDeviceCollection *m_Collection;
    HRESULT result = m_Enumerator->EnumAudioEndpoints(dataFlowType, role, &m_Collection);
    if (logFail(result, "Failed to enumerate the audio endpoints.")) {
        return nullptr;
    }

    UINT device_Count;
    result = m_Collection->GetCount(&device_Count);
    if (logFail(result, "Failed to get the count of devices.") || device_Count == 0) {
        return nullptr;
    }

    IMMDevice *m_Device;
    auto devices = std::make_unique<std::vector<std::unique_ptr<Device> > >();
    for (ULONG i = 0; i < device_Count; i++) {
        result = m_Collection->Item(i, &m_Device);

        if (logFail(result, "Failed to get item from collection in audio devices.")) {
            continue;
        }

        devices->push_back(std::unique_ptr<Device>(createFromIMMDevice(m_Device, dataFlowType, role)));
        //SAFE_RELEASE(m_Device);
    }

    SAFE_RELEASE(m_Collection);
    SAFE_RELEASE(m_Enumerator);

    return devices;
}

Device *AudioManager::createFromIMMDevice(IMMDevice *theDevice, const EDataFlow flow, const ERole role) {
    if (!theDevice) {
        return nullptr;
    }

    IPropertyStore *p_Store;
    HRESULT result = theDevice->OpenPropertyStore(STGM_READ, &p_Store);

    if (logFail(result, "Failed to open a property store for a device when creaing a Device*")) {
        return nullptr;
    }

    //initialize the properties for reading
    PROPVARIANT deviceFriendlyName;
    PROPVARIANT deviceInterfaceFriendlyName;
    PROPVARIANT deviceDescription;

    //init
    PropVariantInit(&deviceFriendlyName);
    PropVariantInit(&deviceInterfaceFriendlyName);
    PropVariantInit(&deviceDescription);

    //obtain friendly name
    result = p_Store->GetValue(PKEY_Device_FriendlyName, &deviceFriendlyName);
    if (logFail(result, "Failed to obtain the friendly name for a device")) {
        return nullptr;
    }

    //obtain interface name
    result = p_Store->GetValue(PKEY_DeviceInterface_FriendlyName, &deviceInterfaceFriendlyName);
    if (logFail(result, "Failed to obtain the friendly interface name for a device")) {
        return nullptr;
    }

    //obtain description
    result = p_Store->GetValue(PKEY_Device_DeviceDesc, &deviceDescription);
    if (logFail(result, "Failed to obtain the description for a device")) {
        return nullptr;
    }

    LPWSTR id;
    result = theDevice->GetId(&id);

    if (logFail(result, "Failed to obtain the id of a device")) {
        return nullptr;
    }

    auto devicePtr = new Device(theDevice, &id, &deviceFriendlyName.pwszVal,
                                &deviceInterfaceFriendlyName.pwszVal,
                                &deviceDescription.pwszVal, flow, role);

    PropVariantClear(&deviceFriendlyName);
    PropVariantClear(&deviceInterfaceFriendlyName);
    PropVariantClear(&deviceDescription);

    return devicePtr;
}

bool AudioManager::sameFormat(const WAVEFORMATEX *formatA, const WAVEFORMATEX *formatB) {
    return formatA != nullptr && formatB != nullptr &&
           formatA->wFormatTag == formatB->wFormatTag &&
           formatA->nChannels == formatB->nChannels &&
           formatA->nSamplesPerSec == formatB->nSamplesPerSec &&
           formatA->wBitsPerSample == formatB->wBitsPerSample;
}


//Code from https://learn.microsoft.com/en-us/windows/win32/coreaudio/rendering-a-stream

// Helper to mix float samples, clamping the result
void AudioManager::mixAudioFloat(std::span<float> mixer, const std::span<const float> sound, const float &volume) {
    for (size_t i = 0; i < mixer.size(); ++i) {
        const float adjustedSample = sound[i] * volume;
        mixer[i] = std::clamp(mixer[i] + adjustedSample, -1.0f, 1.0f);
    }
}

// Template helper to mix integer PCM samples, preventing overflow
template<typename T, typename T_Sum>
void AudioManager::mixAudioPCM(std::span<T> mixer, std::span<const T> sound, const float &volume) {
    const T min_val = std::numeric_limits<T>::min();
    const T max_val = std::numeric_limits<T>::max();

    for (size_t i = 0; i < mixer.size(); ++i) {
        auto adjustedSample = static_cast<T_Sum>(sound[i] * volume); // apply volume here
        T_Sum sum = static_cast<T_Sum>(mixer[i]) + adjustedSample;
        mixer[i] = static_cast<T>(std::clamp(sum, static_cast<T_Sum>(min_val), static_cast<T_Sum>(max_val)));
    }
}


void AudioManager::pcm16ConversionToFloat(std::span<float> mixer, const std::span<const int16_t> sound,
                                          const float &volume) {
    for (size_t i = 0; i < mixer.size(); ++i) {
        constexpr float pcm16_to_float_divisor = 32768.0f;
        const float converted_sample = (static_cast<float>(sound[i]) / pcm16_to_float_divisor) * volume;
        mixer[i] = std::clamp(mixer[i] + converted_sample, -1.0f, 1.0f);
    }
}

void AudioManager::mixSound(WAVEFORMATEX *as_Format, std::span<std::byte> mixerData,
                            std::span<const std::byte> soundData, const float &volume) {
    if (as_Format->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        auto *wfex = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(as_Format);

        if (wfex->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT && as_Format->wBitsPerSample == 32) {
            // This case assumes the source sound is 16-bit PCM needing conversion.
            pcm16ConversionToFloat(
                {reinterpret_cast<float *>(mixerData.data()), mixerData.size_bytes() / sizeof(float)},
                {reinterpret_cast<const int16_t *>(soundData.data()), soundData.size_bytes() / sizeof(int16_t)}
                , volume);
        } else if (wfex->SubFormat == KSDATAFORMAT_SUBTYPE_PCM && as_Format->wBitsPerSample == 32) {
            mixAudioPCM<int32_t, int64_t>(
                {reinterpret_cast<int32_t *>(mixerData.data()), mixerData.size_bytes() / sizeof(int32_t)},
                {reinterpret_cast<const int32_t *>(soundData.data()), soundData.size_bytes() / sizeof(int32_t)}
                , volume);
        }
    } else if (as_Format->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
        mixAudioFloat(
            {reinterpret_cast<float *>(mixerData.data()), mixerData.size_bytes() / sizeof(float)},
            {reinterpret_cast<const float *>(soundData.data()), soundData.size_bytes() / sizeof(float)}
            , volume);
    } else if (as_Format->wBitsPerSample == 16) {
        mixAudioPCM<int16_t, int32_t>(
            {reinterpret_cast<int16_t *>(mixerData.data()), mixerData.size_bytes() / sizeof(int16_t)},
            {reinterpret_cast<const int16_t *>(soundData.data()), soundData.size_bytes() / sizeof(int16_t)}
            , volume);
    }
}


HRESULT AudioManager::startRendering(const Device *usingDevice, std::vector<Sound *> &sounds,
                                     const RenderSettings &settings, RenderResult *r_Result) {
    HRESULT result = usingDevice == nullptr || usingDevice->getDevice() == nullptr ? E_FAIL : S_OK;

    //using device will not be null.
    if (logFail(result, "The device you wish to render on is null.")) {
        return result;
    }

    //IAudioClient *a_Client = usingDevice->getAudioClient();

    IMMDevice *m_Device = usingDevice->getDevice();
    IAudioClient *a_Client;
    result = m_Device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void **>(&a_Client));
    //    result = a_Client == nullptr ? E_FAIL : S_OK;
    //Ensures that a_Client is not a null pointer.
    if (logFail(result, "Failed to get audio client interface.")) {
        return result;
    }

    WAVEFORMATEX *as_Format = settings.format;

    if (!as_Format) {
        result = a_Client->GetMixFormat(&as_Format);

        if (logFail(result, "Failed to get the audio system's mix format")) {
            return result;
        }
    }


    constexpr REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;



    result = a_Client->Initialize(
        settings.shareMode,
        settings.streamFlags,
        hnsRequestedDuration,
        0,
        as_Format,
        nullptr);

    if (logFail(result, "Failed to initialize the interface audio client")) {
        return result;
    }

    UINT32 bufferFrameCount;
    result = a_Client->GetBufferSize(&bufferFrameCount);

    if (logFail(result, "Failed to get the buffer size.")) {
        return result;
    }

    IAudioRenderClient *a_RenderClient;
    result = a_Client->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void **>(&a_RenderClient));
    if (logFail(result, "Failed to get service IAudioRenderClient from the Audio Client")) {
        return result;
    }

    ISimpleAudioVolume* a_Volume;
    result = a_Client->GetService(__uuidof(ISimpleAudioVolume), reinterpret_cast<void **>(&a_Volume));
    if (logFail(result, "Failed to get an audio volume service")) {
        return result;
    }



    //The actual number of frames avaiable to write.
    result = a_Client->Start();
    if (logFail(result, "Failed to start rendering audio client")) {
        return result;
    }

    //Buffer information
    BYTE *bufferData;
    //audio flags
    //num frames of padding per audio
    UINT32 numFramesPadding;

    *r_Result = RenderResult();
    r_Result->audioClient = a_Client;
    r_Result->audioSystemFormat = as_Format;
    r_Result->renderClient = a_RenderClient;
    r_Result->masterVolumeController = a_Volume;
    while (true) {
        result = a_Client->GetCurrentPadding(&numFramesPadding);
        if (logFail(result, "Failed to get padding of the client")) {
            return result;
        }
        const UINT32 numFramesAvailable = bufferFrameCount - numFramesPadding;

        result = a_RenderClient->GetBuffer(numFramesAvailable, &bufferData);
        if (logFail(result, "Failed to get the buffer for the device for audio rendering.")) {
            return result;
        }

        //determine the buffer to sleep between
        const REFERENCE_TIME hnsActualDuration =
                REFTIMES_PER_SEC * bufferFrameCount / as_Format->nSamplesPerSec;

        const REFERENCE_TIME sleepMs = hnsActualDuration / REFTIMES_PER_MILLISEC / 2;

        const UINT32 bufferSizeBytes = numFramesAvailable * as_Format->nBlockAlign;
        std::vector mixerData(bufferSizeBytes, std::byte{0});

        bool anySoundPlaying = false;

        //Do not use for range looped, since this way we remove items from the sound block when stopped.
        for (size_t i = 0; i < sounds.size(); i++) {
            auto *sound = sounds[i];

            // skip paused or stopped sounds
            if (sound->paused()) {
                continue;
            }

            if (sound->stopped()) {
                sounds.erase(sounds.begin() + i);
                i--;
                continue;
            }

            //Set the audio system format, if non exist.
            if (!sound->getAudioSystemFormat()) {
                sound->setAudioSystemFormat(as_Format);
            }


            std::vector soundData(bufferSizeBytes, std::byte{0});
            DWORD currentDataSoundFlags = 0;
            result = sound->loadData(numFramesAvailable, reinterpret_cast<BYTE *>(soundData.data()),
                                     &currentDataSoundFlags);

            if (logFail(result, "Failed to load data into sound data")) {
                return result;
            }

            if (currentDataSoundFlags == AUDCLNT_BUFFERFLAGS_SILENT)
                continue;

            anySoundPlaying = true;
            sound->addTime(sleepMs);

            mixSound(as_Format, mixerData, soundData, sound->getVolume());
        }

        //only render sound when there was sound detected.
        if (anySoundPlaying) {
            constexpr DWORD flags = 0;
            std::memcpy(bufferData, mixerData.data(), bufferSizeBytes);

            result = a_RenderClient->ReleaseBuffer(numFramesAvailable, flags);
            if (logFail(result, "Failed to release the buffer into the audio render client")) {
                return result;
            }
        } else if (settings.stopOnSilence) {
            //break on silence when no sound playing as well...
            break;
        }

        Sleep(sleepMs);
    }

    result = a_Client->Stop();

    //free resources.
    a_Volume->Release();
    a_RenderClient->Release();
    CoTaskMemFree(as_Format);
    a_Client->Release();

    logFail(result, "Failed to stop the audio client");

    return result;
}
