#include "device_provider.h"

#define PORT 9001

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

	vr::VRDriverLog()->Log("Loading open_scale");

	// Start with a reset scale
	vr::VRSettings()->SetFloat(vrchat_id, vr::k_pch_SteamVR_WorldScale_Float, 1.f);

	// Should start the OSC server thread here to read scale
	std::thread t1(&DeviceProvider::MainLoop, this);
	this->oscThread = std::unique_ptr<std::thread>(&t1);
	t1.detach();

	return vr::VRInitError_None;
}

void DeviceProvider::MainLoop() {
	vr::VRDriverLog()->Log("Thread Started");

	this->listener.setCallback([](float scale) {
		vr::VRSettings()->SetFloat(vrchat_id, vr::k_pch_SteamVR_WorldScale_Float, scale);
		});

	vr::VRDriverLog()->Log("Callback Set");

	for (;;) {
		this->listener.receive();
	}
}

void DeviceProvider::Cleanup() {
	VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

const char* const* DeviceProvider::GetInterfaceVersions() {
	return vr::k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {

}

bool DeviceProvider::ShouldBlockStandbyMode() {
	return false;
}

void DeviceProvider::EnterStandby() {

}

void DeviceProvider::LeaveStandby() {

}