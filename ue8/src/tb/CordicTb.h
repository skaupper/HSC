#ifndef CORDIC_TB_H
#define CORDIC_TB_H

#include "cordic_syn.h"

SC_MODULE(CordicTb) {
  sc_in<bool> inRst;
  sc_out<bool> oStart;
  sc_in<bool> iRdy;
  sc_out<phi_t> oPhi;
  sc_in<xy_t> iX;
  sc_in<xy_t> iY;

  sc_out<bool> oSuccess;

  SC_CTOR(CordicTb) {
    SC_THREAD(doStimulate);
  }

 private:
  void doStimulate();
  bool run_test(const double phi, double *pError);
};

#endif /* CORDIC_TB_H */
