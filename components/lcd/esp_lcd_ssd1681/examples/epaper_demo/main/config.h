#pragma once

#include <sdkconfig.h>

#if CONFIG_BOARD_WAVESHARE
#define EXAMPLE_WIDTH               200
#define EXAMPLE_HEIGHT              200
#define BWIDTH                      200
#define BHEIGHT                     200
#define BITMAP                      BITMAP_200_200
#elif CONFIG_BOARD_CROWPANEL
#define EPD_PANEL_POWER             7
#define EXAMPLE_WIDTH               122
#define EXAMPLE_HEIGHT              250
#define BWIDTH                      64
#define BHEIGHT                     128
#define BITMAP                      BITMAP_64_128
#else
#define EXAMPLE_WIDTH               CONFIG_EPD_WIDTH
#define EXAMPLE_HEIGHT              CONFIG_EPD_HEIGHT
#define BWIDTH                      64
#define BHEIGHT                     128
#define BITMAP                      BITMAP_64_128
#endif
