#include "listener.h"

#include "openvr_driver.h"
#include <sstream>

#define OSC_BUFFER 500
#define OSC_PATH "/avatar/parameters/open_scale"

MyPacketListener::MyPacketListener() {
	Socket.Bind(PORT);
}

void MyPacketListener::handlePacket(const OSCPP::Server::Packet& packet)
{
	if (packet.isBundle()) {
		// Convert to bundle
		OSCPP::Server::Bundle bundle(packet);

		// Get packet stream
		OSCPP::Server::PacketStream packets(bundle.packets());

		// Iterate over all the packets and call handlePacket recursively.
		// Cuidado: Might lead to stack overflow!
		while (!packets.atEnd()) {
			handlePacket(packets.next());
		}
	}
	else {
		// Convert to message
		OSCPP::Server::Message msg(packet);

		// Get argument stream
		OSCPP::Server::ArgStream args(msg.args());

		// Directly compare message address to string with operator==.
		// For handling larger address spaces you could use e.g. a
		// dispatch table based on std::unordered_map.
		if (msg == OSC_PATH) {
			float new_scale = args.float32();
			this->m_callback(new_scale);
		}
	}
}

void MyPacketListener::receive() {
	char buffer[OSC_BUFFER];

	sockaddr_in add = Socket.RecvFrom(buffer, OSC_BUFFER);

	vr::VRDriverLog()->Log("received");
	handlePacket(OSCPP::Server::Packet(buffer, OSC_BUFFER));
}

void MyPacketListener::setCallback(std::function<void(float)> callback) {
	this->m_callback = callback;
}
