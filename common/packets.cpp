//
// Created by semoro on 04.02.16.
//

#include "packets.hpp"


packet_t *camera_packet_t::read(ip::tcp::socket *socket1) {
    camera_packet_t *p = new camera_packet_t;
    //TODO Deserialization!
    return p;
}

void camera_packet_t::send(ip::tcp::socket *socket, packet_t *ptr) {
    camera_packet_t *packet = reinterpret_cast<camera_packet_t *>(ptr);
    packet->send(socket);
}

void camera_packet_t::send(ip::tcp::socket *socket) {
    int *w = &cols;
    int *h = &rows;
    socket->send(buffer((void *) w, sizeof(cols)));
    socket->send(buffer((void *) h, sizeof(rows)));
    unsigned int buffer_size = std::numeric_limits<unsigned short>::max();
    u_char buf[buffer_size];
    unsigned int sended = 0;
    while ((size - sended) > 0) {
        size_t bytes = std::min((size - sended), buffer_size);
        memcpy(buf, data + sended, bytes);
        socket->send(buffer(buf, buffer_size));
        sended += bytes;
    }
}

void position2d_packet_t::send(ip::tcp::socket *socket) {
    socket->send(MKVBUF(x));
    socket->send(MKVBUF(y));
    socket->send(MKVBUF(i));
}


packet_reader packet_registry_read[] = {camera_packet_t::read};
packet_writer packet_registry_write[] = {camera_packet_t::send, position2d_packet_t::send};


packet_t *packet_helper::read_packet(ip::tcp::socket *) {

}

void packet_helper::send_packet(ip::tcp::socket *socket, packet_t *packet) {
    int id = packet->get_id();
    socket->send(MKVBUF(id));
    packet_registry_write[id](socket, packet);
}

