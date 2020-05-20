/*
 * gpios.h
 *
 *  Created on: 05.05.2020
 *      Author: Florian Eibensteiner
 */

#ifndef SRC_GPIOS_H_
#define SRC_GPIOS_H_

#include "xgpio.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xparameters.h"

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_PS_DEVICE_ID       XPAR_XGPIOPS_0_DEVICE_ID
#define GPIO_AXI0_DEVICE_ID     XPAR_GPIO_0_DEVICE_ID

#define GPIO_PS_BUTTON_OFFSET    0 // MIO#0
#define GPIO_PS_LED_R_OFFSET    52 // MIO#52 (906+52=958)
#define GPIO_PS_LED_G_OFFSET    53 // MIO#53 (906+53=959)

#define LED_COLOR_OFF           0
#define LED_COLOR_GREEN         1
#define LED_COLOR_RED           2
#define LED_COLOR_AMBER         3



void set_PS_LED_color(unsigned char led_color);
void set_PL_LED_color(unsigned char led_color);

unsigned char get_PS_button_value(void);


int init_PS_led();
int init_PS_button();
int init_PL_led(u16 Device_ID);

#endif /* SRC_GPIOS_H_ */
