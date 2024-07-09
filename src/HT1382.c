/**
 **********************************************************************************
 * @file   HT1382.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  HT1382 RTC chip driver
 **********************************************************************************
 *
 * Copyright (c) 2024 Mahda Embedded System (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */

/* Includes ---------------------------------------------------------------------*/
#include <string.h>
#include "HT1382.h"


/* Private Constants ------------------------------------------------------------*/
/**
 * @brief  The HT1382 Address on I2C BUS
 */ 
#define HT1382_ADDRESS  0x68

/**
 * @brief  Internal Registers Address
 */ 
#define HT1382_REG_ADDR_SECONDS         0x00
#define HT1382_REG_ADDR_MINUTES         0x01
#define HT1382_REG_ADDR_HOURS           0x02
#define HT1382_REG_ADDR_DATE            0x03
#define HT1382_REG_ADDR_MONTH           0x04
#define HT1382_REG_ADDR_DAY             0x05
#define HT1382_REG_ADDR_YEAR            0x06
#define HT1382_REG_ADDR_ST1             0x07
#define HT1382_REG_ADDR_ST2             0x08
#define HT1382_REG_ADDR_INT             0x09
#define HT1382_REG_ADDR_SECONDS_ALARM   0x0A
#define HT1382_REG_ADDR_MINUTES_ALARM   0x0B
#define HT1382_REG_ADDR_HOURS_ALARM     0x0C
#define HT1382_REG_ADDR_DATE_ALARM      0x0D
#define HT1382_REG_ADDR_MONTH_ALARM     0x0E
#define HT1382_REG_ADDR_DAY_ALARM       0x0F
#define HT1382_REG_ADDR_DT              0x10
#define HT1382_REG_ADDR_USR1            0x11
#define HT1382_REG_ADDR_USR2            0x12
#define HT1382_REG_ADDR_USR3            0x13
#define HT1382_REG_ADDR_USR4            0x14

/**
 * @brief  Register bits of Seconds register
 */
#define HT1382_SECONDS_CH               7


/**
 * @brief  Register bits of ST1 register
 */
#define HT1382_ST1_WP                   7

/**
 * @brief  Register bits of ST2 register
 */
#define HT1382_ST2_BE                   1
#define HT1382_ST2_AI                   2
#define HT1382_ST2_EB                   3
#define HT1382_ST2_EWE                  4
#define HT1382_ST2_ARE                  7

/**
 * @brief  Register bits of INT register
 */
#define HT1382_INT_FO0                  0
#define HT1382_INT_FO1                  1
#define HT1382_INT_FO2                  2
#define HT1382_INT_FO3                  3
#define HT1382_INT_OEOBM                4
#define HT1382_INT_LPM                  5
#define HT1382_INT_AE                   6
#define HT1382_INT_IME                  7

/**
 * @brief  Register bits of Seconds Alarm register
 */
#define HT1382_SECONDS_ALARM_SECEN      7

/**
 * @brief  Register bits of Minutes Alarm register
 */
#define HT1382_MINUTES_ALARM_MINEN      7

/**
 * @brief  Register bits of Hours Alarm register
 */
#define HT1382_HOURS_ALARM_HREN         7

/**
 * @brief  Register bits of Date Alarm register
 */
#define HT1382_DATE_ALARM_DTEN          7

/**
 * @brief  Register bits of Month Alarm register
 */
#define HT1382_MONTH_ALARM_MOEN         7

/**
 * @brief  Register bits of Day Alarm register
 */
#define HT1382_DAY_ALARM_DAYEN          7

/**
 * @brief  Register bits of DT register
 */
#define HT1382_DT_DT0                   0
#define HT1382_DT_DT1                   1
#define HT1382_DT_DT2                   2
#define HT1382_DT_DT3                   3
#define HT1382_DT_DT4                   4
#define HT1382_DT_DT5                   5
#define HT1382_DT_DT6                   6
#define HT1382_DT_DTS                   7


/* Private Macro ----------------------------------------------------------------*/
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static uint8_t
HT1382_DECtoBCD(uint8_t DEC)
{
  uint8_t Buff2 = DEC % 10;
  uint8_t Buff1 = (DEC / 10) % 10;
  uint8_t BCD   = (Buff1 << 4) | Buff2;

  return BCD;
}

