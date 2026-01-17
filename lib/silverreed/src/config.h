/*
 * @file config.h
 * @brief Main configuration for Silver Reed knitting machine firmware
 *
 * This file includes platform-specific pin configurations and common settings.
 * Supported platforms: Arduino (AVR, Renesas), ESP32
 */

#ifndef ARDUINO_CONFIG_H
#define ARDUINO_CONFIG_H

#include <stdint.h>

// Include platform-specific pin configurations
// Both platforms are treated equally - the build system defines the target
#ifdef ESP32
#include "config_esp32.h"
#else
#include "config_arduino.h"
#endif

// Solenoid power management
const unsigned long SOLENOID_INACTIVITY_TIMEOUT_MS =
    2000;  // 2 seconds of inactivity before turning off solenoid

// Pattern and needle configuration
const uint8_t DEFAULT_MAX_NEEDLES = 200;  // Default maximum needle count
const int CARRIAGE_OFF_PATTERN = -1;  // Sentinel value: carriage not on pattern

// Bit manipulation constants
const uint8_t BITS_PER_BYTE = 8;
const uint8_t BIT_INDEX_MASK = 0x07;  // Mask for bit position within byte (0-7)

#endif  // ARDUINO_CONFIG_H
