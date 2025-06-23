#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// 定义LED引脚
#define LED_PIN 2  // ESP8266内置LED通常连接到GPIO2

static const char *TAG = "LED_TASK";

// LED闪烁任务
void led_blink_task(void *pvParameter)
{
    // 配置GPIO
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << LED_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    int level = 0;

    while(1) {
        // 设置LED状态
        gpio_set_level(LED_PIN, level);

        // 输出日志
        ESP_LOGI(TAG, "Turning the LED %s!", level ? "ON" : "OFF");

        // 切换LED状态
        level = !level;

        // 延时500ms
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    // 创建LED闪烁任务
    xTaskCreate(led_blink_task, "led_blink_task", 2048, NULL, 5, NULL);

    // app_main函数可以退出，因为LED任务会继续运行
}
