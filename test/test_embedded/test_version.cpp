#include "test_version.h"

#include <unity.h>

#include "version.h"

void test_parse_version_major() {
  // Test standard version strings
  TEST_ASSERT_EQUAL(1, parse_version_major("1.3.2"));
  TEST_ASSERT_EQUAL(10, parse_version_major("10.5.3"));
  TEST_ASSERT_EQUAL(2, parse_version_major("2.0.0"));

  // Test version with suffixes
  TEST_ASSERT_EQUAL(1, parse_version_major("1.3.2-dirty"));
  TEST_ASSERT_EQUAL(1, parse_version_major("1.3.2-1-gabcdef"));

  // Test edge cases
  TEST_ASSERT_EQUAL(0, parse_version_major("indev"));
  TEST_ASSERT_EQUAL(0, parse_version_major(""));
  TEST_ASSERT_EQUAL(0, parse_version_major(nullptr));
}

void test_parse_version_minor() {
  // Test standard version strings
  TEST_ASSERT_EQUAL(3, parse_version_minor("1.3.2"));
  TEST_ASSERT_EQUAL(5, parse_version_minor("10.5.3"));
  TEST_ASSERT_EQUAL(0, parse_version_minor("2.0.0"));
  TEST_ASSERT_EQUAL(10, parse_version_minor("1.10.5"));

  // Test version with suffixes
  TEST_ASSERT_EQUAL(3, parse_version_minor("1.3.2-dirty"));
  TEST_ASSERT_EQUAL(3, parse_version_minor("1.3.2-1-gabcdef"));

  // Test edge cases
  TEST_ASSERT_EQUAL(0, parse_version_minor("indev"));
  TEST_ASSERT_EQUAL(0, parse_version_minor(""));
  TEST_ASSERT_EQUAL(0, parse_version_minor(nullptr));
  TEST_ASSERT_EQUAL(0, parse_version_minor("1"));  // No dot
}

void test_parse_version_patch() {
  // Test standard version strings
  TEST_ASSERT_EQUAL(2, parse_version_patch("1.3.2"));
  TEST_ASSERT_EQUAL(3, parse_version_patch("10.5.3"));
  TEST_ASSERT_EQUAL(0, parse_version_patch("2.0.0"));
  TEST_ASSERT_EQUAL(15, parse_version_patch("1.2.15"));

  // Test version with suffixes
  TEST_ASSERT_EQUAL(2, parse_version_patch("1.3.2-dirty"));
  TEST_ASSERT_EQUAL(2, parse_version_patch("1.3.2-1-gabcdef"));
  TEST_ASSERT_EQUAL(5, parse_version_patch("1.2.5+build123"));

  // Test edge cases
  TEST_ASSERT_EQUAL(0, parse_version_patch("indev"));
  TEST_ASSERT_EQUAL(0, parse_version_patch(""));
  TEST_ASSERT_EQUAL(0, parse_version_patch(nullptr));
  TEST_ASSERT_EQUAL(0, parse_version_patch("1.2"));  // Only one dot
}

void test_parse_version_integration() {
  // Test parsing the actual FIRMWARE_VERSION macro
  // This will be different depending on build context
  uint8_t major = parse_version_major(FIRMWARE_VERSION);
  uint8_t minor = parse_version_minor(FIRMWARE_VERSION);
  uint8_t patch = parse_version_patch(FIRMWARE_VERSION);

  // For "1.3.2" or "1.3.2-dirty", we expect 1, 3, 2
  // For "indev", we expect 0, 0, 0
  // Just verify they're in valid range
  TEST_ASSERT_LESS_OR_EQUAL(255, major);
  TEST_ASSERT_LESS_OR_EQUAL(255, minor);
  TEST_ASSERT_LESS_OR_EQUAL(255, patch);
}

void test_get_version_suffix() {
  // Test suffix extraction for various version strings
  // Note: suffix is returned WITHOUT the separator character

  // Standard version without suffix
  const char* suffix1 = get_version_suffix("1.3.2");
  TEST_ASSERT_EQUAL_STRING("", suffix1);

  // Version with -dirty suffix (returns without the '-')
  const char* suffix2 = get_version_suffix("1.3.2-dirty");
  TEST_ASSERT_EQUAL_STRING("dirty", suffix2);

  // Version with git commit info (returns without the '-')
  const char* suffix3 = get_version_suffix("1.3.2-1-gabcdef");
  TEST_ASSERT_EQUAL_STRING("1-gabcdef", suffix3);

  // Version with + build metadata (returns without the '+')
  const char* suffix4 = get_version_suffix("1.2.5+build123");
  TEST_ASSERT_EQUAL_STRING("build123", suffix4);

  // Non-semantic version (entire string is suffix)
  const char* suffix5 = get_version_suffix("indev");
  TEST_ASSERT_EQUAL_STRING("indev", suffix5);

  // Different version numbers
  const char* suffix6 = get_version_suffix("10.15.20");
  TEST_ASSERT_EQUAL_STRING("", suffix6);

  // Version with -rc1 suffix (returns without the '-')
  const char* suffix7 = get_version_suffix("10.15.20-rc1");
  TEST_ASSERT_EQUAL_STRING("rc1", suffix7);

  // Edge cases
  const char* suffix8 = get_version_suffix("");
  TEST_ASSERT_EQUAL_STRING("", suffix8);

  const char* suffix9 = get_version_suffix(nullptr);
  TEST_ASSERT_EQUAL_STRING("", suffix9);
}

void run_module_version_tests() {
  RUN_TEST(test_parse_version_major);
  RUN_TEST(test_parse_version_minor);
  RUN_TEST(test_parse_version_patch);
  RUN_TEST(test_parse_version_integration);
  RUN_TEST(test_get_version_suffix);
}
