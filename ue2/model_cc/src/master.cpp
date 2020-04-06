#include "master.h"

using namespace std;

uint32_t Master::singleRead(uint32_t addr) {
  cout << name() << "::singleRead()  @ " << sc_time_stamp() << endl;

  wait(i_clk.posedge_event());
  o_adr = addr;
  o_sel = 0xFF;
  o_we = 0;
  o_stb = 1;
  o_cyc = 1;

  // while (!i_ack)
  //  wait(i_clk.posedge_event());

  wait(i_ack.posedge_event());
  wait(i_clk.posedge_event());  // TODO: need to wait for i_ack AND i_clk here?
  uint32_t rd_data = i_data;
  o_stb = 0;
  o_cyc = 0;

  return rd_data;
}

void Master::singleWrite(uint32_t addr, uint32_t data) {
  cout << name() << "::singleWrite() @ " << sc_time_stamp() << endl;

  wait(i_clk.posedge_event());
  o_adr = addr;
  o_data = data;
  o_sel = 0xFF;
  o_we = 1;
  o_stb = 1;
  o_cyc = 1;

  //  wait(i_ack.posedge_event());
  wait(i_clk.posedge_event());  // TODO: need to wait for i_ack AND i_clk here?
  o_stb = 0;
  o_cyc = 0;
}

// ----------------------------------------------------------------------- //

Master::Master(sc_module_name name) : sc_module(name) {
  SC_THREAD(doStimulate);
}

void Master::doStimulate() {
  srand(time(nullptr));

  uint32_t wr_data;
  uint32_t rd_data;

  /* Perform write-readback-check on all addresses */
  for (uint32_t addr = 0; addr < Memory::memory_depth_c; addr++) {
    cout << "addr = " << addr << endl;
    wr_data = addr * 3;

    singleWrite(addr, wr_data);
    rd_data = singleRead(addr);

    sc_assert(rd_data == wr_data);
  }

  /* Perform write-readback-check on all addresses */
  uint32_t addr;
  for (uint32_t i = 0; i < Memory::memory_depth_c; i++) {
    wr_data = rand();
    addr = rand() % Memory::memory_depth_c;

    singleWrite(addr, wr_data);
    rd_data = singleRead(addr);

    sc_assert(rd_data == wr_data);
  }
}
