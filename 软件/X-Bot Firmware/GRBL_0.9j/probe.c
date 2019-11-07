/*
  probe.c - code pertaining to probing methods
  Part of Grbl

  Copyright (c) 2014-2015 Sungeun K. Jeon

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
  
#include "grbl.h"


// Inverts the probe pin state depending on user settings and probing cycle mode.
uint8_t probe_invert_mask;


// Probe pin initialization routine.
void probe_init(void) 
{ 
#if defined(CPU_MAP_ATMEGA328P) || defined(CPU_MAP_ATMEGA2560)
  PROBE_DDR &= ~(PROBE_MASK); // Configure as input pins	 配置为输入引脚
  #ifdef DISABLE_PROBE_PIN_PULL_UP
    PROBE_PORT &= ~(PROBE_MASK); // Normal low operation. Requires external pull-down.	 正常低电平操作。 需要外部下拉。
  #else
    PROBE_PORT |= PROBE_MASK;    // Enable internal pull-up resistors. Normal high operation.  使能内部上拉电阻。 常高运行。
  #endif
  // probe_configure_invert_mask(false); // Initialize invert mask. Not required. Updated when in-use.	初始化反转掩码。不需要。在使用时更新。

#endif		//end of CPU_MAP_ATMEGA328P & CPU_MAP_ATMEGA2560

#if defined(CPU_MAP_STM32F10X)
  HW_GPIO_Init_In(PROBE_GPIO_CLK,PROBE_GPIO,PROBE_GPIO_PIN);
   #ifdef DISABLE_PROBE_PIN_PULL_UP
    	HW_GPIO_Write(PROBE_GPIO,PROBE_GPIO_PIN,0);//下拉。
  #else
    	HW_GPIO_Write(PROBE_GPIO,PROBE_GPIO_PIN,1);//上拉。
  #endif
#endif		//end of CPU_MAP_STM32F10X
}


#if defined(CPU_MAP_ATMEGA328P) || defined(CPU_MAP_ATMEGA2560)
// Called by probe_init() and the mc_probe() routines. Sets up the probe pin invert mask to 
// appropriately set the pin logic according to setting for normal-high/normal-low operation 
// and the probing cycle modes for toward-workpiece/away-from-workpiece. 
void probe_configure_invert_mask(uint8_t is_probe_away)
{
  probe_invert_mask = 0; // Initialize as zero
  if (bit_isfalse(settings.flags,BITFLAG_INVERT_PROBE_PIN)) { probe_invert_mask ^= PROBE_MASK; }
  if (is_probe_away) { probe_invert_mask ^= PROBE_MASK; }
}

// Returns the probe pin state. Triggered = true. Called by gcode parser and probe state monitor.
uint8_t probe_get_state(void) { return((PROBE_PIN & PROBE_MASK) ^ probe_invert_mask); }
#endif		//end of CPU_MAP_ATMEGA328P & CPU_MAP_ATMEGA2560

#if defined(CPU_MAP_STM32F10X)

void probe_configure_invert_mask(uint8_t is_probe_away)
{
	probe_invert_mask = 0; // Initialize as zero.
	if (bit_isfalse(settings.flags,BITFLAG_INVERT_PROBE_PIN)) { probe_invert_mask ^= 0x01; } 
	if (is_probe_away) { probe_invert_mask ^= 0x01; }	
}

uint8_t probe_get_state(void) 
{ 
	return((HW_GPIO_Read(PROBE_GPIO,PROBE_GPIO_PIN))^ probe_invert_mask); 	
}

#endif		//end of CPU_MAP_STM32F10X


// Monitors probe pin state and records the system position when detected. Called by the
// stepper ISR per ISR tick.
// NOTE: This function must be extremely efficient as to not bog down the stepper ISR.
void probe_state_monitor(void)
{
  if (sys_probe_state == PROBE_ACTIVE) {
    if (probe_get_state()) {
      sys_probe_state = PROBE_OFF;
      memcpy(sys.probe_position, sys.position, sizeof(sys.position));
      bit_true(sys_rt_exec_state, EXEC_MOTION_CANCEL);
    }
  }
}
