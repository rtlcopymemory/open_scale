#pragma once

#include "openvr_driver.h"
#define MAX_TRACKERS 32

class DeviceProvider : public vr::IServerTrackedDeviceProvider {
public:
    // Inherited via IServerTrackedDeviceProvider
    vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override;
    void Cleanup() override;
    const char* const* GetInterfaceVersions() override;
    void RunFrame() override;
    bool ShouldBlockStandbyMode() override;
    void EnterStandby() override;
    void LeaveStandby() override;

    void UpdateThings();

private:
    float scale = 0.5f;
    int step = 1;
    float original_ipd;
    float current_ipd;

    vr::TrackedDevicePose_t device_poses[MAX_TRACKERS];
    int devices = 0;
};