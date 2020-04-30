
#include <systemc.h>
#include "emu_cpu.hpp"

extern int main_c();

void run_program()
{
  main_c();
}

int sc_main(int argc, char *argv[])
{
  char *module_name = "TheAwesomeEmuCPU";
  EmuCpu *cpu = EmuCpu::createInstance(module_name, run_program);

  return 0;
}
