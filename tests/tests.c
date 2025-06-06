#define CRITERION_MAIN

#include <criterion/criterion.h>

#include "../includes/config.h"

int main() {
    struct criterion_test_set* tests = criterion_initialize();

    criterion_options.jobs = 1; // Disable parallel jobs
    int result = criterion_run_all_tests(tests);

    criterion_finalize(tests);
    return result ? SUCCESS : FAIL;
}