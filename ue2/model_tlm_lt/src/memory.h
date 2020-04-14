#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <systemc.h>
#include <tlm_utils/simple_target_socket.h>

#include "parameters.h"

SC_MODULE(Memory) {
  /* target socket, with default settings */
  tlm_utils::simple_target_socket<Memory> mSocket;

  SC_CTOR(Memory);

  virtual void b_transport(tlm::tlm_generic_payload & trans, sc_time & delay);

 private:
  int mMem_data[MEMORY_DEPTH];
};

#endif /* _MEMORY_H_ */
