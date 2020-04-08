#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <systemc.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_target_socket.h>

#include "helper.h"

static ofstream sout("memory.log");

#define DEBUG

// **************************************************************************************
// Target module able to handle two pipelined transactions
// **************************************************************************************
DECLARE_EXTENDED_PHASE(internal_ph);
SC_MODULE(Memory) {
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<Memory> socket;

  // constructor: init socket, and all other members
  SC_CTOR(Memory);

  // TLM-2 non-blocking transport method
  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload & trans,
                                             tlm::tlm_phase & phase,
                                             sc_time & delay);

  void peq_cb(tlm::tlm_generic_payload & trans, const tlm::tlm_phase& phase);

  // Method send an END_REQ to the initiator through backwards path
  tlm::tlm_sync_enum send_end_req(tlm::tlm_generic_payload & trans);

  // Method sends a BEGIN_RESP to initiator through backwards path
  void send_response(tlm::tlm_generic_payload & trans);

 private:
  int n_trans;
  bool response_in_progress;
  tlm::tlm_generic_payload* next_response_pending;
  tlm::tlm_generic_payload* end_req_pending;
  tlm_utils::peq_with_cb_and_phase<Memory> m_peq;
};

#endif /* _MEMORY_H_ */
