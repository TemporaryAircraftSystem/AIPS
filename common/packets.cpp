//
// Created by semoro on 05.02.16.
//
#include "packets.h"

void send_packet(boost::asio::ip::tcp::socket *m_socket, PacketPointer resp) {
    std::vector<uint8_t> writebuf;
    PackedMessage<packets::base_message> resp_msg(resp);
    resp_msg.pack(writebuf);
    boost::asio::write(*m_socket, boost::asio::buffer(writebuf));
}