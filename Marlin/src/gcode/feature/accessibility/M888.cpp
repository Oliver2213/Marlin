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

#include "../../../inc/MarlinConfig.h"

#if HAS_ACCESSIBILITY

#include "../../../feature/accessibility/accessibility.h"
#if HAS_A11Y_EARCONS
  #include "../../../feature/accessibility/a11y_earcons.h"
#endif
#include "../../gcode.h"

/**
 * M888: Toggle or set accessibility mode
 *
 *   No args  Toggle accessibility on/off
 *
 *   S<bool>  Set accessibility on (S1) or off (S0)
 */
void GcodeSuite::M888() {
  const bool was_enabled = a11y_enabled;

  if (parser.seenval('S'))
    a11y_enabled = parser.value_bool();
  else
    a11y_enabled = !a11y_enabled;

  // Confirmation earcon on state change
  #if HAS_A11Y_EARCONS
    if (a11y_enabled != was_enabled) {
      if (a11y_enabled)
        a11y_earcon_screen_enter();
      else
        a11y_earcon_screen_exit();
    }
  #endif

  // Always report current state
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Accessibility: ");
  if (a11y_enabled) SERIAL_ECHOLNPGM("on");
  else              SERIAL_ECHOLNPGM("off");
}

#endif // HAS_ACCESSIBILITY
