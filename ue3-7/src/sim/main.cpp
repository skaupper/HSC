#define SC_INCLUDE_FX // for fixed-point data types (like sc_ufixed)
#include <systemc.h>

#include "emu_cpu.hpp"
#include "cordic_tlm.hpp"


extern "C" int main_c(); // from firmware/main_c.c

int sc_main(int argc, char *argv[])
{
  char const *module_name = "TheAwesomeEmuCPU";
  EmuCpu *cpu = EmuCpu::createInstance(module_name, main_c);
  Cordic_TLM cordicTlm("CORDIC_TLM_Wrapper");

  cpu->mSocket.bind(cordicTlm.mSocket);

  sc_start();
  return 0;
}
