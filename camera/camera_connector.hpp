//
// Created by anton on 02.02.16.
//

#ifndef AIPS_CAMERA_CONNECTOR_HPP
#define AIPS_CAMERA_CONNECTOR_HPP

#include <boost/asio.hpp>
#include <opencv2/core/mat.hpp>

using namespace boost::asio;


class camera_connector_t {
public:
    camera_connector_t(ip::address *master_addr_ptr, uint16_t basePort);
    void frame_is_ready(cv::Mat);
    void position_is_ready(double x, double y, int i);
private:
    io_service service;
    ip::tcp::socket* socketImage = new ip::tcp::socket(service);
    ip::tcp::socket* socketPosition = new ip::tcp::socket(service);


};


#endif //AIPS_CAMERA_CONNECTOR_HPP
