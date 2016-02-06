
#include "packedmessage.h"
#include <vector>
#include <boost/asio.hpp>
#include <packets.pb.h>

typedef boost::shared_ptr<packets::base_message> PacketPointer;

extern void send_packet(boost::asio::ip::tcp::socket *m_socket, PacketPointer resp);