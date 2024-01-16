#include "device_provider.h"

#define PORT 9001

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

	vr::VRDriverLog()->Log("Loading open_scale");

	// Start with a reset scale
	vr::VRSettings()->SetFloat(vrchat_id, vr::k_pch_SteamVR_WorldScale_Float, 1.f);
	vr::VRSettings()->SetFloat(vrchat_id_secondary, vr::k_pch_SteamVR_WorldScale_Float, 1.f);

	// Should start the OSC server thread here to read scale
	std::thread t1(&DeviceProvider::MainLoop, this);
	this->oscThread = std::unique_ptr<std::thread>(&t1);
	t1.detach();

	return vr::VRInitError_None;
}

float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

void DeviceProvider::MainLoop() {
	vr::VRDriverLog()->Log("Thread Started");

	this->listener.setCallback([](float scale) {
		// Scale is [0, 1] from VRC due to synced parameters limitations.
		// World scale needs to map that [0, 1] -> [10, 0.1] but as [0, 0.5] -> [10, 1] and [0.5, 1] -> [1, 0.1]
		if (0.5f - scale > 0.08f) {
			// [0, 0.5] -> [10, 1]
			scale *= 2;  // [0, 0.5] -> [0, 1]
			scale = lerp(10.f, 1.f, scale);
		}
		else {
			// [0.5, 1] -> [1, 0.1]
			scale = (scale - 0.5f) * 2; // [0.5, 1] -> [0, 1]
			scale = lerp(1.f, 0.1f, scale);
		}
		vr::VRSettings()->SetFloat(vrchat_id, vr::k_pch_SteamVR_WorldScale_Float, scale);
		vr::VRSettings()->SetFloat(vrchat_id_secondary, vr::k_pch_SteamVR_WorldScale_Float, scale);
		vr::VRDriverLog()->Log(std::to_string(scale).c_str());
		});

	vr::VRDriverLog()->Log("Callback Set");

	for (;;) {
		this->listener.receive();
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
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