#include "packets.pb.h"
#include "packedmessage.h"
#include <vector>
#include <boost/asio.hpp>

typedef boost::shared_ptr<packets::base_message> PacketPointer;

void send_packet(boost::asio::ip::tcp::socket *m_socket, PacketPointer resp) {
    std::vector<uint8_t> writebuf;
    PackedMessage<packets::base_message> resp_msg(resp);
    resp_msg.pack(writebuf);
    boost::asio::write(*m_socket, boost::asio::buffer(writebuf));
}