/**
 * @file debug.h
 * @brief Debugging utilities that can be used in the code and enabled by a
 * build flag -D DEBUG
 */
#ifdef DEBUG
#define DEBUG_START() Serial.begin(9600);
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTDEC(x) Serial.print(x, DEC)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_WAIT_START() KnittingProcess.init();
#define DEBUG_START_KNITTING()                                              \
  KnittingProcess.start_knitting(84, 116, false, false);                    \
  uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                      0x00, 0x00, 0xe0, 0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00, \
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};            \
  KnittingProcess.set_next_line(0, 0, buffer);
#else
#define DEBUG_START()
#define DEBUG_PRINT(x)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_START_KNITTING()
#define DEBUG_WAIT_START()
#define DEBUG_START_KNITTING()
#endif
