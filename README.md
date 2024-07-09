# HT1382 Library
Library for handling HT1382 Real Time Clock chip.

## Library Features
- Time and date management
- non-volatile internal RAM management
- Output square wave management

## Hardware Support
It is easy to port this library to any platform. But now it is ready for use in:
- AVR (ATmega32)
- ESP32 (esp-idf)
- STM32 (HAL)

## How To Use
1. Add `HT1382.h` and `HT1382.c` files to your project.  It is optional to use `HT1382_platform.h` and `HT1382_platform.c` files (open and config `HT1382_platform.h` file).
2. Initialize platform-dependent part of handler.
4. Call `HT1382_Init()`.
5. Call other functions and enjoy.

## Example
<details>
<summary>Using HT1382_platform files</summary>

```c
#include <stdio.h>
#include "HT1382.h"
#include "HT1382_platform.h"

int main(void)
{
  HT1382_Handler_t Handler;
  HT1382_DateTime_t DateTime =
  {
    .Second   = 0,
    .Minute   = 0,
    .Hour     = 14,
    .WeekDay  = 4,
    .Day      = 9,
    .Month    = 7,
    .Year     = 24
  };

  HT1382_Platform_Init(&Handler);
  HT1382_Init(&Handler);
  HT1382_SetDateTime(&Handler, &DateTime);
  HT1382_SetOutWave(&Handler, HT1382_OUTWAVE_1HZ);

  while (1)
  {
    HT1382_GetDateTime(&Handler, &DateTime);
    printf("Date: 20%02u/%02u/%02u\r\n", DateTime.Year, DateTime.Month, DateTime.Day);
    printf("Time: %02u:%02u:%02u\r\n", DateTime.Hour, DateTime.Minute, DateTime.Second);
    printf("WeekDay: %u\r\n", DateTime.WeekDay);
  }

  HT1382_DeInit(&Handler);
  return 0;
}
```
</details>


<details>
<summary>Without using HT1382_platform files (esp-idf)</summary>

```c
#include <stdio.h>
#include <stdint.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_err.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "HT1382.h"

#define HT1382_I2C_NUM   I2C_NUM_1
#define HT1382_I2C_RATE  100000
#define HT1382_SCL_GPIO  GPIO_NUM_13
#define HT1382_SDA_GPIO  GPIO_NUM_14

int8_t
HT1382_Platform_Init(void)
{
  i2c_config_t conf = {0};
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = HT1382_SDA_GPIO;
  conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
  conf.scl_io_num = HT1382_SCL_GPIO;
  conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
  conf.master.clk_speed = HT1382_I2C_RATE;
  if (i2c_param_config(HT1382_I2C_NUM, &conf) != ESP_OK)
    return -1;
  if (i2c_driver_install(HT1382_I2C_NUM, conf.mode, 0, 0, 0) != ESP_OK)
    return -1;
  return 0;
}

int8_t
HT1382_Platform_DeInit(void)
{
  i2c_driver_delete(HT1382_I2C_NUM);
  gpio_reset_pin(HT1382_SDA_GPIO);
  gpio_reset_pin(HT1382_SCL_GPIO);
  return 0;
}

int8_t
HT1382_Platform_Send(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  i2c_cmd_handle_t HT1382_i2c_cmd_handle = 0;
  Address <<= 1;
  Address &= 0xFE;

  HT1382_i2c_cmd_handle = i2c_cmd_link_create();
  i2c_master_start(HT1382_i2c_cmd_handle);
  i2c_master_write(HT1382_i2c_cmd_handle, &Address, 1, 1);
  i2c_master_write(HT1382_i2c_cmd_handle, Data, DataLen, 1);
  i2c_master_stop(HT1382_i2c_cmd_handle);
  if (i2c_master_cmd_begin(HT1382_I2C_NUM, HT1382_i2c_cmd_handle, 1000 / portTICK_RATE_MS) != ESP_OK)
  {
    i2c_cmd_link_delete(HT1382_i2c_cmd_handle);
    return -1;
  }
  i2c_cmd_link_delete(HT1382_i2c_cmd_handle);
  return 0;
}

int8_t
HT1382_Platform_Receive(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
  i2c_cmd_handle_t HT1382_i2c_cmd_handle = 0;
  Address <<= 1;
  Address |= 0x01;

  HT1382_i2c_cmd_handle = i2c_cmd_link_create();
  i2c_master_start(HT1382_i2c_cmd_handle);
  i2c_master_write(HT1382_i2c_cmd_handle, &Address, 1, 1);
  i2c_master_read(HT1382_i2c_cmd_handle, Data, DataLen, I2C_MASTER_LAST_NACK);
  i2c_master_stop(HT1382_i2c_cmd_handle);
  if (i2c_master_cmd_begin(HT1382_I2C_NUM, HT1382_i2c_cmd_handle, 1000 / portTICK_RATE_MS) != ESP_OK)
  {
    i2c_cmd_link_delete(HT1382_i2c_cmd_handle);
    return -1;
  }
  i2c_cmd_link_delete(HT1382_i2c_cmd_handle);
  return 0;
}

int main(void)
{
  HT1382_Handler_t Handler;
  HT1382_DateTime_t DateTime =
  {
    .Second   = 0,
    .Minute   = 0,
    .Hour     = 14,
    .WeekDay  = 4,
    .Day      = 9,
    .Month    = 7,
    .Year     = 24
  };

  HT1382_PLATFORM_LINK_INIT(&Handler, HT1382_Platform_Init);
  HT1382_PLATFORM_LINK_DEINIT(&Handler, HT1382_Platform_DeInit);
  HT1382_PLATFORM_LINK_SEND(&Handler, HT1382_Platform_Send);
  HT1382_PLATFORM_LINK_RECEIVE(&Handler, HT1382_Platform_Receive);

  HT1382_Init(&Handler);
  HT1382_SetDateTime(&Handler, &DateTime);
  HT1382_SetOutWave(&Handler, HT1382_OUTWAVE_1HZ);

  while (1)
  {
    HT1382_GetDateTime(&Handler, &DateTime);
    printf("Date: 20%02u/%02u/%02u\r\n", DateTime.Year, DateTime.Month, DateTime.Day);
    printf("Time: %02u:%02u:%02u\r\n", DateTime.Hour, DateTime.Minute, DateTime.Second);
    printf("WeekDay: %u\r\n", DateTime.WeekDay);
  }

  HT1382_DeInit(&Handler);
  return 0;
}
```
</details>