/*****************************************************************************/
/**
 * @file    lis2ds12.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Constants and function prototypes to interact with the
 *          LIS2DS12 sensor through I2C.
 */
/*****************************************************************************/

#ifndef _LIS2DS12_H_
#define _LIS2DS12_H_

#include <xparameters.h>
#include <xiic.h>



//
// Constants
//

#define IIC_BASE_ADDRESS  XPAR_IIC_0_BASEADDR


#define LIS2DS12_ACC_WHO_AM_I                 0x43


// Sensor addresses
#define MINIZED_MOTION_SENSOR_ADDRESS_SA0_LO  0x1E // 0011110b for LIS2DS12 on MiniZed when SA0 is pulled low
#define MINIZED_MOTION_SENSOR_ADDRESS_SA0_HI  0x1D // 0011101b for LIS2DS12 on MiniZed when SA0 is pulled high

// Device registers
#define LIS2DS12_ACC_SENSORHUB_OUT1           0x06
#define LIS2DS12_ACC_SENSORHUB_OUT2           0x07
#define LIS2DS12_ACC_SENSORHUB_OUT3           0x08
#define LIS2DS12_ACC_SENSORHUB_OUT4           0x09
#define LIS2DS12_ACC_SENSORHUB_OUT5           0x0A
#define LIS2DS12_ACC_SENSORHUB_OUT6           0x0B
#define LIS2DS12_ACC_MODULE_8BIT              0x0C
#define LIS2DS12_ACC_WHO_AM_I_REG             0x0F
#define LIS2DS12_ACC_CTRL1                    0x20
#define LIS2DS12_ACC_CTRL2                    0x21
#define LIS2DS12_ACC_CTRL3                    0x22
#define LIS2DS12_ACC_CTRL4                    0x23
#define LIS2DS12_ACC_CTRL5                    0x24
#define LIS2DS12_ACC_FIFO_CTRL                0x25
#define LIS2DS12_ACC_OUT_T                    0x26
#define LIS2DS12_ACC_STATUS                   0x27
#define LIS2DS12_ACC_OUT_X_L                  0x28
#define LIS2DS12_ACC_OUT_X_H                  0x29
#define LIS2DS12_ACC_OUT_Y_L                  0x2A
#define LIS2DS12_ACC_OUT_Y_H                  0x2B
#define LIS2DS12_ACC_OUT_Z_L                  0x2C
#define LIS2DS12_ACC_OUT_Z_H                  0x2D
#define LIS2DS12_ACC_FIFO_THS                 0x2E
#define LIS2DS12_ACC_FIFO_SRC                 0x2F
#define LIS2DS12_ACC_FIFO_SAMPLES             0x30
#define LIS2DS12_ACC_TAP_6D_THS               0x31
#define LIS2DS12_ACC_INT_DUR                  0x32
#define LIS2DS12_ACC_WAKE_UP_THS              0x33
#define LIS2DS12_ACC_WAKE_UP_DUR              0x34
#define LIS2DS12_ACC_FREE_FALL                0x35
#define LIS2DS12_ACC_STATUS_DUP               0x36
#define LIS2DS12_ACC_WAKE_UP_SRC              0x37
#define LIS2DS12_ACC_TAP_SRC                  0x38
#define LIS2DS12_ACC_6D_SRC                   0x39
#define LIS2DS12_ACC_STEP_C_MINTHS            0x3A
#define LIS2DS12_ACC_STEP_C_L                 0x3B
#define LIS2DS12_ACC_STEP_C_H                 0x3C
#define LIS2DS12_ACC_FUNC_CK_GATE             0x3D
#define LIS2DS12_ACC_FUNC_SRC                 0x3E
#define LIS2DS12_ACC_FUNC_CTRL                0x3F



//
// Function prototypes
//

int LIS2DS12_Init(void);
u16 LIS2DS12_GetX(void);
u16 LIS2DS12_GetY(void);
u16 LIS2DS12_GetZ(void);
int LIS2DS12_GetTemp(void);


// Low level register access functions
unsigned int LIS2DS12_ReadReg(u8 reg, u8 *buf, u8 len);
unsigned int LIS2DS12_WriteReg(u8 reg, u8 *buf, u8 len);

#endif /* _LIS2DS12_H_ */
