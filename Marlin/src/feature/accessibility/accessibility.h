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
#pragma once

/**
 * accessibility.h - Accessibility event system
 *
 * Emits structured events for menu navigation, selection, alerts, etc.
 * Consumed by backends: serial (host TTS), earcons (buzzer tones).
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(ACCESSIBILITY)

// Runtime toggle -- accessibility is compiled in but events are
// suppressed until enabled.  Toggled by multi-click gesture.
extern bool a11y_enabled;

// Call from every click/enter path.  Tracks rapid consecutive clicks
// and toggles a11y_enabled when the configured count is reached within
// the configured window.  Safe to call unconditionally (no-ops when
// ACCESSIBILITY is disabled at compile time).
void a11y_notify_click();

// Emit when the user scrolls to a new menu item.
// label: the display string of the focused item
// NOTE: Position-in-list (e.g. "item 3 of 12") is not yet supported.
// screen_items is only valid after END_MENU, so the count is stale
// on the first frame of a new screen. Revisit when we find a clean
// way to defer the event or buffer the count.
void a11y_focus(FSTR_P label);
void a11y_focus(const char *label);

// Emit when the user clicks/selects a menu item.
void a11y_activate(FSTR_P label);
void a11y_activate(const char *label);

// Emit when navigating into a submenu. label = the submenu name.
void a11y_screen_enter(FSTR_P label);
void a11y_screen_enter(const char *label);

// Emit when navigating back out of a submenu.
void a11y_screen_exit();

// Emit when a value changes during editing (encoder turn on edit screen).
// label: the setting name (e.g. "Fan Speed")
// value: the formatted value string (e.g. "100%")
void a11y_value_change(FSTR_P label, const char *value);
void a11y_value_change(const char *label, const char *value);

// Emit for status bar messages and alerts.
// level: 0 = info/status, >0 = alert (higher = more urgent)
void a11y_status(const char *message, uint8_t level=0);

//
// Internal state used by menu hooks to detect focus changes.
// Do not call directly -- used by menu_item.h / menu.cpp macros.
//
extern int8_t a11y_last_encoderLine;
extern bool   a11y_focus_changed;

// Call from scroll_screen() after encoderLine is updated.
void a11y_check_focus_change(int8_t encoderLine);

#endif // ACCESSIBILITY
