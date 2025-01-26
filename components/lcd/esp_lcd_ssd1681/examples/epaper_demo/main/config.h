#pragma once

#include <sdkconfig.h>

#if CONFIG_BOARD_WAVESHARE
#define BWIDTH                      200
#define BHEIGHT                     200
#define BITMAP                      BITMAP_200_200
#elif CONFIG_BOARD_CROWPANEL
#define EPD_PANEL_POWER             7
#define BWIDTH                      64
#define BHEIGHT                     128
#define BITMAP                      BITMAP_64_128
#else
#define BWIDTH                      64
#define BHEIGHT                     128
#define BITMAP                      BITMAP_64_128
#endif
