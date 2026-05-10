
#include "espnov_rx.h"

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

volatile int64_t last_seen[4] = {0,0,0,0};
volatile int rssi1[4] = {0, 0, 0, 0};

QueueHandle_t espnow_queue;

// #include "espnov_rx.h"

// typedef struct __attribute__((packed))
// {
//     // uint8_t type;
//     uint16_t seq_num;
//     uint8_t node_id;
//     uint32_t timestamp;
// } packet_t;

// static const char *TAG = "MAC_EXTRACT";

// Define your peer MAC addresses
// uint8_t peer1_mac[6] = {0x98, 0xA3, 0x16, 0x7E, 0x61, 0x78};
// uint8_t peer2_mac[6] = {0x98, 0xA3, 0x16, 0x7E, 0x6C, 0x04};
// uint8_t peer3_mac[6] = {0x98, 0xA3, 0x16, 0x7E, 0x58, 0xC4};
// uint8_t peer4_mac[6] = {0x98, 0xA3, 0x16, 0x7E, 0x6F, 0xEC};

// // Put them in a list for easy processing
// uint8_t *peer_list[] = {peer1_mac, peer2_mac, peer3_mac, peer4_mac};
// int peer_count = 4;

void recv_cb(const esp_now_recv_info_t *recv_info,
             const uint8_t *data,
             int len)
{
    //printf("PACKET RECEIVED LEN: %d\n", len);

    if (len != sizeof(packet_t))
    {
        printf("SIZE WRONG\n");
        return;
    }

    espnow_event_t evt;

    memcpy(&evt.pkt,
           data,
           sizeof(packet_t));

    evt.rssi =
        recv_info->rx_ctrl->rssi;

    xQueueSend(
        espnow_queue,
        &evt,
        0);
}

void espnow_process_task(void *pvParams)
{
    espnow_event_t evt;

    while (1)
    {
        if (xQueueReceive(espnow_queue,
                          &evt,
                          portMAX_DELAY))
        {
            switch (evt.pkt.node_id)
            {
            case 1:
                rssi1[0] = evt.rssi;
                last_seen[0] = esp_timer_get_time();
                break;

            case 2:
                rssi1[1] = evt.rssi;
                last_seen[1] = esp_timer_get_time();
                break;

            case 3:
                rssi1[2] = evt.rssi;
                last_seen[2] = esp_timer_get_time();
                break;

            case 4:
                rssi1[3] = evt.rssi;
                last_seen[3] = esp_timer_get_time();
                break;
            }

            // printf("Node %d RSSI %d\n",
            //        evt.pkt.node_id,
            //        evt.rssi);
        }
    }
}

void init_all()
{
    // NVS
    // ESP_ERROR_CHECK(nvs_flash_init());

    // // TCP/IP
    // ESP_ERROR_CHECK(esp_netif_init());

    // Event loop
    // ESP_ERROR_CHECK(esp_event_loop_create_default());

    // WiFi init
    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    //ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // // STA mode
    // ESP_ERROR_CHECK(
    //     esp_wifi_set_mode(WIFI_MODE_STA));

    // // Start WiFi
    // ESP_ERROR_CHECK(
    //     esp_wifi_start());

    // Fixed channel
    ESP_ERROR_CHECK(
        esp_wifi_set_channel(
            1,
            WIFI_SECOND_CHAN_NONE));

    // Queue FIRST
    espnow_queue =
        xQueueCreate(
            20,
            sizeof(espnow_event_t));

    // NOW init ESP-NOW
    ESP_ERROR_CHECK(
        esp_now_init());

    // Register callback
    ESP_ERROR_CHECK(
        esp_now_register_recv_cb(
            recv_cb));

    // Create processing task
    xTaskCreate(
        espnow_process_task,
        "espnow_process",
        4096,
        NULL,
        5,
        NULL);

    ESP_LOGI("ESPNOW",
             "ESP-NOW Ready");

    // for (int i = 0; i < peer_count; i++)
    // {
    //     memcpy(peer_info.peer_addr, peer_list[i], 6);

    //     esp_err_t add_status = esp_now_add_peer(&peer_info);

    //     if (add_status == ESP_OK)
    //     {
    //         ESP_LOGI("ESP_NOW", "Added Peer %d successfully", i + 1);
    //     }
    //     else if (add_status == ESP_ERR_ESPNOW_EXIST)
    //     {
    //         ESP_LOGW("ESP_NOW", "Peer %d already exists", i + 1);
    //     }
    //     else
    //     {
    //         ESP_LOGE("ESP_NOW", "Failed to add Peer %d: %s", i + 1, esp_err_to_name(add_status));
    //     }
    // }
}

// void send_cb(const wifi_tx_info_t *tx_info, esp_now_send_status_t status)
// {
//     if (status == ESP_NOW_SEND_SUCCESS)
//     {
//         ESP_LOGI("SEND", "Packet sent successfully");
//     }
//     else
//     {
//         ESP_LOGI("SEND", "Packet send failed");
//     }
// }
