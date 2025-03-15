/*
 * @file config.h
 * @brief Main configuration of the arduino device.
 */

#ifndef ARDUINO_CONFIG_H
#define ARDUINO_CONFIG_H

namespace PinsCorrespondance {
const int ND1 = 1;             // Sets the position of the pattern (In)
const int KSL = 2;             // Point CAM detection (In)
const int DOB = 3;             // Data Out Buffer (Out)
const int CCP = 4;             // Carriage Clock Pulse (In)
const int HOK = 5;             // Carriage direction (In)
const int SOLENOID_POWER = 6;  // Pin used to power the solenoid
}  // namespace PinsCorrespondance

#endif  // ARDUINO_CONFIG_H
