#include "cordic_drv.h"
#include <math.h>
#include <stdio.h>

int main_c()
{
  static const float ANGLE_INCR = 0.0125;
  static const float START_ANGLE = 0;
  static const float END_ANGLE = 2 * M_PI;

  static unsigned int *CORDIC_BASE_ADDRESS = 0x00000000;

  float cos;
  float sin;

  int success = cordic_init();
  if (success == FAIL)
  {
    printf("Initialization of Cordic core in cordic_init() failed!");
    return -1;
  }

  for (float angle = START_ANGLE; angle < END_ANGLE; angle += ANGLE_INCR)
  {
    CordicCalcXY(angle, &cos, &sin, CORDIC_BASE_ADDRESS);
    printf("Phi: %2.3f; Cos: %1.4f; Sin: %1.4f\n", angle, cos, sin);
  }

  return 0;
}
