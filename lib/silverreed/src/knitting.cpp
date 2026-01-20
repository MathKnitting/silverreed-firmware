#include "knitting.h"

#include "communication/ayab.h"
#include "config.h"
#include "debug.h"
#include "pattern.h"

KnittingProcess_& KnittingProcess = KnittingProcess.getInstance();

KnittingProcess_& KnittingProcess_::getInstance() {
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

  this->current_needle_index = CARRIAGE_OFF_PATTERN;
  this->carriage.power_solenoid(LOW);
  this->is_start_out_of_pattern = false;
  DEBUG_WAIT_START();
}

bool KnittingProcess_::init() {
  /**
   * Initialize the knitting process.
   * This function is called when Ayab sends a request to initialize the
   * knitting process (reqInit).
   *
   * @return true if initialization succeeded
   */
  // If not in Idle state, reset the knitting process first
  if (this->knitting_state != Idle) {
    DEBUG_PRINTLN("reqInit received while not Idle, resetting process");
    KnittingProcess.reset();
  }

  DEBUG_PRINTLN("Initializing knitting process");
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
   * @return true if knitting started successfully, false if invalid parameters
   */
  // Validate needle range
  if (end_needle < start_needle) {
    DEBUG_PRINTLN("Invalid needle range: end < start");
    return false;
  }

  if (end_needle >= DEFAULT_MAX_NEEDLES) {
    DEBUG_PRINTLN("Invalid needle range: exceeds maximum");
    return false;
  }

  // Only allow starting from WaitingStart state
  if (this->knitting_state != WaitingStart) {
    DEBUG_PRINTLN("Cannot start: not in WaitingStart state");
    return false;
  }

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
    this->carriage.power_solenoid(HIGH);
  }
}

void KnittingProcess_::set_next_line(uint8_t line_number, bool last_line_flag,
                                     uint8_t* line) {
  /**
   * Set the next line of the pattern.
   * This function is called when Ayab sends a line of the pattern (cnfLine).
   *
   * @param line_number The number of the line.
   * @param last_line_flag If the line is the last line of the pattern.
   * @param line The buffer of the line.
   *
   * @warning This function assumes the knitting process is in a valid state
   * (WaitingStart or Knitting). It should only be called in response to
   * reqLine requests.
   */
  if (this->knitting_state != WaitingStart &&
      this->knitting_state != Knitting) {
    DEBUG_PRINTLN("set_next_line: invalid state");
    return;
  }

  if (last_line_flag) {
    // Ayab sends one row in advance for brother knitting (preparation row)
    // So when we get the last_line_flag, it means the knitting is already
    // finished
    this->reset();
  }

  if (line == nullptr) {
    DEBUG_PRINTLN("set_next_line: null buffer");
    return;
  }

  this->current_row = line_number + 1;
  this->pattern.set_buffer(line);
  this->is_last_line = last_line_flag;
  return;
}

void KnittingProcess_::knitting_loop() {
  /**
   * The main loop of the knitting process.
   * This function is called in the loop of the Arduino.
   * It handles the knitting process state machine.
   */

  // To avoid any incoherence, we always get the current state of the carriage
  // at the beginning of the loop. This state will be used every time we need to
  // read carriage state during this iteration.
  CarriageState current_carriage_state = CarriageState::read_from_pins();

  // Track carriage movement and manage solenoid power
  bool carriage_is_moving =
      current_carriage_state.is_carriage_moving(this->previousCarriageState);

  switch (knitting_state) {
    case Idle:
      // nothing to do in this case
      break;

    case WaitingStart: {
      // Waiting for the carriage to move to start the knitting process
      this->start_knitting_if_carriage_moves(current_carriage_state);
      if (carriage_is_moving) {
        this->carriage.update_last_movement();
      }
      break;
    }
    case Knitting: {
      // Knitting mode

      // Track movement and update timestamp
      if (carriage_is_moving) {
        this->carriage.update_last_movement();
        // If solenoid was turned off due to inactivity, turn it back on
        if (!this->carriage.is_solenoid_powered()) {
          this->carriage.power_solenoid(HIGH);
        }
      }

      // Check for inactivity timeout and turn off solenoid if necessary
      this->carriage.check_and_shutoff_if_inactive();

      // Always request the first row when the knitting process starts
      if (this->current_row == 0) {
        DEBUG_PRINTLN("Requesting first row");
        Ayab.sendReqLine(0);
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
      if (current_carriage_state.is_start_out_of_pattern(
              this->previousCarriageState)) {
        this->is_start_out_of_pattern = true;
      }

      // pattern
      if (current_carriage_state.is_in_pattern_section() && start_of_needle) {
        // DOB state must change only when the carriage is at the start of the
        // needle
        this->current_needle_index++;

        bool needle_state = this->pattern.get_needle_state(
            this->current_needle_index, current_carriage_state.get_direction());
        this->carriage.set_DOB_state(needle_state);

      } else if (this->is_start_out_of_pattern && start_of_needle) {
        // carriage just moved out of pattern section
        // WARNING: the carriage really finished to knit the pattern only when
        // it is at the start of the needle after the KSL went from HIGH TO LOW
        this->is_start_out_of_pattern = false;
        this->current_needle_index = CARRIAGE_OFF_PATTERN;
        // out of pattern section (KSL HIGH), the DOB must be low to avoid
        // eating the solenoids.
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
