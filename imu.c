#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"
#include "i2c/i2c.h"

#define SCL_PIN (0)
#define SDA_PIN (2)
#define MPU_ADDR 0x68

const int gpio = 2;

void blinkenTask(void *pvParameters)
{
    gpio_enable(gpio, GPIO_OUTPUT);
    while(1) {
        gpio_write(gpio, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_write(gpio, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void read_gyro(void *pvParameters)
{
    uint8_t * chip_id;

    while(1)
    {   
        i2c_slave_read(1,MPU_ADDR, 0x75, chip_id, 1);
        printf("Chip ID: %x\n",chip_id);
        vTaskDelay(10000/portTICK_PERIOD_MS);
    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    i2c_init(1,SCL_PIN,SDA_PIN,I2C_FREQ_400K);
    xTaskCreate(blinkenTask, "blinkenTask", 256, NULL, 2, NULL);
    xTaskCreate(read_gyro, "read_gyro", 256, NULL, 2, NULL);
}