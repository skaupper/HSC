#include "master.h"

using namespace std;

static uint32_t singleRead(uint32_t addr) {
  cout << "singleRead()  @ " << sc_time_stamp() << endl;
  wait(i_clk.posedge_event());

  /*
    $display("singleRead @%0tns", $time);

    sigs.cb.adr <= addr;
    sigs.cb.sel <= '1; sigs.cb.we <= 0;
    sigs.cb.stb <= 1;
    sigs.cb.cyc <= 1;

    @(sigs.cb iff(sigs.cb.ack == 1));
    data = sigs.cb.datS;
    sigs.cb.stb <= 0;
    sigs.cb.cyc <= 0;
   */
}

static void singleWrite(uint32_t addr, uint32_t data) {
  cout << "singleWrite() @ " << sc_time_stamp() << endl;
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
  for (int addr = 0; addr < Memory::memory_depth_c; addr++) {
    wr_data = addr * 3;

    singleWrite(addr, wr_data);
    rd_data = singleRead(addr);

    sc_assert(rd_data == wr_data);
  }

  /* Perform write-readback-check on all addresses */
  uint32_t addr;
  for (int i = 0; i < Memory::memory_depth_c; i++) {
    wr_data = rand();
    addr = rand() % Memory::memory_depth_c;

    singleWrite(addr, wr_data);
    rd_data = singleRead(addr);

    sc_assert(rd_data == wr_data);
  }
}
