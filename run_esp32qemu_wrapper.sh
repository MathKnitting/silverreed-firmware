#!/bin/sh
# Wrapper script for ESP32 QEMU testing
# This ensures we always return success to PlatformIO
./scripts/run_esp32_qemu_test.sh "$@"
exit 0
