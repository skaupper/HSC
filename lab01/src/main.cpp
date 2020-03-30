#include <systemc.h>

#include "MyMethod.h"
#include "MyThread.h"


int sc_main(int argc, char *argv[]) {
    // define signals
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> trigger("trigger");

    // declare modules
    MyMethod myMethod("MyMethod");
    MyThread myThread("MyThread");

    // port binding
    myMethod.iTrigger(trigger);
    myThread.iClk(clk);
    myThread.oTrigger(trigger);


    // start simulation
    sc_start(150, SC_NS);

    return 0;
}
