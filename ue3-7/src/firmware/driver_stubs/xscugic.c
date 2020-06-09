#include "xscugic.h"

#include "xparameter.h"

void XScuGic_InterruptHandler(XScuGic *inst)
{
    // Stub
}

void XScuGic_Enable(XScuGic *inst, u16 irqId)
{
    // Stub
}

int XScuGic_Connect(XScuGic *inst, u16 irqId, Xil_ExceptionHandler irqHandler, void *state)
{
    return XST_SUCCESS;
}
