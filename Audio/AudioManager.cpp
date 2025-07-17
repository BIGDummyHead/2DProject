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
HRESULT AudioManager::startRendering(const Device *usingDevice, Sound *sound) {
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

    //File wave format
    WAVEFORMATEX *fw_Format = sound->getFileHeader()->getAsWavFormatEx();

    //Audio system wave format
    WAVEFORMATEX *as_Format;
    result = a_Client->GetMixFormat(&as_Format);
    if (logFail(result, "Failed to get the mixer format.")) {
        return result;
    }

    const DWORD streamingFlags =
            sameFormat(fw_Format, as_Format)
                ? 0
                : AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY;


    constexpr REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;

    result = a_Client->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        streamingFlags,
        hnsRequestedDuration,
        0,
        fw_Format,
        nullptr);

    if (logFail(result, "Failed to initialize the interface audio client")) {
        return result;
    }

    result = sound->setFormat(fw_Format);
    if (logFail(result, "Failed to set the sound's format")) {
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

    BYTE *bufferData;

    result = a_RenderClient->GetBuffer(bufferFrameCount, &bufferData);
    if (logFail(result, "Failed to get the buffer from the audio render client.")) {
        return result;
    }

    DWORD flags = 0;
    result = sound->loadData(bufferFrameCount, bufferData, &flags);
    if (logFail(result, "Failed to load data into the sound's buffer")) {
        return result;
    }


    result = a_RenderClient->ReleaseBuffer(bufferFrameCount, flags);
    if (logFail(result, "Failed to release the buffer first time for Rendering client")) {
        return result;
    }

    REFERENCE_TIME hnsActualDuration =
            (double) REFTIMES_PER_SEC * bufferFrameCount / fw_Format->nSamplesPerSec;

    //the padding to the buffer
    UINT32 numFramesPadding;

    //The actual number of frames avaiable to write.
    result = a_Client->Start();

    if (logFail(result, "Failed to start rendering audio client")) {
        return result;
    }

    while (flags != AUDCLNT_BUFFERFLAGS_SILENT) {
        //sleep for half the buffer duration
        Sleep(static_cast<DWORD>(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

        result = a_Client->GetCurrentPadding(&numFramesPadding);

        if (logFail(result, "Failed to get the padding for the audio rendering.")) {
            return result;
        }

        const UINT32 numFramesAvaiable = bufferFrameCount - numFramesPadding;

        result = a_RenderClient->GetBuffer(numFramesAvaiable, &bufferData);
        if (logFail(result, "Failed to get the buffer for the device for audio rendering.")) {
            return result;
        }

        result = sound->loadData(numFramesAvaiable, bufferData, &flags);
        if (logFail(result, "Failed to load data into the sound's buffer")) {
            return result;
        }

        result = a_RenderClient->ReleaseBuffer(numFramesAvaiable, flags);
        if (logFail(result, "Failed to release the buffer into the audio render client")) {
            return result;
        }
    }

    Sleep(static_cast<DWORD>(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

    result = a_Client->Stop();
    if (logFail(result, "Failed to stop the audio client")) {
        return result;
    }

    return S_OK;
}

