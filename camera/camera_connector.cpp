//
// Created by anton on 02.02.16.
//

#include <iostream>
#include "camera_connector.hpp"
#include "packets.h"
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
    PacketPointer ptr(new packets::base_message());
    packets::base_message_camera_frame_t *ptrFrame = new packets::base_message_camera_frame_t();
    ptr->set_type(packets::base_message::CAMERA_FRAME);
    ptrFrame->set_cols(mat.cols);
    ptrFrame->set_rows(mat.rows);
    ptrFrame->set_data(mat.data, mat.total() * mat.elemSize());
    ptr->set_allocated_camera_frame(ptrFrame);
    send_packet(socket, ptr);
}


void camera_connector_t::position_is_ready(double x, double y, int i) {
    PacketPointer ptr(new packets::base_message());
    ptr->set_type(packets::base_message::POSITION2D_FRAME);
    packets::base_message_position2d_frame_t *ptrFrame = new packets::base_message_position2d_frame_t();
    ptrFrame->set_x(x);
    ptrFrame->set_y(y);
    ptrFrame->set_i(i);
    ptr->set_allocated_position_frame(ptrFrame);
    send_packet(socket, ptr);
}