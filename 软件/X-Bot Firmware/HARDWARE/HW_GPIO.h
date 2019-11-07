#ifndef __HW_GPIO_H
#define __HW_GPIO_H

//此处代码将实现GPIO的功能控制，包括输入输出初始化，设置引脚电平和读取引脚电平。

//##############################【API】##############################
void HW_GPIO_Init_Out(u32 gpio_clk,GPIO_TypeDef * gpio,u16 gpio_pin);	
void HW_GPIO_Init_In(u32 gpio_clk,GPIO_TypeDef * gpio,u16 gpio_pin);
void HW_GPIO_Write(GPIO_TypeDef * gpio,u16 gpio_pin,u8 val);
u8   HW_GPIO_Read(GPIO_TypeDef * gpio,u16 gpio_pin);




#endif
