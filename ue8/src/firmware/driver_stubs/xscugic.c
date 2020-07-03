#include "xscugic.h"

#include "firmware/hal.h"
#include "xparameters.h"

void XScuGic_InterruptHandler(XScuGic *inst) {
  // Stub
}

void XScuGic_Enable(XScuGic *inst, u16 irqId) {
  // Stub
}

int XScuGic_Connect(XScuGic *inst,
                    u16 irqId,
                    Xil_ExceptionHandler irqHandler,
                    void *state) {
  CordicSetISRCallback(irqHandler);
  return XST_SUCCESS;
}
