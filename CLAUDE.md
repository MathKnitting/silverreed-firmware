# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino firmware project for controlling Silver Reed knitting machines. It implements the AYAB (All Yarns Are Beautiful) protocol for automated pattern knitting. The firmware runs on multiple platforms including Arduino boards (UNO, UNO R4 WiFi) and ESP32 boards (ESP32 DevKit), and uses serial communication with SLIP encoding to control knitting machine solenoids based on pattern data.

## Build System & Commands

The project uses **PlatformIO** for Arduino development, **uv** for Python dependency management, and **taskipy** for task automation.

### Initial Setup
```shell
# Install uv (if not already installed)
curl -LsSf https://astral.sh/uv/install.sh | sh

# Install dependencies
uv sync --all-groups

# Initialize project (install pre-commit hooks and PlatformIO)
uv run task init
```

### Common Development Commands
```shell
# Build firmware
uv run task build

# Build for specific environment
uv run task build -e uno
uv run task build -e unodebug      # UNO with debug flags
uv run task build -e uno_r4_wifi   # Arduino UNO R4 WiFi
uv run task build -e esp32dev      # ESP32 DevKit

# Upload to microcontroller
uv run task upload
uv run task upload -e uno

# Run tests
uv run task test
uv run task test -e simavr         # AVR/Arduino hardware simulation tests
uv run task test -e esp32qemu      # ESP32 QEMU simulation tests
uv run task test -e native         # Desktop/native tests

# Install QEMU for ESP32 testing (one-time setup)
uv run task install-qemu

# Lint code (runs pre-commit hooks)
uv run task lint

# Clean build artifacts
uv run task clean

# Documentation
uv run task docs-serve             # Serve docs locally with hot-reload
uv run task docs-build             # Build static docs site
uv run task docs-deploy            # Deploy to GitHub Pages

# Serial monitor
uv run task monitor

# Debug
uv run task debug

# See all available tasks
uv run task --list
```

### Available Environments
- `uno` - Arduino UNO (default)
- `unodebug` - Arduino UNO with DEBUG flag enabled
- `uno_r4_wifi` - Arduino UNO R4 WiFi
- `esp32dev` - ESP32 DevKit (generic ESP32, also compatible with ESP32-S2)
- `simavr` - AVR simulator for testing (Arduino/AVR only)
- `esp32qemu` - ESP32 QEMU simulator for testing (requires QEMU installation)
- `native` - Native platform for desktop tests

### Direct PlatformIO Commands
```shell
# If you need more control, use platformio directly with uv
uv run platformio run -e uno                     # Build specific environment
uv run platformio test -e simavr                 # Run tests on simulator
uv run platformio device monitor                 # Open serial monitor
```

## Code Architecture

### Core Components

The firmware is organized into three main layers:

1. **Main Entry Point** ([src/main.cpp](src/main.cpp))
   - Minimal Arduino setup/loop structure
   - Initializes two singleton instances: `Ayab` (communication) and `KnittingProcess` (state machine)
   - Configures hardware pins for carriage detection and solenoid control

2. **Communication Layer** ([lib/silverreed/src/communication/](lib/silverreed/src/communication/))
   - [ayab.h](lib/silverreed/src/communication/ayab.h)/[ayab.cpp](lib/silverreed/src/communication/ayab.cpp) - AYAB protocol implementation using PacketSerial (SLIP encoding)
   - Handles bidirectional serial communication at 115200 baud
   - API version 6 protocol with commands like reqStart, cnfLine, indState, reqTest
   - CRC8 checksum validation

3. **Knitting State Machine** ([lib/silverreed/src/knitting.h](lib/silverreed/src/knitting.h)/[knitting.cpp](lib/silverreed/src/knitting.cpp))
   - `KnittingProcess_` singleton manages knitting state (Idle, WaitingStart, Knitting)
   - Coordinates between pattern data and carriage position
   - Tracks current row, stitch position, and needle range (start_needle to end_needle)
   - Main loop: `knitting_loop()` called continuously from Arduino loop

4. **Machine Hardware Interface** ([lib/silverreed/src/machine/carriage.h](lib/silverreed/src/machine/carriage.h)/[carriage.cpp](lib/silverreed/src/machine/carriage.cpp))
   - `Carriage` class controls solenoid power and DOB (Data Out Buffer) state
   - `CarriageState` reads hardware pins (CCP, KSL, HOK) to detect:
     - Carriage direction (TO_LEFT, TO_RIGHT)
     - Pattern section boundaries
     - Needle transitions

5. **Pattern Management** ([lib/silverreed/src/pattern.h](lib/silverreed/src/pattern.h)/[pattern.cpp](lib/silverreed/src/pattern.cpp))
   - Stores and manages knitting pattern data
   - Line-based pattern data structure (up to MAX_LINE_BUFFER_LEN = 25 bytes per line)

### Pin Configuration

Hardware pins are defined in `PinsCorrespondance` namespace in platform-specific files. The appropriate configuration is automatically included based on the build target.

