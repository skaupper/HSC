#include <stdlib.h>

#include "xcordic_cc.h"
#include "xparameters.h"

static XCordic_cc_Config XCordic_cc_ConfigTable[XPAR_XCORDIC_CC_NUM_INSTANCES];

XCordic_cc_Config *XCordic_cc_LookupConfig(u16 DeviceId) {
  XCordic_cc_Config *ConfigPtr = NULL;
  int Index;
  for (Index = 0; Index < XPAR_XCORDIC_CC_NUM_INSTANCES; Index++) {
    if (XCordic_cc_ConfigTable[Index].DeviceId == DeviceId) {
      ConfigPtr = &XCordic_cc_ConfigTable[Index];
      break;
    }
  }

  return ConfigPtr;
}

int XCordic_cc_Initialize(XCordic_cc *InstancePtr, u16 DeviceId) {
  XCordic_cc_Config *ConfigPtr;

  ConfigPtr = XCordic_cc_LookupConfig(DeviceId);
  if (ConfigPtr == NULL) {
    InstancePtr->IsReady = 0;
    return (XST_DEVICE_NOT_FOUND);
  }

  return XCordic_cc_CfgInitialize(InstancePtr, ConfigPtr);
}

int XCordic_cc_CfgInitialize(XCordic_cc *InstancePtr,
                             XCordic_cc_Config *ConfigPtr) {
  ConfigPtr->DeviceId = 0;

  return XST_SUCCESS;
}
