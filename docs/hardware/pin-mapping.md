# Arduino Pin Configuration

This page describes in detail the correspondence between Arduino pins and Silver Reed knitting machine signals.

## Overview

The firmware uses **7 pins** to interface the Arduino with the knitting machine:

```
Arduino UNO
┌────────────────┐
│                │
│  D2  ●────────────── ND1 (not used)
│  D3  ●────────────── KSL (Point CAM)
│  D4  ●────────────── DOB → Solenoid Drivers
│  D5  ●────────────── CCP (Interrupt)
│  D6  ●────────────── HOK (Direction)
│  D7  ●────────────── SOLENOID_POWER → Relay
│                │
│  GND ●────────────── Common ground
│  5V  ●────────────── Logic power
│                │
└────────────────┘

Connectors to Silver Reed machine:
  - ND1, KSL, CCP, HOK: Sensor reading (3.3V/5V)
  - DOB: Solenoid control (logic output)
  - SOLENOID_POWER: Power command (via Relay)
```


## Code Configuration

All pin configuration is centralized in [lib/silverreed/src/config.h](../../lib/silverreed/src/config.h):

## Board Compatibility

### Arduino UNO



