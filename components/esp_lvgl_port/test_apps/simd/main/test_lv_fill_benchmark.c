/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <malloc.h>
#include <sdkconfig.h>

#include "unity.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"  // for xthal_get_ccount()
#include "lv_fill_common.h"
#include "lv_draw_sw_blend.h"
#include "lv_draw_sw_blend_to_argb8888.h"
#include "lv_draw_sw_blend_to_rgb565.h"

#define WIDTH 128
#define HEIGHT 128
#define STRIDE WIDTH
#define UNALIGN_BYTES 1
#define BENCHMARK_CYCLES 750

// ------------------------------------------------- Macros and Types --------------------------------------------------

static const char *TAG_LV_FILL_BENCH = "Benchmark test LV fill";
static const char *asm_ansi_func[] = {"ASM", "ANSI"};

// Colors used for testing
static lv_color_t fg_color = { .blue = 0x56, .green = 0x34, .red = 0x12, };
static lv_color_t bg_color = { .blue = 0xEF, .green = 0xCD, .red = 0xAB, };

// ------------------------------------------------ Static function headers --------------------------------------------

/**
 * @brief Initialize the benchmark test
 */
static void lv_fill_benchmark_init(bench_test_case_params_t *test_params);

/**
 * @brief Run the benchmark test
 */
static float lv_fill_benchmark_run(bench_test_case_params_t *test_params, _lv_draw_sw_blend_fill_dsc_t *dsc);

/**
 * @brief Reinitialize destination array for the next benchmark test
 */
static void reinit_dest_array(bench_test_case_params_t *test_params, _lv_draw_sw_blend_fill_dsc_t *dsc);


// ------------------------------------------------ Test cases ---------------------------------------------------------

/*
Benchmark tests

Requires:
    - To pass functionality tests first

Purpose:
    - Test that an acceleration is achieved by an assembly implementation of LVGL blending API

Procedure:
    - Initialize input parameters (test array length, width, allocate array...) of the benchmark test
    - Run assembly version of LVGL blending API multiple times (1000-times or so)
    - Firstly use an input test parameters for the most common case (16-byte aligned array, array width and height divisible by 4 for ARGB8888 color format)
    - Then use corner-case input test parameters (1-byte aligned array, array width and height NOT divisible by 4 for ARGB8888 color format)
    - Count how many CPU cycles does it take to run a function from the LVGL blending API for each case (common and corner case)
    - Run ansi version of LVGL blending API multiple times (1000-times or so) and repeat the 2 above steps for the ansi version
    - Free test arrays and structures needed for LVGL blending API
*/
// ------------------------------------------------ Test cases stages --------------------------------------------------

TEST_CASE("Benchmark test LV fill for ARGB8888 color format", "[fill][benchmark][ARGB8888]")
{
    uint32_t *dest_array_align16  = (uint32_t *)memalign(16, STRIDE * HEIGHT * sizeof(uint32_t) + UNALIGN_BYTES);
    TEST_ASSERT_NOT_EQUAL(NULL, dest_array_align16);

    // Apply byte unalignment for the corner-case test scenario
    uint32_t *dest_array_align1 = dest_array_align16 + UNALIGN_BYTES;

    bench_test_case_params_t test_params = {
        .height = HEIGHT,
        .width = WIDTH,
        .stride = STRIDE * sizeof(uint32_t),
        .cc_height = HEIGHT - 1,
        .cc_width = WIDTH - 1,
        .benchmark_cycles = BENCHMARK_CYCLES,
        .array_align16 = (void *)dest_array_align16,
        .array_align1 = (void *)dest_array_align1,
        .blend_api_func = &lv_draw_sw_blend_color_to_argb8888,
        .fg_opa = LV_OPA_MAX,                   // Set maximum opacity, to call LV_DRAW_SW_COLOR_BLEND_TO_ARGB8888
        .operation_type = OPERATION_FILL,
        .data_type_size = sizeof(uint32_t),
    };

    ESP_LOGI(TAG_LV_FILL_BENCH, "running for ARGB8888 color format");
    lv_fill_benchmark_init(&test_params);
    free(dest_array_align16);
}

