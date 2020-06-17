/*****************************************************************************/
/**
 * @file    private_timer.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Constants and function prototypes to interact with the
 *          private timer of the ARM core.
 */
/*****************************************************************************/

#ifndef _PRIVATE_TIMER_H_
#define _PRIVATE_TIMER_H_

#include <xparameters.h>
#include <xscutimer.h>
#include <xscugic.h>
#include <xil_exception.h>
#include <xtime_l.h>

#define TIMER_DEVICE_ID    XPAR_XSCUTIMER_0_DEVICE_ID
#define INTC_DEVICE_ID     XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_INTR_ID      XPAR_SCUTIMER_INTR

#define TICKS_PER_SECOND   COUNTS_PER_SECOND


int PrivateTimer_Init(XScuGic *intc);
void PrivateTimer_Disable(XScuGic *intc);

void PrivateTimer_SetReloadValue(u32 value);
void PrivateTimer_SetIntHandler(Xil_ExceptionHandler handler);

void PrivateTimer_SetFrequency(u16 hz);


#endif /* _PRIVATE_TIMER_H_ */
