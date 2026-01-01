/**
 * @file version.h
 * @brief Firmware version information.
 *
 * The FIRMWARE_VERSION macro is automatically injected during build time
 * by the get_version.py script, which extracts the version from git tags.
 */

#ifndef VERSION_H_
#define VERSION_H_

#include <stdint.h>

// FIRMWARE_VERSION is defined via build flags from scripts/get_version.py
// Format: "X.Y.Z" from git tags (e.g., "1.3.2")
// Falls back to "indev" if git is unavailable or no tags exist
#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION "indev"
#endif

/**
 * @brief Parse a version string to extract major version number.
 *
 * Parses a semantic version string (e.g., "1.3.2" or "1.3.2-dirty")
 * and extracts the major version number.
 *
 * @param version_str Version string to parse (null-terminated)
 * @return Major version number, or 0 if parsing fails
 */
inline uint8_t parse_version_major(const char* version_str) {
  if (!version_str) {
    return 0;
  }
  // Simple parsing: read until first '.' or end
  uint8_t major = 0;
  for (const char* p = version_str; *p && *p != '.'; ++p) {
    if (*p >= '0' && *p <= '9') {
      major = major * 10 + (*p - '0');
    } else {
      return 0;  // Non-numeric character before first dot
    }
  }
  return major;
}

/**
 * @brief Parse a version string to extract minor version number.
 *
 * @param version_str Version string to parse (null-terminated)
 * @return Minor version number, or 0 if parsing fails
 */
inline uint8_t parse_version_minor(const char* version_str) {
  if (!version_str) {
    return 0;
  }
  // Find first '.'
  const char* p = version_str;
  while (*p && *p != '.') {
    ++p;
  }
  if (*p != '.') {
    return 0;  // No dot found
  }
  ++p;  // Skip the dot

  // Parse minor version
  uint8_t minor = 0;
  while (*p && *p != '.' && *p != '-') {
    if (*p >= '0' && *p <= '9') {
      minor = minor * 10 + (*p - '0');
    } else {
      return 0;  // Non-numeric character
    }
    ++p;
  }
  return minor;
}

/**
 * @brief Parse a version string to extract patch version number.
 *
 * @param version_str Version string to parse (null-terminated)
 * @return Patch version number, or 0 if parsing fails
 */
inline uint8_t parse_version_patch(const char* version_str) {
  if (!version_str) {
    return 0;
  }
  // Find second '.'
  const char* p = version_str;
  int dot_count = 0;
  while (*p && dot_count < 2) {
    if (*p == '.') {
      ++dot_count;
    }
    ++p;
  }
  if (dot_count < 2) {
    return 0;  // Less than two dots found
  }

  // Parse patch version
  uint8_t patch = 0;
  while (*p && *p != '-' && *p != '+') {
    if (*p >= '0' && *p <= '9') {
      patch = patch * 10 + (*p - '0');
    } else {
      return 0;  // Non-numeric character
    }
    ++p;
  }
  return patch;
}

/**
 * @brief Extract the version suffix (metadata after version numbers).
 *
 * Extracts the suffix part of a semantic version string, WITHOUT the separator.
 * For "1.3.2-dirty", returns "dirty" (not "-dirty")
 * For "1.3.2+build", returns "build" (not "+build")
 * For "1.3.2", returns ""
 * For "indev", returns "indev" (no version numbers, so entire string is suffix)
 *
 * @param version_str Version string to parse (null-terminated)
 * @return Pointer to the suffix part (without separator), or empty string if
 * none
 */
inline const char* get_version_suffix(const char* version_str) {
  if (!version_str) {
    return "";
  }

  // Check if this is a semantic version (has at least one dot)
  const char* p = version_str;
  bool has_dot = false;
  while (*p) {
    if (*p == '.') {
      has_dot = true;
      break;
    }
    ++p;
  }

  if (!has_dot) {
    // No dots, so this is not a semantic version (e.g., "indev")
    // Return the entire string as the suffix
    return version_str;
  }

  // Find the end of the patch number (look for '-' or '+' or end of string)
  p = version_str;
  int dot_count = 0;

  // Skip to after the second dot
  while (*p && dot_count < 2) {
    if (*p == '.') {
      ++dot_count;
    }
    ++p;
  }

  if (dot_count < 2) {
    // Malformed version, return empty string
    return "";
  }

  // Skip the patch number
  while (*p && *p >= '0' && *p <= '9') {
    ++p;
  }

  // Now we're at '-', '+', or end of string
  // Skip the separator character and return the rest
  if (*p == '-' || *p == '+') {
    ++p;  // Skip the separator
  }

  return p;  // Return suffix without separator, or empty string
}

#endif  // VERSION_H_
