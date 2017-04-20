/*
  serial.c - Low level functions for sending and recieving bytes via the serial port
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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

#ifndef serial_h
#define serial_h

#define SERIAL_NO_DATA -1

#define SERIAL_RX_BUFFER_SIZE hal.rx_buffer_size
#define serial_write(c) hal.serial_write(c)
#define serial_read() hal.serial_read()
#define serial_reset_read_buffer() hal.serial_reset_read_buffer()
#define serial_get_rx_buffer_available() hal.serial_get_rx_buffer_available()

#endif
