//
// Created by semoro on 04.02.16.
//

#include <iostream>
#include "server.hpp"

server_t *server_t::instance = nullptr;

server_t::server_t(ip::address address, u_int16_t port) {
    this->acceptor = new ip::tcp::acceptor(service, ip::tcp::endpoint(address, port));
}

void server_t::run() {
    createAcceptor();
    this->service.run();
}

void server_t::createAcceptor() {
    client_handler_t::ptr client = client_handler_t::new_(&service);
    acceptor->async_accept(client->sock(), boost::bind(handle_accept, client, _1));
}

void server_t::handle_accept(client_handler_t::ptr client, const boost::system::error_code &err) {
    std::cout << "Accepted client!" << std::endl;
    client->accepted();
    instance->createAcceptor();
}