**Configuration Files:**
- [lib/silverreed/src/config.h](lib/silverreed/src/config.h) - Main configuration file that includes the platform-specific config
- [lib/silverreed/src/config_arduino.h](lib/silverreed/src/config_arduino.h) - Arduino (AVR/Renesas) pin configuration
- [lib/silverreed/src/config_esp32.h](lib/silverreed/src/config_esp32.h) - ESP32 pin configuration

**Arduino (AVR/Renesas)** pins:
- ND1 (pin 2), KSL (pin 3), DOB (pin 4), CCP (pin 5), HOK (pin 6), SOLENOID_POWER (pin 7)

**ESP32** pins:
- ND1 (GPIO 21), KSL (GPIO 22), DOB (GPIO 23), CCP (GPIO 18), HOK (GPIO 5), SOLENOID_POWER (GPIO 17)
- These pins avoid strapping pins, flash pins, and other problematic GPIOs for good compatibility with ESP32 variants

### Singleton Pattern
Both `Ayab` and `KnittingProcess` use the singleton pattern with `getInstance()`. They are accessed via extern references in their respective headers.

## Testing Structure

Tests are organized by platform:

- **[test/test_embedded/](test/test_embedded/)** - Tests that run on Arduino hardware or simulators (simavr/esp32qemu)
  - [test_integration.cpp](test/test_embedded/test_integration.cpp) - Full knitting process integration tests
  - [test_carriage.cpp](test/test_embedded/test_carriage.cpp) - Carriage state detection tests
  - [test_knitting.cpp](test/test_embedded/test_knitting.cpp) - Knitting state machine tests
  - [test_ayab.cpp](test/test_embedded/test_ayab.cpp) - AYAB protocol and communication tests
  - [test_version.cpp](test/test_embedded/test_version.cpp) - Firmware version parsing tests

- **[test/test_desktop/](test/test_desktop/)** - Tests that run on native platform (ignored by embedded builds)
  - Platform-agnostic logic tests

- **[test/test_common/](test/test_common/)** - Shared test utilities and pattern tests (ignored by simavr due to timing issues)

### QEMU ESP32 Testing

The project supports ESP32 testing via QEMU emulation:

1. **Installation** (one-time setup):
   ```shell
   uv run task install-qemu
   ```
   This downloads and compiles Espressif's QEMU fork with ESP32 support to `~/.platformio/qemu-esp32/`.

2. **Running tests**:
   ```shell
   uv run task test-esp32-qemu
   # Or directly with PlatformIO
   uv run platformio test -e esp32qemu
   ```

3. **How it works**:
   - Builds firmware for ESP32 target
   - Creates a flash image combining bootloader, partition table, and application binary
   - Launches QEMU with the flash image
   - Tests run in the emulated environment with 30s timeout

**Note**: QEMU ESP32 emulation has limitations compared to real hardware. Some peripherals may not be fully emulated.

## Code Style

The project enforces Google C++ style via:
- **clang-format** with Google style (pre-commit hook)
- **clang-tidy** with naming conventions:
  - `lower_case` for namespaces, functions, variables
  - `CamelCase` for structs/classes
  - `UPPER_CASE` for global constants
  - `m_` prefix for private members

Pre-commit hooks run automatically on commit. Run manually with `uv run task lint`.

## Firmware Versioning

The firmware version is automatically managed through git tags and injected at build time:

### How It Works
1. **Git tags** define the version (e.g., `v1.3.2`)
2. **[scripts/get_version.py](scripts/get_version.py)** extracts the version using `git describe --tags`
3. **PlatformIO** injects it as a `FIRMWARE_VERSION` macro during compilation
4. **[lib/silverreed/src/version.h](lib/silverreed/src/version.h)** provides:
   - The `FIRMWARE_VERSION` macro (string format)
   - Parsing functions to extract major, minor, and patch numbers
5. **AYAB protocol** uses this version in the `cnfInfo` response:
   - Bytes 2-4: Parsed version numbers (major, minor, patch)
   - Bytes 5-21: Full version string including suffixes like "-dirty"

### Version Format
- **Released builds**: `1.3.2` (from git tag `v1.3.2`)
- **Development builds**: `1.3.2-dirty` (uncommitted changes) or `1.3.2-1-g<hash>` (commits after tag)
- **Fallback**: `indev` (if git is unavailable)

### Creating a New Release
To release a new version:
```shell
git tag v1.4.0
git push origin v1.4.0
```

The version will be automatically extracted and embedded in all subsequent builds.

## Debugging

Enable debug output by building with the `unodebug` environment:
```shell
uv run task build -e unodebug
uv run task upload -e unodebug
```

Debug macros are defined in [lib/silverreed/src/debug.h](lib/silverreed/src/debug.h):
- `DEBUG_START()` - Initialize debug serial
- `DEBUG_PRINTLN(msg)` - Print debug message
- Only active when `DEBUG` flag is set
