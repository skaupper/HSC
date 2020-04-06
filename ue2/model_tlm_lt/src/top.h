#ifndef _TOP_H_
#define _TOP_H_

#include <systemc.h>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include "master.h"
#include "memory.h"
#include "tlm2_base_protocol_checker.h"

SC_MODULE(top) {
  Master *master;
  Memory *memory;
  tlm_utils::tlm2_base_protocol_checker<> *checker;

  SC_CTOR(top) {
    // Instantiate components
    master = new Master("initiator");
    memory = new Memory("target");
    checker = new tlm_utils::tlm2_base_protocol_checker<>("checker");

    // Bind initiator socket to target socket
    master->socket.bind(checker->target_socket);
    checker->initiator_socket(memory->socket);
  }
};

#endif /* _TOP_H_ */
