# AYAB Protocol

## Protocol Documentation

For complete protocol specifications, message formats, and API details, refer to the official AYAB documentation:

**[AYAB API Documentation](https://manual.ayab-knitting.com/1.0/development/API/)**

The official documentation covers:
- Message structure and encoding
- Complete command reference (reqStart, cnfLine, indState, etc.)
- Error handling and status codes
- Communication flow and timing requirements
- Integration examples

## Implementation in This Firmware

The AYAB protocol is implemented in the communication layer:

- **[ayab.h](../../lib/silverreed/src/communication/ayab.h)** / **[ayab.cpp](../../lib/silverreed/src/communication/ayab.cpp)** - Protocol implementation
- Uses [PacketSerial](https://github.com/bakercp/PacketSerial) library for SLIP encoding

