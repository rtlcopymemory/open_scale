#pragma once

#include "oscpp/server.hpp"
#include <oscpp/print.hpp>
#include "Network.h"

#include <stdexcept>

#include <functional>

#define PORT 9001

class MyPacketListener {
public:
    MyPacketListener();

    void setCallback(std::function<void(float)> callback);
    void receive();

    float scale = 1.f;

private:
    void handlePacket(const OSCPP::Server::Packet& packet);

    std::function<void(float)> m_callback;
    WSASession Session;
    UDPSocket Socket;
};