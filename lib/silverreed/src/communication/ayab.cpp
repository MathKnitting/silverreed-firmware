/**
 * @file ayab.cpp
 * @brief Implementation of the Ayab communication protocol.
 * @see
 * https://manual.ayab-knitting.com/1.0/development/API/#generic-configuration
 * for interface details.
 */

#include "ayab.h"

#include <Arduino.h>

Ayab_ &Ayab = Ayab.getInstance();

Ayab_ &Ayab_::getInstance() {
  /**
   * Get the singleton instance of Ayab.
   */
  static Ayab_ instance;
  return instance;
}

void Ayab_::init() {
  /**
   * Initialize the communications for Ayab communication
   */
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(1);

  m_packetSerial.begin(SERIAL_BAUDRATE);
  m_packetSerial.setPacketHandler(Ayab.receive);
};

void Ayab_::receive(const uint8_t *buffer, size_t size) {
  /**
   * Receive a packet from the serial port and process it.
   *
   * This function is called by the PacketSerial library when a full packet
   * has been received.
   *
   * @param buffer The buffer containing the packet.
   * @param size The size of the packet.
   */
  // Ignore empty packets (sliplib in Python emits END bytes at the start of
  // packets)
  if (size <= 0) {
    return;
  }
  switch (buffer[0]) {
    case static_cast<uint8_t>(AYAB_API::reqInfo):
      Ayab.reqInfo(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::reqStart):
      Ayab.reqStart(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::cnfLine):
      Ayab.cnfLine(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::reqTest):
      Ayab.reqTest(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::reqInit):
      Ayab.reqInit(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::helpCmd):
      Ayab.helpCmd(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::sendCmd):
      Ayab.sendCmd(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::beepCmd):
      Ayab.beepCmd(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::autoReadCmd):
      Ayab.readCmd(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::autoTestCmd):
      Ayab.autoCmd(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::quitCmd):
      Ayab.reqStart(buffer, size);
      break;

    case static_cast<uint8_t>(AYAB_API::setAllCmd):
      Ayab.setCmd(buffer, size);
      break;

    default:
      break;
  };
};

void Ayab_::send(const uint8_t *buffer, size_t size) {
  /**
   * Send a packet to the serial port. (Private method)
   *
   * @param buffer The buffer containing the packet.
   * @param size The size of the packet.
   */
  m_packetSerial.send(buffer, size);
};

void Ayab_::update() {
  /**
   * Update the Ayab communication protocol.
   *
   * This function should be called to be able to get latest serial data.
   */
  m_packetSerial.update();
};

void Ayab_::reqInfo(const uint8_t *buffer, size_t size) {
  // Max. length of suffix string: 16 bytes + \0
  // `payload` will be allocated on stack since length is compile-time constant
  uint8_t payload[22];
  payload[0] = static_cast<uint8_t>(AYAB_API::cnfInfo);
  payload[1] = API_VERSION;
  payload[2] = 0;
  payload[3] = 0;
  payload[4] = 0;
  strncpy((char *)payload + 5, "indev", 16);
  send(payload, 22);
};

void Ayab_::reqStart(const uint8_t *buffer, size_t size) {
  if (size < 5U) {
    // Need 5 bytes from buffer below.
    // todo send_cnfStart(ErrorCode::expected_longer_message);
    return;
  }

  uint8_t start_needle = buffer[1];
  uint8_t stop_needle = buffer[2];
  auto continuous_reporting_enabled = static_cast<bool>(buffer[3] & 1);
  auto beeper_enabled = static_cast<bool>(buffer[3] & 2);

  uint8_t crc8 = buffer[4];
  // Check crc on bytes 0-4 of buffer.
  if (crc8 != CRC8(buffer, 4)) {
    // todo send_cnfStart(ErrorCode::checksum_error);
    return;
  }

  // GlobalBeeper::init(beeperEnabled);
  // memset(_b, 0xFF, MAX_LINE_BUFFER_LEN);
  bool ok = KnittingProcess.start_knitting(
      start_needle, stop_needle, continuous_reporting_enabled, beeper_enabled);
  send_cnfStart(ok == false);
}

