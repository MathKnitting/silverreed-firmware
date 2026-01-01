# How Silver Reed Knitting Machines Work

## Introduction to Knitting Machines

Silver Reed knitting machines are domestic machines designed in the 1980s-1990s. They allow knitting pieces much faster than hand knitting.

## Main Components

### The Carriage

The **carriage** is the moving part that travels left to right across the needle bed. It:

- Forms stitches by pulling yarn through loops
- Selects which needles should knit
- Detects its position and direction using sensors

**Key difference from Brother machines:** On Silver Reed machines, the carriage contains all the patterning logic:
- Sensors send information to the "patterning brain" inside the carriage
- Solenoids (electromagnets) act on selected needles to either select or skip them

### The Needle Bed

The bed contains **200 needles** numbered sequentially. Each needle can be in different positions:

- **Position A** (back): the needle knits
- **Position B** (forward): the needle is in hold
- **Position D** (very forward): the needle doesn't knit (pattern)

### The Solenoids

**Solenoids** are electromagnets that control needle selection. The firmware activates/deactivates these solenoids to create the pattern:

- Solenoid **activated**: the needle knits
- Solenoid **deactivated**: the needle skips (doesn't knit)

## Operating Principle

### 1. Carriage Movement

The carriage moves manually from left to right and vice-versa. On each pass:

1. The carriage detects its direction using the **HOK** signal (HIGH when moving from right to left)
2. It detects needle positions using the **CCP** signal (Carriage Clock Pulse) - one CCP period equals one needle
3. For each needle, the **DOB** state (Data Out Buffer) selects or skips the needle

### 2. Needle Selection

For each carriage pass:

```
┌─────────────────────────────────────────────┐
│  Carriage moves →                           │
│                                              │
│  CCP: ┐  ┐  ┐  ┐  ┐  ┐   (1 pulse/needle)  │
│       └──┘  └──┘  └──┘                       │
│                                              │
│  DOB: ──┐     ┐  ┐     (1=knit, 0=skip)     │
│         └─────┘  └─────                      │
│                                              │
│  Result: ■  □  ■  ■  □  (knitted pattern)   │
└─────────────────────────────────────────────┘
```

### 3. Electronic Signals

The machine uses a **7-pin circular DIN connector** to interface with the electronics:

```
DIN-7 Connector Pinout (front view, looking at female socket):

        2
    4       5
  1           3
    6       7
```

The firmware reads and controls several signals:

| DIN Pin | Signal | Type | Direction | Description |
|---------|--------|------|-----------|-------------|
| 1 | **ND1** | Digital | Input | Pattern position (not used) |
| 2 | **KSL** | Digital | Input | Point CAM detection - detects sections |
| 3 | **DOB** | Digital | Output | Data Out Buffer - controls the needle |
| 4 | **CCP** | Digital | Input | Clock Pulse - counts needles |
| 5 | **HOK** | Digital | Input | Carriage direction (LEFT/RIGHT) |
| 6 | **5V** | Power | Supply | Global power supply (5V) |
| 7 | **16V** | Power | Supply | Solenoid power supply (16V) |
| 8 | **GND** | Ground | Common | Common ground |

## Technical Details

### Direction Detection

The **HOK** signal (Hall Sensor) indicates direction:

- `HOK = HIGH`: carriage moving left ←
- `HOK = LOW`: carriage moving right →

The firmware reverses the pattern reading order based on direction.

### Pattern Zones (Sections)

The **KSL** signal detects pattern sections based on **Point CAMs** (magnets) placed on the needle bed:

```
Needle Bed with Point CAMs:
┌────────────────────────────────────────┐
│  [CAM]                        [CAM]   │  Point CAMs (magnets)
│────┬──────────────────────────┬────────│


KSL Signal:
         Pattern Zone (between CAMs)
         ┌──────────────────┐
KSL: ────┘                  └────
    LOW                         LOW
```

- `KSL = HIGH`: Carriage is between the pattern CAMs (knitting pattern zone)
- `KSL = LOW`: Carriage is outside pattern zone

This allows the firmware to know when to activate pattern selection and optimize communication by only transferring data for the active zone.
