# Arduino Shield for Silver Reed

This page documents the **AYAB Silver Reed shield v0.4**, the interface board that plugs onto an Arduino UNO and connects it to the carriage of a Silver Reed knitting machine (SK840 and compatible).

The KiCad project lives in [`ayab-silver-reed-shield/`](https://github.com/MathKnitting/silverreed-firmware/tree/main/ayab-silver-reed-shield) (KiCad 10 file format).

!!! note "Source of this page"
    The content below was derived from the netlist of the v0.4 PCB (`ayab-silver-reed-shield.kicad_pcb`) and the notes on the schematic. Component references match the silkscreen.

## Overview

The shield does three things:

1. **Routes the carriage signals** (ND1, KSL, DOB, CCP, HOK) straight to Arduino digital pins.
2. **Generates the solenoid supply** (`Vsol`) from the DC jack with an adjustable buck regulator that the firmware can switch on and off.
3. **Limits the solenoid current** with an analog current-sense and comparator loop, and exposes the measurements on two analog pins.

```
                 +15V                         Vout          Vsol
DC jack J6 ──┬────────► TPS54202 buck U1 ──► L1 ──┬── R3 50mΩ ──┬──► DIN pin 6 (solenoids)
             │             ▲   ▲                  │   │     │   │
        TVS D1, C1, C3     │   └─ FB ◄── R1/RV1/R2│   └─U2──┘   └─ R7/R8 ÷4 ──► A1 (Vsol_div)
             │             │        ▲             │   INA180A1
        JP1 (optional)     EN       │ Q1          │     │ Vsense = 1 V/A
             │             │        └──── U3 ◄────┼─────┴──────────────────► A0 (Vsense)
             ▼             │           TL331      │
        Arduino VIN      D7 (SOLENOID_POWER)   threshold RV2

Arduino 5V  ─────────────────────────────────────────────► DIN pin 7 (carriage logic)
Arduino D2..D6 ◄────────────────────────────────────────► DIN pins 1..5 (signals)
```

Board: 2 layers, 68.6 × 53.3 mm (Arduino UNO outline), SMD parts on the top side, assembled by JLCPCB.

## Connectors

### J5 — Carriage connector (DIN, 7 pins + shield)

Pad numbers are those of the `DIN-704-M10` footprint.

| J5 pad | Net | Direction (seen from the board) | Arduino pin | Description |
|--------|-----|---------------------------------|-------------|-------------|
| 1 | ND1 | Input | D2 | Needle 1 / pattern position (not used by the firmware) |
| 2 | KSL | Input | D3 | Point CAM detection |
| 3 | DOB | Output | D4 | Data Out Buffer, needle selection data |
| 4 | CCP | Input | D5 | Carriage Clock Pulse, one pulse per needle |
| 5 | HOK | Input | D6 | Carriage direction |
| 6 | Vsol | Power out | — | Solenoid supply, after the current-sense resistor |
| 7 | +5V | Power out | 5V | Supply for the carriage sensors and logic |
| 8 (shell) | GND | — | GND | Common ground |

The five signals are wired **directly** to the Arduino pins: there is no series resistor, pull-up, buffer or level shifter on the board. The firmware enables the Arduino internal pull-ups on the four inputs.

### J6 — DC barrel jack

Solenoid power input, net `+15V`. Protected by a TVS diode (D1, SMAJ18A, 18V stand-off), so the input must stay at or below about 16–18V.

### J7 — Test header (1×8 socket)

Mirror of the carriage connector for probing or for wiring a carriage without the DIN plug.

| J7 pin | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|--------|---|---|---|---|---|---|---|---|
| Net | GND | ND1 | KSL | DOB | CCP | HOK | Vsol | +5V |

### J1–J4 — Arduino UNO headers

Bottom-side pin headers that plug into the Arduino. J8–J11 are unpopulated duplicates of the same nets, one row inboard, for stacking or probing.

| Arduino pin | Net | Use |
|-------------|-----|-----|
| D2 | ND1 | Carriage input (unused) |
| D3 | KSL | Carriage input |
| D4 | DOB | Carriage output |
| D5 | CCP | Carriage input |
| D6 | HOK | Carriage input |
| D7 | EN | Buck regulator enable (`SOLENOID_POWER`), HIGH = solenoids powered |
| A0 | Vsense | Solenoid current, 1 V/A |
| A1 | Vsol_div | Solenoid voltage ÷ 4 |
| 5V | +5V | Carriage logic supply and comparator supply |
| 3V3 | +3V3 | Current-sense amplifier supply and threshold reference |
| VIN | VIN | Connected to `+15V` when JP1 is closed |
| GND | GND | Common ground |

All other Arduino pins (D0/D1 serial, D8–D13, A2–A5, AREF, IOREF, RESET) only pass through to the duplicate headers.

### JP1 — Arduino power jumper

Closing JP1 connects the DC jack (`+15V`) to the Arduino `VIN` pin so the Arduino can run from the jack without USB. With JP1 open the Arduino is powered from USB only.

## Power: solenoid supply

### Input protection and filtering

| Ref | Value | Role |
|-----|-------|------|
| D1 | SMAJ18A | TVS diode across the input |
| C3 | 47µF electrolytic | Bulk input capacitor |
| C1 | 10µF | Ceramic input capacitor of the buck regulator |

### Buck regulator (U1, TPS54202)

A synchronous 2A buck converter turns `+15V` into `Vout`.

| Ref | Value | Role |
|-----|-------|------|
| U1 | TPS54202 | Buck regulator, 0.596V feedback reference |
| L1 | 10µH | Output inductor |
| C4 | 100nF | Bootstrap capacitor (BOOT–SW) |
| C5 | 47µF, 25V | Output capacitor |
| R1 | 100k | Feedback divider, top |
| RV1 + R2 | 10k trimmer + 4.7k | Feedback divider, bottom |

**Output voltage** is set with trimmer RV1:

```
Vout = 0.596 × (RV1 + R2 + R1) / (RV1 + R2)

Vout_min = 0.596 × (10 + 4.7 + 100) / (10 + 4.7) = 4.65 V
Vout_max = 0.596 × ( 0 + 4.7 + 100) / ( 0 + 4.7) = 13.28 V
```

**Enable.** The `EN` pin of U1 is driven by Arduino D7. The pin has an internal pull-up current source and a 1.21V rising threshold, so:

- D7 HIGH → regulator on, solenoids powered.
- D7 LOW → regulator off.
- D7 floating (Arduino in reset, unprogrammed or absent) → regulator **on**.

The firmware drives D7 LOW at start-up and after 2 seconds of carriage inactivity (`SOLENOID_INACTIVITY_TIMEOUT_MS`) so the solenoids do not overheat.

## Current limiter

### Current measurement (U2, INA180A1)

`Vout` reaches the solenoids through R3, a 50mΩ shunt resistor. U2 amplifies the voltage across R3 by 20:

```
Vsense = 20 × 0.05 Ω × I = 1 V per ampere
```

U2 is powered from the 3.3V rail (decoupled by C2), so `Vsense` saturates around 3.3V. `Vsense` goes to Arduino A0 and to the comparator.

### Comparator with hysteresis (U3, TL331)

U3 compares `Vsense` (inverting input) against a threshold set with trimmer RV2 (non-inverting input).

| Ref | Value | Role |
|-----|-------|------|
| R4 + RV2 | 220k + 50k trimmer | Threshold divider from the 3.3V rail |
| R5 | 100k | Positive feedback, creates the hysteresis |
| R6 | 10k | Pull-up of the open-collector output to 3.3V |
| C7 | 100nF | Decoupling of U3 (5V supply) |
| Q1 | BSS138 | N-MOSFET between the buck `FB` node and GND |
| C6 | 100pF | Not populated (removed from the board, tests showed it is not needed) |

The comparator is powered from 5V for input headroom, while its threshold and pull-up use the 3.3V rail, which is assumed more precise than the 5V rail.

The thresholds depend on the RV2 setting. Values computed in `threshold_calculator.ods`:

| RV2 position | Upper threshold | Lower threshold |
|--------------|-----------------|-----------------|
| Minimum | 0 A | 0 A |
| 20 % | 0.38 A | 0.12 A |
| 40 % | 0.69 A | 0.22 A |
| 60 % | 0.94 A | 0.31 A |
| 80 % | 1.15 A | 0.39 A |
| Maximum | 1.34 A | 0.46 A |

### How the loop behaves

Reading the netlist, the comparator output drives the gate of Q1, and Q1 shorts the buck feedback node to ground when it conducts:

- **Current below the lower threshold** → comparator output high → Q1 on → `FB` held at 0V → the regulator pushes `Vout` as high as it can, close to the input voltage.
- **Current above the upper threshold** → comparator output low → Q1 off → the regulator returns to the voltage set by RV1.

The solenoids therefore receive the full input voltage while they draw little current, and the reduced RV1 voltage once the total current passes the limit. With an inductive load this keeps the average current between the two thresholds.

!!! note
    This description is deduced from the PCB connections, not from a written specification by the board designer. Confirm it with an oscilloscope on `Vsol` and `Vsense` before relying on it.

### Voltage measurement

R7 (30k) and R8 (10k) divide `Vsol` by 4 and feed Arduino A1 (`Vsol_div`). At the maximum regulated voltage, 13.28V gives 3.32V on A1.

The current firmware does not read A0 or A1.

## Bill of materials

Assembled parts for v0.4 (JLCPCB/LCSC part numbers):

| Ref | Value | Package | LCSC |
|-----|-------|---------|------|
| C1 | 10µF | 0805 | C15850 |
| C2, C7 | 100nF | 0603 | C14663 |
| C3 | 47µF electrolytic | 5×5.9mm | C4747958 |
| C4 | 100nF | 0805 | C49678 |
| C5 | 47µF 25V | 1210 | C7432791 |
| D1 | SMAJ18A | SMA | C908771 |
| J5 | DIN-704-M10 | Through-hole | C20611695 |
| J6 | DC barrel jack | Through-hole | C720557 |
| JP1 | 1×2 pin header | Through-hole | C5383111 |
| L1 | 10µH | FXL0518 | C475910 |
| Q1 | BSS138 | SOT-23 | C7420339 |
| R1, R5 | 100k | 0603 | C25803 |
| R2 | 4.7k | 0603 | C23162 |
| R3 | 50mΩ | 1206 | C5159739 |
| R4 | 220k | 0603 | C22961 |
| R6, R8 | 10k | 0603 | C25804 |
| R7 | 30k | 0603 | C22984 |
| RV1 | 10k trimmer | Bourns TC33X | C719176 |
| RV2 | 50k trimmer | Bourns TC33X | C913246 |
| U1 | TPS54202 | SOT-23-6 | C191884 |
| U2 | INA180A1 | SOT-23-5 | C122228 |
| U3 | TL331 | SOT-23-5 | C3658524 |

Hand-soldered: Arduino headers J1–J4 and test socket J7.

## Power Supply

The shield uses two supplies:

1. **5V logic**: provided by the Arduino (USB, or the DC jack through JP1 and the Arduino's own regulator). It powers the carriage sensors through DIN pin 7.
2. **Solenoid power**: an external DC supply on the barrel jack J6, 16V DC maximum.

## Design Files

| File | Content |
|------|---------|
| `ayab-silver-reed-shield.kicad_sch` | Schematic |
| `ayab-silver-reed-shield.kicad_pcb` | PCB layout |
| `ayab-silver-reed-shield.kicad_dru` | JLCPCB design rules |
| `symbols.kicad_sym`, `footprints/` | Project-local symbols and footprints (DIN connector) |
| `production/` | Gerbers, BOM and placement files as ordered (v0.3, v0.4) |
| `bom/` | Interactive HTML BOM |
| `datasheets/` | TPS54202 and trimmer datasheets |
| `threshold_calculator.ods` | Comparator threshold calculation |
