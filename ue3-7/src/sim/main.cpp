
#include <systemc.h>
#include "emu_cpu.hpp"

extern "C" int main_c(); // from firmware/main_c.c

int sc_main(int argc, char *argv[])
{
  char const *module_name = "TheAwesomeEmuCPU";
  EmuCpu *cpu = EmuCpu::createInstance(module_name, main_c);

  (void) cpu;
  return 0;
}
