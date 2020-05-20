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
    : sc_module(name), mSocket("cordic_tlm_rw"),
      mClk("clk", CLK_PERIOD_NS, SC_NS)
{
  mCordicBhvIP = new CordicBhv("CordicBhvIP");
  mCordicCcIP = new CordicCc("CordicCcIP");

  mnRst = 0;
  SC_THREAD(generateReset);

  SC_METHOD(combineAndCheckSignals);
  sensitive << mBhvRdy << mBhvX << mBhvY
            << mCcRdy  << mCcX  << mCcY
            << mStart  << mPhi  << mStartCalculations;


  // Configure trace file
  tf = sc_create_vcd_trace_file("trace");
  tf->delta_cycles(true);
  sc_trace(tf, mCcStart, "cordicCc.iStart");
  sc_trace(tf, mCcRdy, "cordicCc.oRdy");
  sc_trace(tf, mCcPhi, "cordicCc.iPhi");
  sc_trace(tf, mCcX, "cordicCc.oX");
  sc_trace(tf, mCcY, "cordicCc.oY");


  // Port binding
  mCordicBhvIP->iStart(mBhvStart);
  mCordicBhvIP->iPhi(mBhvPhi);
  mCordicBhvIP->oRdy(mBhvRdy);
  mCordicBhvIP->oX(mBhvX);
  mCordicBhvIP->oY(mBhvY);

  mCordicCcIP->iClk(mClk);
  mCordicCcIP->inRst(mnRst);
  mCordicCcIP->iStart(mCcStart);
  mCordicCcIP->iPhi(mCcPhi);
  mCordicCcIP->oRdy(mCcRdy);
  mCordicCcIP->oX(mCcX);
  mCordicCcIP->oY(mCcY);


  // register callbacks for incoming interface method calls
  mSocket.register_b_transport(this, &Cordic_TLM::b_transport);
}

Cordic_TLM::~Cordic_TLM()
{
  delete mCordicBhvIP;
  delete mCordicCcIP;
}


void Cordic_TLM::generateReset()
{
  wait(RESET_DURATION_NS, SC_NS);
  mnRst = 1;
}

void Cordic_TLM::combineAndCheckSignals()
{
  constexpr double EPSILON = std::pow(2, -14);

  static bool bhvFinished = false;
  static bool ccFinished = false;

  if (mBhvRdy.posedge()) {
    mBhvStart = false;
    bhvFinished = true;
  }

  if (mCcRdy.posedge()) {
    mCcStart = false;
    ccFinished = true;
  }

  if (mStartCalculations.posedge() || mStartCalculations.negedge()) {
    bhvFinished = false;
    ccFinished = false;

    mBhvStart = true;
    mCcStart = true;
    mStart = true;
  }

  mRdy = bhvFinished && ccFinished;

  if (bhvFinished && ccFinished) {
    double x = mBhvX.read();
    double y = mBhvY.read();

    if (std::abs(mBhvX - mCcX) > EPSILON) {
      sc_assertion_failed("The difference in X violates is greater than epsilon", __FILE__, __LINE__);
    }
    if (std::abs(mBhvY - mCcY) > EPSILON) {
      sc_assertion_failed("The difference in Y violates is greater than epsilon", __FILE__, __LINE__);
    }

    mX.write(x);
    mY.write(y);

    mStart = false;
  }

  mBhvPhi = mPhi;
  mCcPhi = mPhi;
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
      result = mRdy;
      memcpy(tr_data, &result, tr_len);
      break;

    case OFFSET_XY:
      x = mX * pow(2, XY_FRAC_BITS);
      y = mY * pow(2, XY_FRAC_BITS);
      result = (y << 16) | x;
      memcpy(tr_data, &result, tr_len);
      break;

    default:
      SC_REPORT_ERROR("TLM-2", "Cordic_TLM: Invalid register address used in read access");
      return;
    }
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

      mPhi = phi;
      mStartCalculations = !mStartCalculations;
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
