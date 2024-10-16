/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_err.h"
#include <stdint.h>
#include "lv_color.h"
#include "lv_draw_sw_blend.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------------------- Defines -----------------------------------------------------------

#define DBG_PRINT_OUTPUT false
#define CANARY_BYTES 4

// ------------------------------------------------- Macros and Types --------------------------------------------------

/**
 * @brief Type of blend DUT function
 */
typedef enum {
    OPERATION_FILL,
    OPERATION_FILL_WITH_OPA,
} blend_operation_t;

/**
 * @brief Opacity parameters
 */
typedef struct {
    unsigned int min;                       // Minimum opacity
    unsigned int max;                       // Maximum opacity
    unsigned int step;                      // Opacity step
} opa_matrix_params_t;

/**
 * @brief Functionality test combinations matrix
 */
typedef struct {
    unsigned int min_w;                     // Minimum width of the test array
    unsigned int min_h;                     // Minimum height of the test array
    unsigned int max_w;                     // Maximum width of the test array
    unsigned int max_h;                     // Maximum height of the test array
    unsigned int min_unalign_byte;          // Minimum amount of unaligned bytes of the test array
    unsigned int max_unalign_byte;          // Maximum amount of unaligned bytes of the test array
    unsigned int unalign_step;              // Increment step in bytes unalignment of the test array
    unsigned int dest_stride_step;          // Increment step in destination stride of the test array
    opa_matrix_params_t bg_opa;             // Background opacity parameters
    opa_matrix_params_t fg_opa;             // Foreground opacity parameters
    unsigned int test_combinations_count;   // Count of fest combinations
} test_matrix_params_t;

/**
 * @brief Functionality test case parameters
 */
typedef struct {
    struct {
        void *p_asm;                                        // pointer to the working ASM test buf
        void *p_ansi;                                       // pointer to the working ANSI test buf
        void *p_asm_alloc;                                  // pointer to the beginning of the memory allocated for ASM test buf, used in free()
        void *p_ansi_alloc;                                 // pointer to the beginning of the memory allocated for ANSI test buf, used in free()
    } buf;
    void (*blend_api_func)(_lv_draw_sw_blend_fill_dsc_t *); // pointer to LVGL API function
    lv_color_format_t color_format;                         // LV color format
    size_t data_type_size;                                  // Used data type size, eg sizeof()
    size_t active_buf_len;                                  // Length of buffer, where the actual data are stored (not including Canary bytes)
    size_t total_buf_len;                                   // Total length of buffer (including Canary bytes)
    unsigned int dest_w;                                    // Destination buffer width
    unsigned int dest_h;                                    // Destination buffer height
    unsigned int dest_stride;                               // Destination buffer stride
    unsigned int unalign_byte;                              // Destination buffer memory unalignment
    lv_opa_t bg_opa;                                        // Background opacity
    lv_opa_t fg_opa;                                        // Foreground opacity
    bool static_bg_opa;                                     // Static or dynamic background opacity
    blend_operation_t operation_type;                       // Type of fundamental blend operation
} func_test_case_params_t;

/**
 * @brief Benchmark test case parameters
 */
typedef struct {
    unsigned int height;                                    // Test array height
    unsigned int width;                                     // Test array width
    unsigned int stride;                                    // Test array stride
    unsigned int cc_height;                                 // Corner case test array height
    unsigned int cc_width;                                  // Corner case test array width
    unsigned int benchmark_cycles;                          // Count of benchmark cycles
    void *array_align16;                                    // test array with 16 byte alignment - testing most ideal case
    void *array_align1;                                     // test array with 1 byte alignment - testing wort case
    void (*blend_api_func)(_lv_draw_sw_blend_fill_dsc_t *); // pointer to LVGL API function
    bool dynamic_bg_opa;                                    // Use either static or dynamic background OPA
    lv_opa_t fg_opa;                                        // Foreground opacity
    blend_operation_t operation_type;                       // LVGL operation type
    size_t data_type_size;                                  // Used data type size, eg sizeof()
} bench_test_case_params_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif
