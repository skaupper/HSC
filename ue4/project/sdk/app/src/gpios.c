/*
 * gpios.c
 *
 *  Created on: 05.05.2020
 *      Author: Florian Eibensteiner
 */

#include"gpios.h"

/************************** Variable Definitions **************************/
static u32 PsButtonPin    = 0;    /* MIO_0 = PS Pushbutton */
static u32 PSRedLedPin    = 52;   /* MIO_52 = Red LED */
static u32 PSGreenLedPin  = 53;   /* MIO53 = Green LED */

XGpioPs PS_Gpio;    /* The driver instance for PS GPIO Device. */
XGpio PL_Gpio_Led;  /* The driver instance for PL GPIO Device. */


void set_PS_LED_color(unsigned char led_color)
{
  switch(led_color)
  {
    case LED_COLOR_OFF :
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_R_OFFSET, 0x0); //Red LED off
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_G_OFFSET, 0x0); //Green LED off
      break;
    case LED_COLOR_GREEN :
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_R_OFFSET, 0x0); //Red LED off
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_G_OFFSET, 0x1); //Green LED on
      break;
    case LED_COLOR_RED :
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_R_OFFSET, 0x1); //Red LED on
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_G_OFFSET, 0x0); //Green LED off
      break;
    case LED_COLOR_AMBER :
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_R_OFFSET, 0x1); //Red LED on
      XGpioPs_WritePin(&PS_Gpio, GPIO_PS_LED_G_OFFSET, 0x1); //Green LED on
      break;
    default : /* Error */
      //Do nothing
      break;
  } //switch(led_color)
} //set_PS_LED_color()

void set_PL_LED_color(unsigned char led_color)
{
  switch(led_color)
  {
  case LED_COLOR_OFF :
    XGpio_DiscreteWrite(&PL_Gpio_Led, 1, 0x0); //Red LED off
    XGpio_DiscreteWrite(&PL_Gpio_Led, 2, 0x0); //Green LED off
    break;
  case LED_COLOR_GREEN :
    XGpio_DiscreteWrite(&PL_Gpio_Led, 1, 0x1); //Red LED on
    XGpio_DiscreteWrite(&PL_Gpio_Led, 2, 0x0); //Green LED off
    break;
  case LED_COLOR_RED :
    XGpio_DiscreteWrite(&PL_Gpio_Led, 1, 0x0); //Red LED off
    XGpio_DiscreteWrite(&PL_Gpio_Led, 2, 0x1); //Green LED on
    break;
  case LED_COLOR_AMBER :
    XGpio_DiscreteWrite(&PL_Gpio_Led, 1, 0x1); //Red LED on
    XGpio_DiscreteWrite(&PL_Gpio_Led, 2, 0x1); //Green LED on
    break;
    default : /* Error */
      //Do nothing
      break;
  } //switch(led_color)
} //set_PL_LED_color()


unsigned char get_PS_button_value(void)
{
  unsigned char PsButtonStatus;
  PsButtonStatus = XGpioPs_ReadPin(&PS_Gpio, PsButtonPin);
  return(PsButtonStatus);
} //get_PS_button_value()


int init_PS_led(){
  XGpioPs_Config *ConfigPtr;
  int Status;

  if(PS_Gpio.IsReady != XIL_COMPONENT_IS_READY){
    /* Device is not ready, thus initialize the PS GPIO driver. */
    ConfigPtr = XGpioPs_LookupConfig(GPIO_PS_DEVICE_ID);
    Status = XGpioPs_CfgInitialize(&PS_Gpio, ConfigPtr,
            ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) {
      return XST_FAILURE;
    }

  }
  /* Set the direction for the LED pins to be outputs */
  XGpioPs_SetDirectionPin(&PS_Gpio, PSRedLedPin, 1);
  XGpioPs_SetDirectionPin(&PS_Gpio, PSGreenLedPin, 1);
  /* Set the output Enable for the LED pins */
  XGpioPs_SetOutputEnablePin(&PS_Gpio, PSRedLedPin, 1);
  XGpioPs_SetOutputEnablePin(&PS_Gpio, PSGreenLedPin, 1);

  return Status;
}

int init_PS_button(){
  XGpioPs_Config *ConfigPtr;
  int Status;

  if(PS_Gpio.IsReady != XIL_COMPONENT_IS_READY){
    /* Device is not ready, thus initialize the PS GPIO driver. */
    ConfigPtr = XGpioPs_LookupConfig(GPIO_PS_DEVICE_ID);
    Status = XGpioPs_CfgInitialize(&PS_Gpio, ConfigPtr,
            ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) {
      return XST_FAILURE;
    }

  }

  /* Set the direction for the PS button pin to be input. */
  XGpioPs_SetDirectionPin(&PS_Gpio, PsButtonPin, 0x0);

  return Status;
}

int init_PL_led(u16 Device_ID){
  int Status;

  /* Initialize the PL AXI GPIO driver using the passed device ID */
  Status = XGpio_Initialize(&PL_Gpio_Led, Device_ID);
  if (Status != XST_SUCCESS) {
    return XST_FAILURE;
  }

  /* Set the direction for all LED signals as outputs */
  XGpio_SetDataDirection(&PL_Gpio_Led, 1, 0x00); //All outputs

  return Status;
}
