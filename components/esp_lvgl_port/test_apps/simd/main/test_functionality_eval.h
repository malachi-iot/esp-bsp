
/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "lv_fill_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Evaluate results of functionality test for 32bit data length
 *
 * @param[in] test_case Pointer ot structure defining functionality test case
 */
void test_eval_32bit_data(func_test_case_params_t *test_case, const char *test_msg_buff);

/**
 * @brief Evaluate results of functionality test for 16bit data length
 *
 * @param[in] test_case Pointer ot structure defining functionality test case
 */
void test_eval_16bit_data(func_test_case_params_t *test_case, const char *test_msg_buff);

#ifdef __cplusplus
} /*extern "C"*/
#endif
