#include "packets.pb.h"
#include "packedmessage.h"
#include <vector>
#include <boost/asio.hpp>

typedef boost::shared_ptr<packets::base_message> PacketPointer;

extern void send_packet(boost::asio::ip::tcp::socket *m_socket, PacketPointer resp);