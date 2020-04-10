#include "master.h"
#include "memory.h"

int sc_main(int argc, char *argv[]) {
  // Define signals
  sc_clock clk("clk", Master::CLK_PERIOD_NS, SC_NS);
  sc_signal<bool> nrst;

  sc_signal<uint32_t> adr;
  sc_signal<uint32_t> data_to_mem;
  sc_signal<uint32_t> data_from_mem;
  sc_signal<bool> we;
  sc_signal<bool> cyc;
  sc_signal<bool> stb;
  sc_signal<uint8_t> sel;
  sc_signal<bool> ack;

  // Declare modules
  Master master("master");
  Memory memory("memory");

  // Port binding
  master.i_clk(clk);
  master.o_nrst(nrst);
  master.o_adr(adr);
  master.i_data(data_from_mem);
  master.o_data(data_to_mem);
  master.o_we(we);
  master.o_cyc(cyc);
  master.o_stb(stb);
  master.o_sel(sel);
  master.i_ack(ack);

  memory.i_clk(clk);
  memory.i_nrst(nrst);
  memory.i_adr(adr);
  memory.i_data(data_to_mem);
  memory.o_data(data_from_mem);
  memory.i_we(we);
  memory.i_cyc(cyc);
  memory.i_stb(stb);
  memory.i_sel(sel);
  memory.o_ack(ack);

  // Configure trace file
  sc_trace_file *tf;
  tf = sc_create_vcd_trace_file("bin/model_cc_trace");
  tf->delta_cycles(true);
  sc_trace(tf, clk, "clk");
  sc_trace(tf, nrst, "nrst");

  sc_trace(tf, adr, "adr");
  sc_trace(tf, data_to_mem, "data_to_mem");
  sc_trace(tf, data_from_mem, "data_from_mem");
  sc_trace(tf, we, "we");
  sc_trace(tf, cyc, "cyc");
  sc_trace(tf, stb, "stb");
  sc_trace(tf, sel, "sel");
  sc_trace(tf, ack, "ack");

  sc_start();

  cout << "Simulation took " << sc_delta_count() << " delta cycles." << endl;

  return 0;
}
