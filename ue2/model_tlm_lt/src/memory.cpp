#include "memory.h"

Memory::Memory(sc_module_name name) : sc_module(name), mSocket("bus_rw") {
  // register callbacks for incoming interface method calls
  mSocket.register_b_transport(this, &Memory::b_transport);

  srand(time(nullptr));
}

/*********************************************************
  TLM 2 blocking transport method
*********************************************************/
void Memory::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
  // read parameters of transaction object
  tlm::tlm_command cmd = trans.get_command();
  uint64_t adr = trans.get_address();
  // unsigned int *  ptr = reinterpret_cast<unsigned int
  // *>(trans.get_data_ptr());
  unsigned char* ptr = trans.get_data_ptr();
  unsigned int len = trans.get_data_length();
  unsigned char* byt = trans.get_byte_enable_ptr();
  unsigned int wid = trans.get_streaming_width();

  // decode transaction and check parameters
  if (adr >= (uint64_t)MEMORY_DEPTH) {
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    SC_REPORT_ERROR(
        "TLM-2", "Target does not support given generic payload transaction");
    return;
  }

  if (byt != 0) {
    trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    return;
  }

  if (len > 4 || wid != len) {
    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return;
  }

  // read or write data -> can be copied by memcpy or direct access via pointer
  if (cmd == tlm::TLM_READ_COMMAND) {
    memcpy(ptr, &mMem_data[adr], len);
  } else if (cmd == tlm::TLM_WRITE_COMMAND) {
    memcpy(&mMem_data[adr], ptr, len);
  }

  /* Set DMI hint to indicate that DMI is not supported */
  trans.set_dmi_allowed(false);

  // finaly set the response status attribute of the gerneric payload to
  // indicate successful and complete transaction
  trans.set_response_status(tlm::TLM_OK_RESPONSE);

  /* Random number of "waitstates" in range 1..10 "clk cycles" */
  delay = sc_time(CLK_PERIOD_NS, SC_NS) * ((rand() % 10) + 1);
}
