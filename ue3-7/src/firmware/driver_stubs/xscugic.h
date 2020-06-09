#ifndef _XSCUGIC_H_
#define _XSCUGIC_H_

#include "types.h"
#include "xexception.h"


//
// Typedefs
//

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
int XScuGic_Connect(XScuGic *inst, u16 irqId, Xil_ExceptionHandler irqHandler, void *state);

void XScuGic_Enable(XScuGic *, u16 irqId);
void XScuGic_InterruptHandler(XScuGic *);

#endif
