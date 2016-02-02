//
// Created by anton on 02.02.16.
//

#include <iostream>
#include "camera_connector.hpp"
#include <algorithm>
/**
 * Connects to HUB, two sockets at (basePort, basePort+1)
 */
//TODO: Use port array instead of basePort + addr
camera_connector_t::camera_connector_t(ip::address *master_addr_ptr, uint16_t basePort) {
    try {
        socketImage->connect(ip::tcp::endpoint(*master_addr_ptr,basePort));
        socketPosition->connect(ip::tcp::endpoint(*master_addr_ptr,basePort+1));
    } catch (boost::system::system_error e) {
        std::cerr << "Could not connect to HUB, program stopped" << std::endl;
        std::cerr << e.code() << std::endl;
        exit(1);
    }
}


void camera_connector_t::frame_is_ready(cv::Mat mat) {
    int* w = &mat.cols;
    int* h =& mat.rows;
    socketImage->send(buffer((void*)w,sizeof(mat.cols)));
    socketImage->send(buffer((void*)h,sizeof(mat.rows)));
    unsigned int buffer_size = std::numeric_limits<unsigned short>::max();
    u_char buf[buffer_size];
    unsigned int sended = 0;
    unsigned int size = (unsigned int) (mat.total() * mat.elemSize());
    while((size-sended)>0){
        size_t bytes = std::min((size-sended),buffer_size);
        memcpy(buf,mat.data+sended,bytes);
        socketImage->send(buffer(buf,buffer_size));
        sended+=bytes;
    }
}


void camera_connector_t::position_is_ready(double x, double y, int i) {
    socketPosition->send(buffer((void*)&x,sizeof(x)));
    socketPosition->send(buffer((void*)&y,sizeof(y)));
    socketPosition->send(buffer((void*)&i,sizeof(i)));
}