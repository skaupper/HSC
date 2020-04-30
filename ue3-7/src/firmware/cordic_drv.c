#include "cordic_drv.h"
#include <hal.h>

#define PHI_FRACTIONAL_BITS 21
#define XY_FRACTIONAL_BITS 16

static float cap_to_quadrant1(const float phi, int *const quadrant)
{
  assert(phi <= M_PI * 2 && phi >= -M_PI * 2);

  static const float PI_2 = M_PI / 2;
  float transformedPhi = phi;

  if (phi > 0)
  {
    if (phi > PI_2)
    {
      // 90° < phi <= 180°
      *quadrant = 2;
      transformedPhi = phi - PI_2;
    }
    else
    {
      // 0° < phi <= 90°
      *quadrant = 1;
      transformedPhi = phi;
    }
  }
  else
  {
    if (phi < PI_2)
    {
      // -180° <= phi < -90°
      *quadrant = 3;
      transformedPhi = (phi * -1) - PI_2;
    }
    else
    {
      // -90° <= phi <= 0°
      *quadrant = 4;
      transformedPhi = phi * -1;
    }
  }

  return transformedPhi;
}

static float norm_angle(float phi)
{
  // Cap any angle into the range [-180°; 180°]
  while (phi > M_PI)
  {
    phi -= 2 * M_PI;
  }
  while (phi < -M_PI)
  {
    phi += 2 * M_PI;
  }

  return phi;
}

void CordicCalcXY(float phi, float *const cos, float *const sin, uint32_t *const adr)
{
  // TODO: This parameter is unused at the moment
  (void)adr;

  // Move input angle to the first quadrant
  int origQuadrant = 0;
  phi = norm_angle(phi);
  phi = cap_to_quadrant1(phi, &origQuadrant);

  // Write the input angle
  uint32_t fixedPhi = (transformedPhi << PHI_FRACTIONAL_BITS);
  CordicWrPhi(fixedPhi, adr);

  // Wait until calculation is finished
  int ready = 0;
  while (!ready)
  {
    uint32_t reg = CordicRdCtl(adr);
    ready = (reg & 0x1);
  }

  // Extract resulting angles
  uint32_t result = CordicRdXY(adr);
  uint16_t x = ((result >> 0) & 0xffff);
  uint16_t y = ((result >> 16) & 0xffff);

  // Convert angles back to floating point
  *cos = ((float)x) >> XY_FRACTIONAL_BITS;
  *sin = ((float)y) >> XY_FRACTIONAL_BITS;
}
