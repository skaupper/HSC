#include <sleep.h>
#include <stdio.h>
#include <xil_printf.h>

#include "platform.h"
#include "private_timer.h"

extern int main_c();

#define msleep(msec) usleep((msec)*1000)

static XScuGic intc;

static void timerExpired(void *state) {
  XScuTimer *timer = (XScuTimer *)state;
  XScuTimer_ClearInterruptStatus(timer);

  xil_printf("Timer expired\r\n");
}

int main() {
  xil_printf("### Main Program running now ... ###\n");

  int status = 0;

  // Do initialization
  init_platform();

  status |= PrivateTimer_Init(&intc);
  PrivateTimer_SetFrequency(2);
  PrivateTimer_SetIntHandler((Xil_ExceptionHandler)timerExpired);

  if (status != XST_SUCCESS) {
    xil_printf("Timer initialization failed!!!\n");
    while (1) {
    }
  }
  xil_printf("### Done with initializing peripherals.\n");

  // Main Loop
  while (1) {
    // Call the main function, which is also used in simulation.
    main_c();

    xil_printf("Sleeping for 1 second ...\r\n");
    xil_printf("########################################################\r\n");
    msleep(1000);
  }

  // Cleanup. Not reachable.
  cleanup_platform();
  return 0;
}
