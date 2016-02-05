#include <boost/program_options.hpp>

#include "camera.hpp"
#include "camera_connector.hpp"

namespace po = boost::program_options;

void load_po( int argc, char ** argv, Point3d & cam_pos, string & ip, size_t & idx, size_t & obj_num )
{
    po::options_description opts("Camera module of AIPS system\nCopyright by Temporary Aircraft System");

    opts.add_options()
          ("help,h", "show help")
          ("cam-pos", po::value<std::vector<double>>()->multitoken(), "camera position in world coordinates (x, y, z)")
          ("ip", po::value<std::string>(&ip), "camera ip address (for local working set address: 127.0.0.1)")
          ("id", po::value<size_t>(&idx), "camera index")
          ("obj-num,n", po::value<size_t>(&obj_num), "tracked objects number");

    po::variables_map vm;

    try {
        po::parsed_options parsed_ops = po::command_line_parser(argc, argv).options(
              opts).allow_unregistered().run();

        po::store(parsed_ops, vm);
        po::notify(vm);
    } catch (...) {
        cerr << "Bad program options usage! Example of using options:" << endl;
        cerr << "./<program name> --cam-pos 1 1 1 --ip 192.168.0.1 --id 0 --obj-num 2" << endl << endl;

        throw opts;
    }

    if (vm.empty() || vm.count("help"))
        throw opts;

    vector<double> pos_tmp(3);

    if (!vm["cam-pos"].empty() && (pos_tmp = vm["cam-pos"].as<vector<double>>()).size() == 3) {
        cam_pos.x = pos_tmp[0];
        cam_pos.y = pos_tmp[1];
        cam_pos.z = pos_tmp[2];
    }
}

int main(int argc, char** argv)
{
    //TODO: Index parsing
    size_t   idx
           , obj_num;
    Point3d cam_pos;
    string ip;

    try{
        load_po(argc, argv, cam_pos, ip, idx, obj_num);
    }
    catch(const po::options_description & desc)
    {
        cout << desc;

        return 1;
    }
    catch(...) {
        return 1;
    }

    ip::address master_addr = ip::address::from_string(ip.c_str());

    camera_connector_t * camera_connector = new camera_connector_t(&master_addr, 8934);

    camera_t * cam = new camera_t(cam_pos, obj_num, idx, camera_connector);

    cam->loop();
}