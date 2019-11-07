#ifndef __PROJECT_INCLUDE_H
#define __PROJECT_INCLUDE_H

#define F_CPU 72000000
#define M_PI  3.14159265358979323846

//#####################【芯片级支持库】##############################
#include "stm32f10x.h"

//#####################【工程全局支持库】##############################
#include "delay.h"		//延时
#include "sys.h"		//快捷使用
#include "bit.h"		//快速位操作
#include "strprintf.h"	//格式化字符串核
#include "simui2c.h"	//模拟I2C协议
#include "simuspi.h"	//模拟SPI协议	

//#####################【硬件】##############################
#include "Hardware.h"	//GRBL功能的底层实现


#endif
