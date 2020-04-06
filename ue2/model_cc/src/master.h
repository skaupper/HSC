#ifndef _MASTER_H
#define _MASTER_H

#include <systemc.h>

//#include "memory.h"

SC_MODULE(Master) {
  sc_in<bool> i_clk;
  sc_out<bool> o_nrst;

  sc_out<uint32_t> o_adr;
  sc_inout<uint32_t> io_data;
  sc_out<bool> o_we;
  sc_out<bool> o_cyc;
  sc_out<bool> o_stb;
  sc_out<uint8_t> o_sel;
  sc_in<bool> i_ack;

  SC_CTOR(Master) {
    SC_THREAD(doStimulate);
  }

 private:
  void doStimulate();
};

#endif /* _MASTER_H */
