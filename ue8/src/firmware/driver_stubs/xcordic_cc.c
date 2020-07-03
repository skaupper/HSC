#include "xcordic_cc.h"

#include <stdlib.h>

#include "xparameters.h"

void XCordic_cc_InterruptEnable(XCordic_cc *InstancePtr) {
  InstancePtr->IsReady = 1;
}

void XCordic_cc_InterruptClear(XCordic_cc *InstancePtr, u16 DeviceId) {
}
