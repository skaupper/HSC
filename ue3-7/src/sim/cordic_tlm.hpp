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

#define SC_INCLUDE_FX // for fixed-point data types (like sc_ufixed)
#include <systemc.h>
#include <tlm_utils/simple_target_socket.h>

#include "cordic.h"

SC_MODULE(Cordic_TLM)
{
  /* target socket, with default settings */
  tlm_utils::simple_target_socket<Cordic_TLM> mSocket;

  SC_CTOR(Cordic_TLM);

  virtual void b_transport(tlm::tlm_generic_payload & trans, sc_time & delay);

private:
  Cordic mCordic("CordicInst");
};

#endif /* _CORDIC_TLM_H */
