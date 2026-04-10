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
 * a11y_earcons.cpp - Earcon accessibility backend (Tier E)
 *
 * Short non-blocking tone patterns via the existing buzzer.
 * All patterns use at most 3 tones to stay within the
 * default TONE_QUEUE_LENGTH of 4.
 *
 * Requires SPEAKER for pitch-differentiated tones.
 * On boards without SPEAKER, tones degrade to clicks
 * but rhythm patterns are still distinguishable.
 */

#include "../../inc/MarlinConfig.h"

#if HAS_A11Y_EARCONS

#include "a11y_earcons.h"
#include "../../libs/buzzer.h"

// Single short tick -- one per item while scrolling
void a11y_earcon_focus() {
  BUZZ(8, 2000);
}

// Ascending two-tone chirp -- "confirmed"
void a11y_earcon_activate() {
  BUZZ(30, 1200);
  BUZZ(30, 1600);
}

// Ascending three-tone -- "going deeper"
void a11y_earcon_screen_enter() {
  BUZZ(30, 800);
  BUZZ(30, 1200);
  BUZZ(30, 1600);
}

// Descending two-tone -- "going back"
void a11y_earcon_screen_exit() {
  BUZZ(30, 1600);
  BUZZ(30, 1200);
}

// Alternating urgent tones -- "attention"
void a11y_earcon_alert() {
  BUZZ(100, 880);
  BUZZ(100, 440);
}

#endif // HAS_A11Y_EARCONS
