//
// Created by anton on 02.02.16.
//

#ifndef AIPS_CAMERA_CONNECTOR_HPP
#define AIPS_CAMERA_CONNECTOR_HPP

#include <boost/asio.hpp>
#include <opencv2/core/mat.hpp>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include <libswscale/swscale.h>
};
using namespace boost::asio;

#include <fstream>

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


    void initCodec(int w, int h);

    AVCodec *codec;
    AVCodecContext *codecContext = nullptr;
    SwsContext * swsContext = nullptr;
    AVFrame *picture;
    size_t outputBufSize = 100000;
    uint8_t *pictureBuf = nullptr;
    int pictureSize = 0;
    AVPacket* codecPacket = nullptr;
    std::ofstream out;
};


#endif //AIPS_CAMERA_CONNECTOR_HPP
