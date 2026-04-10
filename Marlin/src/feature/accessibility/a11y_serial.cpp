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
 * a11y_serial.cpp - Serial accessibility backend (Tier 0)
 *
 * Emits structured events over serial using the //action: protocol.
 * A host-side companion (OctoPrint plugin, Python script, etc.)
 * parses these and drives TTS via espeak-ng or system speech.
 *
 * Format:
 *   //action:a11y focus "Label"
 *   //action:a11y activate "Label"
 *   //action:a11y screen_enter "Label"
 *   //action:a11y screen_exit
 *   //action:a11y alert "Message"
 *   //action:a11y status "Message"
 */

#include "../../inc/MarlinConfig.h"

#if HAS_A11Y_SERIAL

#include "a11y_serial.h"
#include "../../core/serial.h"

// Helper: print the //action:a11y prefix and event name
static void a11y_serial_begin(FSTR_P event) {
  PORT_REDIRECT(SerialMask::All);
  SERIAL_ECHOPGM("//action:a11y ");
  SERIAL_ECHO(event);
}

// Helper: print a quoted FSTR label
static void a11y_serial_quote_f(FSTR_P label) {
  SERIAL_CHAR(' ', '"');
  SERIAL_ECHO(label);
  SERIAL_CHAR('"');
}

void a11y_serial_focus(FSTR_P label) {
  a11y_serial_begin(F("focus"));
  a11y_serial_quote_f(label);
  SERIAL_EOL();
}

void a11y_serial_activate(FSTR_P label) {
  a11y_serial_begin(F("activate"));
  a11y_serial_quote_f(label);
  SERIAL_EOL();
}

void a11y_serial_screen_enter(FSTR_P label) {
  a11y_serial_begin(F("screen_enter"));
  a11y_serial_quote_f(label);
  SERIAL_EOL();
}

void a11y_serial_screen_exit() {
  a11y_serial_begin(F("screen_exit"));
  SERIAL_EOL();
}

void a11y_serial_status(const char *message, uint8_t level) {
  a11y_serial_begin(level > 0 ? F("alert") : F("status"));
  SERIAL_CHAR(' ', '"');
  SERIAL_ECHO(message);
  SERIAL_CHAR('"');
  SERIAL_EOL();
}

#endif // HAS_A11Y_SERIAL
