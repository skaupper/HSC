#include "cordic_tlm.hpp"
#include "emu_cpu.hpp"


extern "C" int main_c(); // from firmware/main_c.c

int sc_main(int argc, char *argv[])
{
  char const *module_name = "TheAwesomeEmuCPU";

  // Create all used modules
  EmuCpu *cpu = EmuCpu::createInstance(module_name, main_c);
  Cordic_TLM cordicTlm("CORDIC_TLM_Wrapper");

  // Connect CPU with peripherals
  cpu->mSocket.bind(cordicTlm.mSocket);

  // Start simulation (sc_stop is called when the CPU application is finished)
  sc_start();

  return 0;
}
