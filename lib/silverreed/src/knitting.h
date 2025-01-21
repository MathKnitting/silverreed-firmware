#ifndef KNITTING_H_
#define KNITTING_H_
#include "machine/carriage.h"
#include "pattern.h"

enum KnittingState { Idle, WaitingStart, Knitting };

class KnittingProcess_ {
 private:
  KnittingProcess_() = default;
  int current_row;
  int current_stitch;
  KnittingState knitting_state;
  Carriage carriage;
  Pattern pattern;
  CarriageState previousCarriageState = CarriageState();

  int current_needle_index;
  uint8_t start_needle;
  uint8_t end_needle;

  void start_knitting_if_carriage_moves(CarriageState carriage_state);

 public:
  static KnittingProcess_ &getInstance();

  KnittingProcess_(const KnittingProcess_ &) = delete;
  KnittingProcess_ &operator=(const KnittingProcess_ &) = delete;

  void knitting_loop();
  void reset();
  bool init();
  bool start_knitting(uint8_t start_needle, uint8_t end_needle,
                      bool continuousReportingEnabled, bool beeperEnabled);
  void set_next_line(uint8_t line_number, bool last_line_flag, uint8_t *line);
};

extern KnittingProcess_ &KnittingProcess;
#endif
