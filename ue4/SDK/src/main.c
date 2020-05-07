#include <stdio.h>
#include <xil_printf.h>
#include <sleep.h>

#include "platform.h"

#include "gpios.h"
#include "private_timer.h"
#include "lis2ds12.h"


#define msleep(msec) usleep((msec) * 1000)


static XScuGic intc;


static void timerExpired(void *state)
{
  static const int ON_COLOR = LED_COLOR_GREEN;
  static const int OFF_COLOR = LED_COLOR_RED;

  static int counter = 0;

  // Use PL LED as Bit 0 of `counter`
  if (counter & (1<<0)) {
    set_PL_LED_color(ON_COLOR);
  } else {
    set_PL_LED_color(OFF_COLOR);
  }

  // Use PL LED as Bit 1 of `counter`
  if (counter & (1<<1)) {
    set_PS_LED_color(ON_COLOR);
  } else {
    set_PS_LED_color(OFF_COLOR);
  }

  counter = (counter+1) % 4;

  XScuTimer *timer = (XScuTimer *)state;
  XScuTimer_ClearInterruptStatus(timer);
}



int main()
{
  // Do initialization
  init_platform();

  init_PS_led();
  init_PS_button();
  init_PL_led(GPIO_AXI0_DEVICE_ID);

  PrivateTimer_Init(&intc);
  PrivateTimer_SetFrequency(2);
  PrivateTimer_SetIntHandler((Xil_ExceptionHandler)timerExpired);


  // After startup the LEDs should be turned off
  set_PS_LED_color(LED_COLOR_OFF);
  set_PL_LED_color(LED_COLOR_OFF);


  // Endless loop. Let the timer interrupt do the work.
  while(1);


  // Cleanup. Not reachable.
  cleanup_platform();
  return 0;
}
