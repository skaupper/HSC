/*****************************************************************************/
/**
 * @file    emu_cpu.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   EmuCPU implementation
 */
/*****************************************************************************/

#include "emu_cpu.hpp"
#include "constants.hpp"

#include <stdio.h>

using namespace std;


EmuCpu *EmuCpu::mInstance;

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
    : sc_module(module_name), mMainFuncPtr(main_entry_point)
{
  SC_THREAD(run);

  SC_METHOD(ISR);
  sensitive << iIrq.pos();
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

EmuCpu *EmuCpu::createInstance(char const *module_name, main_func_ptr_t main_entry_point)
{
  if (mInstance)
  {
    cerr << "EmuCpu::createInstance() was already called once!" << endl;
    assert(nullptr);
  }
  mInstance = new EmuCpu(module_name, main_entry_point);
  return mInstance;
}

void EmuCpu::setIsrCallback(isr_func_ptr_t isr_func)
{
  mIsrFunc = isr_func;
}

void EmuCpu::read_bus(uint32_t addr, uint32_t *rd_data)
{
  doTransaction(tlm::TLM_READ_COMMAND, addr, rd_data);
}

void EmuCpu::write_bus(uint32_t addr, uint32_t wr_data)
{
  doTransaction(tlm::TLM_WRITE_COMMAND, addr, &wr_data);
}

void EmuCpu::ISR()
{
  if(mIsrFunc == NULL)
    cerr << "EmuCpu::ISR() was called, before the ISR callback was set!" << endl;
  else
    mIsrFunc(NULL);
}

void EmuCpu::run()
{
  wait(RESET_DURATION_NS+1, SC_NS);

  mMainFuncPtr();

  sc_stop();
}

void EmuCpu::doTransaction(tlm::tlm_command cmd, uint32_t addr, uint32_t *data)
{
  tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload();
  prepareTransactionDefaultParams(trans);

  trans->set_command(cmd);
  trans->set_address(addr);
  trans->set_data_ptr(reinterpret_cast<unsigned char *>(data));

  sc_time delay;
  mSocket->b_transport(*trans, delay);
  checkResponseError(trans);

  wait(delay);
}

/*----------------------------------------------------------------------------*/

extern "C" void read_bus(uint32_t addr, uint32_t *data)
{
  EmuCpu::getInstance()->read_bus(addr, data);
}

extern "C" void write_bus(uint32_t addr, uint32_t data)
{
  EmuCpu::getInstance()->write_bus(addr, data);
}

extern "C" void setIsrCallback(isr_func_ptr_t isr_func)
{
  EmuCpu::getInstance()->setIsrCallback(isr_func);
}
