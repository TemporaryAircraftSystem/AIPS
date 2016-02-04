#include <iostream>
#include "server.hpp"

using namespace std;

int main() {
    cout << "Starting AIPS HUB!" << endl;
    cout << "Binding on port 8934";
    server_t *server = server_t::start(ip::address::from_string("127.0.0.1"), 8934);
    server->run();
    return 0;
}