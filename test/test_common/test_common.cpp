#define RUN_MODULE(run_function) \
  extern void run_function();    \
  run_function();

#include <unity.h>

#include "test_pattern.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void RUN_UNITY_TESTS() {
  UNITY_BEGIN();
  RUN_MODULE(run_module_pattern_tests);
  UNITY_END();
}

int main(int argc, char** argv) {
  RUN_UNITY_TESTS();
  return 0;
}
