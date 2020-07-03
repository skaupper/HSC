#include <stdlib.h>

#include "xexception.h"
#include "xparameters.h"
#include "xscugic.h"

static XScuGic_Config XScuGic_ConfigTable[XPAR_XSCUGIC_NUM_INSTANCES];

XScuGic_Config *XScuGic_LookupConfig(u16 DeviceId) {
  XScuGic_Config *ConfigPtr = NULL;
  int Index;
  for (Index = 0; Index < XPAR_XSCUGIC_NUM_INSTANCES; Index++) {
    if (XScuGic_ConfigTable[Index].DeviceId == DeviceId) {
      ConfigPtr = &XScuGic_ConfigTable[Index];
      break;
    }
  }

  return ConfigPtr;
}

int XScuGic_Initialize(XScuGic *InstancePtr, u16 DeviceId) {
  XScuGic_Config *ConfigPtr;

  ConfigPtr = XScuGic_LookupConfig(DeviceId);
  if (ConfigPtr == NULL) {
    InstancePtr->IsReady = 0;
    return (XST_DEVICE_NOT_FOUND);
  }

  return XScuGic_CfgInitialize(InstancePtr, ConfigPtr, 0);
}

int XScuGic_CfgInitialize(XScuGic *InstancePtr,
                          XScuGic_Config *ConfigPtr,
                          u64 CpuBaseAddress) {
  ConfigPtr->CpuBaseAddress = CpuBaseAddress;
  ConfigPtr->DeviceId = 0;
  return XST_SUCCESS;
}
