#ifndef _MASTER_H_
#define _MASTER_H_

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "helper.h"

// define to enable several messages
#define DEBUG

SC_MODULE(Master) {
  /* initiator socket, with default settings */
  tlm_utils::simple_initiator_socket<Master> mSocket;

  SC_CTOR(Master);

  void stimuli_process();

  uint32_t singleRead(uint32_t addr);
  void singleWrite(uint32_t addr, uint32_t data);

 private:
};

#endif /* _MASTER_H_ */
