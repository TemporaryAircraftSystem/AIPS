//
// Created by semoro on 05.02.16.
//

#include <packedmessage.h>
#include "gui_connector.hpp"

gui_connector_t::gui_connector_t(gui *gui_ptr, ip::address master_addr_ptr, uint16_t basePort) : m_packet(
        boost::shared_ptr<packets::base_message>(new packets::base_message())) {
    socket = new ip::tcp::socket(service);
    socket->connect(ip::tcp::endpoint(master_addr_ptr, basePort));
    this->gui_ptr = gui_ptr;
}

void gui_connector_t::loop() {
    PacketPointer pp(new packets::base_message());
    pp->set_type(packets::base_message_packet_type_SUBSCRIBE);
    packets::base_message_subscribe_t *ptrFrame = new packets::base_message_subscribe_t();
    ptrFrame->set_type(packets::base_message_packet_type_CAMERA_FRAME);
    pp->set_allocated_subscribe(ptrFrame);
    send_packet(socket, pp);
    while (socket->is_open()) {
        m_readbuf.resize(HEADER_SIZE);
        socket->receive(buffer(m_readbuf));
        unsigned int s = m_packet.decode_header(m_readbuf);
        m_readbuf.resize(HEADER_SIZE + s);
        socket->receive(buffer(m_readbuf));
        m_packet.unpack(m_readbuf);
        handlePacket(m_packet.get_msg());
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
    gui_ptr->redraw(QImage((const unsigned char *) (packet.data().c_str()), packet.cols(), packet.rows(),
                           QImage::Format_RGB888));
}
