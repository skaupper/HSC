#ifndef _MEMORY_H
#define _MEMORY_H

#include <systemc.h>

#include "parameters.h"

SC_MODULE(Memory) {
  sc_in<bool> i_clk;
  sc_in<bool> i_nrst;

  sc_in<uint32_t> i_adr;
  sc_in<uint32_t> i_data;
  sc_out<uint32_t> o_data;
  sc_in<bool> i_we;
  sc_in<bool> i_cyc;
  sc_in<bool> i_stb;
  sc_in<uint8_t> i_sel;
  sc_out<bool> o_ack;

  SC_CTOR(Memory);

 private:
  void operate();
  uint32_t data[MEMORY_DEPTH];
};

#endif /* _MEMORY_H */
