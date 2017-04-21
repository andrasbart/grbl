/*
  report.h - reporting and messaging methods
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

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
#ifndef report_h
#define report_h

#include "system.h"

// Prints system status messages.
void report_status_message(status_code_t status_code);

// Prints system alarm messages.
void report_alarm_message(alarm_code_t alarm_code);

// Prints miscellaneous feedback messages.
void report_feedback_message(message_code_t message_code);

// Prints welcome message
void report_init_message();

// Prints Grbl help and current global settings
void report_grbl_help();

// Prints Grbl global settings
void report_grbl_settings();

// Prints an echo of the pre-parsed line received right before execution.
void report_echo_line_received(char *line);

// Prints realtime status report
void report_realtime_status();

// Prints recorded probe position
void report_probe_parameters();

// Prints Grbl NGC parameters (coordinate offsets, probe)
void report_ngc_parameters();

// Prints current g-code parser mode state
void report_gcode_modes();

// Prints startup line when requested and executed.
void report_startup_line(uint8_t n, char *line);
void report_execute_startup_message(char *line, status_code_t status_code);

// Prints build info and user info
void report_build_info(char *line);

#endif
