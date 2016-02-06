//
// Created by anton on 02.02.16.
//

#include <iostream>
#include <opencv2/imgproc.hpp>


extern "C" {
#include "x264.h"
}
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

void camera_connector_t::initX264(int w, int h) {
    x264_param_t x264Param;
    x264_param_default_preset(&x264Param, "ultrafast", "zerolatency");
    x264Param.i_threads = 1;
    x264Param.i_width = w;
    x264Param.i_height = h;
    x264Param.i_fps_num = 30;
    x264Param.i_fps_den = 1;
    x264Param.i_keyint_max = 30;
    x264Param.b_intra_refresh = 1;
    x264Param.rc.i_rc_method = X264_RC_CRF;
    x264Param.rc.f_rf_constant = 25;
    x264Param.rc.f_rf_constant_max = 35;
    x264Param.b_repeat_headers = 1;
    x264Param.b_annexb = 1;
    x264Param.i_log_level = X264_LOG_DEBUG;
    x264_param_apply_profile(&x264Param, "baseline");
    encoder = x264_encoder_open(&x264Param);
    x264_picture_alloc(&pic_in, X264_CSP_I420, w, h);
    pic_in.img.i_plane = 3;
    pic_in.img.i_stride[0] = w;
    pic_in.img.i_stride[1] = w;
    pic_in.img.i_stride[2] = w;
    pic_in.i_type = X264_TYPE_AUTO;
    colorpack.h = h;
    colorpack.w = w;
    colorpack.r = (uchar *) malloc(w * h);
    colorpack.g = (uchar *) malloc(w * h);
    colorpack.b = (uchar *) malloc(w * h);
}

PacketPointer ptr(new packets::base_message());
int frameNumber = 0;
void camera_connector_t::frame_is_ready(cv::Mat mat) {
    if (encoder == nullptr) {
        std::cout << "init x264\n";
        initX264(mat.cols, mat.rows);
    }
    cv::Mat myuv(mat.rows + mat.rows / 2, mat.cols, CV_8UC1);
    cv::cvtColor(mat, myuv, cv::COLOR_RGB2YUV_I420);
    std::cout << mat.channels() << std::endl;

    pic_in.img.plane[0] = myuv.data;
    pic_in.i_pts = frameNumber;
    frameNumber++;
    int i_nals = 0;
    int frame_size = x264_encoder_encode(encoder, &nals, &i_nals, &pic_in, &pic_out);
    if (frame_size > 0) {
        if (frame_size > buffer_size_) {
            std::cout << "Resized buf\n";
            if (buffer_size_ > 0)
                free(frame);
            buffer_size_ = frame_size;
            frame = (uint8_t *) malloc(buffer_size_);
        }

        int p = 0;
        for (int i = 0; i < i_nals; i++) {
            x264_nal_encode(encoder, frame + p, &nals[i]);
            p += nals[i].i_payload;
        }

        packets::base_message_camera_frame_t *ptrFrame = new packets::base_message_camera_frame_t();
        ptr->set_type(packets::base_message::CAMERA_FRAME);
        ptrFrame->set_cols(mat.cols);
        ptrFrame->set_rows(mat.rows);
        ptrFrame->set_data(frame, 100);
        ptr->set_allocated_camera_frame(ptrFrame);
        send_packet(socket, ptr);
    }
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