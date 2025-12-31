#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "CSI_APP";

void wifi_csi_rx_cb(void *ctx, wifi_csi_info_t *info)
{
    if (!info || !info->buf) return;

    printf("CSI_LEN=%d | RSSI=%d | DATA=[", info->len, info->rx_ctrl.rssi);

    for (int i = 0; i < info->len; i++) {
        printf("%d ", info->buf[i]);
    }
    printf("]\n");
}

/* WiFi initialization */
void wifi_init()
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    /* Enable CSI */
    wifi_csi_config_t csi_config = {
        .lltf_en = true,
        .htltf_en = true,
        .stbc_htltf2_en = true,
        .ltf_merge_en = true,
        .channel_filter_en = false,
        .manu_scale = false,
        .shift = false,
    };

    esp_wifi_set_csi_config(&csi_config);
    esp_wifi_set_csi_rx_cb(wifi_csi_rx_cb, NULL);
    esp_wifi_set_csi(true);

    ESP_LOGI(TAG, "WiFi CSI Enabled");
}

void app_main()
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();

    wifi_init();
}
