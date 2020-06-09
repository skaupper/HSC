#include "hal.h"

#include "emu_cpu.hpp" // for isr_func_ptr_t

//
// Write and Read fuction to communicatie with the EmuCPU
// (from src/sim/emu_cpu.cpp)
//
extern void write_bus(uint32_t addr, uint32_t data);
extern void read_bus(uint32_t addr, uint32_t *data);
extern void setIsrCallback(isr_func_ptr_t isr_func);

uint32_t CordicRdCtl(uint32_t addr)
{
  uint32_t data;
  read_bus(addr + OFFSET_CTL, &data);
  return data;
}

void CordicWrPhi(uint32_t addr, uint32_t data)
{
  write_bus(addr + OFFSET_PHI, data);
}

uint32_t CordicRdXY(uint32_t addr)
{
  uint32_t data;
  read_bus(addr + OFFSET_XY, &data);
  return data;
}

void CordicSetISRCallback(isr_func_ptr_t isr_func)
{
  setIsrCallback(isr_func);
}
