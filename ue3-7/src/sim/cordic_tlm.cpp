/*****************************************************************************/
/**
 * @file    cordic_tlm.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Implementation of Cordic TLM abstraction layer
 */
/*****************************************************************************/

#include "cordic_tlm.hpp"
#include "hal.h"

Cordic_TLM::Cordic_TLM(sc_module_name name) : sc_module(name), mSocket("bus_rw")
{
  // register callbacks for incoming interface method calls
  mSocket.register_b_transport(this, &Cordic_TLM::b_transport);
}

/*********************************************************
  TLM 2 blocking transport method
*********************************************************/
void Cordic_TLM::b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
{
  tlm::tlm_command cmd = trans.get_command();
  uint64_t addr = trans.get_address();
  unsigned char *data = trans.get_data_ptr();
  unsigned int len = trans.get_data_length();
  unsigned char *byt = trans.get_byte_enable_ptr();
  unsigned int wid = trans.get_streaming_width();

  // Decode transaction and check parameters
  if (addr != OFFSET_CTL &&
      addr != OFFSET_PHI &&
      addr != OFFSET_XY)
  {
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    SC_REPORT_ERROR("TLM-2", "Cordic_TLM: Invalid register address used in read access");
    return;
  }

  if (byt != 0)
  {
    trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    return;
  }

  if (len > 4 || wid != len)
  {
    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return;
  }

  // read or write data
  if (cmd == tlm::TLM_READ_COMMAND)
  {
  }
  else if (cmd == tlm::TLM_WRITE_COMMAND)
  {
    if (addr == OFFSET_PHI)
    {
      mCordic.iStart = 1;
      mCordic.iPhi = *data;
    }
    else
    {
      SC_REPORT_ERROR("TLM-2", "Cordic_TLM: Invalid register address used in write access");
    }
  }

  /* Set DMI hint to indicate that DMI is not supported */
  trans.set_dmi_allowed(false);
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
  delay = sc_time(1, SC_NS);
}
