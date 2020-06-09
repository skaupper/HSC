#include "cordic_drv.h"
#include "hal.h"
#include <assert.h>
#include <math.h>

#include "xparameter.h"
#include "xexception.h"
#include "xscugic.h"
#include "xcordic.h"


#define PHI_FRACTIONAL_BITS 21
#define XY_FRACTIONAL_BITS 16


static XScuGic xscugic_inst;
static XCordic xcordic_inst;

static u8 rdy_irq = 0;


//
// Helper functions to handle angles
//
static float transform_to_quadrant1(const float phi, int *const quadrant)
{
  // This functions tranforms the incoming angle to a representation in the 1st quadrant.
  // This is not a simple modulus. For example 100° are actually transformed into 180°-100° = 80°.
  // This way the resulting angles of cosinus and sinus can easier be transformed back to the original quadrant.

  assert(quadrant);
  assert(phi <= M_PI * 2 && phi >= -M_PI * 2);

  static const float PI_2 = M_PI / 2;
  float transformedPhi = phi;

  if (phi >= 0)
  {
    if (phi > PI_2)
    {
      // 90° < phi <= 180°
      *quadrant = 2;
      transformedPhi = M_PI - phi;
    }
    else
    {
      // 0° < phi <= 90°
      *quadrant = 1;
      transformedPhi = phi;
    }
  }
  else
  {
    if (phi < -PI_2)
    {
      // -180° <= phi < -90°
      *quadrant = 3;
      transformedPhi = phi + M_PI;
    }
    else
    {
      // -90° <= phi <= 0°
      *quadrant = 4;
      transformedPhi = (phi * -1);
    }
  }

  return transformedPhi;
}

static void transform_to_orig_angle(float *const cos, float *const sin, const int origQuadrant)
{
  assert(cos && sin);

  // Due to the transformation used in `transform_to_quadrant1`
  // the back transformation only alters the sign bits
  switch (origQuadrant)
  {
  case 2:
    *cos *= -1;
  case 1:
    break;

  case 3:
    *cos *= -1;
  case 4:
    *sin *= -1;
    break;

  default:
    assert(0);
    break;
  }
}

static float norm_angle(float phi)
{
  // Cap any angle into the range [-180°; 180°]
  while (phi > M_PI)
  {
    phi -= 2 * M_PI;
  }
  while (phi < -M_PI)
  {
    phi += 2 * M_PI;
  }

  return phi;
}

//
// Driver implementation
//
CordicStatus_t CordicCalcXY(float phi, float *const cos, float *const sin, uint32_t *const addr)
{
  if (!cos || !sin) {
    return FAIL;
  }

  // This double cast avoids pointer-to-int warnings
  uint32_t addrInt = (uint32_t)(uintptr_t)addr;

  // Move input angle to the first quadrant
  int origQuadrant = 0;
  phi = norm_angle(phi);
  phi = transform_to_quadrant1(phi, &origQuadrant);

  // Write the input angle
  uint32_t fixedPhi = phi * pow(2, PHI_FRACTIONAL_BITS);
  CordicWrPhi(addrInt, fixedPhi);

  // Wait until calculation is finished
  // TODO: fix race condition...
  while (!rdy_irq) {
#ifdef EMUCPU
    Wait();
#endif
  }
  rdy_irq = 0;

  // Extract resulting angles
  uint32_t result = CordicRdXY(addrInt);
  uint16_t x = ((result >> 0) & 0xffff);
  uint16_t y = ((result >> 16) & 0xffff);

  // Convert angles back to floating point
  *cos = ((float)x) / pow(2, XY_FRACTIONAL_BITS);
  *sin = ((float)y) / pow(2, XY_FRACTIONAL_BITS);

  // Move angles back to their original quadrants
  transform_to_orig_angle(cos, sin, origQuadrant);

  return OK;
}

void CordicISRHandler(void *_)
{
  rdy_irq = 1;
}

CordicStatus_t cordic_init()
{
  int status;

  int success = XCordic_Initialize(&xcordic_inst, CORDIC_DEVICE_ID);
  if (success != XST_SUCCESS)
    return FAIL;


  //
  // Setup interrupt handling here
  //

  // Query the interrupt controller configuration entry
  status = XScuGic_Initialize(&xscugic_inst, INTC_DEVICE_ID);
  if (status != XST_SUCCESS) {
    return XST_FAILURE;
  }

  // Initialize the exception handling of the CPU
  Xil_ExceptionInit();

  // Register an universal exception handler for all interrupts.
  // The interrupt controller will further forward interrupts.
  Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
      (Xil_ExceptionHandler)XScuGic_InterruptHandler,
      &xscugic_inst);


  // Register an interrupt handler for the timer interrupt at the
  // interrupt controller.
  status = XScuGic_Connect(&xscugic_inst, CORDIC_INTR_ID,
              (Xil_ExceptionHandler)CordicISRHandler,
              (void *)&xcordic_inst);
  if (status != XST_SUCCESS) {
    return status;
  }

  // Enable the generic interrupt controller
  XScuGic_Enable(&xscugic_inst, CORDIC_INTR_ID);

  // Enable the interrupts of the timer
  XCordic_InterruptEnable(&xcordic_inst);

  // Enable interrupts for the processor
  Xil_ExceptionEnable();

  return OK;
}
