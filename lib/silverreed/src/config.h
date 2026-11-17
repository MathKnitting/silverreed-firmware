/*
 * @file config.h
 * @brief Main configuration of the arduino device.
 */

#ifndef ARDUINO_CONFIG_H
#define ARDUINO_CONFIG_H

#include <stdint.h>

/**
 * Select a pin number depending on the target board.
 * First value: Arduino UNO shield. Second value: ESP32-C3 board
 * (ayab-silver-reed-shield/ayab-silver-reed-esp32).
 */
#if defined(ARDUINO_ARCH_ESP32)
#define SILVERREED_PIN(arduino_pin, esp32_pin) (esp32_pin)
#else
#define SILVERREED_PIN(arduino_pin, esp32_pin) (arduino_pin)
#endif

/**
 * @namespace PinsCorrespondance
 * @brief Hardware pin assignments for Silver Reed knitting machine interface
 *
 * The Silver Reed knitting machine uses several signal pins to communicate
 * with the Arduino controller. These pins detect carriage position, movement,
 * and control the solenoid array.
 */
namespace PinsCorrespondance {
/**
 * ND1 (Needle Detection 1) - Pattern position sensor
 * Currently not used in this implementation.
 */
const int ND1 = SILVERREED_PIN(2, 7);

/**
 * KSL (Knitting Section Limit) - Point CAM detection
 * Detects when carriage is between the two point CAMs (magnets on bed).
 * HIGH when carriage is in the pattern section, LOW when outside.
 */
const int KSL = SILVERREED_PIN(3, 6);

/**
 * DOB (Data Out Buffer) - Solenoid control output
 * Controls which needles should be selected by the solenoid array.
 * HIGH selects the needle, LOW deselects it.
 */
const int DOB = SILVERREED_PIN(4, 10);

/**
 * CCP (Carriage Clock Pulse) - Needle transition detector
 * Generates a pulse (LOW to HIGH transition) each time the carriage
 * moves to a new needle position. Used to synchronize pattern data output.
 */
const int CCP = SILVERREED_PIN(5, 0);

/**
 * HOK (Horizontal OK / Direction) - Carriage direction indicator
 * HIGH when carriage moves to the left, LOW when moving to the right.
 */
const int HOK = SILVERREED_PIN(6, 20);

/**
 * SOLENOID_POWER - Power control for solenoid array
 * Controls power supply to the solenoid array. Turned off during inactivity
 * to prevent overheating and reduce power consumption.
 */
const int SOLENOID_POWER = SILVERREED_PIN(7, 3);
}  // namespace PinsCorrespondance

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
