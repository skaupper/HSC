#include <stdio.h>
#include <xil_printf.h>
#include <sleep.h>

#include "platform.h"
#include "private_timer.h"

#define msleep(msec) usleep((msec) * 1000)

static XScuGic intc;

static void timerExpired(void *state)
{
  XScuTimer *timer = (XScuTimer *)state;
  XScuTimer_ClearInterruptStatus(timer);

  static int counter = 0;

  counter = (counter+1) % 4;
}

int main()
{
  int status = 0;

  // Do initialization
  init_platform();

  status |= PrivateTimer_Init(&intc);
  PrivateTimer_SetFrequency(2);
  PrivateTimer_SetIntHandler((Xil_ExceptionHandler)timerExpired);

  if (status != XST_SUCCESS) {
    printf("Initialization failed!!!\n");
    while(1);
  }

  // Endless loop. Let the timer interrupt do the work.
  s16 x, y, z;

  while(1) {
    msleep(250);
    int temp = 4711;

    x++;
    printf("x: %d; y: %d; z: %d; temp: %d\n", x, y, z, temp);
  }

  // Cleanup. Not reachable.
  cleanup_platform();
  return 0;
}
