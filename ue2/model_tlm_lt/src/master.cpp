// #include "master.h"

// #include "memory_manager.h"

// Master::Master(sc_module_name name)
//     : sc_module(name),
//       socket("bus_rw"),
//       request_in_progress(0),       // pointer to a transaction
//       m_peq(this, &Master::peq_cb)  // event queue for pending notifications
// {
//   // Register callbacks for incoming interface method calls
//   socket.register_nb_transport_bw(this, &Master::nb_transport_bw);

//   // process -> doing some r/w accesses
//   SC_THREAD(thread_process);
// }

// void Master::thread_process() {
//   // payload object
//   tlm::tlm_generic_payload* trans;
//   // TLM phase
//   tlm::tlm_phase phase;
//   // delay
//   sc_time delay;

//   // init random function
//   srand(time(NULL));
//   // Generate a sequence of random r/w transactions
//   for (int i = 0; i < 5; i++) {
//     // generate random adr
//     int adr = rand();
//     // generate random TLM command, either read or write command
//     tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
//     // set data
//     if (cmd == tlm::TLM_WRITE_COMMAND)
//       data[i % 16] = rand();

//     // Grab a new transaction from the memory manager
//     trans = m_mm.allocate();
//     trans->acquire();

//     // Set all attributes except byte_enable_length and extensions (unused)
//     trans->set_command(cmd);
//     trans->set_address(adr);
//     trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data[i % 16]));
//     trans->set_data_length(4);
//     trans->set_streaming_width(4);  // = data_length to indicate no streaming
//     trans->set_byte_enable_ptr(0);  // 0 indicates unused
//     trans->set_dmi_allowed(false);  // Mandatory initial value
//     trans->set_response_status(
//         tlm::TLM_INCOMPLETE_RESPONSE);  // Mandatory initial value

//     // Initiator must honor BEGIN_REQ/END_REQ exclusion rule
//     // so if pointer to transaction in request phase is not 0,
//     // wait until ongoing transaction enters response phase
//     if (request_in_progress) {
//       wait(end_request_event);
//     }
//     // transaction in request phase
//     request_in_progress = trans;
//     // set phase to BEGIN_REQ
//     phase = tlm::BEGIN_REQ;

//     // Timing annotation models processing time of initiator prior to call
//     delay = sc_time(rand_ps(), SC_PS);

//     // print status message of new transaction
//     cout << "new, trans = { " << (cmd ? 'W' : 'R') << ", " << hex << adr
//          << " } , data = " << hex << data << " at time " << sc_time_stamp()
//          << " delay = " << delay << endl;

//     // Non-blocking transport call on the forward path
//     tlm::tlm_sync_enum status;
//     status = socket->nb_transport_fw(*trans, phase, delay);

//     // Check value returned from nb_transport_fw
//     // if status == tlm::TLM_ACCEPTED, we must do nothing because
//     // cb function will be triggered
//     if (status == tlm::TLM_UPDATED) {
//       // slave used return path -> initiator must
//       // set event in order to trigger cb function
//       // The timing annotation must be honored
//       m_peq.notify(*trans, phase, delay);
//     } else if (status == tlm::TLM_COMPLETED) {
//       // The completion of the transaction necessarily ends the BEGIN_REQ
//       // phase
//       request_in_progress = 0;
//       // The target has terminated the transaction
//       check_transaction(*trans);
//     }
//     // wait some time before send new transaction
//     wait(sc_time(rand_ps(), SC_PS));
//   }

//   // wait some time before start new test case
//   wait(100, SC_NS);

//   // Allocate a transaction for one final, nominal call to b_transport
//   trans = m_mm.allocate();
//   trans->acquire();
//   trans->set_command(tlm::TLM_WRITE_COMMAND);
//   trans->set_address(0);
//   trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data[0]));
//   trans->set_data_length(4);
//   trans->set_streaming_width(4);  // == data_length to indicate no streaming
//   trans->set_byte_enable_ptr(0);  // 0 indicates unused
//   trans->set_dmi_allowed(false);  // Mandatory initial value
//   trans->set_response_status(
//       tlm::TLM_INCOMPLETE_RESPONSE);  // Mandatory initial value

//   delay = sc_time(rand_ps(), SC_PS);

//   cout << "Calling b_transport at " << sc_time_stamp()
//        << " with delay = " << delay << endl;

//   // Call b_transport to demonstrate the b/nb conversion by the
//   // simple_target_socket
//   // -> call ends in the target's nb_transport, phases are handled internal
//   socket->b_transport(*trans, delay);
//   check_transaction(*trans);
// }
