#include "cordic.h"
#include "CordicTb.h"


int sc_main(int argc, char *argv[])
{
  // define signals
  sc_signal<bool> startCordic("starCordic");
  sc_signal<bool> cordicReady("cordicReady");
  sc_signal<phi_t> phi("phi");
  sc_signal<x_t> x("x");
  sc_signal<y_t> y("y");

  // declare modules
  Cordic cordic("CORDIC");
  CordicTb tb("tb");

  // port binding
  cordic.iStart(startCordic);
  cordic.oRdy(cordicReady);
  cordic.iPhi(phi);
  cordic.oX(x);
  cordic.oY(y);

  tb.oStart(startCordic);
  tb.iRdy(cordicReady);
  tb.oPhi(phi);
  tb.iX(x);
  tb.iY(y);


  // start simulation
  sc_start(100, SC_NS);

  return 0;
}
