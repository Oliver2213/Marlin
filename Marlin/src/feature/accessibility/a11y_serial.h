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
 * a11y_serial.h - Serial accessibility backend (Tier 0)
 *
 * Emits //action:a11y events over serial for host-side TTS.
 */

#include "../../inc/MarlinConfig.h"

void a11y_serial_focus(FSTR_P label);
void a11y_serial_focus(const char *label);
void a11y_serial_activate(FSTR_P label);
void a11y_serial_activate(const char *label);
void a11y_serial_screen_enter(FSTR_P label);
void a11y_serial_screen_enter(const char *label);
void a11y_serial_screen_exit();
void a11y_serial_value_change(FSTR_P label, const char *value);
void a11y_serial_value_change(const char *label, const char *value);
void a11y_serial_status(const char *message, uint8_t level);
