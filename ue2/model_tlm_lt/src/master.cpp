#include "master.h"

#include "StopWatch.h"
#include "memory.h"
#include "testcases.h"

Master::Master(sc_module_name name) : sc_module(name) {
  SC_THREAD(stimuli_process);
}

void Master::stimuli_process() {
  uint32_t wr_data;
  uint32_t rd_data;

  cout << "### Perform write on all addresses ###" << endl;
  stw::Start();
  for (uint32_t addr = 0; addr < MEMORY_DEPTH; addr++) {
    wr_data = addr * 3;
    singleWrite(addr, wr_data);
  }
  cout << "Took " << stw::Stop() << " seconds.\n" << endl;

  cout << "### Perform read on all addresses ###" << endl;
  stw::Start();
  for (uint32_t addr = 0; addr < MEMORY_DEPTH; addr++) {
    rd_data = singleRead(addr);

    sc_assert(rd_data == addr * 3);
  }
  cout << "Took " << stw::Stop() << " seconds.\n" << endl;

  cout << "### Perform " << NUM_RANDOM_TESTS
       << " write-read-checks on random addresses ###" << endl;
  stw::Start();
  uint32_t addr;
  for (uint32_t i = 0; i < NUM_RANDOM_TESTS; i++) {
    wr_data = rand();
    addr = rand() % MEMORY_DEPTH;

    singleWrite(addr, wr_data);
    rd_data = singleRead(addr);

    sc_assert(rd_data == wr_data);
  }
  cout << "Took " << stw::Stop() << " seconds.\n" << endl;

  cout << "### Test sequence done [" << sc_time_stamp() << "] ###" << endl;
  sc_stop();
}

void Master::prepareTransactionDefaultParams(tlm::tlm_generic_payload* trans) {
  // configure standard set of attributes
  trans->set_data_length(4);         // length of data in bytes
  trans->set_streaming_width(4);     // width of streaming burst, for
                                     // non streaming set value equal
                                     // to data length
  trans->set_byte_enable_length(0);  // must not be set because the
                                     // ptr is set to 0
  trans->set_byte_enable_ptr(0);     // set to 0 to indicate that byte
                                     // enables are unused
  trans->set_dmi_allowed(false);     // will maybe set by the target
                                     // to indicate a DMI (direct
                                     // memory interface)
  // status may be set by the target
  trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
}

uint32_t Master::singleRead(uint32_t addr) {
  int rd_data;

  tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
  sc_time delay;

  prepareTransactionDefaultParams(trans);

  /* Set specific parameters for read operation */
  trans->set_command(tlm::TLM_READ_COMMAND);
  trans->set_address(addr);
  /* Pointer to the buffer, in which data will be written by the memory slave */
  trans->set_data_ptr(reinterpret_cast<unsigned char*>(&rd_data));

  /* Call b_transport function of target ->
   * (It's implemented in the target, but executed by the initiator) */
  mSocket->b_transport(*trans, delay);

  /* Initiator needs to check response status */
  if (trans->is_response_error()) {
    char txt[100];
    sprintf(txt,
            "Response error from b_transport, response status = %s",
            trans->get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
  }

#if (OUTPUT_DEBUG_MSG == true)
  cout << "trans = { R, " << hex << addr << " } , data = " << hex << rd_data
       << " at time " << sc_time_stamp() << " delay = " << delay << endl;
#endif

  /* Realize the delay that was annotated onto the transport call */
  wait(delay);

  return rd_data;
}

void Master::singleWrite(uint32_t addr, uint32_t data) {
  tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
  sc_time delay;

  prepareTransactionDefaultParams(trans);

  /* Set specific parameters for write operation */
  trans->set_command(tlm::TLM_WRITE_COMMAND);
  trans->set_address(addr);
  /* Pointer to the buffer, of which data will be read from by the slave */
  trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));

  /* Call b_transport function of target ->
   * (It's implemented in the target, but executed by the initiator) */
  mSocket->b_transport(*trans, delay);

  /* Initiator needs to check response status */
  if (trans->is_response_error()) {
    char txt[100];
    sprintf(txt,
            "Response error from b_transport, response status = %s",
            trans->get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
  }

#if (OUTPUT_DEBUG_MSG == true)
  cout << "trans = { W, " << hex << addr << " } , data = " << hex << data
       << " at time " << sc_time_stamp() << " delay = " << delay << endl;
#endif

  /* Realize the delay that was annotated onto the transport call */
  wait(delay);
}