static uint8_t
HT1382_BCDtoDEC(uint8_t BCD)
{
  uint8_t Buff1 = BCD >> 4;
  uint8_t Buff2 = BCD & 0x0f;
  uint8_t DEC   = (Buff1 * 10) + Buff2;

  return DEC;
}

static int8_t
HT1382_WriteRegs(HT1382_Handler_t *Handler,
                 uint8_t StartReg, uint8_t *Data, uint8_t BytesCount)
{
  uint8_t Buffer[HT1382_SEND_BUFFER_SIZE] = {0};
  uint8_t Len = 0;

  Buffer[0] = StartReg; // send register address to set RTC pointer
  while (BytesCount)
  {
    Len = MIN(BytesCount, sizeof(Buffer)-1);
    memcpy((void*)(Buffer+1), (const void*)Data, Len);

    if (Handler->Platform.Send(HT1382_ADDRESS, Buffer, Len+1) < 0)
      return -1;

    Data += Len;
    Buffer[0] += Len;
    BytesCount -= Len;
  }

  return 0;
}

static int8_t
HT1382_ReadRegs(HT1382_Handler_t *Handler,
                uint8_t StartReg, uint8_t *Data, uint8_t BytesCount)
{
  if (Handler->Platform.Send(HT1382_ADDRESS, &StartReg, 1) < 0)
    return -1;

  if (Handler->Platform.Receive(HT1382_ADDRESS, Data, BytesCount) < 0)
    return -1;

  return 0;
}

static int8_t
HT1382_WriteProtection(HT1382_Handler_t *Handler, uint8_t Enable)
{
  if (Enable)
    Enable = (1 << HT1382_ST1_WP);

  return HT1382_WriteRegs(Handler, HT1382_REG_ADDR_ST1, &Enable, 1);
}



/**
 ==================================================================================
                       ##### Public Common Functions #####                         
 ==================================================================================
 */

/**
 * @brief  Initialize HT1382 
 * @param  Handler: Pointer to handler
 * @retval HT1382_Result_t
 *         - HT1382_OK: Operation was successful.
 *         - HT1382_FAIL: Failed to send or receive data.
 *         - HT1382_INVALID_PARAM: One of parameters is invalid.
 */
HT1382_Result_t
HT1382_Init(HT1382_Handler_t *Handler)
{
  if (!Handler->Platform.Send ||
      !Handler->Platform.Receive)
    return HT1382_INVALID_PARAM;

  if (Handler->Platform.Init)
    if (Handler->Platform.Init() < 0)
      return HT1382_FAIL;

  return HT1382_OK;
}

/**
 * @brief  Uninitialize HT1382 
 * @param  Handler: Pointer to handler
 * @retval HT1382_Result_t
 *         - HT1382_OK: Operation was successful.
 *         - HT1382_FAIL: Failed to send or receive data.
 */
HT1382_Result_t
HT1382_DeInit(HT1382_Handler_t *Handler)
{
  if (Handler->Platform.DeInit)
    if (Handler->Platform.DeInit() < 0)
      return HT1382_FAIL;

  return HT1382_OK;
}



/**
 ==================================================================================
                         ##### Public RTC Functions #####                          
 ==================================================================================
 */

/**
 * @brief  Set date and time on HT1382 real time chip
 * @param  Handler: Pointer to handler
 * @param  DateTime: pointer to date and time value structure
 * @retval HT1382_Result_t
 *         - HT1382_OK: Operation was successful.
 *         - HT1382_FAIL: Failed to send or receive data.
 *         - HT1382_INVALID_PARAM: One of parameters is invalid.
 */
