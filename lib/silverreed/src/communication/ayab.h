#ifndef AYAB_H_
#define AYAB_H_

#include <PacketSerial.h>
#include <stdint.h>

#include "com.h"
#include "machine/carriage.h"

using namespace std;

constexpr uint8_t API_VERSION = 6U;

constexpr uint32_t SERIAL_BAUDRATE = 115200U;

constexpr uint8_t MAX_LINE_BUFFER_LEN = 25U;
constexpr uint8_t MAX_MSG_BUFFER_LEN = 64U;

enum class AYAB_API : unsigned char {
  reqStart = 0x01,
  cnfStart = 0xC1,
  reqLine = 0x82,
  cnfLine = 0x42,
  reqInfo = 0x03,
  cnfInfo = 0xC3,
  reqTest = 0x04,
  cnfTest = 0xC4,
  indState = 0x84,
  helpCmd = 0x25,
  sendCmd = 0x26,
  beepCmd = 0x27,
  setSingleCmd = 0x28,
  setAllCmd = 0x29,
  readEOLsensorsCmd = 0x2A,
  readEncodersCmd = 0x2B,
  autoReadCmd = 0x2C,
  autoTestCmd = 0x2D,
  stopCmd = 0x2E,
  quitCmd = 0x2F,
  reqInit = 0x05,
  cnfInit = 0xC5,
  testRes = 0xEE,
  debug = 0x9F
};
using AYAB_API_t = enum AYAB_API;

#pragma once

class Ayab_ : public Communicator {
 public:
  static Ayab_& getInstance();

  Ayab_(const Ayab_&) = delete;
  Ayab_& operator=(const Ayab_&) = delete;

  void init();
  static void receive(const uint8_t* buffer, size_t size);
  void send(const uint8_t* buffer, size_t size);
  void update();

  void sendIndState(CarriageDirection direction);
  void sendReqLine(uint8_t line);

 private:
  Ayab_() = default;

  PacketSerial_<SLIP, SLIP::END, MAX_MSG_BUFFER_LEN> m_packetSerial;

  // Different calls
  void reqInfo(const uint8_t* buffer, size_t size);
  void reqStart(const uint8_t* buffer, size_t size);
  void cnfLine(const uint8_t* buffer, size_t size);
  void reqTest(const uint8_t* buffer, size_t size);
  void reqInit(const uint8_t* buffer, size_t size);
  void reqQuit(const uint8_t* buffer, size_t size);
  void helpCmd(const uint8_t* buffer, size_t size);
  void sendCmd(const uint8_t* buffer, size_t size);
  void beepCmd(const uint8_t* buffer, size_t size);
  void readCmd(const uint8_t* buffer, size_t size);
  void autoCmd(const uint8_t* buffer, size_t size);
  void testCmd(const uint8_t* buffer, size_t size);
  void quitCmd(const uint8_t* buffer, size_t size);
  void setCmd(const uint8_t* buffer, size_t size);
  uint8_t CRC8(const uint8_t* buffer, size_t len) const;
  void send_cnfStart(bool error);
};

extern Ayab_& Ayab;

#endif
