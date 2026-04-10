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

// Runtime toggle -- defaults off each boot
bool a11y_enabled = false;

// State for focus-change detection across menu frames
int8_t a11y_last_encoderLine = -1;
bool   a11y_focus_changed = false;

//
// Multi-click toggle detection
//
// Tracks rapid consecutive encoder clicks.  When the configured number
// of clicks lands within the timing window, a11y_enabled is toggled.
//
#ifndef A11Y_TOGGLE_CLICKS
  #define A11Y_TOGGLE_CLICKS 3
#endif
#ifndef A11Y_TOGGLE_CLICK_WINDOW_MS
  #define A11Y_TOGGLE_CLICK_WINDOW_MS 175
#endif

void a11y_notify_click() {
  static uint8_t click_count = 0;
  static millis_t last_click_ms = 0;

  const millis_t now = millis();

  // If too long since last click, restart the count
  if (click_count == 0 || ELAPSED(now, last_click_ms + A11Y_TOGGLE_CLICK_WINDOW_MS)) {
    click_count = 1;
    last_click_ms = now;
    return;
  }

  click_count++;
  last_click_ms = now;

  if (click_count >= A11Y_TOGGLE_CLICKS) {
    click_count = 0;
    a11y_enabled = !a11y_enabled;

    // Confirmation earcon: ascending = on, descending = off
    #if HAS_A11Y_EARCONS
      if (a11y_enabled)
        a11y_earcon_screen_enter();   // ascending 3-tone = "enabled"
      else
        a11y_earcon_screen_exit();    // descending 2-tone = "disabled"
    #endif
  }
}

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
  a11y_focus_changed = false;
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_focus(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_focus());
}
void a11y_focus(const char *label) {
  a11y_focus_changed = false;
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_focus(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_focus());
}

void a11y_activate(FSTR_P label) {
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_activate(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_activate());
}
void a11y_activate(const char *label) {
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_activate(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_activate());
}

void a11y_screen_enter(FSTR_P label) {
  a11y_last_encoderLine = -1;
  a11y_focus_changed = false;
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_screen_enter(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_screen_enter());
}
void a11y_screen_enter(const char *label) {
  a11y_last_encoderLine = -1;
  a11y_focus_changed = false;
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_screen_enter(label));
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_screen_enter());
}

void a11y_screen_exit() {
  a11y_last_encoderLine = -1;
  a11y_focus_changed = false;
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_screen_exit());
  TERN_(HAS_A11Y_EARCONS, a11y_earcon_screen_exit());
}

void a11y_value_change(FSTR_P label, const char *value) {
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_value_change(label, value));
}
void a11y_value_change(const char *label, const char *value) {
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_value_change(label, value));
}

void a11y_status(const char *message, uint8_t level) {
  if (!a11y_enabled) return;
  TERN_(HAS_A11Y_SERIAL, a11y_serial_status(message, level));
  if (level > 0) {
    TERN_(HAS_A11Y_EARCONS, a11y_earcon_alert());
  }
}

#endif // ACCESSIBILITY
