/**
 **********************************************************************************
 * @file   HT1382.h
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

/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef _HT1382_H_
#define _HT1382_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>


/* Exported Data Types ----------------------------------------------------------*/

/**
 * @brief  Library functions result data type
 */
typedef enum HT1382_Result_e
{
  HT1382_OK             = 0,
  HT1382_FAIL           = 1,
  HT1382_INVALID_PARAM  = 2,
} HT1382_Result_t;

/**
 * @brief  Function type for Initialize/Deinitialize the platform dependent layer.
 * @retval 
 *         -  0: The operation was successful.
 *         - -1: The operation failed. 
 */
typedef int8_t (*HT1382_PlatformInitDeinit_t)(void);

/**
 * @brief  Function type for Send/Receive data to/from the slave.
 * @param  Address: Address of slave (0 <= Address <= 127)
 * @param  Data: Pointer to data
 * @param  Len: data len in Bytes
 * @retval 
 *         -  0: The operation was successful.
 *         - -1: Failed to send/receive.
 *         - -2: Bus is busy.
 *         - -3: Slave doesn't ACK the transfer.
 */
typedef int8_t (*HT1382_PlatformSendReceive_t)(uint8_t Address,
                                               uint8_t *Data, uint8_t Len);

/**
 * @brief  Platform dependent layer data type
 * @note   It is optional to initialize this functions:
 *         - Init
 *         - DeInit
 * @note   It is mandatory to initialize this functions:
 *         - Send
 *         - Receive
 * @note   If success the functions must return 0 
 */
typedef struct HT1382_Platform_s
{
  // Initialize platform dependent layer
  HT1382_PlatformInitDeinit_t Init;
  // De-initialize platform dependent layer
  HT1382_PlatformInitDeinit_t DeInit;

  // Send data to the slave
  HT1382_PlatformSendReceive_t Send;
  // Receive data from the slave
  HT1382_PlatformSendReceive_t Receive;
} HT1382_Platform_t;

/**
 * @brief  Handler
 * @note   User must initialize platform dependent layer functions
 */
typedef struct HT1382_Handler_s
{
  HT1382_Platform_t Platform;
} HT1382_Handler_t;

/**
 * @brief  Date and time data type
 */
typedef struct HT1382_DateTime_s
{
  uint8_t   Second;
  uint8_t   Minute;
  uint8_t   Hour;
  uint8_t   WeekDay;
  uint8_t   Day;
  uint8_t   Month;
  uint8_t   Year;
} HT1382_DateTime_t;

/**
 * @brief  Wave output signal options
 */
typedef enum HT1382_OutWave_e
{
  HT1382_OUTWAVE_DISABLE    = 0,  // Disable SQW/OUT pin
  HT1382_OUTWAVE_32758HZ    = 1,  // 32758Hz
  HT1382_OUTWAVE_4096HZ     = 2,  // 4096Hz
  HT1382_OUTWAVE_1024HZ     = 3,  // 1024Hz
  HT1382_OUTWAVE_64HZ       = 4,  // 64Hz
  HT1382_OUTWAVE_32HZ       = 5,  // 32Hz
  HT1382_OUTWAVE_16HZ       = 6,  // 16Hz
  HT1382_OUTWAVE_8HZ        = 7,  // 8Hz
  HT1382_OUTWAVE_4HZ        = 8,  // 4Hz
  HT1382_OUTWAVE_2HZ        = 9,  // 2Hz
  HT1382_OUTWAVE_1HZ        = 10, // 1Hz
  HT1382_OUTWAVE_1_2HZ      = 11, // 1/2Hz
  HT1382_OUTWAVE_1_4HZ      = 12, // 1/4Hz
  HT1382_OUTWAVE_1_8HZ      = 13, // 1/8Hz
  HT1382_OUTWAVE_1_16HZ     = 14, // 1/16Hz
  HT1382_OUTWAVE_1_32HZ     = 15, // 1/32Hz
} HT1382_OutWave_t;


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Specify Send buffer size.
 * @note   larger buffer size => better performance
 * @note   The HT1382_SEND_BUFFER_SIZE must be set larger than 1 (9 or more is
 *         suggested)
 */   
#define HT1382_SEND_BUFFER_SIZE   9


/* Exported Macros --------------------------------------------------------------*/
/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define HT1382_PLATFORM_LINK_INIT(HANDLER, FUNC) \
  (HANDLER)->Platform.Init = FUNC

/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define HT1382_PLATFORM_LINK_DEINIT(HANDLER, FUNC) \
  (HANDLER)->Platform.DeInit = FUNC

/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define HT1382_PLATFORM_LINK_SEND(HANDLER, FUNC) \
  (HANDLER)->Platform.Send = FUNC

/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define HT1382_PLATFORM_LINK_RECEIVE(HANDLER, FUNC) \
  (HANDLER)->Platform.Receive = FUNC



/**
 ==================================================================================
                           ##### Common Functions #####                           
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
HT1382_Init(HT1382_Handler_t *Handler);


/**
 * @brief  Uninitialize HT1382 
 * @param  Handler: Pointer to handler
 * @retval HT1382_Result_t
 *         - HT1382_OK: Operation was successful.
 *         - HT1382_FAIL: Failed to send or receive data.
 */
HT1382_Result_t
HT1382_DeInit(HT1382_Handler_t *Handler);



/**
 ==================================================================================
                             ##### RTC Functions #####                             
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
HT1382_SetDateTime(HT1382_Handler_t *Handler, HT1382_DateTime_t *DateTime);


/**
 * @brief  Get date and time from HT1382 real time chip
 * @param  Handler: Pointer to handler
 * @param  DateTime: pointer to date and time value structure
 * @retval HT1382_Result_t
 *         - HT1382_OK: Operation was successful.
 *         - HT1382_FAIL: Failed to send or receive data.
 */
HT1382_Result_t
HT1382_GetDateTime(HT1382_Handler_t *Handler, HT1382_DateTime_t *DateTime);



/**
 ==================================================================================
                          ##### Out Wave Functions #####                           
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
HT1382_SetOutWave(HT1382_Handler_t *Handler, HT1382_OutWave_t OutWave);



#ifdef __cplusplus
}
#endif


#endif //! _HT1382_H_
