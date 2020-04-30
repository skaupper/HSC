#include "hal.h"

//
// Write and Read fuction to communicatie with the EmuCPU
//
extern void write_bus(uint32_t addr, uint32_t data);
extern void read_bus(uint32_t addr, uint32_t *data);

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
