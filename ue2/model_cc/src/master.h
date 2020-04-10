#ifndef _MASTER_H
#define _MASTER_H

#include <systemc.h>

#include "memory.h"

SC_MODULE(Master) {
  sc_in<bool> i_clk;
  sc_out<bool> o_nrst;

  sc_out<uint32_t> o_adr;
  sc_in<uint32_t> i_data;
  sc_out<uint32_t> o_data;
  sc_out<bool> o_we;
  sc_out<bool> o_cyc;
  sc_out<bool> o_stb;
  sc_out<uint8_t> o_sel;
  sc_in<bool> i_ack;

  SC_CTOR(Master);

  static uint32_t const CLK_PERIOD_NS = 10;

 private:
  void stimuli_process();

  uint32_t singleRead(uint32_t addr);
  void singleWrite(uint32_t addr, uint32_t data);
};

#endif /* _MASTER_H */
