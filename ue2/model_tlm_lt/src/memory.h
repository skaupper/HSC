#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <systemc.h>
#include <tlm_utils/simple_target_socket.h>

#include "helper.h"

// #define DEBUG_MSG

SC_MODULE(Memory) {
  /* target socket, defaults to 32-bits wide, base protocol */
  tlm_utils::simple_target_socket<Memory> mSocket;

  // constructor: init socket, and all other members
  SC_CTOR(Memory);

  virtual void b_transport(tlm::tlm_generic_payload & trans, sc_time & delay);

  static uint32_t const memory_depth_c = 4096;

 private:
  static uint32_t const clk_period_ns_c = 10;
  int mem_data[memory_depth_c];
};

#endif /* _MEMORY_H_ */
