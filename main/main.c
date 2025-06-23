/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <stdint.h>
#include <sys/param.h>

#include "sdkconfig.h"
#include "main/tcp_server.h"
#include "main/tcp_netconn.h"
#include "main/kcp_server.h"
#include "main/uart_bridge.h"
#include "main/timer.h"
#include "main/wifi_configuration.h"
#include "main/wifi_handle.h"

#include "components/corsacOTA/src/corsacOTA.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "mdns.h"

extern void DAP_Setup(void);
extern void DAP_Thread(void *argument);
extern void SWO_Thread();

TaskHandle_t kDAPTaskHandle = NULL;

static const char *MDNS_TAG = "server_common";

#if defined(CONFIG_IDF_TARGET_ESP32S3)
#define DAP_TASK_AFFINITY 1
#else
#define DAP_TASK_AFFINITY 0
#endif

void mdns_setup() {
    // initialize mDNS
    int ret;
    ret = mdns_init();
    if (ret != ESP_OK) {
        ESP_LOGW(MDNS_TAG, "mDNS initialize failed:%d", ret);
        return;
    }

    // set mDNS hostname
    ret = mdns_hostname_set(MDNS_HOSTNAME);
    if (ret != ESP_OK) {
        ESP_LOGW(MDNS_TAG, "mDNS set hostname failed:%d", ret);
        return;
    }
    ESP_LOGI(MDNS_TAG, "mDNS hostname set to: [%s]", MDNS_HOSTNAME);

    // set default mDNS instance name
    ret = mdns_instance_name_set(MDNS_INSTANCE);
    if (ret != ESP_OK) {
        ESP_LOGW(MDNS_TAG, "mDNS set instance name failed:%d", ret);
        return;
    }
    ESP_LOGI(MDNS_TAG, "mDNS instance name set to: [%s]", MDNS_INSTANCE);
}

void app_main() {
    // struct rst_info *rtc_info = system_get_rst_info();

    // os_printf("reset reason: %x\n", rtc_info->reason);

    // if (rtc_info->reason == REASON_WDT_RST ||
    //     rtc_info->reason == REASON_EXCEPTION_RST ||
    //     rtc_info->reason == REASON_SOFT_WDT_RST)
    // {
    // if (rtc_info->reason == REASON_EXCEPTION_RST)
    // {
    //     os_printf("Fatal exception (%d):\n", rtc_info->exccause);
    // }
    // os_printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x,excvaddr=0x%08x, depc=0x%08x\n",
    //             rtc_info->epc1, rtc_info->epc2, rtc_info->epc3,
    //             rtc_info->excvaddr, rtc_info->depc);
    // }

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_LOGI(MDNS_TAG, "mDNS instance name set to: [%s]", MDNS_INSTANCE);

}
