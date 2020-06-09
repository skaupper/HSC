#ifndef _CORDIC_DRV_H_
#define _CORDIC_DRV_H_

#include <stdint.h>

typedef enum
{
  FAIL = -1,
  OK = 0
} CordicStatus_t;

CordicStatus_t cordic_init();
CordicStatus_t CordicCalcXY(float phi, float *const cos, float *const sin, uint32_t *const adr);

#endif // _CORDIC_DRV_H_
