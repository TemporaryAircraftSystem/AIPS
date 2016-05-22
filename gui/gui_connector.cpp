//
// Created by semoro on 05.02.16.
//

#include <packedmessage.h>
#include <packets.h>
#include "gui_connector.h"


void gui_connector_t::loop() {


    PacketPointer pp(new packets::base_message());
    pp->set_type(packets::base_message_packet_type_SUBSCRIBE);
    packets::base_message_subscribe_t *ptrFrame = new packets::base_message_subscribe_t();
    ptrFrame->set_type(packets::base_message_packet_type_CAMERA_FRAME);
    pp->set_allocated_subscribe(ptrFrame);
    send_packet(socket, pp);
    while (socket->is_open()) {
        m_readbuf.resize(HEADER_SIZE);
        boost::asio::read(*socket, buffer(m_readbuf));
        unsigned int s = m_packet.decode_header(m_readbuf);
        m_readbuf.resize(HEADER_SIZE + s);
        boost::asio::mutable_buffers_1 buf = boost::asio::buffer(&m_readbuf[HEADER_SIZE], s);
        boost::asio::read(*socket, buf);
        m_packet.unpack(m_readbuf);
        handlePacket(m_packet.get_msg());
        m_readbuf.clear();
    }
}

void gui_connector_t::handlePacket(PacketPointer ptr) {
    switch (ptr->type()) {
        case packets::base_message_packet_type::base_message_packet_type_CAMERA_FRAME:
            handleCameraFrame(ptr->camera_frame());
            break;
        case packets::base_message_packet_type::base_message_packet_type_POSITION2D_FRAME:
            handlePosition(ptr->position_frame()); // STUB HERE!
            break;
        default:
            break;
    }
}


void gui_connector_t::handlePosition(packets::base_message_position2d_frame_t t) { //STUB!

}

void gui_connector_t::handleCameraFrame(packets::base_message_camera_frame_t packet) {



    QImage img((const unsigned char *) (packet.data().c_str()), packet.cols(), packet.rows(),
               QImage::Format_RGB888);
    emit frame_received(img);
}

void gui_connector_t::passParams(gui *gui_ptr, ip::address address, uint16_t t) {
    socket = new ip::tcp::socket(service);
    socket->connect(ip::tcp::endpoint(address, t));
    gui_ptr->set_gui_connector((long long) this);
}
