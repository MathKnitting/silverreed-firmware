# ESP32-C3 Board

This page documents the standalone **ESP32-C3 board**, a variant of the [Arduino shield](shield.md) where the Arduino UNO is replaced by an ESP32-C3-WROOM-02 module soldered on the board.

The KiCad project lives in `ayab-silver-reed-shield/ayab-silver-reed-esp32/`.

!!! warning "Status: untested design"
    The schematic (v0.5) passes the KiCad electrical rules check, and the PCB (68.6 × 47.3 mm, 2 layers) is placed and routed with no design-rule or connectivity errors. The board is cut away under the module antenna, and the top layer under the module carries only ground. The routing was produced by an autorouter and has not been reviewed by hand. No board has been built or tested.

## What stays the same

The solenoid supply and the current limiter are identical to the shield: buck regulator U1, current-sense amplifier U2, comparator U3, the DC jack, the carriage connector J5 and the test header J7. See [Arduino Shield](shield.md) for how they work.

One value changes: **R7 goes from 30k to 100k**. The solenoid voltage can reach the 15V input, and the ESP32 pins must stay below 3.3V, so `Vsol_div` is now `Vsol / 11` instead of `Vsol / 4`.

## What is new

| Block | Parts | Role |
|-------|-------|------|
| Microcontroller | U4 ESP32-C3-WROOM-02, C8, C9 | Replaces the Arduino. WiFi and Bluetooth LE are available for wireless knitting. |
| Boot and reset | R9, C10, SW1 (reset), R10, SW2 (boot), R11, R12 | Reset circuit and pull-ups on the strapping pins IO2, IO8, IO9. |
| USB-C | J1, R13, R14 | Connected to the native USB port of the ESP32-C3. No USB-serial chip. |
| 5V supply | U5 TPS54202, L2, C11, C12, C13, R15, R16 | Second buck regulator, 15V to 5.07V, always on. Powers the carriage logic. |
| 3.3V supply | U6 AP2112K-3.3, D2, D3, C14, C15 | Powers the ESP32 from the 5V buck or from USB, whichever is present. |
| Input level shifters | U7–U10 74LVC1G17, R17–R20 | Convert the 5V carriage signals to 3.3V. R17–R20 pull the carriage lines up to 5V, as the Arduino internal pull-ups did. |
| Output level shifter | U11 74AHCT1G125, R21 | Converts the 3.3V `DOB` output to 5V. R21 holds `DOB` low while the ESP32 is in reset. |
| Solenoid safety | R22 | Holds the solenoid buck off while the ESP32 is in reset or unprogrammed. |

## Power

- **DC jack (up to 16V)**: powers the solenoids, the carriage 5V and the ESP32. This is enough to knit without a USB cable.
- **USB-C alone**: powers the ESP32 only, for flashing and configuration. The carriage is not powered.

## ESP32-C3 pin mapping

| GPIO | Net | Use |
|------|-----|-----|
| IO0 | CCP_3V3 | Carriage Clock Pulse |
| IO1 | Vsol_div | Solenoid voltage ÷ 11 (ADC) |
| IO2 | — | Strapping pin, pulled up |
| IO3 | EN | Solenoid buck enable (`SOLENOID_POWER`) |
| IO4 | Vsense | Solenoid current, 1 V/A (ADC) |
| IO5 | — | Free |
| IO6 | KSL_3V3 | Point CAM detection |
| IO7 | ND1_3V3 | Needle 1 (not used by the firmware) |
| IO8 | — | Strapping pin, pulled up |
| IO9 | BOOT | Boot button |
| IO10 | DOB_3V3 | Needle selection data |
| IO18 / IO19 | USB_D- / USB_D+ | USB |
| IO20 | HOK_3V3 | Carriage direction |
| IO21 | — | Free (UART0 TX, prints the boot log) |

The pins are assigned so that each signal leaves the module on the side where its destination sits on the PCB.

The firmware selects these pins automatically when built for the ESP32:

```shell
uv run task build -e esp32c3
```
