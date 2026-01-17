#!/usr/bin/env bash
# Script to install QEMU with ESP32 support
# Based on: https://alexconesa.wordpress.com/2025/01/17/speeding-up-esp32-embedded-system-testing-with-qemu-on-ubuntu/

set -e

QEMU_DIR="${HOME}/.platformio/qemu-esp32"
QEMU_BIN="${QEMU_DIR}/build/qemu-system-xtensa"

echo "Installing QEMU with ESP32 support..."

# Check if already installed
if [ -f "${QEMU_BIN}" ]; then
    echo "QEMU ESP32 already installed at ${QEMU_BIN}"
    exit 0
fi

# Detect OS
OS="$(uname -s)"
case "${OS}" in
    Linux*)
        echo "Detected Linux"
        # Install required dependencies
        if command -v apt-get &> /dev/null; then
            echo "Installing dependencies with apt..."
            sudo apt-get update
            sudo apt-get install -y \
                git python3 ninja-build \
                libglib2.0-dev libpixman-1-dev \
                libgcrypt20-dev libslirp-dev \
                libgnutls28-dev \
                pkg-config meson
        elif command -v pacman &> /dev/null; then
            echo "Installing dependencies with pacman..."
            sudo pacman -S --needed --noconfirm \
                git python ninja \
                glib2 pixman \
                libgcrypt libslirp gnutls \
                pkg-config meson
        else
            echo "Warning: Unknown package manager. Please install dependencies manually:"
            echo "  git python3 ninja-build libglib2.0-dev libpixman-1-dev"
            echo "  libgcrypt20-dev libslirp-dev libgnutls28-dev pkg-config meson"
        fi
        ;;
    Darwin*)
        echo "Detected macOS"
        if ! command -v brew &> /dev/null; then
            echo "Error: Homebrew not found. Please install it from https://brew.sh"
            exit 1
        fi
        echo "Installing dependencies with brew..."
        brew install git python ninja glib pixman libgcrypt libslirp gnutls pkg-config meson
        ;;
    *)
        echo "Error: Unsupported OS: ${OS}"
        exit 1
        ;;
esac

# Clone QEMU ESP32 fork
echo "Cloning Espressif QEMU fork..."
mkdir -p "$(dirname "${QEMU_DIR}")"
git clone --depth 1 --branch esp-develop \
    https://github.com/espressif/qemu.git "${QEMU_DIR}"

# Build QEMU
echo "Building QEMU with ESP32 support..."
cd "${QEMU_DIR}"

# Configure with ESP32 target
# Minimal configuration for ESP32 emulation only
./configure \
    --target-list=xtensa-softmmu \
    --disable-werror \
    --disable-gnutls \
    --disable-gtk \
    --disable-sdl \
    --disable-vnc \
    --disable-curses

# Build
ninja -C build

# Verify installation
if [ -f "${QEMU_BIN}" ]; then
    echo ""
    echo "✓ QEMU ESP32 installed successfully!"
    echo "  Binary: ${QEMU_BIN}"
    echo "  Version: $(${QEMU_BIN} --version | head -n1)"
else
    echo "✗ Installation failed: Binary not found at ${QEMU_BIN}"
    exit 1
fi
