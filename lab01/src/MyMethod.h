#ifndef MY_METHOD_H
#define MY_METHOD_H

#include <systemc.h>


SC_MODULE(MyMethod) {
    // port definitions
    sc_in<bool> iTrigger;

    // CTOR
    SC_CTOR(MyMethod) {
        SC_METHOD(doWork);
        sensitive << iTrigger.pos();
        dont_initialize();
    }

private:
    void doWork() { std::cout << "Method: " << name() << " triggered @ " << sc_time_stamp() << std::endl; }
};


#endif
