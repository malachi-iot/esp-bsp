/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include "unity.h"
#include "test_functionality_eval.h"

void test_eval_32bit_data(func_test_case_params_t *test_case, const char *test_msg_buf)
{
    // Print results 32bit data
#if DBG_PRINT_OUTPUT
    for (uint32_t i = 0; i < test_case->total_buf_len; i++) {
        printf("dest_buf[%"PRIi32"] %s ansi = %8"PRIx32" \t asm = %8"PRIx32" \n", i, ((i < 10) ? (" ") : ("")), ((uint32_t *)test_case->buf.p_ansi)[i], ((uint32_t *)test_case->buf.p_asm)[i]);
    }
    printf("\n");
#endif

    // Canary bytes area must stay 0
    TEST_ASSERT_EACH_EQUAL_UINT32_MESSAGE(0, (uint32_t *)test_case->buf.p_ansi, CANARY_BYTES, test_msg_buf);
    TEST_ASSERT_EACH_EQUAL_UINT32_MESSAGE(0, (uint32_t *)test_case->buf.p_asm, CANARY_BYTES, test_msg_buf);

    // dest_buf_asm and dest_buf_ansi must be equal
    TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE((uint32_t *)test_case->buf.p_asm + CANARY_BYTES, (uint32_t *)test_case->buf.p_ansi + CANARY_BYTES, test_case->active_buf_len, test_msg_buf);

    // Canary bytes area must stay 0
    TEST_ASSERT_EACH_EQUAL_UINT32_MESSAGE(0, (uint32_t *)test_case->buf.p_ansi + (test_case->total_buf_len - CANARY_BYTES), CANARY_BYTES, test_msg_buf);
    TEST_ASSERT_EACH_EQUAL_UINT32_MESSAGE(0, (uint32_t *)test_case->buf.p_asm + (test_case->total_buf_len - CANARY_BYTES), CANARY_BYTES, test_msg_buf);
}

void test_eval_16bit_data(func_test_case_params_t *test_case, const char *test_msg_buf)
{
    // Print results, 16bit data
#if DBG_PRINT_OUTPUT
    for (uint32_t i = 0; i < test_case->total_buf_len; i++) {
        printf("dest_buf[%"PRIi32"] %s ansi = %8"PRIx16" \t asm = %8"PRIx16" \n", i, ((i < 10) ? (" ") : ("")), ((uint16_t *)test_case->buf.p_ansi)[i], ((uint16_t *)test_case->buf.p_asm)[i]);
    }
    printf("\n");
#endif

    // Canary bytes area must stay 0
    TEST_ASSERT_EACH_EQUAL_UINT16_MESSAGE(0, (uint16_t *)test_case->buf.p_ansi, CANARY_BYTES, test_msg_buf);
    TEST_ASSERT_EACH_EQUAL_UINT16_MESSAGE(0, (uint16_t *)test_case->buf.p_asm, CANARY_BYTES, test_msg_buf);

    // dest_buf_asm and dest_buf_ansi must be equal
    TEST_ASSERT_EQUAL_UINT16_ARRAY_MESSAGE((uint16_t *)test_case->buf.p_asm + CANARY_BYTES, (uint16_t *)test_case->buf.p_ansi + CANARY_BYTES, test_case->active_buf_len, test_msg_buf);

    // Canary bytes area must stay 0
    TEST_ASSERT_EACH_EQUAL_UINT16_MESSAGE(0, (uint16_t *)test_case->buf.p_ansi + (test_case->total_buf_len - CANARY_BYTES), CANARY_BYTES, test_msg_buf);
    TEST_ASSERT_EACH_EQUAL_UINT16_MESSAGE(0, (uint16_t *)test_case->buf.p_asm + (test_case->total_buf_len - CANARY_BYTES), CANARY_BYTES, test_msg_buf);
}
