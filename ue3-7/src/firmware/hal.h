#ifndef _HAL_H_
#define _HAL_H_

#include <stdint.h>

//
// Offsets of Cordic registers
//
#define OFFSET_CTL ((uint32_t)0)
#define OFFSET_PHI ((uint32_t)4)
#define OFFSET_XY ((uint32_t)8)

//
// HW interface
//
uint32_t CordicRdCtl(uint32_t addr);
void CordicWrPhi(uint32_t addr, uint32_t data);
uint32_t CordicRdXY(uint32_t addr);

#endif // _HAL_H_
