#include "knitting.h"

#include "communication/ayab.h"
#include "config.h"
#include "debug.h"
#include "pattern.h"

KnittingProcess_ &KnittingProcess = KnittingProcess.getInstance();

KnittingProcess_ &KnittingProcess_::getInstance() {
  static KnittingProcess_ instance;
  return instance;
}

void KnittingProcess_::reset() {
  /**
   * Reset the knitting process to initial state.
   */
  this->knitting_state = Idle;
  this->current_row = 0;
  this->current_stitch = 0;
  this->carriage = Carriage();
  this->pattern = Pattern();

  this->current_needle_index =
      -1;  // -1 means that the carriage is not on a needle of the pattern
  DEBUG_WAIT_START();
}

bool KnittingProcess_::init() {
  /**
   * Initialize the knitting process.
   * This function is called when Ayab sends a request to initialize the
   * knitting process (reqInit).
   */
  if (this->knitting_state != Idle) {
    return false;
  }
  this->knitting_state = WaitingStart;

  return true;
}

bool KnittingProcess_::start_knitting(uint8_t start_needle, uint8_t end_needle,
                                      bool continuous_reporting_enabled,
                                      bool beeper_enabled) {
  /**
   * Start the knitting process.
   * This function is called when Ayab sends a request to start the knitting
   * process (reqStart).
   *
   * @param start_needle The first needle of the pattern.
   * @param end_needle The last needle of the pattern.
   * @param continuous_reporting_enabled If the continuous reporting is enabled.
   * TODO UNUSED
   * @param beeper_enabled If the beeper is enabled. TODO UNUSED
   */
  this->start_needle = start_needle;
  this->end_needle = end_needle;
  this->knitting_state = Knitting;
  this->pattern.set_needle_range(start_needle, end_needle);
  return true;
}

void KnittingProcess_::start_knitting_if_carriage_moves(
    CarriageState carriage_state) {
  /**
   * Start the knitting process if the carriage moves.
   * This function is called when the knitting process is in the WaitingStart
   * state.
   * It checks if the carriage moves.
   * If the carriage moves, it starts the knitting process by requesting the
   * first row to ayab.
   *
   * NOTE: With this method, if the carriage first moves in the wrong direction,
   * it can cause issues on Ayab side because it will get a wrong information.
   *
   * @param carriage_state The current state of the carriage.
   */
  if (carriage_state.is_carriage_moving(this->previousCarriageState) &&
      carriage_state.is_start_of_needle(this->previousCarriageState)) {
    DEBUG_PRINTLN("Carriage moving, start knitting");
    Ayab.getInstance().sendIndState(carriage_state.get_direction());
  }
}

void KnittingProcess_::set_next_line(uint8_t line_number, bool last_line_flag,
                                     uint8_t *line) {
  /**
   * Set the next line of the pattern.
   * This function is called when Ayab sends a line of the pattern (cnfLine).
   *
   * @param line_number The number of the line.
   * @param last_line_flag If the line is the last line of the pattern.
   * @param line The buffer of the line.
   */
  this->current_row = line_number + 1;
  this->pattern.set_buffer(line);
  return;
}

void KnittingProcess_::knitting_loop() {
  /**
   * The main loop of the knitting process.
   * This function is called in the loop of the Arduino.
   * It handles the knitting process state machine.
   */

  // To avoid any incoherence, we always get the current state of the carriage
  // at the beginning of the loop this state will be used every time we need to
  // read carriage state
  CarriageState current_carriage_state = CarriageState();

  switch (knitting_state) {
    case Idle:
      // nothing to do in this case
      break;

    case WaitingStart:
    {
      // Waiting for the carriage to move to start the knitting process
      this->start_knitting_if_carriage_moves(current_carriage_state);
      break;
    }
    case Knitting:
    {
      // Knitting mode

      // Always request the first row when the knitting process starts
      if (this->current_row == 0) {
        DEBUG_PRINTLN("Requesting first row");
        Ayab.sendReqLine(1);
        this->current_row++;  // need to explicitly increment the row here; if
                              // not the row will be 0
        // and the next loop iteration will request the first row again if ayab
        // didn't have the time to send the row
        break;
      }

      bool start_of_needle = current_carriage_state.is_start_of_needle(
          this->previousCarriageState);  // check if the carriage is at the
                                         // start of the needle, always done
                                         // to refresh the previous state
      // pattern
      if (current_carriage_state.is_in_pattern_section() && start_of_needle) {
        // DOB state must change only when the carriage is at the start of the
        // needle
        this->current_needle_index++;

        bool needle_state = this->pattern.get_needle_state(
            this->current_needle_index, current_carriage_state.get_direction());
        this->carriage.set_DOB_state(needle_state);

      } else if (current_carriage_state.is_start_out_of_pattern(
                     this->previousCarriageState)) {
        this->current_needle_index = -1;
        // out of pattern section (KSL HIGH), the DOB must be low
        this->carriage.set_DOB_state(LOW);
        Ayab.sendReqLine(this->current_row);
      }
    }
    default:
      break;
  }

  // Update the previous carriage to be able to do states comparisons
  this->previousCarriageState = current_carriage_state;
}
