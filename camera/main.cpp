#include "camera.hpp"
#include "camera_connector.hpp"

int main(int argc, char** argv) {
    //TODO: Index parsing
    size_t  idx = 0;


    ip::address master_addr = ip::address::from_string("127.0.0.1");
    camera_connector_t* camera_connector = new camera_connector_t(&master_addr,8934);
    camera_t* cam = new camera_t(2, idx, camera_connector);
    cam->loop();
}