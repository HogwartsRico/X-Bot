#ifndef __HARDWARE_H
#define __HARDWARE_H


#include "stm32f10x.h"

//#####################【标准库】#####################
#include <string.h>	

//#####################【底层】#####################
#include "delay.h"		//延时
#include "HW_GPIO.h"	//GPIO
#include "HW_USART.h"	//串口
#include "HW_TIM.h"		//定时器
#include "HW_EXTI.h"	//外部中断
#include "HW_PWM.h"		//PWM
#include "HW_EEPROM.h"	//STM32的Flash作EEPROM


//#####################【参数】#####################
#define STM32_FLASH_WREN 		1              	//使能FLASH写入(0，不使能; 1，使能)
#define STM32_FLASH_SIZE 		64 	 			//所选STM32的FLASH容量大小(单位为K)

#define STM32_EEPROM_ADDR_START	0x0800FC00		//作为EEPROM的起始地址（地址必须是偶数）
#define STM32_EEPROM_ADDR_MAX	0x400			//EEPROM容量(单位B)

//#####################【宏】#####################
#define cli()      __set_PRIMASK(1)   			//总中断关闭
#define sei()      __set_PRIMASK(0) 			//总中断开启


#endif
