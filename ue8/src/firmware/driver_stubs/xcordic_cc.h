#ifndef _XCORDIC_CC_H_
#define _XCORDIC_CC_H_

#include "types.h"

//
// Typedefs
//

typedef struct {
  u8 IsReady;
} XCordic_cc;

typedef struct {
  u16 DeviceId;
} XCordic_cc_Config;

//
// Function prototypes
//

XCordic_cc_Config *XCordic_cc_LookupConfig(u16 DeviceId);
int XCordic_cc_Initialize(XCordic_cc *InstancePtr, u16 DeviceId);
int XCordic_cc_CfgInitialize(XCordic_cc *InstancePtr,
                             XCordic_cc_Config *ConfigPtr);

void XCordic_cc_InterruptEnable(XCordic_cc *InstancePtr);
void XCordic_cc_InterruptClear(XCordic_cc *InstancePtr, u16 DeviceId);

#endif /* _XCORDIC_CC_H_ */
