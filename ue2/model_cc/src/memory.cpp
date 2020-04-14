#include "memory.h"

using namespace std;

Memory::Memory(sc_module_name name) : sc_module(name) {
  SC_THREAD(operate);
}

/* Listens for signals */
void Memory::operate() {
  uint8_t num_waitstates;

  while (true) {
    wait(i_clk.posedge_event());

    /* Assert illegal input */
    sc_assert(i_adr < MEMORY_DEPTH);

    if (i_nrst == 0) {
      o_data = 0;
      o_ack = 0;
    } else if (i_cyc && i_stb) {
      /* Random number of waitstates in range 1..10 */
      num_waitstates = (rand() % 10) + 1;
      while (num_waitstates--)
        wait(i_clk.posedge_event());

      if (i_we) {
        data[i_adr] = i_data;
      } else {
        o_data = data[i_adr];
      }
      o_ack = 1;
      wait(i_clk.posedge_event());
      o_ack = 0;
      o_data = 0;
      wait(SC_ZERO_TIME);
    }
  }
}
