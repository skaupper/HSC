#ifndef _MEMORY_H
#define _MEMORY_H

#include <systemc.h>

SC_MODULE(Memory) {
  sc_in<bool> i_clk;
  sc_in<bool> i_nrst;

  sc_in<uint32_t> i_adr;
  sc_inout<uint32_t> io_data;
  sc_in<bool> i_we;
  sc_in<bool> i_cyc;
  sc_in<bool> i_stb;
  sc_in<uint8_t> i_sel;
  sc_out<bool> o_ack;

  SC_CTOR(Memory);

 private:
  void calc();
};

#endif /* _MEMORY_H */
