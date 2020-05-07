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
  mCordicIP->oRdy(mRdy_i);
  mCordicIP->oX(mX_i);
  mCordicIP->oY(mY_i);

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
    uint32_t result;
    uint32_t x;
    uint32_t y;

    switch (tr_addr)
    {
    case OFFSET_CTL:
      result = mRdy_i;
      memcpy(tr_data, &result, tr_len);
      break;

    case OFFSET_XY:
      x = mX_i * pow(2, XY_FRAC_BITS);
      y = mY_i * pow(2, XY_FRAC_BITS);
      result = (y << 16) | x;
      memcpy(tr_data, &result, tr_len);
      break;

    default:
      SC_REPORT_ERROR("TLM-2", "Cordic_TLM: Invalid register address used in read access");
      return;
    }

    mStart_o = false;
  }
  else if (tr_cmd == tlm::TLM_WRITE_COMMAND)
  {
    if (tr_addr == OFFSET_PHI)
    {
      // Read phi from input memory
      uint32_t rawPhi;
      memcpy(&rawPhi, tr_data, tr_len);
      rawPhi = (rawPhi & ((1 << PHI_WL)-1));

      // Convert interger representation back to a double and eventually into sc_ufixed
      double phi = ((double)rawPhi) * pow(2, -PHI_FRAC_BITS);

      mPhi_o = phi;
      mStart_o = true;
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
