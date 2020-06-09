#include <stdlib.h>

#include "xparameter.h"
#include "xcordic.h"

void XCordic_InterruptEnable(XCordic *InstancePtr)
{
  InstancePtr->IsReady = 1;
}

void XCordic_InterruptClear(XCordic *InstancePtr, u16 DeviceId)
{

}