TEST_CASE("Benchmark test LV fill with OPA for ARGB8888 color format", "[fill][opa][benchmark][ARGB8888]")
{
    uint32_t *dest_array_align16  = (uint32_t *)memalign(16, STRIDE * HEIGHT * sizeof(uint32_t) + UNALIGN_BYTES);
    TEST_ASSERT_NOT_EQUAL(NULL, dest_array_align16);

    // Apply byte unalignment for the corner-case test scenario
    uint32_t *dest_array_align1 = (uint32_t*)((uint8_t *)dest_array_align16 + UNALIGN_BYTES);

    bench_test_case_params_t test_params = {
        .height = HEIGHT,
        .width = WIDTH,
        .stride = STRIDE * sizeof(uint32_t),
        .cc_height = HEIGHT - 1,
        .cc_width = WIDTH - 1,
        .benchmark_cycles = BENCHMARK_CYCLES,
        .array_align16 = (void *)dest_array_align16,
        .array_align1 = (void *)dest_array_align1,
        .blend_api_func = &lv_draw_sw_blend_color_to_argb8888,
        .fg_opa = LV_OPA_10,
        .operation_type = OPERATION_FILL_WITH_OPA,
        .data_type_size = sizeof(uint32_t),
    };

    // Set destination array to 0
    memset((void *)dest_array_align16, 0, (STRIDE * HEIGHT * sizeof(uint32_t)) + UNALIGN_BYTES);

    ESP_LOGI(TAG_LV_FILL_BENCH, "running for for ARGB8888 color format");
    lv_fill_benchmark_init(&test_params);
    free(dest_array_align16);
}

TEST_CASE("Benchmark test LV fill for RGB565 color format", "[fill][benchmark][RGB565]")
{
    uint16_t *dest_array_align16  = (uint16_t *)memalign(16, STRIDE * HEIGHT * sizeof(uint16_t) + UNALIGN_BYTES);
    TEST_ASSERT_NOT_EQUAL(NULL, dest_array_align16);

    // Apply byte unalignment for the corner-case test scenario
    uint16_t *dest_array_align1 = dest_array_align16 + UNALIGN_BYTES;

    bench_test_case_params_t test_params = {
        .height = HEIGHT,
        .width = WIDTH,
        .stride = STRIDE * sizeof(uint16_t),
        .cc_height = HEIGHT - 1,
        .cc_width = WIDTH - 1,
        .benchmark_cycles = BENCHMARK_CYCLES,
        .array_align16 = (void *)dest_array_align16,
        .array_align1 = (void *)dest_array_align1,
        .blend_api_func = &lv_draw_sw_blend_color_to_rgb565,
        .fg_opa = LV_OPA_MAX,                   // Set maximum opacity, to call LV_DRAW_SW_COLOR_BLEND_TO_RGB565
        .operation_type = OPERATION_FILL,
        .data_type_size = sizeof(uint16_t),
    };

    ESP_LOGI(TAG_LV_FILL_BENCH, "running for RGB565 color format");
    lv_fill_benchmark_init(&test_params);
    free(dest_array_align16);
}
// ------------------------------------------------ Static test functions ----------------------------------------------