void Ayab_::send_cnfStart(bool error) {
  uint8_t payload[2];
  payload[0] = static_cast<uint8_t>(AYAB_API::cnfStart);
  payload[1] = static_cast<uint8_t>(error);
  send(payload, 2);
}

void Ayab_::cnfLine(const uint8_t *buffer, size_t size) {
  uint8_t len_line_buffer = 25U;
  if (size < len_line_buffer + 5U) {
    // message is too short
    // TODO(sl): handle error?
    // TODO(TP): send repeat request with error code?
    return;
  }

  uint8_t line_number = buffer[1];
  /* uint8_t color = buffer[2];  */  // currently unused
  uint8_t flags = buffer[3];
  bool flag_last_line = bitRead(flags, 0U);

  uint8_t line_buffer[MAX_LINE_BUFFER_LEN] = {0};

  for (uint8_t i = 0U; i < len_line_buffer; i++) {
    // Values have to be inverted because of needle states
    line_buffer[i] = ~buffer[i + 4];
  }

  uint8_t crc8 = buffer[len_line_buffer + 4];
  // Calculate checksum of buffer contents
  if (crc8 != CRC8(buffer, len_line_buffer + 4)) {
    // TODO(sl): handle checksum error?
    // TODO(TP): send repeat request with error code?
    return;
  }

  KnittingProcess.set_next_line(line_number, flag_last_line, line_buffer);
  return;
}

void Ayab_::sendReqLine(uint8_t line) {
  uint8_t payload[2];
  payload[0] = static_cast<uint8_t>(AYAB_API::reqLine);
  payload[1] = line;
  send(payload, 2);
}

void Ayab_::reqTest(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::reqInit(const uint8_t *buffer, size_t size) {
  uint8_t payload[2];
  payload[0] = static_cast<uint8_t>(AYAB_API::cnfInit);
  if (KnittingProcess.init()) {
    payload[1] = 0;
  } else {
    payload[1] = 1;
  }
  send(payload, 2);
  delay(500);
}
void Ayab_::reqQuit(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::helpCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::sendCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::beepCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::readCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::autoCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::testCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::quitCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}
void Ayab_::setCmd(const uint8_t *buffer, size_t size) {
  // TODO
  return;
}

void Ayab_::sendIndState(CarriageDirection direction) {
  uint16_t left_hall_value = 0;
  uint16_t right_hall_value = 0;
  // `payload` will be allocated on stack since length is compile-time constant

  uint8_t carriage_direction = direction == TO_LEFT ? 0x00 : 0x01;

  uint8_t payload[10] = {
      static_cast<uint8_t>(AYAB_API::indState),
      static_cast<uint8_t>(0x00),  // O for ready, other valure otherwise
      static_cast<uint8_t>(
          0x00),  // todo Finite State Machine state GlobalFsm::getState()
      highByte(left_hall_value),
      lowByte(left_hall_value),
      highByte(right_hall_value),
      lowByte(right_hall_value),
      static_cast<uint8_t>(0x00),  // Only Knit carriage supported for now
      0x00,                        // carriage position (needle number) TODO
      carriage_direction,          // 1 left, 2 right
  };
  send(static_cast<uint8_t *>(payload), 10);
}

uint8_t Ayab_::CRC8(const uint8_t *buffer, size_t len) const {
  uint8_t crc = 0x00U;

  while (len--) {
    uint8_t extract = *buffer;
    buffer++;

    for (uint8_t tempi = 8U; tempi; tempi--) {
      uint8_t sum = (crc ^ extract) & 0x01U;
      crc >>= 1U;

      if (sum) {
        crc ^= 0x8CU;
      }
      extract >>= 1U;
    }
  }
  return crc;
}
