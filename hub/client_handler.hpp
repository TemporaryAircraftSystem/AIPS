//
// Created by semoro on 2/5/16.
//

#ifndef AIPS_CLIENT_HANDLER_HPP
#define AIPS_CLIENT_HANDLER_HPP


class client_handler {

};

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <packets.h>




// Database server. The constructor starts it listening on the given
// port with the given io_service.
//
class hub_server {
public:
    hub_server(boost::asio::io_service &io_service, unsigned port);

    ~hub_server();

private:
    hub_server();

    void start_accept();

    struct hub_server_impl;
    boost::scoped_ptr<hub_server_impl> d;
};


#endif //AIPS_CLIENT_HANDLER_HPP
