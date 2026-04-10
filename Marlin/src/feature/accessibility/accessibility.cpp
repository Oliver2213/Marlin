/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * accessibility.cpp - Accessibility event dispatcher
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(ACCESSIBILITY)

#include "accessibility.h"

#if HAS_A11Y_SERIAL
  #include "a11y_serial.h"
#endif
#if HAS_A11Y_EARCONS
  #include "a11y_earcons.h"
#endif

// State for focus-change detection across menu frames
int8_t a11y_last_encoderLine = -1;
bool   a11y_focus_changed = false;

// Called from scroll_screen() after encoderLine is computed.
void a11y_check_focus_change(int8_t encoderLine) {
  if (encoderLine != a11y_last_encoderLine) {
    a11y_last_encoderLine = encoderLine;
    a11y_focus_changed = true;
  }
}

//
// Event dispatch -- fans out to all enabled backends
//

void a11y_focus(FSTR_P label) {
  TERN_(HAS_A11Y_SERIAL, a11y_serial_focus(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_focus());
  a11y_focus_changed = false;
}

void a11y_activate(FSTR_P label) {
  TERN_(HAS_A11Y_SERIAL, a11y_serial_activate(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_activate());
}

void a11y_screen_enter(FSTR_P label) {
  a11y_last_encoderLine = -1;
  a11y_focus_changed = false;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_screen_enter(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_screen_enter());
}

void a11y_screen_exit() {
  a11y_last_encoderLine = -1;
  a11y_focus_changed = false;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_screen_exit());
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_screen_exit());
}

void a11y_status(const char *message, uint8_t level) {
  TERN_(HAS_A11Y_SERIAL, a11y_serial_status(message, level));
  if (level > 0) {
    TERN_(HAS_A11Y_EARCONS, a11y_earcon_alert());
  }
}

#endif // ACCESSIBILITY
