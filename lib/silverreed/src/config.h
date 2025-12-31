/*
 * @file config.h
 * @brief Main configuration of the arduino device.
 */

#ifndef ARDUINO_CONFIG_H
#define ARDUINO_CONFIG_H

#include <stdint.h>

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
const int ND1 = 2;

/**
 * KSL (Knitting Section Limit) - Point CAM detection
 * Detects when carriage is between the two point CAMs (magnets on bed).
 * HIGH when carriage is in the pattern section, LOW when outside.
 */
const int KSL = 3;

/**
 * DOB (Data Out Buffer) - Solenoid control output
 * Controls which needles should be selected by the solenoid array.
 * HIGH selects the needle, LOW deselects it.
 */
const int DOB = 4;

/**
 * CCP (Carriage Clock Pulse) - Needle transition detector
 * Generates a pulse (LOW to HIGH transition) each time the carriage
 * moves to a new needle position. Used to synchronize pattern data output.
 */
const int CCP = 5;

/**
 * HOK (Horizontal OK / Direction) - Carriage direction indicator
 * HIGH when carriage moves to the left, LOW when moving to the right.
 */
const int HOK = 6;

/**
 * SOLENOID_POWER - Power control for solenoid array
 * Controls power supply to the solenoid array. Turned off during inactivity
 * to prevent overheating and reduce power consumption.
 */
const int SOLENOID_POWER = 7;
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
