/*
  eeprom_emulate.h - EEPROM emulation methods
  Part of Grbl

  Copyright (c) 2017 Terje Io
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

#ifndef __noeeprom_h__
#define __noeeprom_h__

#define EEPROM_SIZE 1024

typedef struct {
    bool is_dirty;
    bool global_settings;
    bool build_info;
    bool startup_lines[N_STARTUP_LINE];
    bool coord_data[N_COORDINATE_SYSTEM];
} settings_dirty_t;

extern settings_dirty_t settings_dirty;

bool eeprom_emu_init();
void eeprom_emu_sync_physical ();

#endif
