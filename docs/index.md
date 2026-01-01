# Silverreed Firmware Documentation

Open-source firmware for automating Silver Reed knitting machines using Arduino and the AYAB protocol.

## Overview

A modern, open-source solution for programming patterns on Silver Reed electronic knitting machines.

**Key Features:**
- Arduino UNO / UNO R4 WiFi support
- AYAB Protocol v6 implementation
- Real-time control
- Full hardware and software documentation

## Quick Start

```shell
git clone https://github.com/MathKnitting/silverreed-firmware.git
cd silverreed-firmware
curl -LsSf https://astral.sh/uv/install.sh | sh
uv sync --all-groups
uv run task init
uv run task build && uv run task upload
```

See [Installation Guide](getting-started/installation.md) for details.

## Documentation Sections

**[Getting Started](getting-started/installation.md)**
Installation, building, and uploading firmware

**[Concepts](concepts/purpose.md)**
Project purpose and how knitting machines work

**[Hardware](hardware/shield.md)**
Shield design, pin mapping, and connections

**[Firmware](firmware/architecture.md)**
Architecture, components, and implementation

**[Development](development/setup.md)**
Testing, debugging, and contributing

## Technology used

- **Platform:** Arduino UNO / UNO R4 WiFi
- **Language:** C++11
- **Build:** PlatformIO + uv + taskipy
- **Protocol:** AYAB v6 with SLIP encoding
- **Testing:** Unity + simavr
- **Board Design:** Kicad 6

## Contributing

Contributions welcome! Report issues or submit PRs on [GitHub](https://github.com/MathKnitting/silverreed-firmware).

## License

MIT License - see [LICENSE](reference/license.md)
