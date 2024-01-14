#pragma once

#include "openvr_driver.h"

// This can be found in C:\Program Files (x86)\Steam\config\steamvr.vrsettings
// Change the values manually from the "Per-App Video Settings" and see which one
// Changes. (Or just change the last number, it's the steam game ID)
const char vrchat_id[] = "steam.app.438100";

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
};