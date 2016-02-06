//
// Created by semoro on 05.02.16.
//

#pragma once

#include <QObject>


#include <stdint.h>
#include <boost/asio.hpp>
#include <packets.h>
#include "gui.h"


using namespace boost::asio;

class gui_connector_t : public QObject {
Q_OBJECT

public:
    gui_connector_t(QObject *parent = 0) : m_packet(
            boost::shared_ptr<packets::base_message>(new packets::base_message())) { };

    void passParams(gui *, ip::address, uint16_t);
    void loop();

    Q_SIGNAL void frame_received(QImage image);

private:
    io_service service;
    std::vector<uint8_t> m_readbuf;
    ip::tcp::socket *socket = new ip::tcp::socket(service);
    PackedMessage<packets::base_message> m_packet;

    void handlePacket(PacketPointer ptr);

    void handlePosition(packets::base_message_position2d_frame_t);

    void handleCameraFrame(packets::base_message_camera_frame_t);


};



