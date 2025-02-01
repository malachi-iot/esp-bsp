#include "esp_log.h"
#include "esp_check.h"
#include "esp_memory_utils.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_ssd1681.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_ssd1681_commands.h"

static const char *TAG = "lcd_panel.epaper.aux";

static esp_err_t epaper_set_gate_scan_start(esp_lcd_panel_io_handle_t io, unsigned start)
{
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(
        io, SSD1680_CMD_SET_GATE_SCAN_START, SSD1681_PARAM_OUTPUT_CTRL(start), 2), TAG, "SSD1681_CMD_OUTPUT_CTRL err");

    return ESP_OK;
}