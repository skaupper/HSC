#ifndef _XCORDIC_H_
#define _XCORDIC_H_

#include <stdint.h>


//
// Typedefs
//

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

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

#endif
