#include <iostream>
#include <omp.h>
#include "gui.h"
#include "gui_connector.hpp"

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    gui guic;
    guic.show();
#pragma omp parallel num_threads(2) shared(guic)
    {
        size_t thread_idx = omp_get_thread_num();

        if (thread_idx == 0)
            app.exec();
        else {
            gui_connector_t connector_t(&guic, ip::address::from_string("127.0.0.1"), 8934);
            connector_t.loop();
        }

    }
    return 0;
}