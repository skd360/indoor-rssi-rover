
#ifndef ESPNOW_RX_H
#define ESPNOW_RX_H
#include "freertos/FreeRTOS.h"
#include <stdint.h>
#include "esp_now.h"
#include "freertos/task.h"

#include "freertos/queue.h"
typedef struct __attribute__((packed))
{
    uint16_t seq_num;
    uint8_t node_id;
    uint32_t timestamp;
} packet_t;

typedef struct
{
    packet_t pkt;
    int rssi;
} espnow_event_t;

// Global RSSI storage
extern volatile int rssi1[4];
extern volatile int64_t last_seen[4];
// Queue handle
extern QueueHandle_t espnow_queue;

// Functions
void init_all(void);

void recv_cb(const esp_now_recv_info_t *recv_info,
             const uint8_t *data,
             int len);

void espnow_process_task(void *pvParams);

#endif

