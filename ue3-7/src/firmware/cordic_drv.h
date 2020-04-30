#ifndef _CORDIC_DRV_H_
#define _CORDIC_DRV_H_

#include <stddef.h>

void CordicCalcXY(const float phi, float *const cos, float *const sin, uint32_t *const adr);

#endif // _CORDIC_DRV_H_
