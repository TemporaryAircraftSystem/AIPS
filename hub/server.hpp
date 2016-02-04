//
// Created by semoro on 04.02.16.
//

#ifndef AIPS_SERVER_HPP
#define AIPS_SERVER_HPP


#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include "client_handler.hpp"

using namespace boost::asio;

class server_t {
public:
    static server_t *start(ip::address addr, u_int16_t port) {
        if (!instance)
            instance = new server_t(addr, port);
        return instance;
    }

    void createAcceptor();

    void run();

    static server_t *instance;
private:

    io_service service;
    ip::tcp::acceptor *acceptor;

    server_t(ip::address, u_int16_t port);

    static void handle_accept(client_handler_t::ptr client, const boost::system::error_code &err);
};


#endif //AIPS_SERVER_HPP
