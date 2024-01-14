#include "device_provider.h"
#include <thread>

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

    vr::VRDriverLog()->Log("Hello world!");

    const vr::PropertyContainerHandle_t hmd_handle = vr::VRProperties()->TrackedDeviceToPropertyContainer(0);
    float ipd = vr::VRProperties()->GetFloatProperty(hmd_handle, vr::Prop_UserIpdMeters_Float);
    this->original_ipd = ipd;

    // std::thread t1(&DeviceProvider::UpdateThings, this);

    vr::VRSettings()->SetInt32("steamvr", "allowScaling", 2);
    vr::VRSettings()->SetFloat("steamvr", vr::k_pch_SteamVR_WorldScale_Float, 0.5f);

    return vr::VRInitError_None;
}

void DeviceProvider::Cleanup() {
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

const char* const* DeviceProvider::GetInterfaceVersions() {
    return vr::k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {
    /*
    vr::TrackedDevicePose_t hmd_pose = device_poses[0];
    vr::HmdMatrix34_t hmd_matrix = hmd_pose.mDeviceToAbsoluteTracking;
    hmd_matrix.m[0][3] *= this->scale;
    hmd_matrix.m[1][3] *= this->scale;
    hmd_matrix.m[2][3] *= this->scale;

    vr::VRSettings()->SetFloat("steamvr", vr::k_pch_SteamVR_WorldScale_Float, 1.f - ((step++ % 50) / 10));

    // vr::HmdMatrix34_t left_eye = hmd_matrix;
    // vr::HmdMatrix34_t right_eye = hmd_matrix;

    // left_eye.m[0][3] -= (this->current_ipd / 2);
    // right_eye.m[0][3] += (this->current_ipd / 2);
    // vr::VRServerDriverHost()->SetDisplayEyeToHead(0, left_eye, right_eye);
    
    for (int i = 1; i < this->devices; ++i) {
        vr::TrackedDevicePose_t pose = device_poses[i];
        vr::HmdMatrix34_t matrix = pose.mDeviceToAbsoluteTracking;
        hmd_matrix.m[0][3] *= this->scale;
        hmd_matrix.m[1][3] *= this->scale;
        hmd_matrix.m[2][3] *= this->scale;
    }
    */

    float world_scale = vr::VRSettings()->GetFloat("steam.app.438100", vr::k_pch_SteamVR_WorldScale_Float);
    //vr::VRDriverLog()->Log(std::to_string(world_scale).c_str());

    vr::VRSettings()->SetFloat("steam.app.438100", vr::k_pch_SteamVR_WorldScale_Float, 1.f - ((this->step++ % 50) / 100.f));
}

void DeviceProvider::UpdateThings() {
    for (;;) {
        vr::VRServerDriverHost()->GetRawTrackedDevicePoses(0.f, this->device_poses, MAX_TRACKERS);

        for (int i = 0; i < 32; ++i) {
            if (this->device_poses == nullptr) {
                this->devices = i;
                break;
            }
        }

        const vr::PropertyContainerHandle_t hmd_handle = vr::VRProperties()->TrackedDeviceToPropertyContainer(0);
        this->current_ipd = this->original_ipd * this->scale;
        vr::VRProperties()->SetFloatProperty(hmd_handle, vr::Prop_UserIpdMeters_Float, this->current_ipd);
    }
}

bool DeviceProvider::ShouldBlockStandbyMode() {
    return false;
}

void DeviceProvider::EnterStandby() {

}

void DeviceProvider::LeaveStandby() {

}