/**
 **********************************************************************************
 * @file   HT1382_platform.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  HT1382 chip driver platform dependent part
 *         Functionalities of the this file:
 *          + Initialization the platform-dependent part of handler
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
#ifndef	_HT1382_PLATFORM_H_
#define _HT1382_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include "HT1382.h"


/* Functionality Options --------------------------------------------------------*/
#define HT1382_HI2C      hi2c2



/**
 ==================================================================================
                             ##### Functions #####                                 
 ==================================================================================
 */

/**
 * @brief  Initialize platform device to communicate HT1382.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
HT1382_Platform_Init(HT1382_Handler_t *Handler);


#ifdef __cplusplus
}
#endif


#endif //! _HT1382_PLATFORM_H_
