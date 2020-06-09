#ifndef _XEXCEPTIION_H_
#define _XEXCEPTIION_H_

typedef void (*Xil_ExceptionHandler)(void *);


void Xil_ExceptionInit(void);
void Xil_ExceptionEnable(void);


#endif
