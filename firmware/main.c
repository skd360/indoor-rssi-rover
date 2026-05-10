#include <stdio.h>

#include "espnov_rx.h"
#include "web_server.h"          // ← NEW include

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(1000));

    // ── WiFi AP + HTTP server (handles NVS init internally) ──
    wifi_ap_init();              // ← NEW
    http_server_init();          // ← NEW

    // ── ESP-NOW init (runs on top of WiFi driver already started) ──
    init_all();

    while (1)
    {
        int64_t now = esp_timer_get_time();

        for (int i = 0; i < 4; i++)
        {
            if ((now - last_seen[i]) > 2000000)  // 2 sec timeout
                rssi1[i] = 0;
        }

        printf("%d %d %d %d\n",
               rssi1[0], rssi1[1],
               rssi1[2], rssi1[3]);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}