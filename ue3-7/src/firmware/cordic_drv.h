#ifndef _CORDIC_DRV_H_
#define _CORDIC_DRV_H_

#include <stdint.h>

void CordicCalcXY(float phi, float *const cos, float *const sin, uint32_t *const adr);

#endif // _CORDIC_DRV_H_
