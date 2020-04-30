/*****************************************************************************/
/**
 * @file    cordic_tlm.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Implementation of Cordic TLM abstraction layer
 */
/*****************************************************************************/

#include "cordic_tlm.hpp"
#include "../firmware/hal.h"

Cordic_TLM::Cordic_TLM(sc_module_name name)
    : sc_module(name), mSocket("cordic_tlm_rw")
{
  mCordicIP = new Cordic("CordicIP");

  // Port binding
  mCordicIP->iStart(mStart_o);
  mCordicIP->iPhi(mPhi_o);

  // register callbacks for incoming interface method calls
  mSocket.register_b_transport(this, &Cordic_TLM::b_transport);
}

/*********************************************************
  TLM 2 blocking transport method
*********************************************************/
void Cordic_TLM::b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
{
  tlm::tlm_command tr_cmd = trans.get_command();
  uint64_t tr_addr = trans.get_address();
  unsigned char *tr_data = trans.get_data_ptr();
  unsigned int tr_len = trans.get_data_length();
  unsigned char *tr_byte_en = trans.get_byte_enable_ptr();
  unsigned int tr_s_width = trans.get_streaming_width();

  // Decode transaction and check parameters
  if (tr_addr != OFFSET_CTL &&
      tr_addr != OFFSET_PHI &&
      tr_addr != OFFSET_XY)
  {
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    SC_REPORT_ERROR("TLM-2", "Cordic_TLM: Invalid register address");
    return;
  }

  if (tr_byte_en != 0)
  {
    trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    return;
  }

  if (tr_len > 4 || tr_s_width != tr_len)
  {
    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return;
  }

  // Actual read or write access
  if (tr_cmd == tlm::TLM_READ_COMMAND)
  {
    switch (tr_addr)
    {
    case OFFSET_CTL:
      *tr_data = mCordicIP->oRdy;
      break;
    case OFFSET_XY:
      *tr_data = (x_t)(mCordicIP->oY << 16) & (y_t)mCordicIP->oX;
      break;

    default:
      SC_REPORT_ERROR("TLM-2", "Cordic_TLM: Invalid register address used in read access");
      return;
      break;
    }
  }
  else if (tr_cmd == tlm::TLM_WRITE_COMMAND)
  {
    if (tr_addr == OFFSET_PHI)
    {
      mStart_o = true;
      //mCordicIP->iStart = true;

      // TODO: need to '&' here? or better assert(*tr_data & 0x1FFFFF) (invalid tr_data)?
      //mCordicIP->iPhi = (*tr_data & 0x1FFFFF);
      mPhi_o = (*tr_data & 0x1FFFFF);
    }
    else
    {
      SC_REPORT_ERROR("TLM-2", "Cordic_TLM: Invalid register address used in write access");
      return;
    }
  }

  trans.set_dmi_allowed(false);
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
  delay = sc_time(1, SC_NS);
}
