// ChatGPT generated

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./lib/messages.h"

#include "CUnit/Basic.h"

/* Mock variables for verbose mode */
static bool verbose_mode_set = false;

/* Mock implementations of functions */
void msgSetVerboseMode() {
    verbose_mode_set = true;
}

bool msgIsVerboseModeSet() {
    return verbose_mode_set;
}

/* Test functions */
void test_verbose_mode_set() {
    verbose_mode_set = false;
    msgSetVerboseMode();
    CU_ASSERT_TRUE(msgIsVerboseModeSet());
}

void test_msg_verbose_macro() {
    verbose_mode_set = true;

    /* Capture stderr output */
    FILE *stderr_backup = stderr;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    FILE *mock_stderr = fmemopen(buffer, sizeof(buffer), "w");
    stderr = mock_stderr;

    MSG_VERBOSE("Verbose message test");
    fflush(mock_stderr);

    /* Restore stderr */
    stderr = stderr_backup;
    fclose(mock_stderr);

    /* Assert captured output */
    CU_ASSERT_STRING_EQUAL(buffer, "Verbose message test\n");
}

void test_msg_error_macro() {
    /* Capture stderr output */
    FILE *stderr_backup = stderr;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    FILE *mock_stderr = fmemopen(buffer, sizeof(buffer), "w");
    stderr = mock_stderr;

    MSG_ERROR("Error occurred");
    fflush(mock_stderr);

    /* Restore stderr */
    stderr = stderr_backup;
    fclose(mock_stderr);

    /* Assert captured output */
    CU_ASSERT_STRING_EQUAL(buffer, "dev2fs error: Error occurred\n");
}

/* Test Suite Setup */
int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Test Messages", NULL, NULL);

    CU_add_test(suite, "Test verbose mode set", test_verbose_mode_set);
    CU_add_test(suite, "Test MSG_VERBOSE macro", test_msg_verbose_macro);
    CU_add_test(suite, "Test MSG_ERROR macro", test_msg_error_macro);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
