#include <stdlib.h>

#include "xcordic_cc.h"
#include "xparameters.h"

void XCordic_InterruptEnable(XCordic *InstancePtr) {
  InstancePtr->IsReady = 1;
}

void XCordic_InterruptClear(XCordic *InstancePtr, u16 DeviceId) {
}
