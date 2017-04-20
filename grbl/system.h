/*
  system.h - Header for system level commands and real-time processes
  Part of Grbl

  Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef system_h
#define system_h

#include "grbl.h"

// Define system executor bit map. Used internally by realtime protocol as realtime command flags,
// which notifies the main program to execute the specified realtime command asynchronously.
// NOTE: The system executor uses an unsigned 8-bit volatile variable (8 flag limit.) The default
// flags are always false, so the realtime protocol only needs to check for a non-zero value to
// know when there is a realtime command to execute.
#define EXEC_STATUS_REPORT  bit(0) // bitmask 00000001
#define EXEC_CYCLE_START    bit(1) // bitmask 00000010
#define EXEC_CYCLE_STOP     bit(2) // bitmask 00000100
#define EXEC_FEED_HOLD      bit(3) // bitmask 00001000
#define EXEC_RESET          bit(4) // bitmask 00010000
#define EXEC_SAFETY_DOOR    bit(5) // bitmask 00100000
#define EXEC_MOTION_CANCEL  bit(6) // bitmask 01000000
#define EXEC_SLEEP          bit(7) // bitmask 10000000

// Define system state bit map. The state variable primarily tracks the individual functions
// of Grbl to manage each without overlapping. It is also used as a messaging flag for
// critical events.
// NOTE: flags are mutually exclusive, bit map allows testing for multiple states (except STATE_IDLE) in a single statement
#define STATE_IDLE          0      // Must be zero. No flags.
#define STATE_ALARM         bit(0) // In alarm state. Locks out all g-code processes. Allows settings access.
#define STATE_CHECK_MODE    bit(1) // G-code check mode. Locks out planner and motion only.
#define STATE_HOMING        bit(2) // Performing homing cycle
#define STATE_CYCLE         bit(3) // Cycle is running or motions are being executed.
#define STATE_HOLD          bit(4) // Active feed hold
#define STATE_JOG           bit(5) // Jogging mode.
#define STATE_SAFETY_DOOR   bit(6) // Safety door is ajar. Feed holds and de-energizes system.
#define STATE_SLEEP         bit(7) // Sleep state.

// Define Grbl status codes. Valid values (0-255)
typedef enum {
    Status_OK = 0,
    Status_ExpectedCommandLetter = 1,
    Status_BadNumberFormat = 2,
    Status_InvalidStatement = 3,
    Status_NegativeValue = 4,
    Status_SettingDisabled = 5,
    Status_SettingStepPulseMin = 6,
    Status_SettingReadFail = 7,
    Status_IdleError = 8,
    Status_SystemGClock = 9,
    Status_SoftLimitError = 10,
    Status_Overflow = 11,
    Status_MaxStepRateExceeded = 12,
    Status_CheckDoor = 13,
    Status_LineLengthExceeded = 14,
    Status_TravelExceeded = 15,
    Status_InvalidJogCommand = 16,
    Status_SettingDisabledLaser = 17,

    Status_GcodeUnsupportedCommand = 20,
    Status_GcodeModalGroupViolation = 21,
    Status_GcodeUndefinedFeedRate = 22,
    Status_GcodeCommandValueNotInteger = 23,
    Status_GcodeAxisCommandConflict = 24,
    Status_GcodeWordRepeated = 25,
    Status_GcodeNoAxisWords = 26,
    Status_GcodeInvalidLineNumber = 27,
    Status_GcodeValueWordMissing = 28,
    Status_GcodeUnsupportedCoordSys = 29,
    Status_GcodeG53InvalidMotionMode = 30,
    Status_GcodeAxisWordsExist = 31,
    Status_GcodeNoAxisWordsInPlane = 32,
    Status_GcodeInvalidTarget = 33,
    Status_GcodeArcRadiusError = 34,
    Status_GcodeNoOffsetsInPlane = 35,
    Status_GcodeUnusedWords = 36,
    Status_GcodeG43DynamicAxisError = 37,
    Status_GcodeMaxValueExceeded = 38
} status_code_t;

// Define Grbl feedback message codes. Valid values (0-255).
typedef enum {
    Message_CriticalEvent = 1,
    Message_AlarmLock = 2,
    Message_AlarmUnlock = 3,
    Message_Enabled = 4,
    Message_Disabled = 5,
    Message_SafetyDoorAjar = 6,
    Message_CheckLimits = 7,
    Message_ProgramEnd = 8,
    Message_RestoreDefaults = 9,
    Message_SpindleRestore = 10,
    Message_SleepMode = 11
} message_code_t;

// Alarm executor codes. Valid values (1-255). Zero is reserved.
typedef enum {
    Alarm_HardLimit = 1,
    Alarm_SoftLimit = 2,
    Alarm_AbortCycle = 3,
    Alarm_ProbeFailInitial = 4,
    Alarm_ProbeFailContact = 5,
    Alarm_HomingFailReset = 6,
    Alarm_HomingFailDoor = 7,
    Alarm_FailPulloff = 8,
    Alarm_HomingFailApproach = 9
} alarm_code_t;

// Define step segment generator state flags.
typedef union {
    uint8_t value;
    struct {
        uint8_t end_motion         :1,
                execute_hold       :1,
                execute_sys_motion :1,
                update_spindle_pwm :1;
    };
} step_control_t;


typedef union {
    uint8_t value;
    struct {
        uint8_t reset       :1,
                feed_hold   :1,
                cycle_start :1,
                safety_door :1;
    };
} controlsignals_t;

typedef union {
    uint8_t value;
    struct {
        uint8_t hold_complete    :1, // Indicates initial feed hold is complete.
                restart_retract  :1, // Flag to indicate a retract from a restore parking motion.
                retract_complete :1, // (Safety door only) Indicates retraction and de-energizing is complete.
                initiate_restore :1, // (Safety door only) Flag to initiate resume procedures from a cycle start.
                restore_complete :1, // (Safety door only) Indicates ready to resume normal operation.
                safety_door_ajar :1, // Tracks safety door state for resuming.
                motion_cancel    :1, // Indicates a canceled resume motion. Currently used by probing routine.
                jog_cancel       :1; // Indicates a jog cancel in process and to reset buffers when complete.
    };
} suspend_t;

// Define spindle stop override control states.
typedef union {
    uint8_t value;
    struct {
        uint8_t enabled       :1,
                initiate      :1,
                restore       :1,
                restore_cycle :1;
    };
} spindle_stop_t;

// Define global system variables
typedef struct {
  uint8_t state;               // Tracks the current system state of Grbl.
  uint8_t abort;               // System abort flag. Forces exit back to main loop for reset.
  bool exit;				   // System exit flag. Used in combination with abort to terminate main loop.
  suspend_t suspend;           // System suspend bitflag variable that manages holds, cancels, and safety door.
  bool soft_limit;             // Tracks soft limit errors for the state machine. (boolean)
  bool block_delete_enabled;   // Set to true to enable block delete
  step_control_t step_control; // Governs the step segment generator depending on system state.
  bool probe_succeeded;        // Tracks if last probing cycle was successful.
  axes_signals_t homing_axis_lock;    // Locks axes when limits engage. Used as an axis motion mask in the stepper ISR.
  uint8_t f_override;          // Feed rate override value in percent
  uint8_t r_override;          // Rapids override value in percent
  uint8_t spindle_speed_ovr;   // Spindle speed value in percent
  spindle_stop_t spindle_stop_ovr;    // Tracks spindle stop override states
  uint8_t report_ovr_counter;  // Tracks when to add override data to status reports.
  uint8_t report_wco_counter;  // Tracks when to add work coordinate offset data to status reports.
  #ifdef ENABLE_PARKING_OVERRIDE_CONTROL
    uint8_t override_ctrl;     // Tracks override control states.
  #endif
  #ifdef VARIABLE_SPINDLE
    float spindle_speed;
  #endif
} system_t;
extern system_t sys;

// NOTE: These position variables may need to be declared as volatiles, if problems arise.
extern int32_t sys_position[N_AXIS];      // Real-time machine (aka home) position vector in steps.
extern int32_t sys_probe_position[N_AXIS]; // Last probe position in machine coordinates and steps.

extern volatile uint8_t sys_probe_state;   // Probing stue.  Used to coordinate the probing cycle with stepper ISR.
extern volatile uint8_t sys_rt_exec_state;   // Global realtime executor bitflag variable for state management. See EXEC bitmasks.
extern volatile uint8_t sys_rt_exec_alarm;   // Global realtimeate val executor bitflag variable for setting various alarms.

#ifdef DEBUG
  #define EXEC_DEBUG_REPORT  bit(0)
  extern volatile uint8_t sys_rt_exec_debug;
#endif

// Returns bitfield of control pin states, organized by CONTROL_PIN_INDEX. (1=triggered, 0=not triggered).
#define system_control_get_state() hal.system_control_get_state()

// Returns if safety door is open or closed, based on pin state.
bool system_check_safety_door_ajar();

// Executes an internal system command, defined as a string starting with a '$'
status_code_t system_execute_line(char *line);

// Execute the startup script lines stored in EEPROM upon initialization
void system_execute_startup(char *line);


void system_flag_wco_change();

// Returns machine position of axis 'idx'. Must be sent a 'step' array.
inline float system_convert_axis_steps_to_mpos(int32_t *steps, uint32_t idx);

// Updates a machine 'position' array based on the 'step' array sent.
void system_convert_array_steps_to_mpos(float *position, int32_t *steps);

// CoreXY calculation only. Returns x or y-axis "steps" based on CoreXY motor steps.
#ifdef COREXY
  int32_t system_convert_corexy_to_x_axis_steps(int32_t *steps);
  int32_t system_convert_corexy_to_y_axis_steps(int32_t *steps);
#endif

// Checks and reports if target array exceeds machine travel limits.
bool system_check_travel_limits(float *target);

// Special handlers for setting and clearing Grbl's real-time execution flags.
#define system_set_exec_state_flag(mask) hal.set_bits_atomic(&sys_rt_exec_state, (mask))
#define system_clear_exec_state_flag(mask) hal.clear_bits_atomic(&sys_rt_exec_state, (mask))
#define system_clear_exec_states() hal.set_value_atomic(&sys_rt_exec_state, 0)
#define system_set_exec_alarm(code) hal.set_value_atomic(&sys_rt_exec_alarm, (uint8_t)(code))
#define system_clear_exec_alarm() hal.set_value_atomic(&sys_rt_exec_alarm, 0)

void control_interrupt_handler (controlsignals_t signals);

#endif
