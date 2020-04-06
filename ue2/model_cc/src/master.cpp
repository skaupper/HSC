#include "master.h"

using namespace std;

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
  SC_THREAD(doStimulate);
}

void Master::doStimulate() {
  uint32_t wr_data;
  uint32_t rd_data;
  srand(time(nullptr));

  /* Reset sequence on Memory */
  o_nrst = 1;
  wait(10 * clk_period_ns_c, SC_NS);
  o_nrst = 0;
  wait(15 * clk_period_ns_c, SC_NS);
  o_nrst = 1;
  wait(SC_ZERO_TIME);

  cout << "### Perform write-read-check on all addresses ###" << endl;
  for (uint32_t addr = 0; addr < Memory::memory_depth_c; addr++) {
    wr_data = addr * 3;

    singleWrite(addr, wr_data);
    rd_data = singleRead(addr);

    sc_assert(rd_data == wr_data);
  }

  cout << "### Perform 10^6 write-read-check on random addresses ###" << endl;
  uint32_t addr;
  for (uint32_t i = 0; i < pow(10, 3); i++) {
    wr_data = rand();
    addr = rand() % Memory::memory_depth_c;

    singleWrite(addr, wr_data);
    rd_data = singleRead(addr);

    sc_assert(rd_data == wr_data);
  }

  cout << "### Test sequence done. ###" << endl;
}
