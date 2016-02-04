//
// Created by anton on 02.02.16.
//

#include <iostream>
#include "camera_connector.hpp"
#include "packets.hpp"
/**
 * Connects to HUB, two sockets at (basePort, basePort+1)
 */
//TODO: Use port array instead of basePort + addr
camera_connector_t::camera_connector_t(ip::address *master_addr_ptr, uint16_t basePort) {
    try {
        socket->connect(ip::tcp::endpoint(*master_addr_ptr, basePort));
    } catch (boost::system::system_error e) {
        std::cerr << "Could not connect to HUB, program stopped" << std::endl;
        std::cerr << e.code() << std::endl;
        exit(1);
    }
}


void camera_connector_t::frame_is_ready(cv::Mat mat) {
    camera_packet_t *packet = new camera_packet_t(mat);
    packet_helper::send_packet(socket, (packet_t *) packet);

}


void camera_connector_t::position_is_ready(double x, double y, int i) {
    position2d_packet_t *packet = new position2d_packet_t(x, y, i);
    packet_helper::send_packet(socket, (packet_t *) packet);
    delete packet;

}