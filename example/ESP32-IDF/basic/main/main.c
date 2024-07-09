/**
 **********************************************************************************
 * @file   main.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  example code for HT1382 Driver (for ESP32-IDF)
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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "HT1382.h"
#include "HT1382_platform.h"

static const char *TAG = "example";

void app_main(void)
{
  HT1382_Handler_t Handler;
  HT1382_DateTime_t DateTime =
  {
    .Second   = 0,
    .Minute   = 2,
    .Hour     = 10,
    .WeekDay  = 5,
    .Day      = 23,
    .Month    = 11,
    .Year     = 23
  };

  ESP_LOGI(TAG, "HT1382 Driver Example");

  HT1382_Platform_Init(&Handler);
  HT1382_Init(&Handler);
  HT1382_SetDateTime(&Handler, &DateTime);
  HT1382_SetOutWave(&Handler, HT1382_OUTWAVE_1HZ);

  while (1)
  {
    HT1382_GetDateTime(&Handler, &DateTime);
    ESP_LOGI(TAG, "Date: 20%02u/%02u/%02u", DateTime.Year, DateTime.Month, DateTime.Day);
    ESP_LOGI(TAG, "Time: %02u:%02u:%02u", DateTime.Hour, DateTime.Minute, DateTime.Second);
    ESP_LOGI(TAG, "WeekDay: %u", DateTime.WeekDay);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  HT1382_DeInit(&Handler);
}