HT1382_Result_t
HT1382_SetDateTime(HT1382_Handler_t *Handler, HT1382_DateTime_t *DateTime)
{
  uint8_t Buffer[7] = {0};

  if (DateTime->Second > 59 ||
      DateTime->Minute > 59 ||
      DateTime->Hour > 23 ||
      DateTime->WeekDay > 7 || DateTime->WeekDay == 0 ||
      DateTime->Day > 31 || DateTime->Day == 0 ||
      DateTime->Month > 12 || DateTime->Month == 0 ||
      DateTime->Year > 99)
    return HT1382_INVALID_PARAM;

  // convert value of parameter to BCD
  Buffer[HT1382_REG_ADDR_SECONDS] = HT1382_DECtoBCD(DateTime->Second) & ~(1 << HT1382_SECONDS_CH);
  Buffer[HT1382_REG_ADDR_MINUTES] = HT1382_DECtoBCD(DateTime->Minute);
  Buffer[HT1382_REG_ADDR_HOURS]   = HT1382_DECtoBCD(DateTime->Hour);
  Buffer[HT1382_REG_ADDR_DAY]     = HT1382_DECtoBCD(DateTime->WeekDay);
  Buffer[HT1382_REG_ADDR_DATE]    = HT1382_DECtoBCD(DateTime->Day);
  Buffer[HT1382_REG_ADDR_MONTH]   = HT1382_DECtoBCD(DateTime->Month);
  Buffer[HT1382_REG_ADDR_YEAR]    = HT1382_DECtoBCD(DateTime->Year);

  if (HT1382_WriteProtection(Handler, 0) < 0)
    return HT1382_FAIL;

  if (HT1382_WriteRegs(Handler, HT1382_REG_ADDR_SECONDS, Buffer, 7) < 0)
    return HT1382_FAIL;

  if (HT1382_WriteProtection(Handler, 1) < 0)
    return HT1382_FAIL;
  
  return HT1382_OK;
}


/**
 * @brief  Get date and time from HT1382 real time chip
 * @param  Handler: Pointer to handler
 * @param  DateTime: pointer to date and time value structure
 * @retval HT1382_Result_t
 *         - HT1382_OK: Operation was successful.
 *         - HT1382_FAIL: Failed to send or receive data.
 */
HT1382_Result_t
HT1382_GetDateTime(HT1382_Handler_t *Handler, HT1382_DateTime_t *DateTime)
{
  uint8_t Buffer[7] = {0};

  if (HT1382_ReadRegs(Handler, HT1382_REG_ADDR_SECONDS, Buffer, 7) < 0)
    return HT1382_FAIL;

  // convert BCD value to decimal
  DateTime->Second  = HT1382_BCDtoDEC(Buffer[HT1382_REG_ADDR_SECONDS] & ~(1 << HT1382_SECONDS_CH));
  DateTime->Minute  = HT1382_BCDtoDEC(Buffer[HT1382_REG_ADDR_MINUTES]);
  DateTime->Hour    = HT1382_BCDtoDEC(Buffer[HT1382_REG_ADDR_HOURS]);
  DateTime->WeekDay = HT1382_BCDtoDEC(Buffer[HT1382_REG_ADDR_DAY]);
  DateTime->Day     = HT1382_BCDtoDEC(Buffer[HT1382_REG_ADDR_DATE]);
  DateTime->Month   = HT1382_BCDtoDEC(Buffer[HT1382_REG_ADDR_MONTH]);
  DateTime->Year    = HT1382_BCDtoDEC(Buffer[HT1382_REG_ADDR_YEAR]);

  return HT1382_OK;
}



/**
 ==================================================================================
                     ##### Public Out Wave Functions #####                         
 ==================================================================================
 */

/**
 * @brief  Set output Wave on SQW/Out pin of HT1382
 * @param  Handler: Pointer to handler
 * @param  OutWave: where OutWave Shows different output wave states
 * @retval HT1382_Result_t
 *         - HT1382_OK: Operation was successful.
 *         - HT1382_FAIL: Failed to send or receive data.
 *         - HT1382_INVALID_PARAM: One of parameters is invalid.
 */
HT1382_Result_t
HT1382_SetOutWave(HT1382_Handler_t *Handler, HT1382_OutWave_t OutWave)
{
  uint8_t RegBuffer = 0;

  if (OutWave > HT1382_OUTWAVE_1_32HZ)
    return HT1382_INVALID_PARAM;

  if (HT1382_ReadRegs(Handler, HT1382_REG_ADDR_INT, &RegBuffer, 1) < 0)
    return HT1382_FAIL;

  RegBuffer &= ~(0x0F << HT1382_INT_FO0);
  RegBuffer |= ((OutWave & 0x0F) << HT1382_INT_FO0);

  if (HT1382_WriteProtection(Handler, 0) < 0)
    return HT1382_FAIL;
  
  if (HT1382_WriteRegs(Handler, HT1382_REG_ADDR_INT, &RegBuffer, 1) < 0)
    return HT1382_FAIL;

  if (HT1382_WriteProtection(Handler, 1) < 0)
    return HT1382_FAIL;

  if (HT1382_ReadRegs(Handler, HT1382_REG_ADDR_INT, &RegBuffer, 1) < 0)
    return HT1382_FAIL;

  return HT1382_OK; 
}
