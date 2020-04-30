#include "cordic_drv.h"
#include <hal.h>

void CordicCalcXY(const float phi, float *const cos, float *const sin, uint32_t *const adr)
{
  // TODO: This parameter is unused at the moment
  (void)adr;

  // TODO
  float transformedPhi = phi;

  CordicWrPhi(phi, adr);

  int ready = 0;
  while (!ready)
  {
    uint32_t reg = CordicRdCtl(adr);
    ready = (reg & 0x1);
  }

  uint32_t result = CordicRdXY(adr);
  uint16_t x = result & 0xffff;
  uint16_t y = (result >> 16) & 0xffff;
}
