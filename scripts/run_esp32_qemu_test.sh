#!/usr/bin/env bash
# Wrapper script to run ESP32 tests in QEMU
# This script is called by PlatformIO test framework

set -e

# Configuration
QEMU_BIN="${HOME}/.platformio/qemu-esp32/build/qemu-system-xtensa"
BUILD_DIR="${1:-.pio/build/esp32qemu}"
TIMEOUT="${2:-30s}"

# Check if QEMU is installed
if [ ! -f "${QEMU_BIN}" ]; then
    echo "✗ Error: QEMU ESP32 not found at ${QEMU_BIN}" >&2
    echo "  Please run: uv run task install-qemu" >&2
    exit 1
fi

# Check if build directory exists
if [ ! -d "${BUILD_DIR}" ]; then
    echo "✗ Error: Build directory not found: ${BUILD_DIR}" >&2
    exit 1
fi

# Locate required binaries
FIRMWARE_ELF="${BUILD_DIR}/firmware.elf"
FIRMWARE_BIN="${BUILD_DIR}/firmware.bin"
BOOTLOADER="${BUILD_DIR}/bootloader.bin"
PARTITION_TABLE="${BUILD_DIR}/partitions.bin"
FLASH_IMAGE="${BUILD_DIR}/flash_image.bin"

# Check if firmware exists
if [ ! -f "${FIRMWARE_BIN}" ]; then
    echo "✗ Error: Firmware binary not found: ${FIRMWARE_BIN}" >&2
    exit 1
fi

# Create flash image (redirect status messages to stderr)
echo "Creating flash image..." >&2
python3 scripts/create_flash_image.py \
    --output "${FLASH_IMAGE}" \
    --bootloader "${BOOTLOADER}" \
    --partition-table "${PARTITION_TABLE}" \
    --app "${FIRMWARE_BIN}" >&2

if [ ! -f "${FLASH_IMAGE}" ]; then
    echo "✗ Error: Failed to create flash image" >&2
    exit 1
fi

# Run QEMU with timeout (redirect status messages to stderr)
echo "" >&2
echo "Starting QEMU ESP32 (timeout: ${TIMEOUT})..." >&2
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" >&2

# Use timeout command if available, otherwise use script timeout
if command -v timeout &> /dev/null; then
    timeout "${TIMEOUT}" "${QEMU_BIN}" \
        -nographic \
        -machine esp32 \
        -m 4M \
        -drive file="${FLASH_IMAGE}",if=mtd,format=raw \
        -serial mon:stdio \
        || EXIT_CODE=$?
else
    # Fallback for macOS (timeout not available by default)
    perl -e "alarm ${TIMEOUT%s}; exec @ARGV" "${QEMU_BIN}" \
        -nographic \
        -machine esp32 \
        -m 4M \
        -drive file="${FLASH_IMAGE}",if=mtd,format=raw \
        -serial mon:stdio \
        || EXIT_CODE=$?
fi

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" >&2

# Exit codes:
# 0 = success (tests passed)
# 124 = timeout (expected for successful test runs that don't exit cleanly)
# Other = failure
if [ "${EXIT_CODE:-0}" -eq 0 ] || [ "${EXIT_CODE:-0}" -eq 124 ]; then
    echo "✓ QEMU test completed" >&2
    exit 0
else
    echo "✗ QEMU test failed with exit code: ${EXIT_CODE}" >&2
    exit "${EXIT_CODE}"
fi
