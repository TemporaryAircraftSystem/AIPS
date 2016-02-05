//
// Created by semoro on 05.02.16.
//

#ifndef AIPS_GUI_CONNECTOR_HPP
#define AIPS_GUI_CONNECTOR_HPP


#include <stdint.h>
#include <boost/asio.hpp>
#include <packets.h>
#include "gui.h"

using namespace boost::asio;

class gui_connector_t {
public:
    gui_connector_t(gui *gui_ptr, ip::address master_addr_ptr, uint16_t basePort);

    void loop();

private:
    gui *gui_ptr;
    io_service service;
    vector<uint8_t> m_readbuf;
    ip::tcp::socket *socket = new ip::tcp::socket(service);
    PackedMessage<packets::base_message> m_packet;

    void handlePacket(PacketPointer ptr);

    void handlePosition(packets::base_message_position2d_frame_t);

    void handleCameraFrame(packets::base_message_camera_frame_t);
};


#endif //AIPS_GUI_CONNECTOR_HPP
