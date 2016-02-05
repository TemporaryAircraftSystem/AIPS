//
// Created by semoro on 2/5/16.
//

#include "client_handler.hpp"


//
// db_server.cpp: hub_server implementation
//
// Eli Bendersky (eliben@gmail.com)
// This code is in the public domain
//

#include "packets.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


using namespace std;
namespace asio = boost::asio;
using asio::ip::tcp;
using boost::uint8_t;


#define DEBUG true


class client_handler_t : public boost::enable_shared_from_this<client_handler_t> {
public:
    typedef boost::shared_ptr<client_handler_t> Pointer;


    static Pointer create(asio::io_service &io_service) {
        return Pointer(new client_handler_t(io_service));
    }

    tcp::socket &get_socket() {
        return m_socket;
    }

    void start() {
        start_read_header();
    }

private:
    tcp::socket m_socket;
    vector<uint8_t> m_readbuf;
    PackedMessage<packets::base_message> m_packed_request;

    client_handler_t(asio::io_service &io_service)
            : m_socket(io_service),
              m_packed_request(boost::shared_ptr<packets::base_message>(new packets::base_message())) {
    }

    void handle_read_header(const boost::system::error_code &error) {
        //DEBUG && (cerr << "handle read " << error.message() << '\n');
        if (!error) {
            DEBUG && (cerr << "Got header!\n");
            //DEBUG && (cerr << show_hex(m_readbuf) << endl);
            unsigned msg_len = m_packed_request.decode_header(m_readbuf);
            DEBUG && (cerr << msg_len << " bytes\n");
            start_read_body(msg_len);
        }
    }

    void handle_read_body(const boost::system::error_code &error) {
        DEBUG && (cerr << "handle body " << error << '\n');
        if (!error) {
            DEBUG && (cerr << "Got body!\n");
            //DEBUG && (cerr << show_hex(m_readbuf) << endl);
            handle_request();
            start_read_header();
        }
    }

    // Called when enough data was read into m_readbuf for a complete request
    // message.
    // Parse the request, execute it and send back a response.
    //
    void handle_request() {
        if (m_packed_request.unpack(m_readbuf)) {
            PacketPointer req = m_packed_request.get_msg();
            handle_dispatch(req);
/*
            vector<uint8_t> writebuf;
            PackedMessage<stringdb::Response> resp_msg(resp);
            resp_msg.pack(writebuf);
            asio::write(m_socket, asio::buffer(writebuf));*/
        }
    }

    void start_read_header() {
        m_readbuf.resize(HEADER_SIZE);
        asio::async_read(m_socket, asio::buffer(m_readbuf),
                         boost::bind(&client_handler_t::handle_read_header, shared_from_this(),
                                     asio::placeholders::error));
    }

    void start_read_body(unsigned msg_len) {
        // m_readbuf already contains the header in its first HEADER_SIZE
        // bytes. Expand it to fit in the body as well, and start async
        // read into the body.
        //
        m_readbuf.resize(HEADER_SIZE + msg_len);
        asio::mutable_buffers_1 buf = asio::buffer(&m_readbuf[HEADER_SIZE], msg_len);
        asio::async_read(m_socket, buf,
                         boost::bind(&client_handler_t::handle_read_body, shared_from_this(),
                                     asio::placeholders::error));
    }

    void handle_dispatch(PacketPointer req) {

    }
};


struct hub_server::hub_server_impl {
    tcp::acceptor acceptor;


    hub_server_impl(asio::io_service &io_service, unsigned port)
            : acceptor(io_service, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

    void start_accept() {
        // Create a new connection to handle a client. Passing a reference
        // to db to each connection poses no problem since the server is
        // single-threaded.
        //
        client_handler_t::Pointer new_connection =
                client_handler_t::create(acceptor.get_io_service());

        // Asynchronously wait to accept a new client
        //
        acceptor.async_accept(new_connection->get_socket(),
                              boost::bind(&hub_server_impl::handle_accept, this, new_connection,
                                          asio::placeholders::error));
    }

    void handle_accept(client_handler_t::Pointer connection,
                       const boost::system::error_code &error) {
        // A new client has connected
        //
        if (!error) {
            // Start the connection
            //
            connection->start();

            // Accept another client
            //
            start_accept();
        }
    }
};


hub_server::hub_server(asio::io_service &io_service, unsigned port)
        : d(new hub_server_impl(io_service, port)) {
}


hub_server::~hub_server() {
}