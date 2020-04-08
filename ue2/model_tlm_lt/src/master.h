#ifndef _MASTER_H_
#define _MASTER_H_

#include <tlm.h>

#include "helper.h"
#include "memory_manager.h"

// define to enable several messages
#define DEBUG

// **************************************************************************************
// Initiator module generating multiple pipelined generic payload transactions
// **************************************************************************************
SC_MODULE(Master) {
  // initiator socket -> using default settings (32-bits wide, base protocol,
  // etc.)
  tlm_utils::simple_initiator_socket<Master> socket;

  // CTOR
  SC_CTOR(Master);

  /*********************************************************
      Process of the module
  *********************************************************/
  void thread_process();

  /*********************************************************
      TLM-2 backward non-blocking transport method
  *********************************************************/
  virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload & trans,
                                             tlm::tlm_phase & phase,
                                             sc_time & delay);

  /**************************************************************************
      Payload event queue callback to handle transactions from target
      Transaction could have arrived through return path or backward path
  **************************************************************************/
  void peq_cb(tlm::tlm_generic_payload & trans, const tlm::tlm_phase& phase);

  /**************************************************************************
      Called on receiving BEGIN_RESP or TLM_COMPLETED
  **************************************************************************/
  void check_transaction(tlm::tlm_generic_payload & trans);

 private:
  mm m_mm;
  int data[16];
  tlm::tlm_generic_payload* request_in_progress;
  sc_event end_request_event;
  tlm_utils::peq_with_cb_and_phase<Master> m_peq;
};

#endif /* _MASTER_H_ */
