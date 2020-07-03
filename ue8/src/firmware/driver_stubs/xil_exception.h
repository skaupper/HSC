#ifndef _XEXCEPTIION_H_
#define _XEXCEPTIION_H_

#include "types.h"

typedef void (*Xil_ExceptionHandler)(void *);

void Xil_ExceptionInit(void);
void Xil_ExceptionEnable(void);
void Xil_ExceptionRegisterHandler(u16 IrqId,
                                  Xil_ExceptionHandler handler,
                                  void *inst);

#endif
