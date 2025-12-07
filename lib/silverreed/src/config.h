/*
 * @file config.h
 * @brief Main configuration of the arduino device.
 */

#ifndef ARDUINO_CONFIG_H
#define ARDUINO_CONFIG_H

namespace PinsCorrespondance {
const int led_builtin = 13;
const int ND1 = 2;             // Sets the position of the pattern (In)
const int KSL = 3;             // Point CAM detection (In)
const int DOB = 4;             // Data Out Buffer (Out)
const int CCP = 5;             // Carriage Clock Pulse (In)
const int HOK = 6;             // Carriage direction (In)
const int SOLENOID_POWER = 7;  // Pin used to power the solenoid
}  // namespace PinsCorrespondance

#endif  // ARDUINO_CONFIG_H
