/*****************************************************************************/
/**
 * @file    private_timer.c
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of function prototypes for the private timer of
 *          the ARM core.
 */
/*****************************************************************************/

#include "private_timer.h"

static XScuTimer timer;
static u32 timerReloadValue = 0xffff;
static Xil_ExceptionHandler timerInterruptHandler = NULL;

//
// Private helper functions
//

static void __interruptHandler(void *state) {
  if (timerInterruptHandler != NULL) {
    timerInterruptHandler(state);
  }

  XScuTimer_LoadTimer(&timer, timerReloadValue);
}

static int __connectToIntrSystem(XScuGic *intc) {
  int status;
  XScuGic_Config *intcConfig;

  // Query the interrupt controller configuration entry
  intcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
  if (intcConfig == NULL) {
    return XST_FAILURE;
  }

  // Initialize the interrupt controller driver.
  status = XScuGic_CfgInitialize(intc, intcConfig, intcConfig->CpuBaseAddress);
  if (status != XST_SUCCESS) {
    return status;
  }

  // Initialize the exception handling of the CPU
  Xil_ExceptionInit();

  // Register an universal exception handler for all interrupts.
  // The interrupt controller will further forward interrupts.
  Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
                               (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                               intc);

  // Register an interrupt handler for the timer interrupt at the
  // interrupt controller.
  status = XScuGic_Connect(intc,
                           TIMER_INTR_ID,
                           (Xil_ExceptionHandler)__interruptHandler,
                           (void *)&timer);
  if (status != XST_SUCCESS) {
    return status;
  }

  // Enable the generic interrupt controller
  XScuGic_Enable(intc, TIMER_INTR_ID);

  // Enable the interrupts of the timer
  XScuTimer_EnableInterrupt(&timer);

  // Enable interrupts for the processor
  Xil_ExceptionEnable();

  return XST_SUCCESS;
}

//
// Public interface functions
//

int PrivateTimer_Init(XScuGic *intc) {
  int status;
  XScuTimer_Config *config;

  // Query the timer configuration entry
  config = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
  if (config == NULL) {
    return XST_FAILURE;
  }

  // Initialize the driver instance
  status = XScuTimer_CfgInitialize(&timer, config, config->BaseAddr);
  if (status != XST_SUCCESS) {
    return status;
  }

  // Do a basic self test to verify a correctly built hardware
  status = XScuTimer_SelfTest(&timer);
  if (status != XST_SUCCESS) {
    return status;
  }

  // Activate the timer interrupt of the given interrupt controller
  status = __connectToIntrSystem(intc);
  if (status != XST_SUCCESS) {
    return status;
  }

  // Set the timer counter value
  XScuTimer_LoadTimer(&timer, timerReloadValue);
  // Reload the timer automatically if it expires
  XScuTimer_EnableAutoReload(&timer);
  // Start the timer
  XScuTimer_Start(&timer);

  return XST_SUCCESS;
}

void PrivateTimer_Disable(XScuGic *intc) {
  // Disconnect the timer interrupt from the interrupt controller
  XScuGic_Disconnect(intc, TIMER_INTR_ID);
}

void PrivateTimer_SetReloadValue(u32 value) {
  timerReloadValue = value;
}

void PrivateTimer_SetIntHandler(Xil_ExceptionHandler handler) {
  timerInterruptHandler = handler;
}

void PrivateTimer_SetFrequency(u16 hz) {
  // See also:
  // https://developer.arm.com/docs/ddi0407/f/global-timer-private-timers-and-watchdog-registers/about-the-private-timer-and-watchdog-blocks/calculating-timer-intervals
  u8 prescaler = XScuTimer_GetPrescaler(&timer);
  u32 neededReloadValue = (TICKS_PER_SECOND) / (hz * (prescaler + 1)) - 1;
  PrivateTimer_SetReloadValue(neededReloadValue);
}
