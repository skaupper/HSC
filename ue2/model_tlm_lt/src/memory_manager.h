#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_initiator_socket.h>

static ofstream fout("memory_manager.log");

// **************************************************************************************
// User-defined memory manager, which maintains a pool of transactions
// **************************************************************************************
class mm : public tlm::tlm_mm_interface {
  typedef tlm::tlm_generic_payload gp_t;

 public:
  mm();

  gp_t* allocate();
  void free(gp_t* trans);

 private:
  struct access {
    gp_t* trans;
    access* next;
    access* prev;
  };

  access* free_list;
  access* empties;

#ifdef DEBUG
  int count;
#endif
};

#endif /* _MEMORY_MANAGER_H_ */
