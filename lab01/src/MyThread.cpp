#include "MyThread.h"


MyThread::MyThread(sc_module_name name) : sc_module(name) {
    SC_THREAD(doWork);
}

void MyThread::doWork() {
    while (true) {
        wait(iClk.posedge_event());
        std::cout << "Method: " << name() << " triggered @ " << sc_time_stamp() << std::endl;
        oTrigger = !oTrigger;
    }
}
