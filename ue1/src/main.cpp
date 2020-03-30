#include "cordic.h"
#include "CordicTb.h"

int sc_main(int argc, char *argv[])
{
  // Define signals
  sc_signal<bool> startCordic("startCordic");
  sc_signal<bool> cordicReady("cordicReady");
  sc_signal<phi_t> phi("phi");
  sc_signal<x_t> x("x");
  sc_signal<y_t> y("y");

  // Declare modules
  Cordic cordic("CORDIC");
  CordicTb tb("tb");

  // Port binding
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

  // Configure trace file
  sc_trace_file *tf;
  tf = sc_create_vcd_trace_file("bin/cordic_trace");
  tf->delta_cycles(true);
  sc_trace(tf, startCordic, "startCordic");
  sc_trace(tf, cordicReady, "cordicReady");
  sc_trace(tf, phi, "PHI");
  sc_trace(tf, x, "X");
  sc_trace(tf, y, "Y");

  // start simulation
  sc_start(100, SC_NS);

  return 0;
}
