#ifndef _XSCUGIC_H_
#define _XSCUGIC_H_

#include <stdint.h>


//
// Typedefs
//

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct {
    u8 IsReady;
} XScuGic;

typedef struct {
    u16 DeviceId;
    u64 CpuBaseAddress;
} XScuGic_Config;


//
// Function prototypes
//

XScuGic_Config *XScuGic_LookupConfig(u16 DeviceId);
int XScuGic_Initialize(XScuGic *InstancePtr, u16 DeviceId);
int XScuGic_CfgInitialize(XScuGic *InstancePtr, XScuGic_Config *ConfigPtr, u64 CpuBaseAddress);


void XScuGic_InterruptHandler(XScuGic *);

#endif
