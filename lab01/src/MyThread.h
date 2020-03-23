#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <systemc.h>


SC_MODULE(MyThread) {
    sc_in<bool> iClk;
    sc_out<bool> oTrigger;

    SC_CTOR(MyThread);

private:
    void doWork();
};

#endif
