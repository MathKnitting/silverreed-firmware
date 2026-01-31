/*
 * @file config_arduino.h
 * @brief Arduino-specific pin configuration for Silver Reed knitting machine
 *
 * This file defines GPIO pins for Arduino AVR and Renesas boards (UNO, UNO R4 WiFi).
 */

#ifndef ARDUINO_CONFIG_ARDUINO_H
#define ARDUINO_CONFIG_ARDUINO_H

#if !defined(ESP32)

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

/**
 * DOB_INVERTED - Indicates if DOB signal is inverted
 * On Arduino, DOB is directly connected, so no inversion is needed.
 */
const bool DOB_INVERTED = false;

}  // namespace PinsCorrespondance

#endif  // !ESP32

#endif  // ARDUINO_CONFIG_ARDUINO_H
