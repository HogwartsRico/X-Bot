#include "Hardware.h"

void HW_EXTI_Init(u8 int_portsource,u8 int_pinsource,u32 int_line,EXTITrigger_TypeDef trig)
{
	EXTI_InitTypeDef EXTI_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

  	//中断线以及中断初始化配置
  	GPIO_EXTILineConfig(int_portsource,int_pinsource);

  	EXTI_InitStructure.EXTI_Line = int_line;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = trig;			
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 					//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


 	
}

void HW_EXTI_Enable(u8 int_channel,u8 preemption_priority,u8 sub_priority)
{
 	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = int_channel;							//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;	//抢占优先级 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;				//子优先级
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);	
}

void HW_EXTI_Disable(u8 int_channel,u8 preemption_priority,u8 sub_priority)
{
 	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = int_channel;							//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;	//抢占优先级 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;				//子优先级
  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;							//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);		
}



