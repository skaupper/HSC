#include "master.h"

Master::Master(sc_module_name name) : sc_module(name) {
  SC_THREAD(stimuli_process);
}

void Master::stimuli_process() {
}

static void prepareTransactionDefaults(tlm::tlm_generic_payload* trans) {
  // configure standard set of attributes
  // Initialize 8 out of the 10 attributes, byte_enable_length and
  // extensions being unused
  trans->set_data_length(4);         // length of data in bytes
  trans->set_streaming_width(4);     // width of streaming burst, for
                                     // non streaming set value equal
                                     // to data length
  trans->set_byte_enable_length(0);  // must not be set because the
                                     // ptr is set to 0
  trans->set_byte_enable_ptr(0);     // set to 0 to indicate that byte
                                     // enables are unused
  trans->set_dmi_allowed(false);     // will may be set by the target
                                     // to indicate a DMI (direct
                                     // memory interface)
  // status may be set by the target
  trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
}

uint32_t Master::singleRead(uint32_t addr) {
  int data;

  tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
  // set delay of transaction
  sc_time delay = sc_time(20, SC_NS);
  // TLM command object
  tlm::tlm_command cmd;

  // Read back written data
  // prepare transaction -> set parameter
  prepareTransactionDefaults(trans);
  // set specific parameter for read operation
  cmd = tlm::TLM_READ_COMMAND;
  trans->set_command(cmd);   // read cmd
  trans->set_address(addr);  // address for access
  // pointer to a data buffer
  trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));

  // call b_transport function of target ->
  // implemented by the target, executed by the initiator
  mSocket->b_transport(*trans, delay);

  // Initiator obliged to check response status and delay
  if (trans->is_response_error()) {
    char txt[100];
    sprintf(txt,
            "Response error from b_transport, response status = %s",
            trans->get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
  }

#ifdef DEBUG
  cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << addr
       << " } , data = " << hex << data << " at time " << sc_time_stamp()
       << " delay = " << delay << endl;
#endif

  // Realize the delay annotated onto the transport call
  wait(delay);

  return data;
}

void Master::singleWrite(uint32_t addr, uint32_t data) {
  tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
  // set delay of transaction
  sc_time delay = sc_time(20, SC_NS);
  // TLM command object
  tlm::tlm_command cmd;

  prepareTransactionDefaults(trans);
  // set specific parameters
  cmd = tlm::TLM_WRITE_COMMAND;
  trans->set_command(cmd);   // write cmd
  trans->set_address(addr);  // address for access
  // pointer to a data buffer
  trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));

  // call b_transport function of target ->
  // implemented by the target, executed by the initiator
  mSocket->b_transport(*trans, delay);

  // Initiator obliged to check response status and delay
  if (trans->is_response_error()) {
    char txt[100];
    sprintf(txt,
            "Response error from b_transport, response status = %s",
            trans->get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
  }

#ifdef DEBUG
  cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << addr
       << " } , data = " << hex << data << " at time " << sc_time_stamp()
       << " delay = " << delay << endl;
#endif

  // Realize the delay annotated onto the transport call
  wait(delay);
}
