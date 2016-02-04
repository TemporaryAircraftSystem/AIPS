//
// Created by semoro on 04.02.16.
//

#ifndef AIPS_PACKETS_HPP
#define AIPS_PACKETS_HPP

#ifdef __OPENCV_CORE_MAT_HPP__
#include <opencv2/core/mat.hpp>
#endif

#include <boost/asio.hpp>
#include <opencv2/hal/defs.h>
#include "common.hpp"

using namespace boost::asio;


class packet_t {
public:
    int get_id() {
        return id;
    };

protected:
    int id;
};


class camera_packet_t : packet_t {
public:
#ifdef __OPENCV_CORE_MAT_HPP__
    camera_packet_t(cv::Mat mat):camera_packet_t(){
        rows=mat.rows;
        cols=mat.cols;
        data=mat.data;
        size = (unsigned int) (mat.total() * mat.elemSize());
        isMat = true;
    }
#endif

    camera_packet_t() {
        id = 0;
    };

    ~camera_packet_t() {
        if (!isMat)
            delete data;
    }

    static packet_t *read(ip::tcp::socket *);

    static void send(ip::tcp::socket *, packet_t *ptr);

    void send(ip::tcp::socket *);

private:
    int rows, cols;
    //! pointer to the data
    uchar *data;
    bool isMat = false;
    unsigned int size;
};

class position2d_packet_t : packet_t {
public:
    position2d_packet_t(double x, double y, int i) : position2d_packet_t() {
        this->x = x;
        this->y = y;
        this->i = i;
    }

    position2d_packet_t() {
        id = 1;
    };

    ~position2d_packet_t() {

    }

    static packet_t *read(ip::tcp::socket *);

    static void send(ip::tcp::socket *socket, packet_t *ptr) {
        ((position2d_packet_t *) ptr)->send(socket);
    }

    void send(ip::tcp::socket *);

private:
    double x, y;
    int i;
};

typedef packet_t *(*packet_reader)(ip::tcp::socket *);

typedef void (*packet_writer)(ip::tcp::socket *, packet_t *);

extern packet_reader packet_registry_read[];

extern packet_writer packet_registry_write[];

#pragma once
namespace packet_helper {
    extern packet_t *read_packet(ip::tcp::socket *);

    extern void send_packet(ip::tcp::socket *socket, packet_t *packet);

}

#endif //AIPS_PACKETS_HPP
