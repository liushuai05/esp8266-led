#include <string.h>
#include <stdint.h>
#include <sys/param.h>

#include "sdkconfig.h"
#include "driver/gpio.h"  // 包含 GPIO 驱动头文件
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 定义 LED 引脚
#define LED_PIN 2  // ESP8266 内置 LED 通常连接到 GPIO2

static const char *TAG = "LED_TASK";

// LED 闪烁任务
void led_blink_task(void *pvParameter)
{
    // 配置 GPIO
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << LED_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    int level = 0;

    while(1) {
        // 设置 LED 状态
        gpio_set_level(LED_PIN, level);

        // 输出日志
        ESP_LOGI(TAG, "Turning the LED %s!", level ? "ON" : "OFF");

        // 切换 LED 状态
        level = !level;

        // 延时 500ms
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    // 创建 LED 闪烁任务
    xTaskCreate(led_blink_task, "led_blink_task", 2048, NULL, 5, NULL);
}
