#include "Hardware.h"

//此处代码将实现GPIO的功能控制，包括初始化，设置引脚电平和读取引脚电平。
void HW_GPIO_Init_Out(u32 gpio_clk,GPIO_TypeDef * gpio,u16 gpio_pin)			
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(gpio_clk,ENABLE);	 			//使能端口时钟
	GPIO_InitStructure.GPIO_Pin = gpio_pin;						//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
	GPIO_Init(gpio, &GPIO_InitStructure);					//根据设定参数初始化
}

void HW_GPIO_Init_In(u32 gpio_clk,GPIO_TypeDef * gpio,u16 gpio_pin)			
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(gpio_clk,ENABLE);	 			//使能端口时钟
	GPIO_InitStructure.GPIO_Pin = gpio_pin;						//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 		//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
	GPIO_Init(gpio, &GPIO_InitStructure);					//根据设定参数初始化
}

void HW_GPIO_Write(GPIO_TypeDef * gpio,u16 gpio_pin,u8 val)
{
	if(val==0) GPIO_ResetBits(gpio,gpio_pin);				//引脚输出低电平
	else GPIO_SetBits(gpio,gpio_pin);						//引脚输出高电平
}

u8 HW_GPIO_Read(GPIO_TypeDef * gpio,u16 gpio_pin)
{
	return GPIO_ReadInputDataBit(gpio,gpio_pin);			//返回引脚电平
}



