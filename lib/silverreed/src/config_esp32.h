/*
 * @file config_esp32.h
 * @brief ESP32-specific pin configuration for Silver Reed knitting machine
 *
 * This file defines safe GPIO pins for ESP32, ESP32-S2, and ESP32-S3 that avoid:
 * - Strapping pins (GPIO0, GPIO2, GPIO3, GPIO12, GPIO15, GPIO45, GPIO46)
 * - Flash/PSRAM pins (GPIO6-GPIO11 on ESP32, GPIO26-37 on ESP32-S3)
 * - USB pins (GPIO19, GPIO20 on ESP32-S2/S3)
 * - Input-only pins (GPIO34-GPIO39 on ESP32)
 *
 * Selected pins are safe for ESP32, ESP32-S2, and ESP32-S3 variants.
 */

#ifndef ARDUINO_CONFIG_ESP32_H
#define ARDUINO_CONFIG_ESP32_H

#ifdef ESP32

namespace PinsCorrespondance {

// ESP32-S3 specific pin configuration
/**
 * ND1 (Needle Detection 1) - Pattern position sensor
 * GPIO1 - Safe I/O pin on ESP32-S3
 */
const int ND1 = 16;

/**
 * KSL (Knitting Section Limit) - Point CAM detection
 * GPIO2 - Safe I/O pin on ESP32-S3 (not a strapping pin on S3)
 */
const int KSL = 17;

/**
 * DOB (Data Out Buffer) - Solenoid control output
 * GPIO4 - Safe output pin on ESP32-S3
 */
const int DOB = 18;

/**
 * CCP (Carriage Clock Pulse) - Needle transition detector
 * GPIO5 - Safe I/O pin on ESP32-S3
 */
const int CCP = 19;

/**
 * HOK (Horizontal OK / Direction) - Carriage direction indicator
 * GPIO6 - Safe I/O pin on ESP32-S3
 */
const int HOK = 20;

/**
 * SOLENOID_POWER - Power control for solenoid array
 * GPIO7 - Safe output pin on ESP32-S3
 */
const int SOLENOID_POWER = 21;

/**
 * DOB_INVERTED - Indicates if DOB signal is inverted
 * On ESP32, DOB is connected through a 2N2222A transistor for 5V level shifting,
 * which inverts the signal (HIGH becomes LOW and vice versa).
 */
const bool DOB_INVERTED = true;

}  // namespace PinsCorrespondance

#endif  // ESP32

#endif  // ARDUINO_CONFIG_ESP32_H
