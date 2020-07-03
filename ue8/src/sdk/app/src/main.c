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

  xil_printf("Timer expired\n");
}

int main() {
  printf("### Main Program running now ... ###\n");

  int status = 0;

  // Do initialization
  init_platform();

  status |= PrivateTimer_Init(&intc);
  PrivateTimer_SetFrequency(2);
  PrivateTimer_SetIntHandler((Xil_ExceptionHandler)timerExpired);

  if (status != XST_SUCCESS) {
    xil_printf("Initialization failed!!!\n");
    while (1) {
    }
  }
  printf("### Done with initializing peripherals.\n");

  // Endless loop. Let the timer interrupt do the work.
  s16 x = 0;
  s16 y = 0;
  s16 z = 0;

  while (1) {
    // Call the main function, which is also used in simulation.
    main_c();

    msleep(1000);
    int temp = 4711;

    x++;
    xil_printf("x: %d; y: %d; z: %d; temp: %d\n", x, y, z, temp);
    xil_printf(
        "##################################################################\n");
  }

  // Cleanup. Not reachable.
  cleanup_platform();
  return 0;
}