static void lv_fill_benchmark_init(bench_test_case_params_t *test_params)
{
    // Init structure for LVGL blend API, to call the Assembly API
    _lv_draw_sw_blend_fill_dsc_t dsc = {
        .dest_buf = test_params->array_align16,
        .dest_w = test_params->width,
        .dest_h = test_params->height,
        .dest_stride = test_params->stride,  // stride * sizeof()
        .mask_buf = NULL,
        .color = fg_color,
        .opa = test_params->fg_opa,
        .use_asm = true,
    };

    // Init structure for LVGL blend API, to call the ANSI API
    _lv_draw_sw_blend_fill_dsc_t dsc_cc = dsc;
    dsc_cc.dest_buf = test_params->array_align1;
    dsc_cc.dest_w = test_params->cc_width;
    dsc_cc.dest_h = test_params->cc_height;

    // Run benchmark 2 times:
    // First run using assembly, second run using ANSI
    for (int i = 0; i < 2; i++) {

        // Run benchmark with the most common input parameters
        // Use static BG OPA
        if (test_params->operation_type == OPERATION_FILL_WITH_OPA) {
            test_params->dynamic_bg_opa = false;
        }
        float avg_cpu_cycles = lv_fill_benchmark_run(test_params, &dsc);        // Call Benchmark loop
        float cpu_cycles_per_sample = avg_cpu_cycles / ((float)(dsc.dest_w * dsc.dest_h));
        ESP_LOGI(TAG_LV_FILL_BENCH, " %s common case: %.3f avg. CPU cycles for %"PRIi32"x%"PRIi32" matrix, %.3f CPU cycles per sample",
                 asm_ansi_func[i], avg_cpu_cycles, dsc.dest_w, dsc.dest_h, cpu_cycles_per_sample);

        // Run benchmark with the corner case input parameters
        // Use dynamic BG OPA
        if (test_params->operation_type == OPERATION_FILL_WITH_OPA) {
            test_params->dynamic_bg_opa = true;
        }
        avg_cpu_cycles = lv_fill_benchmark_run(test_params, &dsc_cc);           // Call Benchmark loop
        cpu_cycles_per_sample = avg_cpu_cycles / ((float)(dsc_cc.dest_w * dsc_cc.dest_h));
        ESP_LOGI(TAG_LV_FILL_BENCH, " %s corner case: %.3f avg. CPU cycles for %"PRIi32"x%"PRIi32" matrix, %.3f CPU cycles per sample\n",
                 asm_ansi_func[i], avg_cpu_cycles, dsc_cc.dest_w, dsc_cc.dest_h, cpu_cycles_per_sample);

        // change to ANSI
        dsc.use_asm = false;
        dsc_cc.use_asm = false;
    }
}

static void reinit_dest_array(bench_test_case_params_t *test_params, _lv_draw_sw_blend_fill_dsc_t *dsc)
{
    switch (test_params->operation_type) {
    case OPERATION_FILL:
        // No need to refill dest_array for Simple fill test
        break;
    case OPERATION_FILL_WITH_OPA:
        // We need to re initialize dest_array, since the previous run of the benchmark test has modified the dest_array
        // For dynamic background opacity, we fill each sample with different opacity to induce the corner case - being
        // most demanding for processing power
        if (test_params->dynamic_bg_opa) {
            lv_color32_t bg_color_argb8888 = lv_color_to_32(bg_color, LV_OPA_50);
            for (int i = 0; i < dsc->dest_w * dsc->dest_h; i++) {
                // Dynamic BG OPA
                bg_color_argb8888.alpha = (uint8_t)(i % 255);
                ((lv_color32_t *)dsc->dest_buf)[i] = bg_color_argb8888;
            }
        } else {
            // For static background, we set each sample with constant opacity level
            memset(dsc->dest_buf, 0, dsc->dest_w * dsc->dest_h * test_params->data_type_size);
            //memset(dsc->dest_buf, 0, dsc->dest_w * dsc->dest_h);
        }
        break;
    default:
        break;
    }
}

static float lv_fill_benchmark_run(bench_test_case_params_t *test_params, _lv_draw_sw_blend_fill_dsc_t *dsc)
{
    // Call the DUT function for the first time to init the benchmark test
    test_params->blend_api_func(dsc);

    uint64_t total_cpu_count = 0;
    for (int i = 0; i < test_params->benchmark_cycles; i++) {

        // Re-initialize destination array before each test run
        reinit_dest_array(test_params, dsc);

        // We must count cycles, only during the DUT function execution
        const unsigned int start_cpu_count = xthal_get_ccount();                    // Start counting CPU cycles
        test_params->blend_api_func(dsc);                                           // run DUT function
        const unsigned int end_cpu_count = xthal_get_ccount();                      // Stop counting CPU cycles
        total_cpu_count += (uint64_t)(end_cpu_count - start_cpu_count);             // Count total CPU cycles
    }
    return ((float)total_cpu_count) / ((float)test_params->benchmark_cycles);
}
