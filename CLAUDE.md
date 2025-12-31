# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino firmware project for controlling Silver Reed knitting machines. It implements the AYAB (All Yarns Are Beautiful) protocol for automated pattern knitting. The firmware runs on Arduino boards (UNO, UNO R4 WiFi) and uses serial communication with SLIP encoding to control knitting machine solenoids based on pattern data.

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

# Upload to microcontroller
uv run task upload
uv run task upload -e uno

# Run tests
uv run task test
uv run task test -e simavr         # Hardware simulation tests
uv run task test -e native         # Desktop/native tests

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
- `simavr` - AVR simulator for testing
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

### Pin Configuration ([lib/silverreed/src/config.h](lib/silverreed/src/config.h))
Hardware pins are defined in `PinsCorrespondance` namespace:
- ND1 (pin 2): Pattern position
- KSL (pin 3): Point CAM detection
- DOB (pin 4): Data Out Buffer (output to solenoids)
- CCP (pin 5): Carriage Clock Pulse
- HOK (pin 6): Carriage direction
- SOLENOID_POWER (pin 7): Powers the solenoid array

### Singleton Pattern
Both `Ayab` and `KnittingProcess` use the singleton pattern with `getInstance()`. They are accessed via extern references in their respective headers.

## Testing Structure

Tests are organized by platform:

- **[test/test_embedded/](test/test_embedded/)** - Tests that run on Arduino hardware or simavr simulator
  - [test_integration.cpp](test/test_embedded/test_integration.cpp) - Full knitting process integration tests
  - [test_carriage.cpp](test/test_embedded/test_carriage.cpp) - Carriage state detection tests
  - [test_knitting.cpp](test/test_embedded/test_knitting.cpp) - Knitting state machine tests

- **[test/test_desktop/](test/test_desktop/)** - Tests that run on native platform (ignored by embedded builds)
  - Platform-agnostic logic tests

- **[test/test_common/](test/test_common/)** - Shared test utilities and pattern tests (ignored by simavr due to timing issues)

- **[test/test_serial.py](test/test_serial.py)** - Python pytest script for serial communication testing with real hardware

## Code Style

The project enforces Google C++ style via:
- **clang-format** with Google style (pre-commit hook)
- **clang-tidy** with naming conventions:
  - `lower_case` for namespaces, functions, variables
  - `CamelCase` for structs/classes
  - `UPPER_CASE` for global constants
  - `m_` prefix for private members

Pre-commit hooks run automatically on commit. Run manually with `uv run task lint`.

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
