# [Silverreed Firmware](./README.md)

<!-- Table of Contents -->

- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Installation

This project leverages:
- Platformio for Arduino project management
- Poetry for dependencies management and tasks definition


1. Clone the repo
   ```shell
   git clone https://github.com/Knitall/silverreed-firmware.git
   ```

2. Install the dependencies
   ```shell
   poetry install
   ```

3. Initialize the project
   ```shell
   poetry run task init --ide {ide}
   ```

## Usage

You can build the project using
```shell
poetry run task build -e {environment}
```

Then upload it to your micro-controller using
```shell
poetry run task start -e {environment}
```

The available environments are:
- `uno`
- `unodebug`
- `native`

## License

Distributed under the MIT License. See [LICENSE](./LICENSE) for more information.
