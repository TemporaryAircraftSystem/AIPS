//
// Created by semoro on 04.02.16.
//

#ifndef AIPS_CLIENT_HANDLER_H
#define AIPS_CLIENT_HANDLER_H


#include <boost/core/noncopyable.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/system/error_code.hpp>

#include <boost/asio.hpp>

using namespace boost::asio;

class client_handler_t : boost::noncopyable {
    typedef client_handler_t self_type;

    client_handler_t(io_service *service) : sock_(*service), started_(false) { }

public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<client_handler_t> ptr;

    void accepted() {
        started_ = true;
        do_read();
    }

    static ptr new_(io_service *service) {
        ptr new_(new client_handler_t(service));
        return new_;
    }

    void stop() {
        if (!started_) return;
        started_ = false;
        sock_.close();
    }

    ip::tcp::socket &sock() { return sock_; }

    void do_read();

private:
    ip::tcp::socket sock_;
    enum {
        max_msg = 1024
    };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
};


#endif //AIPS_CLIENT_HANDLER_H
