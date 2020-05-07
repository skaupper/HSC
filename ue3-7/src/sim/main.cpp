#define SC_INCLUDE_FX // for fixed-point data types (like sc_ufixed)
#include <systemc.h>

#include "emu_cpu.hpp"
#include "cordic_tlm.hpp"


extern "C" int main_c(); // from firmware/main_c.c

int sc_main(int argc, char *argv[])
{
  char const *module_name = "TheAwesomeEmuCPU";

  // Create all used modules
  EmuCpu *cpu = EmuCpu::createInstance(module_name, main_c);
  Cordic_TLM cordicTlm("CORDIC_TLM_Wrapper");

  // Connect CPU with peripherals
  cpu->mSocket.bind(cordicTlm.mSocket);

  // Configure trace file
  sc_trace_file *tf;
  tf = sc_create_vcd_trace_file("bin/trace");
  tf->delta_cycles(true);
  sc_trace(tf, cordicTlm.mStart_o, "cordic.iStart");
  sc_trace(tf, cordicTlm.mRdy_i, "cordic.oRdy");
  sc_trace(tf, cordicTlm.mPhi_o, "cordic.iPhi");
  sc_trace(tf, cordicTlm.mX_i, "cordic.oX");
  sc_trace(tf, cordicTlm.mY_i, "cordic.oY");

  // Start simulation (sc_stop is called when the CPU application is finished)
  sc_start();

  return 0;
}
