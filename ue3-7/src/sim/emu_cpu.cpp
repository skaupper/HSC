/*****************************************************************************/
/**
 * @file    emu_cpu.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   EmuCPU implementation
 */
/*****************************************************************************/

#include "emu_cpu.hpp"

#include <stdio.h>

using namespace std;

static void prepareTransactionDefaultParams(tlm::tlm_generic_payload *trans)
{
  assert(trans);

  // configure standard set of attributes
  trans->set_data_length(4);        // length of data in bytes
  trans->set_streaming_width(4);    // width of streaming burst, for
                                    // non streaming set value equal
                                    // to data length
  trans->set_byte_enable_length(0); // must not be set because the
                                    // ptr is set to 0
  trans->set_byte_enable_ptr(0);    // set to 0 to indicate that byte
                                    // enables are unused
  trans->set_dmi_allowed(false);    // will maybe set by the target
                                    // to indicate a DMI (direct
                                    // memory interface)
  // status will be set by the target
  trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
}

static void checkResponseError(tlm::tlm_generic_payload *trans)
{
  if (trans->is_response_error())
  {
    char txt[100];
    sprintf(txt,
            "Response error from b_transport, response status = %s",
            trans->get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
  }
}

/*----------------------------------------------------------------------------*/

EmuCpu::EmuCpu(sc_module_name module_name, main_func_ptr_t main_entry_point)
    : sc_module(name), mMainFuncPtr(main_entry_point)
{
  SC_THREAD(run);
}

EmuCpu *EmuCpu::getInstance()
{
  if (!mInstance)
  {
    cerr << "Need to call EmuCpu::createInstance() first!" << endl;
    assert(nullptr);
  }
  return mInstance;
}

EmuCpu *createInstance(char *module_name, main_func_ptr_t main_entry_point)
{
  mInstance = new EmuCpu(module_name, main_entry_point);
}

void EmuCpu::read_bus(uint32_t addr, uint32_t *data)
{
  uint32_t rd_data;
  tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload();

  trans->set_command(tlm::TLM_READ_COMMAND);

  doTransaction(trans, addr, &rd_data);
}

void EmuCpu::write_bus(uint32_t addr, uint32_t data)
{
  uint32_t wr_data;
  tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload();

  trans->set_command(tlm::TLM_WRITE_COMMAND);

  doTransaction(trans, addr, &rd_data);
}

void EmuCpu::run()
{
  mMainFuncPtr();

  sc_stop();
}

void EmuCpu::doTransaction(tlm::tlm_command cmd, uint32_t addr, uint32_t *data)
{
  prepareTransactionDefaultParams(trans);
  trans->set_address(addr);
  trans->set_data_ptr(reinterpret_cast<unsigned char *>(&rd_data));

  mSocket->b_transport(*trans, delay);
  checkResponseError(trans);

  wait(delay);
}
