//
// Created by anton on 02.02.16.
//

#ifndef AIPS_CAMERA_CONNECTOR_HPP
#define AIPS_CAMERA_CONNECTOR_HPP

#include <boost/asio.hpp>
#include <opencv2/core/mat.hpp>

extern "C" {
#include "x264.h"
}
using namespace boost::asio;


struct x264_i_pack {
    uint8_t *r;
    uint8_t *g;
    uint8_t *b;
    int w, h;
};

class camera_connector_t {
public:
    camera_connector_t(ip::address *master_addr_ptr, uint16_t basePort);
    void frame_is_ready(cv::Mat);
    void position_is_ready(double x, double y, int i);
private:
    io_service service;
    ip::tcp::socket *socket = new ip::tcp::socket(service);
    x264_t *encoder = nullptr;
    x264_nal_t *nals;

    void initX264(int w, int h);

    x264_picture_t pic_in;
    x264_picture_t pic_out;
    uint8_t *frame;
    size_t buffer_size_ = 0;
    x264_i_pack colorpack;
};


#endif //AIPS_CAMERA_CONNECTOR_HPP
