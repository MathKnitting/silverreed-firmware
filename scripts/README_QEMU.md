# ESP32 QEMU Testing Scripts

This directory contains scripts for testing ESP32 firmware using QEMU emulation.

## Overview

These scripts enable automated testing of ESP32 firmware without physical hardware by using Espressif's QEMU fork, which provides emulation of the ESP32 CPU, memory, and basic peripherals.

## Scripts

### 1. `install_qemu_esp32.sh`

Installs QEMU with ESP32 support.

**Usage:**
```bash
uv run task install-qemu
# Or directly:
bash scripts/install_qemu_esp32.sh
```

**What it does:**
- Detects your operating system (Linux/macOS)
- Installs required dependencies
- Clones Espressif's QEMU fork (esp-develop-9.0.0 branch)
- Compiles QEMU with ESP32/Xtensa support
- Installs to `~/.platformio/qemu-esp32/`

**Requirements:**
- **Linux**: apt or pacman package manager
- **macOS**: Homebrew
- Build tools: git, python3, ninja, meson
- Libraries: glib, pixman, libgcrypt, libslirp

### 2. `create_flash_image.py`

Creates a flash image suitable for QEMU from individual ESP32 binaries.

**Usage:**
```bash
python3 scripts/create_flash_image.py \
    --output flash_image.bin \
    --bootloader .pio/build/esp32qemu/bootloader.bin \
    --partition-table .pio/build/esp32qemu/partitions.bin \
    --app .pio/build/esp32qemu/firmware.bin
```

**What it does:**
- Creates a 4MB flash image (0xFF filled)
- Places binaries at standard ESP32 offsets:
  - Bootloader: 0x1000
  - Partition table: 0x8000
  - Application: 0x10000

**Options:**
- `--output, -o`: Output flash image file (required)
- `--bootloader, -b`: Bootloader binary (required)
- `--partition-table, -p`: Partition table binary (required)
- `--app, -a`: Application binary (required)
- `--flash-size`: Flash size in bytes (default: 4MB)

### 3. `run_esp32_qemu_test.sh`

Wrapper script that integrates with PlatformIO test framework to run tests in QEMU.

**Usage:**
```bash
# Via PlatformIO (recommended)
uv run task test-esp32-qemu

# Or directly
./scripts/run_esp32_qemu_test.sh .pio/build/esp32qemu 30s
```

**What it does:**
1. Verifies QEMU installation
2. Locates firmware binaries in build directory
3. Creates flash image using `create_flash_image.py`
4. Launches QEMU with the flash image
5. Captures test output
6. Times out after specified duration (default: 30s)

**Exit codes:**
- `0`: Tests passed
- `124`: Timeout (often normal for successful tests that don't exit cleanly)
- Other: Test failure

**QEMU command executed:**
```bash
qemu-system-xtensa \
    -nographic \
    -machine esp32 \
    -m 4M \
    -drive file=flash_image.bin,if=mtd,format=raw \
    -serial mon:stdio
```

## Integration with PlatformIO

The scripts are integrated into the PlatformIO test framework via the `esp32qemu` environment in `platformio.ini`:

```ini
[env:esp32qemu]
platform = espressif32
framework = arduino
board = esp32dev
test_framework = unity
test_testing_command =
    ${PROJECT_DIR}/scripts/run_esp32_qemu_test.sh
    ${PROJECT_BUILD_DIR}/${this.__env__}
    30s
```

## Limitations

QEMU ESP32 emulation has some limitations:

1. **Peripheral Emulation**: Not all ESP32 peripherals are fully emulated
   - Basic peripherals (UART, GPIO, timers) work
   - Advanced peripherals (WiFi, Bluetooth, ADC) may have limited or no support

2. **Timing**: QEMU timing may differ from real hardware
   - Some timing-sensitive code may behave differently
   - Tests with strict timing requirements may fail

3. **Hardware-Specific Features**: Features requiring specific hardware (e.g., external sensors, motors) cannot be tested

4. **Performance**: QEMU emulation is slower than native execution but faster than real hardware

## Troubleshooting

### QEMU not found
```
✗ Error: QEMU ESP32 not found at ~/.platformio/qemu-esp32/build/qemu-system-xtensa
```
**Solution**: Run `uv run task install-qemu`

### Build artifacts not found
```
✗ Error: Firmware binary not found: .pio/build/esp32qemu/firmware.bin
```
**Solution**: Build the firmware first: `uv run platformio run -e esp32qemu`

### Test timeout
Tests timeout after 30 seconds by default. If your tests need more time, adjust the timeout in `platformio.ini`:
```ini
test_testing_command =
    ${PROJECT_DIR}/scripts/run_esp32_qemu_test.sh
    ${PROJECT_BUILD_DIR}/${this.__env__}
    60s  # Increase to 60 seconds
```

### Dependencies missing (Linux)
If QEMU fails to build due to missing dependencies, install them manually:
```bash
# Ubuntu/Debian
sudo apt-get install git python3 ninja-build libglib2.0-dev \
    libpixman-1-dev libgcrypt20-dev libslirp-dev pkg-config meson

# Arch Linux
sudo pacman -S git python ninja glib2 pixman libgcrypt libslirp pkg-config meson
```

### Dependencies missing (macOS)
```bash
brew install git python ninja glib pixman libgcrypt libslirp pkg-config meson
```

## References

- [Espressif QEMU Documentation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/qemu.html)
- [QEMU ESP32 GitHub Repository](https://github.com/espressif/qemu)
- [PlatformIO Testing Guide](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)
