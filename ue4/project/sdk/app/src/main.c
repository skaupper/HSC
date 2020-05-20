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
  XScuTimer *timer = (XScuTimer *)state;
  XScuTimer_ClearInterruptStatus(timer);



  static const int ON_COLOR = LED_COLOR_GREEN;
  static const int OFF_COLOR = LED_COLOR_RED;

  static int counter = 0;

  //
  // Update LEDs with every timer interrupt (2Hz)
  //

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
}



int main()
{
  int status = 0;

  // Do initialization
  init_platform();

  status |= init_PS_led();
  status |= init_PS_button();
  status |= init_PL_led(GPIO_AXI0_DEVICE_ID);

  status |= PrivateTimer_Init(&intc);
  PrivateTimer_SetFrequency(2);
  PrivateTimer_SetIntHandler((Xil_ExceptionHandler)timerExpired);

  status |= LIS2DS12_Init();


  if (status != XST_SUCCESS) {
    printf("Initialization failed!!!\n");
    while(1);
  }


  // After startup the LEDs should be turned off
  set_PS_LED_color(LED_COLOR_OFF);
  set_PL_LED_color(LED_COLOR_OFF);


  // Endless loop. Let the timer interrupt do the work.
  s16 x, y, z;

  while(1) {
    msleep(2000);

    x = LIS2DS12_GetX();
    y = LIS2DS12_GetY();
    z = LIS2DS12_GetZ();
    int temp = LIS2DS12_GetTemp();

    printf("x: %d; y: %d; z: %d; temp: %d\n", x, y, z, temp);
  }


  // Cleanup. Not reachable.
  cleanup_platform();
  return 0;
}
