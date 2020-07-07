#include <sleep.h>
#include <stdio.h>
#include <xil_printf.h>

#include "platform.h"

extern int main_c();

#define msleep(msec) usleep((msec)*1000)

int main() {
  xil_printf("### Main Program running now ... ###\r\n");

  // Do initialization
  init_platform();

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
