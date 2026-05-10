#include "web_server.h"

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "nvs_flash.h"

#include "espnov_rx.h"   // for rssi1[]

static const char *TAG = "WEB_SERVER";

// ─── AP CONFIG ────────────────────────────────────────────────
#define AP_SSID      "ROVER-LIVE"
#define AP_PASSWORD  "rover1234"       // min 8 chars; set "" for open
#define AP_CHANNEL   1
#define AP_MAX_CONN  4

// ─── HTML GENERATOR ───────────────────────────────────────────

static void build_html(char *buf, size_t buf_size)
{
    // colour logic: ≤ -80 red, -80 to -60 yellow, > -60 green, 0 offline
    const char *colours[4];
    const char *labels[4];
    char rssi_str[4][16];

    for (int i = 0; i < 4; i++)
    {
        int r = rssi1[i];
        if (r == 0)
        {
            colours[i]  = "#111111";
            labels[i]   = "OFFLINE";
            snprintf(rssi_str[i], sizeof(rssi_str[i]), "--");
        }
        else if (r > -60)
        {
            colours[i] = "#00e676";   // green
            labels[i]  = "STRONG";
            snprintf(rssi_str[i], sizeof(rssi_str[i]), "%d dBm", r);
        }
        else if (r > -80)
        {
            colours[i] = "#ffea00";   // yellow
            labels[i]  = "MEDIUM";
            snprintf(rssi_str[i], sizeof(rssi_str[i]), "%d dBm", r);
        }
        else
        {
            colours[i] = "#ff1744";   // red
            labels[i]  = "WEAK";
            snprintf(rssi_str[i], sizeof(rssi_str[i]), "%d dBm", r);
        }
    }

    snprintf(buf, buf_size,
        "<!DOCTYPE html><html><head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>Rover Localization</title>"
        "<style>"
        "  @import url('https://fonts.googleapis.com/css2?family=Share+Tech+Mono&family=Orbitron:wght@700&display=swap');"
        "  *{box-sizing:border-box;margin:0;padding:0}"
        "  body{"
        "    background:#0a0a0f;"
        "    color:#e0e0e0;"
        "    font-family:'Share Tech Mono',monospace;"
        "    min-height:100vh;"
        "    display:flex;flex-direction:column;align-items:center;justify-content:center;"
        "    padding:24px;"
        "  }"
        "  h1{"
        "    font-family:'Orbitron',sans-serif;"
        "    font-size:clamp(1.2rem,4vw,2rem);"
        "    letter-spacing:0.15em;"
        "    color:#00e5ff;"
        "    text-shadow:0 0 18px #00e5ff88;"
        "    margin-bottom:8px;"
        "  }"
        "  .subtitle{font-size:0.75rem;color:#555;letter-spacing:0.2em;margin-bottom:36px}"
        "  .grid{"
        "    display:grid;"
        "    grid-template-columns:repeat(2,1fr);"
        "    gap:20px;"
        "    width:100%%;max-width:540px;"
        "  }"
        "  .card{"
        "    background:#12121a;"
        "    border:1px solid #1e1e2e;"
        "    border-radius:12px;"
        "    padding:24px 20px;"
        "    text-align:center;"
        "    position:relative;"
        "    overflow:hidden;"
        "    transition:border-color 0.4s;"
        "  }"
        "  .card::before{"
        "    content:'';"
        "    position:absolute;top:0;left:0;right:0;"
        "    height:3px;"
        "    background:var(--c);"
        "    box-shadow:0 0 12px var(--c);"
        "  }"
        "  .node-label{"
        "    font-family:'Orbitron',sans-serif;"
        "    font-size:0.7rem;"
        "    letter-spacing:0.2em;"
        "    color:#555;"
        "    margin-bottom:12px;"
        "  }"
        "  .rssi-val{"
        "    font-size:1.8rem;"
        "    font-weight:bold;"
        "    color:var(--c);"
        "    text-shadow:0 0 10px var(--c);"
        "    margin-bottom:6px;"
        "  }"
        "  .status{"
        "    font-size:0.65rem;"
        "    letter-spacing:0.25em;"
        "    color:var(--c);"
        "    opacity:0.7;"
        "  }"
        "  .footer{"
        "    margin-top:32px;"
        "    font-size:0.65rem;"
        "    color:#333;"
        "    letter-spacing:0.15em;"
        "  }"
        "  #tick{color:#00e5ff;animation:blink 1s step-end infinite}"
        "  @keyframes blink{50%%{opacity:0}}"
        "</style>"
        // Auto-refresh every 500ms via JS fetch (avoids full page flash)
        "<script>"
        "function refresh(){"
        "  fetch('/data').then(r=>r.json()).then(d=>{"
        "    for(let i=0;i<4;i++){"
        "      const v=d[i];"
        "      const card=document.getElementById('c'+i);"
        "      const rv=document.getElementById('r'+i);"
        "      const st=document.getElementById('s'+i);"
        "      let col,label;"
        "      if(v===0){col='#111111';label='OFFLINE';rv.textContent='--';}"
        "      else if(v>-60){col='#00e676';label='STRONG';rv.textContent=v+' dBm';}"
        "      else if(v>-80){col='#ffea00';label='MEDIUM';rv.textContent=v+' dBm';}"
        "      else{col='#ff1744';label='WEAK';rv.textContent=v+' dBm';}"
        "      card.style.setProperty('--c',col);"
        "      st.textContent=label;"
        "    }"
        "  }).catch(()=>{});"
        "}"
        "setInterval(refresh,500);"   /* 500 ms polling */
        "</script>"
        "</head><body>"
        "<h1>&#x25C6; ROVER NODE MAP</h1>"
        "<div class='subtitle'>ESP-NOW RSSI &bull; LIVE</div>"
        "<div class='grid'>"

        // Node 1
        "<div class='card' id='c0' style='--c:%s'>"
        "  <div class='node-label'>NODE 01</div>"
        "  <div class='rssi-val' id='r0'>%s</div>"
        "  <div class='status' id='s0'>%s</div>"
        "</div>"

        // Node 2
        "<div class='card' id='c1' style='--c:%s'>"
        "  <div class='node-label'>NODE 02</div>"
        "  <div class='rssi-val' id='r1'>%s</div>"
        "  <div class='status' id='s1'>%s</div>"
        "</div>"

        // Node 3
        "<div class='card' id='c2' style='--c:%s'>"
        "  <div class='node-label'>NODE 03</div>"
        "  <div class='rssi-val' id='r2'>%s</div>"
        "  <div class='status' id='s2'>%s</div>"
        "</div>"

        // Node 4
        "<div class='card' id='c3' style='--c:%s'>"
        "  <div class='node-label'>NODE 04</div>"
        "  <div class='rssi-val' id='r3'>%s</div>"
        "  <div class='status' id='s3'>%s</div>"
        "</div>"

        "</div>"
        "<div class='footer'>REFRESH 500ms &nbsp;<span id='tick'>&#x25CF;</span></div>"
        "</body></html>",

        // node 1
        colours[0], rssi_str[0], labels[0],
        // node 2
        colours[1], rssi_str[1], labels[1],
        // node 3
        colours[2], rssi_str[2], labels[2],
        // node 4
        colours[3], rssi_str[3], labels[3]
    );
}

