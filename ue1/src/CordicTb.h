#ifndef CORDIC_TB_H
#define CORDIC_TB_H

#include "cordic.h"

SC_MODULE(CordicTb)
{
  sc_out<bool> oStart;
  sc_in<bool> iRdy;
  sc_out<phi_t> oPhi;
  sc_in<x_t> iX;
  sc_in<y_t> iY;

  SC_CTOR(CordicTb)
  {
    SC_THREAD(doStimulate);
  }

private:
  void doStimulate();
  bool run_test(const double phi);
};

#endif /* CORDIC_TB_H */
