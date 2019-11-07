#ifndef __HW_EEPROM_h
#define __HW_EEPROM_h


/*
需要实现两个底层函数，这里使用STM32自带的flash做EEPROM。（GRBL中使用的内置EEPROM大小为1k）
（容量：1279B，地址必须是偶数）
【需要等到编程结束再确定可以用作EEPROM的开始地址！！！格式u32，最低0x08000000，最高0x0800FFFF】
【先默认用0x0800FF00~0x0800FFFF，作为EPPROM】


【读写时要关闭所有中断!!!!!】这个缺陷不好，可能后期会改为外部EEPROM
*/

//##############################【内部使用】##############################
//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
#define FLASH_KEY1 0X45670123
#define FLASH_KEY2 0XCDEF89AB

//##############################【API】##############################
void HW_EEPROM_Init(void);			//EEPROM初始化

unsigned char HW_EEPROM_getChar(unsigned int addr);				   //EEPROM读取一个u8数据
void HW_EEPROM_putChar(unsigned int addr,unsigned char new_value); //EEPROM写入一个u8数据

#endif
