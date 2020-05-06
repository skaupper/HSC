#include "cordic_drv.h"
#include "hal.h"
#include <assert.h>
#include <math.h>

#define PHI_FRACTIONAL_BITS 21
#define XY_FRACTIONAL_BITS 16

//
// Helper functions to handle angles
//
static float transform_to_quadrant1(const float phi, int *const quadrant)
{
  // This functions tranforms the incoming angle to a representation in the 1st quadrant.
  // This is not a simple modulus. For example 100° are actually transformed into 180°-100° = 80°.
  // This way the resulting angles of cosinus and sinus can easier be transformed back to the original quadrant.

  assert(quadrant);
  assert(phi <= M_PI * 2 && phi >= -M_PI * 2);

  static const float PI_2 = M_PI / 2;
  float transformedPhi = phi;

  if (phi >= 0)
  {
    if (phi > PI_2)
    {
      // 90° < phi <= 180°
      *quadrant = 2;
      transformedPhi = 2 * PI_2 - phi;
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
      transformedPhi = 2 * PI_2 - (phi * -1);
    }
  }

  return transformedPhi;
}

static void transform_to_orig_angle(float *const cos, float *const sin, const int origQuadrant)
{
  assert(cos && sin);

  // Due to the transformation used in `transform_to_quadrant1`
  // the back transformation only alters the sign bits
  switch (origQuadrant)
  {
  case 2:
    *cos *= -1;
  case 1:
    break;

  case 3:
    *cos *= -1;
  case 4:
    *sin *= -1;
    break;

  default:
    assert(0);
    break;
  }
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

//
// Driver implementation
//
void CordicCalcXY(float phi, float *const cos, float *const sin, uint32_t *const addr)
{
  assert(cos && sin);

  // This double cast avoids pointer-to-int warnings
  uint32_t addrInt = (uint32_t)(uintptr_t)addr;

  // Move input angle to the first quadrant
  int origQuadrant = 0;
  phi = norm_angle(phi);
  phi = transform_to_quadrant1(phi, &origQuadrant);

  // Write the input angle
  uint32_t fixedPhi = phi * pow(2, PHI_FRACTIONAL_BITS);
  CordicWrPhi(addrInt, fixedPhi);

  // Wait until calculation is finished
  int ready = 0;
  while (!ready)
  {
    uint32_t reg = CordicRdCtl(addrInt);
    ready = (reg & 0x1);
  }

  // Extract resulting angles
  uint32_t result = CordicRdXY(addrInt);
  uint16_t x = ((result >> 0) & 0xffff);
  uint16_t y = ((result >> 16) & 0xffff);

  // Convert angles back to floating point
  *cos = ((float)x) / pow(2, XY_FRACTIONAL_BITS);
  *sin = ((float)y) / pow(2, XY_FRACTIONAL_BITS);

  // Move angles back to their original quadrants
  transform_to_orig_angle(cos, sin, origQuadrant);
}
