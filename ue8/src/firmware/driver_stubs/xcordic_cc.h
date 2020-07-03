#ifndef _XCORDIC_CC_H_
#define _XCORDIC_CC_H_

#include "types.h"

//
// Typedefs
//

typedef struct {
  u8 IsReady;
} XCordic;

typedef struct {
  u16 DeviceId;
} XCordic_Config;

//
// Function prototypes
//

XCordic_Config *XCordic_LookupConfig(u16 DeviceId);
int XCordic_Initialize(XCordic *InstancePtr, u16 DeviceId);
int XCordic_CfgInitialize(XCordic *InstancePtr, XCordic_Config *ConfigPtr);

void XCordic_InterruptEnable(XCordic *InstancePtr);
void XCordic_InterruptClear(XCordic *InstancePtr, u16 DeviceId);

#endif /* _XCORDIC_CC_H_ */
