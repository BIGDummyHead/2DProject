//
// Created by shawn on 7/14/2025.
//

#ifndef DEVICE_H
#define DEVICE_H
#include <codecvt>
#include <locale>
#include <mmdeviceapi.h>
#include <string>
#include <audioclient.h>

#ifndef INITGUID
#define INITGUID
#endif



#define SAFE_RELEASE(punk) if((punk) != NULL){ (punk)->Release(); (punk) = NULL; }

//An audio device.
struct Device {
private:
    IMMDevice *m_Device;
    EDataFlow dataFlowType;
    ERole role;
    std::string id;
    std::string name;
    std::string interfaceName;
    std::string description;

private:
    static std::string getFromLPCWSTR(const LPCWSTR *lStr) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;

        return converter.to_bytes(std::wstring(*lStr));
    }

public:
    explicit Device(IMMDevice *theDevice, const LPCWSTR *id, const LPCWSTR *deviceFriendlyName
                    , const LPCWSTR *deviceInterfaceName, const LPCWSTR *deviceDescription,
                    const EDataFlow dataFlowType, const ERole role) {
        m_Device = theDevice;
        this->id = getFromLPCWSTR(id);
        this->dataFlowType = dataFlowType;
        this->role = role;
        this->name = getFromLPCWSTR(deviceFriendlyName);
        this->interfaceName = getFromLPCWSTR(deviceInterfaceName);
        this->description = getFromLPCWSTR(deviceDescription);
    }


    [[nodiscard]] EDataFlow getFlowType() const {
        return dataFlowType;
    }

    [[nodiscard]] ERole getRole() const {
        return role;
    }

    [[nodiscard]] const std::string &getID() const {
        return id;
    }

    [[nodiscard]] const std::string &getName() const {
        return name;
    }

    [[nodiscard]] const std::string &getInterfaceName() const {
        return interfaceName;
    }

    [[nodiscard]] const std::string &getDescription() const {
        return description;
    }

    [[nodiscard]] IMMDevice *getDevice() const {
        return m_Device;
    }

    void release() {
        SAFE_RELEASE(m_Device);
    }

    static constexpr IID IID_AudioClient = __uuidof(IAudioClient);

    //Simplifies the process of getting a service, simply pass an id and pointer to your service.
    bool getService(const IID service, void* settingService) const {
        if(!m_Device)
            return false;

        const HRESULT result =
            m_Device->Activate(service, CLSCTX_ALL, nullptr, &settingService);

        return SUCCEEDED(result);
    }

    //Get service for IAudioClient.
    [[nodiscard]] IAudioClient* getAudioClient() const {
        IAudioClient* a_Client;
        return getService(IID_AudioClient, &a_Client) ? a_Client : nullptr;
    }


};

#endif //DEVICE_H
