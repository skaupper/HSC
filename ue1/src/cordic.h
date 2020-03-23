#ifndef _CORDIC_H
#define _CORDIC_H

#define SC_INCLUDE_FX // for fixed-point data types (like sc_ufixed)
#include <systemc.h>

/* NOTE: The format for ufixed is: ufixed<TOTAL_BITS, INTEGER_BITS>  */
typedef sc_ufixed<22, 1> phi_t;
typedef sc_ufixed<16, 1> x_t;
typedef sc_ufixed<16, 1> y_t;

SC_MODULE(Cordic)
{
  sc_in<bool> iStart;
  sc_out<bool> oRdy;
  sc_in<phi_t> iPhi;
  sc_out<x_t> oX;
  sc_out<y_t> oY;

  SC_CTOR(Cordic);

private:
  void calc();
};

#endif /* _CORDIC_H */
