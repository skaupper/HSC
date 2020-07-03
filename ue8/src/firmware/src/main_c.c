#include <math.h>
#include <stdio.h>

#include "cordic_drv.h"

int main_c() {
  static const float ANGLE_INCR = 0.0125;
  static const float START_ANGLE = 0;
  static const float END_ANGLE = 2 * M_PI;

  float cos;
  float sin;

  int success = cordic_init();
  if (success == FAIL) {
    printf("Initialization of Cordic core in cordic_init() failed!");
    return -1;
  }

  for (float angle = START_ANGLE; angle < END_ANGLE; angle += ANGLE_INCR) {
    CordicCalcXY(angle,
                 &cos,
                 &sin,
                 (uint32_t*)XPAR_CORDIC_CC_TOP_0_S_AXI_CORDIC_IF_BASEADDR);
    printf("Phi: %2.3f; Cos: %1.4f; Sin: %1.4f\n", angle, cos, sin);
  }

  return 0;
}
