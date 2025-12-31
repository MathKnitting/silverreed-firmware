# [Silverreed Firmware](./README.md)

<!-- Table of Contents -->

- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Installation

This project leverages:
- PlatformIO for Arduino project management
- uv for Python dependency management
- taskipy for task automation

### Prerequisites

- Python 3.12 or higher
- [uv](https://docs.astral.sh/uv/) - Fast Python package installer

### Setup

1. Clone the repo
   ```shell
   git clone https://github.com/Knitall/silverreed-firmware.git
   cd silverreed-firmware
   ```

2. Install uv (if not already installed)
   ```shell
   curl -LsSf https://astral.sh/uv/install.sh | sh
   ```

3. Install dependencies
   ```shell
   uv sync --all-groups
   ```

4. Initialize the project
   ```shell
   uv run task init
   ```

## Usage

### Common Commands

```shell
# Build the firmware
uv run task build

# Build for specific environment
uv run task build -e uno
uv run task build -e unodebug    # With debug flags
uv run task build -e uno_r4_wifi # For Arduino UNO R4

# Upload to microcontroller
uv run task upload
uv run task upload -e uno

# Run tests
uv run task test
uv run task test -e simavr       # Run on AVR simulator
uv run task test -e native       # Run on native platform

# Run linter
uv run task lint

# Open serial monitor
uv run task monitor

# Documentation
uv run task docs-serve            # Serve locally with hot-reload
uv run task docs-build            # Build static site
uv run task docs-deploy           # Deploy to GitHub Pages

# Clean build artifacts
uv run task clean

# See all available tasks
uv run task --list
```

### Available Environments

- `uno` - Arduino UNO (default)
- `unodebug` - Arduino UNO with debug flags
- `uno_r4_wifi` - Arduino UNO R4 WiFi
- `simavr` - AVR simulator for testing
- `native` - Native platform for desktop tests

## License

Distributed under the MIT License. See [LICENSE](./LICENSE) for more information.
