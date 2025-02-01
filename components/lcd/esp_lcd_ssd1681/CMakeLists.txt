idf_component_register(SRCS ssd1680.c "esp_lcd_panel_ssd1681.c"
                       INCLUDE_DIRS "include"
                       REQUIRES "esp_lcd"
                       PRIV_REQUIRES "driver")
