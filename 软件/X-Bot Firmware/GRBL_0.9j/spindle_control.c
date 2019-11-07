/*
  spindle_control.c - spindle control methods
  Part of Grbl

  Copyright (c) 2012-2015 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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


void spindle_init(void)
{   
#if defined(CPU_MAP_ATMEGA328P) || defined(CPU_MAP_ATMEGA2560)
  // Configure variable spindle PWM and enable pin, if requried. On the Uno, PWM and enable are
  // combined unless configured otherwise.
  //如果需要，配置可变速主轴PWM和使能引脚。 在Uno上，PWM和使能引脚位置重合，除非另有配置。
  #ifdef VARIABLE_SPINDLE
    SPINDLE_PWM_DDR |= (1<<SPINDLE_PWM_BIT); // Configure as PWM output pin.  配置为PWM引脚
    #if defined(CPU_MAP_ATMEGA2560) || defined(USE_SPINDLE_DIR_AS_ENABLE_PIN)
      SPINDLE_ENABLE_DDR |= (1<<SPINDLE_ENABLE_BIT); // Configure as output pin.   配置为输出引脚
    #endif     
  // Configure no variable spindle and only enable pin.		配置无可变速主轴，仅使能引脚。
  #else  
    SPINDLE_ENABLE_DDR |= (1<<SPINDLE_ENABLE_BIT); // Configure as output pin. 配置为输出引脚
  #endif
  
  #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
    SPINDLE_DIRECTION_DDR |= (1<<SPINDLE_DIRECTION_BIT); // Configure as output pin.   配置为输出引脚
  #endif
#endif		//end of CPU_MAP_ATMEGA328P & CPU_MAP_ATMEGA2560

#if defined(CPU_MAP_STM32F10X)
   #ifdef VARIABLE_SPINDLE
	HW_PWM_Init(SPINDLE_PWM_GPIO_CLK,SPINDLE_PWM_GPIO,SPINDLE_PWM_GPIO_PIN,PWM_CLK,PWM_TIMER,PWM_MAX_VALUE);//PWM初始化
	HW_GPIO_Init_Out(SPINDLE_ENABLE_GPIO_CLK,SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN);	//使能脚初始化
   #else 
	HW_GPIO_Init_Out(SPINDLE_ENABLE_GPIO_CLK,SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN);	//使能脚初始化
   #endif

  #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
   	HW_GPIO_Init_Out(SPINDLE_DIRECTION_GPIO_CLK,SPINDLE_DIRECTION_GPIO,SPINDLE_DIRECTION_GPIO_PIN);
  #endif

#endif		//end of CPU_MAP_STM32F10X

  spindle_stop();
}


void spindle_stop(void)
{

#if defined(CPU_MAP_ATMEGA328P) || defined(CPU_MAP_ATMEGA2560)
  // On the Uno, spindle enable and PWM are shared. Other CPUs have seperate enable pin.  
  #ifdef VARIABLE_SPINDLE
    TCCRA_REGISTER &= ~(1<<COMB_BIT); // Disable PWM. Output voltage is zero.	禁用PWM。 输出电压为零。
    #if defined(CPU_MAP_ATMEGA2560) || defined(USE_SPINDLE_DIR_AS_ENABLE_PIN)
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);  // Set pin to high	 将引脚设置为高
      #else
        SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT); // Set pin to low	将引脚设置为低
      #endif
    #endif
  #else
    #ifdef INVERT_SPINDLE_ENABLE_PIN
      SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);  // Set pin to high  将引脚设置为高
    #else
      SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT); // Set pin to low  将引脚设置为低
    #endif
  #endif  
#endif		//end of CPU_MAP_ATMEGA328P & CPU_MAP_ATMEGA2560

#if defined(CPU_MAP_STM32F10X)
  #ifdef VARIABLE_SPINDLE  
	HW_PWM_Disable(PWM_TIMER); 		//PWM失能
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,1);  //将引脚设置为高
      #else
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,0); //将引脚设置为低
      #endif
  #else
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,1);  //将引脚设置为高
      #else
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,0); //将引脚设置为低
      #endif
  #endif 
#endif		//end of CPU_MAP_STM32F10X
}


void spindle_set_state(uint8_t state, float rpm)
{
	uint16_t current_pwm;
  // Halt or set spindle direction and rpm. 
  if (state == SPINDLE_DISABLE) {

    spindle_stop();

  } else {
#if defined(CPU_MAP_ATMEGA328P) || defined(CPU_MAP_ATMEGA2560)
    #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
      if (state == SPINDLE_ENABLE_CW) {
        SPINDLE_DIRECTION_PORT &= ~(1<<SPINDLE_DIRECTION_BIT);
      } else {
        SPINDLE_DIRECTION_PORT |= (1<<SPINDLE_DIRECTION_BIT);
      }
    #endif

    #ifdef VARIABLE_SPINDLE
      // TODO: Install the optional capability for frequency-based output for servos.
	  //TODO: 安装基于频率的输出的可选功能以兼容舵机。
      #ifdef CPU_MAP_ATMEGA2560
      	TCCRA_REGISTER = (1<<COMB_BIT) | (1<<WAVE1_REGISTER) | (1<<WAVE0_REGISTER);
        TCCRB_REGISTER = (TCCRB_REGISTER & 0b11111000) | 0x02 | (1<<WAVE2_REGISTER) | (1<<WAVE3_REGISTER); // set to 1/8 Prescaler	设置为1/8分频
        OCR4A = 0xFFFF; // set the top 16bit value	
      #else
        TCCRA_REGISTER = (1<<COMB_BIT) | (1<<WAVE1_REGISTER) | (1<<WAVE0_REGISTER);
        TCCRB_REGISTER = (TCCRB_REGISTER & 0b11111000) | 0x02; // set to 1/8 Prescaler	 
      #endif

      if (rpm <= 0.0) { spindle_stop(); } // RPM should never be negative, but check anyway.   
      else {
        #define SPINDLE_RPM_RANGE (SPINDLE_MAX_RPM-SPINDLE_MIN_RPM)
        if ( rpm < SPINDLE_MIN_RPM ) { rpm = 0; } 
        else { 
          rpm -= SPINDLE_MIN_RPM; 
          if ( rpm > SPINDLE_RPM_RANGE ) { rpm = SPINDLE_RPM_RANGE; } // Prevent integer overflow	  
        }
        current_pwm = floor( rpm*(PWM_MAX_VALUE/SPINDLE_RPM_RANGE) + 0.5);
        #ifdef MINIMUM_SPINDLE_PWM
          if (current_pwm < MINIMUM_SPINDLE_PWM) { current_pwm = MINIMUM_SPINDLE_PWM; }
        #endif
        OCR_REGISTER = current_pwm; // Set PWM pin output	 
    
        // On the Uno, spindle enable and PWM are shared, unless otherwise specified.	  
        #if defined(CPU_MAP_ATMEGA2560) || defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) 
          #ifdef INVERT_SPINDLE_ENABLE_PIN
            SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT);
          #else
            SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);
          #endif
        #endif
      }
      
    #else
      // NOTE: Without variable spindle, the enable bit should just turn on or off, regardless
      // if the spindle speed value is zero, as its ignored anyhow. 
    
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT);
      #else
        SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);
      #endif
    #endif
#endif		//end of CPU_MAP_ATMEGA328P & CPU_MAP_ATMEGA2560

#if defined(CPU_MAP_STM32F10X)
    #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
      if (state == SPINDLE_ENABLE_CW) {
        HW_GPIO_Write(SPINDLE_DIRECTION_GPIO,SPINDLE_DIRECTION_GPIO_PIN,0);
      } else {
	  	HW_GPIO_Write(SPINDLE_DIRECTION_GPIO,SPINDLE_DIRECTION_GPIO_PIN,1);
      }
    #endif
 	#ifdef VARIABLE_SPINDLE
      // TODO: Install the optional capability for frequency-based output for servos.
      if (rpm <= 0.0) { spindle_stop(); } // RPM should never be negative, but check anyway.   
      else {
        #define SPINDLE_RPM_RANGE (SPINDLE_MAX_RPM-SPINDLE_MIN_RPM)
        if ( rpm < SPINDLE_MIN_RPM ) { rpm = 0; } 
        else { 
          rpm -= SPINDLE_MIN_RPM; 
          if ( rpm > SPINDLE_RPM_RANGE ) { rpm = SPINDLE_RPM_RANGE; } // Prevent integer overflow	  
        }
        current_pwm = floor( rpm*(PWM_MAX_VALUE/SPINDLE_RPM_RANGE) + 0.5);
        #ifdef MINIMUM_SPINDLE_PWM
          if (current_pwm < MINIMUM_SPINDLE_PWM) { current_pwm = MINIMUM_SPINDLE_PWM; }
        #endif
		HW_PWM_Write(PWM_TIMER,current_pwm); // Set PWM pin output
    	HW_PWM_Enable(PWM_TIMER);			//使能
        // On the Uno, spindle enable and PWM are shared, unless otherwise specified.	  
	  #ifdef INVERT_SPINDLE_ENABLE_PIN
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,0);  
      #else
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,1);
      #endif
      }
      
    #else
      // NOTE: Without variable spindle, the enable bit should just turn on or off, regardless
      // if the spindle speed value is zero, as its ignored anyhow. 
  
	  #ifdef INVERT_SPINDLE_ENABLE_PIN
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,0);  
      #else
        HW_GPIO_Write(SPINDLE_ENABLE_GPIO,SPINDLE_ENABLE_GPIO_PIN,1);
      #endif
    #endif
#endif		//end of CPU_MAP_STM32F10X
  }
}


void spindle_run(uint8_t state, float rpm)
{
  if (sys.state == STATE_CHECK_MODE) { return; }
  protocol_buffer_synchronize(); // Empty planner buffer to ensure spindle is set when programmed.  
  spindle_set_state(state, rpm);
}
