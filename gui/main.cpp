#include <iostream>
#include <omp.h>
#include "gui.h"

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    gui guic;
    gui_connector_t connector_t;
    connector_t.passParams(&guic, ip::address::from_string("127.0.0.1"), 8934);
    guic.show();
#pragma omp parallel num_threads(2) shared(guic,connector_t)
    {
        size_t thread_idx = omp_get_thread_num();

        if (thread_idx == 0)
            app.exec();
        else {
            connector_t.loop();
        }

    }
    return 0;
}