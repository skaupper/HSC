/*****************************************************************************/
/**
 * @file    cordic_tlm.hpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Header of Cordic TLM abstraction layer
 */
/*****************************************************************************/

#ifndef _CORDIC_TLM_H
#define _CORDIC_TLM_H

#include "cordic_bhv.hpp"
#include "cordic_cc.hpp"

#include <tlm_utils/simple_target_socket.h>



SC_MODULE(Cordic_TLM)
{
  /* target socket, with default settings */
  tlm_utils::simple_target_socket<Cordic_TLM> mSocket;

  SC_CTOR(Cordic_TLM);
  ~Cordic_TLM();

  virtual void b_transport(tlm::tlm_generic_payload & trans, sc_time & delay);

private:
  void generateReset();
  void combineAndCheckSignals();


  sc_trace_file *tf;

  sc_clock mClk;
  sc_signal<bool> mnRst;

  /* Signal interface ("wires") to Cordic Bhv IP */
  sc_signal<bool> mBhvRdy;
  sc_signal<xy_t> mBhvX;
  sc_signal<xy_t> mBhvY;
  sc_signal<bool> mBhvStart;
  sc_signal<phi_t> mBhvPhi;

  /* Signal interface ("wires") to Cordic CC IP */
  sc_signal<bool> mCcRdy;
  sc_signal<xy_t> mCcX;
  sc_signal<xy_t> mCcY;
  sc_signal<bool> mCcStart;
  sc_signal<phi_t> mCcPhi;

  /* Exposed signals */
  sc_signal<bool> mRdy;
  sc_signal<xy_t> mX;
  sc_signal<xy_t> mY;
  sc_signal<bool> mStart;
  sc_signal<phi_t> mPhi;


  // Internal signal
  sc_signal<bool> mStartCalculations;


  CordicBhv *mCordicBhvIP;
  CordicCc *mCordicCcIP;
};

#endif /* _CORDIC_TLM_H */
