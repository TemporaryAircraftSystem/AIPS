//
// Created by anton on 02.02.16.
//

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>


#include "camera_connector.hpp"
#include "packets.h"


/**
 * Connects to HUB, two sockets at (basePort, basePort+1)
 */
//TODO: Use port array instead of basePort + addr
camera_connector_t::camera_connector_t(ip::address *master_addr_ptr, uint16_t basePort) {
    try {
        socket->connect(ip::tcp::endpoint(*master_addr_ptr, basePort));
        std::cout << "Connected to HUB\n";
    } catch (boost::system::system_error e) {
        std::cerr << "Could not connect to HUB, program stopped" << std::endl;
        std::cerr << e.code() << std::endl;
        exit(1);
    }
    out.open("/home/semoro/out.264", std::ios::binary | std::ios::out);

}

PacketPointer ptr(new packets::base_message());

int nextPTS() {
    static int static_pts = 0;
    return (static_pts++);
}

void camera_connector_t::frame_is_ready(cv::Mat mat) {
    if (codecContext == nullptr) {
        initCodec(mat.cols, mat.rows);
    }

    uint8_t *inData[1] = {mat.data};
    int inLinesize[1] = {3 * mat.cols};
    sws_scale(swsContext, (const uint8_t *const *) inData, inLinesize, 0, mat.rows, picture->data, picture->linesize);
    int got_packet = 0;
    picture->pts = nextPTS();

    int code = avcodec_encode_video2(codecContext, codecPacket, picture, &got_packet);

    if (got_packet) {
        packets::base_message_camera_frame_t *ptrFrame = new packets::base_message_camera_frame_t();
        ptr->set_type(packets::base_message::CAMERA_FRAME);
        ptrFrame->set_cols(mat.cols);
        ptrFrame->set_rows(mat.rows);
        ptrFrame->set_data(codecPacket->data, codecPacket->size);
        out.write((const char *) codecPacket->data, codecPacket->size);
        out.flush();
        ptr->set_allocated_camera_frame(ptrFrame);
        send_packet(socket, ptr);
    }
    av_free_packet(codecPacket);

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


#define RNDTO2(X) ( ( (X) & 0xFFFFFFFE ) )
#define RNDTO32(X) ( ( (X) % 32 ) ? ( ( (X) + 32 ) & 0xFFFFFFE0 ) : (X) )

void camera_connector_t::initCodec(int w, int h) {

    std::cout << w << " " << h << std::endl;
    codec = avcodec_find_encoder(CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    codecContext = avcodec_alloc_context3(codec);
    picture = av_frame_alloc();
    av_log_set_level(AV_LOG_DEBUG);
    codecContext->width = w;
    codecContext->height = h;
    codecContext->bit_rate = (int) (outputBufSize * 4);

    codecContext->time_base = (AVRational) {1, 30};
    //codecContext->max_b_frames = 0;
    codecContext->gop_size = 10;
    codecContext->pix_fmt = PIX_FMT_YUV420P;

    AVDictionary * codec_options( 0 );
    av_dict_set( &codec_options, "preset", "veryfast", 0 );

    avcodec_open2(codecContext, codec, &codec_options);


    swsContext = sws_getContext(w, h,
                                AV_PIX_FMT_BGR24, w, h,
                                AV_PIX_FMT_YUV420P, 0, 0, 0, 0);


    int width = RNDTO2 (codecContext->width);
    int height = RNDTO2 (codecContext->height);
    int ystride = RNDTO32 (width);
    int uvstride = RNDTO32 (width / 2);
    int ysize = ystride * height;
    int vusize = uvstride * (height / 2);
    int size = ysize + (2 * vusize);


    pictureBuf = (uint8_t *) malloc(size);
    pictureSize = size;

    picture->data[0] = pictureBuf;
    picture->data[1] = pictureBuf + ysize;
    picture->data[2] = pictureBuf + ysize + vusize;
    picture->linesize[0] = ystride;
    picture->linesize[1] = uvstride;
    picture->linesize[2] = uvstride;
    picture->format = codecContext->pix_fmt;
    picture->width = w;
    picture->height = h;

    codecPacket = (AVPacket *) av_mallocz(sizeof(AVPacket));

    av_init_packet(codecPacket);
    codecPacket->data = nullptr;
    codecPacket->size = 0;


    std::cout << "Init avcodec finished\n";
}


