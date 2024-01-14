#include "device_provider.h"
#include <thread>
#include "oscpp/server.hpp"

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

    vr::VRDriverLog()->Log("Loading OpenScale");
    // Should start the OSC server thread here to read scale

    return vr::VRInitError_None;
}

void DeviceProvider::Cleanup() {
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

const char* const* DeviceProvider::GetInterfaceVersions() {
    return vr::k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {
    vr::VRSettings()->SetFloat(vrchat_id, vr::k_pch_SteamVR_WorldScale_Float, 1.f);
}

void DeviceProvider::UpdateThings() {
    //
}

bool DeviceProvider::ShouldBlockStandbyMode() {
    return false;
}

void DeviceProvider::EnterStandby() {

}

void DeviceProvider::LeaveStandby() {

}