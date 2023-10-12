/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_RESUME_CONTINUE

#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/planner.h" // for synchronize()
#include "../../MarlinCore.h"     // for wait_for_user_response()

#if HAS_LCD_MENU
#include "../../lcd/ultralcd.h"
#elif ENABLED(EXTENSIBLE_UI)
#include "../../lcd/extui/ui_api.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
#include "../../feature/host_actions.h"
#endif

 /**
  * M0: Unconditional stop - Wait for user to press a button or a timer to end visible on the screen
  */
void GcodeSuite::M2() {
  millis_t ms = 0;
  if (parser.seenval('S'))
  {
    ms = parser.value_millis_from_seconds();
  }
  else
  {
    return;
  }

  planner.synchronize();
     // Seconds to wait
  if (ms > 599000 || !parser.string_arg)//9 minuti e 59 secondi
  {
    return;
  }

  TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, PSTR("M2 Stop"), CONTINUE_STR));

  wait_for_timer_or_response(ms, parser.string_arg);

  TERN_(HAS_LCD_MENU, ui.reset_status());
}

#endif // HAS_RESUME_CONTINUE
