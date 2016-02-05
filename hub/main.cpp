#include <iostream>
#include <boost/asio.hpp>
#include "client_handler.hpp"

using namespace std;

int main() {
    cout << "Starting AIPS HUB!" << endl;
    cout << "Binding on port 8934";
    boost::asio::io_service service;
    hub_server server(service, 8934);
    service.run();
    return 0;
}