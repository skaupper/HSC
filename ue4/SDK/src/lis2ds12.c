/*****************************************************************************/
/**
 * @file    lis2ds12.c
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Implementation of functions to interact with the
 *          LIS2DS12 sensor through I2C.
 */
/*****************************************************************************/

#include "lis2ds12.h"


static u8 iicDeviceAddress = 0x00;



int LIS2DS12_Init(void)
{
  unsigned int bytes;

  //
  // Check sensor identity
  //

  // The WHO_AM_I register should be at a fixed 0x43.
  // If the read data byte does not match that value, something failed.
  u8 whoAmI;

  iicDeviceAddress = MINIZED_MOTION_SENSOR_ADDRESS_SA0_HI;
  bytes = LIS2DS12_ReadReg(LIS2DS12_ACC_WHO_AM_I_REG, &whoAmI, 1);
  if (bytes == 0) {
    return XST_FAILURE;
  }

  if (whoAmI != LIS2DS12_ACC_WHO_AM_I) {
    // Maybe the address bit was changed, try the other one
    iicDeviceAddress = MINIZED_MOTION_SENSOR_ADDRESS_SA0_LO;
    bytes = LIS2DS12_ReadReg(LIS2DS12_ACC_WHO_AM_I_REG, &whoAmI, 1);
    if (bytes == 0) {
      return XST_FAILURE;
    }

    if (whoAmI != LIS2DS12_ACC_WHO_AM_I) {
      return XST_FAILURE;
    }
  }


  //
  // Configure sensor
  //

  u8 send_byte;

  // No auto increment of addresses
  send_byte = 0x00;
  bytes = LIS2DS12_WriteReg(LIS2DS12_ACC_CTRL2, &send_byte, 1);
  if (bytes == 0) {
    return XST_FAILURE;
  }

  // Turn on the accelerometer.
  //    14-bit mode, ODR = 400 Hz, FS = 2g
  send_byte = 0x60;
  bytes = LIS2DS12_WriteReg(LIS2DS12_ACC_CTRL1, &send_byte, 1);
  if (bytes == 0) {
    return XST_FAILURE;
  }

  // Enable interrupt
  //    Acc data-ready interrupt on INT1
  send_byte = 0x01;
  bytes = LIS2DS12_WriteReg(LIS2DS12_ACC_CTRL4, &send_byte, 1);
  if (bytes == 0) {
    return XST_FAILURE;
  }

  return XST_SUCCESS;
}

u16 LIS2DS12_GetX(void)
{
  u8 lsb, msb;
  LIS2DS12_ReadReg(LIS2DS12_ACC_OUT_X_L, &lsb, 1);
  LIS2DS12_ReadReg(LIS2DS12_ACC_OUT_X_H, &msb, 1);
  return (((u16)msb) << 8) + lsb;
}

u16 LIS2DS12_GetY(void)
{
  u8 lsb, msb;
  LIS2DS12_ReadReg(LIS2DS12_ACC_OUT_Y_L, &lsb, 1);
  LIS2DS12_ReadReg(LIS2DS12_ACC_OUT_Y_H, &msb, 1);
  return (((u16)msb) << 8) + lsb;
}

u16 LIS2DS12_GetZ(void)
{
  u8 lsb, msb;
  LIS2DS12_ReadReg(LIS2DS12_ACC_OUT_Z_L, &lsb, 1);
  LIS2DS12_ReadReg(LIS2DS12_ACC_OUT_Z_H, &msb, 1);
  return (((u16)msb) << 8) + lsb;
}

int LIS2DS12_GetTemp(void)
{
  static const int TEMP_OFFSET = 25;

  u8 uRawTemp;
  s8 sRawTemp;
  unsigned int bytes;

  bytes = LIS2DS12_ReadReg(LIS2DS12_ACC_OUT_T, &uRawTemp, 1);
  if (bytes != 1) {
    return 0;
  }

  // Copy unsigned temperature bitwise into the signed temperature
  // The temperature value returned from the sensor is signed!
  memcpy(&sRawTemp, &uRawTemp, 1);

  return ((int)sRawTemp) + TEMP_OFFSET;
}


unsigned int LIS2DS12_ReadReg(u8 reg, u8 *buf, u8 len)
{
  u8 writeBuffer = reg;
  unsigned int bytes;

  bytes = XIic_Send(IIC_BASE_ADDRESS, iicDeviceAddress,
                    (u8*)&writeBuffer, 1, XIIC_REPEATED_START);
  if (bytes != 1) {
    return bytes;
  }

  bytes = XIic_Recv(IIC_BASE_ADDRESS, iicDeviceAddress,
                    (u8*)buf, len, XIIC_STOP);
  return bytes;
}

unsigned int LIS2DS12_WriteReg(u8 reg, u8 *buf, u8 len)
{
  static u8 writeBuffer[257];
  unsigned int bytes;

  writeBuffer[0] = reg;

  for (int i = 0; i < len; i++) {
    writeBuffer[i+1] = buf[i];
  }

  bytes = XIic_Send(IIC_BASE_ADDRESS, iicDeviceAddress,
                    &writeBuffer[0], (len+1), XIIC_STOP);
  return bytes;
}
