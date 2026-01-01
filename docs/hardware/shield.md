# Arduino Shield for Silver Reed

This page describes the shield (interface board) that connects the Arduino to the Silver Reed knitting machine.

## Overview

The **Arduino shield** is an electronic board that plugs onto an Arduino UNO and provides:

1. **Connections** to the knitting machine signals
2. **Protection** of Solenoid by controling the 16V power


## Assembly

## External Connections

### To the Machine

The machine connector has **6 wires**:

| Pin | Signal | Suggested Color | Description |
|-----|--------|-----------------|-------------|
| 1 | GND | Black | Common ground |
| 2 | CCP | Yellow | Clock Pulse |
| 3 | HOK | Green | Direction |
| 4 | KSL | Blue | Point CAM |
| 5 | DOB | Orange | Data Out |
| 6 | ND1 | Purple | Not used |

### Power Supply

The shield requires **two power supplies**:

1. **5V logic**: Provided by Arduino (via USB or jack)
2. **16V power**: External supply for solenoids
   - Voltage: 16V DC

## Shield Testing

## Design Files