// ─── HTTP HANDLERS ────────────────────────────────────────────

// GET /  →  full HTML page (initial load)
static esp_err_t root_handler(httpd_req_t *req)
{
    static char html_buf[6000];
    build_html(html_buf, sizeof(html_buf));
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_buf, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// GET /data  →  tiny JSON array for JS polling
static esp_err_t data_handler(httpd_req_t *req)
{
    char json[64];
    snprintf(json, sizeof(json),
             "[%d,%d,%d,%d]",
             rssi1[0], rssi1[1], rssi1[2], rssi1[3]);
    httpd_resp_set_type(req, "application/json");
    // Allow browser to cache-bust freely
    httpd_resp_set_hdr(req, "Cache-Control", "no-store");
    httpd_resp_send(req, json, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// ─── SERVER INIT ──────────────────────────────────────────────

void http_server_init(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_uri_t root_uri = {
        .uri      = "/",
        .method   = HTTP_GET,
        .handler  = root_handler,
    };
    httpd_uri_t data_uri = {
        .uri      = "/data",
        .method   = HTTP_GET,
        .handler  = data_handler,
    };

    httpd_register_uri_handler(server, &root_uri);
    httpd_register_uri_handler(server, &data_uri);

    ESP_LOGI(TAG, "HTTP server started — open http://192.168.4.1");
}

// ─── WIFI AP INIT ─────────────────────────────────────────────

void wifi_ap_init(void)
{
    // NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create AP netif
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    wifi_config_t ap_config = {
        .ap = {
            .ssid            = AP_SSID,
            .ssid_len        = strlen(AP_SSID),
            .channel         = AP_CHANNEL,
            .password        = AP_PASSWORD,
            .max_connection  = AP_MAX_CONN,
            .authmode        = WIFI_AUTH_WPA2_PSK,
        },
    };

    // Open network if no password given
    if (strlen(AP_PASSWORD) == 0)
        ap_config.ap.authmode = WIFI_AUTH_OPEN;

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());

            ESP_LOGI(TAG, "AP started — SSID: %s  Pass: %s", AP_SSID, AP_PASSWORD);
    ESP_LOGI(TAG, "Connect and open: http://192.168.4.1");
}