# Firmware Architecture

## Overview

The Silverreed firmware is designed with a layered architecture that clearly separates responsibilities:

```
┌─────────────────────────────────────────────┐
│         User Application                    │
│         (AYAB Desktop, etc.)                │
└─────────────────┬───────────────────────────┘
                  │ Serial + SLIP
┌─────────────────▼───────────────────────────┐
│      Communication Layer (AYAB)             │
│      - AYAB protocol v6                     │
│      - SLIP encoding                        │
│      - CRC8 validation                      │
└─────────────────┬───────────────────────────┘
                  │ Internal API
┌─────────────────▼───────────────────────────┐
│   State Machine (KnittingProcess)          │
│   - Idle / WaitingStart / Knitting         │
│   - Pattern management                      │
│   - Knitting coordination                   │
└─────────────────┬───────────────────────────┘
                  │ Commands
┌─────────────────▼───────────────────────────┐
│    Hardware Interface (Carriage)           │
│    - Sensor reading                        │
│    - Solenoid control                      │
│    - Position detection                    │
└─────────────────────────────────────────────┘
```

## Data Flow

### Starting a Session

```
1. User: Starts knitting in AYAB Desktop
2. AYAB Desktop → Arduino: reqStart(start=50, end=150, ...)
3. Arduino → AYAB Desktop: cnfStart(success=true)
4. Arduino → AYAB Desktop: reqLine(line=0)
5. AYAB Desktop → Arduino: cnfLine(line=0, data=[...])
6. Arduino: Waiting for carriage...
```

### Knitting a Line

```
1. Carriage starts: CarriageState detects movement
2. For each needle (CCP interrupt):
   - Read Pattern.get_pixel(needle)
   - Carriage.set_dob(pixel_value)
   - Wait for next CCP
3. Line complete: reqLine(line+1)
4. Receive new line: cnfLine(...)
5. Repeat for next line
```
