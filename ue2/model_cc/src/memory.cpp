#include "memory.h"

using namespace std;

Memory::Memory(sc_module_name name) : sc_module(name) {
  SC_THREAD(operate);
}

void Memory::operate() {
  while (true) {
    /* listen for signals */
    wait(i_clk.posedge_event());
    // assert illegal input states (re + we)
    // assert input range
  }
}
