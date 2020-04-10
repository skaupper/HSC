#ifndef _TOP_H_
#define _TOP_H_

#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include "master.h"
#include "memory.h"

SC_MODULE(top) {
  Master *master;
  Memory *memory;

  SC_CTOR(top) {
    // Instantiate components
    master = new Master("initiator");
    memory = new Memory("target");

    // Bind initiator socket to target socket
    master->mSocket.bind(memory->mSocket);
  }
};

#endif /* _TOP_H_ */
