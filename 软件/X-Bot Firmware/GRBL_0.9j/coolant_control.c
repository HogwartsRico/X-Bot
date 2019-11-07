/*
  coolant_control.c - coolant control methods
  Part of Grbl

  Copyright (c) 2012-2015 Sungeun K. Jeon

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

#if defined(CPU_MAP_ATMEGA328P) || defined(CPU_MAP_ATMEGA2560)

void coolant_init(void)	 
{
  COOLANT_FLOOD_DDR |= (1 << COOLANT_FLOOD_BIT);
  #ifdef ENABLE_M7
    COOLANT_MIST_DDR |= (1 << COOLANT_MIST_BIT);
  #endif
  coolant_stop();
}


void coolant_stop(void)
{
  COOLANT_FLOOD_PORT &= ~(1 << COOLANT_FLOOD_BIT);
  #ifdef ENABLE_M7
    COOLANT_MIST_PORT &= ~(1 << COOLANT_MIST_BIT);
  #endif
}


void coolant_set_state(uint8_t mode)
{
  if (mode == COOLANT_FLOOD_ENABLE) {
    COOLANT_FLOOD_PORT |= (1 << COOLANT_FLOOD_BIT);

  #ifdef ENABLE_M7  
    } else if (mode == COOLANT_MIST_ENABLE) {
      COOLANT_MIST_PORT |= (1 << COOLANT_MIST_BIT);
  #endif

  } else {
    coolant_stop();
  }
}

#endif		//end of CPU_MAP_ATMEGA328P & CPU_MAP_ATMEGA2560

#if defined(CPU_MAP_STM32F10X)

void coolant_init(void)	  //冷却引脚初始化
{
  HW_GPIO_Init_Out(COOLANT_FLOOD_GPIO_CLK,COOLANT_FLOOD_GPIO,COOLANT_FLOOD_GPIO_PIN);
  #ifdef ENABLE_M7
    HW_GPIO_Init_Out(COOLANT_MIST_GPIO_CLK,COOLANT_MIST_GPIO,COOLANT_MIST_GPIO_PIN);
  #endif
  coolant_stop();
}


void coolant_stop(void)		 //冷却关闭
{
  HW_GPIO_Write(COOLANT_FLOOD_GPIO,COOLANT_FLOOD_GPIO_PIN,0);		//流冷却引脚置0
  #ifdef ENABLE_M7
    HW_GPIO_Write(COOLANT_MIST_GPIO,COOLANT_MIST_GPIO_PIN,0);		//雾冷却引脚置0
  #endif
}


void coolant_set_state(uint8_t mode)  //冷却设置状态
{
  if (mode == COOLANT_FLOOD_ENABLE) {
    HW_GPIO_Write(COOLANT_FLOOD_GPIO,COOLANT_FLOOD_GPIO_PIN,1);	   //流冷却引脚置1

  #ifdef ENABLE_M7  
    } else if (mode == COOLANT_MIST_ENABLE) {
      HW_GPIO_Write(COOLANT_MIST_GPIO,COOLANT_MIST_GPIO_PIN,1);	   //雾冷却引脚置1
  #endif

  } else {
    coolant_stop();
  }
}

#endif		//end of CPU_MAP_STM32F10X



void coolant_run(uint8_t mode)
{
  if (sys.state == STATE_CHECK_MODE) { return; }
  protocol_buffer_synchronize(); // Ensure coolant turns on when specified in program. 	
  coolant_set_state(mode);
}
