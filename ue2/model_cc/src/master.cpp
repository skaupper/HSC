#include "master.h"

#include "testcases.h"

uint32_t Master::singleRead(uint32_t addr) {
  o_adr = addr;
  o_sel = 0xFF;
  o_we = 0;
  o_stb = 1;
  o_cyc = 1;

  wait(i_ack.posedge_event());
  //  wait(i_clk.posedge_event());  // TODO: need to wait for i_ack AND i_clk
  //  here?
  uint32_t rd_data = i_data;
  o_stb = 0;
  o_cyc = 0;

  return rd_data;
}

void Master::singleWrite(uint32_t addr, uint32_t data) {
  o_adr = addr;
  o_data = data;
  o_sel = 0xFF;
  o_we = 1;
  o_stb = 1;
  o_cyc = 1;

  wait(i_ack.posedge_event());
  //  wait(i_clk.posedge_event());  // TODO: need to wait for i_ack AND i_clk
  //  here?
  o_stb = 0;
  o_cyc = 0;
}

// ----------------------------------------------------------------------- //

Master::Master(sc_module_name name) : sc_module(name) {
  SC_THREAD(stimuli_process);
}

void Master::stimuli_process() {
  /* Reset sequence on Memory */
  o_nrst = 1;
  wait(10 * CLK_PERIOD_NS, SC_NS);
  o_nrst = 0;
  wait(15 * CLK_PERIOD_NS, SC_NS);
  o_nrst = 1;
  wait(SC_ZERO_TIME);

  run_test_sequence(*this);

  sc_stop();
}
